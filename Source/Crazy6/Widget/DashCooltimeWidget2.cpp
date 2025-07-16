// Fill out your copyright notice in the Description page of Project Settings.


#include "DashCooltimeWidget2.h"
#include "UIHeader.h"
#include "../../Crazy6/Widget/DashCooltimeWidget.h"


void UDashCooltimeWidget2::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mDashCoolTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("DashCoolTime")));
	mDashWidgetImage = Cast<UDashCooltimeWidget>(GetWidgetFromName(TEXT("DashWidgetImage")));

}
void UDashCooltimeWidget2::SetDash(float Time,float Cooltime)
{
	mDashCoolTime->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), Time)));
	mDashWidgetImage->SetDash(Time, Cooltime);
}

