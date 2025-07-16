#pragma once

#include "CoreMinimal.h"
#include "EnemyEnum.h"
#include "EnemyInfo.Generated.h"

USTRUCT(BlueprintType)
struct FEnemyAnimData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, UAnimSequence*> mSequenceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, UBlendSpace*> mBlendSpaceMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, UAnimMontage*> mMontageMap;
};

USTRUCT(BlueprintType)
struct FAIStatInfoByState : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mConfiguredMoveSpeed = 0.0f;
};

USTRUCT(BlueprintType)
struct FEnemyInfoTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString mName = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mAttack = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mHP = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mAttackRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mMoveSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EAIState, FAIStatInfoByState> mStatInfoByState;

};

USTRUCT()
struct FPlayerDPS
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	AActor* mPlayerActor = nullptr;

	UPROPERTY(VisibleAnywhere)
	float mAccumulatedDamage = 0.0f;
	
	UPROPERTY(VisibleAnywhere)
	float mDPS = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float mStartDamagedTime = -1.0f;

	UPROPERTY(VisibleAnywhere)
	float mLastDamagedTime = 0.0f;
};