// Fill out your copyright notice in the Description page of Project Settings.


#include "KillMIssionObjective.h"
#include "Crazy6/Player/PlayerBase.h"
#include "Crazy6/Actor/Enemy/EnemyBase.h"
#include "Crazy6/Actor/Enemy/AISpawner.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"
#include "../MissionActor.h"
#include "../MissionManager.h"
#include "Crazy6/DebugMacros.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"


AKillMIssionObjective::AKillMIssionObjective()
	: bRandomSpawn(true)
	, bSpawnCoolTime(true)
	, bUseRandomSpawn(true)
{
	PrimaryActorTick.bCanEverTick = true;
}

/*
The current structure spawns the first entity immediately, ignoring the specified spawn cool time.
Therefore, adjustments are made to ensure that the initial spawn respects the spawn cool time in certain maps.
*/
void AKillMIssionObjective::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));
		if (CurrentLevelName != "Bridge_P")
		{
			// Difficulty Adjustment Based on Player Count
			UCrazy6GameInstanceSubsystem* CrazySubsystem = GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>();
			if (IsValid(CrazySubsystem))
			{
				int32 NumPlayerController = CrazySubsystem->GetDifficultyOption();
				SpawnCountAdjust(NumPlayerController);
			}
		}
		// Boss Spawn Cool time 
		if (CurrentLevelName == "Bridge_P")
		{
			mSpawnUpdateTime = mSpawnCoolTime;
		}
	}
} 

void AKillMIssionObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		// Cur Mission Complete
		if (false == bActive)
		{
			return;
		}

		// If the initial number of spawned enemies reaches mMaxSpawnableEnemy, stop additional spawning
		if (mTotalSpawnedEnemyCount >= mMaxSpawnableEnemy)
		{
			return;
		}

		// If the current number of spawned enemies exceeds the maximum limit, stop spawning
		if (mCurSpawnedEnemyCount >= mMaxSpawnableEnemy)
		{
			return;
		}

		mSpawnUpdateTime -= DeltaTime;

		if (mSpawnUpdateTime <= 0.0f)
		{
			mSpawnUpdateTime = mSpawnCoolTime;


			SpawnAI();

		}
	}


}

void AKillMIssionObjective::StartObjective()
{
	if (HasAuthority())
	{
		Super::StartObjective();

		mSpawnNumber = 0;

		if (false == bSpawnCoolTime)
		{

			for (int32 i = 0; i < mObjectiveUseData.TargetNumber; i++)
			{
				SpawnAI();
			}

		}
	}
}

void AKillMIssionObjective::EndObjective()
{
	Super::EndObjective();
}

void AKillMIssionObjective::CallBack_KillMissionReflect()
{
	// Server Call
	if (HasAuthority())
	{
		++mObjectiveUseData.ObjectiveCount; // mObjectiveUseData Call On_mObjectiveUseData; (Update Kill Count UI)
		--mCurSpawnedEnemyCount;
		GEngine->AddOnScreenDebugMessage(2, 5, FColor::Cyan, TEXT("KillMission Reflect"));

		// 미션 다른 애로 넘어가던 문제떄문에 했던거

		// 현재 내가 잡은 갯수가 목표수가 같거나 더 크면 갱신    13        12 
		if (IsValid(mOwner))
		{
			
			mOwner->UpdateWidgetContent();
			GEngine->AddOnScreenDebugMessage(2, 5, FColor::Cyan, TEXT("Server Kill Update"));
		
		}

		if (true == CompleteCheck() && bActive)
		{
			GEngine->AddOnScreenDebugMessage(2, 5, FColor::Cyan, TEXT("KillMission Complete!"));
			Complete();
			return;
		}
	}
}

bool AKillMIssionObjective::CompleteCheck() const
{
	// Server Call
	return mObjectiveUseData.TargetNumber <= mObjectiveUseData.ObjectiveCount;
}

void AKillMIssionObjective::SpawnAI()
{
	// Server Call

	if (true == mSpawners.IsEmpty())
	{
		//LOG_ERROR(TEXT("spawner is empty"));

		Complete();
		return;
	}

	bool IsRandomSpawn = bUseRandomSpawn;
	AEnemyBase* SpawnedPawn = mSpawners[GetSpawnNumber()]->Spawn(IsRandomSpawn, mRandomRadius);

	if (nullptr == SpawnedPawn)
	{
		return;
	}


	if (true == IsValid(SpawnedPawn))
	{ 
		SpawnedPawn->AddDeathAIDelegate(this, &AKillMIssionObjective::CallBack_KillMissionReflect);
	}

	++mTotalSpawnedEnemyCount;
	++mCurSpawnedEnemyCount;
}

void AKillMIssionObjective::SpawnCountAdjust(const int32 playerCount)
{
	mObjectiveUseData.TargetNumber *= playerCount;
	mMaxSpawnableEnemy *= playerCount;
}

uint32 AKillMIssionObjective::GetSpawnNumber()
{
	// Server Call
	uint32 Result = 0;

	if (true == bRandomSpawn)
	{
		Result = FMath::RandRange(0, mSpawners.Num() - 1);
	}
	else
	{
		++mSpawnNumber;
		mSpawnNumber = (mSpawners.Num() <= mSpawnNumber) ? 0 : mSpawnNumber;
		Result = mSpawnNumber;
	}


	return Result;

}



