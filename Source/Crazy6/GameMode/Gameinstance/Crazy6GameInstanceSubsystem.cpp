// Fill out your copyright notice in the Description page of Project Settings.


#include "Crazy6GameInstanceSubsystem.h"
#include "../../Widget/LobbyWidget.h"
#include "../../Widget/RoomWidget.h"


void UCrazy6GameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();

	if (nullptr != OnlineSubsystemInterface)
	{
		mIdentityInterface = OnlineSubsystemInterface->GetIdentityInterface();
		mSessionInterface = OnlineSubsystemInterface->GetSessionInterface();

		mSteamName = mIdentityInterface->GetUniquePlayerId(0)->ToString();
		if (mSessionInterface.IsValid())
		{

			mSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
				this, &UCrazy6GameInstanceSubsystem::OnCreateSessionComplete);
			mSessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
				this, &UCrazy6GameInstanceSubsystem::OnDestroySessionComplete);
			mSessionInterface->OnEndSessionCompleteDelegates.AddUObject(
				this, &UCrazy6GameInstanceSubsystem::OnEndSessionComplete);
			mSessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
				this, &UCrazy6GameInstanceSubsystem::OnFindSessionsComplete);
			mSessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
				this, &UCrazy6GameInstanceSubsystem::OnJoinSessionsComplete);
			mSessionInterface->OnStartSessionCompleteDelegates.AddUObject(
				this, &UCrazy6GameInstanceSubsystem::OnStartSessionComplete);
			/*mSessionInterface->OnSessionParticipantsChangeDelegates.AddUObject(
				this, &UCrazy6GameInstanceSubsystem::OnSessionParticipantsChange);*/

			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red,
				TEXT("Crazy6GameInstance Init"));
			GEngine->AddOnScreenDebugMessage(20, 15, FColor::Blue, FString::Printf(TEXT("SubSystem : %s"),
				*OnlineSubsystemInterface->GetSubsystemName().ToString()));
		}
	}
}

void UCrazy6GameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UCrazy6GameInstanceSubsystem::Reset()
{
	mSessionSearch = nullptr;

	mSearchResultNum = 0;
	mLobbyWidget = nullptr;
	mRoomWidget = nullptr;
	
	mSessionInfos = TArray<TSharedPtr<FSessionInfo>>();
	mSessionName = TEXT("");
	mCurrentSessionName = TEXT("");
	mCurrentLevelName  = TEXT("");
	mNextLevelName = TEXT("LobbyLevel");
	
	mDifficultyOption = 1;
	mConnectedPlayerCount = 0;
	mSteamName  = TEXT(""); 
}

void UCrazy6GameInstanceSubsystem::CreateSession(int Number, FName SessionName)
{
	if (mSessionInterface.IsValid())
	{
		//Verification
		auto ExistingSession = mSessionInterface->GetNamedSession(mCurrentSessionName);
		if (ExistingSession != nullptr)
		{
			mSessionInterface->DestroySession(mCurrentSessionName);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, TEXT("Destroy session, create Session"));
		}

		TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
		FName Key = TEXT("Key");
		FString RoomName = SessionName.ToString();

		SessionSettings->bIsLANMatch = false;
		SessionSettings->NumPublicConnections = Number;
		SessionSettings->bAllowJoinViaPresence = true;
		SessionSettings->bShouldAdvertise = true;
		SessionSettings->bUsesPresence = true;
		SessionSettings->bUseLobbiesIfAvailable = true;
		SessionSettings->bShouldAdvertise = true;
		SessionSettings->bAllowJoinInProgress = true;
		SessionSettings->bIsDedicated = false;

		SessionSettings->Set(Key, RoomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


		mSessionInterface->CreateSession(0, SessionName, *SessionSettings);
	}
}

void UCrazy6GameInstanceSubsystem::FindSession()
{
	mSessionSearch = nullptr;
	if (mSessionInterface.IsValid())
	{
		mSessionSearch = MakeShareable(new FOnlineSessionSearch());
		if (mSessionSearch.IsValid())
		{
			mSessionSearch->MaxSearchResults = 1000; // Max Search
			mSessionSearch->bIsLanQuery = false; // LAN 
			mSessionSearch->TimeoutInSeconds = 7.0f; // Search time out
			mSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);


			mSessionInterface->FindSessions(0, mSessionSearch.ToSharedRef());
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(4, 10.f, FColor::Emerald, TEXT("No mSessionSearch"));
		}
	}
}

