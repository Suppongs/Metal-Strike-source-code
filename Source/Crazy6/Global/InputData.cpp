// Fill out your copyright notice in the Description page of Project Settings.


#include "InputData.h"

UDefaultInputData::UDefaultInputData()
{
	{
		// Default IMC Load
		static ConstructorHelpers::FObjectFinder<UInputMappingContext>
			Asset(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Dev/Player/Input/Mappings/IMC_Default.IMC_Default'"));

		if (Asset.Succeeded())
		{
			mDefaultContext = Asset.Object;
		}
	}

	{
		// MoveAction
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Move.IA_Move'"));

		if (Asset.Succeeded())
		{
			mMove = Asset.Object;
		}
	}

	{
		// JumpAction
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Jump.IA_Jump'"));

		if (Asset.Succeeded())
		{
			mJump = Asset.Object;
		}
	}

	{
		// CrouchAction
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Crouch.IA_Crouch'"));

		if (Asset.Succeeded())
		{
			mCrouch = Asset.Object;
		}
	}

	{
		// DashAction
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Ability_Dash.IA_Ability_Dash'"));

		if (Asset.Succeeded())
		{
			mDash = Asset.Object;
		}
	}

	{
		// LookMouseAction
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Look_Mouse.IA_Look_Mouse'"));

		if (Asset.Succeeded())
		{
			mLookMouse = Asset.Object;
		}
	}

	{
		// ESC menu
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Menu.IA_Menu'"));

		if (Asset.Succeeded())
		{
			mMenu = Asset.Object;
		}
	}

	{
		// Sprint
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Sprint.IA_Sprint'"));

		if (Asset.Succeeded())
		{
			mSprint = Asset.Object;
		}
	}

	{
		// Weapon Fire
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Weapon_Fire.IA_Weapon_Fire'"));

		if (Asset.Succeeded())
		{
			mFire = Asset.Object;
		}
	}

	{
		// Weapon Reload
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Weapon_Reload.IA_Weapon_Reload'"));

		if (Asset.Succeeded())
		{
			mReload = Asset.Object;
		}
	}

	{
		// Equip Weapon
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Weapon_Equip.IA_Weapon_Equip'"));

		if (Asset.Succeeded())
		{
			mEquipWeapon = Asset.Object;
		}
	}

	{
		// Zoom
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Zoom.IA_Zoom'"));

		if (Asset.Succeeded())
		{
			mZoom = Asset.Object;
		}
	}

	{
		// SwapWeaponMode
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_SwapABMode.IA_SwapABMode'"));

		if (Asset.Succeeded())
		{
			mSwapAB = Asset.Object;
		}
	}

	{
		// Interaction
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Interaction.IA_Interaction'"));

		if (Asset.Succeeded())
		{
			mInteraction = Asset.Object;
		}
	}

	{
		// Drop
		static ConstructorHelpers::FObjectFinder<UInputAction>
			Asset(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/Player/Input/Actions/IA_Weapon_Drop.IA_Weapon_Drop'"));

		if (Asset.Succeeded())
		{
			mDrop = Asset.Object;
		}
	}
}

