// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDailogWidget.h"
#include "Blueprint/WidgetTree.h"
#include "UIHeader.h"


void UItemDailogWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mItemDailog = Cast<UTextBlock>(GetWidgetFromName(TEXT("BindItemDailog")));


	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	int32 AnimCount = WidgetClass->Animations.Num();

	for (int32 i = 0; i < AnimCount; ++i)
	{
		FString Name = WidgetClass->Animations[i]->GetName();

		mAnimMap.Add(Name, WidgetClass->Animations[i]);
		GEngine->AddOnScreenDebugMessage(7, 10.f, FColor::Magenta, FString::Printf(TEXT("Animation : %s"), *Name));
	}
	mInandOut = mAnimMap.FindRef("InandOut_INST");
}


void UItemDailogWidget::SetText(const FString& Text)
{
	if (mItemDailog)
	{
		mItemDailog->SetText(FText::FromString(Text));
	}
}

void UItemDailogWidget::PlayMyAnimation()
{
	if (mInandOut)
	{
		PlayAnimation(mInandOut);
	}
}



