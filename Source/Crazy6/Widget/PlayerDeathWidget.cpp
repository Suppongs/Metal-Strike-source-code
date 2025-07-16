// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDeathWidget.h"
#include "UIHeader.h"
#include "Kismet/GameplayStatics.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstance.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"

void UPlayerDeathWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	mMainButton = Cast<UButton>(GetWidgetFromName(TEXT("GotoMain")));
	if (mMainButton)
	{
		mMainButton->OnClicked.AddDynamic(this, &UPlayerDeathWidget::BacktoMain);
	}
	mServerText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ServerText")));
	if (mServerText)
	{
		mServerText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerDeathWidget::BacktoMain()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC && PC->GetLocalRole() == ROLE_Authority)
	{
		mServerText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// Session
		UCrazy6GameInstanceSubsystem* mGameInstSubsystem = GetWorld()->GetGameInstance<UCrazy6GameInstance>()->GetSubsystem<
			UCrazy6GameInstanceSubsystem>();
		IOnlineSessionPtr mSessionInterface = mGameInstSubsystem->GetSessionInterface();
		mSessionInterface->DestroySession(mGameInstSubsystem->GetCurrentSessionName());
		RemoveFromParent();
		UGameplayStatics::OpenLevel(GetWorld(),TEXT("StartLevel"));
		GetWorld()->GetFirstPlayerController()->ClientTravel("/Game/Dev/UI/Lev/StartLevel", TRAVEL_Absolute);
	}
}
