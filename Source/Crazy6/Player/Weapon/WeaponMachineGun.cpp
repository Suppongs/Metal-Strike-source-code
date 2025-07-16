// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponMachineGun.h"
#include "Components/SkeletalMeshComponent.h"

#include "BulletTrace.h"
#include "ImpactDecal.h"

#include "../../Actor/Enemy/EnemyBase.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"
#include "NiagaraSystem.h"

#include "../PlayerBase.h"

AWeaponMachineGun::AWeaponMachineGun()
{
	mAModeDamage = mMachineGunDamageA;
	mBModeDamage = mMachineGunDamageB;
	mFireRate = mMachineGunFireRate;
	mAModeFireRange = mMachineGunFireRange;
	mBModeFireRange = mMachineGunFireRange;
	mAModeBaseSpreadAngle = mMachineGunBaseSpreadAngleA;
	mBModeBaseSpreadAngle = mMachineGunBaseSpreadAngleB;
	mAModeMaxSpreadAngle = mMachineGunMaxSpreadAngleA;
	mBModeMaxSpreadAngle = mMachineGunMaxSpreadAngleB;
	mAModeSpreadIncreaseRate = mMachineGunSpreadIncreaseRateA;
	mBModeSpreadIncreaseRate = mMachineGunSpreadIncreaseRateB;

	mClipAmmo = 100;
	mNetCurrentAmmo = 100;
	mNetCurrentMaxAmmo = 400;
	mDefaultMaxAmmo = 400;

	mMuzzle->SetRelativeLocation(FVector(0.0, 70.0, 9.0));
	mMuzzle->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));

	// mBulletTraceClass = ABulletTrace::StaticClass();
	// mBulletDecalClass = AImpactDecal::StaticClass();
	
	static	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Dev/Player/Human/Weapon/WeaponEffects/Rifle/Mesh/SK_Rifle.SK_Rifle'"));

	if (MeshAsset.Succeeded())
	{
		mWeaponMesh->SetSkeletalMeshAsset(MeshAsset.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UCameraShakeBase>
		CameraShakeAsset(TEXT("/Script/Engine.Blueprint'/Game/Dev/Player/Human/Weapon/Fire_CameraShake.Fire_CameraShake_C'"));
	
	if (CameraShakeAsset.Succeeded())
	{
		mCameraShakeClass = CameraShakeAsset.Class;
	}
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		MuzzleFireNSAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/Player/Human/Weapon/WeaponEffects/Effects/Particles/Weapons/NS_WeaponFire_MuzzleFlash_Rifle.NS_WeaponFire_MuzzleFlash_Rifle'"));
	
	if (MuzzleFireNSAsset.Succeeded())
	{
		mMuzzleFireNS = MuzzleFireNSAsset.Object;
	}
 
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		FireNSAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/Player/Human/Weapon/WeaponEffects/Effects/Particles/Weapons/NS_WeaponFire.NS_WeaponFire'"));

	if (FireNSAsset.Succeeded())
	{
		mImpactNS = FireNSAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundWave>
		BulletFallSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Dev/Player/Sounds/Bullet_Fall.Bullet_Fall'"));
	
	if (BulletFallSoundAsset.Succeeded())
	{
		mBulletFallSound = BulletFallSoundAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundWave>
		WeaponFireSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Dev/Player/Sounds/Rifle_Fire.Rifle_Fire'"));
	
	if (WeaponFireSoundAsset.Succeeded())
	{
		mWeaponFireSound = WeaponFireSoundAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		WeaponFireMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Dev/Player/Human/Weapon/WeaponEffects/Rifle/Animations/AM_Weap_Rifle_Fire.AM_Weap_Rifle_Fire'"));

	if (WeaponFireMontageAsset.Succeeded())
	{
		mWeaponFireMontage = WeaponFireMontageAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		WeaponReloadMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Dev/Player/Human/Weapon/WeaponEffects/Rifle/Animations/AM_Weap_Rifle_Reload.AM_Weap_Rifle_Reload'"));

	if (WeaponReloadMontageAsset.Succeeded())
	{
		mWeaponReloadMontage = WeaponReloadMontageAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
FireSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Dev/Player/Sounds/Rifle_Hit_Distant.Rifle_Hit_Distant'"));
	
	if (FireSoundAsset.Succeeded())
	{
		mImpactSoundBase = FireSoundAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		FireATTAsset(TEXT("/Script/Engine.SoundAttenuation'/Game/Dev/Player/ThirdPerson/Audio/AttenuationPresets/ATT_Rifle.ATT_Rifle'"));
	
	if (FireATTAsset.Succeeded())
	{
		mImpactATT = FireATTAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundConcurrency>
		FireSCONAsset(TEXT("/Script/Engine.SoundConcurrency'/Game/Dev/Player/ThirdPerson/Audio/Concurrency/SCON_Guns_LimitToOwner.SCON_Guns_LimitToOwner'"));
	
	if (FireSCONAsset.Succeeded())
	{
		mImpactSCON = FireSCONAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		PlayerDryMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Dev/Player/ThirdPerson/Blueprints/Animation/Weapons/Rifle/Montages/MM_Rifle_DryFire.MM_Rifle_DryFire'"));
	
	if (PlayerDryMontageAsset.Succeeded())
	{
		mPlayerDryMontage = PlayerDryMontageAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		PlayerFireAnimAsset(TEXT("/Script/Engine.AnimMontage'/Game/Dev/Player/ThirdPerson/Blueprints/Animation/Weapons/Rifle/Montages/MM_Rifle_Fire.MM_Rifle_Fire'"));
	
	if (PlayerFireAnimAsset.Succeeded())
	{
		mPlayerFireMontage = PlayerFireAnimAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		PlayerReloadMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Dev/Player/ThirdPerson/Blueprints/Animation/Weapons/Rifle/Montages/MM_Rifle_Reload.MM_Rifle_Reload'"));
	
	if (PlayerReloadMontageAsset.Succeeded())
	{
		mPlayerReloadMontage = PlayerReloadMontageAsset.Object;
	}
	
	// Weapon & Bullet UI
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UIBulletAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/Machinegun.Machinegun_C'"));

	if (UIBulletAsset.Succeeded())
	{
		mBulletWidgetClass = UIBulletAsset.Class;
	}
}

void AWeaponMachineGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponMachineGun::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponMachineGun::FireStart()
{
	mFireStartTime = GetWorld()->GetTimeSeconds();

	// BMode
	if (false == bNetIsAMode)
	{
		if (mNetOwnerPlayer)
		{
			mNetOwnerPlayer->GetCharacterMovement()->DisableMovement();
		}
	}

	GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AWeaponMachineGun::Fire, mMachineGunFireRate, true, 0);
}

void AWeaponMachineGun::Fire()
{
	bool WeaponCanFire = FireAmmo(mAmmoCount);
	
	if (WeaponCanFire)
	{
		ShootLineTrace();
	}

	FireEffectsMulticast(WeaponCanFire);
}

void AWeaponMachineGun::FireStop()
{
	Super::FireStop();
	if (false == bNetIsAMode)
	{
		if (mNetOwnerPlayer)
		{
			mNetOwnerPlayer->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			// GetOwningPlayer()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		// mNetOwnerPlayer->GetController()->SetIgnoreMoveInput(false);
	}
}

void AWeaponMachineGun::Reload()
{
	Super::Reload();
}

void AWeaponMachineGun::ShootLineTrace()
{
	Super::ShootLineTrace();
}