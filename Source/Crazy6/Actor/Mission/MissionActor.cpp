// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionActor.h"
#include "MissionManager.h"
#include "Objectives/MissionObjectiveActor.h"
#include "Objectives/TriggerMissionObjective.h"
#include "Objectives/KillMIssionObjective.h"
#include "Objectives/EliteKillMIssionObjective.h"
#include "Crazy6/Widget/MissionWidget.h"
#include "Crazy6/DebugMacros.h"
#include "Crazy6/Player/PlayerBase.h"
#include "Net/UnrealNetwork.h"
AMissionActor::AMissionActor()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
}

void AMissionActor::MissionStart()
{
	// Server Call
	GEngine->AddOnScreenDebugMessage(2, 15, FColor::Cyan, TEXT("Mission Start!"));

	if (HasAuthority())
	{
		if (true == mObjectives.IsEmpty())
		{
			//LOG_ERROR(TEXT("Objectives not found"));
			return;
		}

		for (AMissionObjectiveActor* objective : mObjectives)
		{
			if (objective)
			{
				objective->SetMissionOwner(this);
				if (AKillMIssionObjective* killObjective = Cast<AKillMIssionObjective>(objective))
				{
					mKillObjective = killObjective;
				}
				else if (AEliteKillMIssionObjective* EliteObjective = Cast<AEliteKillMIssionObjective>(objective))
				{
					mEliteObjective = EliteObjective;
				}
				else if (ATriggerMissionObjective* TriggerObjective = Cast<ATriggerMissionObjective>(objective))
				{
					mTriggerObjective = TriggerObjective;
				}
				objective->StartObjective();
			}
		}
		mObjectivesNum = mObjectives.Num();
	}
		UpdateWidgetContent();
	
}


void AMissionActor::EndMission()
{

	// TODO : KTHb
	// if MissionComplete. It is Reward Logic.z
}

void AMissionActor::MissionComplete()
{
	// Server Call
	if (nullptr != mManager)
	{
		EndMission();
		mManager->MissionComplete();
	}
}

void AMissionActor::Callback_ObjectiveReflect()
{
	// ==== May be Not Use. Ths Func not Ref anywhere. ====
	//if (IsValid(mCurrentObjective))
	//{
	/* UI */

	//mCurrentObjective->mObjectiveUseData.ObjectiveCount;
	//mCurrentObjective->mObjectiveUseData.TargetNumber;
	//}

	// Provides UI information suitable for the Kill mission
	// Example: A mission where the player needs to defeat enemies
	//Objective->mObjectiveUseData.ActorName; // Mission title (e.g., Defeat Enemies)
	//Objective->mObjectiveUseData.Content; // Mission description (e.g., Defeat the enemies)
	//Objective->mObjectiveUseData.ObjectiveCount; // Current progress
	//Objective->mObjectiveUseData.TargetNumber; // Target number of enemies
}

bool AMissionActor::IsMissionComplete()
{
	bool Result = true;
	for (AMissionObjectiveActor* Objective : mObjectives)
	{
		if (IsValid(Objective) && !Objective->mObjectiveUseData.bComplete)
		{
			Result = false;
			break; 
		}
	}

	return Result;
}

void AMissionActor::MissionUpdate()
{
	// Server Call
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("MissionUpdate."));

	if (true == IsMissionComplete())
	{
		MissionComplete();
	}
}

void AMissionActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMissionActor, mObjectives);
	DOREPLIFETIME(AMissionActor, mKillObjective);
	DOREPLIFETIME(AMissionActor, mEliteObjective);
	DOREPLIFETIME(AMissionActor, mTriggerObjective);
	DOREPLIFETIME(AMissionActor, mObjectivesNum);
	DOREPLIFETIME(AMissionActor, Change); 
	
}


