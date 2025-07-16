// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSniper.h"
#include "Components/SkeletalMeshComponent.h"

#include "../../Actor/Enemy/EnemyBase.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"
#include "NiagaraSystem.h"

#include "BulletTrace.h"
#include "ImpactDecal.h"
#include "../PlayerBase.h"

AWeaponSniper::AWeaponSniper()
{
	mAModeDamage = mSniperDamage;
	mFireRate = mSniperFireRate;
	mAModeFireRange = mSniperFireRange;
	mAModeBaseSpreadAngle = mSniperBaseSpreadAngle;
	mAModeMaxSpreadAngle = mSniperMaxSpreadAngle;
	mAModeSpreadIncreaseRate = mSniperSpreadIncreaseRate;

	// temp
	mClipAmmo = 15;
	mNetCurrentAmmo = 15;
	mNetCurrentMaxAmmo = 100;
	mDefaultMaxAmmo = 100;

	mBulletTraceClass = ABulletTrace::StaticClass();
	mBulletDecalClass = AImpactDecal::StaticClass();
	
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

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
	FireNSAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/Player/Human/Weapon/WeaponEffects/Effects/Particles/Weapons/NS_WeaponFire.NS_WeaponFire'"));

	if (FireNSAsset.Succeeded())
	{
		mImpactNS = FireNSAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		FireSoundAsset(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Dev/Player/ThirdPerson/Audio/Sounds/Weapons/Rifle2/MSS_Weapons_Rifle2_Fire.MSS_Weapons_Rifle2_Fire'"));
	
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
		UIBulletAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/Sniper.Sniper_C'"));

	if (UIBulletAsset.Succeeded())
	{
		mBulletWidgetClass = UIBulletAsset.Class;
	}
}

void AWeaponSniper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponSniper::FireStart()
{
	mFireStartTime = GetWorld()->GetTimeSeconds();

	// AMode
	if (true == bNetIsAMode)
	{
		
	}
	// BMode
	else
	{
		GetOwningPlayer()->GetCharacterMovement()->DisableMovement();
	}

	GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AWeaponSniper::Fire, mSniperFireRate, true);
}

void AWeaponSniper::Fire()
{
	bool WeaponCanFire = FireAmmo(mAmmoCount);
	
	if (WeaponCanFire)
	{
		ShootLineTrace();
	}

	FireEffectsMulticast(WeaponCanFire);
}

void AWeaponSniper::FireStop()
{
	GetOwningPlayer()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Super::FireStop();
}

void AWeaponSniper::Reload()
{
	Super::Reload();
}

void AWeaponSniper::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponSniper::ShootLineTrace()
{
	Super::ShootLineTrace();
}
