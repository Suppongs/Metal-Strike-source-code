#include "PlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerBase.h"

void UPlayerAnimInstance::SetMoveDir(const FVector& ActionValue)
{
	mMoveDir = 0.f;

	if (ActionValue.Y > 0.f)
	{
		if (ActionValue.X > 0.f)
			mMoveDir = 45.f;

		else if (ActionValue.X < 0.f)
			mMoveDir = -45.f;
	}

	else if (ActionValue.Y < 0.f)
	{
		if (ActionValue.X > 0.f)
			mMoveDir = 135.f;

		else if (ActionValue.X < 0.f)
			mMoveDir = -135.f;

		else
			mMoveDir = 180.f;
	}

	else
	{
		if (ActionValue.X > 0.f)
			mMoveDir = 90.f;

		else if (ActionValue.X < 0.f)
			mMoveDir = -90.f;
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerBase* PlayerCharacter =
		Cast<APlayerBase>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
	{
		UCharacterMovementComponent* Movement =
			PlayerCharacter->GetCharacterMovement();

		if (IsValid(Movement))
		{
			mMoveSpeed = Movement->Velocity.Length();

			mAcceleration = Movement->GetCurrentAcceleration().Length() > 0.f;

			mIsAir = Movement->IsFalling();

			if (!mIsAir)
			{
				mJump = false;
			}
			// --- Crouch Montage Code ---
		}
	}
}

void UPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UPlayerAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void UPlayerAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}
