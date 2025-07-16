// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crazy6/Widget/TeamListWidget.h"
#include "Crazy6/Global/SessionInfo.h"
#include "LobbyWidget.generated.h"

/**
 *
 */
UCLASS()
class CRAZY6_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ULobbyWidget(const FObjectInitializer& ObjectInitializer);	
protected:
	UPROPERTY()
	TObjectPtr<class UButton> mCreateSessionButton;
	UPROPERTY()
	TObjectPtr<class UButton> mFindSessionButton;
	UPROPERTY()
	TObjectPtr<class UButton> mQuitButton;
	UPROPERTY()
	TObjectPtr<class UListView> mLobbyListView;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mNoJoin;

	UPROPERTY()
	TObjectPtr<class UWidgetAnimation> mTextAnim;
	TMap<FString, TObjectPtr<class UWidgetAnimation>>	mAnimMap;
	
	UPROPERTY()
	TObjectPtr<class UUserWidget>mCreateSessionUIWidget;
	UPROPERTY()
	TSubclassOf<class UUserWidget> mCreateSessionUIWidgetClass;


protected:
	virtual void NativeOnInitialized() override;
public:
	UFUNCTION()
	void CreateSessionWidgetOpen();
	UFUNCTION()
	void FindSession();
	UFUNCTION()
	void QuitLobby();
	UFUNCTION()
	void PlayText();

	void SessionListReset(class UCrazy6GameInstanceSubsystem* GameInstSubsystem);
	void AddSessionToList(const TSharedPtr<FSessionInfo>& SessionInfo);


};
