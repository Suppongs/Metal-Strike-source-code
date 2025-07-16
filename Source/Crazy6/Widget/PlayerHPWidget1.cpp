// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHPWidget1.h"
#include "UIHeader.h"
#include "PlayerHPWidget.h"


void UPlayerHPWidget1::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mPlayerProgressbarMain = Cast<UPlayerHPWidget>(GetWidgetFromName(TEXT("PlayerProgressbarMain")));
	mPlayerHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerHP")));

}
void UPlayerHPWidget1::SetHPPercent(float Damage)
{
	mPlayerProgressbarMain->SetHPPercent(Damage);
	mPlayerHP->SetText(FText::AsNumber(Damage));
}





