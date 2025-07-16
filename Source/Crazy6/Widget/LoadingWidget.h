// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "LoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	TObjectPtr<class UImage> mBack;

	virtual void NativeOnInitialized();
	
public:
	void SetTexture(UTexture2D* RandomTexture);
};
