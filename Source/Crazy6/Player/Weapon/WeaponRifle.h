// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMasterWeapon.h"
#include "WeaponRifle.generated.h"

UCLASS()
class CRAZY6_API AWeaponRifle : public APlayerMasterWeapon
{
	GENERATED_BODY()

public:
	AWeaponRifle();

	// ==================== [ Networking ] ====================
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ========================================================
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ==================== [ Combat ] ====================
public:
	virtual void FireStart() override;
	virtual void Fire() override;
	virtual void FireStop() override;

	virtual void Reload() override;
	virtual void ShootLineTrace() override;

protected:
	virtual TObjectPtr<USkeletalMesh> GetWeaponMesh() const override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Rifle|Combat|Status")
	int mBurstCount			= 0; 
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Rifle|Combat|Status")
	int mShotsPerBurst		= 3;
	
	// ==================== [ Combat ] ====================
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rifle|Combat|Status")
	double mRifleFireRate			= 60.0 / 650.0; // 650 RPM

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rifle|Combat|Status")
	double mRifleFireRange			= 15000.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rifle|Combat|Spread")
	double mRifleBaseSpreadAngle	= 0.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rifle|Combat|Spread")
	double mRifleMaxSpreadAngle		= 5.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rifle|Combat|Spread")
	double mRifleSpreadIncreaseRate	= 0.25;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rifle|Combat|Damage")
	float mRifleAModeDamage			= 8.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rifle|Combat|Damage")
	float mRifleBModeDamage			= 15.f;
};
