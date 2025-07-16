// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeModeWidget.h"
#include "UIHeader.h"
#include "../Player/PlayerBase.h"


void UChangeModeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//first
	CurrentMode = true;

	mPlayer = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	mMode = Cast<UTextBlock>(GetWidgetFromName(TEXT("Mode")));

	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	int32 AnimCount = WidgetClass->Animations.Num();

	for (int32 i = 0; i < AnimCount; ++i)
	{
		FString Name = WidgetClass->Animations[i]->GetName();

		mAnimMap.Add(Name, WidgetClass->Animations[i]);
		GEngine->AddOnScreenDebugMessage(7, 10.f, FColor::Magenta, FString::Printf(TEXT("Animation : %s"),*Name));
	}
	mDissolveIn = mAnimMap.FindRef("DissolveIn_INST");
	mDissolveOut = mAnimMap.FindRef("DissolveOut_INST");


}


void UChangeModeWidget::SetText(const FText &text)
{
	mMode->SetText(text);
}

void UChangeModeWidget::ChangeMode()
{
	// A -> A
	if (true == CanChange)
	{
		if (true == CurrentMode)
		{
			//current
			CurrentMode = mPlayer->GetIsAMode();
			// if B
			if (false == CurrentMode)
			{
				//Delete A
				if (mDissolveOut)
				{
					PlayAnimation(mDissolveOut);
				}
				CanChange = false;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UChangeModeWidget::OnDissolveOutFinishedChangeB, 0.5f, false);
			}

		}
		else 
		{
			// current
			CurrentMode = mPlayer->GetIsAMode();

			//if A
			if (true == CurrentMode)
			{
				//Delete B
				if (mDissolveOut)
				{
					PlayAnimation(mDissolveOut);
				}
				CanChange = false;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UChangeModeWidget::OnDissolveOutFinishedChangeA, 0.5f, false);
			}
			//현재가 B모드라면 변경X
		}
	}
	else
	{
		return;
	}
}

void UChangeModeWidget::OnDissolveOutFinishedChangeA()
{
	//Set Text
	SetText(FText::FromString(TEXT("A")));
	// Play Anim DissolveIn
	if (mDissolveIn)
	{
		PlayAnimation(mDissolveIn);
	}
	CanChange = true;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UChangeModeWidget::OnDissolveOutFinishedChangeB()
{
	//Set Text
	SetText(FText::FromString(TEXT("B")));
	// Play Anim DissolveIn
	if (mDissolveIn)
	{
		PlayAnimation(mDissolveIn);
	}
	CanChange = true;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}





