// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Widget/MissionWidget.h"
#include "MissionInfo.h"
#include "MissionActor.generated.h"

class AMissionManager;
class AMissionObjectiveActor;
class AEliteKillMIssionObjective;
class AKillMIssionObjective;
class ATriggerMissionObjective;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllPlayersReady);

UCLASS()
class CRAZY6_API AMissionActor : public AActor
{
	GENERATED_BODY()

public:
	AMissionActor();
	
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnAllPlayersReady OnAllPlayersReady;

public:
	//UFUNCTION(NetMulticast, Reliable)
	void SetManager(AMissionManager* Manager) { mManager = Manager; }


	AMissionManager* GetManager() { return mManager; }

public:
	/* Call by MissionManager, When Mission Start */
	virtual void MissionStart();
	/* Call by MissionManager, When Missions are Completed */
	virtual void EndMission();
	/* This function is called when all Objectives tasks are completed. */
	virtual void MissionComplete();
	/* The Objective is reflected, for example, when a monster dies or a specific trigger is activated. */
	void Callback_ObjectiveReflect();
	/* True is returned, when all Objectives are completed. */
	bool IsMissionComplete();
	/* Call when Objective is completed */
	void MissionUpdate();


	// ==================== [ Networking ] ====================
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		// ======================================================


protected:
	/* An object that manages missions. */
	UPROPERTY()
	TObjectPtr<AMissionManager> mManager = nullptr;

	/* Main or Sub Mission */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMissionType MissionType;

	/* Mission Title */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	FString mTitle;

	/* for objectives container */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission", Replicated)
	TArray<AMissionObjectiveActor*> mObjectives;

	UPROPERTY(Replicated)
	int32 mObjectivesNum;

	UPROPERTY(Replicated)
	TObjectPtr<AKillMIssionObjective> mKillObjective;

	UPROPERTY(Replicated)
	TObjectPtr<AEliteKillMIssionObjective> mEliteObjective;

	UPROPERTY(Replicated)
	TObjectPtr <ATriggerMissionObjective> mTriggerObjective;

	UPROPERTY(ReplicatedUsing = OnRep_Change)
	bool Change = false;

	class APlayerBase* CurrentPlayer = nullptr;

public:
	void UpdateWidgetContent();
	
	void ChangeWidget()
	{
		if(Change == true)
		{
			Change = false;
		}
		else
		{
			Change = true;
		}
	} 
	UFUNCTION()
	void OnRep_Change();


	UFUNCTION()
	void MissionChangeForWidget();


};
