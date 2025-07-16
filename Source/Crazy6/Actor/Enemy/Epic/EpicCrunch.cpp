// Fill out your copyright notice in the Description page of Project Settings.


#include "EpicCrunch.h"
#include "BrainComponent.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Crazy6/GameInfo.h"
#include "../EnemyAnimTemplate.h"
#include "Blueprint/UserWidget.h"
#include "Crazy6/Widget/EpicAppearanceWidget.h"
#include "Crazy6/Actor/Enemy/Controller/EnemyController.h"
#include "Crazy6/Global/ProfileInfo.h"
#include "Net/UnrealNetwork.h"


AEpicCrunch::AEpicCrunch()
{

	// FK
	bUseFabrik  = false;

	/* <=========== Mesh Setting ===========> */
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Dev/AI/Enemy/Epic/Crunch_Black_Site.Crunch_Black_Site'"));
	if (true == MeshAsset.Succeeded())
	{
		mMesh->SetSkeletalMeshAsset(MeshAsset.Object);
		mMesh->SetRelativeLocation(FVector(0.0, 0.0, -95.0));
		mMesh->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
		mCapsule->SetCapsuleHalfHeight(99.44f);
		mCapsule->SetCapsuleRadius(99.44f);

		mRightArmCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightArmCapsule"));
		mRightArmCapsule->SetupAttachment(mMesh, TEXT("HandCapsule_r"));
		mRightArmCapsule->SetCapsuleHalfHeight(67.f);
		mRightArmCapsule->SetCapsuleRadius(30.4f);
		mRightArmCapsule->SetRelativeLocation(FVector(-15.91, -0.82, -18.57));
		mRightArmCapsule->SetRelativeRotation(FRotator(91.0, 0.0, 180.0));
		mRightArmCapsule->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMY);
		mRightArmCapsule->SetCanEverAffectNavigation(false);

		mLeftArmCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftArmCapsule"));
		mLeftArmCapsule->SetupAttachment(mMesh, TEXT("HandCapsule_l"));
		mLeftArmCapsule->SetCapsuleHalfHeight(67.f);
		mLeftArmCapsule->SetCapsuleRadius(30.4f);
		mLeftArmCapsule->SetRelativeLocation(FVector(14.8, 9.94, 19.41));
		mLeftArmCapsule->SetRelativeRotation(FRotator(88.0, 0.0, 180.0));
		mLeftArmCapsule->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMY);
		mLeftArmCapsule->SetCanEverAffectNavigation(false);

		static ConstructorHelpers::FClassFinder<UAnimInstance>
			AnimObject(TEXT("/Script/Engine.AnimBlueprint'/Game/Dev/AI/Enemy/Epic/ABP_Crunch.ABP_Crunch_C'"));
		if (true == AnimObject.Succeeded())
		{
			mMesh->SetAnimInstanceClass(AnimObject.Class);
		}
	}

	/* <=========== Key Setting ===========> */
	mEnemyKey = TEXT("EpicCrunch");

	/* <=========== Hand HitBox Setting ===========> */
	mHandHitBoxLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("HandHitBoxLeft"));
	mHandHitBoxLeft->SetupAttachment(mMesh, TEXT("AttackHitBox_l"));
	mHandHitBoxLeft->SetBoxExtent(FVector(52.0, 35.0, 26.0));
	mHandHitBoxLeft->SetRelativeLocation(FVector(9.0, 19.0, -3.0));
	mHandHitBoxLeft->SetRelativeRotation(FRotator(0.0, 0.0, 5.0));
	mHandHitBoxLeft->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMYATTACK);
	mHandHitBoxLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mHandHitBoxRight = CreateDefaultSubobject<UBoxComponent>(TEXT("HandHitBoxRight"));
	mHandHitBoxRight->SetupAttachment(mMesh, TEXT("AttackHitBox_r"));
	mHandHitBoxRight->SetBoxExtent(FVector(52.0, 35.0, 26.0));
	mHandHitBoxRight->SetRelativeLocation(FVector(-10.0, -20.0, 5.0));
	mHandHitBoxRight->SetRelativeRotation(FRotator(0.0, 0.0, 20.0));
	mHandHitBoxRight->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMYATTACK);
	mHandHitBoxRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mRushHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RushHitBox"));
	mRushHitBox->SetupAttachment(mMesh, TEXT("RushAttackHitBox"));
	mRushHitBox->SetBoxExtent(FVector(99.0, 72.0, 209.0));
	mRushHitBox->SetRelativeLocation(FVector(26.0, 27.0, -11.0));
	mRushHitBox->SetRelativeRotation(FRotator(18.0, -35.0, -41.0));
	mRushHitBox->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMYATTACK);
	mRushHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/* <=========== Headshot HitBox Setting ===========> */
	mHeadShotHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadShotHitBox"));
	mHeadShotHitBox->SetupAttachment(mMesh, TEXT("Head_Socket"));
	mHeadShotHitBox->SetBoxExtent(FVector(13.0, 20.0, 19.0));
	mHeadShotHitBox->SetRelativeLocation(FVector(0.0, 3.0, 3.0));
	mHeadShotHitBox->SetRelativeRotation(FRotator(6.0, 0.0, -33.0));
	mHeadShotHitBox->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMYHEADHIT);
	
	/* <=========== Attack OverLap Setting ===========> */
	mHandHitBoxRight->OnComponentBeginOverlap.AddDynamic(this,
		&AEpicCrunch::OnBegineOverlap_Hand);
	mHandHitBoxLeft->OnComponentBeginOverlap.AddDynamic(this,
		&AEpicCrunch::OnBegineOverlap_Hand);
	mRushHitBox->OnComponentBeginOverlap.AddDynamic(this,
		&AEpicCrunch::OnBegineOverlap_Rush);

	/* <=========== Niagara Setting ===========> */
	mRushAttackIndicator = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RushAttackIndicator"));
	mRushAttackIndicator->SetupAttachment(mCapsule);
	mRushAttackIndicator->SetRelativeLocation(FVector(0.0, 0.0, 1.0 - mCapsule->GetScaledCapsuleHalfHeight()));
	mRushAttackIndicator->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
			NiagaraParticle(TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/UI/AttackIndicator/NS_SquareIndicator.NS_SquareIndicator'"));
	if (true == NiagaraParticle.Succeeded())
	{
		mRushAttackIndicator->SetAsset(NiagaraParticle.Object);
	}
	mRushAttackIndicator->SetAutoActivate(false);

	// <======= Particle Setting =======>
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		HitParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Dev/AI/Enemy/Humanoid/FX/P_Minion_Impact_Default.P_Minion_Impact_Default'"));
	if (HitParticle.Succeeded())
	{
		mHitParticle = HitParticle.Object;
	}

	// <======= Sound Setting =======>	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		HitSound(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Epic/FX/Sound/punch-hit.punch-hit'"));
	if (HitSound.Succeeded())
	{
		mHandHitSound = HitSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>
		ChargingSound(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Epic/FX/Sound/charging.charging'"));
	if (ChargingSound.Succeeded())
	{
		mChargingSound = ChargingSound.Object;
	}
}

void AEpicCrunch::BeginPlay()
{
	Super::BeginPlay();

	mAnimInstance = Cast<UEnemyAnimTemplate>(mMesh->GetAnimInstance());
	if (true == IsValid(mAnimInstance))
	{
		mAnimInstance->SetAnimData(mEnemyKey);
	}
}

void AEpicCrunch::SetAIState(EAIState State)
{
	Super::SetAIState(State);

	mServerAnimationType = mState;

	if (true == IsValid(mAnimInstance))
	{
		mAnimInstance->ChangeAnimation(mServerAnimationType);
	}
}

void AEpicCrunch::Attack()
{	
	Super::Attack();

	if (false == GetWorld()->GetTimerManager().IsTimerActive(mRushAttackTimer))
	{
		MulticastRushAttack();
	}
	else
	{
		MulticastPlayAttackMontage();
	}
}

void AEpicCrunch::CallBack_Hit(float Damage)
{
	Super::CallBack_Hit(Damage);
	
	SetServerIsHit(true);
	mAnimInstance->SetIsHit(mServerIsHit);
}

void AEpicCrunch::CallBack_HpZERO()
{
	Super::CallBack_HpZERO();

	MulticastEpicDeath();
	
	SetAIState(EAIState::Death);
	
	AEnemyController* AIControl = GetController<AEnemyController>();
	if (true == IsValid(AIControl))
	{
		AIControl->GetBrainComponent()->StopLogic(TEXT("Death"));
	}
}

void AEpicCrunch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (true == IsRushing)
	{
		FVector NextLocation = GetActorLocation() + (GetActorForwardVector() * mRushDistance * DeltaTime);
		NextLocation.Z -= mCapsule->GetScaledCapsuleHalfHeight();
		FNavLocation NavLocation;
		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (true == IsValid(NavSystem) && true == NavSystem->ProjectPointToNavigation(NextLocation, NavLocation))
		{
			NavLocation.Location.Z += mCapsule->GetScaledCapsuleHalfHeight();
			mCapsule->SetRelativeLocation(NavLocation.Location);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("**Epic : out of navmesh**"));
		}
	}
}

