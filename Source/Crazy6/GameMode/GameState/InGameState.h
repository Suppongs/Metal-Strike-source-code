// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InGameState.generated.h"

class UBulletPool;

// This Class Access Client and Server
UCLASS()
class CRAZY6_API AInGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AInGameState();
	
	UPROPERTY()
	TObjectPtr<class AMissionActor> MissionActor; // MissionActor

public:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FORCEINLINE UBulletPool* GetUsingPool() {return mUsingPool;}

	UFUNCTION(NetMulticast, Reliable)
	void SetEveryoneEnableInputMulticast();

	UFUNCTION(Server,Unreliable)
	void SetConnectedPlayerNum();
	
	// ===  Player Team HP ==========
	UPROPERTY(Replicated)
	TArray<float> PlayerHPs;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerNames)
	TArray<FString> PlayerNames;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerHPs)
	bool mHP = false;
	UPROPERTY(Replicated)
	TArray<bool> PlayerDeaths;
	UPROPERTY(Replicated)
	TObjectPtr<class AMissionActor> CurrentMission;
	
	UPROPERTY(Replicated)
	uint8 ConnectedPlayerNum = 0;

	void ChangeHP();

	// Player HP Update
	void UpdatePlayerHP(int32 PlayerIndex, float NewHP);
	void UpdatePlayerName(int32 PlayerIndex, FString Name);
public:
	void SetTeamPlayerWidget();
	void SetReadyPlayer();
	UINT8 GetReadyPlayer() { return ReadyPlayer; }


protected:
	UFUNCTION()
	void OnRep_PlayerHPs();
	UFUNCTION()
	void OnRep_PlayerNames();

	UPROPERTY(Replicated)
	int32 ReadyPlayer = 0;
	
	// GameState의 Replication 설정
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;




	
protected:
	void OnConstruction(const FTransform& Transfrom) override;


protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBulletPool> mUsingPool = nullptr;

};


