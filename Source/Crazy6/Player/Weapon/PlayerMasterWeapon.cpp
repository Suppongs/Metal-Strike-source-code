#include "PlayerMasterWeapon.h"

#include "ActiveSound.h"
#include "PlayerWeaponSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

#include "AIController.h"
#include "BulletProjectile.h"
#include "../../Actor/Enemy/EnemyBase.h"

#include "BulletTrace.h"
#include "ImpactDecal.h"
#include "../../../Crazy6/Widget/BulletWidget.h"

#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Field/FieldSystemActor.h" 
#include "Crazy6/Actor/Buidling/CacheTriggerBox.h"

#include "../PlayerBase.h"
#include "Crazy6/Actor/Enemy/Boss/BossZero.h"
#include "Crazy6/Player/LyraPlayerAnimInstance.h"

APlayerMasterWeapon::APlayerMasterWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	mNetWeaponSystem = CreateDefaultSubobject<UPlayerWeaponSystem>(TEXT("PlayerWeaponSystem"));
	mWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	mDefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));

	SetRootComponent(mDefaultSceneComponent);
	mWeaponMesh->SetupAttachment(mDefaultSceneComponent);

	mMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleSceneComponent"));
	mMuzzle->SetupAttachment(mWeaponMesh);

	// Boss Armor Hit Sound
	static ConstructorHelpers::FObjectFinder<USoundWave>
		ArmorHitSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Boss/Sound/Metal_Hit.Metal_Hit'"));

	if (ArmorHitSoundAsset.Succeeded())
	{
		mSoundBossArmorHit = ArmorHitSoundAsset.Object;
	}	

	static ConstructorHelpers::FObjectFinder<USoundWave>
	EnemyHitSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Dev/Player/Sounds/AI_Bullet_Hit.AI_Bullet_Hit'"));
	
	if (EnemyHitSoundAsset.Succeeded())
	{
		mSoundEnemyHit = EnemyHitSoundAsset.Object;
	}
	
	// Chaos
	static ConstructorHelpers::FClassFinder<AFieldSystemActor> MasterFieldBP(TEXT("/Script/Engine.Blueprint'/Game/Dev/Level/GC/FS_MasterField.FS_MasterField_C'"));
	if (MasterFieldBP.Succeeded())
	{
		mMasterFieldActor = (UClass*)MasterFieldBP.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>
	BuildingHitSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Dev/Player/Sounds/Explosion_Big.Explosion_Big'"));
	
	if (EnemyHitSoundAsset.Succeeded())
	{
		mBuildingImpactSoundBase = BuildingHitSoundAsset.Object;
	}

	bReplicates = true;
	bAlwaysRelevant = true;
}

void APlayerMasterWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerMasterWeapon::SetOwningPlayer(APlayerBase* Player)
{
	mNetOwnerPlayer = Player;
	SetOwner(Player);
}

void APlayerMasterWeapon::BeginPlay()
{
	Super::BeginPlay();
	//Bullet
	if (IsValid(mBulletWidgetClass))
	{
		mBulletWidget = CreateWidget<UBulletWidget>(GetWorld(), mBulletWidgetClass);
	}

	if (HasAuthority())
	{
		// SetReplicates(true);
		mNetWeaponSystem->SetIsReplicated(true);
		mWeaponMesh->SetIsReplicated(true);
	}
}

void APlayerMasterWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerMasterWeapon, mNetWeaopnDetails);
	DOREPLIFETIME(APlayerMasterWeapon, mNetWeaponSystem);
	DOREPLIFETIME(APlayerMasterWeapon, mNetHitActorType);
	DOREPLIFETIME(APlayerMasterWeapon, mNetOwnerPlayer);
	DOREPLIFETIME(APlayerMasterWeapon, bNetWeaponIsReload);
	DOREPLIFETIME(APlayerMasterWeapon, bNetIsAMode);

	DOREPLIFETIME(APlayerMasterWeapon, mNetCameraEnd);
	DOREPLIFETIME(APlayerMasterWeapon, mNetCameraHitActor);
	DOREPLIFETIME(APlayerMasterWeapon, mNetCameraHitLocation);
	DOREPLIFETIME(APlayerMasterWeapon, mNetCameraHitImpactNormal);
	DOREPLIFETIME(APlayerMasterWeapon, mNetCameraHitComponent);
}

