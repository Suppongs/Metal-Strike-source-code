// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Global/InputData.h"
#include "../Item/CustomItemData.h"
#include "../Item/ItemBase.h"
#include "InfoData/PlayerEnum.h"
#include "Crazy6/Actor/TeamEnum.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerBase.generated.h"


class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UCharacterMovementComponent;

class ULyraPlayerAnimInstance;
class APlayerMasterWeapon;
class UPlayerWeaponSystem;

class AEnemyBase;

class AItemBase;
class UUserWidget;
class UPlayerHUDWidget;
class UMainTeamUIWidget;
class UChangeModeWidget;
class UDashCooltimeWidget2;
class UPlayerDeathWidget;
class APostProcessVolume;

class UWidgetComponent;
class URebirthUI;
class UPlayerName;

class UMinimapWidget;
class USceneCaptureComponent2D;
class UCanvasRenderTarget2D;
class UMaterial;
class UPaperSpriteComponent;
class UMaterialInstanceDynamic;

USTRUCT(BlueprintType)
struct FOrientationAngle
{
   GENERATED_USTRUCT_BODY()

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UpdateOrientation")
   double mNetFAngle = 0.0;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UpdateOrientation")
   double mNetRAngle = 0.0;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UpdateOrientation")
   double mNetLAngle = 0.0;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UpdateOrientation")
   double mNetBAngle = 0.0;
};

