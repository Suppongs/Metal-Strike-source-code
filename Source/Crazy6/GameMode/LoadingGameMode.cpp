// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingGameMode.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstance.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"
#include "../Widget/LoadingWidget.h"
#include  "../Controller/LoadingController.h"
#include "Kismet/GameplayStatics.h"
#include "../Setting/LoadingSettings.h"

ALoadingGameMode::ALoadingGameMode()
{
	HUDClass = nullptr;
	PlayerControllerClass = ALoadingController::StaticClass();
	bUseSeamlessTravel = false;
}

void ALoadingGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(mWidgetTimerHandle, this,
										   &ALoadingGameMode::StartMapTransition, 7.0f, false);
}

void ALoadingGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ALoadingGameMode::StartMapTransition()
{
	UCrazy6GameInstanceSubsystem* GameInstanceSubsystem = GetWorld()->GetGameInstance<UCrazy6GameInstance>()->
																	  GetSubsystem<UCrazy6GameInstanceSubsystem>();
	FString MapName = GameInstanceSubsystem->GetNextLevelName().ToString();
	NextMapName = FString::Printf(TEXT("%s?listen"), *MapName);
	
	// Host  Load
	FLatentActionInfo Info;
	Info.CallbackTarget = this; // Callback target is the current object
	Info.ExecutionFunction = FName("mServerTravel"); // Function to call when loading is done
	Info.Linkage = 0; // Linkage index
	Info.UUID = 0; // Unique identifier for this action

	UGameplayStatics::LoadStreamLevel(this, *NextMapName, true, false, Info);
}

void ALoadingGameMode::mServerTravel()
{
	GetWorld()->ServerTravel(*NextMapName, true);
}