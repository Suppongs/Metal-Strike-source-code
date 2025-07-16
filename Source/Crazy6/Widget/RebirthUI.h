// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UIHeader.h"
#include "RebirthUI.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API URebirthUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	class UImage* mPressImage;
	UPROPERTY()
	TObjectPtr<class UWidgetAnimation> mPressAnim;
	
	TMap<FString, TObjectPtr<class UWidgetAnimation>>	mAnimMap;


protected:
	virtual void NativeOnInitialized() override;
	

public:
	void PlayMyAnimation();
	void StopMyAnimation();
	
};
