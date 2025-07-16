// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBulletWithinPool.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AHumanoidGunMan;
class UBulletPool;
class UParticleSystem;
class UParticleSystemComponent;
class USoundBase;

UCLASS()
class CRAZY6_API AEnemyBulletWithinPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBulletWithinPool();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:	
	// Called every frame
	void Tick(float DeltaTime) override;

	void SetBulletOwner(AHumanoidGunMan* _Owner){ mOwner = _Owner;}
	FORCEINLINE void SetObjectPool(UBulletPool* _ObjectPool) {mBelongedPool = _ObjectPool;}
	
	void SetActive(bool _IsActive);
	
	FORCEINLINE void ResetBulletRange() {mAccumulatedBulletRange = mDefaultBulletRange;}

private:
	void SelfReturnToPool();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastBulletSoundFX();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastBulletStopFX(const FHitResult& HitResult);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastTargetHitFX(AActor* OtherActor);
	
	UFUNCTION()
	void On_ProjectileStop_Bullet(const FHitResult& HitResult);
	
	UFUNCTION()
	void OnBegineOverlap_EnemyBullet(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);


public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> mMovement = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mDefaultBulletRange = 5000.f;

	float mAccumulatedBulletRange = 0.f;
	
	FVector mPrevLocation = FVector::ZeroVector;
	FVector mCurLocation = FVector::ZeroVector;
	
private:
	bool bActive = false;
	TObjectPtr<UBulletPool> mBelongedPool = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> mBulletHitBox = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AHumanoidGunMan> mOwner = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystem> mBulletTrailSource = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> mBulletTrail = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystem> mTargetHitParticle = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystem> mBlockHitParticle = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mBulletSound = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mBulletBlockSound = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mBulletHitSound = nullptr;
};
