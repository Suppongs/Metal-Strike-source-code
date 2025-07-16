// Fill out your copyright notice in the Description page of Project Settings.


#include "MainBossHPbar.h"
#include "BossHPWidget.h"

void UMainBossHPbar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mProgressbar = Cast<UBossHPWidget>(GetWidgetFromName(TEXT("ProgressBar")));
}

void UMainBossHPbar::SetProgressBar(float mPercent)
{
	mProgressbar->SetMainProgressBar(mPercent);
	
}

void UMainBossHPbar::SetProgressBarImage(float mPercent)
{
	mProgressbar->ProgressAnimation(mPercent);
}

