// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EnemyBulletSpawner.generated.h"

class AEnemyBulletWithinPool;
class UParticleSystem;
class UBulletPool;
class AHumanoidGunMan;

UCLASS()
class CRAZY6_API UEnemyBulletSpawner : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyBulletSpawner();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	FORCEINLINE void SetSpawnerOwner(AHumanoidGunMan* _Owner) {mOwner = _Owner;}
	
	void SpawnBullet();

private:
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpawnBulletFX();
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBulletPool> mUsingPool = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AEnemyBulletWithinPool> PoolableActor = nullptr;
	
	float mSettedInitialSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	float fSpread = 7.0f;
	
	FVector BulletDirection = FVector::ZeroVector;
	FRotator BulletRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AHumanoidGunMan> mOwner = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystem> mShootParticle = nullptr;
};
