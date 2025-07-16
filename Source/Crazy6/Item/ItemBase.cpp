// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "Networking.h"
#include "Net/UnrealNetwork.h"

#include "../Player/PlayerBase.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "../Widget/ItemDailogWidget.h"
#include "../Widget/ItemNameWidget.h"
#include "../Widget/UIHeader.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Crazy6/Global/ProfileInfo.h"
#include "Crazy6/Player/Weapon/PlayerMasterWeapon.h"


// Sets default values
AItemBase::AItemBase()
{
	// NetWorking
	bReplicates = true;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mRotationMesh = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMesh"));
	mAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	//Replicate the object itself
	mMesh->SetIsReplicated(true);
	mSphere->SetIsReplicated(true);

	SetRootComponent(mSphere);
	mMesh->SetupAttachment(mSphere);
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mSphere->SetSphereRadius(100.0f);
	mSphere->BodyInstance.SetCollisionProfileName(UE_COLLISION_PROFILE_EVENTTRIGGER);
	mRotationMesh->RotationRate = FRotator(0.f, 50.f, 0.f);
	mAudioComponent->bAutoActivate = false;

	// First Height Change in GamePlayMode
	SetActorLocation(FVector(0.0f, 0.0f, 63.0f));

	// Sound Setting
	static ConstructorHelpers::FObjectFinder<USoundBase> Sound(
		TEXT("/Script/Engine.SoundWave'/Game/Dev/UI/Sound/LemonSound.LemonSound'"));
	if (Sound.Succeeded())
	{
		mSoundToPlay = Sound.Object;
		mAudioComponent->SetSound(mSoundToPlay);
	}

	//Datatable Setting
	static ConstructorHelpers::FObjectFinder<UDataTable> DataObject(
		TEXT("/Script/Engine.DataTable'/Game/Dev/UI/Data/ItemData.ItemData'"));
	if (DataObject.Succeeded())
	{
		mDataTable = DataObject.Object;
	}
	/*ItemName UI Setting*/
	mItemNameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemName"));
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/ItemNameWidget.ItemNameWidget_C'"));
		if (WidgetClass.Succeeded())
		{
			mItemNameWidgetComponent->SetWidgetClass(WidgetClass.Class);
		}
	}

	mItemNameWidgetComponent->SetupAttachment(mSphere);
	mItemNameWidgetComponent->SetDrawSize(FVector2D(381.0f, 42.0f));
	mItemNameWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	mItemNameWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	mItemNameWidgetComponent->SetVisibility(false);

	/*Item Dailog Setting*/
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/UITextWidget.UITextWidget_C'"));
		if (WidgetClass.Succeeded())
		{
			mItemDailogWidgetClass = WidgetClass.Class;
		}
	}

	// ======== Niagara
	mCircleIndicatorFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ItemCircleIndicator"));
	mCircleIndicatorFXComponent->SetupAttachment(mMesh);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(
		TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/UI/AttackIndicator/NS_CircleOutline.NS_CircleOutline'"));
	if (NE.Succeeded())
	{
		mCircleIndicatorFXComponent->SetAsset(NE.Object);
		// Niagara
		mCircleIndicatorFXComponent->SetRelativeLocation(FVector(-4.0f, 0.0f, 0.0f));
		mCircleIndicatorFXComponent->SetVariableFloat(TEXT("Scale"), 96.187332f);
	}
}

void AItemBase::SpawnItem(const FVector& DropLocation, const FRotator& DropRotation) const
{
	// Linetrace 
	FVector TraceEnd = DropLocation - FVector(0, 0, 1000);
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, DropLocation, TraceEnd, ECC_GameTraceChannel7, Params))
	{
		FVector GroundLocation = HitResult.Location;
		// spawn
		AItemBase* DroppedItem = GetWorld()->SpawnActor<AItemBase>(AItemBase::StaticClass(), GroundLocation, DropRotation);

		if (DroppedItem)
		{
			DroppedItem->SetDataAndMesh(EItemType::None);
		}
	}
}

