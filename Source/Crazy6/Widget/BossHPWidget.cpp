// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHPWidget.h"

#include "NiagaraCommon.h"
#include "UIHeader.h"

void UBossHPWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mMainProgressbar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Main")));
	mSubProgressbar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Sub")));
	mMainProgressbar->SetPercent(1.0f);
	mSubProgressbar->SetPercent(1.0f);
	
}

void UBossHPWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(mProgessAnimationEnable)
	{
		float Percent = mSubProgressbar->GetPercent();

		Percent -= 0.023f * InDeltaTime;

		if(Percent <= mProgressAnimationPercent)
		{
			Percent = mProgressAnimationPercent;
			mProgessAnimationEnable = false;
		}
		
		mSubProgressbar->SetPercent(Percent);
	}
}

void UBossHPWidget::SetMainProgressBar(float Percent)
{
	mMainProgressbar->SetPercent(Percent);
}

void UBossHPWidget::ProgressAnimation(float Percent)
{
	UWorld* ThisWorld = GetWorld();
	if (!ThisWorld)
	{
		return;
	}
	if (!mSubProgressbar || !mMainProgressbar)
	{
		return;
	}

	mProgressAnimationPercent = Percent;
	mProgessAnimationEnable = true;
	
	//  StartValue = mSubProgressbar->GetPercent();
	//  EndValue = Percent;
	//  Duration = 2.0f; // 2sec
	//
	// if( StartValue < EndValue)
	// {
	// 	return;
	// }
	// if (ThisWorld->GetTimerManager().IsTimerActive(TimerHandle))
	// {
	// 	ThisWorld->GetTimerManager().ClearTimer(TimerHandle);
	// }
	// ElapsedTime = 0.0f;
	// ThisWorld->GetTimerManager().SetTimer(TimerHandle, this, &UBossHPWidget::UpdateProgressAnimation, 0.016f, true);
}

void UBossHPWidget::UpdateProgressAnimation()
{
	if (!mSubProgressbar || !mMainProgressbar)
	{
		return;
	}
	//ElapsedTime += GetWorld()->GetDeltaSeconds();
	ElapsedTime += 0.016f;
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);
	float CurrentValue = FMath::Lerp(StartValue, EndValue, Alpha);
	mSubProgressbar->SetPercent(CurrentValue);
	
	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		ElapsedTime = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Emerald, "Progress Animation Complete!");
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Emerald, "Progress Animation Complete!");
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Emerald, "Progress Animation Complete!");
		if(mMainProgressbar->GetPercent() != mSubProgressbar->GetPercent())
		{
			mSubProgressbar->SetPercent(mMainProgressbar->GetPercent());
		}
	}
}
