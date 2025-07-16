// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPWidget1.generated.h"

UCLASS()
class CRAZY6_API UPlayerHPWidget1 : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UPlayerHPWidget> mPlayerProgressbarMain;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mPlayerHP;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetHPPercent(float Damage);



};
