// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/NoExportTypes.h"
#include "DamageWidgetData.generated.h"

/**
 *
 */
UCLASS()
class CRAZY6_API UDamageWidgetData : public UObject
{
	GENERATED_BODY()
public:
	UDamageWidgetData();

public:
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> mWidgetComponent;


};
