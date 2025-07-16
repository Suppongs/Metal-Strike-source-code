// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponRPG.h"
#include "BulletProjectile.h"
#include "Components/SkeletalMeshComponent.h"

#include "../../Actor/Enemy/EnemyBase.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"
#include "NiagaraSystem.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AWeaponRPG::AWeaponRPG()
{
	mAModeDamage = mRPGAModeDamage;
	mBModeDamage = mRPGBModeDamage;
	mFireRate = mRPGFireRate;
	mAModeFireRange = mRPGFireRange;
	mBModeFireRange = mRPGFireRange;
	mAModeBaseSpreadAngle = mRPGBaseSpreadAngle;
	mBModeBaseSpreadAngle = mRPGBaseSpreadAngle;
	mAModeMaxSpreadAngle = mRPGMaxSpreadAngle;
	mBModeMaxSpreadAngle = mRPGMaxSpreadAngle;
	mAModeSpreadIncreaseRate = mRPGSpreadIncreaseRate;
	mBModeSpreadIncreaseRate = mRPGSpreadIncreaseRate;

	// temp
	mClipAmmo = 3;
	mNetCurrentAmmo = 3;
	mNetCurrentMaxAmmo = 9;
	mDefaultMaxAmmo = 9;

	mWeaponMesh->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));
	mWeaponMesh->SetWorldScale3D({2.5, 2.5, 2.5});
	
	mMuzzle->SetRelativeLocation(FVector(0.0, -24.0, 8.0));
	mMuzzle->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	static	ConstructorHelpers::FObjectFinder<USkeletalMesh>
	MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Dev/Player/Human/Weapon/RPG/SKM_RPG7.SKM_RPG7'"));

	if (MeshAsset.Succeeded())
	{
		mWeaponMesh->SetSkeletalMeshAsset(MeshAsset.Object);
	}
	
	static ConstructorHelpers::FClassFinder<AActor>
	BulletDecalAsset(TEXT("/Script/Engine.Blueprint'/Game/Dev/Player/Human/Weapon/BP_ImpactDecal_Small.BP_ImpactDecal_Small_C'"));

	if (BulletDecalAsset.Succeeded())
	{
		mBulletDecalClass = BulletDecalAsset.Class;
	}
	
	static ConstructorHelpers::FClassFinder<ABulletProjectile>
	BulletProjectileAsset(TEXT("/Script/Engine.Blueprint'/Game/Dev/Player/Human/Weapon/RPG/BP_RPG_Rocket.BP_RPG_Rocket_C'"));

	if (BulletProjectileAsset.Succeeded())
	{
		mBulletProjectileClass = BulletProjectileAsset.Class;
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
		WeaponFireSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Dev/Player/Sounds/RPG_FIre.RPG_FIre'"));
	
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

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
	FireNSAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/Player/Human/Weapon/WeaponEffects/Effects/Particles/Weapons/NS_WeaponFire.NS_WeaponFire'"));

	if (FireNSAsset.Succeeded())
	{
		mImpactNS = FireNSAsset.Object;
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
	
	static ConstructorHelpers::FObjectFinder<USoundWave>
		EnemyHitSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Dev/Player/Sounds/AI_Bullet_Hit.AI_Bullet_Hit'"));
	
	if (EnemyHitSoundAsset.Succeeded())
	{
		mSoundEnemyHit = EnemyHitSoundAsset.Object;
	}
	
	// Weapon & Bullet UI
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UIBulletAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/RPG.RPG_C'"));

	if (UIBulletAsset.Succeeded())
	{
		mBulletWidgetClass = UIBulletAsset.Class;
	}
}

void AWeaponRPG::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponRPG::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponRPG, mBurstCount);
	DOREPLIFETIME(AWeaponRPG, mShotsPerBurst);
	DOREPLIFETIME(AWeaponRPG, mLastFireStartTime);
}

void AWeaponRPG::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponRPG::FireStart()
{
	mFireStartTime = GetWorld()->GetTimeSeconds();

	if (0.f < mLastFireStartTime)
	{
		float Term = mLastFireStartTime - mFireStartTime;
		Term = 0 > Term ? (-1 * Term) : Term;
		if (mFireRate > Term)
		{
			return;
		}
	}
	
	// AMode
	if (true == bNetIsAMode)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AWeaponRPG::Fire, mFireRate, true, 0);
	}
	// BMode
	else
	{
		mBurstCount = 0; // Reset burst count
		GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AWeaponRPG::Fire, 0.6f, true, 0);
	}
}

void AWeaponRPG::Fire()
{
	mLastFireStartTime = GetWorld()->GetTimeSeconds();
	
	// BMode
	if (false == bNetIsAMode)
	{
		mBurstCount++;
		if (mBurstCount >= mShotsPerBurst)
		{
			FireStop();
		}
	}
	
	bool WeaponCanFire = FireAmmo(mAmmoCount);
	
	if (WeaponCanFire)
	{
		ShootProjectile();
	}
	
	FireEffectsMulticast(WeaponCanFire);
}

void AWeaponRPG::FireStop()
{
	Super::FireStop();
}

void AWeaponRPG::Reload()
{
	Super::Reload();
}

void AWeaponRPG::PlaySmokeParticleVFX() const
{
	if (IsValid(mFirePS))
	{
		FVector SmokeLocation = mWeaponMesh->GetSocketLocation("Breech");
		UGameplayStatics::SpawnEmitterAttached(mFirePS, mWeaponMesh, TEXT("None"),
			SmokeLocation, FRotator::ZeroRotator, { 1.f, 1.f, 1.f }, EAttachLocation::KeepRelativeOffset, false, EPSCPoolMethod::AutoRelease, true);
	}
}