void AEpicCrunch::WhenRushEnd()
{
	IsRushing = false;
	SetAttackRange(100.0);

	mRushAttackedCollection.Empty();
}

void AEpicCrunch::AttackHitBoxOn()
{
	Super::AttackHitBoxOn();

	if (true == IsRushing)
	{
		mRushHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		if (false == IsLeftHandAttacking)
		{
			mHandHitBoxLeft->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			IsLeftHandAttacking = true;
		}
		else
		{
			mHandHitBoxRight->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			IsLeftHandAttacking = false;
		}
	}
}

void AEpicCrunch::AttackHitBoxOff()
{
	Super::AttackHitBoxOff();

	if (true == IsRushing)
	{
		mRushHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		if (true == IsLeftHandAttacking)
		{
			mHandHitBoxLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			mHandHitBoxRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void AEpicCrunch::MulticastResumeMontage_Implementation()
{
	//if reference is nullptr, it will resume all active montage
	mAnimInstance->Montage_Resume(nullptr);
}

void AEpicCrunch::MulticastPlayAttackMontage_Implementation()
{
	mAnimInstance->PlayAttackMontage();
}

void AEpicCrunch::MulticastRushAttack_Implementation()
{
	if (true == HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(mRushAttackTimer, this,
		&AEpicCrunch::SwitchTrueIsRushAttackReady, mRushAttackCoolTime, false);
	}
			
	mAnimInstance->PlayAttackMontage();
	mAnimInstance->Montage_Pause();
		
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mChargingSound, this->GetActorLocation());
		
	mRushAttackIndicator->Activate();

	if (true == HasAuthority())
	{
		//Set affect navigation off, because Epic's ground is out of navmesh, so can't rush
		mCapsule->SetCanEverAffectNavigation(false);
		
		GetWorld()->GetTimerManager().SetTimer(mRushAttackChargeTimer, this,
			&AEpicCrunch::ChargeEnd, mRushAttackChargingTime, false);
	}
	
	mServerIsRushAttackReady = false;
	OnRep_mServerIsRushAttackReady();
}

void AEpicCrunch::MulticastHandHitFX_Implementation(AActor* OtherActor)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mHitParticle,
			OtherActor->GetActorLocation(), OtherActor->GetActorRotation(), true);

	UGameplayStatics::SpawnSoundAttached(mHandHitSound, OtherActor->GetRootComponent());
}

