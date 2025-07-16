// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Humanoid.h"
#include "HumanoidSwordMan.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API AHumanoidSwordMan : public AHumanoid
{
	GENERATED_BODY()
	
public:
	AHumanoidSwordMan();

public:
	void AttackHitBoxOn() override;
	void AttackHitBoxOff() override;

private:
	UFUNCTION()
	void OnBegineOverlap_Sword(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSwordHitFX(UPrimitiveComponent* OtherComp);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> mSwordHitBox = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystem> mHitParticle = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mSwordHitSound = nullptr;
	
};
