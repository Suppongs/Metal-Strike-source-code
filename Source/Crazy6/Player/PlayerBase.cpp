#include "PlayerBase.h"

#include "cmath"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "LyraPlayerAnimInstance.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/SceneCaptureComponent2D.h"

#include "../GameMode/GameState/InGameState.h"
#include "../GameMode/PlayerState/MainPlayerState.h"
#include "../Widget/UIHeader.h"
#include "../Widget/PlayerHUDWidget.h"
#include "../Widget/MainTeamUIWidget.h"
#include "../Widget/ChangeModeWidget.h"
#include "../Widget/DashCooltimeWidget2.h"
#include "../Widget/PauseWidget.h"
#include "../Widget/PlayerDeathWidget.h"
#include "../Widget/RebirthUI.h"
#include "../Widget/PlayerName.h"
#include "../Widget/MissionWidget.h"
#include "../Widget/MinimapWidget.h"


#include "Crazy6/Actor/Enemy/EnemyBase.h"
#include "Weapon/PlayerWeaponSystem.h"
#include "Weapon/PlayerMasterWeapon.h"
#include "Crazy6/Global/ProfileInfo.h"

#include "Engine/CanvasRenderTarget2D.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Components/Image.h"
#include "Crazy6/GameMode/MainGameMode.h"

APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// [ Component Setting ]
	mArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	mMovement = GetCharacterMovement();
	mRebirthBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RebirthCollisionBox"));
	mDeathParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DeathParticle"));

	mNetWeaponHand = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponHand"));
	mNetWeaponBack = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponBack"));
	mNetWeaponBackMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponBackMesh"));
	mNetWeaponHandChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponHandChild"));
	mNetWeaponSystem = CreateDefaultSubobject<UPlayerWeaponSystem>(TEXT("WeaponSystem"));

	GetCapsuleComponent()->SetCanEverAffectNavigation(true);
	GetCapsuleComponent()->SetCollisionProfileName(UE_COLLISION_PROFILE_PLAYER);
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);

	// Default RootComponent is CapsuleComponent
	mArm->SetupAttachment(RootComponent);
	mCamera->SetupAttachment(mArm);
	GetMesh()->SetupAttachment(RootComponent);
	GetMesh()->bReceivesDecals = false;
	mRebirthBox->SetupAttachment(GetMesh());
	
	mDeathParticle->SetupAttachment(GetMesh());
	mDeathParticle->bAutoActivate = false;
	mDeathParticle->bAutoManageAttachment = true;
	
	/* Real Weapon */
	mNetWeaponHand->SetupAttachment(GetMesh());
	mNetWeaponHandChild->SetupAttachment(mNetWeaponHand);

	/* Visual Weapon */
	mNetWeaponBack->SetupAttachment(GetMesh());
	mNetWeaponBackMesh->SetupAttachment(mNetWeaponBack);
	mNetWeaponBackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -90.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mRebirthBox->SetBoxExtent({100.0, 70.0, 100.0}, true);
	mRebirthBox->SetCollisionProfileName(UE_COLLISION_PROFILE_EVENTTRIGGER);
	mRebirthBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mMovement->NetworkSmoothingMode = ENetworkSmoothingMode::Linear;

	bUseControllerRotationYaw = true;
	mArm->bInheritPitch = true;
	mArm->bInheritYaw = true;
	mArm->bInheritRoll = true;
	mArm->bEnableCameraLag = true;
	mArm->SetRelativeLocation(FVector(0.0, 40.0, 60.0));
	mArm->TargetArmLength = mClientDefaultArmLength;

	// [ Asset Loading ]
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT(
			"/Script/Engine.SkeletalMesh'/Game/Dev/Player/Characters/Heroes/Mannequin/Meshes/SKM_Manny.SKM_Manny'"));

	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMeshAsset(MeshAsset.Object);
	}

	//Loading Esc widget
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UIEscAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/PauseWidget.PauseWidget_C'"));

	if (UIEscAsset.Succeeded())
	{
		mEscWidgetClass = UIEscAsset.Class;
	}

	//Loading PlayerDiplay widget
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UIPlayerAsset(
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/PlayerHUDbasic.PlayerHUDbasic_C'"));

	if (UIPlayerAsset.Succeeded())
	{
		mDisplayWidgetClass = UIPlayerAsset.Class;
	}

	// Loading Team HP Widget
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UITeamAsset(
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/TeamWidget.TeamWidget_C'"));

	if (UITeamAsset.Succeeded())
	{
		mTeamHPWidgetClass = UITeamAsset.Class;
	}

	// Loading Death Widget
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UIDeathAsset(
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/DieWidget.DieWidget_C'"));

	if (UIDeathAsset.Succeeded())
	{
		mDeathWidgetClass = UIDeathAsset.Class;
	}

	//Loading cross hair 
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UICrossHairAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/CrossHair.CrossHair_C'"));

	if (UICrossHairAsset.Succeeded())
	{
		mCrossHairClass = UICrossHairAsset.Class;
	}

	//Loading crosshair Zoom
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UIRiffleZoomAsset(
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/CrossHair_Zoom.CrossHair_Zoom_C'"));

	if (UIRiffleZoomAsset.Succeeded())
	{
		mRiffleZoomWidgetClass = UIRiffleZoomAsset.Class;
	}

	//Loading Mode
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UIModeAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/ModeChange.ModeChange_C'"));

	if (UIModeAsset.Succeeded())
	{
		mModeWidgetClass = UIModeAsset.Class;
	}

	//Loading dash
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UIDashAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/WideDashWidget.WideDashWidget_C'"));

	if (UIDashAsset.Succeeded())
	{
		mDashWidgetClass = UIDashAsset.Class;
	}

	// Rebirth UI
	mRebirthComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("RebirthPress"));
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UIRebirthAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/RebirthUI.RebirthUI_C'"));

	if (UIRebirthAsset.Succeeded())
	{
		mRebirthComponent->SetWidgetClass(UIRebirthAsset.Class);
	}
	mRebirthComponent->SetupAttachment(GetCapsuleComponent());
	mRebirthComponent->SetDrawSize(FVector2D(381.0f, 42.0f));
	mRebirthComponent->SetWidgetSpace(EWidgetSpace::Screen);
	mRebirthComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	mRebirthComponent->SetVisibility(false);

	// PlayerName UI
	mPlayerNameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Playername"));
	static ConstructorHelpers::FClassFinder<UUserWidget>
		PlayernameWidget(
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/PlayerNameWidget.PlayerNameWidget_C'"));
	if (PlayernameWidget.Succeeded())
	{
		mPlayerNameWidgetComponent->SetWidgetClass(PlayernameWidget.Class);
	}
	// Widget component setup
	mPlayerNameWidgetComponent->SetupAttachment(GetCapsuleComponent());
	mPlayerNameWidgetComponent->SetDrawSize(FVector2D(173.0f, 36.0f));
	mPlayerNameWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	mPlayerNameWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	mPlayerNameWidgetComponent->SetVisibility(false);

	// Mission UI
	{
		static ConstructorHelpers::FClassFinder<UUserWidget>
			MissionAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/Widget/Mission.Mission_C'"));

		if (UIDashAsset.Succeeded())
		{
			mMissionWidgetClass = MissionAsset.Class;
		}
	}

	/* ------------ Weapon BP_Class ---------------- */
	/* Rifle */
	static ConstructorHelpers::FClassFinder<APlayerMasterWeapon>
		WeaponRifleAsset(TEXT(
			"/Script/Engine.Blueprint'/Game/Dev/Player/Human/Weapon/Rifle/BP_WeaponRifle.BP_WeaponRifle_C'"));

	if (WeaponRifleAsset.Succeeded())
	{
		mRifleClass = WeaponRifleAsset.Class;
	}

	/* MachineGun */
	static ConstructorHelpers::FClassFinder<APlayerMasterWeapon>
		WeaponMGAsset(TEXT(
			"/Script/Engine.Blueprint'/Game/Dev/Player/Human/Weapon/MachineGun/BP_MachineGun.BP_MachineGun_C'"));

	if (WeaponMGAsset.Succeeded())
	{
		mMachineGunClass = WeaponMGAsset.Class;
	}

	/* ShotGun */
	static ConstructorHelpers::FClassFinder<APlayerMasterWeapon>
		WeaponShotGunAsset(TEXT(
			"/Script/Engine.Blueprint'/Game/Dev/Player/Human/Weapon/ShotGun/BP_WeaponShotGun.BP_WeaponShotGun_C'"));

	if (WeaponShotGunAsset.Succeeded())
	{
		mShotGunClass = WeaponShotGunAsset.Class;
	}

	/* RPG */
	static ConstructorHelpers::FClassFinder<APlayerMasterWeapon>
		WeaponRPGAsset(
			TEXT("/Script/Engine.Blueprint'/Game/Dev/Player/Human/Weapon/RPG/BP_WeaponRPG.BP_WeaponRPG_C'"));

	if (WeaponRPGAsset.Succeeded())
	{
		mRPGClass = WeaponRPGAsset.Class;
	}

	// /* Sniper */
	// static ConstructorHelpers::FClassFinder<APlayerMasterWeapon>
	// 	WeaponSniperAsset(TEXT(
	// 		"/Script/Engine.Blueprint'/Game/Dev/Player/Human/Weapon/Sniper/BP_WeaponSinper.BP_WeaponSinper_C'"));
	//
	// if (WeaponSniperAsset.Succeeded())
	// {
	// 	mSniperClass = WeaponSniperAsset.Class;
	// }


	/* ------------ Weapon AnimInstance Class ---------------- */
	//Loading UnArmedAnimClass
	static ConstructorHelpers::FClassFinder<ULyraPlayerAnimInstance>
		UnArmAnimAsset(TEXT(
			"/Script/Engine.AnimBlueprint'/Game/Dev/Player/Human/Animations/Locomotion/Unarmed/ABP_Unarmed_Layer.ABP_Unarmed_Layer_C'"));

	if (UnArmAnimAsset.Succeeded())
	{
		mUnArmedAnimClass = UnArmAnimAsset.Class;
	}

	//Loading RifleAnimClass
	static ConstructorHelpers::FClassFinder<ULyraPlayerAnimInstance>
		RifleAnimAsset(TEXT(
			"/Script/Engine.AnimBlueprint'/Game/Dev/Player/Human/Animations/Locomotion/Rifle/ABP_Rifle_Layer.ABP_Rifle_Layer_C'"));

	if (RifleAnimAsset.Succeeded())
	{
		mRifleAnimClass = RifleAnimAsset.Class;
	}

	//Loading MachineGunAnimClass
	static ConstructorHelpers::FClassFinder<ULyraPlayerAnimInstance>
		MachineGunAnimAsset(TEXT(
			"/Script/Engine.AnimBlueprint'/Game/Dev/Player/Human/Animations/Locomotion/MachineGun/ABP_MachineGun_Layer.ABP_MachineGun_Layer_C'"));

	if (MachineGunAnimAsset.Succeeded())
	{
		mMachineGunAnimClass = MachineGunAnimAsset.Class;
	}

	//Loading ShotGunAnimClass
	static ConstructorHelpers::FClassFinder<ULyraPlayerAnimInstance>
		ShotGunAnimAsset(TEXT(
			"/Script/Engine.AnimBlueprint'/Game/Dev/Player/Human/Animations/Locomotion/ShotGun/ABP_ShotGun_Layer.ABP_ShotGun_Layer_C'"));

	if (ShotGunAnimAsset.Succeeded())
	{
		mShotGunAnimClass = ShotGunAnimAsset.Class;
	}

	//Loading RPGAnimClass
	static ConstructorHelpers::FClassFinder<ULyraPlayerAnimInstance>
		RPGAnimAsset(TEXT(
			"/Script/Engine.AnimBlueprint'/Game/Dev/Player/Human/Animations/Locomotion/RPG/ABP_RPG_Layer.ABP_RPG_Layer_C'"));

	if (RPGAnimAsset.Succeeded())
	{
		mRPGAnimClass = RPGAnimAsset.Class;
	}

	//Loading SniperAnimClass
	// static ConstructorHelpers::FClassFinder<ULyraPlayerAnimInstance>
	// 	SniperAnimAsset(TEXT(
	// 		"/Script/Engine.AnimBlueprint'/Game/Dev/Player/Human/Animations/Locomotion/Sniper/ABP_Sinper_Layer.ABP_Sinper_Layer_C'"));
	//
	// if (SniperAnimAsset.Succeeded())
	// {
	// 	mSniperAnimClass = SniperAnimAsset.Class;
	// }

	// =========== For Minimap
	//Loading Minimap Widget
	{
		static ConstructorHelpers::FClassFinder<UUserWidget>
			UMinimapAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Dev/UI/MiniMap/UI_Minimap.UI_Minimap_C'"));

		if (UMinimapAsset.Succeeded())
		{
			mMinimapWidgetClass = UMinimapAsset.Class;
		}
	}

	// Spring Arm
	mMinimapCamSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mMinimapCamSpringArm->SetupAttachment(RootComponent);
	//위에서 아래로 내려다 보는 각도를 위해 회전
	mMinimapCamSpringArm->SetWorldRotation(FRotator::MakeFromEuler(FVector(0.f, -90.f, -180.f)));
	mMinimapCamSpringArm->SetWorldLocation(FVector(0.f, 0.f, 1000.0f));

	//미니맵 자체가 회전하지 않게 고정(회전하는 미니맵을 원할 경우 true)
	mMinimapCamSpringArm->bUsePawnControlRotation = false;
	mMinimapCamSpringArm->bInheritPitch = false;
	mMinimapCamSpringArm->bInheritRoll = false;
	mMinimapCamSpringArm->bInheritYaw = false;

#pragma region mMinimapCapture(SeceneCaptureComponent)
	mMinimapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapCapture"));
	//카메라 암에 어태치
	mMinimapCapture->SetupAttachment(mMinimapCamSpringArm);
	//카메라 투영 타입을 직교로 전환하여 거리감이 없는 이미지로 구현
	mMinimapCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	//카메라에서 캡처될 크기 ( 클수록 축소되 보이는 미니맵 )
	mMinimapCapture->OrthoWidth = 30000;


#pragma endregion

#pragma region mMinimapPlayerIcon
	mMinimapPlayerIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapPlayerIcon"));
	mMinimapPlayerIcon->SetupAttachment(GetMesh());

	mMinimapPlayerIcon->SetWorldRotation(FRotator::MakeFromEuler(FVector(270.f, 0.f, 90.f)));
	mMinimapPlayerIcon->SetWorldScale3D(FVector(2.0f));
	mMinimapPlayerIcon->SetWorldLocation(FVector(0.f, 0.f, 350.0f));

	mMinimapPlayerIcon->bVisibleInSceneCaptureOnly = true;

	ConstructorHelpers::FObjectFinder<UPaperSprite> FOBJ_PaperSprite(
		TEXT("/Script/Paper2D.PaperSprite'/Game/Dev/UI/MiniMap/PS_Player.PS_Player'"));
	if (FOBJ_PaperSprite.Succeeded())
	{
		mMinimapPlayerIcon->SetSprite(FOBJ_PaperSprite.Object);
	}

#pragma endregion
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT(
		"/Script/Engine.Material'/Game/Dev/UI/MiniMap/Basic/MT_MinimapCanvasRenderTarget2D.MT_MinimapCanvasRenderTarget2D'"));
	if (MaterialFinder.Succeeded())
	{
		mBaseRenderTargetMT = MaterialFinder.Object;
		UE_LOG(LogTemp, Log, TEXT("Material successfully found in constructor: %s"), *mBaseRenderTargetMT->GetName());
	}
#pragma region mBaseRenderTargetMT


#pragma endregion

	bReplicates = true;
	bAlwaysRelevant = false;

	// SetReplicates(true);

	if (mRifleClass && mShotGunClass && mMachineGunClass && mRPGClass)
	{
		mWeaponClassMap.Add(EItemType::Rifle, mRifleClass);
		mWeaponClassMap.Add(EItemType::ShotGun, mShotGunClass);
		mWeaponClassMap.Add(EItemType::MachineGun, mMachineGunClass);
		mWeaponClassMap.Add(EItemType::RPG, mRPGClass);
	}

	if (mRifleAnimClass && mShotGunAnimClass && mMachineGunAnimClass && mRPGAnimClass)
	{
		mWeaponAnimClassMap.Add(EItemType::Rifle, mRifleAnimClass);
		mWeaponAnimClassMap.Add(EItemType::ShotGun, mShotGunAnimClass);
		mWeaponAnimClassMap.Add(EItemType::MachineGun, mMachineGunAnimClass);
		mWeaponAnimClassMap.Add(EItemType::RPG, mRPGAnimClass);
	}

	// ==================== Network Setting ====================
    mNetWeaponSystem->SetIsReplicated(true);
    mNetWeaponHandChild->SetIsReplicated(true);
    mNetWeaponHand->SetIsReplicated(true);
    mNetWeaponBack->SetIsReplicated(true);
    mNetWeaponBackMesh->SetIsReplicated(true);
    mRebirthBox->SetIsReplicated(true);
    // ========================================================
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


	//if (HasAuthority())
	{

    	SetReplicateMovement(true);
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}
	// ========================================================
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
GEngine->AddOnScreenDebugMessage(INDEX_NONE, 15.f, FColor::Red,
		FString::Printf(TEXT("Called APlayerBase::BeginPlay(), IsDead? : %d, IsServer? : %d"), bNetIsDead, HasAuthority()));

	// Mission
	CreateMissionUI();
	SetupPlayerInput();

	/* Animation Setting */
	ChangeAnimInstanceClass(mUnArmedAnimClass);

	if (HasAuthority())
	{
		/* Component Setting */
		mRebirthBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerBase::TriggerBeginOverlap);
		mRebirthBox->OnComponentEndOverlap.AddDynamic(this, &APlayerBase::TriggerEndOverlap);
		ResetPlayerStatus();
		mNetRebirthCount = 0;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	mNetWeaponBack->AttachToComponent(GetMesh(), AttachmentRules, mWeaponBackSocketName);
	mNetWeaponHand->AttachToComponent(GetMesh(), AttachmentRules, mWeaponHandSocketName);
	mDeathParticle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, mWeaponBackSocketName);

	SetMasterWeapon(EItemType::Rifle);
	CreateWeapon();
	SetWeaponVisibility(true, false);

	// ==================== Minimap Setting ====================
	if (IsLocallyControlled())
	{
		mMinimapCamSpringArm->TargetArmLength = 300.f;

		// Initialize and set the position of the SceneCaptureComponent2D.
		if (mMinimapCapture)
		{
			MinimapRenderTarget = NewObject<UCanvasRenderTarget2D>(this);
			if (MinimapRenderTarget)
			{
				MinimapRenderTarget->InitAutoFormat(320, 180);
				MinimapRenderTarget->UpdateResource();
				// After creating a render target, explicitly call to ensure synchronization with the GPU.
				mMinimapCapture->TextureTarget = MinimapRenderTarget;

				mMinimapCapture->bCaptureEveryFrame = false;
			}
		}

		if (IsValid(mBaseRenderTargetMT))
		{
			mMinimapRenderTargetMT = UMaterialInstanceDynamic::Create(mBaseRenderTargetMT, this);

			// Set the dynamically created render target to the texture parameter.
			mMinimapRenderTargetMT->SetTextureParameterValue(FName("RenderTargetParam"), MinimapRenderTarget);
		}

		FName CurLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));

		if (CurLevelName == "Bridge_P")
		{
			mMinimapCamSpringArm->SetWorldRotation(FRotator::MakeFromEuler(FVector(0.0f, -90.f, 0.0f)));
		}

		if (IsValid(mMinimapWidgetClass))
		{
			mMinimapWidget = CreateWidget<UMinimapWidget>(GetWorld(), mMinimapWidgetClass);

			FSlateBrush MinimapBrush;
			MinimapBrush.SetResourceObject(mMinimapRenderTargetMT);
			MinimapBrush.ImageSize = FVector2D(32.0f, 32.0f);
			mMinimapWidget->MinimapImage->SetBrush(MinimapBrush);

			mMinimapWidget->AddToViewport();


			// Set a timer: Update the minimap rotation based on the character's direction.
			FTimerHandle delayTimer{};

			GetWorld()->GetTimerManager().SetTimer(delayTimer, this,
			                                       &APlayerBase::RotateMinimapWithPlayer, 0.5f, false);
		}
	}


	// ==================== UI Setting ====================
	bIsMenuVisible = false;

	if (IsLocallyControlled())
	{
		//HP
		if (IsValid(mDisplayWidgetClass))
		{
			mDisplayWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), mDisplayWidgetClass);

			if (mDisplayWidget)
			{
				mDisplayWidget->AddToViewport(0);
			}
		}
		// Team HP
		if (IsValid(mTeamHPWidgetClass))
		{
			mTeamHPWidget = CreateWidget<UMainTeamUIWidget>(GetWorld(), mTeamHPWidgetClass);

			if (mTeamHPWidget)
			{
				mTeamHPWidget->AddToViewport();
				UpdateTeamPlayerUI();
			}
		}

		//CrossHair
		if (IsValid(mCrossHairClass))
		{
			mCrossHairWidget = CreateWidget<UUserWidget>(GetWorld(), mCrossHairClass);
		}
		if (IsValid(mRiffleZoomWidgetClass))
		{
			mRiffleZoomWidget = CreateWidget<UUserWidget>(GetWorld(), mRiffleZoomWidgetClass);
		}

		//Mode
		if (IsValid(mModeWidgetClass))
		{
			mModeWidget = CreateWidget<UChangeModeWidget>(GetWorld(), mModeWidgetClass);
		}

		//dash
		if (IsValid(mDashWidgetClass))
		{
			mDashWidget = CreateWidget<UDashCooltimeWidget2>(GetWorld(), mDashWidgetClass);
		}

		// esc
		if (IsValid(mEscWidgetClass))
		{
			mEscWidget = CreateWidget<UPauseWidget>(GetWorld(), mEscWidgetClass);
		}
		// moribund state PostProcessVolume
		if (nullptr == PostProcessVolume)
		{
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), FoundActors);
			FName DesiredVolumeName = TEXT("PlayerMoribundState");

			for (AActor* Actor : FoundActors)
			{
				PostProcessVolume = Cast<APostProcessVolume>(Actor);
				if (PostProcessVolume && PostProcessVolume->GetFName() == DesiredVolumeName)
				{
					PostProcessVolume->BlendWeight = 0.0f;
					break;
				}
			}

			if (nullptr == PostProcessVolume)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("NO PostProcessVolume")));
			}
		}
		else
		{
			PostProcessVolume->BlendWeight = 0.0f;
		}
	}
	/* This Code : Move to SetupPlayerInput() */
	// //Ready! 
	// if (HasAuthority())
	// {
	// 	SetReadyCurrentPlayer();
	// }
}

