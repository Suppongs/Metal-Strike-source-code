// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LoadingController.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API ALoadingController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void ChangeRandomTexture();

protected:
	UPROPERTY()
	class ULoadingWidget* LoadingWidget;
	FTimerHandle TimerHandleChangeTexture;

};
