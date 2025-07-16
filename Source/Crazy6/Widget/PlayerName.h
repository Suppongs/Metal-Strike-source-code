// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerName.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UPlayerName : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> mPlayerName;

protected:
	UFUNCTION()
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void SetPlayerText(const FText &text);
	
};