// ==================== [ Combat ] ====================
void APlayerMasterWeapon::FireStart()
{
	/* DamageSetting, PlayTimer */
}

void APlayerMasterWeapon::Fire()
{
	/* Use one bullet */
	// Fire or Dry
}

void APlayerMasterWeapon::FireStop()
{
	/* Delete FireTimerHandle */
	GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
}

void APlayerMasterWeapon::Reload()
{
	/* Server */
	if (!mNetWeaponSystem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("!! APlayerMasterWeapon->Reload() : No mWeaponSystem !!")));
		return;
	}

	if (!mPlayerReloadMontage
		|| !mWeaponReloadMontage)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("!! APlayerMasterWeapon : No Animation !!")));
		return;
	}

	ReloadAmmo();
	ReloadEffectsMulticast();
}

void APlayerMasterWeapon::UpdateHitResults(const FHitResult& HitResults)
{
	mNetCameraHitActor = HitResults.GetActor();
	mNetCameraEnd = HitResults.bBlockingHit ? HitResults.ImpactPoint : HitResults.TraceEnd;
	mNetCameraHitLocation = HitResults.Location;
	mNetCameraHitImpactNormal = HitResults.ImpactNormal;
	mNetCameraHitComponent = HitResults.GetComponent();
}

/* Server OR Client */
void APlayerMasterWeapon::ChangeABMode(bool IsAMode)
{
	if (HasAuthority())
	{
		bNetIsAMode = IsAMode;
	}
	else
	{
		ChangeABModeServer(IsAMode);
	}
}

void APlayerMasterWeapon::ChangeABModeServer_Implementation(bool IsAMode)
{
	bNetIsAMode = IsAMode;
}

void APlayerMasterWeapon::ShootLineTrace()
{
	FHitResult	pCameraHitResult;
	TArray<AActor*> LineTraceIgnoreActors;
	LineTraceIgnoreActors.Add(GetOwner());
	UWorld* World = GetWorld();

	double FireRange = bNetIsAMode? mAModeFireRange : mBModeFireRange;
	double BaseSpreadAngle = bNetIsAMode? mAModeBaseSpreadAngle : mBModeBaseSpreadAngle;
	double MaxSpreadAngle = bNetIsAMode? mAModeMaxSpreadAngle : mBModeMaxSpreadAngle;
	double SpreadIncreaseRate = bNetIsAMode? mAModeSpreadIncreaseRate : mBModeSpreadIncreaseRate;
	
	if (!mNetOwnerPlayer)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
			FString::Printf(TEXT("!! AWeaponRifle::ShootLineTrace() : mOwnerPlayer is NULL !!")));
		return;
	}

	if (nullptr == World)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
			FString::Printf(TEXT("!! AWeaponRifle::ShootLineTrace() : GetWorld returns nullptr !!")));
		return;
	}

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

	/* Gun Recoil */
	// if (true == bNetIsAMode)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		float TimeSinceStart = CurrentTime - mFireStartTime;

		float SpreadAngle = FMath::Min(MaxSpreadAngle, BaseSpreadAngle + (TimeSinceStart * SpreadIncreaseRate));

		float RandomPitch = FMath::FRandRange(-SpreadAngle, SpreadAngle);
		float RandomYaw = FMath::FRandRange(-SpreadAngle, SpreadAngle);

		FRotator FinalAimDirection = CameraRotation;
		FinalAimDirection.Pitch += RandomPitch; 
		FinalAimDirection.Yaw += RandomYaw;
		
		CameraEnd = CameraStart + (CameraRotation.Vector() + FinalAimDirection.Vector() * FireRange);
	}

	UKismetSystemLibrary::LineTraceSingle(World, CameraStart, CameraEnd, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel5)
	, true, LineTraceIgnoreActors, EDrawDebugTrace::None, pCameraHitResult, true, FLinearColor::Blue, FLinearColor::Green, 5.0f);

	UpdateHitResults(pCameraHitResult);
	/* CheckHitType + ApplyDamage*/
	CheckHitActorType();
}

