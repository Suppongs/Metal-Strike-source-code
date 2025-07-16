// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "AIController.h"
#include "../../Actor/Enemy/EnemyBase.h"
#include "Crazy6/Actor/Enemy/Boss/BossZero.h"
#include "../../../Crazy6/Widget/BulletWidget.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

#include "Crazy6/Actor/Buidling/CacheTriggerBox.h"
#include "PlayerWeaponSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BulletProjectile.h"

#include "KismetTraceUtils.h"
#include "PlayerMasterWeapon.h"

// Sets default values
ABulletProjectile::ABulletProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// mDefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	mSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticle"));
	mProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	static	ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Dev/Player/Human/Weapon/RPG/RPG7_rocket.RPG7_rocket'"));
	
	if (MeshAsset.Succeeded())
	{
		mStaticMesh->SetStaticMesh(MeshAsset.Object);
	}
	SetRootComponent(mSphere);
	mStaticMesh->SetupAttachment(mSphere);
	mExplosionParticle->SetupAttachment(mSphere);

	if (HasAuthority())
	{
		mSphere->OnComponentHit.AddDynamic(this, &ABulletProjectile::OnBulletHit);
		mExplosionParticle->OnSystemFinished.AddDynamic(this, &ABulletProjectile::OnParticleFinished);
	}
	mSphere->SetCollisionProfileName("PlayerAttack");
	mSphere->SetCanEverAffectNavigation(false);
	mSphere->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	mSphere->CanCharacterStepUpOn = ECB_No;

	mExplosionParticle->bAutoActivate = false;
	mExplosionParticle->bAutoManageAttachment = true;
	mProjectileMovement->InitialSpeed = mProjectileInitialSpeed;
	mProjectileMovement->ProjectileGravityScale = 1.0f;
	mProjectileMovement->bInitialVelocityInLocalSpace = true;
	mProjectileMovement->MaxSpeed = mProjectileMaxSpeed;
	mProjectileMovement->bRotationFollowsVelocity = true;
	
	InitialLifeSpan = 5.0f;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
}

// Called every frame
void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABulletProjectile::ShootRPGRocket(const FVector& CameraLocation, const FRotator& CameraRotation, const float& FireRange) const
{
	// < Camera >
	FVector CameraStart = CameraLocation;
	FVector CameraEnd = CameraStart + (CameraRotation.Vector() * FireRange);

	mProjectileMovement->SetVelocityInLocalSpace((CameraEnd-CameraStart).GetSafeNormal() * mProjectileInitialSpeed);
}

void ABulletProjectile::OnBulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                    FVector NormalImpulse, const FHitResult& Hit)
{
	mSphere->SetVisibility(false, true);
	mSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// mRocketParticle->Deactivate();

	if (HasAuthority())
	{
		if (OtherActor && OtherActor != this)
		{
			if (mOwningWeapon)
			{
				mOwningWeapon->UpdateHitResults(Hit);
				mOwningWeapon->CheckHitActorType();
				mOwningWeapon->FireEffectsMulticast(true);
				Explode();
				PlayExplosionEffects(Hit.GetActor()->GetRootComponent(), Hit.Location, Hit.ImpactNormal);
			}
		}
	}
}

void ABulletProjectile::Explode()
{
	// Server
	TArray<FHitResult>	pRocketHitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	float ShapeRadius = 500.f;

	FVector Start = GetActorLocation();
	FVector End = Start;
	
	bool IsHit = GetWorld()->SweepMultiByChannel(pRocketHitResult, Start, End,  FQuat::Identity, ECC_GameTraceChannel5, FCollisionShape::MakeSphere(ShapeRadius), CollisionParams);
	// DrawDebugSphere(GetWorld(), Start, ShapeRadius, 12, FColor::Red, false, 2.f);	

	if (IsHit)
	{
		for (const FHitResult& Hit : pRocketHitResult)
		{
			if (Hit.GetActor())
			{
				AEnemyBase* HitAI = Cast<AEnemyBase>(Hit.GetActor());
				if (HitAI)
				{
					mOwningWeapon->ApplyDamage(Hit.GetActor(), mOwningWeapon->GetFireDamage());
				}
			}
		}	
	}
}

void ABulletProjectile::PlayExplosionEffects_Implementation(USceneComponent* HitComponent, const FVector& ImpactLocation, const FVector& HitNormal) const
{
	mExplosionParticle->Activate();
	ExplosionSoundFX(ImpactLocation);
}

void ABulletProjectile::ExplosionSoundFX(const FVector& ImpactLocation) const
{
	if (IsValid (mExplosionSoundBase))
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mExplosionSoundBase, ImpactLocation);
	}
}

void ABulletProjectile::OnParticleFinished(UParticleSystemComponent* FinishedComponent)
{
	Destroy();
}