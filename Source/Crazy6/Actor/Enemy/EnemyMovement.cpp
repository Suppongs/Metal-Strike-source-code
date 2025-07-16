// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMovement.h"
#include "EnemyBase.h"
#include "NavigationSystem.h"
#include "NavFilters/NavigationQueryFilter.h"

void UEnemyMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (false == IsValid(UpdatedComponent) || true == ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	//Check it's on the ground
	FHitResult Hit;
	AEnemyBase* EnemyPawn = Cast<AEnemyBase>(GetPawnOwner());
	if (true == IsValid(EnemyPawn))
	{
		float CapsuleHalfHeight = EnemyPawn->GetCapsule()->GetScaledCapsuleHalfHeight() + 3.f;
		FVector Start = UpdatedComponent->GetComponentLocation();
		FVector End = Start - FVector(0.0f, 0.0f, CapsuleHalfHeight);
    
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetPawnOwner());

		IsOnGround = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel7, QueryParams);
	
		if (false == IsOnGround)
		{
			ApplyGravity(DeltaTime);
		}
		else
		{
			MoveOnGround(DeltaTime);
		}
	}
}

void UEnemyMovement::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	AEnemyBase* AIPawn = Cast<AEnemyBase>(GetPawnOwner());
	if (false == IsValid(AIPawn))
	{
		return;
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (true == IsValid(NavSys))
	{
 		FVector Start = UpdatedComponent->GetComponentLocation();
		FVector End = Start + MoveVelocity.GetSafeNormal() * 1000.0f;
        
		//Get navigation data
		const ANavigationData* NavData = NavSys->GetNavDataForProps(AIPawn->GetNavAgentPropertiesRef(), Start);
		if (true == IsValid(NavData))
		{
			//Create query filter
			const FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, this, nullptr);
			
			FPathFindingQuery Query(AIPawn, *NavData, Start, End, NavFilter);
			Query.SetAllowPartialPaths(false);
            
			FPathFindingResult PathResult = NavSys->FindPathSync(Query);
			if (true == PathResult.IsSuccessful() && true == PathResult.Path.IsValid())
			{
				CurrentPath = PathResult.Path;
				CurrentPathIndex = 0;
			}
		}
	}

	//Move by path
	if (true == CurrentPath.IsValid() && CurrentPath->GetPathPoints().Num() > 0)
	{
		const TArray<FNavPathPoint>& Points = CurrentPath->GetPathPoints();
        
		//Find next point
		while (CurrentPathIndex < Points.Num() - 1)
		{
			FVector NextPoint = Points[CurrentPathIndex + 1].Location;
			FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
            
			//If current location is close enough, then to the next point
			if (FVector::DistSquared2D(CurrentLocation, NextPoint) < FMath::Square(AcceptanceRadius))
			{
				CurrentPathIndex++;
			}
			else
			{
				break;
			}
		}

		//Move to the next point
		if (CurrentPathIndex < Points.Num() - 1)
		{
			FVector NextPoint = Points[CurrentPathIndex + 1].Location;
			FVector Direction = (NextPoint - UpdatedComponent->GetComponentLocation()).GetSafeNormal();
			float Speed = AIPawn->GetMoveSpeed();
			Velocity = Direction * Speed;
		}
	}
}

void UEnemyMovement::ApplyGravity(float DeltaTime)
{
	//Set Gravity
	float GravityAmount = -980.0f * mGravityScale;
	Velocity.Z += GravityAmount * DeltaTime;

	//Move
	FHitResult Hit;
	FVector NextMove = Velocity * DeltaTime;
	SafeMoveUpdatedComponent(NextMove, UpdatedComponent->GetComponentRotation(), true, Hit);

	//When Blocked
	if (true == Hit.IsValidBlockingHit())
	{
		if (true == CanWalkOnSlope(Hit.Normal))
		{
			SlideAlongSurface(NextMove, 1.0f - Hit.Time, Hit.Normal, Hit);
		}
		
		Velocity = FVector::ZeroVector;
	}
}

void UEnemyMovement::MoveOnGround(float DeltaTime)
{
	FVector NextMove = Velocity * DeltaTime;
    
	if (false == NextMove.IsNearlyZero())
	{
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (true == IsValid(NavSys))
		{
			FVector TargetLocation = UpdatedComponent->GetComponentLocation() + NextMove;
			FNavLocation ProjectedTarget;
            
			//Check target location is on navmesh
			if (false == NavSys->ProjectPointToNavigation(TargetLocation, ProjectedTarget, INVALID_NAVEXTENT, NavSys->GetDefaultNavDataInstance()))
			{
				//Cancel movement that is out of NavMesh
				return;
			}
		}
		//Move
		FHitResult Hit;
		NextMove.Z = 0.0;
		SafeMoveUpdatedComponent(NextMove, UpdatedComponent->GetComponentRotation(), true, Hit);

		//When Blocked
		if (true == Hit.IsValidBlockingHit())
		{
			if (true == CanWalkOnSlope(Hit.Normal))
			{
				FVector Delta = FVector::VectorPlaneProject(NextMove, Hit.Normal);
				//Step Up & Move
				FHitResult StepUpHit;
				SafeMoveUpdatedComponent(FVector(0, 0, mStepHeight), UpdatedComponent->GetComponentRotation(), true, StepUpHit);
				if (false == StepUpHit.IsValidBlockingHit())
				{
					SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentRotation(), true, StepUpHit);
					if (true == StepUpHit.IsValidBlockingHit())
					{
						SlideAlongSurface(Delta, 1.0f - StepUpHit.Time, StepUpHit.Normal, StepUpHit);
					}
				}
				else
				{
					SlideAlongSurface(NextMove, 1.0f - Hit.Time, Hit.Normal, Hit);
				}
			}
			else
			{
				SlideAlongSurface(NextMove, 1.0f - Hit.Time, Hit.Normal, Hit);
			}
		}
	}
}

bool UEnemyMovement::CanWalkOnSlope(const FVector& Normal) const
{
	float SlopeAngle = FMath::RadiansToDegrees(FMath::Acos(Normal.Dot(FVector::UpVector)));
	return SlopeAngle <= mMaxSlopeAngleToWalk;
}
