// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 *
 */
UCLASS()
class CRAZY6_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UPlayerHPWidget1> mPlayerProgressMainBar;

protected:
	void NativeOnInitialized();
public:
	// Set HP
	void SetHPPercent(float Damage);


};
