// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KillMIssionObjective.h"
#include "EliteKillMIssionObjective.generated.h"

/**
 *
 */
UCLASS()
class CRAZY6_API AEliteKillMIssionObjective : public AKillMIssionObjective
{
	GENERATED_BODY()

public:
	AEliteKillMIssionObjective();

public:
	virtual void StartObjective() override;
	// ==================== [ Networking ] ====================
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FTimerHandle VisibleTime;

	//UI
	UFUNCTION()
	void VisibleWidget();
	void ChangeCruchWidget();
	UFUNCTION()
	void OnRep_IsCruchWidget();
	
protected:
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class UUserWidget>	mCrunchWidgetClass;
	UPROPERTY(VisibleAnywhere)
	class UEpicAppearanceWidget* mCrunchWidget;

	UPROPERTY(ReplicatedUsing= OnRep_IsCruchWidget)
	bool IsCruchWidget = false;
	


};
