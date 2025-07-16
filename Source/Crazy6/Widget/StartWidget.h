// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"

/**
 *
 */
UCLASS()
class CRAZY6_API UStartWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UStartWidget(const FObjectInitializer& ObjectInitializer);
protected:
	
	UPROPERTY()
	TObjectPtr<class UButton> mStartButton;
	UPROPERTY()
	TObjectPtr<class UButton> mOptionButton;
	UPROPERTY()
	TObjectPtr<class UButton> mExitButton;

	UPROPERTY()
	TSubclassOf<class UUserWidget>mLobbyUIWidgetClass;
	UPROPERTY()
	TObjectPtr<class UUserWidget>mLobbyUIWidget;
	UPROPERTY()
	TSubclassOf<class UUserWidget>mOptionUIWidgetClass;
	UPROPERTY()
	TObjectPtr<class UUserWidget>mOptionUIWidget;
	

protected:
	UFUNCTION()
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void QuitGame();
	UFUNCTION()
	void OptionWidgetOpen();
	UFUNCTION()
	void OptionWidgetEsc();




};
