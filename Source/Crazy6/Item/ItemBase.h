#pragma once

#include "EngineMinimal.h"
#include "CustomItemData.h"
#include "../Player/InfoData/WeaponInfo.h"
#include "ItemBase.generated.h"

class APlayerBase;

UCLASS()
class CRAZY6_API AItemBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemBase();

	void SpawnItem(const FVector& DropLocation, const FRotator& DropRotation) const;
	void SpawnItem(const FVector& DropLocation, const FRotator& DropRotation, const EItemType& ItemType,
	               const FWeaponDetails& PlayerWeaponAmmo) const;
	void SetDataAndMesh(EItemType ItemType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TimerbCanPlayerUse();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* mSphere;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* mMesh;

	UPROPERTY(VisibleAnywhere)
	class URotatingMovementComponent* mRotationMesh;

	// Niagara
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> mCircleIndicatorFXComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraSystem> mCircleIndicatorFXSystem;

	// Overlap Funtion
public:
	UFUNCTION()
	void TriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void TriggerEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void UseItem(APlayerBase* TriggerPlayer);

	// UFUNCTION()
	// void UseItemPopUp();
	//
	UFUNCTION(Server, Unreliable)
	void UseItemServer(APlayerBase* TriggerPlayer);

	//UI

	//timer
private:
	// widget timer
	FTimerHandle TimerHandle;
	// item timer
	FTimerHandle ItemTimer;
	float Time = 2.5f; // 2.5 seconds 

public:
	//name
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetComponent> mItemNameWidgetComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UItemNameWidget> mItemNameWidget;

	//dailog
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UItemDailogWidget> mItemDailogWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> mItemDailogWidgetClass;

	//sound base
	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* mAudioComponent;

	// Sound to play component
	UPROPERTY(EditAnywhere)
	class USoundBase* mSoundToPlay;

	UPROPERTY(Replicated, EditAnywhere)
	FItemData mItemData;

public:
	//Widget Visible Function, Destroy
	UFUNCTION()
	void ViewportFalse();
	//Item Visible Fuction
	UFUNCTION()
	void VisibleFalse();
	UFUNCTION(Server, Unreliable)
	void VisibleFalseServer();
	UFUNCTION(NetMulticast, Unreliable)
	void VisibleFalseMulticast();
	// UFUNCTION(Server,Reliable)
	// void VisbleItem();
	// void VisbleItem_Implementation();
	UFUNCTION(Server, Reliable)
	void DestroyServerItem();
	void DestroyServerItem_Implementation();


	//Item Datatable 
public:
	bool GetbCanPlayerUse() const { return bCanPlayerUse; }
	void SetData(EItemType ItemType);
	void SetbIsWeapon(bool IsWeapon) { bIsWeapon = IsWeapon; }
	void SetWeaponData(const FWeaponDetails& WeaponDetails)
	{
		mWeaponDetails = WeaponDetails;
	};

	EItemType GetWeightedRandomItem(const TMap<EItemType, int32>& DropRates);

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	EItemType mItemType = EItemType::None;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* mStaticMesh;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FVector mRelativeLocation;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FRotator mRelativeRotation;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FVector mRelativeScale;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FString mItemName = "";
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FString mDescription;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FWeaponDetails mWeaponDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsWeapon = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool bIsPlayerDropped = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool bCanPlayerUse = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	class UDataTable* mDataTable;

	TMap<EItemType, int32> mItemTypeMap =
	{
		{EItemType::Magazine, 40},
		{EItemType::HpPotion, 20},
		{EItemType::Rifle, 10},
		{EItemType::ShotGun, 10},
		{EItemType::MachineGun, 10},
		{EItemType::RPG, 10}
	};

	TArray<EItemType> mItemTypeArr =
	{
		EItemType::HpPotion,
		EItemType::Magazine,
		EItemType::Rifle,
		EItemType::ShotGun,
		EItemType::MachineGun,
		EItemType::RPG,
		// EItemType::Sniper
	};
};
