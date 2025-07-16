// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"

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

AMainGameMode::AMainGameMode()
{
	GameStateClass = AInGameState::StaticClass();
	PlayerStateClass 	= AMainPlayerState::StaticClass();
	bUseSeamlessTravel = false;
	
	CEnemyDataManager::Inst();
	CMissionDataManager::Inst();
	
	PlayerControllerClass = APlayerController::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClass(TEXT("/Script/Engine.Blueprint'/Game/Dev/Player/Human/BP_TestPlayer.BP_TestPlayer_C'"));
	if (PlayerPawnClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnClass.Class;
	}

	OnRestart.AddDynamic(this, &AMainGameMode::RestartEvent);
	
}


void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// Must Have Only 1 in Level.
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null!"));
		return;
	}
	GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>()->SetNextLevelName("LoadingMap");
	for (TActorIterator<AMissionManager> It(GetWorld()); It; ++It)
	{
		AMissionManager* FoundActor = *It;
		if (FoundActor)
		{
			mMissionSystem = FoundActor;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
FString::Printf(TEXT("!! AMainGameMode::beginPlay() : %s"), *UGameplayStatics::GetCurrentLevelName(GetWorld())));
}

void AMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	APlayerBase* mPlayer = Cast<APlayerBase>(NewPlayer->GetPawn());
	GetGameState<AInGameState>()->SetTeamPlayerWidget();
	if (mPlayer)
	{
		mPlayer->UpdateTeamPlayerUI();
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
FString::Printf(TEXT("AMainGameMode::PostLogin, Player : %s"), *mPlayer->GetName()));
	}
}

void AMainGameMode::Logout(AController* Exiting)
{
	GetGameState<AInGameState>()->SetTeamPlayerWidget();
	if( false == IsTraveling)
	{
		if (GetGameInstance())
		{
			if (GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>())
			{
				GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>()->LogoutPlayer();
			}
		}
	}

	/* Debug */
	APlayerController* PlayerController = Cast<APlayerController>(Exiting);
	if (PlayerController)
	{
		APlayerBase* mPlayer = Cast<APlayerBase>(PlayerController->GetPawn());
		if (mPlayer)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
FString::Printf(TEXT("!! AMainGameMode::Logout(), Player %s is logging out. Actor Name: %s"), *PlayerController->GetName(), *mPlayer->GetName()));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
FString::Printf(TEXT("!! AMainGameMode::Logout(), Player %s is logging out. No Pawn found"), *PlayerController->GetName()));
		}
	}
	Super::Logout(Exiting);
}

void AMainGameMode::RestartEvent()
{
	IsTraveling = true;
}

