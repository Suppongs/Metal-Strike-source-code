// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainTeamUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UMainTeamUIWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> mPlayerName1;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mPlayerName2;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mPlayerName3;
	UPROPERTY()
	TObjectPtr<class UTextBlock> mPlayerName4;
	
	UPROPERTY()
	TObjectPtr<class UTeamHPProgressBar> mPlayerProgressbar1;
	UPROPERTY()
	TObjectPtr<class UTeamHPProgressBar> mPlayerProgressbar2;
	UPROPERTY()
	TObjectPtr<class UTeamHPProgressBar> mPlayerProgressbar3;
	UPROPERTY()
	TObjectPtr<class UTeamHPProgressBar> mPlayerProgressbar4;
	

protected:
	virtual void NativeOnInitialized() override;
public:
	void UpdateTeamDisplay(TArray<FString> PlayerNames,TArray<float>PlayerHPs);
	
};
