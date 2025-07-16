// Fill out your copyright notice in the Description page of Project Settings.


#include "EpicAppearanceWidget.h"
#include "UIHeader.h"

void UEpicAppearanceWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	int32 AnimCount = WidgetClass->Animations.Num();

	for (int32 i = 0; i < AnimCount; ++i)
	{
		FString Name = WidgetClass->Animations[i]->GetName();

		mAnimMap.Add(Name, WidgetClass->Animations[i]);
		GEngine->AddOnScreenDebugMessage(7, 10.f, FColor::Magenta, FString::Printf(TEXT("Animation : %s"), *Name));
	}
	mWarningAnim = mAnimMap.FindRef("WarningAnim_INST");
}

void UEpicAppearanceWidget::PlayMyAnimation()
{
	if (mWarningAnim)
	{
		if (false == IsPlayingAnimation())
		{
			PlayAnimation(mWarningAnim);
		}
	}
}


