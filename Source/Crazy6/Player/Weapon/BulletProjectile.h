// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "BulletProjectile.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileHitDelegate, const FHitResult&, HitResult, AActor*, HitActor);

class UPlayerWeaponSystem;
class APlayerMasterWeapon;

class UProjectileMovementComponent;
class USphereComponent;

class UCameraShakeBase;
class USoundBase;
class USoundAttenuation;
class USoundConcurrency;
class UNiagaraSystem;

UCLASS()
class CRAZY6_API ABulletProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetOwningWeapon(APlayerMasterWeapon* Weapon) { mOwningWeapon = Weapon; }
	void ShootRPGRocket(const FVector& CameraLocation, const FRotator& CameraRotation, const float& FireRange) const;

private:
	UFUNCTION()
	void OnBulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void Explode();

	UFUNCTION(NetMulticast, Unreliable)
	void PlayExplosionEffects(USceneComponent* HitComponent, const FVector& ImpactLocation, const FVector& HitNormal) const;
	void ExplosionSoundFX(const FVector& ImpactLocation) const;

	UFUNCTION()
	void OnParticleFinished(UParticleSystemComponent* FinishedComponent);
	
	// /* -------------------------- Components ------------------------ */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent>			mStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent>				mSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystemComponent>		mExplosionParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent>	mProjectileMovement;

protected:
	TObjectPtr<APlayerMasterWeapon> mOwningWeapon;
	TObjectPtr<UPlayerWeaponSystem>	mWeaponSystem;

	float mProjectileInitialSpeed = 6000.f;
	float mProjectileMaxSpeed = 7000.f;

	/* -------------------------- Explosion Effects ------------------------ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|Sound")
	TObjectPtr<USoundBase>			mExplosionSoundBase;				 
																 
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|Sound")
	// TObjectPtr<USoundAttenuation>	mExplosionATT;					 
	// 															 
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|Sound")
	// TObjectPtr<USoundConcurrency>	mExplosionSCON;
};
