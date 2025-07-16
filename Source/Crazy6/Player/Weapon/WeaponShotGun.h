// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMasterWeapon.h"
#include "WeaponShotGun.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FShotGunHitInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FHitResult	mHitResult;

	FShotGunHitInfo()
		:mHitResult(FHitResult())
	{ }
};

USTRUCT(BlueprintType)
struct FShotGunHitResults : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FHitResult mNetShotGunHits_Zero;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FHitResult mNetShotGunHits_One;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FHitResult mNetShotGunHits_Two;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FHitResult mNetShotGunHits_Three;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	FHitResult mNetShotGunHits_Four;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	FHitResult mNetShotGunHits_Five;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	FHitResult mNetShotGunHits_Six;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly) 
	FHitResult mNetShotGunHits_Seven;
};

UCLASS()
class CRAZY6_API AWeaponShotGun : public APlayerMasterWeapon
{
	GENERATED_BODY()
public:
	AWeaponShotGun();
	
	virtual void Tick(float DeltaTime) override;
	// ==================== [ Combat ] ====================
	virtual void FireStart() override;
	virtual void Fire() override;
	virtual void FireStop() override;
	virtual void Reload() override;
	
	void UpdateShotgunHitResults(TArray<FHitResult>& HitResults);
	// void UpdateShotgunHitResults(TArray<FVector>& HitEndLocations, TArray<FHitResult>& HitResults);
	
protected:
	virtual void BeginPlay() override;
	virtual void ShootLineTrace() override;

	// ========================= [ Effects ] =========================
	virtual void PlayFireEffects() override;

private:
	// ==================== [ Networking ] ====================
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ======================================================

	// ==================== [ Setting ] ====================

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShotGun|Damage")
	float mShotGunDamageA = 7.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShotGun|Damage")
	float mShotGunDamageB = 9.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShotGun|Status")
	double mShotGunFireRate = 60.0 / 300.0; // 300 RPM

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShotGun|Status")
	double mShotGunFireRangeA = 10000.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShotGun|Status")
	double mShotGunFireRangeB = 8000.0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShotGun|Status")
	double mShotGunBaseSpreadAngleA = 0.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShotGun|Status")
	double mShotGunBaseSpreadAngleB = 0.2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShotGun|Status")
	double mShotGunMaxSpreadAngleA = 2.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShotGun|Status")
	double mShotGunMaxSpreadAngleB = 1.25;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShotGun|Status")
	double mShotGunSpreadIncreaseRateA = 0.25;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShotGun|Status")
	double mShotGunSpreadIncreaseRateB = 0.1;
	
	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "ShotGun|Replicatation")
	FShotGunHitResults mShotGunHitResults;
	
	FHitResult mShotGunHitResultsArr[8] =
	{
		mShotGunHitResults.mNetShotGunHits_Zero,
		mShotGunHitResults.mNetShotGunHits_One,
		mShotGunHitResults.mNetShotGunHits_Two,
		mShotGunHitResults.mNetShotGunHits_Three,
		mShotGunHitResults.mNetShotGunHits_Four,
		mShotGunHitResults.mNetShotGunHits_Five,
		mShotGunHitResults.mNetShotGunHits_Six,
		mShotGunHitResults.mNetShotGunHits_Seven
	};

};
