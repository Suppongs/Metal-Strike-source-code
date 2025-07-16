// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/GameInstance.h"
#include "Crazy6GameInstance.generated.h"

/**
 *
 */
UCLASS()
class CRAZY6_API UCrazy6GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UCrazy6GameInstance();
	UFUNCTION(Server,Unreliable)
	void Logout();

protected:
	virtual void Init() override;
	virtual void Shutdown() override;
	class UCrazy6GameInstanceSubsystem* mGameInstanceSubsystem;

};

