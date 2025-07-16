// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "OptimizationManager.generated.h"

UCLASS()
class CRAZY6_API AOptimizationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AOptimizationManager();

protected:
	void BeginPlay() override;

	UFUNCTION()
	void SetOptimizationForAllActors(float NewCullDistance);

	
	
};
