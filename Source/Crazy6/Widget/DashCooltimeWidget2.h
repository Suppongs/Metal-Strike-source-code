// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashCooltimeWidget2.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UDashCooltimeWidget2 : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> mDashCoolTime;
	UPROPERTY()
	TObjectPtr<class UDashCooltimeWidget> mDashWidgetImage;

protected:
	void NativeOnInitialized();
	
public:
	void SetDash(float Time, float Cooltime);
	
	
};