void APlayerMasterWeapon::ShootProjectile()
{
	if (!IsValid(mBulletProjectileClass)
		|| !IsValid(mWeaponMesh))
	{
		return;
	}
	FVector WeaponStart = mWeaponMesh->GetSocketLocation("Muzzle");
	FVector StartLocation = mMuzzle->GetComponentLocation();
	FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(WeaponStart, UKismetMathLibrary::MakeRotFromX(WeaponStart.ForwardVector));

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	FVector CameraLocation;
	FRotator CameraRotation;

	if (PlayerController)
	{
		// Get the Client's camera position and direction values
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Instigator = GetInstigator();
	
	ABulletProjectile* Rocket = Cast<ABulletProjectile>(GetWorld()->SpawnActor<AActor>(mBulletProjectileClass, SpawnTransform));
	if (Rocket)
	{
		Rocket->ShootRPGRocket(CameraLocation, CameraRotation, mAModeFireRange);
		Rocket->SetOwningWeapon(this); 
	}
}

void APlayerMasterWeapon::ApplyDamage(AActor* HitActor, float Damage)
{
	if (nullptr != HitActor
		&& nullptr != mNetOwnerPlayer
		&& 0.f < Damage)
	{
		UGameplayStatics::ApplyDamage(HitActor, Damage, mNetOwnerPlayer->GetController(), mNetOwnerPlayer, UDamageType::StaticClass());
		AEnemyBase* HitPawn = Cast<AEnemyBase>(HitActor);
		if (true == IsValid(HitPawn))
		{
			if (true == HasAuthority())
			{
				mNetOwnerPlayer->DamageUIClient(HitPawn);
			}
		}
	}
}

void APlayerMasterWeapon::CheckHitActorType()
{
	float FireDamage = GetFireDamage();
	
	// ========== Hit Check ==========
	// No Hit
	if (nullptr == mNetCameraHitActor)
	{
		mNetHitActorType = EHitActorType::None;
		return;
	}

	// Hit Building (Chaos)
	ACacheTriggerBox* BuildingTrigger = Cast<ACacheTriggerBox>(mNetCameraHitActor);
	if (nullptr != BuildingTrigger && nullptr != BuildingTrigger->FindComponentByClass<UBoxComponent>())
	{
		mNetHitActorType = EHitActorType::Building;

		if (true == BuildingTrigger->IsTriggerPlayEvent())
			return;

		
		// ====== Test 
		// Chaos Chaced Play From Server
		if (BuildingTrigger->IsDeath())
		{
			FOutputDeviceNull Ar;
			mNetCameraHitActor->CallFunctionByNameWithArguments(TEXT("NetPlayCache"), Ar, nullptr, true);
			BuildingTrigger->SetTriggerPlayEvent(true);
		}
		else
		{
			ApplyDamage(mNetCameraHitActor, FireDamage);
		}
	}

	APawn* HitPawn = Cast<APawn>(mNetCameraHitActor);
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
		bool IsHeadShot = mNetCameraHitComponent->IsA(UBoxComponent::StaticClass());
		float fDamage = FireDamage;
		if (true == IsHeadShot
			&& IsValid(Cast<ABossZero>(mNetCameraHitActor)))
		{
			mNetHitActorType = EHitActorType::AIBoss;
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
		ApplyDamage(mNetCameraHitActor, fDamage);
	}
}

bool APlayerMasterWeapon::CanReload() const
{
	if (0 < mNetCurrentMaxAmmo
		&& (mNetCurrentAmmo < mClipAmmo))
	{
		return true;
	}
	return false;
}

bool APlayerMasterWeapon::CanGetMagazine() const
{
	if (mNetCurrentMaxAmmo < mDefaultMaxAmmo)
	{
		return true;
	}
	return false;
}

void APlayerMasterWeapon::AddMagazine()
{
	/* Server */
	if (mDefaultMaxAmmo >= (mNetCurrentMaxAmmo+ mClipAmmo))
	{
		mNetCurrentMaxAmmo += mClipAmmo;
	}
	else
	{
		mNetCurrentMaxAmmo = mDefaultMaxAmmo;
	}

	WeaponUIReloadingMulticast();
}

bool APlayerMasterWeapon::FireAmmo(int32 AmmoCount)
{
	if (0 <= mNetCurrentAmmo - AmmoCount)
	{
		mNetCurrentAmmo -= AmmoCount;
		return true;
	}
	return false;
}

void APlayerMasterWeapon::ReloadAmmo()
{
	//if (mOwnerPlayer && mOwnerPlayer->GetmNetWeaponInstance())
	{
		if (0 < mNetCurrentMaxAmmo)
		{
			// Bullet X
			if (0 == mNetCurrentAmmo)
			{
				if (mNetCurrentMaxAmmo >= mClipAmmo)
				{
					mNetCurrentAmmo = mClipAmmo;
					mNetCurrentMaxAmmo -= mClipAmmo;
				}
				else
				{
					mNetCurrentAmmo = mNetCurrentMaxAmmo;
					mNetCurrentMaxAmmo = 0;
				}
			}

			// Bullet O
			else
			{
				if (mNetCurrentMaxAmmo >= mClipAmmo)
				{
					mNetCurrentMaxAmmo -= (mClipAmmo - mNetCurrentAmmo);
					mNetCurrentAmmo = mClipAmmo;
				}
				else
				{
					if ((mClipAmmo - mNetCurrentAmmo) > mNetCurrentMaxAmmo)
					{
						mNetCurrentAmmo += mNetCurrentMaxAmmo;
						mNetCurrentMaxAmmo = 0;
					}
					else
					{
						mNetCurrentAmmo = mClipAmmo;
						mNetCurrentMaxAmmo -= (mClipAmmo - mNetCurrentAmmo);
					}
				}
			}
			// FireAmmo(0);
		}
	}
}

// ==================== [ Effects ] ====================
TObjectPtr<USkeletalMesh> APlayerMasterWeapon::GetWeaponMesh() const
{
	if (mWeaponMesh)
	{
		return mWeaponMesh->GetSkeletalMeshAsset();
	}
	return nullptr;
}

/* Entire Effects : Fire or Dry Effects */
void APlayerMasterWeapon::FireEffectsMulticast_Implementation(bool WeaponCanFire)
{
	/* Fire : Ammo O */
	if (true == WeaponCanFire)
	{
		if (!mNetOwnerPlayer)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
				FString::Printf(TEXT("!! APlayerMasterWeapon::FireEffectsMulticast : mOwnerPlayer is NULL !!")));
			return;
		}
		
		PlayFireEffects();
		
		/* LocalPlayer Effects : CameraShake, WeaponUI */
		APlayerController* PlayerController = Cast<APlayerController>(mNetOwnerPlayer->GetController());
		if (PlayerController)
		{
			if (PlayerController->IsLocalPlayerController())
			{
				PlayerController->PlayerCameraManager->StartCameraShake(mCameraShakeClass);
				WeaponUIReloading();
			}
		}
	}
	/* Dry : Ammo X */
	else
	{
		PlayDryEffects();
	}
}

