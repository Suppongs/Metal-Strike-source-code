// Fill out your copyright notice in the Description page of Project Settings.


#include "BossZero.h"
#include "Beam.h"
#include "Bomb.h"
#include "BossAISpawner.h"
#include "BossAttackCameraShake.h"
#include "BrainComponent.h"
#include "EnergyBall.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "../EnemyAnimTemplate.h"
#include "Crazy6/Actor/TeamEnum.h"
#include "Crazy6/Actor/Enemy/Controller/BossController.h"
#include "Crazy6/Global/ProfileInfo.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollection.h"
#include "Crazy6/Widget/MainBossHPbar.h"
#include "BossGeometryCollectionActor.h"
#include "Components/WidgetComponent.h"
#include "Crazy6/Actor/Enemy/EnemyMovement.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"
#include "Net/UnrealNetwork.h"

ABossZero::ABossZero()
{
	bUseFabrik = false;
	bReplicates = true;

	/* <=========== Mesh Setting ===========> */
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Dev/AI/Enemy/Boss/Mesh/SM_Sphere_Bot.SM_Sphere_Bot'"));
	if (true == MeshAsset.Succeeded())
	{
		mMesh->SetSkeletalMeshAsset(MeshAsset.Object);
		mMesh->SetRelativeLocation(FVector(0.0, 0.0, -300.0));
		mMesh->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
		mMesh->SetRelativeScale3D(FVector(3.0, 3.0, 3.0));
		mCapsule->SetCapsuleHalfHeight(300.f);
		mCapsule->SetCapsuleRadius(300.f);
		mCapsule->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMY);
		mCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		static ConstructorHelpers::FClassFinder<UAnimInstance>
			AnimObject(TEXT("/Script/Engine.AnimBlueprint'/Game/Dev/AI/Enemy/Boss/ABP_Zero.ABP_Zero_C'"));
		if (true == AnimObject.Succeeded())
		{
			mMesh->SetAnimInstanceClass(AnimObject.Class);
		}
	}

	/* <=========== Key Setting ===========> */
	mEnemyKey = TEXT("BossZero");

	// <======= AI Setting =======>
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABossController::StaticClass();

	/* <=========== Boss HitBox Setting ===========> */
	mBossHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("BossHitBox"));
	mBossHitBox->SetupAttachment(mMesh, TEXT("BossHitBox"));
	mBossHitBox->SetRelativeScale3D(FVector(0.01, 0.01, 0.01));
	mBossHitBox->SetSphereRadius(35.f);
	mBossHitBox->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMY);
	mBossHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mBossHitBox->SetCanEverAffectNavigation(false);

	for (int i = 1; i <= 12; ++i)
	{
		FString ArmorSocketName = FString::Printf(TEXT("Leg%d_Armor"), i);
		mArmorHitBox = CreateDefaultSubobject<UBoxComponent>(*ArmorSocketName);
		mArmorHitBox->SetupAttachment(mMesh, *ArmorSocketName);
		mArmorHitBox->SetRelativeScale3D(FVector(0.01, 0.01, 0.01));
		mArmorHitBox->SetBoxExtent(FVector(40.0, 60.0, 10.0));
		mArmorHitBox->SetRelativeLocation(FVector(0.0, -0.665, -0.053));
		mArmorHitBox->SetCollisionProfileName(UE_COLLISION_PROFILE_BOSSARMOR);
		mArmorHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		mArmorHitBox->SetCanEverAffectNavigation(false);
	}

	/* <=========== RollingAttack HitBox Setting ===========> */
	mRollingHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("RollingHitBox"));
	mRollingHitBox->SetupAttachment(mCapsule);
	mRollingHitBox->SetSphereRadius(300.f);
	mRollingHitBox->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMYATTACK);
	mRollingHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mRollingHitBox->SetCanEverAffectNavigation(false);

	/* <=========== SlamAttack HitBox Setting ===========> */
	mSlamHitBox = CreateDefaultSubobject<USceneComponent>(TEXT("SlamHitBox"));
	mSlamHitBox->SetupAttachment(mCapsule);
	mSlamHitBox->SetRelativeLocation(FVector(0.0, 0.0, -270.0));

	/* <=========== Attack OverLap Setting ===========> */
	mRollingHitBox->OnComponentBeginOverlap.AddDynamic(this, &ABossZero::OnBegineOverlap_Rolling);

	// <======= Attack Setting =======>
	mEnergyBallClass = AEnergyBall::StaticClass();

	mBeam = CreateDefaultSubobject<UBeam>(TEXT("Beam"));
	mBeam->SetupAttachment(mMesh, TEXT("Muzzle"));

	mBombClass = ABomb::StaticClass();

	mAISpawner = CreateDefaultSubobject<UBossAISpawner>(TEXT("AISpawner"));
	mAISpawner->SetupAttachment(mCapsule);
	mAISpawner->SetRelativeLocation(FVector(0.0, 0.0, -mCapsule->GetScaledCapsuleHalfHeight()));

	// <========= Chaos Setting ========>
	mGCActorClass = ABossGeometryCollectionActor::StaticClass();

	// <======= FX Setting =======>
	mCameraShakeClass = UBossAttackCameraShake::StaticClass();

	mPhase2Visual = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Phase2Visual"));
	mPhase2Visual->SetupAttachment(mMesh, TEXT("Muzzle"));
	mPhase2Visual->SetRelativeScale3D(FVector(0.015, 0.015, 0.015));
	mPhase2Visual->SetRelativeRotation(FRotator(-90.0, 0.0, 0.0));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		Phase2Visual(TEXT(
			"/Script/Niagara.NiagaraSystem'/Game/Dev/AI/Enemy/Boss/PhaseFX/Phase2/LightningField/N_LightningField.N_LightningField'"));
	if (true == Phase2Visual.Succeeded())
	{
		mPhase2Visual->SetAsset(Phase2Visual.Object);
	}
	mPhase2Visual->SetAutoActivate(false);

	mPhase3Visual = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Phase3Visual"));
	mPhase3Visual->SetupAttachment(mMesh, TEXT("Muzzle"));
	mPhase3Visual->SetRelativeScale3D(FVector(0.005, 0.005, 0.005));
	mPhase3Visual->SetRelativeLocation(FVector(-1.0, 0.0, 0.0));
	mPhase3Visual->SetRelativeRotation(FRotator(-90.0, 0.0, 0.0));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		Phase3Visual(
			TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/AI/Enemy/Boss/PhaseFX/Phase3/NS_Burnning.NS_Burnning'"));
	if (true == Phase3Visual.Succeeded())
	{
		mPhase3Visual->SetAsset(Phase3Visual.Object);
	}
	mPhase3Visual->SetAutoActivate(false);

	static ConstructorHelpers::FObjectFinder<USoundBase>
		ChargeSoundBase(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Boss/Sound/Laser_Charge.Laser_Charge'"));
	if (ChargeSoundBase.Succeeded())
	{
		mLaserChargeSound = ChargeSoundBase.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		RollingHitParticle(TEXT(
			"/Script/Engine.ParticleSystem'/Game/Dev/AI/Enemy/Humanoid/FX/P_Minion_Impact_Default.P_Minion_Impact_Default'"));
	if (RollingHitParticle.Succeeded())
	{
		mRollingHitParticle = RollingHitParticle.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>
		RollingSoundBase(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Boss/Sound/Rolling.Rolling'"));
	if (RollingSoundBase.Succeeded())
	{
		mRollingSound = RollingSoundBase.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>
		RollingHitSoundBase(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Boss/Sound/Rolling_Hit.Rolling_Hit'"));
	if (RollingHitSoundBase.Succeeded())
	{
		mRollingHitSound = RollingHitSoundBase.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		ExplosionNiagaraSystem(TEXT(
			"/Script/Niagara.NiagaraSystem'/Game/Dev/AI/Enemy/Boss/SlamFX/VFX/EarthExplosion/N_EarthExplosion.N_EarthExplosion'"));
	if (true == ExplosionNiagaraSystem.Succeeded())
	{
		mSlamFX = ExplosionNiagaraSystem.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>
		SlamSoundBase(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Boss/Sound/Slam.Slam'"));
	if (SlamSoundBase.Succeeded())
	{
		mSlamSound = SlamSoundBase.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		SlamHitParticle(TEXT(
			"/Script/Engine.ParticleSystem'/Game/Dev/AI/Enemy/Humanoid/FX/P_Minion_Impact_Default.P_Minion_Impact_Default'"));
	if (SlamHitParticle.Succeeded())
	{
		mSlamHitParticle = SlamHitParticle.Object;
	}

	// ===== Boss HP Setting  =======

	static ConstructorHelpers::FClassFinder<UUserWidget>
		BossHPAsset(TEXT(
			"/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/MainBossProgressBar.MainBossProgressBar_C'"));
	if (BossHPAsset.Succeeded())
	{
		mBossHPWidgetclass = BossHPAsset.Class;
	}

	// ===== Mission clear Widget Setting =====
	static ConstructorHelpers::FClassFinder<UUserWidget>
		MissionClearAsset(TEXT(
			"/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/MissionClearWidget.MissionClearWidget_C'"));
	if (MissionClearAsset.Succeeded())
	{
		mMissionClearWidgetclass = MissionClearAsset.Class;
	}
}

void ABossZero::BeginPlay()
{
	Super::BeginPlay();

	mAnimInstance = Cast<UEnemyAnimTemplate>(mMesh->GetAnimInstance());
	if (true == IsValid(mAnimInstance))
	{
		mAnimInstance->SetAnimData(mEnemyKey);
	}

	// ========= Boss HP =============
	if (IsValid(mBossHPWidgetclass))
	{
		mBossHPWidget = CreateWidget<UMainBossHPbar>(GetWorld(), mBossHPWidgetclass);

		if (mBossHPWidget)
		{
			// delete Enemybase Widget
			mMonsterHUDWidgetComponent->DestroyComponent();
			MonsterHPInstance = nullptr;

			mBossHPWidget->AddToViewport();
		}
	}
	// ========= Mission Widget Create =============
	if (IsValid(mMissionClearWidgetclass))
	{
		mMissionClearWidget = CreateWidget<UUserWidget>(GetWorld(), mMissionClearWidgetclass);
		if (mMissionClearWidget)
		{
			mMissionClearWidget->AddToViewport();
			mMissionClearWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(mAISpawnTimer, [&]()
		{
			mAISpawner->SpawnRandomAI();
		}, mAISpawnTime, true);

		// Initialization
		CurrentHP = mStat->GetMaxHP();
	}
}

void ABossZero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ABossController* pAIController = Cast<ABossController>(NewController);
	if (true == IsValid(pAIController))
	{
		pAIController->SetTeamID(ETeamID::ENEMY);
	}
}

void ABossZero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Change Phase
	if (mStat->GetHPPercentage() < 80.f
		&& mCurrentPhase == EBossPhase::Phase1)
	{
		mCurrentPhase = EBossPhase::Phase2;
		OnRep_mCurrentPhase();
	}
	else if (mStat->GetHPPercentage() < 50.f
		&& mCurrentPhase == EBossPhase::Phase2)
	{
		mCurrentPhase = EBossPhase::Phase3;
		OnRep_mCurrentPhase();
	}

	switch (mAttackMove)
	{
	case EBossAttackMove::RollingMove:
		{
			FVector NextLocation = GetActorLocation() + (GetActorForwardVector() * 30);
			NextLocation.Z -= mCapsule->GetScaledCapsuleHalfHeight();
			FNavLocation NavLocation;
			UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			if (true == IsValid(NavSystem) &&
				true == NavSystem->ProjectPointToNavigation(NextLocation, NavLocation))
			{
				NavLocation.Location.Z += mCapsule->GetScaledCapsuleHalfHeight();
				mCapsule->SetRelativeLocation(NavLocation.Location);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("**Boss : out of navmesh**"));
			}
		}
		break;
	case EBossAttackMove::SlamMove:
		{
			FNavLocation NavLocation;
			UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			if (true == IsValid(NavSystem) &&
				true == NavSystem->ProjectPointToNavigation(mSlamDestination, NavLocation))
			{
				NavLocation.Location.Z += mCapsule->GetScaledCapsuleHalfHeight();
				FVector CurrentLocation = GetActorLocation();
				FVector NextLocation = FMath::VInterpTo(CurrentLocation, NavLocation.Location, DeltaTime, 3.f);
				mCapsule->SetRelativeLocation(NextLocation);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("**Boss : out of navmesh**"));
			}
		}
		break;
	case EBossAttackMove::AdvancedLaserMove:
		{
			FVector TargetDirection = mTarget->GetActorLocation() - GetActorLocation();
			TargetDirection.Z = 0.0;
			FRotator TargetRotation = TargetDirection.Rotation();
			FRotator CurrentRotation = GetActorRotation();
			FRotator NextRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, 500.f);
			SetActorRotation(NextRotation);
		}
		break;
	default:
		break;
	}
}

void ABossZero::SetAIState(EAIState State)
{
	Super::SetAIState(State);

	mServerAnimationType = mState;

	if (true == IsValid(mAnimInstance))
	{
		mAnimInstance->ChangeAnimation(mServerAnimationType);
	}
}

void ABossZero::Attack()
{
	Super::Attack();

	SwitchAttackModeRandom();
	OnRep_mServerAttackMode();

	//Play by AttackMode
	MulticastPlayAttackMontage();
}

float ABossZero::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//Accumulate damage & Initialize Start damaged time
	ABossController* BossController = Cast<ABossController>(GetController());
	if (true == IsValid(BossController))
	{
		TArray<FPlayerDPS>& TargetCollectionRef = BossController->GetTargetCollection();
		FPlayerDPS* DamageCauserInfo = TargetCollectionRef.FindByPredicate([DamageCauser](const FPlayerDPS& PlayerInfo)
		{
			return PlayerInfo.mPlayerActor == DamageCauser;
		});
		if (true == IsValid(DamageCauserInfo->mPlayerActor))
		{
			if (DamageCauserInfo->mStartDamagedTime <= 0.0f)
			{
				DamageCauserInfo->mStartDamagedTime = GetWorld()->GetTimeSeconds();
			}

			DamageCauserInfo->mLastDamagedTime = GetWorld()->GetTimeSeconds();
			DamageCauserInfo->mAccumulatedDamage += DamageAmount;
		}
	}

	return DamageAmount;
}

void ABossZero::CallBack_Hit(float Damage)
{
	Super::CallBack_Hit(Damage);
}

void ABossZero::CallBack_HpZERO()
{
	Super::CallBack_HpZERO();

	if (mServerAnimationType == EAIState::Death)
		return;

	MulticastBossDeath();

	SetAIState(EAIState::Death);

	GetWorld()->GetTimerManager().ClearTimer(mAISpawnTimer);

	ABossController* AIControl = GetController<ABossController>();
	if (true == IsValid(AIControl))
	{
		AIControl->GetBrainComponent()->StopLogic(TEXT("Death"));
	}

	if (HasAuthority())
	{
		TimeHandleSpawnGC(5.f);
	}
	GetWorld()->GetTimerManager().SetTimer(mRestartTimer, this, &ABossZero::GoMain, 10.0f, false);
	GEngine->AddOnScreenDebugMessage(
		-1, // Ű �� (-1�̸� �� �޽����� �߰�)
		5.0f, // �޽����� ȭ�鿡 ǥ�õ� �ð� (�� ����)
		FColor::Red, // �޽����� ����
		TEXT("AnimNotify!!!!!!!!!!!!!!!!!!!!") // ǥ���� �޽���
	);
}

void ABossZero::Shooting()
{
	FVector SpawnLocation = mMesh->GetSocketLocation("Muzzle");
	FRotator SpawnRotation = mMesh->GetSocketRotation("Muzzle");
	AEnergyBall* EnergyBall = GetWorld()->SpawnActor<AEnergyBall>(mEnergyBallClass, SpawnLocation, SpawnRotation);
	if (true == IsValid(EnergyBall))
	{
		EnergyBall->SetEnergyBallOwner(this);
	}
}

void ABossZero::LaunchEnergyBall(const AEnergyBall* EnergyBall)
{
	ABossController* BossController = GetController<ABossController>();
	if (true == IsValid(BossController))
	{
		mTarget = BossController->GetTarget();
		FVector TargetLocation = mTarget->GetActorLocation();

		FVector EnergyBallLocation = EnergyBall->GetActorLocation();

		FVector mLaunchDirection = TargetLocation - EnergyBallLocation;

		EnergyBall->mMovement->SetVelocityInLocalSpace(mLaunchDirection.GetSafeNormal() * mEnergyBallLaunchSpeed);
	}
}

void ABossZero::Laser()
{
	MulticastLaser();
}

void ABossZero::Rolling()
{
	if (mAttackMove == EBossAttackMove::RollingMove) //When RollingMove end
	{
		mRollingHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		mAttackMove = EBossAttackMove::Nothing;
	}
	else //When RollingMove start
	{
		mRollingHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		ABossController* BossController = GetController<ABossController>();
		if (true == IsValid(BossController))
		{
			mTarget = BossController->GetTarget();
		}

		FVector MovingDirection = mTarget->GetActorLocation() - GetActorLocation();
		MovingDirection.Z = 0.0;
		FRotator TargetRotation = MovingDirection.Rotation();
		SetActorRotation(TargetRotation);

		mAttackMove = EBossAttackMove::RollingMove;

		MulticastRollingSound();
	}
}

void ABossZero::Slam()
{
	if (mAttackMove == EBossAttackMove::SlamMove) //When SlamMove end
	{
		TArray<FHitResult> mSlamHitResults;
		FVector Center = mSlamHitBox->GetComponentLocation();
		FVector SlamHitBoxExtent = FVector(470.0, 470.0, 60.0);
		bool IsHit = GetWorld()->SweepMultiByChannel(
			mSlamHitResults, Center, Center, FQuat::Identity, ECC_GameTraceChannel4, FCollisionShape::MakeBox(SlamHitBoxExtent));
		if (true == IsHit)
		{
			for (const FHitResult& HitResult : mSlamHitResults)
			{
				ACharacter* Player = Cast<ACharacter>(HitResult.GetActor());
				if (true == IsValid(Player))
				{
					FDamageEvent DmgEvent;
					Player->TakeDamage(GetAttack(), DmgEvent, GetController(), this);

					Player->LaunchCharacter((mCapsule->GetForwardVector() + mCapsule->GetUpVector()) * 900.0, true, true);

					MulticastSlamHitFX(Player);
				}
			}	
		}

		mAttackMove = EBossAttackMove::Nothing;

		MulticastCameraShake();
		MulticastSlamLandingFX();
	}
	else //When SlamMove start
	{
		ABossController* BossController = GetController<ABossController>();
		if (true == IsValid(BossController))
		{
			mTarget = BossController->GetTarget();

			FHitResult HitResult;
			FVector Start = mTarget->GetActorLocation();
			FVector End = Start - FVector(0.0f, 0.0f, 1000.0f);
			bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel7);
			if (true == Hit)
			{
				mSlamDestination = mTarget->GetActorLocation();
				mSlamDestination.Z = HitResult.ImpactPoint.Z;
			}
		}

		mAttackMove = EBossAttackMove::SlamMove;
	}
}

void ABossZero::Bomb()
{
	if (false == GetWorld()->GetTimerManager().IsTimerActive(mBombTimer)) //When Bomb start
	{
		GetWorld()->GetTimerManager().SetTimer(mBombTimer, this, &ABossZero::SpawnBomb, mBombTime, true);
	}
	else //When Bomb end
	{
		GetWorld()->GetTimerManager().ClearTimer(mBombTimer);
	}
}

void ABossZero::SpawnBomb()
{
	ABossController* BossController = GetController<ABossController>();
	if (true == IsValid(BossController))
	{
		mTarget = BossController->GetTarget();

		FHitResult HitResult;
		FVector Start = mTarget->GetActorLocation();
		FVector End = Start - FVector(0.0f, 0.0f, 1000.0f);
		bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel7);
		if (true == Hit)
		{
			FVector BombLocation = mTarget->GetActorLocation();
			BombLocation.Z = HitResult.Location.Z;
			ABomb* Bomb = GetWorld()->SpawnActor<ABomb>(mBombClass, BombLocation, FRotator::ZeroRotator);
			Bomb->SetBombOwner(this);
		}
	}
}

void ABossZero::AdvancedLaser()
{
	MulticastLaser();

	ABossController* BossController = GetController<ABossController>();
	mTarget = BossController->GetTarget();

	GetWorld()->GetTimerManager().SetTimer(mAdvancedLaserMoveTimer, [this]()
	{
		mAttackMove = EBossAttackMove::Nothing;
	}, mAdvancedLaserMovingTime, false);

	mAttackMove = EBossAttackMove::AdvancedLaserMove;
}

void ABossZero::MulticastResumeAttackMontage_Implementation()
{
	//if reference is nullptr, it will resume all active montage
	mAnimInstance->Montage_Resume(nullptr);
}

void ABossZero::MulticastCameraShake_Implementation()
{
	for (FConstPlayerControllerIterator Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		APlayerController* PlayerController = Iter->Get();
		if (true == IsValid(PlayerController) &&
			true == PlayerController->IsLocalController())
		{
			PlayerController->ClientStartCameraShake(mCameraShakeClass);
		}
	}
}

void ABossZero::SwitchAttackModeRandom()
{
	int8 RandomIndex = 0;

	switch (mCurrentPhase)
	{
	case EBossPhase::Phase1:
		{
			RandomIndex = FMath::RandRange(0, 1);
			break;
		}
	case EBossPhase::Phase2:
		{
			RandomIndex = FMath::RandRange(0, 3);
			break;
		}
	case EBossPhase::Phase3:
		{
			RandomIndex = FMath::RandRange(0, 5);
			break;
		}
	}

	mServerAttackMode = static_cast<EBossAttackMode>(RandomIndex);
}

void ABossZero::MulticastBossDeath_Implementation()
{
	mAnimInstance->StopAllMontages(0.f);
	
	mRollingHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mBeam->Deactivate();
}

void ABossZero::OnBegineOverlap_Rolling(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	FDamageEvent DmgEvent;
	OtherActor->TakeDamage(GetAttack() / 2.f, DmgEvent, GetController(), this);

	MulticastRollingHitFX(OtherActor);
}

void ABossZero::MulticastPlayAttackMontage_Implementation()
{
	mAnimInstance->PlayAttackMontage();
}

void ABossZero::MulticastLaser_Implementation()
{
	mAnimInstance->Montage_Pause();

	mBeam->mBeamHead->Activate();
	//Bind Indicator_On_Finished & mBeamBody_On_Finished Function in UBeam Class
	mBeam->mBeamIndicator->Activate();

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mLaserChargeSound, this->GetActorLocation());
}

void ABossZero::MulticastRollingSound_Implementation()
{
	UGameplayStatics::SpawnSoundAttached(mRollingSound, this->GetRootComponent());
}

void ABossZero::MulticastRollingHitFX_Implementation(AActor* OtherActor)
{
	UGameplayStatics::SpawnEmitterAttached(mRollingHitParticle, OtherActor->GetRootComponent());
	UGameplayStatics::SpawnSoundAttached(mRollingHitSound, OtherActor->GetRootComponent());
}

void ABossZero::MulticastSlamLandingFX_Implementation()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mSlamFX, GetActorLocation() + FVector(0.0, 0.0, -270.0));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mSlamSound, this->GetActorLocation());
}

