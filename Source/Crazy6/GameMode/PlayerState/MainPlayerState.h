// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	virtual void SetPlayerName(const FString& PlayerName) override;
	FString GetPlayerName() { return mPlayerName ; }
	void SetPlayerHP(float PlayerHP);
	float GetPlayerHP() { return mPlayerHP; }
	void SetPlayerIndex(int32 PlayerIndex);
	int32 GetPlayerIndex() { return mPlayerIndex; }

protected:
	AMainPlayerState();
	
	UPROPERTY(Replicated)
	FString mPlayerName;
	UPROPERTY(Replicated)
	float mPlayerHP = 100.f;
	UPROPERTY(Replicated)
	int32 mPlayerIndex = -1;

	UFUNCTION(Server,Unreliable )
	void ServerSetPlayerName(const FString& PlayerName);
	UFUNCTION(Server, Unreliable)
	void ServerSetPlayerHP(float PlayerHP);

public:
	// 클라이언트에서 호출되는 RepNotify 함수
	/*UFUNCTION()
	void OnRep_SetPlayerName();
	UFUNCTION()
	void OnRep_PlayerHP();*/

	// Replication 설정
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
