// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingController.h"
#include "../GameMode/LoadingGameMode.h"
#include "../Widget/LoadingWidget.h"
#include "../Setting/LoadingSettings.h"

void ALoadingController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
	
	const ULoadingSettings* LS = GetDefault<ULoadingSettings>();
	LoadingWidget = CreateWidget<ULoadingWidget>(this, LS->mWidgetClass);

	if (LoadingWidget)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandleChangeTexture, this,
											   &ALoadingController::ChangeRandomTexture, 5.0f, true);
		ChangeRandomTexture();
		LoadingWidget->AddToViewport(10000);
	}
}

void ALoadingController::ChangeRandomTexture()
{
	const ULoadingSettings* LS = GetDefault<ULoadingSettings>();
	if (LS->mTextureArray.Num() == 0)
	{
		return;
	}

	// RandomIndex
	int32 RandomIndex = FMath::RandRange(0, LS->mTextureArray.Num() - 1);

	// TextureLoad
	UTexture2D* RandomTexture = LS->mTextureArray[RandomIndex].IsValid()
									? LS->mTextureArray[RandomIndex].Get()
									: LS->mTextureArray[RandomIndex].LoadSynchronous();

	if (RandomTexture)
	{
		LoadingWidget->SetTexture(RandomTexture);
	}
}

