// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDailogWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UItemDailogWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> mItemDailog;
	UPROPERTY()
	TObjectPtr<class UWidgetAnimation> mInandOut;
	UPROPERTY()
	TMap<FString, TObjectPtr<class UWidgetAnimation>>	mAnimMap;


protected:
	UFUNCTION()
	virtual void NativeOnInitialized() override;


public:
	UFUNCTION()
	void SetText(const FString& Text);
	//PlayAnimation
	UFUNCTION()
	void PlayMyAnimation();
};
