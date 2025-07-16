// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamListWidget.h"
#include "UIHeader.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstance.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"
#include "Crazy6/Widget/RoomWidget.h"

void UTeamListWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	mJoinSessionButton = Cast<UButton>(GetWidgetFromName(TEXT("JoinButton")));
	if (IsValid(mJoinSessionButton))
	{
		mJoinSessionButton->OnClicked.AddDynamic(this, &UTeamListWidget::JoinSession);
	}
	mSessionName = Cast<UTextBlock>(GetWidgetFromName(TEXT("RoomName")));
	mNumberOfSession = Cast<UTextBlock>(GetWidgetFromName(TEXT("current")));
	mMaxOfSession= Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxNumber")));
}
void UTeamListWidget::NativeOnListItemObjectSet(UObject* ItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ItemObject);
	//save
	mItemObject = Cast<USessionObject>(ItemObject);
	ListRoomSessionData = mItemObject->MySessionData;
	//Widget Setting
	mSessionName->SetText(FText::FromString(mItemObject->MySessionData->SessionName));
	mNumberOfSession->SetText(FText::AsNumber(mItemObject->MySessionData->CurrentPlayers));
	mMaxOfSession->SetText(FText::AsNumber(mItemObject->MySessionData->MaxPlayers));

}
void UTeamListWidget::JoinSession()
{
	UCrazy6GameInstanceSubsystem*GameInstSubsystem = GetWorld()->GetGameInstance<UCrazy6GameInstance>()->GetSubsystem<UCrazy6GameInstanceSubsystem>();
	GameInstSubsystem->mJoinSession(ListRoomSessionData);
}
