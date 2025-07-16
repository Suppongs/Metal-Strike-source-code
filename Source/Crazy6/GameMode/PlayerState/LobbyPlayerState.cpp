// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"

void ALobbyPlayerState::SetPlayerName(const FString& PlayerName)
{
	Super::SetPlayerName(PlayerName);
	mPlayerName = PlayerName;
}
