// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Crazy6/Player/InfoData/WeaponInfo.h"
#include "UObject/NoExportTypes.h"
#include "CustomItemData.generated.h"

UENUM()
enum class EItemType : uint8
{
	None = 0x00,
	HpPotion,
	Magazine,
	Rifle,
	ShotGun,
	MachineGun,
	RPG,
	Sniper,
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	EItemType DataItemType = EItemType::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* DataStaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DataRelativeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator DataRelativeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DataRelativeScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString		DataItemName = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DataDescription = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponDetails DataWeaponDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DataIsWeapon = false;
};
