// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chaos/CacheManagerActor.h" 
#include "CacheTriggerBox.generated.h"


UCLASS()

/*
����ٲٱ�
������Ʈ���� �����ϴ� �����޽� �̸��� --�ϰ�� �׿����� ������ �޽� ���� �ٸ��� �������ֱ�
������ ����Ÿ�Ը��� Ŭ�����İ� ���������� ���� �� ���ͷ� �����޴µ� �̰�� �ڽĵ� �� ���ø��ɟ�, �ؾߵǼ� ���� x
��,�� ������ �θ� ������ ����Ÿ�Կ����� �ٸ������޽��� �������ִ� �ٸ� ������ �ձ⶧���� �̸� ���� �� ���� ����
*/
class CRAZY6_API ACacheTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACacheTriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


protected:
	UPROPERTY(BlueprintReadOnly, Category = "Building")
	class AChaosCacheManager* mCachePlayer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	TObjectPtr<UStaticMeshComponent> mStaticBuildingMesh = nullptr;

	FObservedComponent* mObservedComponent = nullptr;
	TObjectPtr<UChaosCacheCollection> mCacheCollection = nullptr;
	TObjectPtr<UGeometryCollectionComponent> mGeometryCollectionComponent = nullptr;
	TObjectPtr<UChaosCache> mChaosCache = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FName mRefPlayerActorTagName = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FName mPlayCacheName = "None";



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FName mBuildingTypeNumber = "None"; 

	
	FTransform mInitialComTransform{};

	UPROPERTY(BlueprintReadOnly)
	FVector mInitialComLocation = FVector::ZeroVector;


	UPROPERTY(BlueprintReadOnly, Category = "Building")
	bool bDeath = false;

	UPROPERTY(BlueprintReadWrite, Category = "Building")
	bool bCachePlay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float mMaxHP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	float mCurHP = 100.f;


	float mCachePlayTimeAcc = 0.f;

	bool bTriggerPlayEvent = false;
	

public:
	virtual float TakeDamage(float _DamageAmount, FDamageEvent const& _DamageEvent,
		AController* _EventInstigator, AActor* _DamageCauser) override;

	void FindGeometryComFromCachePlayer();
	void FindObservedComFromCachePlayer();
	void FindInitialTransformFromCachePlayer();


	void SetMaxHP(float Hp) { mMaxHP = Hp; }

	// ���⿡ ������ ���� Player�ʿ��� �Ѱ��� TakeDamage ���� ���ڷ� �����ؼ� �����ǰ� ���ָ� �ȴ�. 
	//void BuildingAttack()
	//{
	//	if (bDeath)
	//		return;

	//	CurHP -= Damage; 
	//}

	float GetCurHP() const { return mCurHP;  }
	float GetMaxHP() const { return mMaxHP; }


	bool IsDeath() const { return bDeath;  }

	void SetTriggerPlayEvent(bool _TriggerOn) { bTriggerPlayEvent = _TriggerOn; }
	
	bool IsTriggerPlayEvent() { return bTriggerPlayEvent; }

	
	
};