UCLASS()
class CRAZY6_API APlayerBase : public ACharacter
   , public IGenericTeamAgentInterface
{
   GENERATED_BODY()

public:
   APlayerBase();
   virtual void Tick(float DeltaTime) override;

protected:
   virtual void PossessedBy(AController* NewController) override;
   virtual void BeginPlay() override;

   void SetupPlayerInput();
   
   virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
   // ==================== [ Delegate ] ====================
   UFUNCTION()
   void OnMontageEnded(class UAnimMontage* Montage, bool bInterrupted);
   // ======================================================
   
   // ==================== [ Components ] ====================
protected:
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Component")
   TObjectPtr<USpringArmComponent>           mArm;

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Component")
   TObjectPtr<UCameraComponent>              mCamera;

   UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Component")
   TObjectPtr<UCharacterMovementComponent>   mMovement;

   UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Component")
   TObjectPtr<USkeletalMeshComponent>        mNetWeaponBackMesh;

   UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Component")
   TObjectPtr<USceneComponent>               mNetWeaponHand;

   UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Component")
   TObjectPtr<USceneComponent>               mNetWeaponBack;

   UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Component")
   TObjectPtr<UChildActorComponent>          mNetWeaponHandChild;

   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
   TObjectPtr<UBoxComponent>                 mRebirthBox;

   // ==================== [ InputAction ] ====================
public:
   virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
   bool CanAction() const;
   void MoveAction(const FInputActionValue& Value);
   void JumpAction(const FInputActionValue& Value);

   /* ------------------------- Crouch ------------------------- */
   void CrouchAction(const FInputActionValue& Value);
   void UnCrouchAction(const FInputActionValue& Value);

   UFUNCTION(Server, UnReliable)
   void CrouchServer(bool _bNetIsCrouch);

   UFUNCTION(NetMulticast, UnReliable)
   void CrouchMulticast();
   /* ------------------------- Dash ------------------------- */
protected:
   void DashAction(const FInputActionValue& Value);

   UFUNCTION(Server, UnReliable)   
   void DashServer(EDashKey _mNewDashKey);

   UFUNCTION(NetMulticast, UnReliable)
   void DashMulticast(EDashKey _mNewDashKey);

   bool CanDash() const;

   UFUNCTION()
   void PlayDash(EDashKey _mNewDashKey);
   
   void TimerDash();
   /* ------------------------------------------------------------ */
   
   void LookMouseAction(const FInputActionValue& Value);
   void MenuAction(const FInputActionValue& Value);   /* Esc Widget */

   /* ------------------------- Sprint ------------------------- */
   void SprintAction(const FInputActionValue& Value);
   void NotSprintAction(const FInputActionValue& Value);
   
   UFUNCTION(Server, Unreliable)
   void SprintServer(bool _bNetIsSprint);

   UFUNCTION()
   void OnRep_bNetIsSprint() const;
   /* ------------------------------------------------------------ */

   /* ------------------------- Fire ------------------------- */
   void FireAction(const FInputActionValue& Value);
   void FireEndAction(const FInputActionValue& Value);
   
   UFUNCTION(Server, Unreliable)
   void FireActionServer(bool IsFire);

   bool CanFire();
   /* ------------------------- Reload ------------------------- */
   void ReloadAction(const FInputActionValue& Value);
   
   UFUNCTION(Server, Unreliable)
   void ReloadActionServer();
   
   /* ------------------------- EquipWeapon ------------------------- */
   void EquipWeaponAction(const FInputActionValue& Value);

   void EquipOrUnequip();

   UFUNCTION(Server, Unreliable)
   void EquipOrUnequipServer();
   
   UFUNCTION(Server, Unreliable)
   void PlayEquipAnimServer();

   UFUNCTION(NetMulticast, Unreliable)
   void PlayEquipAnimMulticast();

   UFUNCTION()
   void OnRep_bNetIsEquip();
   /* ------------------------- Zoom ------------------------- */
   void ZoomStartAction(const FInputActionValue& Value);
   void ZoomEndAction(const FInputActionValue& Value);

   void TimerZoom();
   /* ------------------------------------------------------------ */   

   void SwapABModeAction(const FInputActionValue& Value);

   /* ------------------------- Interaction ------------------------- */
   void InteractionAction(const FInputActionValue& Value);

   UFUNCTION(Server, Unreliable)
   void InteractionServer();
   
   /* ------------------------- Drop ------------------------- */
   void DropAction(const FInputActionValue& Value);

   void Drop(const FWeaponDetails& WeaponAmmo);

   void DropWeapon(const FWeaponDetails& WeaponAmmo);
   
   UFUNCTION(Server, Unreliable)
   void DropServer(const FWeaponDetails& WeaponAmmo);
   
   // ==================== [ InputAction|Dash ] ====================
   FTimerHandle DashTimerHandle;

   UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|Dash|Status")
   bool bNetIsDashing      = false;

   UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|Dash")
   double mDashSpeed       = 5000.0;

   UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|Dash")
   FVector2D mDashAxis     = { 0, 0 };

   UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|Dash")
   EDashKey mOldDashKey    = EDashKey::None;

   UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|Dash")
   EDashKey mNetNewDashKey = EDashKey::None;

   /* ---------- DashDoubleTap ---------- */
   UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "InputAction|Dash|CheckDoubleTap")
   double mTapDashLimitTerm      = 0.4;

   UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|Dash|CheckDoubleTap")
   double mCurrentTapDashTime    = 0.0;

   UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|Dash|CheckDoubleTap")
   double mLastTapDashTime       = 0.0;
   
   /* ---------- DashCoolTime ---------- */
   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "InputAction|Dash|CoolTime")
   double mDashCoolTime          = 3.0;

   UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|Dash|CoolTime")
   double mRemainDashCoolTime    = 0.0;
   
   // ==================== [ InputAction|Sprint ] ====================
   UPROPERTY(ReplicatedUsing = OnRep_bNetIsSprint, VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|Sprint")
   bool bNetIsSprint    = false;

   float mWalkSpeed     = 400.f;
   float mSprintSpeed   = 800.f;

   // ==================== [ InputAction|Fire ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "InputAction|Fire")
   bool bNetIsFire      = false;

   // ==================== [ InputAction|Reload ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "InputAction|Reload")
   bool bNetIsReload   = false;

   // ==================== [ InputAction|EquipWeapon ] ====================
   UPROPERTY(ReplicatedUsing = OnRep_bNetIsEquip, VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|EquipWeapon")
   bool bNetIsEquip        = false;

   UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|EquipWeapon")
   bool bNetWantsUnequip   = false;

   // ==================== [ InputAction|Zoom ] ====================
   FTimerHandle CameraZoomTimerHandle;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InputAction|Zoom")
   bool bClientIsAim             = false;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InputAction|Zoom")
   float mClientDefaultArmLength       = 350.0f;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InputAction|Zoom")
   float mClientAimedArmLength         = 100.0f;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InputAction|Zoom")
   float mClientZoomSpeed              = 10.0f;

   // ==================== [ InputAction|Drop ] ====================
   UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "InputAction|Drop")
   bool bNetWantsDrop = false;
   
   // ==================== [ Team ] ====================
public:
   virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(static_cast<uint8>(ETeamID::PLAYER)); }

   // ==================== [ Weapon ] ====================
   TObjectPtr<APlayerMasterWeapon>  GetWeaponInstance() const  { return mNetWeaponInstance; }
   bool                             GetIsAMode() const         { return bIsAMode; }
   
   void SetMasterWeapon(EItemType WeaponType);

   UFUNCTION(Server, Unreliable)
   void SetMasterWeaponServer(EItemType WeaponType);

