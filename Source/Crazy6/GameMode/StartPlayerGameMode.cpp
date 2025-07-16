// Fill out your copyright notice in the Description page of Project Settings.


#include "StartPlayerGameMode.h"
#include "../Controller/StartPlayerController.h"
#include "Gameinstance/Crazy6GameInstanceSubsystem.h"

AStartPlayerGameMode::AStartPlayerGameMode()
{
	PlayerControllerClass = AStartPlayerController::StaticClass();
	
}

void AStartPlayerGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>()->Reset();
}



