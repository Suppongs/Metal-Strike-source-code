// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UMonsterHPWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<class UWidgetAnimation> mTextUp;
	UPROPERTY()
	TObjectPtr<class UWidgetAnimation> mTextUPRED;
protected:
	class UTextBlock* mDamage;
	
	TMap<FString, TObjectPtr<class UWidgetAnimation>>	mAnimMap;


protected:
	virtual void NativeOnInitialized() override;
	

public:
	void SetText(FText text);
	void PlayMyAnimation(UWidgetAnimation* Animation);
	
};