void APlayerBase::SetupPlayerInput()
{
	/* Controller Setting */
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
	case NM_Client:
		{
			/* Input Setting */
			if (PlayerController && PlayerController->IsLocalPlayerController())
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red,
				FString::Printf(TEXT("Called APlayerBase::SetupPlayerInput(), Is This Server? : %d"), HasAuthority()));
				FInputModeGameOnly InputMode;
				PlayerController->SetInputMode(InputMode);
				PlayerController->bShowMouseCursor = false;
				UEnhancedInputLocalPlayerSubsystem* Subsystem =
					ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
				const UDefaultInputData* InputData = GetDefault<UDefaultInputData>();
				if (Subsystem)
				{
					Subsystem->AddMappingContext(InputData->mDefaultContext, 0);
				}

				FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));
				if (CurrentLevelName != "Bridge_P")
				{
					PlayerController->GetPawn()->DisableInput(PlayerController);
				}
				else
				{
					PlayerController->GetPawn()->EnableInput(PlayerController);
				}
				PlayerController->SetAudioListenerAttenuationOverride(RootComponent, FVector::ZeroVector);
				
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red,
FString::Printf(TEXT("APlayerBase::SetupPlayerInput(), HasClientLoadedCurrentWorld? : %d"), PlayerController->HasClientLoadedCurrentWorld()));
				
				//Ready! 
				if (HasAuthority())
				{
					SetReadyCurrentPlayer();
				}
				else
				{
					SetReadyCurrentPlayerServer();
				}
			}
		}
	}
}

void APlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerBase, mNetPlayerCurHP);
	DOREPLIFETIME(APlayerBase, bNetWantsUnequip);

	// ==================== [ Components ] ====================
	DOREPLIFETIME(APlayerBase, mNetWeaponHand);
	DOREPLIFETIME(APlayerBase, mNetWeaponBack);
	DOREPLIFETIME(APlayerBase, mNetWeaponBackMesh);
	DOREPLIFETIME(APlayerBase, mNetWeaponHandChild);
	DOREPLIFETIME(APlayerBase, mMovement);

	// ==================== [ InputAction|Dash ] ====================
	DOREPLIFETIME(APlayerBase, mNetNewDashKey);
	DOREPLIFETIME(APlayerBase, bNetIsDashing);

	// ==================== [ InputAction|Sprint ] ====================
	DOREPLIFETIME(APlayerBase, bNetIsSprint);

	// ==================== [ InputAction|Drop ] ====================
	DOREPLIFETIME(APlayerBase, bNetWantsDrop);

	// ==================== [ Weapon ] ====================
	DOREPLIFETIME(APlayerBase, mNetWeaponSystem);
	DOREPLIFETIME(APlayerBase, mNetMasterWeapon);
	DOREPLIFETIME(APlayerBase, mNetWeaponInstance);

	DOREPLIFETIME(APlayerBase, bIsAMode)
	DOREPLIFETIME(APlayerBase, bNetIsEquip);
	DOREPLIFETIME(APlayerBase, bNetIsFire);
	DOREPLIFETIME(APlayerBase, bNetIsReload);

	// ==================== [ Status ] ====================
	DOREPLIFETIME(APlayerBase, bNetIsDead);
	DOREPLIFETIME(APlayerBase, bNetCanRebirth);
	DOREPLIFETIME(APlayerBase, mNetDeadOtherPlayer);
	DOREPLIFETIME(APlayerBase, mNetRebirthCount);

	// ========== [ Replication|To LyraPlayerAnimInstance ] ==========
	DOREPLIFETIME(APlayerBase, mNetDirectionAngle);

	// ==================== [ UpdateLandState ] ====================
	DOREPLIFETIME(APlayerBase, mNetLandState);

	// ==================== [ UpdateMoveState ] ====================
	DOREPLIFETIME(APlayerBase, mNetEMoveState);

	// ==================== [ UpdateAcceleration ] ====================
	DOREPLIFETIME(APlayerBase, bNetHasAcceleration);

	// ==================== [ RunningIntoWall ] ====================
	DOREPLIFETIME(APlayerBase, bNetIsWall);

	// ==================== [ UpdateCharacterState ] ====================
	DOREPLIFETIME(APlayerBase, bNetIsCrouch);
	DOREPLIFETIME(APlayerBase, mNetPitch);

	// ==================== [ UpdateVelocityAndGroundSpeed ] ====================
	DOREPLIFETIME(APlayerBase, mNetVelocity);
	DOREPLIFETIME(APlayerBase, mNetGroundSpeed);

	// ==================== [ UpdateCheckMoving ] ====================
	DOREPLIFETIME(APlayerBase, bNetIsShouldMove);

	// ==================== [ UpdateFalling ] ====================
	DOREPLIFETIME(APlayerBase, bNetIsFalling);

	// ==================== [ UpdateDirection ] ====================
	DOREPLIFETIME(APlayerBase, mNetDirection);

	// ==================== [ UpdateOrientation ] ====================
	DOREPLIFETIME(APlayerBase, mNetOrientationAngle);

	// ==================== [ TurnInPlace ] ====================
	DOREPLIFETIME(APlayerBase, mNetRootYawOffset);

	// ==================== [ Item ] ====================
	DOREPLIFETIME(APlayerBase, mEOverlapItem);
	DOREPLIFETIME(APlayerBase, mNetOverlappingItem);
	DOREPLIFETIME(APlayerBase, mEWeaponItemType);

	// ============================= [PlayerName] ================
	DOREPLIFETIME(APlayerBase, mPlayerName);
}

void APlayerBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// if (Montage == mFowardDashMontage
	//   || Montage == mLeftDashMontage
	//   || Montage == mBackwardDashMontage
	//   || Montage == mRightDashMontage)
	// {
	// 	if (HasAuthority())
	// 	{
	// 		bNetIsDashing = false;
	// 	}
	// 	return;
	// }

	if (Montage == mWeaponUnequipMontage
		&& true == bNetWantsUnequip)
	{
		if (HasAuthority())
		{
			bNetWantsUnequip = false;
		}

		/* Drop */
		if (bNetWantsDrop)
		{
			if (HasAuthority())
			{
				bNetWantsDrop = false;
			}
			SetWeaponVisibility(false, false);
		}
		/* Unequip */
		else
		{
			SetWeaponVisibility(true, false);
		}
	}

	if (mNetWeaponInstance)
	{
		if (Montage == mNetWeaponInstance->GetPlayerReloadMontage()
			&& true == bNetIsReload)
		{
			if (HasAuthority())
			{
				bNetIsReload = false;
			}
		}
		return;
	}

	if (Montage == mDeathMontage)
	{
		// TODO : 사망애니메이션 종료 위치랑 플레이어 로케이션 맞추기
		// SetActorLocation(GetMesh()->GetComponentLocation());
		return;
	}
}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputCom = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputCom)
	{
		const UDefaultInputData* InputData = GetDefault<UDefaultInputData>();
		EnhancedInputCom->BindAction(InputData->mMove, ETriggerEvent::Triggered, this, &APlayerBase::MoveAction);
		EnhancedInputCom->BindAction(InputData->mJump, ETriggerEvent::Triggered, this, &APlayerBase::JumpAction);
		EnhancedInputCom->BindAction(InputData->mCrouch, ETriggerEvent::Started, this, &APlayerBase::CrouchAction);
		EnhancedInputCom->BindAction(InputData->mCrouch, ETriggerEvent::Completed, this, &APlayerBase::UnCrouchAction);
		EnhancedInputCom->BindAction(InputData->mDash, ETriggerEvent::Started, this, &APlayerBase::DashAction);
		EnhancedInputCom->BindAction(InputData->mLookMouse, ETriggerEvent::Triggered, this,
		                             &APlayerBase::LookMouseAction);
		EnhancedInputCom->BindAction(InputData->mMenu, ETriggerEvent::Started, this, &APlayerBase::MenuAction);
		EnhancedInputCom->BindAction(InputData->mSprint, ETriggerEvent::Started, this, &APlayerBase::SprintAction);
		EnhancedInputCom->BindAction(InputData->mSprint, ETriggerEvent::Completed, this, &APlayerBase::NotSprintAction);

		EnhancedInputCom->BindAction(InputData->mFire, ETriggerEvent::Started, this, &APlayerBase::FireAction);
		EnhancedInputCom->BindAction(InputData->mFire, ETriggerEvent::Completed, this, &APlayerBase::FireEndAction);
		EnhancedInputCom->BindAction(InputData->mReload, ETriggerEvent::Started, this, &APlayerBase::ReloadAction);
		EnhancedInputCom->BindAction(InputData->mEquipWeapon, ETriggerEvent::Started, this,
		                             &APlayerBase::EquipWeaponAction);
		EnhancedInputCom->BindAction(InputData->mZoom, ETriggerEvent::Triggered, this, &APlayerBase::ZoomStartAction);
		EnhancedInputCom->BindAction(InputData->mZoom, ETriggerEvent::Completed, this, &APlayerBase::ZoomEndAction);
		EnhancedInputCom->BindAction(InputData->mSwapAB, ETriggerEvent::Started, this, &APlayerBase::SwapABModeAction);
		EnhancedInputCom->BindAction(InputData->mInteraction, ETriggerEvent::Started, this,
		                             &APlayerBase::InteractionAction);
		EnhancedInputCom->BindAction(InputData->mDrop, ETriggerEvent::Completed, this, &APlayerBase::DropAction);
	}
}

