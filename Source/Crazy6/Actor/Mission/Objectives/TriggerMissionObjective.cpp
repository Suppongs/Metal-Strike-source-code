// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerMissionObjective.h"

#include "EngineUtils.h"
#include "AIController.h"

#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Crazy6/Global/ProfileInfo.h"
#include "Crazy6/Player/PlayerBase.h"
#include "Crazy6/Widget/CinematicPlayWidget.h"


ATriggerMissionObjective::ATriggerMissionObjective()
{
	PrimaryActorTick.bCanEverTick = true;
	
	mTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerComponent"));
	mTrigger->SetCollisionProfileName(UE_COLLISION_PROFILE_EVENTTRIGGER);
	RootComponent = mTrigger;
	OffCollision();

	mStaticConeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConeStaticMesh"));
	mStaticConeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mStaticConeMesh->SetupAttachment(mTrigger);
	mStaticConeMesh->SetRelativeLocation(FVector(0.0, 0.0, 80.0));
	mStaticConeMesh->SetRelativeRotation(FRotator(-90.0, 0.0, 0.0));
	mStaticConeMesh->SetRelativeScale3D(FVector(0.5, 0.5, 2.0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Dev/Level/GC/Cone.Cone'"));
	if (true == MeshAsset.Succeeded())
	{
		mStaticConeMesh->SetStaticMesh(MeshAsset.Object);
	}

	mStaticConeMesh->SetVisibility(false);

	//SetRelativeLocation(FVector(17789.289074f, 1381.999262f, 487.360291f));
	//SetRelativeScale3D(FVector(-6.324279f, -5.648526f, -7.668319f));

	mTrigger->SetRelativeRotation(FRotator(180.0f, 0.0f, 180.0f));
	mTrigger->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	mTrigger->SetBoxExtent(FVector(50.856385f, 67.640144f, 45.003978f));


	// ======= Widget =======

	static ConstructorHelpers::FClassFinder<UCinematicPlayWidget>
		UIAsset(TEXT("/Game/Dev/UI/Cinematic/CinematicPlayerWidget.CinematicPlayerWidget_C"));

	if (UIAsset.Succeeded())
	{
		mCinematicWidgetClass = UIAsset.Class;
	}

}

void ATriggerMissionObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurRotator = mStaticConeMesh->GetRelativeRotation();
	CurRotator += FRotator(0.0f,0.0f,  400.f * DeltaTime);
	mStaticConeMesh->SetRelativeRotation(CurRotator);
}

void ATriggerMissionObjective::BeginPlay()
{
	Super::BeginPlay();

	FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));
	if (CurrentLevelName == "Bridge_P")
	{
		mStaticConeMesh->SetStaticMesh(nullptr);
	}

	else
	{
		mStaticConeMesh->SetIsReplicated(true);
	}
	


	mTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATriggerMissionObjective::OnBeginOverlap);



}

void ATriggerMissionObjective::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

void ATriggerMissionObjective::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void ATriggerMissionObjective::OnCollision()
{
	mTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ATriggerMissionObjective::OffCollision()
{
	mTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATriggerMissionObjective::StartObjective()
{
	Super::StartObjective();

	mStaticConeMesh->SetVisibility(true);

	OnCollision();


}

void ATriggerMissionObjective::EndObjective()
{
	Super::EndObjective();
}

void ATriggerMissionObjective::CallWidgetFunc_Implementation()
{
	mCinematicWidget = CreateWidget<UCinematicPlayWidget>(GetWorld(), mCinematicWidgetClass);

	if (IsValid(mCinematicWidget))
	{
		mCinematicWidget->AddToViewport();

	}

	for (APlayerBase* Character : TActorRange<APlayerBase>(GetWorld()))
	{
		Character->DisableInput(nullptr);
		if (HasAuthority())
		{
			Character->ResetPlayerStatus();
		}
	}
}

void ATriggerMissionObjective::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		mStaticConeMesh->SetVisibility(false);

		bActive = false;
		Complete();



		FName CurLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));

		if (CurLevelName == "Bridge_P")
		{
			CallWidgetFunc();
		}
	}

	OffCollision();





}

