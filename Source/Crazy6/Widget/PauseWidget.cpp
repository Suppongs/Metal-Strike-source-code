// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "../Widget/StartWidget.h"
#include "UIHeader.h"
#include "Crazy6/GameMode/MainGameMode.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstance.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"

UPauseWidget::UPauseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	////Loading Option Widget
	mOptionUIWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("/Game/Dev/UI/Widget/OptionWidget.OptionWidget_C"));
	mReStart = Cast<UButton>(GetWidgetFromName(TEXT("ReStart1")));
	if (mReStart)
	{
		mReStart->OnClicked.AddDynamic(this, &UPauseWidget::RestartLevel);
	}

	mMainExit = Cast<UButton>(GetWidgetFromName(TEXT("MainExit1")));
	if (mMainExit)
	{
		mMainExit->OnClicked.AddDynamic(this, &UPauseWidget::QuitGame);
	}

	mOption = Cast<UButton>(GetWidgetFromName(TEXT("Option1")));
	if (mOption)
	{
		mOption->OnClicked.AddDynamic(this, &UPauseWidget::PauseOption);
	}
}

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void UPauseWidget::RestartLevel()
{
	if (AMainGameMode* GameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->OnRestart.Broadcast();
	}
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void UPauseWidget::QuitGame()
{
	// Session
	UCrazy6GameInstanceSubsystem* mGameInstSubsystem = GetWorld()->GetGameInstance<UCrazy6GameInstance>()->GetSubsystem<
		UCrazy6GameInstanceSubsystem>();
	IOnlineSessionPtr mSessionInterface = mGameInstSubsystem->GetSessionInterface();
	mSessionInterface->DestroySession(mGameInstSubsystem->GetCurrentSessionName());
	RemoveFromParent();
	UGameplayStatics::OpenLevel(GetWorld(),TEXT("StartLevel"));
}

void UPauseWidget::PauseOption()
{
	if (mOptionUIWidgetClass)
	{
		// create
		mOptionUIWidget = CreateWidget<UUserWidget>(GetWorld(), mOptionUIWidgetClass);
		if (mOptionUIWidget)
		{
			mOptionUIWidget->AddToViewport();
			IsOptionWidget = true;
		}
	}

}






