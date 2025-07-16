// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "UObject/NoExportTypes.h"
#include "InputData.generated.h"

UCLASS()
class CRAZY6_API UDefaultInputData : public UObject
{
	GENERATED_BODY()
	
public:
	UDefaultInputData();
	
public:
	UInputMappingContext* mDefaultContext;

public:
	UInputAction* mMove;
	UInputAction* mJump;
	UInputAction* mCrouch;
	UInputAction* mDash;
	UInputAction* mLookMouse;
	UInputAction* mMenu;
	UInputAction* mSprint;
	UInputAction* mFire;
	UInputAction* mReload;
	UInputAction* mEquipWeapon;
	UInputAction* mZoom;
	UInputAction* mSwapAB;
	UInputAction* mInteraction;
	UInputAction* mDrop;
};