bool APlayerBase::CanAction() const
{
	if (bNetIsDead || bNetIsReload || bNetIsFire || bNetWantsUnequip || bNetWantsDrop)
	{
		return false;
	}

	if (nullptr == mMovement)
	{
		return false;
	}

	if (nullptr == mLyraAnimInstance)
	{
		return false;
	}

	return true;
}

void APlayerBase::MoveAction(const FInputActionValue& Value)
{
	if (!bNetIsDead)
	{
		FVector2D Axis = Value.Get<FVector2D>();
		mDashAxis = Axis;;
		AddMovementInput(GetActorForwardVector(), Axis.Y);
	
		AddMovementInput(GetActorRightVector(), Axis.X);
	}
}

void APlayerBase::JumpAction(const FInputActionValue& Value)
{
	if (true == CanJump()
		&& true == CanAction())
	{
	
		Super::Jump();
	}
}

void APlayerBase::CrouchAction(const FInputActionValue& Value)
{
	if (false == CanAction())
	{
		return;
	}

	if (mMovement->IsFalling())
	{
		return;
	}
	
	if (mNetWeaponInstance)
	{
		if (mNetMasterWeapon == mMachineGunClass
			&& false == bIsAMode && true == bNetIsFire)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
			                                 FString::Printf(TEXT(
				                                 "CrouchAction() : 'MachineGun, BMode' -> Crouch is IMPOSSIBLE")));
			return;
		}
	}

	if (HasAuthority())
	{
		bNetIsCrouch = true;
		CrouchMulticast();
	}
	else
	{
		CrouchServer(true);
	}
}

void APlayerBase::UnCrouchAction(const FInputActionValue& Value)
{
	if (false == CanAction())
	{
		return;
	}

	if (mMovement->IsFalling())
	{
		return;
	}
	
	if (HasAuthority())
	{
		bNetIsCrouch = false;
		CrouchMulticast();
	}
	else
	{
		CrouchServer(false);
	}
}

void APlayerBase::CrouchServer_Implementation(bool _bNetIsCrouch)
{
	bNetIsCrouch = _bNetIsCrouch;
	CrouchMulticast_Implementation();
}

void APlayerBase::CrouchMulticast_Implementation()
{
	if (true == bNetIsCrouch)
	{
		Super::Crouch();
	}
	else
	{
		Super::UnCrouch();
	}
}

void APlayerBase::DashAction(const FInputActionValue& Value)
{
	if (false == CanDash())
	{
		return;
	}

	mCurrentTapDashTime = GetWorld()->GetTimeSeconds();

	if (1.0 == mDashAxis.X)
	{
		mNetNewDashKey = EDashKey::D;
	}
	else if (-1.0 == mDashAxis.X)
	{
		mNetNewDashKey = EDashKey::A;
	}
	else if (1.0 == mDashAxis.Y)
	{
		mNetNewDashKey = EDashKey::W;
	}
	else if (-1.0 == mDashAxis.Y)
	{
		mNetNewDashKey = EDashKey::S;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("DashAction() : No Axis Value")));
		return;
	}

	if (EDashKey::None == mNetNewDashKey)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT("DashAction() : mNewDashKey is NONE")));
		return;
	}

	/* Initialization )
	Dash O -> | OldKey = None
	Dash X -> | OldKey = NewKey */

	// Dash O )
	if (mTapDashLimitTerm >= (mCurrentTapDashTime - mLastTapDashTime)
		&& mOldDashKey == mNetNewDashKey)
	{
		mOldDashKey = EDashKey::None;
		if (true == CanDash())
		{
			mRemainDashCoolTime = mDashCoolTime;
			bNetIsDashing = true;
			GetWorldTimerManager().SetTimer(DashTimerHandle, this, &APlayerBase::TimerDash, 0.1f, true);
			AddDashUI();
			PlayDash(mNetNewDashKey);
		}
	}
	// Dash X )
	else
	{
		mOldDashKey = mNetNewDashKey;
	}

	mLastTapDashTime = mCurrentTapDashTime;
}

void APlayerBase::DashServer_Implementation(EDashKey _mNewDashKey)
{
	if (bNetIsFire)
	{
		mNetWeaponInstance->FireStop();
	}
	DashMulticast(_mNewDashKey);
}

void APlayerBase::DashMulticast_Implementation(EDashKey _mNewDashKey)
{
	if (HasAuthority())
	{
		// TODO : It must be replaced with something other than 'LaunchCharacter()'.
		switch (_mNewDashKey)
		{
		case EDashKey::W:
			{
				LaunchCharacter(GetActorForwardVector() * mDashSpeed, false, false);
				break;
			}
		case EDashKey::A:
			{
				LaunchCharacter(GetActorRightVector() * -mDashSpeed, false, false);
				break;
			}
		case EDashKey::S:
			{
				LaunchCharacter(GetActorForwardVector() * -mDashSpeed, false, false);
				break;
			}
		case EDashKey::D:
			{
				LaunchCharacter(GetActorRightVector() * mDashSpeed, false, false);
				break;
			}
		default:
			break;
		}
	}

	switch (_mNewDashKey)
	{
	case EDashKey::W:
		{
			mLyraAnimInstance->Montage_Play(mFowardDashMontage);
			break;
		}
	case EDashKey::A:
		{
			mLyraAnimInstance->Montage_Play(mLeftDashMontage);
			break;
		}
	case EDashKey::S:
		{
			mLyraAnimInstance->Montage_Play(mBackwardDashMontage);
			break;
		}
	case EDashKey::D:
		{
			mLyraAnimInstance->Montage_Play(mRightDashMontage);
			break;
		}
	}
}

bool APlayerBase::CanDash() const
{
	if (false == CanAction())
	{
		return false;
	}

	if (mMovement->IsFalling())
	{
		return false;
	}
	
	if (true == bNetIsDashing)
	{
		return false;
	}

	if (!IsValid(mFowardDashMontage) || !IsValid(mLeftDashMontage) || !IsValid(mBackwardDashMontage) || !IsValid(
		mRightDashMontage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("CanDash() But No DashMontage")));
		return false;
	}

	if (mNetWeaponInstance)
	{
		if (mNetMasterWeapon == mMachineGunClass
			&& false == bIsAMode && true == bNetIsFire)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
			                                 FString::Printf(
				                                 TEXT("CanDash() : 'MachineGun, BMode' -> Dash is IMPOSSIBLE")));
			return false;
		}
	}

	return true;
}

void APlayerBase::PlayDash(EDashKey _mNewDashKey)
{
	if (HasAuthority())
	{
		if (bNetIsFire)
		{
			mNetWeaponInstance->FireStop();
		}
		DashMulticast(_mNewDashKey);
	}
	else
	{
		DashServer(_mNewDashKey);
	}
}

void APlayerBase::TimerDash()
{
	// Start Counting ) Dash CoolTime
	if (0.0 < (mRemainDashCoolTime - 0.1f))
	{
		mRemainDashCoolTime -= 0.1;
		if (IsValid(mDashWidget))
		{
			mDashWidget->SetDash(mRemainDashCoolTime, mDashCoolTime);
		}
	}
	else
	{
		bNetIsDashing = false;
		mRemainDashCoolTime = 0.0;
		GetWorldTimerManager().ClearTimer(DashTimerHandle);
		DeleteDashUI();
	}
}

// = Mouse Rotation
void APlayerBase::LookMouseAction(const FInputActionValue& Value)
{
	FVector Axis = Value.Get<FVector>();
	mServerTurnRate = FMath::Clamp(Axis.X, -1.0, 1.0);
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void APlayerBase::MenuAction(const FInputActionValue& Value)
{
	//if Option widget open, first Delete! 
	if (true == bIsMenuVisible && true == mEscWidget->GetIsOptionWidget())
	{
		mEscWidget->SetIsOptionWidget(false);
		mEscWidget->GetOptionWidget()->RemoveFromParent();
		return;
	}
	else
	{
		if (bIsMenuVisible == true)
		{
			if (true == mEscWidget->IsInViewport())
			{
				///Esc Widget -> Not Option
				mEscWidget->RemoveFromParent();
				bIsMenuVisible = false;
				FInputModeGameOnly InputMode;
				Cast<APlayerController>(GetController())->SetInputMode(InputMode);
				Cast<APlayerController>(GetController())->bShowMouseCursor = false;
			}
		}
		else
		{
			if (IsValid(mEscWidgetClass))
			{
				if (mEscWidget)
				{
					mEscWidget->AddToViewport();
					bIsMenuVisible = true;
					FInputModeGameAndUI InputMode;
					Cast<APlayerController>(GetController())->SetInputMode(InputMode);
					Cast<APlayerController>(GetController())->bShowMouseCursor = true;
				}
			}
		}
	}
}

void APlayerBase::SprintAction(const FInputActionValue& Value)
{
	if (bNetIsFire)
	{
		return;
	}
	/* Client */
	if (!HasAuthority())
	{
		SprintServer(true);
	}
	/* Server + Client(Local Player) */
	bNetIsSprint = true;
	mMovement->MaxWalkSpeed = mSprintSpeed;
}

void APlayerBase::NotSprintAction(const FInputActionValue& Value)
{
	/* Client */
	if (!HasAuthority())
	{
		SprintServer(false);
	}

	/* Server + Client(Local Player) */
	bNetIsSprint = false;
	mMovement->MaxWalkSpeed = mWalkSpeed;
}

void APlayerBase::SprintServer_Implementation(bool _bNetIsSprint)
{
	bNetIsSprint = _bNetIsSprint;
	OnRep_bNetIsSprint();
}

void APlayerBase::OnRep_bNetIsSprint() const
{
	/* Sprint Speed      : 600.f */
	if (true == bNetIsSprint)
	{
		mMovement->MaxWalkSpeed = mSprintSpeed;
	}
	/* Not Sprint Speed   : 300.f */
	else
	{
		mMovement->MaxWalkSpeed = mWalkSpeed;
	}
}

void APlayerBase::FireAction(const FInputActionValue& Value)
{
	if (false == CanFire())
	{
		return;
	}

	if (nullptr != mLyraAnimInstance->GetCurrentActiveMontage())
	{
		return;
	}

	if (!mNetWeaponInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT("!! FireAction : No mNetWeaponInstance !!")));
		return;
	}

	if (HasAuthority())
	{
		bNetIsFire = true;
		mNetWeaponInstance->FireStart();
	}
	else
	{
		FireActionServer(true);
	}
}

void APlayerBase::FireEndAction(const FInputActionValue& Value)
{
	if (!mNetWeaponInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT("!! FireEndAction : No mNetWeaponInstance !!")));
		return;
	}

	if (HasAuthority())
	{
		bNetIsFire = false;
		mNetWeaponInstance->FireStop();
	}
	else
	{
		FireActionServer(false);
	}
}

