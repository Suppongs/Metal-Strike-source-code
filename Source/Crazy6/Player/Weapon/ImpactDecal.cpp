// Fill out your copyright notice in the Description page of Project Settings.

#include "ImpactDecal.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstance.h"

// Sets default values
AImpactDecal::AImpactDecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mDefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	mDecal1 = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal1"));
	mDecal2 = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal2"));

	SetRootComponent(mDefaultScene);

	mDecal1->SetupAttachment(mDefaultScene);
	mDecal2->SetupAttachment(mDefaultScene);

	mDecal2->SetWorldScale3D({1.3, 1.3, 1.3});

	mDecal1->DecalSize = { 256.0, 256.0, 256.0 };
	mDecal2->DecalSize = { 256.0, 256.0, 256.0 };

	mDecal1->FadeDuration = 3.0f;
	mDecal2->FadeDuration = 3.0f;

	mDecal1->bDestroyOwnerAfterFade = true;
	mDecal2->bDestroyOwnerAfterFade = true;

	static	ConstructorHelpers::FObjectFinder<UMaterialInstance>
		Decal1Asset1(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Dev/Player/Weapons/Decal/Effects/Materials/MI_Decal_Bullet_Concrete.MI_Decal_Bullet_Concrete'"));

	if (Decal1Asset1.Succeeded())
	{
		mDecal1->SetDecalMaterial(Decal1Asset1.Object);
	}

	static	ConstructorHelpers::FObjectFinder<UMaterialInstance>
		Decal1Asset2(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Dev/Player/Weapons/Decal/Effects/Materials/MI_Decal_Punch_Concrete.MI_Decal_Punch_Concrete'"));

	if (Decal1Asset2.Succeeded())
	{
		mDecal2->SetDecalMaterial(Decal1Asset2.Object);
	}

	bReplicates = true;
}

// Called when the game starts or when spawned
void AImpactDecal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AImpactDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AImpactDecal::IncreaseDecalScale()
{
	SetActorScale3D({3.0f, 3.0f, 3.0f});
}

