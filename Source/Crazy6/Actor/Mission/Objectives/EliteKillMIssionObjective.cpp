// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteKillMIssionObjective.h"
#include "Crazy6/Widget/EpicAppearanceWidget.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Net/UnrealNetwork.h"

AEliteKillMIssionObjective::AEliteKillMIssionObjective()
// Appearance UI Data
{
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UIAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/WarningWidget.WarningWidget_C'"));

	if (UIAsset.Succeeded())
	{
		mCrunchWidgetClass = UIAsset.Class;
	}
}

void AEliteKillMIssionObjective::StartObjective()
{
	Super::StartObjective();

	if (HasAuthority())
	{
		// Server UI
		if (!GetWorld()->bIsTearingDown && IsValid(mCrunchWidgetClass))
		{
			mCrunchWidget = CreateWidget<UEpicAppearanceWidget>(GetWorld(),
			                                                    mCrunchWidgetClass);

			if (IsValid(mCrunchWidget))
			{
				mCrunchWidget->AddToViewport();
				mCrunchWidget->PlayMyAnimation();
				GetWorld()->GetTimerManager().ClearTimer(VisibleTime);
				GetWorld()->GetTimerManager().SetTimer(VisibleTime, this,
				                                       &AEliteKillMIssionObjective::VisibleWidget, 2.0f, false);
			}
		}
		// Client UI
		ChangeCruchWidget();
	}
}


void AEliteKillMIssionObjective::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEliteKillMIssionObjective, IsCruchWidget);
}

void AEliteKillMIssionObjective::VisibleWidget()
{
	// Call Client

	mCrunchWidget->RemoveFromParent();

	// TODO: @KYJ
	// Display the Elite Mission, When Mission Start apear
}

void AEliteKillMIssionObjective::ChangeCruchWidget()
{
	if (HasAuthority())
	{
		if (true == IsCruchWidget)
		{
			IsCruchWidget = false;
		}
		else
		{
			IsCruchWidget = true;
		}
	}
}

void AEliteKillMIssionObjective::OnRep_IsCruchWidget()
{
	if (!GetWorld()->bIsTearingDown && IsValid(mCrunchWidgetClass))
	{
		mCrunchWidget = CreateWidget<UEpicAppearanceWidget>(GetWorld(),
		                                                    mCrunchWidgetClass);

		if (IsValid(mCrunchWidget))
		{
			mCrunchWidget->AddToViewport();
			mCrunchWidget->PlayMyAnimation();
			GetWorld()->GetTimerManager().ClearTimer(VisibleTime);
			GetWorld()->GetTimerManager().SetTimer(VisibleTime, this,
			                                       &AEliteKillMIssionObjective::VisibleWidget, 2.0f, false);
		}
	}
}