void AMissionActor::UpdateWidgetContent()
{
		TArray<AActor*> CurrentPlayers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), CurrentPlayers);

		for (AActor* Player : CurrentPlayers)
		{
			CurrentPlayer = Cast<APlayerBase>(Player);
			if(IsValid(CurrentPlayer->GetMissionWidget()))
			{
				if (1 == mObjectivesNum)
				{
					if (mKillObjective)
					{
						CurrentPlayer->GetMissionWidget()->SetOneMission(mKillObjective->mObjectiveUseData.ActorName,
															  mKillObjective->mObjectiveUseData.Content);
						CurrentPlayer->GetMissionWidget()->SetOneMissionNumber(mKillObjective->mObjectiveUseData.ObjectiveCount,
																 mKillObjective->mObjectiveUseData.TargetNumber);
						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,FString::Printf(TEXT("Kill Objective Change !!!!")));
					}
					else if (mEliteObjective)
					{
						CurrentPlayer->GetMissionWidget()->SetOneMission(mEliteObjective->mObjectiveUseData.ActorName,
															  mEliteObjective->mObjectiveUseData.Content);
						CurrentPlayer->GetMissionWidget()->SetOneMissionNumber(mEliteObjective->mObjectiveUseData.ObjectiveCount,
																 mEliteObjective->mObjectiveUseData.TargetNumber);
						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,FString::Printf(TEXT("Elite Objective Change !!!!")));
					}
					else if (mTriggerObjective)
					{
						CurrentPlayer->GetMissionWidget()->SetOneMission(TEXT("Find Point Mission"),mTitle);
						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,FString::Printf(TEXT("Trigger Objective Change !!!!")));
					}
				}
				else if (2 == mObjectivesNum)
				{
					CurrentPlayer->GetMissionWidget()->SetTwoMission(mObjectives[0]->mObjectiveUseData.Content,mObjectives[1]->mObjectiveUseData.Content);
					CurrentPlayer->GetMissionWidget()->SetTwoMissionNumber(mObjectives[0]->mObjectiveUseData.ObjectiveCount,
						mObjectives[0]->mObjectiveUseData.TargetNumber,
						mObjectives[1]->mObjectiveUseData.ObjectiveCount,
						mObjectives[1]->mObjectiveUseData.TargetNumber
						);
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,FString::Printf(TEXT("Two Alpha!  Objective Change !!!!")));
				}

			/*	if(HasAuthority())
				{
					ChangeWidget();
				}*/
			}
		}
	}

void AMissionActor::OnRep_Change()
{
	MissionChangeForWidget();
}

void AMissionActor::MissionChangeForWidget()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("OnRep_MissionWidgetChange")));

	TArray<AActor*> CurrentPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), CurrentPlayers);

	for (AActor* Player : CurrentPlayers)
	{
		CurrentPlayer = Cast<APlayerBase>(Player);
		if (IsValid(CurrentPlayer->GetMissionWidget()))
		{
			if (1 == mObjectivesNum)
			{
				if (mKillObjective)
				{
					CurrentPlayer->GetMissionWidget()->SetOneMission(mKillObjective->mObjectiveUseData.ActorName,
						mKillObjective->mObjectiveUseData.Content);
					CurrentPlayer->GetMissionWidget()->SetOneMissionNumber(mKillObjective->mObjectiveUseData.ObjectiveCount,
						mKillObjective->mObjectiveUseData.TargetNumber);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Kill Objective Change !!!!")));
				}
				else if (mEliteObjective)
				{
					CurrentPlayer->GetMissionWidget()->SetOneMission(mEliteObjective->mObjectiveUseData.ActorName,
						mEliteObjective->mObjectiveUseData.Content);
					CurrentPlayer->GetMissionWidget()->SetOneMissionNumber(mEliteObjective->mObjectiveUseData.ObjectiveCount,
						mEliteObjective->mObjectiveUseData.TargetNumber);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Elite Objective Change !!!!")));
				}
				else if (mTriggerObjective)
				{
					CurrentPlayer->GetMissionWidget()->SetOneMission(TEXT("Find Point Mission"), mTitle);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Trigger Objective Change !!!!")));
				}
			}
			else if (2 == mObjectivesNum)
			{
				CurrentPlayer->GetMissionWidget()->SetTwoMission(mObjectives[0]->mObjectiveUseData.Content, mObjectives[1]->mObjectiveUseData.Content);
				CurrentPlayer->GetMissionWidget()->SetTwoMissionNumber(mObjectives[0]->mObjectiveUseData.ObjectiveCount,
					mObjectives[0]->mObjectiveUseData.TargetNumber,
					mObjectives[1]->mObjectiveUseData.ObjectiveCount,
					mObjectives[1]->mObjectiveUseData.TargetNumber
				);
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Two Alpha!  Objective Change !!!!")));
			}
		}
	}
}
	