void APlayerBase::FireActionServer_Implementation(bool IsFire)
{
	bNetIsFire = IsFire;

	if (bNetIsFire)
	{
		mNetWeaponInstance->FireStart();
	}
	else
	{
		mNetWeaponInstance->FireStop();
	}
}

bool APlayerBase::CanFire()
{
	if (false == CanAction())
	{
		return false;
	}

	if (false == bNetIsEquip)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("!! IsEquip is FALSE !!")));
		return false;
	}

	if (bNetIsSprint)
	{
		return false;
	}

	return true;
}

void APlayerBase::ReloadAction(const FInputActionValue& Value)
{
	if (!bNetIsEquip)
	{
		return;
	}

	if (nullptr != mLyraAnimInstance->GetCurrentActiveMontage())
	{
		return;
	}

	if (!mNetWeaponInstance || !mNetWeaponHandChild->GetChildActor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("!! APlayerBase::ReloadAction : No mNetWeaponInstance !!")));
		return;
	}

	if (false == CanAction())
	{
		return;
	}

	if (mMovement->IsFalling())
	{
		return;
	}

	if (HasAuthority())
	{
		if (true == mNetWeaponInstance->CanReload())
		{
			bNetIsReload = true;
			mNetWeaponInstance->Reload();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
			                                 FString::Printf(TEXT("!! APlayerBase::ReloadAction : Ammo Is FULL !!")));
		}
	}
	else
	{
		ReloadActionServer();
	}
}

void APlayerBase::ReloadActionServer_Implementation()
{
	if (!mNetWeaponInstance || !mNetWeaponHandChild->GetChildActor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("!! APlayerBase::ReloadAction : No mNetWeaponInstance !!")));
		return;
	}

	if (true == mNetWeaponInstance->CanReload())
	{
		bNetIsReload = true;
		mNetWeaponInstance->Reload();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT("!! APlayerBase::ReloadActionServer : Ammo Is FULL !!")));
	}
}

void APlayerBase::EquipWeaponAction(const FInputActionValue& Value)
{
	if (false == CanAction())
	{
		return;
	}

	if (mMovement->IsFalling())
	{
		return;
	}
	
	if (!mNetWeaponInstance || !mNetWeaponHandChild->GetChildActor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT("NO mNetWeaponInstance OR No ChildActor")));
		return;
	}

	if (!IsValid(mWeaponEquipMontage) && !IsValid(mWeaponUnequipMontage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT("NO mWeaponEquipMontage OR mWeaponUnequipMontage")));
		return;
	}

	if (HasAuthority())
	{
		EquipOrUnequip();
	}
	else
	{
		EquipOrUnequipServer();
	}
}


void APlayerBase::EquipOrUnequip()
{
	/* ONLY SERVER */
	if (!HasAuthority())
	{
		return;
	}

	// (Client)->OnRep_bNetIsEquip()
	/* Unequip -> Equip */
	if (false == bNetIsEquip)
	{
		bNetIsEquip = true;
	}
	/* Equip -> Unequip */
	else
	{
		bNetIsEquip = false;
		bNetWantsUnequip = true;
	}

	OnRep_bNetIsEquip();

	PlayEquipAnimMulticast();
}

void APlayerBase::EquipOrUnequipServer_Implementation()
{
	EquipOrUnequip();
}

void APlayerBase::PlayEquipAnimServer_Implementation()
{
	PlayEquipAnimMulticast();
}

void APlayerBase::PlayEquipAnimMulticast_Implementation()
{
	// Equip
	if (true == bNetIsEquip)
	{
		if (EItemType::None != mEWeaponItemType)
		{
			TSubclassOf<ULyraPlayerAnimInstance> WeaponAnimClass = mWeaponAnimClassMap.FindRef(mEWeaponItemType);
			if (WeaponAnimClass)
			{
				ChangeAnimInstanceClass(WeaponAnimClass);
			}
			SetWeaponVisibility(false, true);
		}

		mLyraAnimInstance->Montage_Play(mWeaponEquipMontage);
	}
	// UnEquip
	else
	{
		ChangeAnimInstanceClass(mUnArmedAnimClass);
		mLyraAnimInstance->Montage_Play(mWeaponUnequipMontage);
	}
}

void APlayerBase::OnRep_bNetIsEquip()
{
	// // UI
	if (IsLocallyControlled())
	{
		WeaponUIUpdate(bNetIsEquip);
	}
}

void APlayerBase::ZoomStartAction(const FInputActionValue& Value)
{
	if (IsLocallyControlled())
	{
		if (false == bClientIsAim)
		{
			bClientIsAim = true;

			GetWorldTimerManager().SetTimer(CameraZoomTimerHandle, this, &APlayerBase::TimerZoom, 0.02f, true);

			// ( UI Crosshair ZOOM IN )
			if (mCrossHairWidget)
			{
				if (true == mCrossHairWidget->IsInViewport())
				{
					mCrossHairWidget->RemoveFromParent();
				}
			}
			if (mRiffleZoomWidget)
			{
				if (false == mRiffleZoomWidget->IsInViewport())
				{
					mRiffleZoomWidget->AddToViewport();
				}
			}
		}
	}
}

void APlayerBase::ZoomEndAction(const FInputActionValue& Value)
{
	if (IsLocallyControlled())
	{
		if (true == bClientIsAim)
		{
			bClientIsAim = false;

			GetWorldTimerManager().ClearTimer(CameraZoomTimerHandle);
			GetWorldTimerManager().SetTimer(CameraZoomTimerHandle, this, &APlayerBase::TimerZoom, 0.02f, true);

			// ( UI Crosshair ZOOM OUT )
			if (mRiffleZoomWidget)
			{
				if (true == mRiffleZoomWidget->IsInViewport())
				{
					mRiffleZoomWidget->RemoveFromParent();
				}
			}
			if (mCrossHairWidget)
			{
				if (false == mCrossHairWidget->IsInViewport())
				{
					mCrossHairWidget->AddToViewport();
				}
			}
		}
	}
}

void APlayerBase::TimerZoom()
{
	if (IsLocallyControlled())
	{
		float NewTargetArmLength = bClientIsAim ? mClientAimedArmLength : mClientDefaultArmLength;

		mArm->TargetArmLength = FMath::FInterpTo(mArm->TargetArmLength, NewTargetArmLength,
		                                         GetWorld()->GetDeltaSeconds(), mClientZoomSpeed);

		if (!bClientIsAim && FMath::IsNearlyEqual(mArm->TargetArmLength, mClientDefaultArmLength, 0.1f))
		{
			GetWorldTimerManager().ClearTimer(CameraZoomTimerHandle);
		}
	}
}

void APlayerBase::SwapABModeAction(const FInputActionValue& Value)
{
	if (bNetIsEquip)
	{
		if (bIsAMode)
		{
			bIsAMode = false;
			GetWeaponInstance()->ChangeABMode(false);
		}
		else
		{
			bIsAMode = true;
			GetWeaponInstance()->ChangeABMode(true);
		}

		if (IsValid(mModeWidget))
		{
			mModeWidget->ChangeMode();
		}
	}
}

void APlayerBase::InteractionAction(const FInputActionValue& Value)
{
	if (mNetDeadOtherPlayer)
	{
		RebirthUIUnView();
		if (HasAuthority())
		{
			mNetDeadOtherPlayer->Rebirth(this);
		}
		else
		{
			InteractionServer();
		}
	}
	else if (mNetOverlappingItem)
	{
		if (HasAuthority())
		{
			if (mNetOverlappingItem->GetbCanPlayerUse())
			{
				UsePlayerItem();
			}
		}
		else
		{
			InteractionServer();
		}
	}
}

void APlayerBase::InteractionServer_Implementation()
{
	if (mNetDeadOtherPlayer)
	{
		mNetDeadOtherPlayer->Rebirth(this);
	}
	else if (mNetOverlappingItem)
	{
		if (mNetOverlappingItem->GetbCanPlayerUse())
		{
			UsePlayerItem();
		}
	}
}

void APlayerBase::DropAction(const FInputActionValue& Value)
{
	if (!mNetWeaponInstance || !mNetWeaponHandChild->GetChildActor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT(
			                                 "!! APlayerBase::DropAction : No mNetWeaponInstance OR No ChildActor!!")));
		return;
	}

	if (false == CanAction())
	{
		return;
	}

	if (mMovement->IsFalling())
	{
		return;
	}
	
	if (HasAuthority())
	{
		Drop(mNetWeaponInstance->GetWeaponDetails());
	}
	else
	{
		DropServer(mNetWeaponInstance->GetWeaponDetails());
	}
}


void APlayerBase::Drop(const FWeaponDetails& WeaponAmmo)
{
	/* ONLY SERVER */
	if (!HasAuthority())
	{
		return;
	}

	if (bNetIsEquip)
	{
		bNetWantsDrop = true;
		EquipOrUnequip();
	}

	DropWeapon(WeaponAmmo);
	DestroyWeapon();
	SetWeaponVisibility(false, false);
}

void APlayerBase::DropWeapon(const FWeaponDetails& WeaponAmmo)
{
	AItemBase* ItemClass = NewObject<AItemBase>(this);
	FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100.f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	if (EItemType::None != mEWeaponItemType)
	{
		ItemClass->SpawnItem(SpawnLocation, SpawnRotation, mEWeaponItemType, WeaponAmmo);
	}
}

void APlayerBase::DropServer_Implementation(const FWeaponDetails& WeaponAmmo)
{
	if (IsValid(mNetWeaponHandChild->GetChildActor()))
	{
		Drop(WeaponAmmo);
	}
}

// ==================== [ Weapon ] ====================
void APlayerBase::SetMasterWeapon(EItemType WeaponType)
{
	if (HasAuthority())
	{
		mEWeaponItemType = WeaponType;
		if (EItemType::None == mEWeaponItemType)
		{
			mNetMasterWeapon = nullptr;
		}
		else
		{
			TSubclassOf<APlayerMasterWeapon> WeaponClass = mWeaponClassMap.FindRef(mEWeaponItemType);
			if (WeaponClass)
			{
				mNetMasterWeapon = WeaponClass;
			}
		}
	}
	else
	{
		if (IsLocallyControlled())
		{
			SetMasterWeaponServer(WeaponType);
		}
	}
}

void APlayerBase::SetMasterWeaponServer_Implementation(EItemType WeaponType)
{
	mEWeaponItemType = WeaponType;
	if (EItemType::None == mEWeaponItemType)
	{
		mNetMasterWeapon = nullptr;
	}
	else
	{
		TSubclassOf<APlayerMasterWeapon> WeaponClass = mWeaponClassMap.FindRef(mEWeaponItemType);
		if (WeaponClass)
		{
			mNetMasterWeapon = WeaponClass;
		}
	}
}


void APlayerBase::CreateWeapon()
{
	// Server
	if (HasAuthority())
	{
		ExecuteCreateWeaponLogic();
	}
	// Client
	else
	{
		if (IsLocallyControlled())
		{
			CreateWeaponServer();
		}
	}
}

