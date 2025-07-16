#include "WeaponRifle.h"
#include "Components/SkeletalMeshComponent.h"

#include "../PlayerBase.h"
#include "../../Actor/Enemy/EnemyBase.h"

#include "BulletTrace.h"
#include "ImpactDecal.h"

#include "NiagaraSystem.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"

#include "Net/UnrealNetwork.h"

AWeaponRifle::AWeaponRifle() 
	: APlayerMasterWeapon()
{
	// initialize
	mAModeDamage = mRifleAModeDamage;
	mBModeDamage = mRifleBModeDamage;
	mAModeFireRange	= mRifleFireRange;
	mBModeFireRange	= mRifleFireRange;
	mAModeBaseSpreadAngle = mRifleBaseSpreadAngle;
	mBModeBaseSpreadAngle = mRifleBaseSpreadAngle;
	mAModeMaxSpreadAngle = mRifleMaxSpreadAngle;
	mBModeMaxSpreadAngle = mRifleMaxSpreadAngle;
	mAModeSpreadIncreaseRate = mRifleSpreadIncreaseRate;
	mBModeSpreadIncreaseRate = mRifleSpreadIncreaseRate;

	mMuzzle->SetRelativeLocation(FVector(0.0, 70.0, 9.0));
	mMuzzle->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));

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
		UIBulletAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/Riffle.Riffle_C'"));

	if (UIBulletAsset.Succeeded())
	{
		mBulletWidgetClass = UIBulletAsset.Class;
	}
}

void AWeaponRifle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponRifle, mBurstCount);
	DOREPLIFETIME(AWeaponRifle, mShotsPerBurst);
}


void AWeaponRifle::BeginPlay()
{
	Super::BeginPlay();
}

// ==================== [ Combat ] ====================

void AWeaponRifle::FireStart()
{
	// AMode
	if (true == bNetIsAMode)
	{
		// mFireDamage = mRifleAModeDamage;
		mFireStartTime = GetWorld()->GetTimeSeconds();
	}
	// BMode
	else
	{
		// mFireDamage = mRifleBModeDamage;
		mBurstCount = 0; // Reset burst count

		if (GetWorldTimerManager().IsTimerActive(TimerHandle_Fire))
		{
			return;
		}	
	}

	GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AWeaponRifle::Fire, mRifleFireRate, true, 0);
}

void AWeaponRifle::Fire()
{
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
		ShootLineTrace();
	}

	FireEffectsMulticast(WeaponCanFire);
}

void AWeaponRifle::FireStop()
{
	Super::FireStop();
}

void AWeaponRifle::Reload()
{
	Super::Reload();
}

void AWeaponRifle::ShootLineTrace()
{
	Super::ShootLineTrace();
}

// ==================== [ MasterWeapon Override ] ====================

TObjectPtr<USkeletalMesh> AWeaponRifle::GetWeaponMesh() const
{
	return mWeaponMesh->GetSkeletalMeshAsset();
}
