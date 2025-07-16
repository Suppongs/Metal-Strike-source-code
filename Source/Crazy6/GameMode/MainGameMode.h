// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

class AMissionManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRestartDelegate);

UCLASS()
class CRAZY6_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

	friend class AInGameState;
public:
	uint8 GetPlayerNum() { return PlayerNum; }
	uint64 PlusMonster() { return ++MonsterNum; }
	uint64 MinusMonster() { return --MonsterNum; }
	void SetIsTraveling(bool CurrentTraveling){ IsTraveling = CurrentTraveling;	};
	bool GetIsTraveling(){ return IsTraveling; };
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnRestartDelegate OnRestart;
	UFUNCTION()
	void RestartEvent();
	
	
protected:
	AMainGameMode(); 
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;


protected:	
	UPROPERTY(VisibleAnywhere, Category = "MissionSystem")
	TObjectPtr<AMissionManager> mMissionSystem;
	
	uint8 PlayerNum = 0;
	uint64 MonsterNum = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsTraveling = false;
};
