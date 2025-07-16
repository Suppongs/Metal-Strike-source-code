// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyController.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API ALobbyController : public APlayerController
{
	GENERATED_BODY()
protected:
	//Room Widget Load
	UPROPERTY()
	TSubclassOf<class UUserWidget>mRoomUIWidgetClass;
	UPROPERTY()
	TObjectPtr<class URoomWidget>mRoomUIWidget;

public:
	ALobbyController();
	virtual void BeginPlay() override;

	UFUNCTION(Server, Unreliable)
	void SetDifficultyOption();
	
};
