// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StartPlayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API AStartPlayerGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	AStartPlayerGameMode();
	virtual void BeginPlay() override;
	
};
