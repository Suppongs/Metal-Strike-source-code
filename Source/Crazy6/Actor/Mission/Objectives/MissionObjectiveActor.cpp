// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionObjectiveActor.h"
#include "../MissionActor.h"
#include "../MissionManager.h"
#include "Crazy6/DebugMacros.h"
#include "Net/UnrealNetwork.h"

AMissionObjectiveActor::AMissionObjectiveActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bActive = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AMissionObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
}


void AMissionObjectiveActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//On_REP
	DOREPLIFETIME(AMissionObjectiveActor, mObjectiveUseData);
	// IF, Do not Replicate, Client No have mOwner 
	DOREPLIFETIME(AMissionObjectiveActor, mOwner);
	
}

void AMissionObjectiveActor::OnRep_mObjectiveUseData()
{
	// Retrieves the widget used in OnRep_mCallMissionStartUI() of the Mission Actor.

	// Updates the real-time kill count during the mission.

	if (IsValid(mOwner) && mObjectiveUseData.ObjectiveCount <= mObjectiveUseData.TargetNumber)
	{
		mOwner->UpdateWidgetContent();
		GEngine->AddOnScreenDebugMessage(2, 5, FColor::Cyan, TEXT("Server Kill Update"));
	}
}

void AMissionObjectiveActor::StartObjective()
{
	// Server Call
	if (HasAuthority())
	{
		bActive = true;
	}
}

void AMissionObjectiveActor::EndObjective()
{

}


// ����� �Ѿ�Դٸ� CallBack_KillMissionReflect ���⿡�� Ÿ�ٸ�ǥ�� �޼��߱� ������ �Ѿ���� ���� 
void AMissionObjectiveActor::Complete()
{
	LOG_MSG(TEXT("Objective Clear"));

	if (HasAuthority())
	{
		bActive = false;
		mObjectiveUseData.bComplete = true;

		if (true == IsValid(mOwner))
		{
			mOwner->MissionUpdate();
		}

		else
		{
			//LOG_ERROR(TEXT("MissionActor isn't exist"));
		}
	}
}

