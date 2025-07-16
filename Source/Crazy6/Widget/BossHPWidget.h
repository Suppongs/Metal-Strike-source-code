// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "BossHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UBossHPWidget : public UUserWidget
{
	GENERATED_BODY()
protected:	
	UPROPERTY()
	TObjectPtr<UProgressBar> mMainProgressbar;
	UPROPERTY()
	TObjectPtr<UProgressBar> mSubProgressbar;

	float	mProgressAnimationPercent= 0.f;
	bool	mProgessAnimationEnable = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	float StartValue;
	float EndValue;
	float Duration;
	FTimerHandle TimerHandle;
	float ElapsedTime = 0.0f;


public:
	 void SetMainProgressBar(float Percent);
	void ProgressAnimation(float Percent);
	void UpdateProgressAnimation();
};
