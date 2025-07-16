// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerName.h"

#include "Components/TextBlock.h"

void UPlayerName::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mPlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerName")));
}

void UPlayerName::SetPlayerText(const FText& text)
{
	if(IsValid(mPlayerName))
	{
		mPlayerName->SetText(text);
	}
}
