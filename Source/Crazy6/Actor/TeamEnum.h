#pragma once

#include "CoreMinimal.h"


enum class ETeamID : uint8
{
	NONE = 0,
	PLAYER = 1,
	ENEMY = 2,
	NEUTRALITY = 255,
};