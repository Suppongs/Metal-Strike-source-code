// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletPool.h"
#include "EnemyBulletWithinPool.h"


// Sets default values for this component's properties
UBulletPool::UBulletPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	mPooledObjectClass = AEnemyBulletWithinPool::StaticClass();
}

// Called when the game starts
void UBulletPool::BeginPlay()
{
	Super::BeginPlay();

	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			for (int i = 0; i < mPoolSize; ++i)
			{
				AEnemyBulletWithinPool* PoolableActor = GetWorld()->SpawnActor<AEnemyBulletWithinPool>(mPooledObjectClass,
					FVector(0.0, 0.0, 100000.0), FRotator().ZeroRotator);
				PoolableActor->SetActive(false);
				PoolableActor->SetObjectPool(this);
				mPool.Push(PoolableActor);
			}
		}
		break;
	default:
		break;
	}
}

AEnemyBulletWithinPool* UBulletPool::GetPooledObject()
{
	//When the Editor crashed here, check current level's GameMode and change it to "MainGameMode"
	if(mPool.Num() == 0)
	{
		Expand();
	}
	
	return mPool.Pop();
}

void UBulletPool::ObjectReturnToPool(AEnemyBulletWithinPool* ReturnObject)
{
	mPool.Push(ReturnObject);
}

void UBulletPool::Expand()
{
	for (int i = 0; i < mExpandSize; ++i)
	{
		AEnemyBulletWithinPool* PoolableActor = GetWorld()->SpawnActor<AEnemyBulletWithinPool>(mPooledObjectClass,
			FVector(0.0, 0.0, 100000.0), FRotator().ZeroRotator);
		PoolableActor->SetActive(false);
		PoolableActor->SetObjectPool(this);
		mPool.Push(PoolableActor);
	}
	mPoolSize += mExpandSize;
}

