// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMasterWeapon.h"
#include "WeaponSniper.generated.h"

UCLASS()
class CRAZY6_API AWeaponSniper : public APlayerMasterWeapon
{
	GENERATED_BODY()
public:
	AWeaponSniper();
	
	virtual void Tick(float DeltaTime) override;
	// ==================== [ Combat ] ====================
	virtual void FireStart() override;
	virtual void Fire() override;
	virtual void FireStop() override;
	virtual void Reload() override;

protected:
	virtual void BeginPlay() override;
	virtual void ShootLineTrace() override;
	
	// ==================== [ Setting ] ====================

protected:
	// TODO : Sniper Setting !
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sniper|Status")
	float mSniperDamage = 10.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sniper|Status")
	double mSniperFireRate = 60.0 / 800.0; // 800 RPM
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sniper|Status")
	double mSniperFireRange = 20000.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sniper|Status")
	double mSniperBaseSpreadAngle = 0.5;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sniper|Status")
	double mSniperMaxSpreadAngle = 5.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sniper|Status")
	double mSniperSpreadIncreaseRate = 0.25;
};
