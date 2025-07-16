// Fill out your copyright notice in the Description page of Project Settings.


#include "Humanoid.h"

#include "BrainComponent.h"
#include "Crazy6/GameInfo.h"
#include "../EnemyInfo.h"
#include "../EnemyAnimTemplate.h"
#include "Crazy6/Actor/Enemy/Controller/EnemyController.h"
#include "Crazy6/Global/ProfileInfo.h"


AHumanoid::AHumanoid()
{
	/* <=========== Mesh Setting ===========> */
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Dev/AI/Enemy/Humanoid/Meshs/SK_Sci_Fi_Character_08_Full_01.SK_Sci_Fi_Character_08_Full_01'"));
	if (true == MeshAsset.Succeeded())
	{
		mMesh->SetSkeletalMeshAsset(MeshAsset.Object);
		mMesh->SetRelativeLocation(FVector(0.0, 0.0, -84.0));
		mMesh->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
		mCapsule->SetCapsuleHalfHeight(84.f);
		mCapsule->SetCapsuleRadius(37.f);
	}

	// <======= Weapon Mesh Setting =======>
	mWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HumanoidWeaponMesh"));
	mWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/* <=========== Headshot HitBox Setting ===========> */
	mHeadShotHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadShotHitBox"));
	mHeadShotHitBox->SetupAttachment(mMesh, TEXT("Head_Socket"));
	mHeadShotHitBox->SetBoxExtent(FVector(10.0, 9.0, 9.0));
	mHeadShotHitBox->SetRelativeLocation(FVector(4.0, 4.0, 0.0));
	mHeadShotHitBox->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMYHEADHIT);
}

void AHumanoid::BeginPlay()
{
	Super::BeginPlay();
	
	mWeaponMesh->SetIsReplicated(true);

	mAnimInstance = Cast<UEnemyAnimTemplate>(mMesh->GetAnimInstance());
	if (true == IsValid(mAnimInstance))
	{
		mAnimInstance->SetAnimData(mEnemyKey);
	}
}


void AHumanoid::SetAIState(EAIState State)
{
	Super::SetAIState(State);	

	mServerAnimationType = mState;

	if (true == IsValid(mAnimInstance))
	{
		mAnimInstance->ChangeAnimation(mServerAnimationType);
	}
}

void AHumanoid::Attack()
{	
	Super::Attack();
	
	if (true == HasAuthority())
	{
		MulticastPlayAttackMontage();
	}
}

void AHumanoid::CallBack_Hit(float Damage)
{
	Super::CallBack_Hit(Damage);
	SetServerIsHit(true);
	mAnimInstance->SetIsHit(mServerIsHit);
}

void AHumanoid::CallBack_HpZERO()
{
	Super::CallBack_HpZERO();
	
	SetAIState(EAIState::Death);
	
	AEnemyController* AIControl = GetController<AEnemyController>();
	if (true == IsValid(AIControl))
	{
		AIControl->GetBrainComponent()->StopLogic(TEXT("Death"));
	}
}

float AHumanoid::TakeDamage(
	float DamageAmount, 
	FDamageEvent const& DamageEvent, 
	AController* EventInstigator, 
	AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float fDamage = DamageAmount;

	return 0.0f;
}

void AHumanoid::MulticastPlayAttackMontage_Implementation()
{
	mAnimInstance->PlayAttackMontage();
}
