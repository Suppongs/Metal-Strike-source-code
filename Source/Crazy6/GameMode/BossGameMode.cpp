// Fill out your copyright notice in the Description page of Project Settings.


#include "BossGameMode.h"

#include "EngineUtils.h"
#include "Crazy6/DebugMacros.h"

#include "Crazy6/Actor/Enemy/EnemyDataManager.h"
#include "Crazy6/Actor/Mission/MissionActor.h"
#include "Crazy6/Actor/Mission/MissionDataManager.h"
#include "Crazy6/Actor/Mission/MissionManager.h"

#include "GameState/InGameState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerState/MainPlayerState.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"
#include "Crazy6/Player/PlayerBase.h"

ABossGameMode::ABossGameMode()
{
	GameStateClass = AInGameState::StaticClass();
	PlayerStateClass = AMainPlayerState::StaticClass();
	
	CEnemyDataManager::Inst();
	CMissionDataManager::Inst();
	
	PlayerControllerClass = APlayerController::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("/Script/Engine.Blueprint'/Game/Dev/Player/Human/BP_TestPlayer.BP_TestPlayer_C'"));
	if (PlayerPawnClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}
}

void ABossGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ABossGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APlayerBase* mPlayer = Cast<APlayerBase>(NewPlayer->GetPawn());
	GetGameState<AInGameState>()->SetTeamPlayerWidget();
	if (mPlayer)
	{
		mPlayer->UpdateTeamPlayerUI();
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
FString::Printf(TEXT("ABossGameMode::PostLogin, Player : %s"), *mPlayer->GetName()));
	}
}

void ABossGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	GetGameState<AInGameState>()->SetTeamPlayerWidget();
	/* Debug */
	APlayerController* PlayerController = Cast<APlayerController>(Exiting);
	if (PlayerController)
	{
		APlayerBase* mPlayer = Cast<APlayerBase>(PlayerController->GetPawn());
		if (mPlayer)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
FString::Printf(TEXT("!! ABossGameMode::Logout(), Player %s is logging out. Actor Name: %s"), *PlayerController->GetName(), *mPlayer->GetName()));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
FString::Printf(TEXT("!! ABossGameMode::Logout(), Player %s is logging out. No Pawn found"), *PlayerController->GetName()));
		}
	}
}
