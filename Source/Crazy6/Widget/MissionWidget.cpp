// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionWidget.h"
#include "../../Crazy6/Actor/Mission/MissionInfo.h"
#include "UIHeader.h"
#include "Net/UnrealNetwork.h"

void UMissionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mMissionName = Cast<UTextBlock>(GetWidgetFromName(TEXT("MissionName")));
	mMissionConText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MissionContext")));
	mMissionConText1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("MissionContext1")));
	mMissionConText2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("MissionContext2")));
	mMissionConText3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("MissionContext3")));
}

void UMissionWidget::SetOneMission(FString CurrentMissionName = "", FString CurrentMissionText = "")
{
	FText MissionName = FText::FromString(CurrentMissionName);
	FText MissionText = FText::FromString(CurrentMissionText);

	mMissionName->SetText(MissionName);
	mMissionConText->SetText(FText::FromString(TEXT("")));
	mMissionConText1->SetText(MissionText);
	mMissionConText2->SetText(FText::FromString(TEXT("")));
	mMissionConText3->SetText(FText::FromString(TEXT("")));
}

void UMissionWidget::SetOneMissionNumber(int32 CurrentCount = 0, int32 TargetNumber = 0)
{
	FString TempOutput;
	TempOutput = FString::Printf(TEXT("%d / %d"), CurrentCount, TargetNumber);
	mMissionConText2->SetText(FText::FromString(TempOutput));
}

void UMissionWidget::SetTwoMission(FString CurrentMissionText1 = "", FString CurrentMissionText2 = "")
{
	mMissionName->SetText(FText::FromString(TEXT("Extinction Mission")));
	mMissionConText->SetText(FText::FromString(CurrentMissionText1));
	mMissionConText1->SetText(FText::FromString(TEXT("")));
	mMissionConText2->SetText(FText::FromString(CurrentMissionText2));
	mMissionConText3->SetText(FText::FromString(TEXT("")));

}

void UMissionWidget::SetTwoMissionNumber(int32 CurrentCount1 = 0, int32 TargetNumber1 = 0,
                                         int32 CurrentCount2 = 0, int32 TargetNumber2 = 0)
{
	FString TempOutput1;
	TempOutput1 = FString::Printf(TEXT("%d / %d"), CurrentCount1, TargetNumber1);
	FString TempOutput2;
	TempOutput2 = FString::Printf(TEXT("%d / %d"), CurrentCount2, TargetNumber2);
	mMissionConText1->SetText(FText::FromString(TempOutput1));
	mMissionConText3->SetText(FText::FromString(TempOutput2));
}
