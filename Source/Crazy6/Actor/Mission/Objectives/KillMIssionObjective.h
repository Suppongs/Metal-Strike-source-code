// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MissionObjectiveActor.h"
#include "KillMIssionObjective.generated.h"

class AAISpawner;

UCLASS()
class CRAZY6_API AKillMIssionObjective : public AMissionObjectiveActor
{
	GENERATED_BODY()

public:
	AKillMIssionObjective();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Mission
public:
	void StartObjective() override;
	void EndObjective() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	TArray<AAISpawner*> mSpawners;

protected:
	/* Call delegate function, if ai was dead. */
	void CallBack_KillMissionReflect();

	bool CompleteCheck() const;
	void SpawnAI();
	void SpawnCountAdjust(const int32 playerCount);

private:
	uint32 GetSpawnNumber();

private:

	// ======================================================

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	uint32 bRandomSpawn : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	uint32 bSpawnCoolTime : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Random")
	uint32 bUseRandomSpawn : 1;


	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 mSpawnNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 mMaxSpawnableEnemy = 20;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 mCurSpawnedEnemyCount = 0;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 mTotalSpawnedEnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float mSpawnCoolTime = 2.0f;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite, Category = "Spawn")
	float mSpawnUpdateTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn|Random")
	float mRandomRadius = 300.0f;

};
