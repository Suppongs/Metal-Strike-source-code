// Fill out your copyright notice in the Description page of Project Settings.


#include "CacheTriggerBox.h"
#include "Kismet/GameplayStatics.h"
#include "Chaos/CacheCollection.h"
#include "Chaos/ChaosCache.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ACacheTriggerBox::ACacheTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACacheTriggerBox::BeginPlay()
{
	Super::BeginPlay();


	/* 
	You must set the name of the mRefPlayerActorTagName inside the [Trigger Box] and the 'Tag' name of the [Cache Player Actor] 
	to be exactly the same for it to function correctly.
	*/
	TArray<AActor*> FoundActors{};
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), mRefPlayerActorTagName, FoundActors);


	for (AActor* Actor : FoundActors)
	{
		if (IsValid(Actor))
		{
			mCachePlayer = Cast<AChaosCacheManager>(Actor);
			break;
		}
	}
	

	// AChaosCacheManager
	if (nullptr != mCachePlayer && mCachePlayer->CacheMode != ECacheMode::Record)
	{
		FindGeometryComFromCachePlayer();
		FindObservedComFromCachePlayer();

		if (nullptr != mObservedComponent)
		{
			FindInitialTransformFromCachePlayer();
		}

		mGeometryCollectionComponent->SetComponentTickEnabled(false);

		//========================================



		// Gets all components of the actor. While the values may appear different, there are no issues.		
		mStaticBuildingMesh->SetMobility(EComponentMobility::Movable);
		mStaticBuildingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		// Gets the location of the Geometry Collection component.
		FVector RelativeLocation = mGeometryCollectionComponent->GetRelativeLocation();
		FVector WorldLocation = mGeometryCollectionComponent->GetOwner()->ActorToWorld().TransformPosition(RelativeLocation);
		mStaticBuildingMesh->SetWorldLocation(WorldLocation);

		FRotator RelativeRot = mGeometryCollectionComponent->GetRelativeRotation();
		FQuat WorldQuat = mGeometryCollectionComponent->GetOwner()->GetActorTransform().TransformRotation(RelativeRot.Quaternion());

		// Applies world rotation to the static mesh.
		mStaticBuildingMesh->SetWorldRotation(WorldQuat.Rotator());		

		mStaticBuildingMesh->SetMobility(EComponentMobility::Static);

		// Find the StaticMeshComponent added in Blueprint.
		UBoxComponent* BoxMeshComp = FindComponentByClass<UBoxComponent>();


		if (mBuildingTypeNumber == TEXT("1"))
		{
			UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>
				(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Dev/Level/GC/MERGE_SM/SM_MERGED_01.SM_MERGED_01'"));

			if (LoadedMesh)
			{
				mStaticBuildingMesh->SetStaticMesh(LoadedMesh);
			}

			FVector RelativelLocation = FVector(1931.0f, -841.0f, 2882.0f);
			BoxMeshComp->SetRelativeLocation(RelativelLocation);
			BoxMeshComp->SetBoxExtent(FVector(2058.658295f, 1175.859297f, 2744.351797f));

		}

		else if (mBuildingTypeNumber == TEXT("2"))
		{
			UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>
				(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Dev/Level/GC/MERGE_SM/SM_MERGED_StaticMeshActor_2.SM_MERGED_StaticMeshActor_2'"));

			if (LoadedMesh)
			{
				mStaticBuildingMesh->SetStaticMesh(LoadedMesh);
			}

			FVector RelativelLocation = FVector(699.0f, -1041.0f, 1514.0f);
			BoxMeshComp->SetRelativeLocation(RelativelLocation);
			BoxMeshComp->SetBoxExtent(FVector(720.516589f, 1429.932471f, 1409.80473f));
		}

		else if (mBuildingTypeNumber == TEXT("3"))
		{
		/*	UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>
				(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Dev/Level/GC/MERGE_SM/SM_MERGED_StaticMeshActor_3.SM_MERGED_StaticMeshActor_3'"));

			if (LoadedMesh)
			{
				mStaticBuildingMesh->SetStaticMesh(LoadedMesh);
			}*/

			FVector RelativelLocation = FVector(1515.0f, -403.0f, 552.0f);
			BoxMeshComp->SetRelativeLocation(RelativelLocation);
			BoxMeshComp->SetBoxExtent(FVector(1537.946889f, 586.37561f, 515.776746f));


		}

		else if (mBuildingTypeNumber == TEXT("4"))
		{
			UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>
				(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Dev/Level/GC/MERGE_SM/SM_MERGED_StaticMeshActor_4.SM_MERGED_StaticMeshActor_4'"));

			if (LoadedMesh)
			{
				mStaticBuildingMesh->SetStaticMesh(LoadedMesh);
			}

			FVector RelativelLocation = FVector(-1248.0f, -1073.0f, 1579.0f);
			BoxMeshComp->SetRelativeLocation(RelativelLocation);
			BoxMeshComp->SetBoxExtent(FVector(1378.774018f, 1200.36909f, 1582.608989f));
		}


		else if (mBuildingTypeNumber == TEXT("5"))
		{
			UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>
				(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Dev/Level/GC/MERGE_SM/SM_MERGED_StaticMeshActor_5.SM_MERGED_StaticMeshActor_5'"));

			if (LoadedMesh)
			{
				mStaticBuildingMesh->SetStaticMesh(LoadedMesh);
			}

			FVector RelativelLocation = FVector(1285.0f, -493.0f, 2257.0f);
			BoxMeshComp->SetRelativeLocation(RelativelLocation);
			BoxMeshComp->SetBoxExtent(FVector(1378.774048f, 748.273424f, 2003.793704f));
		}

		else if (mBuildingTypeNumber == TEXT("6"))
		{
			UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>
				(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Dev/Level/GC/MERGE_SM/SM_SM_MERGED_StaticMeshActor_6.SM_SM_MERGED_StaticMeshActor_6'"));

			if (LoadedMesh)
			{
				mStaticBuildingMesh->SetStaticMesh(LoadedMesh);
			}

			FVector RelativelLocation = FVector(507.286652f, 0.f, 4494.079238f);
			BoxMeshComp->SetRelativeLocation(RelativelLocation);
			BoxMeshComp->SetBoxExtent(FVector(3022.521434f, 2078.533912f, 4234.851118f));
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Need to Write Building Type Number"));
		}

		mStaticBuildingMesh->SetVisibility(false);


	}	


		
}

// Called every frame
void ACacheTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 
	//float test22 = mChaosCache->ParticleTracks[0].TransformData.GetBeginTime();
	//float test23 = mChaosCache->ParticleTracks[0].TransformData.GetEndTime();

	if (bCachePlay)
	{
		mCachePlayTimeAcc += DeltaTime;

		float Delay = 5.f;

		if (mCachePlayTimeAcc > mChaosCache->RecordedDuration + Delay)
		{
			GEngine->AddOnScreenDebugMessage(2, 5, FColor::Cyan, TEXT("Cache Play"));
			mGeometryCollectionComponent->DestroyComponent();

			mStaticBuildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			mStaticBuildingMesh->SetVisibility(true);
			SetActorTickEnabled(false); // Disables tick for the actor and all components.

		}

	}


}


float ACacheTriggerBox::TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent, 
	AController* _EventInstigator, AActor* _DamageCauser)
{
	Super::TakeDamage(_DamageAmount, _DamageEvent, _EventInstigator, _DamageCauser);

	mCurHP -= _DamageAmount;

	if (mCurHP <= 0)
	{
		bDeath = true;	
	}

	return _DamageAmount;
}

void ACacheTriggerBox::FindGeometryComFromCachePlayer()
{
	/*
	For the function to work correctly, the "Geometry Collection component of the Cache Player placed
	 in the level must be renamed to same "CachePlayerName." 

	get name�� �α�����θ� ���°� ���ٴ� ��Ⱑ �־ �±� ��� ���� ����ȭ
	ACacheTriggerBox�� CachePlayerName�� CachePlayer�� ������Ʈ�� ������Ʈ�� �±� �̸��� ���ƾ� �Ѵ�.
	*/
	const TSet<UActorComponent*>& Components = mCachePlayer->GetComponents();

	for (UActorComponent* Component : Components)
	{
		UGeometryCollectionComponent* GeoComp = Cast<UGeometryCollectionComponent>(Component);
		if (GeoComp && GeoComp->ComponentHasTag(mPlayCacheName))
		{
			mGeometryCollectionComponent = GeoComp;
			//mGeometryCollectionComponent->DestroyComponent(); // test complete 
			break;
		}
	}


}

void ACacheTriggerBox::FindObservedComFromCachePlayer()
{
	TArray<FObservedComponent>& ObjservedComArray = mCachePlayer->GetObservedComponents();

	for (FObservedComponent& ObservedComponent : ObjservedComArray)
	{
		if (ObservedComponent.CacheName == mPlayCacheName)
		{
			mObservedComponent = &ObservedComponent;
			break;
		}

	}


}

void ACacheTriggerBox::FindInitialTransformFromCachePlayer()
{
	// == Retrieves the transform information of a specific Chaos Cache stored in the Cache Collection (For Static Mesh)
	mCacheCollection = mCachePlayer->CacheCollection;
	mChaosCache = mCacheCollection->FindCache(mPlayCacheName);
	const FCacheSpawnableTemplate& Spawnable = mChaosCache->GetSpawnableTemplate();
	mInitialComTransform = Spawnable.InitialTransform;
	mInitialComLocation = mInitialComTransform.GetLocation();

}


