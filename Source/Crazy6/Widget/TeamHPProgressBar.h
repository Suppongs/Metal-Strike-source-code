// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamHPProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UTeamHPProgressBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> mTeamProgressBar;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetHPPercent(float Damage);
	
	
};
