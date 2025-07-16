// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EpicAppearanceWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UEpicAppearanceWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UWidgetAnimation> mWarningAnim;
	UPROPERTY()
	TMap<FString, TObjectPtr<class UWidgetAnimation>>	mAnimMap;

protected:
	UFUNCTION()
	virtual void NativeOnInitialized() override;

public:
	//PlayAnimation
	UFUNCTION()
	void PlayMyAnimation();


	
	
	
};
