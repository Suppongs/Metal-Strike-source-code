#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMissionType : uint8
{
	Main,
	Sub,

};

UENUM(BlueprintType)
enum class EMissionObjectiveType : uint8
{
	None,
	TargetPoint,
	KillEnemy,
	KillBoss,
};
