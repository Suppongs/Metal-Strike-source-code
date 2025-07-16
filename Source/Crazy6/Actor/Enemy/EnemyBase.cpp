// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnemyDataManager.h"
#include "EnemyMovement.h"
#include "EnemyStatComponent.h"
#include "Controller/EnemyController.h"
#include "Components/WidgetComponent.h"
#include "Crazy6/Widget/MonsterHPWidget.h"
#include "Crazy6/Widget/MonsterHUDWidget.h"
#include "../../../Crazy6/Widget/DamageWidgetData.h"
#include "Crazy6/Player/PlayerBase.h"
#include "Crazy6/Global/ProfileInfo.h"
#include "Crazy6/Actor/TeamEnum.h"
#include "Net/UnrealNetwork.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Crazy6/GameMode/MainGameMode.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;	
	
	mCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	mCapsule->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMY);
	
	mCapsule->SetCanEverAffectNavigation(false);

	mCapsule->CanCharacterStepUpOn = ECB_No;
	
	mCapsule->BodyInstance.bLockRotation = true;
	mCapsule->BodyInstance.bLockXRotation = true;
	mCapsule->BodyInstance.bLockYRotation = true;
	mCapsule->BodyInstance.bLockZRotation = false;
	SetRootComponent(mCapsule);

	// <======= Mesh Setting =======>
	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	mMesh->SetupAttachment(mCapsule);
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mMesh->bReceivesDecals = false;

	// <======= AI Setting =======>
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyController::StaticClass();

	// <======== Movement Setting ==========>
	mMovement = CreateDefaultSubobject<UEnemyMovement>(TEXT("MovementComponent"));
	mMovement->SetUpdatedComponent(RootComponent);

	// <======== Stat Setting ==========>
	mStat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("AIStatComponent"));

	/*Monster HP HUD Setting*/
	mMonsterHUDWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterHUDWidget"));

	{
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/MonsterHUDWidget.MonsterHUDWidget_C'"));
		if (WidgetClass.Succeeded())
		{
			mMonsterHUDWidgetComponent->SetWidgetClass(WidgetClass.Class);
		}

		mMonsterHUDWidgetComponent->SetupAttachment(mCapsule);
		mMonsterHUDWidgetComponent->SetDrawSize(FVector2D(173.0f, 36.0f));
		mMonsterHUDWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		mMonsterHUDWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
		mMonsterHUDWidgetComponent->SetVisibility(false);

	}

	// <======== Minimap ==========>
#pragma region mMinimapMonsterIcon
	mMinimapMonsterIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapPlayerIcon"));
	mMinimapMonsterIcon->SetupAttachment(GetMesh());

	mMinimapMonsterIcon->SetWorldRotation(FRotator::MakeFromEuler(FVector(270.f, 0.f, 90.f)));
	mMinimapMonsterIcon->SetWorldScale3D(FVector(4.5f));
	mMinimapMonsterIcon->SetWorldLocation(FVector(0.f, 0.f, 350.0f));

	mMinimapMonsterIcon->bVisibleInSceneCaptureOnly = true;

	ConstructorHelpers::FObjectFinder<UPaperSprite> FOBJ_PaperSprite(TEXT("/Script/Paper2D.PaperSprite'/Game/Dev/UI/MiniMap/SP_EnemyIcon.SP_EnemyIcon'"));
	if (FOBJ_PaperSprite.Succeeded())
	{
		mMinimapMonsterIcon->SetSprite(FOBJ_PaperSprite.Object);
	}
#pragma endregion


	
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	bReplicates = true;
	mCapsule->SetIsReplicated(true);
	mMesh->SetIsReplicated(true);

	const FEnemyInfoTable* pEnemyInfo = CEnemyDataManager::GetInst()->FindInfo(mEnemyKey);
	check(pEnemyInfo);
	mStat->SetStat(pEnemyInfo);

	mStat->OnChangedHpDelegate.AddUObject(this, &ThisClass::CallBack_Hit);
	mStat->OnChangedHpDelegate.AddUObject(this, &ThisClass::Callback_SetUIHp);
	mStat->OnHitDamageDelegate.AddUObject(this, &ThisClass::Callback_OutputDamageIndicator);
	mStat->OnHpZERODelegate.AddUObject(this, &ThisClass::CallBack_HpZERO);

	// HUD view
	if (nullptr != mMonsterHUDWidgetComponent
		&& mMonsterHUDWidgetComponent->GetWidget())
	{
		MonsterHPInstance = Cast<UMonsterHUDWidget>(mMonsterHUDWidgetComponent->GetWidget());
	}



	// 플레이어 컨트롤러 가져오기
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		return;  // 컨트롤러가 없으면 종료
	}

	// 컨트롤러가 조종 중인 Pawn 가져오기
	ACharacter* ControlledPlayer = PlayerController->GetCharacter();
	if (!ControlledPlayer)
	{
		return;  // 컨트롤러가 조종하는 Pawn이 없으면 종료
	}

	// 플레이어 캐릭터로 캐스팅
	APlayerBase* PlayerCharacter = Cast<APlayerBase>(ControlledPlayer);
	if (PlayerCharacter)
	{
		// PlayerCharacter를 통해 원하는 작업 수행
		PlayerCharacter->GetMinimapCaptureComp()->HideComponent(mMesh);
	}
	// Monster Log
	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		uint64 CurrentMonster = GameMode->PlusMonster();
		FString Message = FString::Printf(TEXT("Current Monster Count: %llu"), CurrentMonster);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message);
	}


}

void AEnemyBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Monster Log
	AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		uint64 CurrentMonster = GameMode->MinusMonster();
		FString Message = FString::Printf(TEXT("Current Monster Count: %llu"), CurrentMonster);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message);
	}
	Super::EndPlay(EndPlayReason);

	/*if (true == mDeathDelegate.IsBound())
	{
		mDeathDelegate.Broadcast();
	}*/
}

void AEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AEnemyController* pAIController = Cast<AEnemyController>(NewController);
	if (true == IsValid(pAIController))
	{
		pAIController->SetTeamID(ETeamID::ENEMY);
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Z 값 비교
	if (GetActorLocation().Z < -10000.f)
	{
		// 화면에 출력
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.f,
			FColor::Emerald,
			FString::Printf(TEXT("Z value: CurrentZ=%f"),GetActorLocation().Z));
			Destroy();
	}
}

void AEnemyBase::SetAIState(EAIState State)
{
	mState = State;
	mStat->ChangeStatInfoByState(State);
}

float AEnemyBase::GetAttack()
{
	return mStat->GetAttack();
}

float AEnemyBase::GetAttackRange()
{
	const float fAtttackRange = mStat->GetAttackRange();
	const float HalfCapsuleRadius = mCapsule->GetScaledCapsuleRadius();
	const float fResult = fAtttackRange + HalfCapsuleRadius;
	return fResult;
}

void AEnemyBase::Attack()
{
	mCapsule->SetCanEverAffectNavigation(true);
}

void AEnemyBase::SetAttackEndDelegate(const FAIAttackEnd& InOnAttackFinished)
{
	OnAttackFininsed = InOnAttackFinished;
}

void AEnemyBase::AttackHitBoxOn()
{
}

void AEnemyBase::AttackHitBoxOff()
{
}

void AEnemyBase::AttackEnd()
{
	mCapsule->SetCanEverAffectNavigation(false);
	OnAttackFininsed.ExecuteIfBound();
}

float AEnemyBase::TakeDamage(float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent,
	                                 EventInstigator, DamageCauser);
	mStat->ApplyDamage(DamageAmount);

	AEnemyController* AIController = Cast<AEnemyController>(GetController());
	if (true == IsValid(AIController))
	{
		if (false == AIController->GetTargetCollection().Contains(DamageCauser))
		{
			AIController->PushTargetCollection(DamageCauser);
		}
	}

	return DamageAmount;
}

void AEnemyBase::CallBack_HpZERO()
{
	mMovement->Deactivate();
	
	ensure(mCapsule);
	mCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyBase::StartRagdoll()
{
	//AddActorWorldOffset(FVector::UpVector * 3.0f);
	
	mCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//mCapsule->SetCollisionProfileName(UE_COLLISION_PROFILE_RAGDOLL);
	
	mMesh->SetCollisionProfileName(UE_COLLISION_PROFILE_RAGDOLL);
	mMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	mMesh->SetSimulatePhysics(true);
	
	if (true == HasAuthority())
	{
		AItemBase* DroppedItem = NewObject<AItemBase>(this);
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = FRotator::ZeroRotator;
		/* Drop Item */
		DroppedItem->SpawnItem(SpawnLocation, SpawnRotation);
	}

	// ===== Relation Mission Manager
	// by Call AKillMIssionObjective::CallBack_KillMissionReflect Func
	if (true == mDeathDelegate.IsBound())
	{
		mDeathDelegate.Broadcast();
	}
}

void AEnemyBase::SetDeathTimer(float DelayTime)
{
	FTimerHandle DeathTimerHandle;

	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemyBase::Death, DelayTime, false);
}

void AEnemyBase::Death()
{
	Destroy();
}

void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyBase, mState);
	DOREPLIFETIME(AEnemyBase, mMesh);
	DOREPLIFETIME(AEnemyBase, mCapsule);
	DOREPLIFETIME(AEnemyBase, mServerAnimationType);
	DOREPLIFETIME(AEnemyBase, mServerIsHit);
}

void AEnemyBase::OnRep_mServerAnimationType()
{
	if (true == IsValid(mAnimInstance))
	{
		mAnimInstance->ChangeAnimation(mServerAnimationType);
	}
}

