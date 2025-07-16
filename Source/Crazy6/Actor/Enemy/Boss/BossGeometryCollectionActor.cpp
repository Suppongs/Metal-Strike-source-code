// Fill out your copyright notice in the Description page of Project Settings.


#include "BossGeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollection.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

#include "Net/UnrealNetwork.h"


ABossGeometryCollectionActor::ABossGeometryCollectionActor()
{
	bReplicates = true;

}

void ABossGeometryCollectionActor::BeginPlay()
{
	Super::BeginPlay();

	GeometryCollectionComponent->SetCollisionProfileName(TEXT("BlockAll"));
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // ���Ϳ� �浹 ����
	
	UGeometryCollection* bossBreakGC = LoadObject<UGeometryCollection>(nullptr, TEXT("/Game/Dev/Level/GC/GC_/GC_SM_Boss.GC_SM_Boss"));
	GeometryCollectionComponent->SetRestCollection(bossBreakGC);
	GeometryCollectionComponent->RecreatePhysicsState(); // �������� �缳��
	GeometryCollectionComponent->DamageThreshold.Empty();
}