void ABossZero::MulticastSlamHitFX_Implementation(AActor* OtherActor)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mSlamHitParticle,
	                                         OtherActor->GetActorLocation(), OtherActor->GetActorRotation(), true);
	UGameplayStatics::SpawnSoundAttached(mSlamHitSound, OtherActor->GetRootComponent());
}


void ABossZero::TimeHandleSpawnGC(float DT)
{
	FTimerHandle SpawnGeometryCollectionTimerHandle{};

	GetWorldTimerManager().SetTimer(
		SpawnGeometryCollectionTimerHandle,
		this,
		&ABossZero::Server_SpawnGeometrycollection,
		DT,
		false

	);
}

void ABossZero::BossHP(float Percent)
{
	if (IsValid(mBossHPWidget))
	{
		mBossHPWidget->SetProgressBar(Percent);
		BossHPAfterimage(Percent);
	}
	Net_BossHP(Percent);
}

void ABossZero::Net_BossHP_Implementation(float Percent)
{
	if (IsValid(mBossHPWidget))
	{
		mBossHPWidget->SetProgressBar(Percent);
	}
}

void ABossZero::BossHPAfterimage(float Percent)
{
	if (IsValid(mBossHPWidget))
	{
		mBossHPWidget->SetProgressBarImage(Percent);
	}
	Net_BossHPAfterimage(Percent);
}

