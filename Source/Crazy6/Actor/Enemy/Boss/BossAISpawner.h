// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BossAISpawner.generated.h"


UCLASS()
class CRAZY6_API UBossAISpawner : public USceneComponent
{
	GENERATED_BODY()

public:	
	UBossAISpawner();

protected:
	virtual void BeginPlay() override;

public:
	void SpawnRandomAI();

		
};
