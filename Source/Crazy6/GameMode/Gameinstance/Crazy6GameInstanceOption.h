// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Crazy6GameInstanceOption.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UCrazy6GameInstanceOption : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	float GetCurrentVolume() { return mCurrentVolume; }
	void SetCurrentVolume(float Volume) { mCurrentVolume = Volume; }

	FIntPoint GetCurrentResolution() { return mCurrentResolution; }
	void SetCurrentResolution(FIntPoint Resolution) { mCurrentResolution = Resolution; }
	

protected:
	UCrazy6GameInstanceOption();
	
	//Sound
	float mCurrentVolume = 0.5f;

	TObjectPtr<class USoundClass> mSoundClass;
	TObjectPtr<class USoundMix> mSoundMix;
	//Resolution
	FIntPoint mCurrentResolution = FIntPoint(1920, 1080); 
	

};

