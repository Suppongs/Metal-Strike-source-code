// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBulletSpawner.h"
#include "BulletPool.h"
#include "EnemyBulletWithinPool.h"
#include "Crazy6/DebugMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "HumanoidGunMan.h"
#include "Crazy6/GameMode/GameState/InGameState.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Crazy6/Actor/Enemy/Controller/EnemyController.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UEnemyBulletSpawner::UEnemyBulletSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	AInGameState* GameState = Cast<AInGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (true == IsValid(GameState))
	{
		mUsingPool = GameState->GetUsingPool();
		if (false == IsValid(mUsingPool))
		{
			LOG_ERROR(TEXT("Cannot get UsingPool"));
		}
	}

	// <======= Particle Setting =======>
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ShootParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Dev/AI/Enemy/Humanoid/FX/P_Wraith_Drone_Fire.P_Wraith_Drone_Fire'"));
	if (ShootParticle.Succeeded())
	{
		mShootParticle = ShootParticle.Object;
	}
}


// Called when the game starts
void UEnemyBulletSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void UEnemyBulletSpawner::SpawnBullet()
{
	// KTH : Add Null check 
	if (false == IsValid(mOwner))
	{
		LOG_ERROR(TEXT("Owner is not Validable."));
		return;
	}

	PoolableActor = mUsingPool->GetPooledObject();
	mSettedInitialSpeed = PoolableActor->mMovement->InitialSpeed;
	PoolableActor->SetBulletOwner(mOwner);

	// KTH : Add Spread Logic by JHH requesting
	AEnemyController* mOwnerController = Cast<AEnemyController>(mOwner->GetController());
	if (false == IsValid(mOwnerController))
	{
		LOG_ERROR(TEXT("Cast failed on EnemyController"));
		return;
	}
	AActor* TargetPawn = mOwnerController->GetTargetActor();
	if (false == IsValid(TargetPawn))
	{
		LOG_ERROR(TEXT("Not Focusing Target"));
		return;
	}
	const FVector FireLocation = GetComponentLocation();
	const FVector TargetLocation = TargetPawn->GetActorLocation();
	const FVector AimDirection = UKismetMathLibrary::GetDirectionUnitVector(FireLocation, TargetLocation);
	BulletDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(AimDirection, fSpread);
	BulletRotation = AimDirection.ToOrientationRotator();

	PoolableActor->mPrevLocation = GetComponentLocation();
	PoolableActor->ResetBulletRange();
			
	PoolableActor->SetActorLocation(GetComponentLocation());
	PoolableActor->mMovement->SetUpdatedComponent(PoolableActor->GetRootComponent());
	PoolableActor->mMovement->SetVelocityInLocalSpace(BulletDirection * mSettedInitialSpeed);
	PoolableActor->SetActive(true);
	
	MulticastSpawnBulletFX();
}

void UEnemyBulletSpawner::MulticastSpawnBulletFX_Implementation()
{
	if (true == IsValid(mShootParticle))
	{
		UGameplayStatics::SpawnEmitterAttached(mShootParticle, this);
	}
}