void AItemBase::SpawnItem(const FVector& DropLocation, const FRotator& DropRotation,
	const EItemType& ItemType, const FWeaponDetails& PlayerWeaponAmmo) const
{
	// Linetrace 
	FVector TraceEnd = DropLocation - FVector(0, 0, 1000);
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, DropLocation, TraceEnd, ECC_GameTraceChannel7, Params))
	{
		FVector GroundLocation = HitResult.Location + (GetActorForwardVector() * 50.f);
		// spawn 
		AItemBase* DroppedItem = GetWorld()->SpawnActor<AItemBase>(AItemBase::StaticClass(), GroundLocation, DropRotation);

		if (DroppedItem)
		{
			DroppedItem->SetDataAndMesh(ItemType);
			DroppedItem->SetWeaponData(PlayerWeaponAmmo);
			DroppedItem->SetbIsWeapon(true);
		}
	}
}

void AItemBase::SetDataAndMesh(EItemType ItemType)
{
	if (HasAuthority())
	{
		/* From Enemy */
		if (EItemType::None == ItemType)
		{
			ItemType = GetWeightedRandomItem(mItemTypeMap);
			// int8 DropItemIndex = FMath::RandHelper(mItemTypeArr.Num());
			// ItemType = mItemTypeArr[DropItemIndex];	
		}
		SetData(ItemType);
		mMesh->SetRelativeLocation(mRelativeLocation);
		mMesh->SetRelativeRotation(mRelativeRotation);
		mMesh->SetRelativeScale3D(mRelativeScale);
		mMesh->SetStaticMesh(mStaticMesh);
	}
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	mSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::TriggerBeginOverlap);
	mSphere->OnComponentEndOverlap.AddDynamic(this, &AItemBase::TriggerEndOverlap);

	GetWorld()->GetTimerManager().SetTimer(ItemTimer, this, &AItemBase::TimerbCanPlayerUse, 1.f, false);
}

void AItemBase::TimerbCanPlayerUse()
{
	bCanPlayerUse = true;
	GetWorld()->GetTimerManager().ClearTimer(ItemTimer);
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AItemBase, mItemType);
	DOREPLIFETIME(AItemBase, mRelativeLocation);
	DOREPLIFETIME(AItemBase, mRelativeRotation);
	DOREPLIFETIME(AItemBase, mRelativeScale);
	DOREPLIFETIME(AItemBase, mStaticMesh);
	DOREPLIFETIME(AItemBase, mItemName);
	DOREPLIFETIME(AItemBase, mDescription);
	DOREPLIFETIME(AItemBase, mWeaponDetails);
	DOREPLIFETIME(AItemBase, bCanPlayerUse);
	DOREPLIFETIME(AItemBase, bIsPlayerDropped);
	DOREPLIFETIME(AItemBase, mDataTable);
}

void AItemBase::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	APlayerBase* mPlayer = Cast<APlayerBase>(OtherActor);
	if (mPlayer)
	{
		mPlayer->SetOverlapItem(mItemType, this);
	}
	if (mPlayer && mPlayer->IsLocallyControlled())
	{
		// HUD view
		if (IsValid(mItemNameWidgetComponent))
		{
			mItemNameWidget = Cast<UItemNameWidget>(mItemNameWidgetComponent->GetWidget());
			if (mItemNameWidget && "" != mItemName)
			{
				mItemNameWidget->SetText(FText::FromString(mItemName));
				mItemNameWidgetComponent->SetVisibility(true);
			}
		}
		if (nullptr != mAudioComponent
			&& nullptr != mSoundToPlay)
		{
			mAudioComponent->Play();
		}
	}
}

void AItemBase::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerBase* mPlayer = Cast<APlayerBase>(OtherActor);

	if (mPlayer)
	{
		mPlayer->SetEndOverlappingItem();
	}
	if (mPlayer && mPlayer->IsLocallyControlled())
	{
		mItemNameWidgetComponent->SetVisibility(false);
	}
}


