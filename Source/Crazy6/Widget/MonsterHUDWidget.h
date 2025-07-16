// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UMonsterHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> mMonsterHPBar;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetHPPercent(float Percent);
	
	
};
