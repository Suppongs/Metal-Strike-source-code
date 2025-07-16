// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TestBossMap.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API ATestBossMap : public ATriggerBox
{
	GENERATED_BODY()
public:
	ATestBossMap();

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor,AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	FString LevelToLoad = "Bridge_P?listen";
	
};
