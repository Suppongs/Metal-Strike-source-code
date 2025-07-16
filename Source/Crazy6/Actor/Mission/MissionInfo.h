#pragma once

#include "CoreMinimal.h"
#include "MissionEnum.h"
#include "MissionInfo.generated.h"

USTRUCT(BlueprintType)
struct FObjectiveInfo
{
	GENERATED_USTRUCT_BODY()

	FObjectiveInfo()
		/*: DetailedMissionType(EMissionObjectiveType::None)
		, ActorName(TEXT(""))
		, Content(TEXT(""))
		, TargetNumber(0)*/
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMissionObjectiveType DetailedMissionType = EMissionObjectiveType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString	ActorName =	TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString	Content = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TargetNumber = 0;
}; 

USTRUCT(BlueprintType)
struct FObjectiveUseInfo
{
	GENERATED_USTRUCT_BODY()

	FObjectiveUseInfo()
		/*: ActorName(TEXT(""))
		, Content(TEXT(""))
		, TargetNumber(0)
		, ObjectiveCount(0)
		, bComplete(false)*/
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission|Objective")
	FString	ActorName = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission|Objective")
	FString	Content =TEXT("");;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission|Objective")
	int32 TargetNumber = 0; // Mission Goal Number

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission|Objective")
	int32 ObjectiveCount = 0; // Killing Count in Mission

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission|Objective")
	bool bComplete = false;
};

USTRUCT(BlueprintType)
struct FMissionInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FMissionInfo()
		/*: MissionType(EMissionType::Main)
		, Name(TEXT(""))
		, Objectives()*/
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMissionType MissionType = EMissionType::Main;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString	Name = TEXT("");;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FObjectiveInfo> Objectives = {};
};