private:
   void CreateWeapon();

   UFUNCTION(Server, UnReliable)
   void CreateWeaponServer();

   void ExecuteCreateWeaponLogic();
   
   void SetWeaponVisibility(bool IsBackVisible, bool IsHandVisible);
   
   UFUNCTION(Server, UnReliable)
   void SetWeaponVisibilityServer(bool IsBackVisible, bool IsHandVisible);
   
   UFUNCTION(NetMulticast, UnReliable)
   void SetWeaponVisibilityMulticast(bool IsBackVisible, bool IsHandVisible);

   void DestroyWeapon();
   
   UFUNCTION()
   void OnRep_mNetWeaponInstance();

protected:
   FName mWeaponHandSocketName = "weapon_r_muzzle";
   FName mWeaponBackSocketName = "RifleHost_Socket";
   
   UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon|State")
   bool   bIsAMode      = true;

   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
   TObjectPtr<UPlayerWeaponSystem>     mNetWeaponSystem;

   UPROPERTY()
   TMap<EItemType, TSubclassOf<APlayerMasterWeapon>>     mWeaponClassMap;

   UPROPERTY()
   TMap<EItemType, TSubclassOf<ULyraPlayerAnimInstance>> mWeaponAnimClassMap;

   UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Weapon")
   TSubclassOf<APlayerMasterWeapon>    mNetMasterWeapon;

   TSubclassOf<APlayerMasterWeapon>    mRifleClass;
   TSubclassOf<APlayerMasterWeapon>    mMachineGunClass;
   TSubclassOf<APlayerMasterWeapon>    mShotGunClass;
   TSubclassOf<APlayerMasterWeapon>    mRPGClass;
   TSubclassOf<APlayerMasterWeapon>    mSniperClass;

   UPROPERTY(ReplicatedUsing = OnRep_mNetWeaponInstance, VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
   TObjectPtr<APlayerMasterWeapon>           mNetWeaponInstance = nullptr;

   // ==================== [ Animations ] ====================
public:
   ULyraPlayerAnimInstance* GetAnimInstance() { return mLyraAnimInstance; }

protected:
   TObjectPtr<ULyraPlayerAnimInstance>    mLyraAnimInstance;

   TSubclassOf<ULyraPlayerAnimInstance>   mUnArmedAnimClass;
   TSubclassOf<ULyraPlayerAnimInstance>   mRifleAnimClass;
   TSubclassOf<ULyraPlayerAnimInstance>   mMachineGunAnimClass;
   TSubclassOf<ULyraPlayerAnimInstance>   mShotGunAnimClass;
   TSubclassOf<ULyraPlayerAnimInstance>   mRPGAnimClass;
   TSubclassOf<ULyraPlayerAnimInstance>   mSniperAnimClass;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Dash")
   TObjectPtr<UAnimMontage> mFowardDashMontage;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Dash")
   TObjectPtr<UAnimMontage> mBackwardDashMontage;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Dash")
   TObjectPtr<UAnimMontage> mRightDashMontage;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Dash")
   TObjectPtr<UAnimMontage> mLeftDashMontage;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Equip")
   TObjectPtr<UAnimMontage> mWeaponEquipMontage;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Equip")
   TObjectPtr<UAnimMontage> mWeaponUnequipMontage;
   
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Hit")
   TObjectPtr<UAnimMontage> mHitMontageLight;
   
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Hit")
   TObjectPtr<UAnimMontage> mHitMontageHeavy;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|DeathRebirth")
   TObjectPtr<UAnimMontage> mDeathMontage;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|DeathRebirth")
   TObjectPtr<UAnimMontage> mRebirthMontage;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|DeathEffect")
   TObjectPtr<UParticleSystemComponent>		mDeathParticle;
   
   // ==================== [ Status ] ====================
public:
   float GetPlayerCurHP() const { return mNetPlayerCurHP; }
   void  SetPlayerCurHP(float ApplyHP);
   void ResetPlayerStatus();
   
protected:
   virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
   AController* EventInstigator, AActor* DamageCauser) override;

   UFUNCTION(NetMulticast, Unreliable)
   void HitMulticast(float Damage);
   
   UFUNCTION()
   void OnRep_mNetPlayerCurHP();


