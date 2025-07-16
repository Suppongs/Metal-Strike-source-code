// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Crazy6/Global/SessionInfo.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Online.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystemUtils.h" 
#include "Crazy6GameInstanceSubsystem.generated.h"


/**
 * 
 */
UCLASS()
class CRAZY6_API UCrazy6GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void Reset();
	void CreateSession(int Number, FName SessionName);
	void FindSession();
	void mJoinSession(TSharedPtr<FSessionInfo> SessionInfo);
	void DestroySession(FName SessionName);
	void StartGameSession(FName SessionName);
	void CheckSessionStatus(FName SessionName);

protected:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnEndSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	

protected:
	//Networking
	IOnlineSessionPtr mSessionInterface;
	IOnlineIdentityPtr mIdentityInterface;
	TSharedPtr<FOnlineSessionSearch> mSessionSearch;

	INT8 mSearchResultNum;
	//LobbyWidgetSET
	TObjectPtr<class ULobbyWidget> mLobbyWidget;
	//Save Room Widget
	TObjectPtr<class URoomWidget> mRoomWidget;
	
	TArray<TSharedPtr<FSessionInfo>> mSessionInfos;
	FName mSessionName;
	FName mCurrentSessionName;

	FName mCurrentLevelName;
	// Environment
	FName mNextLevelName = TEXT("LobbyLevel");
	// 1~4 the number of people
	UINT8 mDifficultyOption = 1;
	UINT8 mConnectedPlayerCount = 0;
	//SteamName
	FString mSteamName;

public:
	// Get
	IOnlineSessionPtr GetSessionInterface() { return mSessionInterface; }
	TSharedPtr<class FOnlineSessionSearch> GetSessionSearch() { return mSessionSearch; }

	FORCEINLINE UINT8 GetDifficultyOption()	const { return mDifficultyOption; }
	UINT8 GetConnectedPlayerCount() const { return mConnectedPlayerCount; };

	FORCEINLINE FName GetNextLevelName() const { return mNextLevelName; }

	FORCEINLINE FName GetSessionName() { return mSessionName; }
	FORCEINLINE FName GetCurrentSessionName() { return mCurrentSessionName; }
	FORCEINLINE FString GetSteamName() { return mSteamName; }
	FORCEINLINE TObjectPtr<class URoomWidget> GetRoomWidget() { return mRoomWidget; }
	FORCEINLINE TObjectPtr<class ULobbyWidget> GetLobbyWidget() { return mLobbyWidget; }
	
	TArray<TSharedPtr<FSessionInfo>> GetSessionInfos() { return mSessionInfos; }
	IOnlineIdentityPtr GetIdentityInterface() {return mIdentityInterface;}

	// bool IsEveryoneConnected(UINT8 ReadyCount) const { return (ReadyCount == 2); } /* TEST */
	bool IsEveryoneConnected(UINT8 ReadyCount) const { return (ReadyCount == mConnectedPlayerCount); }
	
	//set
	FORCEINLINE void SetCurrentLevelName(const FName& Name) { mCurrentLevelName = Name; }
	FORCEINLINE void SetNextLevelName(const FName& Name) { mNextLevelName = Name; }
	FORCEINLINE void SetDifficultyOption(const UINT8& Option) { mDifficultyOption = Option; }
	void LogoutPlayer() { mDifficultyOption = (mDifficultyOption > 0) ? --mDifficultyOption : 0; }

	FORCEINLINE void SetSessionName(const FName& SessionName) { mSessionName = SessionName; }
	FORCEINLINE void SetCurrentSessionName(const FName& SessionName) { mCurrentSessionName = SessionName; }

	void SetSessionInfos(const TArray<TSharedPtr<FSessionInfo>>& SessionInfos) { mSessionInfos = SessionInfos; }

	//
	void SetLobbyWidget(class ULobbyWidget* LobbyWidget) { mLobbyWidget = LobbyWidget; }
	void SetRoomWidget(class URoomWidget* RoomWidget) { mRoomWidget = RoomWidget; }
};
