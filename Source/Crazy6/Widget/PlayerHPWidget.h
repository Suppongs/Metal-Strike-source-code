// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UPlayerHPWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> mPlayerProgressBar;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetHPPercent(float Damage);
	
	
	
	
};
