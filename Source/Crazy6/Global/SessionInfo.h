// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SessionInfo.generated.h"

USTRUCT()
struct FSessionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SessionName;
	UINT8 CurrentPlayers ;
	UINT8 MaxPlayers;
	UINT8 IndexNumber;
};

UCLASS()
class USessionObject : public UObject
{
	GENERATED_BODY()

public:
	TSharedPtr<FSessionInfo> MySessionData;

	void Initialize(TSharedPtr<FSessionInfo> SessionInfo);

};
