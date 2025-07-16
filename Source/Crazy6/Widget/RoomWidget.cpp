// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomWidget.h"
#include "UIHeader.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "OnlineSubsystem.h"
#include "Crazy6/Controller/LobbyController.h"
#include "Crazy6/GameMode/GameState/LobbyGameStateBase.h"


void URoomWidget::NativeOnInitialized()
{
	mGameInstSubsystem = GetWorld()->GetGameInstance<UCrazy6GameInstance>()->GetSubsystem<
		UCrazy6GameInstanceSubsystem>();
	mGameInstSubsystem->SetRoomWidget(this);
	mGameStart = Cast<UButton>(GetWidgetFromName(TEXT("GameStart")));
	if (mGameStart)
	{
		mGameStart->OnClicked.AddDynamic(this, &URoomWidget::GameStart);
	}
	mQuitRoom = Cast<UButton>(GetWidgetFromName(TEXT("QuitRoom")));
	if (mQuitRoom)
	{
		mQuitRoom->OnClicked.AddDynamic(this, &URoomWidget::QuitRoom);
	}

	mPlayerNames.SetNum(4);
	for( int i = 0 ; i < mPlayerNames.Num() ; i++ )
	{
		FName mWidgetName = FName(*FString::Printf(TEXT("PlayerName%d"), i+1));
		mPlayerNames[i] = Cast<UTextBlock>(GetWidgetFromName(mWidgetName));
	}
	
	mRoomSessionName = Cast<UTextBlock>(GetWidgetFromName(TEXT("RoomName")));
}

void URoomWidget::GameStart()
{
	// Session
	mGameInstSubsystem = GetWorld()->GetGameInstance<UCrazy6GameInstance>()->GetSubsystem<
		UCrazy6GameInstanceSubsystem>();
	mPlayerController = GetWorld()->GetFirstPlayerController();

	GEngine->AddOnScreenDebugMessage(3, 15, FColor::Blue, FString::Printf(TEXT("GameStart")));
	if (mPlayerController->HasAuthority())
	{
		mGameInstSubsystem->StartGameSession(mGameInstSubsystem->GetCurrentSessionName());
	}
}

void URoomWidget::QuitRoom()
{
	// Session
	mGameInstSubsystem = GetWorld()->GetGameInstance<UCrazy6GameInstance>()->GetSubsystem<UCrazy6GameInstanceSubsystem>();
	mSessionInterface = mGameInstSubsystem->GetSessionInterface();

	ALobbyController* MyController = Cast<ALobbyController>(GetOwningPlayer());
	if(IsValid(MyController))
	{
		mGameInstSubsystem->SetRoomWidget(nullptr);
		if(MyController->HasAuthority())
		{
			mSessionInterface->DestroySession(FName(*mRoomSessionInfo.SessionName));
			UGameplayStatics::OpenLevel(GetWorld(),TEXT("StartLevel"));
		}
		else
		{
			const FUniqueNetIdRepl& UniqueIdRepl = MyController->PlayerState->GetUniqueId();
			TSharedPtr<const FUniqueNetId> UniqueNetId;
			if (UniqueIdRepl.IsValid())
			{
				UniqueNetId = UniqueIdRepl.GetUniqueNetId();
			}
			mSessionInterface->UnregisterPlayer(FName(*mRoomSessionInfo.SessionName),*UniqueNetId);
			MyController->ClientTravel(TEXT("/Game/Maps/StartLevel"), TRAVEL_Absolute);
		}
		RemoveFromParent();
	}
}

//PlayerName
void URoomWidget::SetPlayerName(const FString& Text, int32 Index)
{
	mPlayerNames[Index]->SetText(FText::FromString(Text));
}

//Session Info
void URoomWidget::SetSessionInfo(FName SessionName)
{
	// Session
	mGameInstSubsystem = GetWorld()->GetGameInstance<UCrazy6GameInstance>()->GetSubsystem<
		UCrazy6GameInstanceSubsystem>();
	mSessionInterface = mGameInstSubsystem->GetSessionInterface();

	// Current Session
	FNamedOnlineSession* CurrentSession = mSessionInterface->GetNamedSession(SessionName);
	if (CurrentSession)
	{
		FString StringSessionName = (CurrentSession->SessionName).ToString();
		FText RoomSessionName = FText::FromString(StringSessionName);
		mRoomSessionName->SetText(RoomSessionName);
		mRoomSessionInfo.SessionName = (CurrentSession->SessionName).ToString();
		mRoomSessionInfo.CurrentPlayers = CurrentSession->SessionSettings.NumPublicConnections - CurrentSession->
			NumOpenPublicConnections;
		mRoomSessionInfo.MaxPlayers = CurrentSession->SessionSettings.NumPublicConnections;
		
		PlayerUI();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(4, 15, FColor::Blue, FString::Printf(TEXT("No Find Set Session")));
	}
}
void URoomWidget::ClearPlayerName()
{
	for (int32 i = 0; i < mPlayerNames.Num(); ++i)
	{
		mPlayerNames[i]->SetText(FText::FromString(TEXT("")));
	}
}

void URoomWidget::PlayerUI()
{
	ClearPlayerName();
	// Player Set
	TArray<FString> PlayerIdArray;
	PlayerIdArray = Cast<ALobbyGameStateBase>(GetWorld()->GetGameState())->GetPlayerIdArray();
	for(int32 i = 0; i < PlayerIdArray.Num(); i++)
	{
		SetPlayerName(PlayerIdArray[i],i);
	}
}
