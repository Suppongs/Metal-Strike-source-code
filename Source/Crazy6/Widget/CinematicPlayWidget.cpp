// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicPlayWidget.h"


#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaSource.h"

#include "Crazy6/Actor/Cinematic/CinematicPlayActor.h"
#include "Crazy6/Actor/Mission/Objectives/TriggerMissionObjective.h"


void UCinematicPlayWidget::NativeConstruct()
{

	Super::NativeConstruct();

	mOwner = Cast<ACinematicPlayActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACinematicPlayActor::StaticClass()));

	if (IsValid(mOwner))
	{
		mCinemaImage->SetBrushFromMaterial(mOwner->GetVideoMT());

		mOwner->GetVideoPlayer()->OnMediaOpened.AddDynamic(this, &UCinematicPlayWidget::HandleMediaOpened);
		mOwner->GetVideoPlayer()->OnEndReached.AddDynamic(this, &UCinematicPlayWidget::HandleMediaEnded);

		mOwner->CinematicPlay();
	}

	mCinemaImage->SetVisibility(ESlateVisibility::Visible);

}

// The Open Media function may return 0 if it has not been fully loaded and prepared.
// This is because the media source is loaded asynchronously.
// Therefore, the duration is obtained through this event.
void UCinematicPlayWidget::HandleMediaOpened(FString OpenedUrl)
{
	ACinematicPlayActor* foundActor = Cast<ACinematicPlayActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACinematicPlayActor::StaticClass()));

	FTimespan time = foundActor->GetVideoPlayer()->GetDuration();
	double videoDuration = time.GetTotalSeconds();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCinematicPlayWidget::RemoveFromParent, videoDuration, false);

}

void UCinematicPlayWidget::HandleMediaEnded()
{

	FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));

	if (CurrentLevelName == "Bridge_P")
	{
		ATriggerMissionObjective* foundPlayTirrigerMissionBox = Cast<ATriggerMissionObjective>(UGameplayStatics::GetActorOfClass(GetWorld(), ATriggerMissionObjective::StaticClass()));

		foundPlayTirrigerMissionBox->mObjectiveUseData.ObjectiveCount = 1; // For jump next mission (Cur Complete Checke)
	}

	mOwner->PlayEndController();

}



