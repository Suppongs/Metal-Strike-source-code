// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBossMap.h"

ATestBossMap::ATestBossMap()
{
	OnActorBeginOverlap.AddDynamic(this, &ATestBossMap::OnOverlapBegin);
}

void ATestBossMap::OnOverlapBegin(AActor* OverlappedActor,AActor* OtherActor)
{
	if (ENetMode::NM_ListenServer == OtherActor->GetNetMode())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(LevelToLoad);
		}
	}
}
