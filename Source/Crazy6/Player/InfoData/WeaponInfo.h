// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponInfo.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mCurrentAmmo = 32;

	/* Current Max Ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mCurrentMaxAmmo = 200;

	/* Fixed value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mClipAmmo = 32;

	/* Fixed value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mDefaultMaxAmmo = 200;

	/* Fixed value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mAmmoCount = 1;

	/* later .. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 mShortGunTrace = 0;
};

USTRUCT(BlueprintType)
struct FWeaponDetails : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeaponData WeaponData;
};

USTRUCT(BlueprintType)
struct FWeaponHitResultInfo : public FTableRowBase
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