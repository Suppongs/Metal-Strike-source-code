// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../InfoData/WeaponInfo.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Crazy6/Player/InfoData/PlayerEnum.h"
#include "PlayerMasterWeapon.generated.h"

class UPlayerWeaponSystem;
class APlayerBase;
class AImpactDecal;

class UCameraShakeBase;
class USoundBase;
class USoundAttenuation;
class USoundConcurrency;
class UNiagaraSystem;
class UAnimMontage;

class UBulletWidget;

UCLASS()
class CRAZY6_API APlayerMasterWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerMasterWeapon();
	virtual void Tick(float DeltaTime) override;

	void SetOwningPlayer(APlayerBase* Player);
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// ==================== [ Combat ] ====================
	// !! Firing Order !!
	/* FireStart -> Fire -> ShootLineTrace -> LineTraceEffectsMulticast
	 * -> PlayFireEffects + WeaponUIReloading
	 * OR PlayDryEffects
	 */
	virtual void FireStart();
	virtual void Fire();
	virtual void FireStop();
	virtual void Reload();

	void UpdateHitResults(const FHitResult& HitResults);
	
	void ChangeABMode(bool IsAMode);

	float GetFireDamage() const { return bNetIsAMode? mAModeDamage : mBModeDamage; }
	
	void ApplyDamage(AActor* HitActor, float Damage);

	void CheckHitActorType();
	
protected:
	UFUNCTION(Server, Unreliable)
	void ChangeABModeServer(bool IsAMode);
	
	virtual void ShootLineTrace();
	virtual void ShootProjectile();

protected:
	/* ----------------------------- Combat|Info ----------------------------- */
	FTimerHandle TimerHandle_Fire;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Info")
	TObjectPtr<APlayerBase>				mNetOwnerPlayer = nullptr;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Info")
	TObjectPtr<UPlayerWeaponSystem>		mNetWeaponSystem = nullptr;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Info")
	EHitActorType mNetHitActorType = EHitActorType::None;
	
	/* ----------------------------- Combat|Status ----------------------------- */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Damage")
	// float mFireDamage		= 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Damage")
	float mAModeDamage		= 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Status")
	float mBModeDamage		= 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Status")
	double mFireStartTime	= 0.0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Status")
	double mFireRate		= 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Status")
	double mAModeFireRange		= 0.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Status")
	double mBModeFireRange		= 0.0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Spread")
	double mAModeBaseSpreadAngle		= 0.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Spread")
	double mBModeBaseSpreadAngle		= 0.0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Spread")
	double mAModeMaxSpreadAngle		= 0.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Spread")
	double mBModeMaxSpreadAngle		= 0.0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Spread")
	double mAModeSpreadIncreaseRate	= 0.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Spread")
	double mBModeSpreadIncreaseRate	= 0.0;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat|Status")
	bool bNetIsAMode			= true;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Status")
	bool bNetWeaponIsReload		= false;

	// FHitResults INFO
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Combat|HitResult")
	TObjectPtr<AActor> mNetCameraHitActor	= nullptr;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Combat|HitResult")
	FVector mNetCameraEnd	= {0, 0, 0};

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Combat|HitResult")
	FVector mNetCameraHitLocation	= {0, 0, 0};
	// FVector_NetQuantize mNetCameraHitLocation	= {0, 0, 0};

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Combat|HitResult")
	FVector mNetCameraHitImpactNormal	= {0, 0, 0};

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Combat|HitResult")
	TObjectPtr<UPrimitiveComponent> mNetCameraHitComponent = nullptr;

public:
	/* ----------------------------- Ammo ----------------------------- */
	FWeaponDetails& GetWeaponDetails() { return mNetWeaopnDetails; }
	
	UFUNCTION()
	bool CanReload() const;

	UFUNCTION()
	bool CanGetMagazine() const;
	
	UFUNCTION()
	void SetWeaponDetials(const FWeaponDetails& AmmoInfo) { mNetWeaopnDetails = AmmoInfo; }
	
	UFUNCTION()
	void AddMagazine();
	
	UFUNCTION()
	bool FireAmmo(int32 AmmoCount);

	UFUNCTION()
	void ReloadAmmo();

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FWeaponDetails mNetWeaopnDetails;

	int32& mNetCurrentAmmo = mNetWeaopnDetails.WeaponData.mCurrentAmmo;
	int32& mNetCurrentMaxAmmo = mNetWeaopnDetails.WeaponData.mCurrentMaxAmmo;
	int32& mClipAmmo = mNetWeaopnDetails.WeaponData.mClipAmmo;
	int32& mDefaultMaxAmmo = mNetWeaopnDetails.WeaponData.mDefaultMaxAmmo;
	int32& mAmmoCount = mNetWeaopnDetails.WeaponData.mAmmoCount;
	int32& mShortGunTrace = mNetWeaopnDetails.WeaponData.mShortGunTrace;
	// ========================= [ Combat END ] =========================
	
	// ========================= [ Effects ] =========================