void AEnemyBase::OnRep_mServerIsHit()
{
	if (true == IsValid(mAnimInstance))
	{
		mAnimInstance->SetIsHit(mServerIsHit);
	}
}

void AEnemyBase::CheckHPFunctionTime()
{
	GetWorld()->GetTimerManager().ClearTimer(CheckHPTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(CheckHPTimerHandle, this, &AEnemyBase::HPTriggerVisibleFalse, CheckHPTime, false);
}

void AEnemyBase::HPTriggerVisibleFalse()
{
	if(IsValid(mMonsterHUDWidgetComponent))
	{
		mMonsterHUDWidgetComponent->SetVisibility(false);
	}
	bDamageUICheckKey = false;
}

void AEnemyBase::CheckDamageFunctionTime()
{
	GetWorld()->GetTimerManager().ClearTimer(CheckDamageTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(CheckDamageTimerHandle, this, &AEnemyBase::DamageTriggerDeleteFunction, CheckDamageTime, false);
}

void AEnemyBase::DamageTriggerDeleteFunction()
{
	if (ActiveDamageWidgets.Num() > 0)
	{
		UWidgetComponent* FinishedWidget = ActiveDamageWidgets[0];
		if (FinishedWidget)
		{
			FinishedWidget->UnregisterComponent();
			FinishedWidget->DestroyComponent();
		}
		ActiveDamageWidgets.RemoveAt(0);
	}
}

void AEnemyBase::Callback_SetUIHp_Implementation(float CurHp)
{
	if (true == bDamageUICheckKey)
	{
		CheckHPFunctionTime();
		const float fMaxHp = mStat->GetMaxHP();
		const float fPercent = CurHp / fMaxHp;
		if (true == IsValid(MonsterHPInstance))
		{
			MonsterHPInstance->SetHPPercent(fPercent);
		}
		if (IsValid(mMonsterHUDWidgetComponent))
		{
			if(false == mMonsterHUDWidgetComponent->IsVisible())
			{
				mMonsterHUDWidgetComponent->SetVisibility(true);
			}
		}
		if (0 == CurHp)
		{
			HPTriggerVisibleFalse();
		}
	}
}

void AEnemyBase::Callback_OutputDamageIndicator_Implementation(float Damage)
{
	if (true == bDamageUICheckKey)
	{
		if (0.0f == Damage)
		{
			return;
		}
		/*Monster Damage Setting*/
		UWidgetComponent* NewDamageWidgetComponent = NewObject<UWidgetComponent>(this);
		if (NewDamageWidgetComponent)
		{
			const UDamageWidgetData* InputData = GetDefault<UDamageWidgetData>();
			//make Widget
			UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), InputData->mWidgetComponent);
			NewDamageWidgetComponent->SetWidget(WidgetInstance);
		}
		// actor attach
		NewDamageWidgetComponent->SetupAttachment(mCapsule);
		NewDamageWidgetComponent->SetDrawSize(FVector2D(192.0f, 108.0f));
		NewDamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		NewDamageWidgetComponent->SetRelativeLocation(FVector(0.0, FMath::RandRange(50, 100), FMath::RandRange(100,200)));
		NewDamageWidgetComponent->RegisterComponent();

		UMonsterHPWidget* DamageInstance = Cast<UMonsterHPWidget>(NewDamageWidgetComponent->GetWidget());
		if (DamageInstance)
		{
			if (false == GetIsHeadShot())
			{  
				DamageInstance->SetText(FText::AsNumber(Damage));
				Animation = DamageInstance->mTextUp;
				if (Animation)
				{
					DamageInstance->PlayAnimation(Animation);
					AnimationEndEvent.BindDynamic(this, &AEnemyBase::DamageTriggerDeleteFunction);
					DamageInstance->BindToAnimationFinished(Animation, AnimationEndEvent);
					NewDamageWidgetComponent->SetVisibility(true);
					ActiveDamageWidgets.Add(NewDamageWidgetComponent);

				}
			}
			if (true == GetIsHeadShot())
			{
				DamageInstance->SetText(FText::AsNumber(Damage));
				HeadshotAnimation = DamageInstance->mTextUPRED;
				if (HeadshotAnimation)
				{
					NewDamageWidgetComponent->SetDrawSize(FVector2D(307.2f, 172.8f));
					DamageInstance->PlayAnimation(HeadshotAnimation);
					AnimationEndEvent.BindDynamic(this, &AEnemyBase::DamageTriggerDeleteFunction);
					DamageInstance->BindToAnimationFinished(HeadshotAnimation, AnimationEndEvent);
					NewDamageWidgetComponent->SetVisibility(true);
					ActiveDamageWidgets.Add(NewDamageWidgetComponent);
				}
			}
		}
	}
}


