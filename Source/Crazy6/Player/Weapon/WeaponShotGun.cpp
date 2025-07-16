// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponShotGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerWeaponSystem.h"

#include "BulletTrace.h"
#include "ImpactDecal.h"

#include "NiagaraSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"

#include "Field/FieldSystemActor.h" 
#include "Crazy6/Actor/Buidling/CacheTriggerBox.h"
#include "Net/UnrealNetwork.h"
#include "AIController.h"
#include "../../Actor/Enemy/EnemyBase.h"
#include "Crazy6/Actor/Enemy/Boss/BossZero.h"
#include "../PlayerBase.h"

AWeaponShotGun::AWeaponShotGun()
{
	mAModeDamage = mShotGunDamageA;
	mBModeDamage = mShotGunDamageB;
	mFireRate = mShotGunFireRate;
	mAModeFireRange = mShotGunFireRangeA;
	mBModeFireRange = mShotGunFireRangeB;
	mAModeBaseSpreadAngle = mShotGunBaseSpreadAngleA;
	mBModeBaseSpreadAngle = mShotGunBaseSpreadAngleB;
	mAModeMaxSpreadAngle = mShotGunMaxSpreadAngleA;
	mBModeMaxSpreadAngle = mShotGunMaxSpreadAngleB;
	mAModeSpreadIncreaseRate = mShotGunSpreadIncreaseRateA;
	mBModeSpreadIncreaseRate = mShotGunSpreadIncreaseRateB;

	mClipAmmo = 16;
	mNetCurrentAmmo = 16;
	mNetCurrentMaxAmmo = 80;
	mDefaultMaxAmmo = 80;
	mShortGunTrace = 8;

	mBulletTraceClass = ABulletTrace::StaticClass();
	mBulletDecalClass = AImpactDecal::StaticClass();

	static	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Dev/Player/Weapons/Shotgun/Mesh/SKM_Shotgun.SKM_Shotgun'"));
	
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
		PlayerDryMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Dev/Player/ThirdPerson/Blueprints/Animation/Weapons/ShotGun/MM_ShotGun_DryFire.MM_ShotGun_DryFire'"));
	
	if (PlayerDryMontageAsset.Succeeded())
	{
		mPlayerDryMontage = PlayerDryMontageAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		PlayerFireAnimAsset(TEXT("/Script/Engine.AnimMontage'/Game/Dev/Player/Weapons/Shotgun/Animations/AM_MM_Shotgun_Fire.AM_MM_Shotgun_Fire'"));
	
	if (PlayerFireAnimAsset.Succeeded())
	{
		mPlayerFireMontage = PlayerFireAnimAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		PlayerReloadMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Dev/Player/Weapons/ShotGun/Animations/AM_MM_Shotgun_Reload.AM_MM_Shotgun_Reload'"));
	
	if (PlayerReloadMontageAsset.Succeeded())
	{
		mPlayerReloadMontage = PlayerReloadMontageAsset.Object;
	}
	
	// Weapon & Bullet UI
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UIBulletAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/ShotGun.ShotGun_C'"));

	if (UIBulletAsset.Succeeded())
	{
		mBulletWidgetClass = UIBulletAsset.Class;
	}
}

void AWeaponShotGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponShotGun::BeginPlay()
{
	Super::BeginPlay();
}

/* Server */
void AWeaponShotGun::FireStart()
{
	// AMode
	if (true == bNetIsAMode)
	{
		// mAModeFireRange = mShotGunFireRangeA;
		// mAModeBaseSpreadAngle = mShotGunBaseSpreadAngleA;
		// mAModeMaxSpreadAngle = mShotGunMaxSpreadAngleA;
		// mAModeSpreadIncreaseRate = mShotGunSpreadIncreaseRateA;
	}
	// BMode
	else
	{
		// mAModeFireRange = mShotGunFireRangeB;
		// mAModeBaseSpreadAngle = mShotGunBaseSpreadAngleB;
		// mAModeMaxSpreadAngle = mShotGunMaxSpreadAngleB;
		// mAModeSpreadIncreaseRate = mShotGunSpreadIncreaseRateB;
	}

	GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AWeaponShotGun::Fire, mShotGunFireRate, true, 0);
}

void AWeaponShotGun::Fire()
{
	// AMode
	if (true == bNetIsAMode)
	{
		// mFireDamage = mShotGunAModeDamage;
		mFireStartTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		// mFireDamage = mShotGunBModeDamage;
	}
	bool WeaponCanFire = FireAmmo(mAmmoCount);
	if (WeaponCanFire)
	{
		ShootLineTrace();
	}

	FireEffectsMulticast(WeaponCanFire);
}

void AWeaponShotGun::FireStop()
{
	GetOwningPlayer()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Super::FireStop();
}

void AWeaponShotGun::ShootLineTrace()
{
	float FireDamage = bNetIsAMode? mAModeDamage : mBModeDamage;
	double FireRange = bNetIsAMode? mAModeFireRange : mBModeFireRange;
	double MaxSpreadAngle = bNetIsAMode? mAModeMaxSpreadAngle : mBModeMaxSpreadAngle;
	double SpreadIncreaseRate = bNetIsAMode? mAModeSpreadIncreaseRate : mBModeSpreadIncreaseRate;
	
	TArray<AActor*> LineTraceIgnoreActors;
	LineTraceIgnoreActors.Add(GetOwner());
	UWorld* World = GetWorld();

	if (!mNetOwnerPlayer)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
			FString::Printf(TEXT("!! AWeaponShotGun::ShootLineTrace() : mOwnerPlayer is NULL !!")));
		return;
	}

	if (nullptr == World)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
			FString::Printf(TEXT("!! AWeaponShotGun::ShootLineTrace() : GetWorld returns nullptr !!")));
		return;
	}
	
	// TArray<FVector> PelletsEndLocations;
	// PelletsEndLocations.SetNum(mShortGunTrace);
	
	TArray<FHitResult> PelletsHitResults;
	PelletsHitResults.SetNum(mShortGunTrace);
	
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	FVector CameraLocation;
	FRotator CameraRotation;

	if (PlayerController)
	{
		// Get the Client's camera position and direction values
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	}

	// < Camera >
	FVector CameraStart = CameraLocation;
	FVector CameraEnd = CameraStart + (CameraRotation.Vector() * FireRange);
	
	/* ShotGun Recoil */
	float SpreadAngle = FMath::DegreesToRadians(MaxSpreadAngle);
	
	for (int i = 0; i < mShortGunTrace; ++i)
	{
		float RandomPitch = FMath::FRandRange(-SpreadAngle, SpreadAngle);
		float RandomYaw = FMath::FRandRange(-SpreadAngle, SpreadAngle);

		FRotator FinalAimDirection = CameraRotation;
		FinalAimDirection.Pitch += FMath::RadiansToDegrees(RandomPitch);
		FinalAimDirection.Yaw += FMath::RadiansToDegrees(RandomYaw);

		FVector PelletEnd = CameraStart + FinalAimDirection.Vector() * FireRange;

		UKismetSystemLibrary::LineTraceSingle(World, CameraStart, PelletEnd, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5)
		, true, LineTraceIgnoreActors, EDrawDebugTrace::None, PelletsHitResults[i], true, FLinearColor::Blue, FLinearColor::Green, 5.0f);
	}

	UpdateShotgunHitResults(PelletsHitResults);

	// ========== Hit Check ==========
	float ToTalDamage = 0.f;
	
	for (int i = 0; i < mShortGunTrace; ++i)
	{
		// No Hit
		if (nullptr == mShotGunHitResultsArr[i].GetActor())
		{
			mNetHitActorType = EHitActorType::None;
			return;
		}

		APawn* HitPawn = Cast<APawn>(mShotGunHitResultsArr[i].GetActor());
		AAIController* HitAIController = nullptr;

		// Hit Actor
		if (nullptr == HitPawn)
		{
			mNetHitActorType = EHitActorType::Actor;
		}
		// Hit Pawn
		else
		{
			HitAIController = Cast<AAIController>(HitPawn->GetController());
		}
	
		// Hit AI
		if (nullptr != HitAIController)
		{
			mNetHitActorType = EHitActorType::AI;
		
			// HeadShot System
			bool IsHeadShot = mShotGunHitResultsArr[i].GetComponent()->IsA(UBoxComponent::StaticClass());
			float fDamage = FireDamage;
			if (true == IsHeadShot
				&& IsValid(Cast<ABossZero>(mShotGunHitResultsArr[i].GetActor())))
			{
				fDamage = 0.f; //Hit Boss Armor
				IsHeadShot = false;
			}
			if (true == IsHeadShot)
			{
				fDamage = FireDamage * 1.25; // Headshot Damage
			}

			AEnemyBase* EnemyPawn = Cast<AEnemyBase>(HitPawn);
			if (true == IsValid(EnemyPawn))
			{
				mNetOwnerPlayer->DamageHeadShotUIClient(EnemyPawn, IsHeadShot);
			}
			ApplyDamage(mShotGunHitResultsArr[i].GetActor(), fDamage);
		}
	}
}

