// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBulletWithinPool.h"
#include "BulletPool.h"
#include "HumanoidGunMan.h"
#include "Components/SphereComponent.h"
#include "Crazy6/Global/ProfileInfo.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AEnemyBulletWithinPool::AEnemyBulletWithinPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	// <======= HitBox Setting =======>
	mBulletHitBox = CreateDefaultSubobject<USphereComponent>(TEXT("BulletHitBox"));
	SetRootComponent(mBulletHitBox);
	mBulletHitBox->SetSphereRadius(3.f);
	mBulletHitBox->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMYBULLET);

	mBulletHitBox->SetCanEverAffectNavigation(false);

	// <======= Movement Setting =======>
	switch (GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			mMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
			mMovement->SetUpdatedComponent(mBulletHitBox);
			mMovement->InitialSpeed = 10000.f;
			mMovement->ProjectileGravityScale = 0.f;

			ResetBulletRange();
		}
		break;
	default:
		break;
	}

	// <======= Particle Setting =======>
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		Particle(TEXT("/Script/Engine.ParticleSystem'/Game/Dev/AI/Enemy/Humanoid/FX/P_Wraith_Primary_Trail.P_Wraith_Primary_Trail'"));
	if (Particle.Succeeded())
	{
		mBulletTrailSource = Particle.Object;
	}

	mBulletTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BulletTrail"));
	mBulletTrail->SetupAttachment(mBulletHitBox);
	mBulletTrail->SetTemplate(mBulletTrailSource);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		HitParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Dev/AI/Enemy/Humanoid/FX/P_Wraith_Primary_HitCharacter.P_Wraith_Primary_HitCharacter'"));
	if (HitParticle.Succeeded())
	{
		mTargetHitParticle = HitParticle.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		BlockParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Dev/AI/Enemy/Humanoid/FX/P_Wraith_Primary_HitCharacter.P_Wraith_Primary_HitCharacter'"));
	if (BlockParticle.Succeeded())
	{
		mBlockHitParticle = BlockParticle.Object;
	}

	// <======= Sound Setting =======>
	static ConstructorHelpers::FObjectFinder<USoundBase>
		BulletSound(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Humanoid/FX/Sound/Wave/laser-gun-blast.laser-gun-blast'"));
	if (BulletSound.Succeeded())
	{
		mBulletSound = BulletSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>
		BulletBlockSound(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Humanoid/FX/Sound/Wave/bullet-ricochet.bullet-ricochet'"));
	if (BulletBlockSound.Succeeded())
	{
		mBulletBlockSound = BulletBlockSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>
		BulletHitSound(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Humanoid/FX/Sound/Wave/bullet-hit.bullet-hit'"));
	if (BulletHitSound.Succeeded())
	{
		mBulletHitSound = BulletHitSound.Object;
	}
	
	// <======= Bullet Overlap Setting =======>
	switch (GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			mMovement->OnProjectileStop.AddDynamic(this, &AEnemyBulletWithinPool::On_ProjectileStop_Bullet);
			mBulletHitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBulletWithinPool::OnBegineOverlap_EnemyBullet);
		}
		break;
	default:
		break;
	}
}

// Called when the game starts or when spawned
void AEnemyBulletWithinPool::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);
	mBulletTrail->SetIsReplicated(true);
}

// Called every frame
void AEnemyBulletWithinPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			if (true == bActive)
			{
				mCurLocation = GetActorLocation();
				FVector Dir = mCurLocation - mPrevLocation;
				float Distance = Dir.Length();
				mAccumulatedBulletRange -= Distance;
				if (mAccumulatedBulletRange <= 0.f)
				{
					SelfReturnToPool();
				}
				mPrevLocation = mCurLocation;
			}
		}
		break;
	default:
		break;
	}
}

void AEnemyBulletWithinPool::SetActive(bool _IsActive)
{
	bActive = _IsActive;
	if (true == bActive)
	{
		mBulletTrail->SetRelativeRotation(mMovement->Velocity.Rotation());
		MulticastBulletSoundFX();
	}
	else
	{
		mMovement->SetUpdatedComponent(nullptr);
	}
	
	SetActorHiddenInGame(!bActive);
}

void AEnemyBulletWithinPool::SelfReturnToPool()
{	
	if(false == IsValid(mBelongedPool))
	{
		return;
	}
	
	SetActive(false);
	
	mBelongedPool->ObjectReturnToPool(this);
}

void AEnemyBulletWithinPool::MulticastBulletSoundFX_Implementation()
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mBulletSound, this->GetActorLocation());
}

void AEnemyBulletWithinPool::MulticastBulletStopFX_Implementation(const FHitResult& HitResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mBlockHitParticle,
			HitResult.Location, FRotator::ZeroRotator, true);

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mBulletBlockSound, HitResult.Location);
}

void AEnemyBulletWithinPool::MulticastTargetHitFX_Implementation(AActor* OtherActor)
{
	if (nullptr == OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("***Enemy Bullet Whithin Pool Target Hit FX Error***"));
	}
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mTargetHitParticle,
		OtherActor->GetActorLocation(), FRotator::ZeroRotator, true);

	UGameplayStatics::SpawnSoundAttached(mBulletHitSound, OtherActor->GetRootComponent());
}

void AEnemyBulletWithinPool::On_ProjectileStop_Bullet(const FHitResult& HitResult)
{
	switch (GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			MulticastBulletStopFX(HitResult);
		}
		break;
	default:
		break;
	}
	
	SelfReturnToPool();
}

void AEnemyBulletWithinPool::OnBegineOverlap_EnemyBullet(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	FDamageEvent	DmgEvent;

	if (true == IsValid(mOwner))
	{
		OtherActor->TakeDamage(mOwner->GetAttack(), DmgEvent, mOwner->GetController(), this);

		switch (GetNetMode())
		{
		case NM_Standalone:
		case NM_ListenServer:
			{
				MulticastTargetHitFX(OtherActor);
			}
			break;
		default:
			break;
		}

		SelfReturnToPool();
	}
}

