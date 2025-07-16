// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainBossHPbar.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UMainBossHPbar : public UUserWidget
{
	GENERATED_BODY()

protected:	
	UPROPERTY()
	TObjectPtr<class UBossHPWidget> mProgressbar;

protected:
	virtual void NativeOnInitialized() override;
public:
	void SetProgressBar(float mPercent);
	void SetProgressBarImage(float mPercent);
	
};
