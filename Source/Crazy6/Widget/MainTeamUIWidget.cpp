// Fill out your copyright notice in the Description page of Project Settings.


#include "MainTeamUIWidget.h"
#include "UIHeader.h"
#include "TeamHPProgressBar.h"

void UMainTeamUIWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mPlayerName1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name1")));
	mPlayerName2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name2")));
	mPlayerName3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name3")));
	mPlayerName4 = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name4")));

	mPlayerProgressbar1 = Cast<UTeamHPProgressBar>(GetWidgetFromName(TEXT("Bar1")));
	mPlayerProgressbar2 = Cast<UTeamHPProgressBar>(GetWidgetFromName(TEXT("Bar2")));
	mPlayerProgressbar3 = Cast<UTeamHPProgressBar>(GetWidgetFromName(TEXT("Bar3")));
	mPlayerProgressbar4 = Cast<UTeamHPProgressBar>(GetWidgetFromName(TEXT("Bar4")));
	
}

void UMainTeamUIWidget::UpdateTeamDisplay(TArray<FString> PlayerNames, TArray<float> PlayerHPs)
{
	if (PlayerNames.Num() != PlayerHPs.Num())
	{
		return;
	}
	switch (PlayerNames.Num())
	{
	case 1:
		{
			mPlayerName1->SetText(FText::FromString(PlayerNames[0]));
			mPlayerName2->SetText(FText::FromString(TEXT("")));
			mPlayerName3->SetText(FText::FromString(TEXT("")));
			mPlayerName4->SetText(FText::FromString(TEXT("")));

			mPlayerProgressbar1->SetHPPercent(PlayerHPs[0]);
			mPlayerProgressbar2->SetHPPercent(0);
			mPlayerProgressbar3->SetHPPercent(0);
			mPlayerProgressbar4->SetHPPercent(0);
		}
		break;
	case 2:
		{
			mPlayerName1->SetText(FText::FromString(PlayerNames[0]));
			mPlayerName2->SetText(FText::FromString(PlayerNames[1]));
			mPlayerName3->SetText(FText::FromString(TEXT("")));
			mPlayerName4->SetText(FText::FromString(TEXT("")));

			mPlayerProgressbar1->SetHPPercent(PlayerHPs[0]);
			mPlayerProgressbar2->SetHPPercent(PlayerHPs[1]);
			mPlayerProgressbar3->SetHPPercent(0);
			mPlayerProgressbar4->SetHPPercent(0);
		}
		break;
	case 3:
		{
			mPlayerName1->SetText(FText::FromString(PlayerNames[0]));
			mPlayerName2->SetText(FText::FromString(PlayerNames[1]));
			mPlayerName3->SetText(FText::FromString(PlayerNames[2]));
			mPlayerName4->SetText(FText::FromString(TEXT("")));

			mPlayerProgressbar1->SetHPPercent(PlayerHPs[0]);
			mPlayerProgressbar2->SetHPPercent(PlayerHPs[1]);
			mPlayerProgressbar3->SetHPPercent(PlayerHPs[2]);
			mPlayerProgressbar4->SetHPPercent(0);
		}
		break;
	case 4:
		{
			mPlayerName1->SetText(FText::FromString(PlayerNames[0]));
			mPlayerName2->SetText(FText::FromString(PlayerNames[1]));
			mPlayerName3->SetText(FText::FromString(PlayerNames[2]));
			mPlayerName4->SetText(FText::FromString(PlayerNames[3]));

			mPlayerProgressbar1->SetHPPercent(PlayerHPs[0]);
			mPlayerProgressbar2->SetHPPercent(PlayerHPs[1]);
			mPlayerProgressbar3->SetHPPercent(PlayerHPs[2]);
			mPlayerProgressbar4->SetHPPercent(PlayerHPs[3]);
		}
		break;
	default:
		{
			return;
		}
	}
}
