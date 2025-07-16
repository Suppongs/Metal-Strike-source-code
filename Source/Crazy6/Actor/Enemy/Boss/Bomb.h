// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"


class USoundBase;
class ABossZero;
class UNiagaraComponent;

UCLASS()
class CRAZY6_API ABomb : public AActor
{
	GENERATED_BODY()

public:	
	ABomb();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE void SetBombOwner(ABossZero* _Owner){ mOwner = _Owner;}
	
private:
	UFUNCTION()
	void OnIndicatorEnd(UNiagaraComponent* FinishedComponent);

	UFUNCTION()
	void OnExplosionEnd(UParticleSystemComponent* FinishedComponent);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastExplosionFX();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> mBombRoot = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ABossZero> mOwner = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> mBombVisual = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> mBombIndicator = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mLaunchSound = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USoundBase> mExplosionSound = nullptr;

		
};
