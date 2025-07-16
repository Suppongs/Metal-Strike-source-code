// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameStateBase.h"

#include "Components/TextBlock.h"
#include "Crazy6/GameMode/PlayerState/LobbyPlayerState.h"
#include "Crazy6/Controller/LobbyController.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"
#include "Crazy6/Widget/RoomWidget.h"
#include "Net/UnrealNetwork.h"

void ALobbyGameStateBase::SetPlayerArray()
{
	TArray<FString> TempPlayerIdArray;
	for (APlayerState* PlayerState : PlayerArray)
	{
		ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
		if (LobbyPlayerState)
		{
			FString PlayerName = LobbyPlayerState->GetPlayerName();
			TempPlayerIdArray.Add(PlayerName);
		}
	}
	PlayerIdArray = TempPlayerIdArray;
	ChangeConnect();
	OnRep_PlayerIdArray();
}

void ALobbyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameStateBase, PlayerIdArray);
	DOREPLIFETIME(ALobbyGameStateBase, Connect);
}


void ALobbyGameStateBase::OnRep_PlayerIdArray()
{
	if (IsValid(GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>()
	                             ->GetRoomWidget()))
	{
		URoomWidget* mCurrentWidget =
			GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>()
			                 ->GetRoomWidget();
		if (mCurrentWidget->IsInViewport())
		{
			mCurrentWidget->PlayerUI();
		}
	}
}

void ALobbyGameStateBase::ChangeConnect()
{
	if (true == Connect)
	{
		Connect = false;
	}
	else
	{
		Connect = true;
	}
}
