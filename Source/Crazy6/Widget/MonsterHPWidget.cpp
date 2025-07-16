// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHPWidget.h"
#include "UIHeader.h"
#include "../Actor/Enemy/EnemyBase.h"


void UMonsterHPWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mDamage = Cast<UTextBlock>(GetWidgetFromName(TEXT("Damage")));

	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	int32 AnimCount = WidgetClass->Animations.Num();

	for (int32 i = 0; i < AnimCount; ++i)
	{
		FString Name = WidgetClass->Animations[i]->GetName();

		mAnimMap.Add(Name, WidgetClass->Animations[i]);
		GEngine->AddOnScreenDebugMessage(7, 10.f, FColor::Magenta, FString::Printf(TEXT("Animation : %s"), *Name));
	}
	mTextUp = mAnimMap.FindRef("TextUp_INST");
	mTextUPRED = mAnimMap.FindRef("TextUPRED_INST");
}

void UMonsterHPWidget::SetText(FText text)
{
	if (IsValid(mDamage))
	{
		mDamage->SetText(text);
	}
}


void UMonsterHPWidget::PlayMyAnimation(UWidgetAnimation* Animation)
{
	if (Animation)
	{
		PlayAnimation(Animation);
	}
}