protected:
   UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
   float mServerPlayerMaxHP      = 100.f;

   UPROPERTY(ReplicatedUsing = OnRep_mNetPlayerCurHP, EditAnywhere, BlueprintReadWrite, Category = "Status")
   float mNetPlayerCurHP   = mServerPlayerMaxHP;

   // ==================== [ Die/Rebirth ] ====================
public:
   bool           GetIsDead()             const { return bNetIsDead; }
   APlayerBase*   GetDeadOtherPlayer()    const { return mNetDeadOtherPlayer; }

   void SetDeadOtherPlayer(APlayerBase* DeadPlayer);
   
   UFUNCTION(Server, Reliable)
   void SetDeadOtherPlayerServer(APlayerBase* DeadPlayer);
   
   bool CanRebirth() const
   {
      if (0 == mNetRebirthCount && bNetCanRebirth)
      {
         return true;
      }
      return false;
   }

protected:
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

   void Die();
   
   UFUNCTION(Server, Reliable)
   void DieServer();

   UFUNCTION(NetMulticast, Reliable)
   void DieMulticast();
   
   void ExecuteDieLogic();
   // void TimerDeath();
   
   void Rebirth(APlayerBase* Reviver);
   
   UFUNCTION(Server, Reliable)
   void RebirthServer(APlayerBase* Reviver);

   UFUNCTION(NetMulticast, Reliable)
   void RebirthMulticast();
   
   void ExecuteRebirthLogic(APlayerBase* Reviver);
   
protected:
   UPROPERTY(Replicated, VisibleInstanceOnly, Category = "Status")
   int8 mNetRebirthCount = 0;
   
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Status")
   bool bNetIsDead         = false;

   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Status")
   bool bNetCanRebirth     = false;

   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Status")
   TObjectPtr<APlayerBase> mNetDeadOtherPlayer = nullptr;
   
   // ========== [ Replication|To LyraPlayerAnimInstance ] ==========
public:
   void UpdatePlayerStates();

private:
   UFUNCTION()
   void UpdateLandState();

   UFUNCTION()
   void UpdateTurnRate();
   
   UFUNCTION()
   void UpdateAcceleration();

   UFUNCTION()
   void UpdateWallDetection();

   UFUNCTION()
   void RunningIntoWall();

   UFUNCTION()
   void UpdateCharacterState();

   UFUNCTION()
   void UpdateVelocityAndGroundSpeed();

   UFUNCTION()
   void UpdateCheckMoving();

   UFUNCTION()
   void UpdateFalling();

   UFUNCTION()
   void UpdateDirection();

   UFUNCTION()
   void UpdateMoveState();                    

   UFUNCTION()
   void UpdateOrientation();

   UFUNCTION()
   void UpdateTurnInPlace();

