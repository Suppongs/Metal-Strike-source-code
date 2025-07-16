// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpactDecal.generated.h"

UCLASS()
class CRAZY6_API AImpactDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AImpactDecal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void IncreaseDecalScale();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent>			mDefaultScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDecalComponent>	mDecal1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDecalComponent>	mDecal2;
};
