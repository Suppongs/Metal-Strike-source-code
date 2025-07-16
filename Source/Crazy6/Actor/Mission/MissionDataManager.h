// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MissionInfo.h"
#include "Crazy6/Utils/Singleton.h"


// ===== Maybe later Remove this (I checking this class where use) =====
class UDataTable;

class CRAZY6_API CMissionDataManager : public CSingleton<CMissionDataManager>
{
public:
	CMissionDataManager();

public:
	const FMissionInfo* Find(const FName& _Key);

private:
	TObjectPtr<UDataTable> mMissionTable = nullptr;

};