void AWeaponShotGun::PlayFireEffects()
{
	if (!IsValid(mWeaponMesh))
	{
		return;
	}

	FVector WeaponStart = mWeaponMesh->GetSocketLocation("Muzzle");

	if (FVector::ZeroVector == WeaponStart)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		FString::Printf(TEXT("!! AWeaponShotGun::ShootLineTrace() : Socket 'Muzzle' is ZeroVector !!")));
	}
	
	/* 8 */
	if (IsValid(mBulletTraceClass))
	{
		for (int i = 0; i < mShortGunTrace; ++i)
		{
			FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(WeaponStart,
				UKismetMathLibrary::MakeRotFromX(mShotGunHitResultsArr[i].TraceEnd - WeaponStart));
			GetWorld()->SpawnActor<AActor>(mBulletTraceClass, SpawnTransform);	
		}
	}

	/* 1 */
	if (IsValid(mBulletFallSound) && IsValid(mWeaponFireSound))
	{
		if (FVector::ZeroVector != mWeaponMesh->GetSocketLocation("Door"))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), mBulletFallSound, mWeaponMesh->GetSocketLocation("Door"));
		}
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), mWeaponFireSound, WeaponStart);	
	}
	/* 1 */
	if (IsValid(mMuzzleFireNS) && IsValid(mPlayerFireMontage)
		&& IsValid(mWeaponFireMontage))
	{
		MuzzleVFX(mMuzzleFireNS, mMuzzle);
		FireMontage(mPlayerFireMontage);
		mWeaponMesh->PlayAnimation(mWeaponFireMontage, false);
	}
	
	// ========== Hit Check ==========
	switch(mNetHitActorType)
	{
	case EHitActorType::None:
		{
		}

	case EHitActorType::Actor:
	// case EHitActorType::LandScape:
		{
			// TODO : Refactoring...
			if (IsValid(mBulletDecalClass))
			{
				int8 idx = 0;
				for (int i = 0; i < mShortGunTrace; ++i)
				{
					if(mShotGunHitResultsArr[i].bBlockingHit)
					{
						idx = i;
						ImpactDecal(mBulletDecalClass, mShotGunHitResultsArr[i].Location,
							mShotGunHitResultsArr[i].ImpactNormal, { 0.01f, 0.03f, 0.03f });

						if (IsValid(mImpactNS)
							&& FVector::ZeroVector != mNetCameraHitLocation
							&& FVector::ZeroVector != mNetCameraHitImpactNormal)
						{
							ImpactNiagaraVFX(mImpactNS, mShotGunHitResultsArr[i].Location, mShotGunHitResultsArr[i].ImpactNormal);
						}
					}
				}
				if (IsValid(mImpactSoundBase)
					|| IsValid(mImpactATT)
					|| IsValid(mImpactSCON))
				{
					FireImpactFX(mImpactSoundBase, mShotGunHitResultsArr[idx].TraceEnd, mImpactATT, mImpactSCON);
				}	
			}
			break;
		}
		
	case EHitActorType::Building:
		{
			/* Building Impact Effects */
		}
		
	case EHitActorType::AI:
		{
			for (int i = 0; i < mShortGunTrace; ++i)
			{
				if(mShotGunHitResultsArr[i].bBlockingHit)
				{
					// UGameplayStatics::PlaySoundAtLocation(GetWorld(), mSoundEnemyHit, GetActorLocation());
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), mSoundEnemyHit, mShotGunHitResultsArr[i].Location);
				}
			}
			break;
		}

	case EHitActorType::AIBoss:
		{
			for (int i = 0; i < mShortGunTrace; ++i)
			{
				if(mShotGunHitResultsArr[i].bBlockingHit)
				{
					UGameplayStatics::SpawnSoundAttached(mSoundBossArmorHit, GetOwningPlayer()->GetRootComponent());
				}
			}
			break;
		}
	}

	mNetHitActorType = EHitActorType::None;
}

void AWeaponShotGun::Reload()
{
	Super::Reload();
}

void AWeaponShotGun::UpdateShotgunHitResults(TArray<FHitResult>& HitResults)
{
	for (int i = 0; i < HitResults.Num(); i++)
	{
		mShotGunHitResultsArr[i] = HitResults[i];
	}
}

void AWeaponShotGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWeaponShotGun, mShotGunHitResults);
}
