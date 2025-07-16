// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"
#include "UIHeader.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstance.h"

UStartWidget::UStartWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	////Loading Session Widget
	{
		static ConstructorHelpers::FClassFinder<UUserWidget>
			LobbyUIAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/Lobby.Lobby_C'"));

		if (LobbyUIAsset.Succeeded())
		{
			mLobbyUIWidgetClass = LobbyUIAsset.Class;
		}
	}
	////Loading Option Widget
	{
		static ConstructorHelpers::FClassFinder<UUserWidget>
			OptionUIAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/OptionWidget.OptionWidget_C'"));

		if (OptionUIAsset.Succeeded())
		{
			mOptionUIWidgetClass = OptionUIAsset.Class;
		}
	}
}
void UStartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	mStartButton = Cast<UButton>(GetWidgetFromName(TEXT("Start")));
	if (mStartButton)
	{
		mStartButton->OnClicked.AddDynamic(this, &UStartWidget::StartGame);
	}

	mOptionButton = Cast<UButton>(GetWidgetFromName(TEXT("Option")));
	if (mOptionButton)
	{
		mOptionButton->OnClicked.AddDynamic(this, &UStartWidget::OptionWidgetOpen);
	}

	mExitButton = Cast<UButton>(GetWidgetFromName(TEXT("Exit")));
	if (mExitButton)
	{
		mExitButton->OnClicked.AddDynamic(this, &UStartWidget::QuitGame);
	}


}

void UStartWidget::StartGame()
{
	// Open LobbyWidget
	if (mLobbyUIWidgetClass)
	{
		// create
		mLobbyUIWidget = CreateWidget<UUserWidget>(GetWorld(), mLobbyUIWidgetClass);
		if (mLobbyUIWidget)
		{
			mLobbyUIWidget->AddToViewport();
		}
	}
}

void UStartWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UStartWidget::OptionWidgetOpen()
{
	if (mOptionUIWidgetClass)
	{
		// create
		mOptionUIWidget = CreateWidget<UUserWidget>(GetWorld(), mOptionUIWidgetClass);
		if (mOptionUIWidget)
		{
			mOptionUIWidget->AddToViewport();
		}
	}
}

void UStartWidget::OptionWidgetEsc()
{
	if (mOptionUIWidget && mOptionUIWidget->IsInViewport())
	{
		mOptionUIWidget->RemoveFromParent();
		AddToViewport();
	}
}
