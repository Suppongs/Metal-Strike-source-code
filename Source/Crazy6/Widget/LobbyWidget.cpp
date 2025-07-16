// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "UIHeader.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"
#include "Crazy6/DebugMacros.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstance.h"

ULobbyWidget::ULobbyWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	////Loading Session Room Widget
	static ConstructorHelpers::FClassFinder<UUserWidget> CreateSessionUIWidgetClassFinder(TEXT("/Game/Dev/UI/Widget/CreateTeam.CreateTeam_C"));
	if (CreateSessionUIWidgetClassFinder.Succeeded())
	{
		mCreateSessionUIWidgetClass = CreateSessionUIWidgetClassFinder.Class;
	}
}

void ULobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	mCreateSessionButton = Cast<UButton>(GetWidgetFromName(TEXT("CreateTeam")));
	if (mCreateSessionButton)
	{
		mCreateSessionButton->OnClicked.AddDynamic(this, &ULobbyWidget::CreateSessionWidgetOpen);
	}

	mFindSessionButton = Cast<UButton>(GetWidgetFromName(TEXT("FindTeam")));
	if (mFindSessionButton)
	{
		mFindSessionButton->OnClicked.AddDynamic(this, &ULobbyWidget::FindSession);
	}

	mQuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitLobby")));
	if (mQuitButton)
	{
		mQuitButton->OnClicked.AddDynamic(this, &ULobbyWidget::QuitLobby);
	}
	mLobbyListView = Cast<UListView>(GetWidgetFromName(TEXT("LobbyListView")));
	mNoJoin = Cast<UTextBlock>(GetWidgetFromName(TEXT("NoJoin")));
	mNoJoin->SetVisibility(ESlateVisibility::Hidden);
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	int32 AnimCount = WidgetClass->Animations.Num();

	for (int32 i = 0; i < AnimCount; ++i)
	{
		FString Name = WidgetClass->Animations[i]->GetName();

		mAnimMap.Add(Name, WidgetClass->Animations[i]);
	}
	mTextAnim = mAnimMap.FindRef("TextAnimation_INST");
}
void ULobbyWidget::CreateSessionWidgetOpen()
{
	if (IsValid(mCreateSessionUIWidgetClass))
	{
		mCreateSessionUIWidget = CreateWidget<UUserWidget>(GetWorld(), mCreateSessionUIWidgetClass);
		if (mCreateSessionUIWidget)
		{
			mCreateSessionUIWidget->AddToViewport();
			UCrazy6GameInstanceSubsystem* GameInstSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>();
			if(GameInstSubsystem)
			{
				GameInstSubsystem->SetLobbyWidget(this);
			}
		}
	}
}
void ULobbyWidget::FindSession()
{
	UCrazy6GameInstanceSubsystem* GameInstSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>();
	if(GameInstSubsystem)
	{
		GameInstSubsystem->SetLobbyWidget(this);
		ULobbyWidget* LobbyWidget = GameInstSubsystem->GetLobbyWidget();
		GEngine->AddOnScreenDebugMessage(
			-1, 
			15, 
			FColor::Blue, 
			FString::Printf(TEXT("LobbyWidget Address: %p"), LobbyWidget));
		GameInstSubsystem->FindSession();
	}
}
void ULobbyWidget::QuitLobby()
{
	RemoveFromParent();
	UCrazy6GameInstanceSubsystem* GameInstSubsystem = GetWorld()->GetGameInstance<UCrazy6GameInstance>()->GetSubsystem<UCrazy6GameInstanceSubsystem>();
	if(GameInstSubsystem)
	{
		GameInstSubsystem->SetLobbyWidget(nullptr);
	}
}

void ULobbyWidget::PlayText()
{
	if (mTextAnim)
	{
		PlayAnimation(mTextAnim);
	}
}

void ULobbyWidget::SessionListReset(UCrazy6GameInstanceSubsystem* GameInstSubsystem)
{
	mLobbyListView->ClearListItems(); //Delete
	TArray<TSharedPtr<FSessionInfo>> Sessions = GameInstSubsystem->GetSessionInfos();
	for (TSharedPtr<FSessionInfo> SessionInfo : Sessions)
	{
		AddSessionToList(SessionInfo); // Create
	}
}
void ULobbyWidget::AddSessionToList(const TSharedPtr<FSessionInfo>& SessionInfo)
{
	USessionObject* ItemObject = NewObject<USessionObject>(this);
	mLobbyListView->AddItem(ItemObject); //Plus List View
	ItemObject->Initialize(SessionInfo);
	if (mLobbyListView == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("mLobbyListView is nullptr"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(5, 1000.f, FColor::Red,
		TEXT("List Add!"));

}