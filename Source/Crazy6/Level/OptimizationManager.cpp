
#include "OptimizationManager.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "../DebugMacros.h"

AOptimizationManager::AOptimizationManager()
{

	PrimaryActorTick.bCanEverTick = true;

}

void AOptimizationManager::BeginPlay()
{
	SetOptimizationForAllActors(50000.0f);
}

void AOptimizationManager::SetOptimizationForAllActors(float NewCullDistance)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* Actor = *ActorItr;
		if (Actor)
		{
			//Search Static Mesh
			TArray<UStaticMeshComponent*> StaticMeshComponents;
			Actor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

			for (UStaticMeshComponent* MeshComponent : StaticMeshComponents)
			{
				if (MeshComponent)
				{
					//Set Cull Distance
					MeshComponent->LDMaxDrawDistance = NewCullDistance;
				}
			}
			//LOG
		}
	}
}

