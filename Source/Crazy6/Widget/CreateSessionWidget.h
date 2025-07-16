// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateSessionWidget.generated.h"

UCLASS()
class CRAZY6_API UCreateSessionWidget : public UUserWidget
{
	GENERATED_BODY()	
protected:
	virtual void NativeOnInitialized() override;
	bool IsEnglishAndNumericOnly(const FString& InputText);
	UFUNCTION()
	void CreateSession();
	UFUNCTION()
	void QuitUI();

protected:
	UPROPERTY()
	TObjectPtr<class UEditableTextBox> mTeamName;
	UPROPERTY()
	TObjectPtr<class USpinBox> mSpinBox;
	UPROPERTY()
	TObjectPtr<class UButton> mSessionButton;
	UPROPERTY()
	TObjectPtr<class UButton> mQuitButton;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mNametext;
	UPROPERTY()
	TObjectPtr<class UWidgetAnimation> mTextAnimation;
	TMap<FString, TObjectPtr<class UWidgetAnimation>>	mAnimMap;




};
