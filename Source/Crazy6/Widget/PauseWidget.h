// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 *
 */
UCLASS()
class CRAZY6_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPauseWidget(const FObjectInitializer& ObjectInitializer);
	TObjectPtr<class UUserWidget> GetOptionWidget() { return mOptionUIWidget; }
	bool GetIsOptionWidget() { return IsOptionWidget;}
	void SetIsOptionWidget(bool Option) { IsOptionWidget = Option; }
protected:
	UPROPERTY()
	TObjectPtr<class UButton> mReStart;
	UPROPERTY()
	TObjectPtr<class UButton> mMainExit;
	UPROPERTY()
	TObjectPtr<class UButton> mOption;
	UPROPERTY()
	TSubclassOf<class UUserWidget>mOptionUIWidgetClass;
	UPROPERTY()
	TObjectPtr<class UUserWidget>mOptionUIWidget;
	UPROPERTY()
	bool IsOptionWidget = false;


protected:
	UFUNCTION()
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RestartLevel();
	UFUNCTION()
	void QuitGame();
	UFUNCTION()
	void PauseOption();


};