void AItemBase::UseItem(APlayerBase* TriggerPlayer)
{
	if(IsValid(TriggerPlayer))
	{
		if (IsValid(mItemDailogWidgetClass) && !IsValid(mItemDailogWidget))
		{
			mItemDailogWidget = CreateWidget<UItemDailogWidget>(GetWorld(), mItemDailogWidgetClass);
		}
		if (IsValid(mItemDailogWidget))
		{
			mItemDailogWidget->SetText(mDescription);
			mItemDailogWidget->AddToViewport();
			mItemDailogWidget->PlayMyAnimation();
		}
		VisibleFalse();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AItemBase::ViewportFalse, 1.5f, false);

		if (IsValid(TriggerPlayer))
		{
			if (HasAuthority())
			{
				if (bIsWeapon)
				{
					if (TriggerPlayer->GetWeaponInstance())
					{
						TriggerPlayer->GetWeaponInstance()->SetWeaponDetials(mWeaponDetails);
					}
				}
				TriggerPlayer->SetEndOverlappingItem();	
			}
			else
			{
				UseItemServer(TriggerPlayer);
			}

		}
	}
}

void AItemBase::UseItemServer_Implementation(APlayerBase* TriggerPlayer)
{
	if (bIsWeapon)
	{
		if (TriggerPlayer->GetWeaponInstance())
		{
			TriggerPlayer->GetWeaponInstance()->SetWeaponDetials(mWeaponDetails);
		}
	}
	TriggerPlayer->SetEndOverlappingItem();	
}

void AItemBase::VisibleFalse()
{
	if (!HasAuthority())
	{
		VisibleFalseServer();
	}
	if (HasAuthority())
	{
		VisibleFalseMulticast();
	}
}

void AItemBase::VisibleFalseServer_Implementation()
{
	VisibleFalseMulticast();
}

void AItemBase::VisibleFalseMulticast_Implementation()
{
	mMesh->SetVisibility(false);
	mItemNameWidgetComponent->SetVisibility(false);
	mCircleIndicatorFXComponent->SetVisibility(false);	
}

void AItemBase::DestroyServerItem_Implementation()
{
	if (HasAuthority())
	{
		Destroy();
	}
}

void AItemBase::ViewportFalse()
{
	if (!HasAuthority())
	{
		mItemDailogWidget->RemoveFromParent();
		DestroyServerItem();
	}
	if (HasAuthority())
	{
		mItemDailogWidget->RemoveFromParent();
		Destroy();
	}
}

void AItemBase::SetData(EItemType ItemType)
{
	if (HasAuthority())
	{
		const FString ContextString(TEXT("Item Data"));
		// RowName
		FString EnumName = UEnum::GetValueAsString(ItemType);
		FString CleanedEnumName = EnumName.Replace(TEXT("EItemType::"), TEXT(""));
		FName RowName = FName(*CleanedEnumName);
		FItemData* mItemDataTable = mDataTable->FindRow<FItemData>(RowName, ContextString);
		if (mItemDataTable != nullptr)
		{
			mItemType = mItemDataTable->DataItemType;
			mStaticMesh = mItemDataTable->DataStaticMesh;
			mRelativeLocation = mItemDataTable->DataRelativeLocation;
			mRelativeRotation = mItemDataTable->DataRelativeRotation;
			mRelativeScale = mItemDataTable->DataRelativeScale;
			mItemName = mItemDataTable->DataItemName;
			mDescription = mItemDataTable->DataDescription;
			mWeaponDetails = mItemDataTable->DataWeaponDetails;
			bIsWeapon  = mItemDataTable->DataIsWeapon;
		}
	}
}

EItemType AItemBase::GetWeightedRandomItem(const TMap<EItemType, int32>& DropRates)
{
	int32 TotalWeight = 0;
	for (const auto& EItem : DropRates)
	{
		TotalWeight += EItem.Value;
	}

	if (TotalWeight <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("AItemBase::GetWeightedRandomItem : Total weight is Zero or invalid."));
		ensureMsgf(false, TEXT("DropRates must have valid weights!"));
		return DropRates.begin()->Key;
	}

	float RandomValue = FMath::FRand() * TotalWeight;

	for (const auto& Elem : DropRates)
	{
		if (RandomValue <= Elem.Value)
		{
			return Elem.Key;
		}
		RandomValue -= Elem.Value;
	}

	UE_LOG(LogTemp, Error, TEXT("AItemBase::GetWeightedRandomItem : Unexpected error!"));
	ensureMsgf(false, TEXT("Logic error in GetWeightedRandomItem!"));

	return DropRates.begin()->Key;
}