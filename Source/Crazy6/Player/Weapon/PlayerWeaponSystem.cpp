#include "PlayerWeaponSystem.h"
#include "PlayerMasterWeapon.h"
#include "../PlayerBase.h"
#include "Net/UnrealNetwork.h"

#include "Components/SceneComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/Actor.h"

#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"

#include "../../Widget/BulletWidget.h"
#include "Crazy6/DebugMacros.h"


UPlayerWeaponSystem::UPlayerWeaponSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerWeaponSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerWeaponSystem, mNetOwnerPlayer);
	// DOREPLIFETIME(UPlayerWeaponSystem, mNetCurrentWeapon);
}

void UPlayerWeaponSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerWeaponSystem::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);

	AActor* OwnerActor = GetOwner();

	if (Cast<APlayerBase>(OwnerActor) && OwnerActor->HasAuthority())
	{
		mNetOwnerPlayer = Cast<APlayerBase>(OwnerActor);
		// if (mNetOwnerPlayer)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
		// 		FString::Printf(TEXT("(UPlayerWeaponSystem) mOwnerPlayer : %s O"), *mNetOwnerPlayer->GetName()));
		// }
	}
}

void UPlayerWeaponSystem::UpdateWeaponState(EItemType WeaponType, EWeaponState WeaponState)
{
	if (nullptr == mNetOwnerPlayer->GetWeaponInstance())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("WeaponState : No mNetWeaponInstance")));
		return;
	}

	if (nullptr == mNetOwnerPlayer->GetMesh())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("WeaponState : No mOwnerPlayer->GetMesh()")));
		return;
	}

	if (EWeaponState::Equip == WeaponState)
	{
		// Equip();
	}
	else if (EWeaponState::Unequip == WeaponState)
	{
		// Unequip();
	}
}

// void UPlayerWeaponSystem::Equip()
// {
// }
//
// void UPlayerWeaponSystem::Unequip()
// {
// }