private:
   // ==================== [ UpdateLandState ] ====================
   UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateLandState")
   ELandState mNetLandState      = ELandState::None;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateLandState")
   float mServerLandDepth        = 0.0f;

   // ==================== [ UpdateTurnRate ] ====================
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|TurnInPlace")
   double mServerTurnRate        = 0.0;
   
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|TurnInPlace")
   double mNetDirectionAngle     = 0.0;
   
   // ==================== [ UpdateMoveState ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateMoveState")
   EMovementDirectionState mNetEMoveState = EMovementDirectionState::None;

   // ==================== [ UpdateAcceleration ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateAcceleration")
   bool     bNetHasAcceleration        = false;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|pdateAcceleration")
   FRotator mServerWorldRotation       { 0, 0, 0 };        
                                               
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|pdateAcceleration")
   FVector mServerLocalAcceleration2D  { 0, 0, 0 };

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|pdateAcceleration")
   FVector mServerWorldAcceleration2D  { 0, 0, 0 };

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|pdateAcceleration")
   FVector mServerWorldVelocity        { 0, 0, 0 };

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|pdateAcceleration")
   FVector mServerWorldVelocity2D      { 0, 0, 0 };

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|pdateAcceleration")
   FVector mServerLocalVelocity2D      { 0, 0, 0 };

   // ==================== [ UpdateWallDetection ] ====================
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateWallDetection")
   bool bServerIsRunningWall   = false;
   
   // ==================== [ RunningIntoWall ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|RunningIntoWall")
   bool bNetIsWall            = false;

   // ==================== [ UpdateCharacterState ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateCharacterState")
   bool bNetIsCrouch          = false;

   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateCharacterState")
   double mNetPitch           = 0.0;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateCharacterState")
   bool bServerIsJump         = false;

   // ==================== [ UpdateVelocityAndGroundSpeed ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateVelocityAndGroundSpeed")
   FVector mNetVelocity       { 0, 0, 0 };

   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateVelocityAndGroundSpeed")
   double mNetGroundSpeed     = 0.0;

   // ==================== [ UpdateCheckMoving ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateCheckMoving")
   bool bNetIsShouldMove         = false;
   
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateCheckMoving")
   FVector mServerAcceleration   { 0, 0, 0 };

   // ==================== [ UpdateFalling ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateFalling")
   bool bNetIsFalling         = false;
      
   // ==================== [ UpdateDirection ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateDirection")
   double mNetDirection      = 0.0;

   // ==================== [ UpdateOrientation ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|UpdateOrientation")
   FOrientationAngle mNetOrientationAngle;

   // ==================== [ TurnInPlace ] ====================
   UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|TurnInPlace")
   double  mNetRootYawOffset   = 0.0;
         
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|TurnInPlace")
   double  mServerYawExcess         = 0.0;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|TurnInPlace")
   double  mServerAbsRootYawOffset      = 0.0;

public:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|TurnInPlace")
   FRotator mServerMovingRotation      { 0, 0, 0 };

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|TurnInPlace")
   FRotator mServerLastMovingRotation   { 0, 0, 0 };
   
private:
   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|TurnInPlace")
   double  mServerDistanceCurve      = 0.0;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|TurnInPlace")
   double  mServerLastDistanceCurve   = 0.0;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Replication|TurnInPlace")
   double  mServerDeltaDistanceCurve   = 0.0;

   // ==================== [ Item ] ====================
public:
   void SetOverlapItem(EItemType ItemType,AItemBase* Item);
   void SetEndOverlappingItem();

