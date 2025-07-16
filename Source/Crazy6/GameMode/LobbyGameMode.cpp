// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "../Controller/LobbyController.h"
#include "GameState/LobbyGameStateBase.h"
#include "Gameinstance/Crazy6GameInstanceSubsystem.h"
#include "PlayerState/LobbyPlayerState.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = ALobbyController::StaticClass();
	PlayerStateClass = ALobbyPlayerState::StaticClass();
	GameStateClass = ALobbyGameStateBase::StaticClass();
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>()->SetNextLevelName("LoadingMap");
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	ALobbyGameStateBase* LobbyGameState = GetGameState<ALobbyGameStateBase>();
	if (LobbyGameState)
	{
		LobbyGameState->SetPlayerArray();
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	ALobbyGameStateBase* LobbyGameState = GetGameState<ALobbyGameStateBase>();
	if (LobbyGameState)
	{
		LobbyGameState->SetPlayerArray();
	}
}
