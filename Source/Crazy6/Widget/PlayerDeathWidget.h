// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UPlayerDeathWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UButton> mMainButton;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mServerText;
protected:
	UFUNCTION()
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void BacktoMain();

};
