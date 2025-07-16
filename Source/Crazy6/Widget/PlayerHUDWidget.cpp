// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "UIHeader.h"
#include "MissionWidget.h"
#include "PlayerHPWidget1.h"


void UPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mPlayerProgressMainBar = Cast<UPlayerHPWidget1>(GetWidgetFromName(TEXT("PlayerProgressMainBar")));
}

void UPlayerHUDWidget::SetHPPercent(float Damage)
{
	if(IsValid(mPlayerProgressMainBar))
	{
		mPlayerProgressMainBar->SetHPPercent(Damage);
	}
}
