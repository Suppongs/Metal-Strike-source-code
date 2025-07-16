// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOptionWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	TObjectPtr<class USlider> mVolumeSlider;

	TObjectPtr<class UComboBoxString> mResolutionComboBox;

	TObjectPtr<class USoundClass> mSoundClass;

	TObjectPtr<class USoundMix> mSoundMix;
protected:
	UFUNCTION()
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void VolumeSet(float Value);
	UFUNCTION()
	void ResolutionSet(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	
};
