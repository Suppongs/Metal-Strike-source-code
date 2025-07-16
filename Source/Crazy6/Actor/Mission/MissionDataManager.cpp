// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionDataManager.h"


CMissionDataManager::CMissionDataManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>
		MonsterAnimTable(TEXT("/Script/Engine.DataTable'/Game/Dev/Data/Mission/DT_MissionData.DT_MissionData'"));
	if (true == MonsterAnimTable.Succeeded())
	{
		mMissionTable = MonsterAnimTable.Object;
	}
	else
	{
	//	LOG_ERROR(TEXT("Not load data table asset"));
	}
}

const FMissionInfo* CMissionDataManager::Find(const FName& _Key)
{
	if (nullptr == mMissionTable)
	{
		//LOG_ERROR(TEXT("Not load data table asset"));
		return nullptr;
	}

	return mMissionTable->FindRow<FMissionInfo>(_Key, TEXT("Not Found Table"));
}
