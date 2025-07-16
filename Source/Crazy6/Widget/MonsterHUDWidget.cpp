// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHUDWidget.h"
#include "UIHeader.h"



void UMonsterHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mMonsterHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("MonsterHPBar")));
}
void UMonsterHUDWidget::SetHPPercent(float Percent)
{         
	if (mMonsterHPBar)
	{
		mMonsterHPBar->SetPercent(Percent);
	}
}