void UCrazy6GameInstanceSubsystem::mJoinSession(TSharedPtr<FSessionInfo> SessionInfo)
{
	if (mSessionInterface.IsValid() && mSessionSearch.IsValid())
	{
		if (mSessionSearch->SearchResults.Num() > SessionInfo->IndexNumber)
		{
			FOnlineSessionSearchResult SearchResult = mSessionSearch->SearchResults[SessionInfo->IndexNumber];
			FNamedOnlineSession* NamedSession = mSessionInterface->GetNamedSession(FName(SessionInfo->SessionName));

			if (NamedSession && NamedSession->SessionState == EOnlineSessionState::InProgress)
			{
				mLobbyWidget->PlayText();
				return;
			}

			// Session Join
			mSessionInterface->JoinSession(0, FName(*SessionInfo->SessionName), SearchResult);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(8, 20.f, FColor::Red, TEXT("Invalid Session Index"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(8, 20.f, FColor::Red, TEXT("SessionInterface or SessionSearch is invalid"));
	}
}

void UCrazy6GameInstanceSubsystem::DestroySession(FName SessionName)
{
	if (mSessionInterface.IsValid())
	{
		mSessionInterface->DestroySession(SessionName);
	}
}

void UCrazy6GameInstanceSubsystem::StartGameSession(FName SessionName)
{
	CheckSessionStatus(SessionName);
	
	if (mSessionInterface.IsValid())
	{
		FNamedOnlineSession* NamedSession = mSessionInterface->GetNamedSession(SessionName);
		if (NamedSession && NamedSession->SessionState != EOnlineSessionState::InProgress)
		{
			mSessionInterface->StartSession(SessionName);
		}
		else
		{
			FString MapName = mNextLevelName.ToString();
			GetWorld()->ServerTravel(*FString::Printf(TEXT("%s?listen"), *MapName), true);
			SetNextLevelName("Landscape_map");
		}
	}
	
}

void UCrazy6GameInstanceSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful) // Session Succeded
	{
		SetCurrentSessionName(SessionName);
		CheckSessionStatus(SessionName);

		UWorld* World = GetWorld();
		if (World)
		{

			FString MapName = mNextLevelName.ToString();
			World->ServerTravel(*FString::Printf(TEXT("%s?listen"), *MapName));
			FString CurrentMapPath = GetWorld()->GetCurrentLevel()->GetName();
			GEngine->AddOnScreenDebugMessage(14, 15, FColor::Magenta,
				FString::Printf(
					TEXT("MySession : %s"), *CurrentMapPath));
			SetNextLevelName("LoadingMap");
		}
	}
	else // Session Failed
	{
		GEngine->AddOnScreenDebugMessage(3, 15, FColor::Blue, TEXT("Session Failed!"));
	}
}

void UCrazy6GameInstanceSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		SetCurrentSessionName(TEXT(""));
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartLevel"));
		GEngine->AddOnScreenDebugMessage(20, 15, FColor::Blue,
			FString::Printf(TEXT("Session Delete! : %s"), *SessionName.ToString()));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(20, 15, FColor::Blue, TEXT("Session Delete FAIL!"));
	}
}

void UCrazy6GameInstanceSubsystem::OnEndSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		SetCurrentSessionName(TEXT(""));
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartLevel"));
		GEngine->AddOnScreenDebugMessage(20, 15, FColor::Blue,
			FString::Printf(TEXT("Session end! : %s"), *SessionName.ToString()));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(20, 15, FColor::Blue, TEXT("Session End"));
	}
}

void UCrazy6GameInstanceSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!mSessionInterface.IsValid()
		|| !bWasSuccessful)
	{
		return;
	}
	if (bWasSuccessful)
	{
		// Delete Info
		mSessionInfos.Empty();
		if (mSessionSearch->SearchResults.Num() > 0)
		{
			for (int i = 0; i < mSessionSearch->SearchResults.Num(); i++)
			{
				FOnlineSessionSearchResult SearchResult = mSessionSearch->SearchResults[i];
				TSharedPtr<FSessionInfo> Info = MakeShareable(new FSessionInfo());

				FName Key = TEXT("Key");
				FString Name;

				if (SearchResult.Session.SessionSettings.Get(Key, Name))
				{
					Info->SessionName = Name;
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Yellow, TEXT("Session Name not found!!!!!!!!!"));
				}


				Info->MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections; // Max Player
				Info->CurrentPlayers = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections; //  Current People
				Info->IndexNumber = i; //Index number
				mSessionInfos.Add(Info);
				if (mLobbyWidget)
				{
					if (mLobbyWidget->IsInViewport())
					{
						mLobbyWidget->SessionListReset(this);
					}
				}
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(3, 15, FColor::Blue, TEXT("Find Ok, but find session Noting"));
		}
	}
}

void UCrazy6GameInstanceSubsystem::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	SetCurrentSessionName(SessionName);
	FString ConnectString;
	if (mSessionInterface->GetResolvedConnectString(SessionName, ConnectString))
	{
		// Host server
		APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (PlayerController && Result == EOnJoinSessionCompleteResult::Success)
		{

			PlayerController->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
			SetNextLevelName("LoadingMap");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(21, 15, FColor::Blue, TEXT("Join not Success"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(20, 15, FColor::Orange, TEXT("X Session Join X"));
	}
}

void UCrazy6GameInstanceSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
			FString MapName = mNextLevelName.ToString();
			GetWorld()->ServerTravel(*FString::Printf(TEXT("%s?listen"), *MapName), true);
			SetNextLevelName("Landscape_map");
	}
}

void UCrazy6GameInstanceSubsystem::CheckSessionStatus(FName SessionName)
{
	FNamedOnlineSession* Session = mSessionInterface->GetNamedSession(SessionName);
	if (Session)
	{
		switch (Session->SessionState)
		{
		case EOnlineSessionState::NoSession:
			GEngine->AddOnScreenDebugMessage(7, 15, FColor::Yellow, TEXT("No session"));
			break;
		case EOnlineSessionState::Creating:
			GEngine->AddOnScreenDebugMessage(7, 15, FColor::Yellow, TEXT("Creating"));
			break;
		case EOnlineSessionState::Pending:
			GEngine->AddOnScreenDebugMessage(7, 15, FColor::Yellow, TEXT("Pending"));
			break;
		case EOnlineSessionState::InProgress:
			GEngine->AddOnScreenDebugMessage(7, 15, FColor::Yellow, TEXT("InProgress"));
			break;
		case EOnlineSessionState::Ending:
			GEngine->AddOnScreenDebugMessage(7, 15, FColor::Yellow, TEXT("Ending"));
			break;
		case EOnlineSessionState::Ended:
			GEngine->AddOnScreenDebugMessage(7, 15, FColor::Yellow, TEXT("Ended"));
			break;
		default:
			break;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(3, 15, FColor::Yellow, TEXT("No session found STATE"));
	}
}
