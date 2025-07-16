// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoadingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API ALoadingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALoadingGameMode();

	virtual void BeginPlay() override;
	virtual void Logout(AController* Exiting) override;
	void StartMapTransition();

	UFUNCTION()
	void mServerTravel();

	FString NextMapName;
	
protected:
	FTimerHandle mWidgetTimerHandle;
};
