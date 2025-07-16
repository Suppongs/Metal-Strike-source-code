// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanoidSwordMan.h"
#include "Components/BoxComponent.h"
#include "Crazy6/Global/ProfileInfo.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

AHumanoidSwordMan::AHumanoidSwordMan()
{
	/* <=========== Anim Setting ===========> */
	static ConstructorHelpers::FClassFinder<UAnimInstance>
			AnimObject(TEXT("/Script/Engine.AnimBlueprint'/Game/Dev/AI/Enemy/Humanoid/ABP_HumanoidSwordMan.ABP_HumanoidSwordMan_C'"));
	if (true == AnimObject.Succeeded())
	{
		mMesh->SetAnimInstanceClass(AnimObject.Class);
	}
	/* <=========== Key Setting ===========> */
	mEnemyKey = TEXT("HumanoidSwordMan");


	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Dev/AI/Enemy/Humanoid/SimpleSwordPack/Meshes/SM_Sword_05.SM_Sword_05'"));
	if (true == MeshAsset.Succeeded())
	{
		mWeaponMesh->SetStaticMesh(MeshAsset.Object);
		mWeaponMesh->SetupAttachment(mMesh, TEXT("Humanoid_Sword_Socket"));
	}

	/* <=========== Sword HitBox Setting ===========> */
	mSwordHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordHitBox"));
	mSwordHitBox->SetupAttachment(mMesh, TEXT("Humanoid_Sword_Socket"));
	mSwordHitBox->SetBoxExtent(FVector(3.0, 2.0, 33.5));
	mSwordHitBox->SetRelativeLocation(FVector(0.0, 0.0, 26.5));
	mSwordHitBox->SetCollisionProfileName(UE_COLLISION_PROFILE_ENEMYATTACK);
	mSwordHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// <======= Particle Setting =======>
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		HitParticle(TEXT("/Script/Engine.ParticleSystem'/Game/Dev/AI/Enemy/Humanoid/FX/P_Minion_Impact_Default.P_Minion_Impact_Default'"));
	if (HitParticle.Succeeded())
	{
		mHitParticle = HitParticle.Object;
	}

	// <======= Sound Setting =======>	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		HitSound(TEXT("/Script/Engine.SoundWave'/Game/Dev/AI/Enemy/Humanoid/FX/Sound/Wave/breeze-of-blood.breeze-of-blood'"));
	if (HitSound.Succeeded())
	{
		mSwordHitSound = HitSound.Object;
	}

	/* <=========== Sword OverLap Setting ===========> */
	mSwordHitBox->OnComponentBeginOverlap.AddDynamic(this,
		&AHumanoidSwordMan::OnBegineOverlap_Sword);
}

void AHumanoidSwordMan::AttackHitBoxOn()
{
	Super::AttackHitBoxOn();

	mSwordHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AHumanoidSwordMan::AttackHitBoxOff()
{
	Super::AttackHitBoxOff();

	mSwordHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHumanoidSwordMan::OnBegineOverlap_Sword(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	FDamageEvent	DmgEvent;
	OtherActor->TakeDamage(GetAttack(), DmgEvent, GetController(),this);

	MulticastSwordHitFX(OtherComp);
}

void AHumanoidSwordMan::MulticastSwordHitFX_Implementation(UPrimitiveComponent* OtherComp)
{
	UGameplayStatics::SpawnEmitterAttached(mHitParticle, OtherComp);

	UGameplayStatics::SpawnSoundAttached(mSwordHitSound, OtherComp);
}