public:
	virtual TObjectPtr<USkeletalMesh>	GetWeaponMesh() const;
	TObjectPtr<UAnimMontage>		GetPlayerFireMontage()		{ return mPlayerFireMontage; }
	TObjectPtr<UAnimMontage>		GetPlayerReloadMontage()	{ return mPlayerReloadMontage; }
	TObjectPtr<APlayerBase>			GetOwningPlayer()			{ return mNetOwnerPlayer; }
	TObjectPtr<UPlayerWeaponSystem> GetWeaponSystem()			{ return mNetWeaponSystem; }

	UFUNCTION(NetMulticast, Unreliable)
	void FireEffectsMulticast(bool WeaponCanFire);
protected:

	virtual void PlayFireEffects();
	virtual void PlayDryEffects();
	
	UFUNCTION(NetMulticast, Unreliable)
	void ReloadEffectsMulticast();

	UFUNCTION()
	void FireImpactFX(USoundBase* Sound, FVector& Location, USoundAttenuation* AttenuationSettings,
		USoundConcurrency* ConcurrencySettings) const;

	UFUNCTION()
	void MuzzleVFX(UNiagaraSystem* MuzzleNS, USceneComponent* AttachToComponent) const;

	UFUNCTION()
	void ImpactNiagaraVFX(UNiagaraSystem* ImpactNS, const FVector& ImpactLocation, const FVector& HitNormal) const;

	UFUNCTION()
	void FireMontage(UAnimMontage* FireAnim) const;

	UFUNCTION()
	void ImpactDecal(UClass* DecalClass, const FVector_NetQuantize& Location, const FVector_NetQuantize& Normal, const FVector_NetQuantize& Scale) const;

	UFUNCTION()
	void ReloadMontage(UAnimMontage* ReloadAnim) const;

	UFUNCTION()
	void EmptyFX(USoundBase* Sound) const;

	UFUNCTION()
	void EmptyMontage(UAnimMontage* EmptyAnim) const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|Component")
	TObjectPtr<USceneComponent>			mMuzzle						= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|Component")
	TObjectPtr<USkeletalMeshComponent>	mWeaponMesh					= nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects|Component")
	TObjectPtr<USceneComponent>			mDefaultSceneComponent		= nullptr;

	/* ----------------------------- Class ----------------------------- */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|Class")
	TSubclassOf<AActor>					mBulletTraceClass			= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|Class")
	TSubclassOf<AImpactDecal>			mBulletDecalClass			= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Class")
	TSubclassOf<AActor>					mBulletProjectileClass		= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|Class")
	TSubclassOf<UCameraShakeBase>		mCameraShakeClass			= nullptr;

	/* -------------------------- Weapon Effects ------------------------ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|WeaponFire")
	TObjectPtr<UNiagaraSystem>		mMuzzleFireNS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|WeaponFire")
	TObjectPtr<USoundWave>			mBulletFallSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|WeaponFire")
	TObjectPtr<USoundWave>			mWeaponFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|WeaponFire")
	TObjectPtr<UAnimMontage>		mWeaponFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|WeaponReload")
	TObjectPtr<UAnimMontage>		mWeaponReloadMontage;

	/* -------------------------- Imapct Effects ------------------------ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|ImpactFire")
	TObjectPtr<UNiagaraSystem>		mImpactNS;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|ImpactFire")
	TObjectPtr<USoundBase>			mImpactSoundBase;				 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|ImpactFire")
	TObjectPtr<USoundBase>			mBuildingImpactSoundBase;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|ImpactFire")
	TObjectPtr<USoundAttenuation>	mImpactATT;					 
																 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|ImpactFire")
	TObjectPtr<USoundConcurrency>	mImpactSCON;

	/* -------------------------- Player Animations ------------------------ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|PlayerMontage")
	TObjectPtr<UAnimMontage>		mPlayerDryMontage		= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|PlayerMontage")
	TObjectPtr<UAnimMontage>		mPlayerFireMontage		= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|PlayerMontage")
	TObjectPtr<UAnimMontage>		mPlayerReloadMontage	= nullptr;

	/* -------------------------- Enemy Effects ------------------------ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle|EnemyHit")
	TObjectPtr<USoundWave>			mSoundEnemyHit			= nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle|EnemyHit")
	TObjectPtr<USoundWave>			mSoundBossArmorHit		= nullptr;
	// ========================= [ Effects END ] =========================
	
	// ======================== [ Chaos ] ========================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	TSubclassOf<class AFieldSystemActor> mMasterFieldActor;

	// ======================== [ UI ] ========================
public:
	virtual void InitalizeWeaponUI();
	virtual void DestroyWeaponUI();
	virtual void WeaponUIReloading();

	UFUNCTION(NetMulticast, Unreliable)
	void WeaponUIReloadingMulticast();
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf <UUserWidget>	mBulletWidgetClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBulletWidget>	mBulletWidget;
};
