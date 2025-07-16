// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "BossGeometryCollectionActor.generated.h"

/**
 * 
 */
UCLASS()
class CRAZY6_API ABossGeometryCollectionActor : public AGeometryCollectionActor
{
	GENERATED_BODY()

public:
	ABossGeometryCollectionActor();
	virtual void BeginPlay() override;
	
};
