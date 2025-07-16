#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerLocomotionTemplate.generated.h"

UCLASS()
class CRAZY6_API UPlayerLocomotionTemplate : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float	mMoveSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float	mMoveDir = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool	mAcceleration = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool	mIsAir = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float	mIdleAOPitch = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float	mIdleAOYaw = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool	mJump = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool	mCrouch = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequence* mIdle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequence* mWalkStart;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequence* mWalk;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequence* mWalkStop;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UAnimSequence* mRunStart;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UAnimSequence* mRun;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UAnimSequence* mRunStop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequence* mJumpStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequence* mJumpApex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequence* mFallLand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequence* mFallRecovery;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequence* mCrouchEntry;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequence* mCrouchIdle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAnimSequence* mCrouchExit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBlendSpace* mIdleAO;

};