void APlayerBase::CreateWeaponServer_Implementation()
{
	ExecuteCreateWeaponLogic();
}

void APlayerBase::ExecuteCreateWeaponLogic()
{
	/* ONLY SERVER */
	if (!HasAuthority())
	{
		return;
	}

	if (!mNetMasterWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(TEXT("APlayerBase::CreateWeapon : No mNetMasterWeapon")));
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

	// 1. Check : No ChildActor
	if (nullptr == mNetWeaponHandChild->GetChildActor())
	{
		// 2. Create : ChildActor
		mNetWeaponHandChild->SetChildActorClass(mNetMasterWeapon);
		if (mNetWeaponHandChild->GetChildActor())
		{
			// 3. Assignment : Weapon Object To 'mNetWeaponInstance'
			mNetWeaponInstance = Cast<APlayerMasterWeapon>(mNetWeaponHandChild->GetChildActor());
			if (mNetWeaponInstance)
			{
				// 4. Create : Visual WeaponMesh
				// 5. Attach : Visual WeaponMesh To 'mNetWeaponBack'
				mNetWeaponBackMesh->SetSkeletalMesh(mNetWeaponInstance->GetWeaponMesh());
				if (mNetWeaponBackMesh->GetSkeletalMeshAsset())
				{
					mNetWeaponBackMesh->AttachToComponent(mNetWeaponBack, AttachmentRules);
					if (mRPGClass == mNetMasterWeapon)
					{
						mNetWeaponBackMesh->SetRelativeScale3D({2.5, 2.5, 2.5});
					}
					else
					{
						mNetWeaponBackMesh->SetRelativeScale3D({1.0, 1.0, 1.0});
					}
				}
				// 6. Assignment : Owner of 'mNetWeaponInstance'
				// mNetWeaponInstance->SetOwningPlayer(this);
				mNetWeaponInstance->SetOwningPlayer(mNetWeaponSystem->GetOwningPlayer());
			}
		}
	}
}

void APlayerBase::SetWeaponVisibility(bool IsBackVisible, bool IsHandVisible)
{
	if (HasAuthority())
	{
		SetWeaponVisibilityMulticast(IsBackVisible, IsHandVisible);
	}
	else
	{
		if (IsLocallyControlled())
		{
			SetWeaponVisibilityServer(IsBackVisible, IsHandVisible);
		}
	}
}

void APlayerBase::SetWeaponVisibilityServer_Implementation(bool IsBackVisible, bool IsHandVisible)
{
	SetWeaponVisibilityMulticast(IsBackVisible, IsHandVisible);
}

void APlayerBase::SetWeaponVisibilityMulticast_Implementation(bool IsBackVisible, bool IsHandVisible)
{
	mNetWeaponBack->SetVisibility(IsBackVisible, true);
	mNetWeaponHand->SetVisibility(IsHandVisible, true);
}

void APlayerBase::DestroyWeapon()
{
	mNetWeaponHandChild->DestroyChildActor();
	mNetWeaponInstance->SetOwningPlayer(nullptr);
	// mNetWeaponInstance = nullptr;
	SetMasterWeapon(EItemType::None);
}

void APlayerBase::OnRep_mNetWeaponInstance()
{
	if (mNetWeaponInstance)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		mNetWeaponBackMesh->SetSkeletalMesh(mNetWeaponInstance->GetWeaponMesh());
		if (mNetWeaponBackMesh->GetSkeletalMeshAsset())
		{
			mNetWeaponBackMesh->AttachToComponent(mNetWeaponBack, AttachmentRules);
			if (mRPGClass == mNetMasterWeapon)
			{
				mNetWeaponBackMesh->SetRelativeScale3D({2.5, 2.5, 2.5});
			}
			else
			{
				mNetWeaponBackMesh->SetRelativeScale3D({1.0, 1.0, 1.0});
			}
		}
		mNetWeaponInstance->SetOwningPlayer(mNetWeaponSystem->GetOwningPlayer());
	}
}

// ==================== [ Status ] ====================
void APlayerBase::SetPlayerCurHP(float ApplyHP)
{
	if (HasAuthority())
	{
		/* HP Potion */
		if (0 <= ApplyHP)
		{
			if (100.f > mNetPlayerCurHP + ApplyHP)
			{
				mNetPlayerCurHP += ApplyHP;
			}
			/* Full HP */
			else
			{
				mNetPlayerCurHP = 100.f;
			}
		}
		/* Damage */
		else
		{
			if (0.f < mNetPlayerCurHP + ApplyHP)
			{
				mNetPlayerCurHP += ApplyHP;
			}
			/* Dead */
			else
			{
				mNetPlayerCurHP = 0.f;
				Die();
			}
		}
	}

	if (IsLocallyControlled())
	{
		mDisplayWidget->SetHPPercent(mNetPlayerCurHP);
		AMainPlayerState* CurrentPlayerState = Cast<AMainPlayerState>(GetPlayerState());
		if (IsValid(CurrentPlayerState))
		{
			CurrentPlayerState->SetPlayerHP(mNetPlayerCurHP);
			UpdateTeamPlayerUI();
		}


		if (IsValid(PostProcessVolume))
		{
			if (mNetPlayerCurHP <= 30.f)
			{
				PostProcessVolume->BlendWeight = 1.0f;
			}
			else
			{
				PostProcessVolume->BlendWeight = 0.0f;
			}
		}
	}
}

float APlayerBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                              AActor* DamageCauser)
{
	/* ONLY SERVER */
	if (!HasAuthority())
	{
		return 0.f;
	}
	
	if (HasAuthority())
	{
		Damage = std::floor(Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser));
		if (10 <= Damage)
		{
			if (bNetIsFire)
			{
				mNetWeaponInstance->FireStop();
			}
		}
		
		if (0.f >= mNetPlayerCurHP)
		{
			return 0.f;
		}

		if (0.f < mNetPlayerCurHP - Damage)
		{
			if (10 <= Damage)
			{
				HitMulticast(Damage);
			}
			
			SetPlayerCurHP(-Damage);
		}
		else
		{
			/* Dead */
			SetPlayerCurHP(-100.f);
		}
	}
	return Damage;
}

void APlayerBase::HitMulticast_Implementation(float Damage)
{
	if (!IsValid(mHitMontageLight) || !IsValid(mHitMontageHeavy))
	{
		return;
	}

	if (10 > Damage)
	{
		mLyraAnimInstance->Montage_Play(mHitMontageLight);
	}
	else
	{
		mLyraAnimInstance->Montage_Play(mHitMontageHeavy);
	}
}

void APlayerBase::OnRep_mNetPlayerCurHP()
{
	/* Client */
	if (IsLocallyControlled())
	{
		mDisplayWidget->SetHPPercent(mNetPlayerCurHP);
		AMainPlayerState* CurrentPlayerState = Cast<AMainPlayerState>(GetPlayerState());
		if (IsValid(CurrentPlayerState))
		{
			CurrentPlayerState->SetPlayerHP(mNetPlayerCurHP);
			UpdateTeamPlayerUI();
		}

		if (IsValid(PostProcessVolume))
		{
			if (mNetPlayerCurHP <= 30.f)
			{
				PostProcessVolume->BlendWeight = 1.0f;
			}
			else
			{
				PostProcessVolume->BlendWeight = 0.0f;
			}
		}
		if (0.f == mNetPlayerCurHP)
		{
			Die();
		}
	}
}

void APlayerBase::ResetPlayerStatus()
{
	if (HasAuthority())
	{
		bNetIsDead = false;
		
		if (bNetIsSprint)
		{
			if (mMovement)
			{
				mMovement->MaxWalkSpeed = mWalkSpeed;
// 				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
// FString::Printf(TEXT("APlayerBase::ResetPlayerStatus() : init MaxWalkSpeed!!")));
			}
		}
		bNetIsSprint = false;

		if (bNetIsCrouch)
		{
			if (mMovement->IsCrouching())
			{
				mMovement->UnCrouch();
// 				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
// FString::Printf(TEXT("APlayerBase::ResetPlayerStatus() : UnCrouch!!")));
			}
		}
		bNetIsCrouch = false;
		
		if (bNetIsFire)
		{
			if (mNetWeaponInstance)
			{
				mNetWeaponInstance->FireStop();
// 				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
// FString::Printf(TEXT("APlayerBase::ResetPlayerStatus() : FireStop!!")));
			}
		}
		bNetIsFire = false;
		
		bNetIsReload = false;
		bNetIsDashing = false;
		bNetWantsUnequip = false;
		bNetWantsDrop = false;
	}
}

// ==================== [ Die/Rebirth ] ====================
void APlayerBase::SetDeadOtherPlayer(APlayerBase* DeadPlayer)
{
	if (HasAuthority())
	{
		if (!DeadPlayer)
		{
			mNetDeadOtherPlayer = nullptr;
			return;
		}
		mNetDeadOtherPlayer = DeadPlayer;
	}
	else
	{
		SetDeadOtherPlayerServer(DeadPlayer);
	}
}

void APlayerBase::SetDeadOtherPlayerServer_Implementation(APlayerBase* DeadPlayer)
{
	if (!DeadPlayer)
	{
		mNetDeadOtherPlayer = nullptr;
		return;
	}

	mNetDeadOtherPlayer = DeadPlayer;
}


void APlayerBase::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	if (!OverlappedComponent || !OtherActor)
	{
		return;
	}

	APlayerBase* DeadPlayer = Cast<APlayerBase>(OverlappedComponent->GetOwner());
	if (!DeadPlayer->GetIsDead())
	{
		return;
	}

	if (!DeadPlayer->CanRebirth())
	{
		return;
	}

	APlayerBase* TriggerPlayer = Cast<APlayerBase>(OtherActor);
	if (!TriggerPlayer || TriggerPlayer->GetIsDead())
	{
		return;
	}

	TriggerPlayer->SetDeadOtherPlayer(DeadPlayer);
	TriggerPlayer->RebirthUIViewClient();
}

void APlayerBase::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerBase* DeadPlayer = Cast<APlayerBase>(OverlappedComponent->GetOwner());

	if (!DeadPlayer->GetIsDead() || !DeadPlayer->CanRebirth())
	{
		return;
	}

	APlayerBase* TriggerPlayer = Cast<APlayerBase>(OtherActor);


	if (!TriggerPlayer || TriggerPlayer->GetIsDead())
	{
		return;
	}

	TriggerPlayer->SetDeadOtherPlayer(nullptr);
	TriggerPlayer->RebirthUIUnViewClient();
}

void APlayerBase::Die()
{
	if (HasAuthority())
	{
		ExecuteDieLogic();
		DieMulticast();
	}
	else
	{
		DieServer();
	}

	if (IsLocallyControlled())
	{
		//Death UI
		if (!IsValid(mDeathWidget))
		{
			mDeathWidget = CreateWidget<UPlayerDeathWidget>(GetWorld(), mDeathWidgetClass);
		}
		if (IsValid(mDeathWidget))
		{
			mDeathWidget->AddToViewport(100);
		}
	}
}

