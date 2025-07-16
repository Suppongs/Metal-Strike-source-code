#pragma once

#include "CoreMinimal.h"
#include "../InfoData/PlayerEnum.h"
#include "../../Item/CustomItemData.h"
#include "../InfoData/WeaponInfo.h"
#include "Components/ActorComponent.h"
#include "UObject/NoExportTypes.h"
#include "PlayerWeaponSystem.generated.h"

class UPhysicalMaterial;
class UNiagaraSystem;
class APlayerMasterWeapon;
class APlayerBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRAZY6_API UPlayerWeaponSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerWeaponSystem();

	// ==================== [ Networking ] ====================
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ========================================================

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	TObjectPtr<APlayerBase> GetOwningPlayer() { return mNetOwnerPlayer; }

	UFUNCTION()
	void UpdateWeaponState(EItemType WeaponType, EWeaponState WeaponState);

	// TODO : Equip/Unequip 여기로 옮기기
	// UFUNCTION()
	// void Equip();
	//
	// UFUNCTION()
	// void Unequip();
	
	/* ----------------------------- Combat ----------------------------- */

	// 안씀
	// void SetCurrentWeapon(APlayerMasterWeapon* CurrentWeapon) { mNetCurrentWeapon = CurrentWeapon; }

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<APlayerBase>			mNetOwnerPlayer	= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDryAmmo = false;
};
