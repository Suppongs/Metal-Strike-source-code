// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHPWidget.h"
#include "UIHeader.h"

void UPlayerHPWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mPlayerProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PlayerProgressBar")));

}
void UPlayerHPWidget::SetHPPercent(float Damage)
{
	mPlayerProgressBar->SetPercent(Damage/100);
}




