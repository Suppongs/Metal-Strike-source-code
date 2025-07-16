#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EInGameState : uint8
{
	Idle,
	Ready,
	OnMission,
	ReWard,
	End,

};
