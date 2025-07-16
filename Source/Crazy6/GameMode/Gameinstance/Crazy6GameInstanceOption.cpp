// Fill out your copyright notice in the Description page of Project Settings.


#include "Crazy6GameInstanceOption.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound//SoundMix.h"
#include "GameFramework/GameUserSettings.h"

UCrazy6GameInstanceOption::UCrazy6GameInstanceOption()
{
	////Loading Master Sound
	static ConstructorHelpers::FObjectFinder<USoundClass> SoundClassFinder(TEXT("/Engine/EngineSounds/Master.Master"));
	if (SoundClassFinder.Succeeded())
	{
		mSoundClass = SoundClassFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundMix> SoundMixFinder(
		TEXT("/Game/Dev/UI/Sound/NewSoundMix.NewSoundMix"));
	if (SoundMixFinder.Succeeded())
	{
		mSoundMix = SoundMixFinder.Object;
	}
}

void UCrazy6GameInstanceOption::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	//Sound Set
	if (mSoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(
			GetWorld(),
			mSoundMix,
			mSoundClass,
			mCurrentVolume,
			1.0f,
			0.0f,
			true
		);


		UGameplayStatics::PushSoundMixModifier(GetWorld(), mSoundMix);
	}
	
	//Resolution Set
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	Settings->SetScreenResolution(mCurrentResolution);
	Settings->ApplySettings(false);
}

void UCrazy6GameInstanceOption::Deinitialize()
{
	Super::Deinitialize();
}
