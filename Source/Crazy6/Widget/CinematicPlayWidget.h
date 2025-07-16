// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CinematicPlayWidget.generated.h"

/**
 *
 */

class UImage;

UCLASS()
class CRAZY6_API UCinematicPlayWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleMediaOpened(FString OpenedUrl);

	UFUNCTION()
	void HandleMediaEnded();


protected:
	TObjectPtr<class ACinematicPlayActor> mOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> mCinemaImage = nullptr;


};