/* Use : Rifle, MG, RPG, Sniper */
/* Override : ShotGun */
void APlayerMasterWeapon::PlayFireEffects()
{
	if (!IsValid(mWeaponMesh))
	{
		return;
	}

	FVector WeaponStart = mWeaponMesh->GetSocketLocation("Muzzle");
	if (FVector::ZeroVector == WeaponStart)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		FString::Printf(TEXT("!! APlayerMasterWeapon::ShootLineTrace() : Socket 'Muzzle' is ZeroVector !!")));
	}

	// ========== Common Effect ==========
	if (IsValid(mBulletTraceClass))
	{
		FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(WeaponStart, UKismetMathLibrary::MakeRotFromX(mNetCameraEnd - WeaponStart));
		GetWorld()->SpawnActor<AActor>(mBulletTraceClass, SpawnTransform);	
	}

	if (mNetOwnerPlayer && IsValid(mBulletFallSound))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), mBulletFallSound, mNetOwnerPlayer->GetMesh()->GetSocketLocation("foot_r"));
	}

	if (IsValid(mWeaponFireSound))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), mWeaponFireSound, WeaponStart);
	}

	if (IsValid(mMuzzleFireNS))
	{
		MuzzleVFX(mMuzzleFireNS, mMuzzle);
	}
	FireMontage(mPlayerFireMontage);
	mWeaponMesh->PlayAnimation(mWeaponFireMontage, false);

	// ========== Hit Effect ==========
	switch(mNetHitActorType)
	{
	case EHitActorType::None:
		{
			break;
		}

	case EHitActorType::Building:
	case EHitActorType::Actor:
	case EHitActorType::LandScape:
		{
			if (IsValid(mBulletDecalClass))
			{
				ImpactDecal(mBulletDecalClass, mNetCameraHitLocation, mNetCameraHitImpactNormal, { 0.01f, 0.03f, 0.03f });
			}
			if (IsValid(mImpactSoundBase)
				&& IsValid(mImpactATT)
				&& IsValid(mImpactSCON))
			{
				FireImpactFX(mImpactSoundBase, mNetCameraHitLocation, mImpactATT, mImpactSCON);
				// FireImpactFX(mImpactSoundBase, mNetCameraHitLocation, mImpactATT, mImpactSCON);
			}
			if (IsValid(mImpactNS))
			{
				ImpactNiagaraVFX(mImpactNS, mNetCameraHitLocation, mNetCameraHitImpactNormal);
			}
			
			break;
		}
		
	case EHitActorType::AI:
		{
			if (IsValid(mImpactNS))
			{ 
				ImpactNiagaraVFX(mImpactNS, mNetCameraEnd, mNetCameraHitImpactNormal);
			}
			if (IsValid(mWeaponFireSound))
			{
				//mSoundEnemyHit
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), mSoundEnemyHit, WeaponStart);
			}
			
			// if (IsValid(mSoundEnemyHit)
			// 	&& IsValid(mImpactATT)
			// 	&& IsValid(mImpactSCON))
			// {
			// 	if (GetWorld())
			// 	{	
			// 		UGameplayStatics::PlaySoundAtLocation(GetWorld(), mSoundEnemyHit, mNetCameraHitLocation, FRotator::ZeroRotator, 1.f, 1.f, 0.f, mImpactATT, mImpactSCON, this);
			// 		// FireImpactFX(mSoundEnemyHit, mNetCameraHitLocation, mImpactATT, mImpactSCON);
			// 	}
			// }
			break;
		}

	case EHitActorType::AIBoss:
		{
			if (IsValid(mImpactNS))
			{
				ImpactNiagaraVFX(mImpactNS, mNetCameraHitLocation, mNetCameraHitImpactNormal);
			}
			UGameplayStatics::SpawnSoundAttached(mSoundBossArmorHit, GetOwningPlayer()->GetRootComponent());
			break;
		}
	}
}

