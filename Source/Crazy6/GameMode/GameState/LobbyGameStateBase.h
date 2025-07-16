// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API ALobbyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	void SetPlayerArray();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	TArray<FString> GetPlayerIdArray(){ return PlayerIdArray; }

	UFUNCTION()
	void OnRep_PlayerIdArray();
	
	void ChangeConnect();
	
	
protected:
	UPROPERTY(Replicated)
	TArray<FString> PlayerIdArray;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerIdArray)
	bool Connect = false;
};
