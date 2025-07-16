// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateSessionWidget.h"
#include "UIHeader.h"
#include <regex>
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstance.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"

void UCreateSessionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	mTeamName = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("TEAMNAME")));
	mSpinBox = Cast<USpinBox>(GetWidgetFromName(TEXT("SpinBox")));
	mSessionButton = Cast<UButton>(GetWidgetFromName(TEXT("SessionButton")));
	if (mSessionButton)
	{
		mSessionButton->OnClicked.AddDynamic(this, &UCreateSessionWidget::CreateSession);
	}
	mQuitButton = Cast<UButton>(GetWidgetFromName(TEXT("SessionButton2")));
	if (mQuitButton)
	{
		mQuitButton->OnClicked.AddDynamic(this, &UCreateSessionWidget::QuitUI);
	}
	mNametext = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));
	mNametext->SetVisibility(ESlateVisibility::Hidden);
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	int32 AnimCount = WidgetClass->Animations.Num();

	for (int32 i = 0; i < AnimCount; ++i)
	{
		FString Name = WidgetClass->Animations[i]->GetName();

		mAnimMap.Add(Name, WidgetClass->Animations[i]);
	}
	mTextAnimation = mAnimMap.FindRef("TextAnimation_INST");
}

bool UCreateSessionWidget::IsEnglishAndNumericOnly(const FString& InputText)
{
	std::wregex EnglishAndNumericRegex(L"^[A-Za-z0-9]*$");
	return std::regex_match(*InputText, EnglishAndNumericRegex);
}

void UCreateSessionWidget::CreateSession()
{
	UCrazy6GameInstanceSubsystem* GameInstanceSubsystem = GetWorld()->GetGameInstance<UCrazy6GameInstance>()->GetSubsystem<UCrazy6GameInstanceSubsystem>();
	
	FString TeamNameText = mTeamName->GetText().ToString();
	if (!IsEnglishAndNumericOnly(TeamNameText))
	{
		if (mTextAnimation)
		{
			PlayAnimation(mTextAnimation);
		}
		return; 
	}
	// FText -> FName Cast
	FName SessionName = FName(*TeamNameText);

	GameInstanceSubsystem->CreateSession(static_cast<int>(mSpinBox->GetValue()), SessionName);
	RemoveFromParent();
	
}

void UCreateSessionWidget::QuitUI()
{
	RemoveFromParent();
}
