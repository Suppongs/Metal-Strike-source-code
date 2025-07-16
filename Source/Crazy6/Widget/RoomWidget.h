// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crazy6/Global/SessionInfo.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstance.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"
#include "RoomWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API URoomWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
protected:
	UPROPERTY()
	TObjectPtr<class UButton> mGameStart;
	UPROPERTY()
	TObjectPtr<class UButton> mQuitRoom;
	UPROPERTY()
	TArray<TObjectPtr<class UTextBlock>> mPlayerNames;
	
	UPROPERTY()
	TObjectPtr<class UTextBlock> mRoomSessionName;

	FSessionInfo mRoomSessionInfo;
	UCrazy6GameInstanceSubsystem* mGameInstSubsystem;
	IOnlineSessionPtr mSessionInterface;
	APlayerController* mPlayerController;
	IOnlineIdentityPtr mIdentityInterface;
	FString Host;


public:
	UFUNCTION()
	void GameStart();
	UFUNCTION()
	void QuitRoom();
	void SetPlayerName(const FString& Text, int32 Index);

	void SetSessionInfo(FName SessionName);
	void ClearPlayerName();
	void PlayerUI();
	FSessionInfo GetSessionInfo() { return mRoomSessionInfo; }
};
