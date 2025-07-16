// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound//SoundMix.h"
#include "GameFramework/GameUserSettings.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceOption.h"
#include "UIHeader.h"

UOptionWidget::UOptionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
		////Loading Master Sound
        static ConstructorHelpers::FObjectFinder<USoundClass> SoundClassFinder(TEXT("/Engine/EngineSounds/Master.Master"));
        if (SoundClassFinder.Succeeded())
        {
            mSoundClass = SoundClassFinder.Object;
        }
        static ConstructorHelpers::FObjectFinder<USoundMix> SoundMixFinder(TEXT("/Game/Dev/UI/Sound/NewSoundMix.NewSoundMix"));
        if (SoundMixFinder.Succeeded())
        {
            mSoundMix = SoundMixFinder.Object;
        }
}
void UOptionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	mVolumeSlider = Cast<USlider>(GetWidgetFromName(TEXT("Volume")));
	mResolutionComboBox = Cast<UComboBoxString>(GetWidgetFromName(TEXT("Resolution")));
	
	mResolutionComboBox->AddOption(TEXT("1920x1080"));
	mResolutionComboBox->AddOption(TEXT("1280x720"));

	mVolumeSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::VolumeSet);
	mResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UOptionWidget::ResolutionSet);
}

void UOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UCrazy6GameInstanceOption* GameOption = GetGameInstance()->GetSubsystem<UCrazy6GameInstanceOption>();

	// Current Volume
	if (mSoundClass)
	{
		mVolumeSlider->SetValue(GameOption->GetCurrentVolume());
	}

	// Current Resolution
	FIntPoint CurrentResolution = GameOption->GetCurrentResolution();
	FString CurrentResolutionString = FString::Printf(TEXT("%dx%d"), CurrentResolution.X, CurrentResolution.Y);
	mResolutionComboBox->SetSelectedOption(CurrentResolutionString);
}

void UOptionWidget::VolumeSet(float Value)
{
	if (mSoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(
			GetWorld(),
			mSoundMix,
			mSoundClass,            
			Value,                 
			1.0f,                   
			0.0f,                  
			true                   
		);


		UGameplayStatics::PushSoundMixModifier(GetWorld(), mSoundMix);
		
		//Save
		UCrazy6GameInstanceOption* GameOption = GetGameInstance()->GetSubsystem<UCrazy6GameInstanceOption>();
		GameOption->SetCurrentVolume(Value);
	}
}

void UOptionWidget::ResolutionSet(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	//Save
	UCrazy6GameInstanceOption* GameOption = GetGameInstance()->GetSubsystem<UCrazy6GameInstanceOption>();

	if (ESelectInfo::OnMouseClick)
	{
		if (SelectedItem == "1920x1080")
		{
			Settings->SetScreenResolution(FIntPoint(1920, 1080));
			Settings->ApplySettings(false);
			GameOption->SetCurrentResolution(FIntPoint(1920, 1080));
		}
		else if (SelectedItem == "1280x720")
		{
			Settings->SetScreenResolution(FIntPoint(1280, 720));
			Settings->ApplySettings(false);
			GameOption->SetCurrentResolution(FIntPoint(1280, 720));
		}
	}
	else
	{
		return;
	}
}


