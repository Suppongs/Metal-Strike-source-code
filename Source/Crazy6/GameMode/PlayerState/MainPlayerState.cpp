// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"
#include "../GameState/InGameState.h"
#include "Crazy6/Player/PlayerBase.h"
#include "Net/UnrealNetwork.h"

AMainPlayerState::AMainPlayerState()
{
}

void AMainPlayerState::SetPlayerName(const FString& PlayerName)
{
	if (HasAuthority())
	{
		Super::SetPlayerName(PlayerName);
		mPlayerName = PlayerName;
		AInGameState* GameState = GetWorld()->GetGameState<AInGameState>();
		if (GameState)
		{
			GameState->UpdatePlayerName(mPlayerIndex, PlayerName);
		}
	}
	else
	{
		ServerSetPlayerName(PlayerName);
	}

}
void AMainPlayerState::SetPlayerHP(float PlayerHP)
{
	if (HasAuthority())
	{
		mPlayerHP = PlayerHP;
		AInGameState* GameState = GetWorld()->GetGameState<AInGameState>();
		if (GameState)
		{
			GameState->UpdatePlayerHP(mPlayerIndex, PlayerHP);

		}
	}
	else
	{
		ServerSetPlayerHP(PlayerHP);
	}
}

void AMainPlayerState::ServerSetPlayerName_Implementation(const FString& PlayerName)
{
	SetPlayerName(PlayerName);
}
void AMainPlayerState::ServerSetPlayerHP_Implementation(float PlayerHP)
{
	SetPlayerHP(PlayerHP);
}
void AMainPlayerState::SetPlayerIndex(int32 PlayerIndex)
{
	mPlayerIndex = PlayerIndex;
}

void AMainPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMainPlayerState, mPlayerName);
	DOREPLIFETIME(AMainPlayerState, mPlayerHP);
	DOREPLIFETIME(AMainPlayerState, mPlayerIndex);
}
