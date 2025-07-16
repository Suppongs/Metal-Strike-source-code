// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAISpawner.h"
#include "NavigationSystem.h"
#include "Crazy6/Actor/Enemy/Epic/EpicCrunch.h"
#include "Crazy6/Actor/Enemy/Humanoid/HumanoidGunMan.h"
#include "Crazy6/Actor/Enemy/Humanoid/HumanoidSwordMan.h"

UBossAISpawner::UBossAISpawner()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UBossAISpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBossAISpawner::SpawnRandomAI()
{
	FVector BaseLocation = GetComponentLocation();

	for (int i = 0; i < 2; ++i)
	{
		int8 RandomIndex = FMath::RandRange(0, 1);
		switch (RandomIndex)
		{
		case 0:
			{
				FNavLocation NavLocation;
				UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
				if (true == IsValid(NavSystem) &&
					true == NavSystem->ProjectPointToNavigation(BaseLocation, NavLocation))
				{
					AHumanoidSwordMan* SwordMan = AHumanoidSwordMan::StaticClass()->GetDefaultObject<AHumanoidSwordMan>();
					if (true == IsValid(SwordMan))
					{
						NavLocation.Location.Z += SwordMan->GetCapsule()->GetScaledCapsuleHalfHeight();
					}
				}
				
				GetWorld()->SpawnActor<AHumanoidSwordMan>(AHumanoidSwordMan::StaticClass(), NavLocation, GetComponentRotation());
			}
			break;
		case 1:
			{
				FNavLocation NavLocation;
				UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
				if (true == IsValid(NavSystem) &&
					true == NavSystem->ProjectPointToNavigation(BaseLocation, NavLocation))
				{
					AEpicCrunch* EpicCrunch = AEpicCrunch::StaticClass()->GetDefaultObject<AEpicCrunch>();
					if (true == IsValid(EpicCrunch))
					{
						NavLocation.Location.Z += EpicCrunch->GetCapsule()->GetScaledCapsuleHalfHeight();
					}
				}
				
				GetWorld()->SpawnActor<AEpicCrunch>(AEpicCrunch::StaticClass(), NavLocation, GetComponentRotation());
			}
			break;
		case 2:
			{
				FNavLocation NavLocation;
				UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
				if (true == IsValid(NavSystem) &&
					true == NavSystem->ProjectPointToNavigation(BaseLocation, NavLocation))
				{
					AHumanoidGunMan* GunMan = AHumanoidGunMan::StaticClass()->GetDefaultObject<AHumanoidGunMan>();
					if (true == IsValid(GunMan))
					{
						NavLocation.Location.Z += GunMan->GetCapsule()->GetScaledCapsuleHalfHeight();
					}
				}
				
				GetWorld()->SpawnActor<AHumanoidGunMan>(AHumanoidGunMan::StaticClass(), NavLocation, GetComponentRotation());
			}
			break;
		}
	}
}

