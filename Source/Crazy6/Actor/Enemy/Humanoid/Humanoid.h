// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnemyBase.h"
#include "Humanoid.generated.h"

class UEnemyAnimTemplate;
class UBoxComponent;

UCLASS()
class CRAZY6_API AHumanoid : public AEnemyBase
{
	GENERATED_BODY()

public:
	AHumanoid();

protected:
	void BeginPlay() override;

public:
	void SetAIState(EAIState State) override;
	void Attack() override;
	void CallBack_Hit(float Damage) override;
	void CallBack_HpZERO() override;
	
	float TakeDamage(float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

private:
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAttackMontage();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> mWeaponMesh = nullptr;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> mHeadShotHitBox = nullptr;



};
