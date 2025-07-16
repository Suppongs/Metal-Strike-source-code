// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "EnemyMovement.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API UEnemyMovement : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	FORCEINLINE void SetVelocity(const FVector& _Velocity) { Velocity = _Velocity; }
	FORCEINLINE FVector GetVelocity() const { return Velocity; }

private:
	void ApplyGravity(float DeltaTime);
	void MoveOnGround(float DeltaTime);
	bool CanWalkOnSlope(const FVector& Normal) const;

private:
	FNavPathSharedPtr CurrentPath;
	int32 CurrentPathIndex = 0;
	float AcceptanceRadius = 30.f;

	UPROPERTY(EditAnywhere)
	float MaxAllowedVerticalDistance = 50.0f;
	
	UPROPERTY(EditAnywhere)
	float mGravityScale = 30.0f;

	UPROPERTY(EditAnywhere)
	float mMaxSlopeAngleToWalk = 45.0f;

	UPROPERTY(EditAnywhere)
	float mStepHeight = 1.0f;
	
	bool IsOnGround = true;
};
