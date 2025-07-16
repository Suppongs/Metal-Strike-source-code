// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Humanoid.h"
#include "HumanoidGunMan.generated.h"

class AEnemyController;
class UEnemyBulletSpawner;

UCLASS()
class CRAZY6_API AHumanoidGunMan : public AHumanoid
{
	GENERATED_BODY()

public:
	AHumanoidGunMan();

public:
	void Attack() override;

protected:
	void BeginPlay() override;
	
private:
	void Shoot();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEnemyBulletSpawner> mEnemyBulletSpawner = nullptr;
};
