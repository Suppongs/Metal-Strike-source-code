#pragma once

#include "PlayerEnum.generated.h"

UENUM()
enum class EDashKey : uint8
{
	None = 0x00,
	W = 10,
	A = 20,
	S = 30,
	D = 40
};

UENUM()
enum class EMovementDirectionState : uint8
{
	None = 0x00,
	Forward,
	Right,
	Backward,
	Left
};

UENUM()
enum class ELandState : uint8
{
	None = 0x00,
	Normal,
	Soft,
	Heavy
};

UENUM()
enum class EPlayerWeaponType : uint8
{
	None = 0x00,
	Unarmed = 1,
	Rifle = 2,
	Sniper = 3,
	Shotgun = 4,
	RPG = 5,
	MG = 6
};

UENUM()
enum class EWeaponState : uint8
{
	None = 0x00,
	Equip,
	Unequip
};

UENUM()
enum class EHitActorType : uint8
{
	None		= 0x00,
	Actor		= 1,
	Building	= 2,
	Player		= 3,
	AI			= 4,
	AIBoss		= 5,
	LandScape	= 6,

	/* Number of HitActor Types */
	TypeMax			= 7
};