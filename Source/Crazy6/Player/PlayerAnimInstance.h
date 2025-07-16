#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class CRAZY6_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
	virtual void NativePostEvaluateAnimation() override;
	virtual void NativeUninitializeAnimation() override;
	virtual void NativeBeginPlay() override;

//protected:
//	UFUNCTION()
//	void UpdateAcceleration();
//
//	UFUNCTION()
//	void UpdateCharacterState();
//
//	UFUNCTION()
//	void UpdateVelocityAndGroundSpeed();
//
//	UFUNCTION()
//	void UpdateCheckMoving();
//
//	UFUNCTION()
//	void UpdateFalling();
//
//	UFUNCTION()
//	void UpdateDirection();
//
//	UFUNCTION()
//	void UpdateMoveState();
//
//	UFUNCTION()
//	void UpdateOrientation();
//
//	UFUNCTION()
//	void UpdateTurmInPlace();
//
//	UFUNCTION()
//	void UpdateWallDetection();
//
//	UFUNCTION()
//	void RunningIntoWall();

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
	bool	mDash = false;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//EPlayerAnimType	mAnimType = EPlayerAnimType::Locomotion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, UAnimSequence*>	mSequenceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, UBlendSpace*>		mBlendSpaceMap;

public:
//	void SetAnimType(EPlayerAnimType Type)
//	{
//		mAnimType = Type;
//	}
//
	void SetMoveDir(const FVector& ActionValue);

	void SetIdleAOData(float Pitch, float Yaw)
	{
		mIdleAOPitch = Pitch;
		mIdleAOYaw = Yaw;
	}

	void OnJump()
	{
		mJump = true;
	}

	void OnCrouch()
	{
		mCrouch = !mCrouch;
	}
};
