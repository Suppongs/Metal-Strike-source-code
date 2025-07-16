// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMasterWeapon.h"
#include "WeaponMachineGun.generated.h"

UCLASS()
class CRAZY6_API AWeaponMachineGun : public APlayerMasterWeapon
{
	GENERATED_BODY()
public:
	AWeaponMachineGun();
	
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG|Damage")
	float mMachineGunDamageA = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG|Damage")
	float mMachineGunDamageB = 7.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG|Status")
	double mMachineGunFireRate = 60.0 / 900.0; // 900 RPM
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG|Status")
	double mMachineGunFireRange = 15000.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG|Status")
	double mMachineGunBaseSpreadAngleA = 0.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG|Status")
	double mMachineGunBaseSpreadAngleB = 0.2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG|Status")
	double mMachineGunMaxSpreadAngleA = 5.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG|Status")
	double mMachineGunMaxSpreadAngleB = 3.0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG|Status")
	double mMachineGunSpreadIncreaseRateA = 0.25;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MG|Status")
	double mMachineGunSpreadIncreaseRateB = 0.125;
};
