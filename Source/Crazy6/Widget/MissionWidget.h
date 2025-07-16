// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UMissionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> mMissionName;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mMissionConText;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mMissionConText1;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mMissionConText2;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mMissionConText3;

protected:
	virtual void NativeOnInitialized() override;
public:
	// if Mission Use

	//Mission One
	void SetOneMission(FString CurrentMissionName, FString CurrentMissionText);
	void SetOneMissionNumber(int32 CurrentCount, int32 TargetNumber);
	//Mission Two
	void SetTwoMission(FString CurrentMissionText1,FString CurrentMissionText2);
	void SetTwoMissionNumber(int32 CurrentCount1, int32 TargetNumber1,
										int32 CurrentCount2, int32 TargetNumber2);
};