void ABossZero::Net_BossHPAfterimage_Implementation(float Percent)
{
	if (IsValid(mBossHPWidget))
	{
		mBossHPWidget->SetProgressBarImage(Percent);
	}
}

void ABossZero::GoMain()
{
	GetWorld()->GetTimerManager().ClearTimer(mRestartTimer);
	GetWorld()->ServerTravel(TEXT("StartLevel"));
	UCrazy6GameInstanceSubsystem* SubSystem = GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>();
	SubSystem->DestroySession(SubSystem->GetCurrentSessionName());
	SubSystem->Reset();
}

void ABossZero::UnVisibleUI()
{
	if (IsValid(mBossHPWidget))
	{
		mBossHPWidget->RemoveFromParent();
	}
	if(IsValid(mMissionClearWidget))
	{
		mMissionClearWidget->SetVisibility(ESlateVisibility::Visible);
	}
	Net_UnVisibleUI();
}

void ABossZero::Net_UnVisibleUI_Implementation()
{
	if (IsValid(mBossHPWidget))
	{
		mBossHPWidget->RemoveFromParent();
	}
	if(IsValid(mMissionClearWidget))
	{
		mMissionClearWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABossZero::Callback_SetUIHp(float CurHp)
{
	CurrentHP = CurHp;
	float fMaxHp = mStat->GetMaxHP();
	float fPercent = CurHp / fMaxHp;
	if (IsValid(mBossHPWidget))
	{
		BossHP(fPercent);
	}
	if (0 == CurHp)
	{
		UnVisibleUI();
	}
}

void ABossZero::Server_SpawnGeometrycollection_Implementation()
{
	FVector bossPos = GetActorLocation();
	bossPos.Z = 0.0f;

	FRotator bossRot = GetActorRotation();

	mMesh->SetVisibility(false);

	if (IsValid(mGCActorClass))
	{
		GetWorld()->SpawnActor<ABossGeometryCollectionActor>(mGCActorClass, bossPos, bossRot);
	}
}


void ABossZero::OnRep_mCurrentPhase()
{
	switch (mCurrentPhase)
	{
	case EBossPhase::Phase2:
		{
			mPhase2Visual->Activate();
		}
		break;
	case EBossPhase::Phase3:
		{
			mPhase3Visual->Activate();
		}
		break;
	default:
		break;
	}
}

void ABossZero::OnRep_mServerAttackMode()
{
	mAnimInstance->SetAttackMode(mServerAttackMode);
}

void ABossZero::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABossZero, mServerAttackMode);
	DOREPLIFETIME(ABossZero, mCurrentPhase);
	DOREPLIFETIME(ABossZero, CurrentHP);
}
