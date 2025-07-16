// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MissionManager.generated.h"

class AMissionActor;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMissionChangeDelegate, AMissionActor* /*CurrentMisison*/)

UCLASS()
class CRAZY6_API AMissionManager : public AActor
{
	GENERATED_BODY()

public:
	AMissionManager();

protected:
	/* Check if all missions are valid with an iterator and start the first mission. */
	virtual void BeginPlay() override;

public:
	/* Call Only 'CheckAndCompleteMission' function, if current mission is complete. */
	void MissionComplete();
	/* Assign mission, If Main Mission is remain  */
	void NextMission();

	// Get Current Mission
	TObjectPtr<AMissionActor> GetCurrentMission() { return mCurrentMission;}
	
	/* Call When  Mission will change */

	/* Mission Change Delegate */
	FOnMissionChangeDelegate mOnMissionChangeDelegate;

	void CinematicPlay();

	// ==================== [ Networking ] ====================
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:
	UFUNCTION()
	void OnRep_mMissionCompleteUI();

	UFUNCTION()
	void OnRep_NewMissionWidgetChange();
	

	// Map Travel
private:
	FTimerHandle DelayTimerHandle;
    
	UFUNCTION()
	void MapTravelFunction();


	// ======================================================


protected:
	/* Current Mission for TArray Current Mission */
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Mission", Replicated)
	int32 mCurrentMissionNumber = -1;

	UPROPERTY(BlueprintReadOnly, Category = "Mission", ReplicatedUsing = OnRep_mMissionCompleteUI)
	bool mMissionCompleteUI = false;
	
	// ====== Mission Actor ======
	/* Current Mission in prgressing */
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	TArray<AMissionActor*> mMissions;

	/* Missions bound in the Unreal Editor.  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mission", ReplicatedUsing = OnRep_NewMissionWidgetChange)
	TObjectPtr<AMissionActor> mCurrentMission;


	// ======= Level Sequence =======
	TSubclassOf<class UCinematicPlayWidget>	mCinematicWidgetClass;
	TObjectPtr<UCinematicPlayWidget>	 mCinematicWidget;
	

};