protected:
   void UsePlayerItem();
   
   UFUNCTION(Client, Unreliable)
   void UsePlayerItemClient(APlayerBase* TriggerPlayer);
   
   void ChangeAnimInstanceClass(UClass* AnimClass);
   
   void PickUpWeapon(EItemType OverlappedWeapon);
   
private:
   FTimerHandle AmmoTimer;
   
   UPROPERTY(Replicated)
   EItemType mEOverlapItem = EItemType::None;

   UPROPERTY(Replicated)
   EItemType mEWeaponItemType = EItemType::None;

   UPROPERTY(Replicated)
   TObjectPtr<AItemBase> mNetOverlappingItem = nullptr;

   UPROPERTY(Replicated)
   bool bNetHasItem = false;
   
   // ==================== [ UI ] ====================
public:
   bool GetbIsMenuVisible() const { return bIsMenuVisible; }
   UFUNCTION()
   void OnRep_mPlayerName();
   
   UFUNCTION(Client, Reliable)
   void DamageUIClient(AEnemyBase* HitPawn);

   UFUNCTION(Client, Reliable)
   void DamageHeadShotUIClient(AEnemyBase* HitPawn, bool IsHeadShot);
   
protected:
   void AddDashUI();
   void DeleteDashUI();
   void WeaponUIUpdate(bool IsEquip);

   
protected:
   bool bIsMenuVisible = false;

   UPROPERTY(EditAnywhere, Category = "UI|Widget|Screen")
   TObjectPtr<UUserWidget>          MenuWidget;
   
   UPROPERTY(EditAnywhere, Category = "UI|Widget|Screen")
   TObjectPtr<UPlayerHUDWidget>     mDisplayWidget;

   UPROPERTY(EditAnywhere, Category = "UI|Widget|Screen")
   TObjectPtr<UMainTeamUIWidget>     mTeamHPWidget;

   UPROPERTY(EditAnywhere, Category = "UI|Widget|Screen")
   TSubclassOf<class UMissionWidget>	mMissionWidgetClass;
   
   UPROPERTY(EditAnywhere, Category = "UI|Widget|Screen")
   TObjectPtr<UMissionWidget>	 mMissionWidget;

public:
   void UpdateTeamPlayerUI();
   void CreateMissionUI();
   UMissionWidget* GetMissionWidget()
   {
      if(!mMissionWidget)
      {
         return nullptr;
      }
      return mMissionWidget;
   } 
   
