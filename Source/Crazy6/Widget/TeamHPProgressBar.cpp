// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamHPProgressBar.h"
#include "UIHeader.h"

void UTeamHPProgressBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mTeamProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("TeamProgressBar")));

}
void UTeamHPProgressBar::SetHPPercent(float Damage)
{
	mTeamProgressBar->SetPercent(Damage/100);
}