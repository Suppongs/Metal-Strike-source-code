// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameState.h"

#include "Crazy6/Actor/Enemy/Humanoid/BulletPool.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"

#include "Crazy6/GameMode/MainGameMode.h"
#include "Crazy6/GameMode/PlayerState/MainPlayerState.h"
#include "Crazy6/Player/PlayerBase.h"
#include "Net/UnrealNetwork.h"

AInGameState::AInGameState()
{
	// BulletPool
	mUsingPool = CreateDefaultSubobject<UBulletPool>(TEXT("BulletPool"));
}

void AInGameState::BeginPlay()
{
	Super::BeginPlay();
}

void AInGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AInGameState::SetEveryoneEnableInputMulticast_Implementation()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		if (PC->IsLocalPlayerController() && PC->GetPawn())
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
											 FString::Printf(
												 TEXT(
													 "UCrazy6GameInstanceSubsystem::SetEveryoneEnableInputMulticast() : EnableInput(), Player : %s"),
												 *PC->GetName()));
			PC->GetPawn()->EnableInput(PC);
		}
	}
}

void AInGameState::SetConnectedPlayerNum_Implementation()
{
	ConnectedPlayerNum = GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>()->GetDifficultyOption();
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
								 FString::Printf(TEXT("Difficulty : %d"), GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>()->GetDifficultyOption()));
}

void AInGameState::OnConstruction(const FTransform& Transfrom)
{
	Super::OnConstruction(Transfrom);
}

void AInGameState::SetTeamPlayerWidget()
{
	if (HasAuthority())
	{
		TArray<float> TempPlayerHPArray;
		TArray<FString> TempPlayerNameArray;
		TArray<bool> TempPlayerDeathArray;
		int32 PlayerIndex = 0;

		for (APlayerState* PlayerState : PlayerArray)
		{
			AMainPlayerState* MainPlayerState = Cast<AMainPlayerState>(PlayerState);
			if (MainPlayerState)
			{
				FString PlayerName = MainPlayerState->GetPlayerName();
				float PlayerHP = MainPlayerState->GetPlayerHP();
				MainPlayerState->SetPlayerIndex(PlayerIndex);
				PlayerIndex++;

				TempPlayerNameArray.Add(PlayerName);
				TempPlayerHPArray.Add(PlayerHP);
				TempPlayerDeathArray.Add(false);
			}
		}

		PlayerHPs = TempPlayerHPArray;
		PlayerNames = TempPlayerNameArray;
		PlayerDeaths = TempPlayerDeathArray;

		ChangeHP();
		OnRep_PlayerHPs();
	}
}

void AInGameState::SetReadyPlayer()
{
	++ReadyPlayer;
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
	                                 FString::Printf(
		                                 TEXT("AInGameState::SetReadyPlayer(), ReadyPlayer Count : %d"), ReadyPlayer));
	SetConnectedPlayerNum();
	if (ConnectedPlayerNum == ReadyPlayer)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
		                                 FString::Printf(TEXT("AInGameState::SetReadyPlayer() : All Player Ready!")));

		SetEveryoneEnableInputMulticast();
		for (APlayerState* PlayerState : PlayerArray)
		{
			if (PlayerState)
			{
				APlayerController* Controller = PlayerState->GetOwner<APlayerController>();
				// BeginPlay Controller
				if (Controller)
				{
					APlayerBase* Character = Cast<APlayerBase>(Controller->GetCharacter());
					if (Character)
					{
						// Player Ready, after PlayerName Setting
						Character->SetPlayerName();
					}
				}
			}
		}
	}
}

void AInGameState::ChangeHP()
{
	if (true == mHP)
	{
		mHP = false;
	}
	else
	{
		mHP = true;
	}
}

void AInGameState::UpdatePlayerHP(int32 PlayerIndex, float NewHP)
{
	if (PlayerIndex >= 0 && PlayerIndex < PlayerHPs.Num())
	{
		PlayerHPs[PlayerIndex] = NewHP;
		if (100 == NewHP && true == PlayerDeaths[PlayerIndex])
		{
			PlayerDeaths[PlayerIndex] = false;
		}
		if (0 == NewHP)
		{
			int32 CurrentDeathNumber = 0;
			PlayerDeaths[PlayerIndex] = true;
			for (int i = 0; i < PlayerDeaths.Num(); i++)
			{
				if (true == PlayerDeaths[i])
				{
					CurrentDeathNumber++;
				}
				else
				{
					break;
				}
			}
			if (CurrentDeathNumber == PlayerDeaths.Num())
			{
				FString MapName = TEXT("LobbyLevel");
				GetWorld()->ServerTravel(*FString::Printf(TEXT("%s?listen"), *MapName));
			}
		}
		OnRep_PlayerHPs();
		ChangeHP();
	}
}

void AInGameState::UpdatePlayerName(int32 PlayerIndex, FString Name)
{
	if (PlayerIndex >= 0 && PlayerIndex < PlayerHPs.Num())
	{
		PlayerNames[PlayerIndex] = Name;
		OnRep_PlayerNames();
	}
}

void AInGameState::OnRep_PlayerHPs()
{
	for (APlayerState* PlayerState : PlayerArray)
	{
		if (PlayerState)
		{
			AController* Controller = PlayerState->GetOwner<AController>();
			if (Controller)
			{
				APlayerBase* Character = Cast<APlayerBase>(Controller->GetCharacter());
				if (Character)
				{
					Character->UpdateTeamPlayerUI();
				}
			}
		}
	}
}

void AInGameState::OnRep_PlayerNames()
{
	for (APlayerState* PlayerState : PlayerArray)
	{
		if (PlayerState)
		{
			AController* Controller = PlayerState->GetOwner<AController>();
			if (Controller)
			{
				APlayerBase* Character = Cast<APlayerBase>(Controller->GetCharacter());
				if (Character)
				{
					Character->UpdateTeamPlayerUI();
				}
			}
		}
	}
}

void AInGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Players Replicated
	DOREPLIFETIME(AInGameState, PlayerHPs);
	DOREPLIFETIME(AInGameState, PlayerNames);
	DOREPLIFETIME(AInGameState, mHP);
	DOREPLIFETIME(AInGameState, PlayerDeaths);
	DOREPLIFETIME(AInGameState, ReadyPlayer);
	DOREPLIFETIME(AInGameState, ConnectedPlayerNum);
}
