// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyController.h"
#include "Blueprint/UserWidget.h"
#include  "../Widget/RoomWidget.h"
#include "../GameMode/Gameinstance/Crazy6GameInstance.h"
#include "Crazy6/GameMode/GameState/LobbyGameStateBase.h"


ALobbyController::ALobbyController()
{
}

void ALobbyController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = true;

	mRoomUIWidgetClass = LoadClass<UUserWidget>(
		nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/Room.Room_C'"));

	if (IsValid(mRoomUIWidgetClass))
	{
		mRoomUIWidget = CreateWidget<URoomWidget>(GetWorld(), mRoomUIWidgetClass);
		if (IsValid(mRoomUIWidget))
		{
			mRoomUIWidget->SetSessionInfo(
				GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>()->GetCurrentSessionName());
			mRoomUIWidget->AddToViewport();
			SetDifficultyOption();
		}
	}
}

void ALobbyController::SetDifficultyOption_Implementation()
{
	TArray<FString> PlayerIdArray;
	PlayerIdArray = Cast<ALobbyGameStateBase>(GetWorld()->GetGameState())->GetPlayerIdArray();
	UCrazy6GameInstanceSubsystem* Subsystem = GetWorld()->GetGameInstance<UCrazy6GameInstance>()->GetSubsystem<
		UCrazy6GameInstanceSubsystem>();
	Subsystem->SetDifficultyOption(PlayerIdArray.Num());
	GEngine->AddOnScreenDebugMessage(3, 15, FColor::Magenta,
									 FString::Printf(
										 TEXT("Set DifficultyOptionr:%d"), Subsystem->GetDifficultyOption()));
}

