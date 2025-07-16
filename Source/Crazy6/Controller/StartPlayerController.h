// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StartPlayerController.generated.h"

/**
 *
 */
UCLASS()
class CRAZY6_API AStartPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AStartPlayerController();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void OptionEsc();
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget>	mWidgetClass;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStartWidget> mWidget;
};