protected:
   UPROPERTY(EditAnywhere, Category = "UI|Widget|Screen")
   TObjectPtr<class UPauseWidget>   mEscWidget;

   UPROPERTY(EditAnywhere, Category = "UI|Widget|Weapon")
   TObjectPtr<UUserWidget>          mCrossHairWidget;
   
   UPROPERTY(EditAnywhere, Category = "UI|Widget|Weapon")
   TObjectPtr<UUserWidget>          mRiffleZoomWidget;

   UPROPERTY(EditAnywhere, Category = "UI|Widget|Weapon")   
   TObjectPtr<UChangeModeWidget>    mModeWidget;
   
   UPROPERTY(EditAnywhere, Category = "UI|Widget|Dash")
   TObjectPtr<UDashCooltimeWidget2> mDashWidget;
   
   
   UPROPERTY(EditAnywhere, Category = "UI|Class|Screen")
   TSubclassOf<UUserWidget> MenuWidgetClass;

   UPROPERTY(EditAnywhere, Category = "UI|Class|Screen")
   TSubclassOf<UUserWidget> mEscWidgetClass;

   UPROPERTY(EditAnywhere, Category = "UI|Class|Screen")
   TSubclassOf<UUserWidget> mDisplayWidgetClass;
   
   UPROPERTY(EditAnywhere, Category = "UI|Class|Screen")
   TSubclassOf<UUserWidget> mTeamHPWidgetClass;
   
   UPROPERTY(EditAnywhere, Category = "UI|Class|Weapon")
   TSubclassOf<UUserWidget> mCrossHairClass;

   UPROPERTY(EditAnywhere, Category = "UI|Class|Weapon")
   TSubclassOf<UUserWidget> mRiffleZoomWidgetClass;

   UPROPERTY(EditAnywhere, Category = "UI|Class|Weapon")
   TSubclassOf<UUserWidget> mModeWidgetClass;

   UPROPERTY(EditAnywhere, Category = "UI|Class|Dash")
   TSubclassOf<UUserWidget> mDashWidgetClass;

   UPROPERTY(EditAnywhere, Category = "UI|Class|Rebirth")
   TObjectPtr<UWidgetComponent> mRebirthComponent;

   UPROPERTY(EditAnywhere, Category = "UI|Class|Rebirth")
   TObjectPtr<URebirthUI> mRebirthWidget;
   
   UPROPERTY(EditAnywhere, Category = "UI|Class|Name")
   TObjectPtr<UWidgetComponent> mPlayerNameWidgetComponent;

   UPROPERTY(EditAnywhere, Category = "UI|Class|Name")
   TObjectPtr<UPlayerName> mPlayerNameWidget;

   UPROPERTY(ReplicatedUsing = OnRep_mPlayerName,EditAnywhere,Category = "UI|Class|Name")
   FText mPlayerName;

   UPROPERTY(EditAnywhere, Category = "UI|Class|Death")
   TSubclassOf<UUserWidget> mDeathWidgetClass;

   UPROPERTY(EditAnywhere, Category = "UI|Class|Death")
   TObjectPtr<UPlayerDeathWidget> mDeathWidget;
   
   
public:
   UFUNCTION()
   void RebirthUIView();
   UFUNCTION()
   void RebirthUIUnView();
   UFUNCTION(Client, Reliable)
   void RebirthUIViewClient();
   UFUNCTION(Client, Reliable)
   void RebirthUIUnViewClient();
   void SetReadyCurrentPlayer();
   UFUNCTION(Server, Unreliable)
   void SetReadyCurrentPlayerServer();
   void SetPlayerName();

 

private:
   UPROPERTY(EditAnywhere, Category = "UI|PostProcess")
   TObjectPtr<APostProcessVolume> PostProcessVolume = nullptr;
   
   /* ------------------------- Minimap ------------------------- */
public:
   TObjectPtr<USceneCaptureComponent2D> GetMinimapCaptureComp() { return mMinimapCapture; }
   TObjectPtr<UCanvasRenderTarget2D> GetMinimapRenderTarget() const { return MinimapRenderTarget.Get(); }
   TObjectPtr<UMaterial> GetMaterial() const { return mBaseRenderTargetMT; }
   
   UFUNCTION(Client, Reliable)
   void RotateMinimapWithPlayer();

protected:
   UPROPERTY(EditAnywhere, Category = "UI|Widget|Minimap")
   TObjectPtr<UMinimapWidget>       mMinimapWidget;
   
   UPROPERTY(EditAnywhere, Category = "UI|Class|Minimap")
   TSubclassOf<UUserWidget>         mMinimapWidgetClass;

private:
   UPROPERTY(EditAnywhere, Category = "UI")
   TObjectPtr<USpringArmComponent> mMinimapCamSpringArm;

   UPROPERTY(VisibleAnywhere, Category = "UI")
   TObjectPtr<USceneCaptureComponent2D> mMinimapCapture;

   UPROPERTY(EditAnywhere, Category = "UI")
   TObjectPtr<UPaperSpriteComponent> mMinimapPlayerIcon;

   UPROPERTY()
   TObjectPtr<UCanvasRenderTarget2D> MinimapRenderTarget;  // 캔버스 렌더 타겟 멤버 변수

   UPROPERTY()
   TObjectPtr<UMaterial> mBaseRenderTargetMT;

   UPROPERTY()
   TObjectPtr<UMaterialInstanceDynamic> mMinimapRenderTargetMT;
};