void APlayerMasterWeapon::PlayDryEffects()
{
	if (IsValid(mPlayerDryMontage))
	{
		EmptyMontage(mPlayerDryMontage);
	}
}

void APlayerMasterWeapon::ReloadEffectsMulticast_Implementation()
{
	if (!IsValid(mPlayerReloadMontage) || !IsValid(mWeaponReloadMontage))
	{
		return;
	}
	
	ReloadMontage(mPlayerReloadMontage);
	mWeaponMesh->PlayAnimation(mWeaponReloadMontage, false);
	
	/* Local Client */
	if (ROLE_AutonomousProxy)
	{
		WeaponUIReloading();
	}
}

void APlayerMasterWeapon::FireImpactFX(USoundBase* Sound, FVector& Location,
	USoundAttenuation* AttenuationSettings, USoundConcurrency* ConcurrencySettings) const
{
	if (IsValid(Sound) && IsValid(AttenuationSettings) && IsValid(ConcurrencySettings))
	{
		// UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location, FRotator::ZeroRotator,
		// 	1.f, 1.f, 0.f, AttenuationSettings, ConcurrencySettings);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);
		// UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, Location, UE::Math::TRotator<double>::ZeroRotator,
		// 	1.0f, 1.0f, 0.0f, AttenuationSettings, ConcurrencySettings, true);
	}
}

void APlayerMasterWeapon::MuzzleVFX(UNiagaraSystem* MuzzleNS, USceneComponent* AttachToComponent) const
{
	if (IsValid(MuzzleNS))
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleNS, AttachToComponent, TEXT("None"), FVector::ZeroVector,
		FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false, true, ENCPoolMethod::None, true);	
	}
}

