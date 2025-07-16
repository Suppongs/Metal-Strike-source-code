// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMasterWeapon.h"
#include "WeaponRPG.generated.h"

UCLASS()
class CRAZY6_API AWeaponRPG : public APlayerMasterWeapon
{
	GENERATED_BODY()
public:
	AWeaponRPG();
	
	virtual void Tick(float DeltaTime) override;

	// ==================== [ Networking ] ====================
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ========================================================
	
	// ==================== [ Combat ] ====================
	virtual void FireStart() override;
	virtual void Fire() override;
	virtual void FireStop() override;
	virtual void Reload() override;

protected:
	virtual void BeginPlay() override;

	void PlaySmokeParticleVFX() const;
	
	// ==================== [ Setting ] ====================
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RPG|Combat|Damage")
	float mRPGAModeDamage			= 70.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RPG|Combat|Damage")
	float mRPGBModeDamage			= 40.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RPG|Combat|Status")
	double mRPGFireRate				= 60.0 / 60.0; //60 RPM
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RPG|Combat|Status")
	double mRPGFireRange			= 20000.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RPG|Combat|Spread")
	double mRPGBaseSpreadAngle		= 0.5;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RPG|Combat|Spread")
	double mRPGMaxSpreadAngle		= 5.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RPG|Combat|Spread")
	double mRPGSpreadIncreaseRate	= 0.25;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "RPG|Combat|Status")
	int mBurstCount					= 0; 
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "RPG|Combat|Status")
	int mShotsPerBurst				= 3;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "RPG|Combat|Status")
	float mLastFireStartTime		= 0.f;
	
	// UPROPERTY(Replicated, BlueprintReadOnly, Category = "RPG|Combat|Status")
	// bool bNetCanRPGFire				= true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RPG|Effects")
	TObjectPtr<UParticleSystem>		mFirePS;

};
