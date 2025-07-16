// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletWidget.h"
#include "UIHeader.h"
#include "Crazy6/DebugMacros.h"


void UBulletWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mCurrentbullet = Cast<UTextBlock>(GetWidgetFromName(TEXT("currentbullet")));
	mClipbullet = Cast<UTextBlock>(GetWidgetFromName(TEXT("Clipbullet")));
	mMaxBullet = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxBullet")));
}

void UBulletWidget::SetCurrentBullet(int32 mCurrentAmmo)
{
	if (nullptr != mCurrentbullet)
	{
		float current = static_cast<float>(mCurrentAmmo);
		mCurrentbullet->SetText(FText::AsNumber(current));
	}
}

void UBulletWidget::SetClipBullet(int32 mClipAmmo)
{
	if (nullptr != mClipbullet)
	{
		float Clip = static_cast<float>(mClipAmmo);
		mClipbullet->SetText(FText::AsNumber(Clip));
	}
}

void UBulletWidget::SetMaxBullet(int32 mMaxAmmo)
{
	if (nullptr != mMaxBullet)
	{
		mMaxBullet->SetText(FText::AsNumber(mMaxAmmo));
	}
}







