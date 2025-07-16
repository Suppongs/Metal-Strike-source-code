// Fill out your copyright notice in the Description page of Project Settings.


#include "DashCooltimeWidget.h"
#include "UIHeader.h"



void UDashCooltimeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mDashCooltimeBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("DashCooltimeBar")));
	

}
void UDashCooltimeWidget::SetDash(float Time, float Cooltime)
{
	//percent
	mDashCooltimeBar->SetPercent((Time / Cooltime));
}
