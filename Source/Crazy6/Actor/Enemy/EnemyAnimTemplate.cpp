// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimTemplate.h"
#include "EnemyBase.h"
#include "EnemyInfo.h"
#include "EnemyDataManager.h"
#include "EnemyMovement.h"
#include "Boss/BossZero.h"
#include "Epic/EpicCrunch.h"

UEnemyAnimTemplate::UEnemyAnimTemplate()
{
	
}

void UEnemyAnimTemplate::SetAnimData(const FName& _Key)
{
	const FEnemyAnimData* pAnimData = CEnemyDataManager::GetInst()->FindAnim(_Key);
	if (nullptr != pAnimData)
	{
		mSequenceMap = pAnimData->mSequenceMap;
		mBlendSpaceMap = pAnimData->mBlendSpaceMap;
		mMontageMap = pAnimData->mMontageMap;
	}
}

void UEnemyAnimTemplate::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	AEnemyBase* EnemyPawn = Cast<AEnemyBase>(TryGetPawnOwner());
	if (true == IsValid(EnemyPawn))
	{
		UEnemyMovement* Movement = Cast<UEnemyMovement>(EnemyPawn->GetMovementComponent());
		if (true == IsValid(Movement))
		{
			FVector EnemyForwardVec = EnemyPawn->GetActorForwardVector();
			FVector VelocityVec = Movement->GetVelocity().GetSafeNormal();
			mMoveDirection = FMath::RadiansToDegrees(FMath::Acos(EnemyForwardVec.Dot(VelocityVec)));
			FVector CrossProduct = FVector::CrossProduct(EnemyForwardVec, VelocityVec);
			if (CrossProduct.Z < 0)
			{
				mMoveDirection *= -1;
			}
			
			mMoveSpeed = Movement->GetVelocity().Length();
		}
	}
}

void UEnemyAnimTemplate::PlayAttackMontage()
{
	Montage_Play(mAttackMontage);
}

void UEnemyAnimTemplate::AnimNotify_AttackHitBoxOn()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			AEnemyBase* EnemyPawn = Cast<AEnemyBase>(TryGetPawnOwner());
			if (true == IsValid(EnemyPawn))
			{
				EnemyPawn->AttackHitBoxOn();
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_AttackHitBoxOff()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			AEnemyBase* EnemyPawn = Cast<AEnemyBase>(TryGetPawnOwner());
			if (true == IsValid(EnemyPawn))
			{
				EnemyPawn->AttackHitBoxOff();
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_AttackEnd()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			AEnemyBase* EnemyPawn = Cast<AEnemyBase>(TryGetPawnOwner());
			if (true == IsValid(EnemyPawn))
			{
				EnemyPawn->AttackEnd();
			}
		}
		break;
	default:
		break;
	}

	Montage_Stop(0.1, mAttackMontage);
}

void UEnemyAnimTemplate::AnimNotify_RushEnd()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			AEpicCrunch* EpicPawn = Cast<AEpicCrunch>(TryGetPawnOwner());
			if (true == IsValid(EpicPawn))
			{
				EpicPawn->WhenRushEnd();
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_EnergyBallSpawn()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			ABossZero* BossPawn = Cast<ABossZero>(TryGetPawnOwner());
			if (true == IsValid(BossPawn))
			{
				BossPawn->Shooting();
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_ChargeStart()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			ABossZero* BossPawn = Cast<ABossZero>(TryGetPawnOwner());
			if (true == IsValid(BossPawn))
			{
				BossPawn->Laser();
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_RollingMoveStart()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			ABossZero* BossPawn = Cast<ABossZero>(TryGetPawnOwner());
			if (true == IsValid(BossPawn))
			{
				BossPawn->Rolling();
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_RollingMoveEnd()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			ABossZero* BossPawn = Cast<ABossZero>(TryGetPawnOwner());
			if (true == IsValid(BossPawn))
			{
				BossPawn->Rolling();
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_SlamMoveStart()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			ABossZero* BossPawn = Cast<ABossZero>(TryGetPawnOwner());
			if (true == IsValid(BossPawn))
			{
				BossPawn->Slam();				
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_SlamMoveEnd()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			ABossZero* BossPawn = Cast<ABossZero>(TryGetPawnOwner());
			if (true == IsValid(BossPawn))
			{
				BossPawn->Slam();
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_BombStart()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			ABossZero* BossPawn = Cast<ABossZero>(TryGetPawnOwner());
			if (true == IsValid(BossPawn))
			{
				BossPawn->Bomb();
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_BombEnd()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			ABossZero* BossPawn = Cast<ABossZero>(TryGetPawnOwner());
			if (true == IsValid(BossPawn))
			{
				BossPawn->Bomb();
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_AdvancedChargeStart()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			ABossZero* BossPawn = Cast<ABossZero>(TryGetPawnOwner());
			if (true == IsValid(BossPawn))
			{
				BossPawn->AdvancedLaser();
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_HitEnd()
{
	switch (GetWorld()->GetNetMode())
	{
	case NM_Standalone:
	case NM_ListenServer:
		{
			IsHit = false;
			AEnemyBase* EnemyPawn = Cast<AEnemyBase>(TryGetPawnOwner());
			if (true == IsValid(EnemyPawn))
			{
				EnemyPawn->SetServerIsHit(IsHit);
			}
		}
		break;
	default:
		break;
	}
}

void UEnemyAnimTemplate::AnimNotify_DeathEnd()
{
	AEnemyBase* EnemyPawn = Cast<AEnemyBase>(TryGetPawnOwner());
	if (true == IsValid(EnemyPawn))
	{
		EnemyPawn->StartRagdoll();
	
		switch (GetWorld()->GetNetMode())
		{
		case NM_Standalone:
		case NM_ListenServer:
			{
				EnemyPawn->SetDeathTimer(3.0f);
			}
			break;
		default:
			break;
		}
	}
}

void UEnemyAnimTemplate::AnimNotify_FK_Off()
{
	IsUseFabrik = false;
}


