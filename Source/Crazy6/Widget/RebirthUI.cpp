// Fill out your copyright notice in the Description page of Project Settings.


#include "RebirthUI.h"

void URebirthUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mPressImage = Cast<UImage>(GetWidgetFromName(TEXT("PressF")));
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	int32 AnimCount = WidgetClass->Animations.Num();

	for (int32 i = 0; i < AnimCount; ++i)
	{
		FString Name = WidgetClass->Animations[i]->GetName();

		mAnimMap.Add(Name, WidgetClass->Animations[i]);
		GEngine->AddOnScreenDebugMessage(7, 10.f, FColor::Magenta, FString::Printf(TEXT("Animation : %s"), *Name));
	}
	mPressAnim = mAnimMap.FindRef("Press_INST");
	
}

void URebirthUI::PlayMyAnimation()
{
	if (mPressAnim)
	{
		PlayAnimation(mPressAnim, 0.0f,
			0, EUMGSequencePlayMode::Forward,
			1.0f, false);
	}
}

void URebirthUI::StopMyAnimation()
{
	if (mPressAnim && IsAnimationPlaying(mPressAnim))
	{
		StopAnimation(mPressAnim);
	}
}
