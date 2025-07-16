// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BulletPool.generated.h"

class AEnemyBulletWithinPool;

UCLASS()
class CRAZY6_API UBulletPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBulletPool();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	AEnemyBulletWithinPool* GetPooledObject();
	void ObjectReturnToPool(AEnemyBulletWithinPool* ReturnObject);

private:
	void Expand();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyBulletWithinPool> mPooledObjectClass = nullptr;
	
	int32 mPoolSize = 100;
	int32 mExpandSize = 10;

	UPROPERTY(VisibleAnywhere)
	TArray<AEnemyBulletWithinPool*> mPool;
};