void AEpicCrunch::MulticastRushHitFX_Implementation(UPrimitiveComponent* OtherComp)
{
	UGameplayStatics::SpawnEmitterAttached(mHitParticle, OtherComp);

	UGameplayStatics::SpawnSoundAttached(mHandHitSound, OtherComp);
}

void AEpicCrunch::MulticastEpicDeath_Implementation()
{
	mAnimInstance->StopAllMontages(0.f);

	mRushAttackIndicator->Deactivate();
	mRushHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mHandHitBoxLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mHandHitBoxRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorTickEnabled(false);
}

void AEpicCrunch::OnRep_mServerIsRushAttackReady()
{
	if (true == IsValid(mAnimInstance))
	{
		mAnimInstance->SetIsRushAttackReady(mServerIsRushAttackReady);
	}
}

void AEpicCrunch::SwitchTrueIsRushAttackReady()
{
	IsRushing = false;
	mServerIsRushAttackReady = true;
	OnRep_mServerIsRushAttackReady();
	SetAttackRange(1000.0);
	GetWorld()->GetTimerManager().ClearTimer(mRushAttackTimer);
}
void AEpicCrunch::ChargeEnd()
{
	MulticastResumeMontage();
	IsRushing = true;
	GetWorld()->GetTimerManager().ClearTimer(mRushAttackChargeTimer);
}

void AEpicCrunch::OnBegineOverlap_Hand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FDamageEvent	DmgEvent;
	OtherActor->TakeDamage(GetAttack(), DmgEvent, GetController(),this);
	
	MulticastHandHitFX(OtherActor);
}

void AEpicCrunch::OnBegineOverlap_Rush(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (false == mRushAttackedCollection.Contains(OtherActor))
	{
		mRushAttackedCollection.Push(OtherActor);
		
		FDamageEvent	DmgEvent;
		OtherActor->TakeDamage(GetAttack() * 2, DmgEvent, GetController(),this);

		ACharacter* Player = Cast<ACharacter>(OtherActor);
		if (true == IsValid(Player))
		{
			Player->LaunchCharacter((mCapsule->GetForwardVector() + mCapsule->GetUpVector()) * 700.0, true, true);
		}

		MulticastRushHitFX(OtherComp);
	}
}

void AEpicCrunch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEpicCrunch, mServerIsRushAttackReady);
}

void AEpicCrunch::VisibleWidget()
{
	mWidget->RemoveFromParent();
}
