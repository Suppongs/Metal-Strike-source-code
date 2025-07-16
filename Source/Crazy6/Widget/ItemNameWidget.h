// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UItemNameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> mItemName;

protected:
	UFUNCTION()
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void SetText(const FText &text);
	
	
};