void APlayerBase::DieServer_Implementation()
{
	ExecuteDieLogic();
	DieMulticast();
}

void APlayerBase::DieMulticast_Implementation()
{
	bUseControllerRotationYaw = false;
	
	if (IsValid(mDeathParticle))
	{
		mDeathParticle->Activate();
		mDeathParticle->SetVisibility(true);
	}
	
	// if (mLyraAnimInstance && mDeathMontage)
	if (GetMesh())
	{
		if (GetMesh()->GetAnimInstance() && mDeathMontage)
		if (GetMesh()->GetAnimInstance()->GetCurrentActiveMontage())
		{
			GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
		}
		GetMesh()->GetAnimInstance()->Montage_Play(mDeathMontage);
	}
	
	if (HasAuthority())
	{
		if (mMovement)
		{
			mMovement->DisableMovement();
		}
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (PlayerController->InputComponent)
		{
			PlayerController->InputComponent->ClearBindingValues();
		}
		PlayerController->StopMovement();
		// PlayerController->GetPawn()->DisableInput(PlayerController);
	
		FInputModeGameAndUI InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
}

void APlayerBase::ExecuteDieLogic()
{
	// if (bNetIsFire)
	// {
	// 	mNetWeaponInstance->FireStop();
	// }
	//
	// if (mMovement->IsCrouching())
	// {
	// 	mMovement->UnCrouch();
	// }
	
	if (0 == mNetRebirthCount)
	{
		bNetCanRebirth = true;
	}

	ResetPlayerStatus();
	bNetIsDead = true;
	
	// Only 'Landscape' Collision O
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Custom"));
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
	mRebirthBox->SetCollisionProfileName(UE_COLLISION_PROFILE_EVENTTRIGGER);
	mRebirthBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APlayerBase::Rebirth(APlayerBase* Reviver)
{
	if (HasAuthority())
	{
		ExecuteRebirthLogic(Reviver);
		RebirthMulticast();
	}
	else
	{
		RebirthServer(Reviver);
	}
}

void APlayerBase::RebirthServer_Implementation(APlayerBase* Reviver)
{
	ExecuteRebirthLogic(Reviver);
	RebirthMulticast();
}

void APlayerBase::RebirthMulticast_Implementation()
{
	bUseControllerRotationYaw = true;

	if (IsValid(mDeathParticle))
	{
		mDeathParticle->Deactivate();
		mDeathParticle->SetVisibility(false);
	}
	
	//Death UI DELETE!
	if (IsLocallyControlled())
	{
		if (IsValid(mDeathWidget))
		{
			mDeathWidget->RemoveFromParent();
		}
	}
	
	if (HasAuthority())
	{
		if (mMovement)
		{
			mMovement->SetMovementMode(MOVE_Walking);
		}
	}
		
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		// PlayerController->GetPawn()->EnableInput(PlayerController);
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}
	
	if (mLyraAnimInstance && mRebirthMontage)
	{
		mLyraAnimInstance->Montage_Play(mRebirthMontage);
	}
}

void APlayerBase::ExecuteRebirthLogic(APlayerBase* Reviver)
{
	Reviver->SetDeadOtherPlayer(nullptr);

	mNetRebirthCount++;
	bNetIsDead = false;
	bNetCanRebirth = false;
	SetPlayerCurHP(100.f);

	// Revert collision settings to Default
	mRebirthBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName(UE_COLLISION_PROFILE_PLAYER);
}

// ========== [ Replication|To LyraPlayerAnimInstance ] ==========
void APlayerBase::UpdatePlayerStates()
{
	if (HasAuthority())
	{
		UpdateLandState();
		UpdateTurnRate();
		UpdateAcceleration();
		UpdateWallDetection();
		RunningIntoWall();

		UpdateCharacterState();
		UpdateVelocityAndGroundSpeed();
		UpdateCheckMoving();
		UpdateFalling();
		UpdateDirection();
		UpdateMoveState();
		UpdateOrientation();
		UpdateTurnInPlace();
	}
}

void APlayerBase::UpdateLandState()
{
	mServerLandDepth = abs(mMovement->Velocity.Z);

	if (300.0f <= mServerLandDepth && mServerLandDepth <= 900.0f)
	{
		mNetLandState = ELandState::Normal;
	}
	else if (1000.0f <= mServerLandDepth && mServerLandDepth <= 1200.0f)
	{
		mNetLandState = ELandState::Soft;
	}
	else if (1250.0f < mServerLandDepth)
	{
		mNetLandState = ELandState::Heavy;
	}
}

void APlayerBase::UpdateTurnRate()
{
	mServerTurnRate = FMath::FInterpTo(mNetDirectionAngle, mServerTurnRate, GetWorld()->GetDeltaSeconds(), 10.0);
}

void APlayerBase::UpdateAcceleration()
{
	double Sqared2D = mServerLocalAcceleration2D.SizeSquared2D();
	bool bIsMoving = UKismetMathLibrary::NearlyEqual_FloatFloat(Sqared2D, 0.0);
	bNetHasAcceleration = !bIsMoving;

	mServerWorldAcceleration2D = mMovement->GetCurrentAcceleration();
	mServerLocalAcceleration2D = UKismetMathLibrary::LessLess_VectorRotator(
		mServerWorldAcceleration2D, mServerWorldRotation);
	mServerWorldVelocity2D = mServerWorldVelocity * FVector(1.0, 1.0, 0.0);
	mServerLocalVelocity2D = UKismetMathLibrary::LessLess_VectorRotator(mServerWorldVelocity2D, mServerWorldRotation);

	mServerWorldRotation = GetActorRotation();
	mServerWorldVelocity = GetVelocity();
}

void APlayerBase::UpdateWallDetection()
{
	double DotValue = FVector::DotProduct(mServerLocalAcceleration2D.GetSafeNormal(),
	                                      mServerLocalVelocity2D.GetSafeNormal());

	if ((mServerLocalAcceleration2D.Size2D() > 0.1)
		&& (mServerLocalVelocity2D.Size2D() < 200.0)
		&& ((-0.6 <= DotValue) && (0.6 >= DotValue)))
	{
		bServerIsRunningWall = true;
	}
	else
	{
		bServerIsRunningWall = false;
	}
}

void APlayerBase::RunningIntoWall()
{
	if (bServerIsRunningWall)
	{
		bNetIsWall = true;
		mMovement->MaxWalkSpeed = 0.0f;
	}
	else
	{
		bNetIsWall = false;
		if (bNetIsSprint && mSprintSpeed != mMovement->MaxWalkSpeed)
		{
			OnRep_bNetIsSprint();
		}
		else if (!bNetIsSprint && mWalkSpeed != mMovement->MaxWalkSpeed)
		{
			OnRep_bNetIsSprint();
		}
	}
}

void APlayerBase::UpdateCharacterState()
{
	bServerIsJump = bPressedJump;
	mNetPitch = UKismetMathLibrary::NormalizedDeltaRotator(GetBaseAimRotation(), GetActorRotation()).Pitch;
}

void APlayerBase::UpdateVelocityAndGroundSpeed()
{
	mNetVelocity = mMovement->Velocity;
	mNetGroundSpeed = mNetVelocity.Size2D();
}

void APlayerBase::UpdateCheckMoving()
{
	mServerAcceleration = mMovement->GetCurrentAcceleration();
	bool bIsAcceleration = UKismetMathLibrary::NotEqual_VectorVector(mServerAcceleration, FVector(0.0, 0.0, 0.0), 0.0);
	bool bIsGroundspeed = (mNetGroundSpeed > 3.0);
	bNetIsShouldMove = (bIsAcceleration && bIsGroundspeed);
}

void APlayerBase::UpdateFalling()
{
	bNetIsFalling = mMovement->IsFalling();
}

void APlayerBase::UpdateDirection()
{
	FVector fDirection;
	fDirection.X = mNetVelocity.X;
	fDirection.Y = mNetVelocity.Y;
	fDirection.Z = 0.0;

	float fAngle = UKismetAnimationLibrary::CalculateDirection(fDirection, GetActorRotation());
	mNetDirection = FRotator::NormalizeAxis(fAngle);
}

void APlayerBase::UpdateMoveState()
{
	if (-70.0 <= mNetDirection && 70.0 >= mNetDirection)
	{
		mNetEMoveState = EMovementDirectionState::Forward;
	}
	else if (70.0 < mNetDirection && 110.0 >= mNetDirection)
	{
		mNetEMoveState = EMovementDirectionState::Right;
	}
	else if (-110.0 <= mNetDirection && -70.0 >= mNetDirection)
	{
		mNetEMoveState = EMovementDirectionState::Left;
	}
	else
	{
		mNetEMoveState = EMovementDirectionState::Backward;
	}
}

void APlayerBase::UpdateOrientation()
{
	mNetOrientationAngle.mNetFAngle = mNetDirection;
	mNetOrientationAngle.mNetRAngle = mNetDirection - 90.0;
	mNetOrientationAngle.mNetBAngle = mNetDirection - 180.0;
	mNetOrientationAngle.mNetLAngle = mNetDirection + 90.0;
}

void APlayerBase::UpdateTurnInPlace()
{
	if (bNetIsShouldMove || bNetIsFalling)
	{
		mNetRootYawOffset = FMath::FInterpTo(mNetRootYawOffset, 0.0, GetWorld()->GetDeltaSeconds(), 20.0);
		mServerMovingRotation = GetActorRotation();
		mServerLastMovingRotation = mServerMovingRotation;
	}
	else
	{
		mServerLastMovingRotation = mServerMovingRotation;
		mServerMovingRotation = GetActorRotation();
		mNetRootYawOffset -= UKismetMathLibrary::NormalizedDeltaRotator(
			mServerMovingRotation, mServerLastMovingRotation).Yaw;;
		if (IsValid(mLyraAnimInstance))
		{
			if (mLyraAnimInstance->GetCurveValue("IsTurn") > 0.0)
			{
				mServerLastDistanceCurve = mServerDistanceCurve;
				mServerDistanceCurve = mLyraAnimInstance->GetCurveValue("DistanceCurve");
				mServerDeltaDistanceCurve = mServerDistanceCurve - mServerLastDistanceCurve;


				if (mNetRootYawOffset > 0.0)
				{
					mNetRootYawOffset -= mServerDeltaDistanceCurve;
				}
				else
				{
					mNetRootYawOffset += mServerDeltaDistanceCurve;
				}

				mServerAbsRootYawOffset = FMath::Abs(mNetRootYawOffset);

				if (mServerAbsRootYawOffset > 45.0)
				{
					mServerYawExcess = mServerAbsRootYawOffset - 45.0;

					if (mNetRootYawOffset > 0.0)
					{
						mNetRootYawOffset -= mServerYawExcess;
					}
					else
					{
						mNetRootYawOffset += mServerYawExcess;
					}
				}
			}
		}
	}
}

// ==================== [ Item ] ====================
void APlayerBase::SetOverlapItem(EItemType ItemType, AItemBase* Item)
{
	if (EItemType::None != ItemType)
	{
		mEOverlapItem = ItemType;
	}

	if (Item)
	{
		mNetOverlappingItem = Item;
		Item->SetOwner(this);
	}
}

void APlayerBase::SetEndOverlappingItem()
{
	mEOverlapItem = EItemType::None;
	mNetOverlappingItem = nullptr;
}

/* Server */
void APlayerBase::UsePlayerItem()
{
	if (!IsValid(mNetOverlappingItem))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("APlayerBase::UsePlayerItem() : NO mNetOverlappingItem")));
		return;
	}
	
