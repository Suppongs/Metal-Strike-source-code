// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemNameWidget.h"
#include "UIHeader.h"

void UItemNameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNameChange")));
}


void UItemNameWidget::SetText(const FText &text)
{
	if (IsValid(mItemName))
	{
		mItemName->SetText(text);
	}
}
