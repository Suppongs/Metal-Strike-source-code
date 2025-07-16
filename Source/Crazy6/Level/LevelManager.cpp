// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Kismet/GameplayStatics.h"

void LevelManager::LevelChange(const FName& NextLevelName)
{
    
    /*UGameplayStatics::OpenLevel(GetWorld(), TEXT("Loading"));

FLatentActionInfo LatentInfo;
LatentInfo.CallbackTarget = this;
LatentInfo.ExecutionFunction = "OnLevelLoadCompleted";
LatentInfo.Linkage = 0;
LatentInfo.UUID = UUID++;
UGameplayStatics::LoadStreamLevel(this, NextLevelName, true, true, LatentInfo);*/

}
