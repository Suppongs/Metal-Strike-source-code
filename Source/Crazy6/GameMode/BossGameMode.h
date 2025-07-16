// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BossGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API ABossGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABossGameMode();

private:

public:
	// 보스가 나오면 이 함수를 호출하며 여기에서 타이머가 실행된다.
	void OnBossTimer()
	{
	}

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};
