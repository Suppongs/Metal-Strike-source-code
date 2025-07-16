// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPlayerAnimInstance.h"
//#include "GameFramework/CharacterMovementComponent.h"
//#include "Kismet/KismetMathLibrary.h"
//#include "KismetAnimationLibrary.h"
//#include "Networking.h"
//#include "Net/UnrealNetwork.h"
#include "PlayerBase.h"


void ULyraPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AActor* OwningActor = GetOwningActor();

	if (OwningActor)
	{
		mPlayer = Cast<APlayerBase>(OwningActor);
	}
}

void ULyraPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(mPlayer))
	{
		return;
	}

	//if (mPlayer->HasAuthority())
	{
		mPlayer->UpdatePlayerStates();
	}
}

void ULyraPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void ULyraPlayerAnimInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

void ULyraPlayerAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void ULyraPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

float ULyraPlayerAnimInstance::GetCurveValueForCharacter(FName CurveName)
{
	if (GetCurrentActiveMontage())
	{
		return GetCurveValue(CurveName);
	}

	///* Not Moving */
	//if (mPlayer && mPlayer->GetVelocity().SizeSquared() == 0.0f)
	//{
	//	return 0.0f;
	//}

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
		FString::Printf(TEXT("GetCurveValueForCharacter : mPlayer->mServerMovingRotation.Yaw : %f"), 
			mPlayer->mServerMovingRotation.Yaw));
	return mPlayer->mServerMovingRotation.Yaw;
	//return mPlayer->GetActorRotation().Yaw;
}
