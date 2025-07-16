// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashCooltimeWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UDashCooltimeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> mDashCooltimeBar;

protected:
	void NativeOnInitialized();
	
public:
	void SetDash(float Time, float Cooltime);

};
