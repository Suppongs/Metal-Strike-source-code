// Fill out your copyright notice in the Description page of Project Settings.


#include "StartPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../Global/InputData.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../GameMode/Gameinstance/Crazy6GameInstance.h"
#include "../Widget/StartWidget.h"

AStartPlayerController::AStartPlayerController()
{
}

void AStartPlayerController::BeginPlay()
{
	Super::BeginPlay();
	

	if (IsLocalPlayerController())
	{
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
		bShowMouseCursor = true;
		
		mWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Dev/UI/Widget/StartWidget.StartWidget_C"));
		if (IsValid(mWidgetClass))
		{
			mWidget = CreateWidget<UStartWidget>(GetWorld(),
				mWidgetClass);

			if (IsValid(mWidget))
			{
				mWidget->AddToViewport();
			}
		}
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

		const UDefaultInputData* InputData = GetDefault<UDefaultInputData>();
		Subsystem->AddMappingContext(InputData->mDefaultContext, 0);
	}

}

void AStartPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(IsLocalPlayerController())
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			const UDefaultInputData* InputData = GetDefault<UDefaultInputData>();
			// ESC Input Action Bind
			if (EnhancedInputComponent)
			{
				EnhancedInputComponent->BindAction(InputData->mMenu, ETriggerEvent::Started, this, &AStartPlayerController::OptionEsc);
			}
		}
		
	}
}

void AStartPlayerController::OptionEsc()
{
	mWidget->OptionWidgetEsc();
}
