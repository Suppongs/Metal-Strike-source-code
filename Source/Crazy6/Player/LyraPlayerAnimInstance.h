// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InfoData/PlayerEnum.h"
#include "Animation/AnimInstance.h"
#include "LyraPlayerAnimInstance.generated.h"

class APlayerBase;
class UCharacterMovementComponent;

UCLASS()
class CRAZY6_API ULyraPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
	virtual void NativePostEvaluateAnimation() override;
	virtual void NativeUninitializeAnimation() override;
	virtual void NativeBeginPlay() override;

public:
	float GetCurveValueForCharacter(FName CurveName);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<APlayerBase> mPlayer = nullptr;

	/* ========================= Animation =========================*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Idle")
	UAnimSequence* mIdle;

	// ==================== [ Animation|Turn ] ====================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Turn")
	UAnimSequence* mTurnLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Turn")
	UAnimSequence* mTurnRight;

	// ==================== [ Animation|Walk ] ====================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Walk")
	UAnimSequence* mWalkFwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Walk")
	UAnimSequence* mWalkRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Walk")
	UAnimSequence* mWalkLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Walk")
	UAnimSequence* mWalkBwd;

	// ==================== [ Animation|Jog ] ====================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Jog")
	UAnimSequence* mJogFwd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Jog")
	UAnimSequence* mJogRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Jog")
	UAnimSequence* mJogLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Jog")
	UAnimSequence* mJogBwd;
	
	// ==================== [ Animation|Crouch ] ====================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Crouch")
	UAnimSequence* mCrouchEntry;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Crouch")
	UAnimSequence* mCrouchIdle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Crouch")
	UAnimSequence* mCrouchExit;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Crouch|Walk")
	UAnimSequence* mCrouchWalkFwd;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Crouch|Walk")
	UAnimSequence* mCrouchWalkBwd;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Crouch|Walk")
	UAnimSequence* mCrouchWalkRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Crouch|Walk")
	UAnimSequence* mCrouchWalkLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Crouch|Turn")
	UAnimSequence* mCrouchTurnRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Crouch|Turn")
	UAnimSequence* mCrouchTurnLeft;

	// ==================== [ Animation|Jump ] ====================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Jump")
	UAnimSequence* mJumpApex;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Jump")
	UAnimSequence* mJumpLoop;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Jump")
	UAnimSequence* mJumpRecoveryAdditive;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Fall")
	UAnimSequence* mJumpFallLand;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Fall")
	UAnimSequence* mJumpFallLightLand;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Fall")
	UAnimSequence* mJumpFallHeavyLand;

	// ==================== [ Animation|AimOffset ] ====================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|AimOffset")
	UBlendSpace* mAimOffset;
};
