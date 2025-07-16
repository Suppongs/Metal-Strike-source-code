// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MissionObjectiveActor.h"
#include "TriggerMissionObjective.generated.h"


class URotatingMovementComponent;
class UBoxComponent;

UCLASS()
class CRAZY6_API ATriggerMissionObjective : public AMissionObjectiveActor
{
	GENERATED_BODY()
	
public:
	ATriggerMissionObjective();
	virtual void Tick(float DeltaTime) override;

public:
	void OnCollision();
	void OffCollision();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	// ==================== [ Networking ] ====================
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Mission
public:
	void StartObjective() override;
	void EndObjective() override;



public:
	UFUNCTION(NetMulticast, Reliable)
	void CallWidgetFunc();

	void CallWidgetFunc_Implementation();



private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	TObjectPtr<UBoxComponent> mTrigger;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mStaticConeMesh = nullptr;

	TSubclassOf<class UCinematicPlayWidget>	mCinematicWidgetClass;
	TObjectPtr<UCinematicPlayWidget>	 mCinematicWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URotatingMovementComponent> mRotatingMovement;
};
