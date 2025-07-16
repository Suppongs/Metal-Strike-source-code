// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Crazy6/Global/SessionInfo.h"
#include "TeamListWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UTeamListWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
protected:

	UPROPERTY()
	TObjectPtr<class UButton> mJoinSessionButton;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mSessionName;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mNumberOfSession;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mMaxOfSession;
	UPROPERTY()
	USessionObject* mItemObject;
public:
	TSharedPtr<FSessionInfo> ListRoomSessionData;


public:
	UFUNCTION()
	virtual void NativeOnListItemObjectSet(UObject* ItemObject) override;
	UFUNCTION()
	void JoinSession();
};