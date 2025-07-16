// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChangeModeWidget.generated.h"


/**
 * 
 */
UCLASS()
class CRAZY6_API UChangeModeWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	FTimerHandle TimerHandle;
protected:
	UPROPERTY()
	TObjectPtr<class APlayerBase> mPlayer;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mMode;
	UPROPERTY()
	TMap<FString, TObjectPtr<class UWidgetAnimation>>	mAnimMap;
	UPROPERTY()
	TObjectPtr<class UWidgetAnimation> mDissolveIn;
	UPROPERTY()
	TObjectPtr<class UWidgetAnimation> mDissolveOut;
	
	bool CurrentMode;
	bool CanChange = true;


protected:
	virtual void NativeOnInitialized() override;
	
public:
	void SetText(const FText& text);
	void ChangeMode();
	void OnDissolveOutFinishedChangeA();
	void OnDissolveOutFinishedChangeB();

	
	
};
