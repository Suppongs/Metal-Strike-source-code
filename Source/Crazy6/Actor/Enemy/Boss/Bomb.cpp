// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "BossZero.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Engine/DamageEvents.h"

ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	// <======= HitBox Setting =======>
	mBombRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mBombRoot);
	mBombRoot->SetCanEverAffectNavigation(false);

	// <======= Visual Setting =======>
	mBombVisual = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BombVisual"));
	mBombVisual->SetupAttachment(mBombRoot);
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
			VisualParticleSystem(TEXT("/Script/Engine.ParticleSystem'/Game/Dev/AI/Enemy/Boss/BombFX/P_ConsGround_Bubble_Pop.P_ConsGround_Bubble_Pop'"));
	if (true == VisualParticleSystem.Succeeded())
	{
		mBombVisual->SetTemplate(VisualParticleSystem.Object);
	}
	mBombVisual->SetAutoActivate(false);

	// <======= Indicator Setting =======>
	mBombIndicator = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BombIndicator"));
	mBombIndicator->SetupAttachment(mBombRoot);
	mBombIndicator->SetRelativeScale3D(FVector(1.0, 1.0, 1.0));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
			IndicatorNiagaraSystem(TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/UI/AttackIndicator/NS_CircleIndicator.NS_CircleIndicator'"));
	if (true == IndicatorNiagaraSystem.Succeeded())
	{
		mBombIndicator->SetAsset(IndicatorNiagaraSystem.Object);
	}

	// <======= FX Setting =======>
	static ConstructorHelpers::FObjectFinder<USoundBase>
		LaunchSoundBase(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Boss/Sound/Bomb_Launch.Bomb_Launch'"));
	if (LaunchSoundBase.Succeeded())
	{
		mLaunchSound = LaunchSoundBase.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>
		ExplosionSoundBase(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Boss/Sound/Bomb_Explosion.Bomb_Explosion'"));
	if (ExplosionSoundBase.Succeeded())
	{
		mExplosionSound = ExplosionSoundBase.Object;
	}
}


void ABomb::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		mBombIndicator->OnSystemFinished.AddDynamic(this, &ABomb::OnIndicatorEnd);
		mBombVisual->OnSystemFinished.AddDynamic(this, &ABomb::OnExplosionEnd);
	}

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mLaunchSound, this->GetActorLocation());
}

void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABomb::OnIndicatorEnd(UNiagaraComponent* FinishedComponent)
{
	mOwner->MulticastCameraShake();
	MulticastExplosionFX();
	
	TArray<FHitResult> mBombHitResults;
	FVector Center = GetActorLocation();
	float BombHitBoxRadius = 240.f;
	bool IsHit = GetWorld()->SweepMultiByChannel(mBombHitResults, Center, Center, FQuat::Identity, ECC_GameTraceChannel4, FCollisionShape::MakeSphere(BombHitBoxRadius));
	if (true == IsHit)
	{
		for (const FHitResult& HitResult : mBombHitResults)
		{
			ACharacter* Player = Cast<ACharacter>(HitResult.GetActor());
			if (true == IsValid(Player))
			{
				FDamageEvent DmgEvent;
				Player->TakeDamage(mOwner->GetAttack(), DmgEvent, mOwner->GetController(), this);

				FVector LaunchDirection = Player->GetActorLocation() - GetActorLocation();
				Player->LaunchCharacter((LaunchDirection.GetSafeNormal() + Player->GetActorUpVector()) * 500.0, true, true);
			}
		}	
	}
}

void ABomb::OnExplosionEnd(UParticleSystemComponent* FinishedComponent)
{
	this->Destroy();
}

void ABomb::MulticastExplosionFX_Implementation()
{
	mBombVisual->Activate();

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), mExplosionSound, this->GetActorLocation());
}


