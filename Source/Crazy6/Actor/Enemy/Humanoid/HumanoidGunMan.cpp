// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanoidGunMan.h"
#include "EnemyBulletSpawner.h"

AHumanoidGunMan::AHumanoidGunMan()
{
	static ConstructorHelpers::FClassFinder<UAnimInstance>
			AnimObject(TEXT("/Script/Engine.AnimBlueprint'/Game/Dev/AI/Enemy/Humanoid/ABP_HumanoidGunMan.ABP_HumanoidGunMan_C'"));
	if (true == AnimObject.Succeeded())
	{
		mMesh->SetAnimInstanceClass(AnimObject.Class);
	}

	/* <=========== Key Setting ===========> */
	mEnemyKey = TEXT("HumanoidGunMan");

	/* <=========== Weapon Setting ===========> */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Dev/AI/Enemy/Humanoid/SubmachineGun/SM_SubMachineGun.SM_SubMachineGun'"));
	if (true == MeshAsset.Succeeded())
	{
		mWeaponMesh->SetStaticMesh(MeshAsset.Object);
		mWeaponMesh->SetupAttachment(mMesh,TEXT("Humanoid_Gun_Socket"));
		mWeaponMesh->SetRelativeRotation(FRotator(0.0f, 18.0f, -10.0f));
	}

	/* <=========== Bullet Spawn Point Setting ===========> */
	mEnemyBulletSpawner = CreateDefaultSubobject<UEnemyBulletSpawner>(TEXT("EnemyBulletSpawner"));
	mEnemyBulletSpawner->SetSpawnerOwner(this);
	mEnemyBulletSpawner->SetRelativeRotation(FRotator(1.0, -90.0, 0.0));
	mEnemyBulletSpawner->SetupAttachment(mWeaponMesh, "Muzzle");
}

void AHumanoidGunMan::Attack()
{
	Super::Attack();
	mCapsule->SetCanEverAffectNavigation(false);
	Shoot();
}

void AHumanoidGunMan::BeginPlay()
{
	Super::BeginPlay();

	mAnimInstance->SetIsMoving(true);
}

void AHumanoidGunMan::Shoot()
{	
	mEnemyBulletSpawner->SpawnBullet();
}