void APlayerMasterWeapon::ImpactNiagaraVFX(UNiagaraSystem* ImpactNS, const FVector& ImpactLocation, const FVector& HitNormal) const
{
	if (IsValid(ImpactNS))
	{
		FRotator Rot = UKismetMathLibrary::Conv_VectorToRotator(HitNormal);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactNS, ImpactLocation, Rot, { 1.f, 1.f, 1.f });
	}
}

void APlayerMasterWeapon::FireMontage(UAnimMontage* FireAnim) const
{
	if (IsValid(FireAnim))
	{
		if (mNetOwnerPlayer && mNetOwnerPlayer->GetMesh()->GetAnimInstance())
		{
			mNetOwnerPlayer->GetMesh()->GetAnimInstance()->Montage_Play(FireAnim);
		}
	}
}

void APlayerMasterWeapon::ImpactDecal(UClass* DecalClass, const FVector_NetQuantize& Location, const FVector_NetQuantize& Normal, const FVector_NetQuantize& Scale) const
{
	if (IsValid(DecalClass))
	{
		FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(Location, UKismetMathLibrary::MakeRotFromX(Normal), Scale);
		
		FActorSpawnParameters SpawnParams;

		GetOwner()->GetWorld()->SpawnActor<AActor>(DecalClass, SpawnTransform);
	}
}

void APlayerMasterWeapon::ReloadMontage(UAnimMontage* ReloadAnim) const
{
	if (IsValid(ReloadAnim))
	{
		if (!mNetOwnerPlayer)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("!! UPlayerWeaponSystem : No mOwnerPlayer !!")));
			return;
		}

		if (!mNetOwnerPlayer->GetMesh()->GetAnimInstance())
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("!! UPlayerWeaponSystem : No GetAnimInstance !!")));
			return;
		}

		if (mNetOwnerPlayer && mNetOwnerPlayer->GetMesh()->GetAnimInstance())
		{
			mNetOwnerPlayer->GetMesh()->GetAnimInstance()->Montage_Play(ReloadAnim);
		}	
	}
}

void APlayerMasterWeapon::EmptyFX(USoundBase* Sound) const
{
	if (mNetOwnerPlayer)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, mNetOwnerPlayer->GetActorLocation());
	}
}

void APlayerMasterWeapon::EmptyMontage(UAnimMontage* EmptyAnim) const
{
	if (IsValid(EmptyAnim))
	{
		if (mNetOwnerPlayer && mNetOwnerPlayer->GetMesh()->GetAnimInstance()
		&& !(mNetOwnerPlayer->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage()))
		{
			mNetOwnerPlayer->GetMesh()->GetAnimInstance()->Montage_Play(EmptyAnim);
		}	
	}
}

// ==================== [ UI ] ====================
void APlayerMasterWeapon::InitalizeWeaponUI()
{
	if (mNetOwnerPlayer && mNetOwnerPlayer->IsLocallyControlled())
	{
		if (IsValid(mBulletWidget))
		{
			mBulletWidget->AddToViewport();
			WeaponUIReloading();
		}
	}	
}

void APlayerMasterWeapon::DestroyWeaponUI()
{
	if (mNetOwnerPlayer && mNetOwnerPlayer->IsLocallyControlled())
	{
		if (IsValid(mBulletWidget))
		{
			mBulletWidget->RemoveFromParent();
		}
	}
}

void APlayerMasterWeapon::WeaponUIReloading()
{
	if (mNetOwnerPlayer && mNetOwnerPlayer->IsLocallyControlled())
	{
		if (IsValid(mBulletWidget))
		{
			mBulletWidget->SetCurrentBullet(mNetCurrentAmmo);
			mBulletWidget->SetClipBullet(mClipAmmo);
			mBulletWidget->SetMaxBullet(mNetCurrentMaxAmmo);
		}
	}
}

void APlayerMasterWeapon::WeaponUIReloadingMulticast_Implementation()
{
	if (mNetOwnerPlayer && mNetOwnerPlayer->IsLocallyControlled())
	{
		if (IsValid(mBulletWidget))
		{
			mBulletWidget->SetCurrentBullet(mNetCurrentAmmo);
			mBulletWidget->SetClipBullet(mClipAmmo);
			mBulletWidget->SetMaxBullet(mNetCurrentMaxAmmo);
		}
	}
}
