// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../MissionInfo.h"
#include "MissionObjectiveActor.generated.h"

DECLARE_DELEGATE(FOnMissionReflectDelegate);

class AMissionActor;

UCLASS()
class CRAZY6_API AMissionObjectiveActor : public AActor
{
	GENERATED_BODY()

public:
	AMissionObjectiveActor();

public:
	FORCEINLINE void SetMissionOwner(AMissionActor* MissionOwner) { mOwner = MissionOwner; };

	virtual void StartObjective();
	virtual void EndObjective();
	void Complete();

	//template<typename ActorType>
	//void AddOnMissionRelfectDelegate(ActorType* Actor, void(ActorType::* Func)())
	//{
	//	mOnMissionReflectDelegate.BindUObject(Actor, Func);
	//}

protected:
	virtual void BeginPlay() override;


	// ==================== [ Networking ] ====================
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:

	UFUNCTION()
	void OnRep_mObjectiveUseData();
	// =====================================================

public:
	//FOnMissionReflectDelegate mOnMissionReflectDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MissionObjective", ReplicatedUsing = OnRep_mObjectiveUseData)
	FObjectiveUseInfo mObjectiveUseData;

protected:
	UPROPERTY(Replicated)
	TObjectPtr<AMissionActor> mOwner = nullptr;

	UPROPERTY()
	bool bActive = false;
};