switch (mEOverlapItem)
   {
   case EItemType::HpPotion:
      {
         if (mServerPlayerMaxHP > mNetPlayerCurHP)
         {
            SetPlayerCurHP(mServerPlayerMaxHP);
         }
         else
         {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
            FString::Printf(TEXT("HP is Already FULL")));
            return;
         }
         break;
      }
   case EItemType::Magazine:
      {
   		if (mNetWeaponInstance && IsValid(mNetWeaponHandChild->GetChildActor()))
   		{
   			if (mNetWeaponInstance->CanGetMagazine())
   			{
   				mNetWeaponInstance->AddMagazine();
   			}
   			else
   			{
   				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
				   FString::Printf(TEXT("!! APlayerBase::UsePlayerItem : Ammo Is FULL !!")));
   				return; 
   			}	
   		}
	    else
	    {
	    	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
FString::Printf(TEXT("!! APlayerBase::UsePlayerItem : No mNetWeaponInstance !!")));
		    return;
	    }
         break;
      }
   case EItemType::Rifle:
   case EItemType::ShotGun:
   case EItemType::MachineGun:
   case EItemType::RPG:
      {
   		if (mNetWeaponHandChild->GetChildActor())
   		{
   			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("! Drop Plz..  !")));
   			return;
   		}
         PickUpWeapon(mEOverlapItem);
         break;
      }
   }
   
   if (EItemType::None != mEOverlapItem)
   {
   	APlayerBase* Player = Cast<APlayerBase>(mNetOverlappingItem->GetOwner());
   	if (Player)
   	{
   		/* Server UI Update(Self) */
   		if (IsLocallyControlled())
   		{
   			if (EItemType::Magazine == mEOverlapItem)
   			{
   				mNetWeaponInstance->WeaponUIReloading();
   			}
   			mNetOverlappingItem->UseItem(Player);
   		}
   		/* Client UI Update(From Server) */
   		else
   		{
   			UsePlayerItemClient(Player);
   		}
   	}
   }
}

void APlayerBase::UsePlayerItemClient_Implementation(APlayerBase* TriggerPlayer)
{
	if (IsLocallyControlled())
	{
		// mNetWeaponInstance->WeaponUIReloading();
		mNetOverlappingItem->UseItem(TriggerPlayer);
	/* Item UI */
	// GetWorldTimerManager().SetTimer(AmmoTimer, this, &APlayerBase::TimerAmmo, 1.f, false);
	}
	
}

void APlayerBase::ChangeAnimInstanceClass(UClass* AnimClass)
{
	if (IsValid(AnimClass))
	{
		GetMesh()->SetAnimInstanceClass(AnimClass);
		mLyraAnimInstance = Cast<ULyraPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerBase::OnMontageEnded);
		}
	}
}


void APlayerBase::PickUpWeapon(EItemType OverlappedWeapon)
{
	/* ONLY SERVER */
	if (bNetIsFire || bServerIsJump || bNetIsDead || bNetIsDashing)
	{
		return;
	}

	if (EItemType::None == OverlappedWeapon)
	{
		return;
	}

	if (mNetWeaponInstance && IsValid(mNetWeaponHandChild->GetChildActor()))
	{
		if (!bIsAMode)
		{
			mNetWeaponInstance->ChangeABMode(true);
			if (IsValid(mModeWidget))
			{
				mModeWidget->ChangeMode();
			}
		}
	}

	SetMasterWeapon(OverlappedWeapon);
	CreateWeapon();
	SetWeaponVisibility(true, false);
}

// ================ UI Funtion ==================== 
void APlayerBase::DamageUIClient_Implementation(AEnemyBase* HitPawn)
{
	if (true == IsLocallyControlled())
	{
		HitPawn->bDamageUICheckKey = true;
	}
}

void APlayerBase::DamageHeadShotUIClient_Implementation(AEnemyBase* HitPawn, bool IsHeadShot)
{
	if (true == IsLocallyControlled())
	{
		if (nullptr == HitPawn)
		{
			return;
		}
		HitPawn->SetIsHeadShot(IsHeadShot);
	}
}


void APlayerBase::OnRep_mPlayerName()
{
	
	if (IsValid(mPlayerNameWidgetComponent))
	{
		//Cast
		mPlayerNameWidget = Cast<UPlayerName>(mPlayerNameWidgetComponent->GetUserWidgetObject());
		if (mPlayerNameWidget)
		{
			// PlayerName
			if (mPlayerNameWidget)
			{
				mPlayerNameWidget->SetPlayerText(mPlayerName);
				mPlayerNameWidgetComponent->SetVisibility(true);
			}
		}
	}
}

void APlayerBase::AddDashUI()
{
	if (IsValid(mDashWidget))
	{
		mDashWidget->AddToViewport();
	}
}

void APlayerBase::DeleteDashUI()
{
	if (IsValid(mDashWidget))
	{
		mDashWidget->RemoveFromParent();
	}
}

void APlayerBase::WeaponUIUpdate(bool IsEquip)
{
	if (IsValid(mCrossHairWidget) && IsValid(mModeWidget) && mNetWeaponInstance)
	{
		/* Equip */
		if (true == IsEquip)
		{
			if (false == mCrossHairWidget->IsInViewport())
			{
				mCrossHairWidget->AddToViewport();
			}

			if (false == mModeWidget->IsInViewport())
			{
				mModeWidget->AddToViewport();
			}
			mNetWeaponInstance->InitalizeWeaponUI();
			mNetWeaponInstance->WeaponUIReloading();
		}
		/* Unequip */
		else
		{
			if (true == mCrossHairWidget->IsInViewport())
			{
				mCrossHairWidget->RemoveFromParent();
			}
			if (true == mModeWidget->IsInViewport())
			{
				mModeWidget->RemoveFromParent();
			}
			mNetWeaponInstance->DestroyWeaponUI();
		}
	}
}


void APlayerBase::UpdateTeamPlayerUI()
{
	AInGameState* GameState = GetWorld()->GetGameState<AInGameState>();
	if (GameState && mTeamHPWidget)
	{
		mTeamHPWidget->UpdateTeamDisplay(GameState->PlayerNames, GameState->PlayerHPs);
	}
}

void APlayerBase::CreateMissionUI()
{
	// Get the full URL of the current level
	FName CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald,
	                                 FString::Printf(TEXT("CurrentLevelName: %s"), *CurrentLevelName.ToString()));
	// Check if the URL contains the specific level and parameter
	if (CurrentLevelName == "Bridge_P")
	{
		mMissionWidgetClass = nullptr;
		mMissionWidget = nullptr;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, "WidgetDelete!!!!");
		return;
	}
	if (IsValid(mMissionWidgetClass))
	{
		if (GetWorld() && !GetWorld()->bIsTearingDown)
		{
			if(IsLocallyControlled())
			{
				mMissionWidget = CreateWidget<UMissionWidget>(GetWorld(), mMissionWidgetClass);
				mMissionWidget->AddToViewport();
			}
		}
	}
}

/// == RebirthUI ====
void APlayerBase::RebirthUIView()
{
	if (IsValid(mRebirthComponent))
	{
		mRebirthWidget = Cast<URebirthUI>(mRebirthComponent->GetWidget());
		if (mRebirthWidget)
		{
			mRebirthWidget->PlayMyAnimation();
			mRebirthComponent->SetVisibility(true);
		}
	}
}

void APlayerBase::RebirthUIUnView()
{
	if (mRebirthWidget)
	{
		mRebirthComponent->SetVisibility(false);
		mRebirthWidget->StopMyAnimation();
	}
}


void APlayerBase::SetReadyCurrentPlayer()
{
	AInGameState* GameState = GetWorld()->GetGameState<AInGameState>();
	GameState->SetReadyPlayer();
}

void APlayerBase::SetReadyCurrentPlayerServer_Implementation()
{
	AInGameState* GameState = GetWorld()->GetGameState<AInGameState>();
	GameState->SetReadyPlayer();
}


void APlayerBase::SetPlayerName()
{
	AMainPlayerState* CurrentPlayerState = Cast<AMainPlayerState>(GetPlayerState());
	if (IsValid(CurrentPlayerState))
	{
		mPlayerName = FText::FromString(CurrentPlayerState->GetPlayerName());
	}
	if (mPlayerName.IsEmpty())
	{
		return;
	}
	if(IsValid(mPlayerNameWidgetComponent))
    {
      		//Cast
            mPlayerNameWidget = Cast<UPlayerName>(mPlayerNameWidgetComponent->GetUserWidgetObject());
             if (mPlayerNameWidget)
             {
                 // PlayerName
                 if (mPlayerNameWidget)
                 {
                 	mPlayerNameWidget->SetPlayerText(mPlayerName);
                 	mPlayerNameWidgetComponent->SetVisibility(true);
                 }
             }
     }
	
}

void APlayerBase::RebirthUIViewClient_Implementation()
{
	if (IsValid(mRebirthComponent))
	{
		mRebirthWidget = Cast<URebirthUI>(mRebirthComponent->GetWidget());
		if (mRebirthWidget)
		{
			mRebirthWidget->PlayMyAnimation();
			mRebirthComponent->SetVisibility(true);
		}
	}
}

void APlayerBase::RebirthUIUnViewClient_Implementation()
{
	if (mRebirthWidget)
	{
		mRebirthComponent->SetVisibility(false);
		mRebirthWidget->StopMyAnimation();
	}
}

/* ------------------------- Minimap ------------------------- */
// The minimap rotates accordingly based on the direction the player moves.
void APlayerBase::RotateMinimapWithPlayer_Implementation()
{
	if (true == IsLocallyControlled())
	{
		FAttachmentTransformRules attachTransformRule(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,
		                                              EAttachmentRule::SnapToTarget, false);

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		mMinimapCapture->AttachToComponent(RootComponent, attachTransformRule);
		mMinimapCapture->SetRelativeLocation(FVector(0.0f, 0.0f, 4010.f));
	}
}
