// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatComponent.h"
#include "../Enemy/EnemyDataManager.h"
#include "Boss/BossZero.h"
#include "Crazy6/GameMode/Gameinstance/Crazy6GameInstanceSubsystem.h"
#include "Net/UnrealNetwork.h"


UEnemyStatComponent::UEnemyStatComponent()
{
	SetIsReplicatedByDefault(true);
}

void UEnemyStatComponent::SetStat(const FEnemyInfoTable* DataTable)
{
	check(DataTable)

	//Set Boss' MaxHp according to the number of players
	ABossZero* BossPawn = Cast<ABossZero>(GetOwner());
	if (true == IsValid(BossPawn))
	{
		UCrazy6GameInstanceSubsystem* GameSubsystem = BossPawn->GetGameInstance()->GetSubsystem<UCrazy6GameInstanceSubsystem>();
		if (true == IsValid(GameSubsystem))
		{
			mHP = DataTable->mHP * GameSubsystem->GetDifficultyOption();
			mHPMax = DataTable->mHP * GameSubsystem->GetDifficultyOption();
		}
	}
	else
	{
		mHP = DataTable->mHP;
		mHPMax = DataTable->mHP;
	}
	
	mAttack = DataTable->mAttack;
	mAttackRange = DataTable->mAttackRange;
	mStatInfoByState = DataTable->mStatInfoByState;

	SetHp(mHPMax);
}

void UEnemyStatComponent::ChangeStatInfoByState(EAIState State)
{
	const FAIStatInfoByState* StatInfoByState = FindStatInfoByState(State);
	if (nullptr == StatInfoByState)
	{
		return;
	}

	mMoveSpeed = StatInfoByState->mConfiguredMoveSpeed;
}

float UEnemyStatComponent::ApplyDamage(float Damage)
{
	const bool IsDeath = mHP <= Damage;
	const float CurHp = mHP - Damage;

	if (true == OnHitDamageDelegate.IsBound())
	{
		const float fDamage = IsDeath ? mHP : Damage;
		OnHitDamageDelegate.Broadcast(fDamage);
	}

	SetHp(CurHp);

	if (true == IsDeath)
	{
		if (true == OnHpZERODelegate.IsBound())
		{
			OnHpZERODelegate.Broadcast();
		}
	}

	return CurHp;
}

const FAIStatInfoByState* UEnemyStatComponent::FindStatInfoByState(EAIState State)
{
	if (true == mStatInfoByState.IsEmpty())
	{
		return nullptr;
	}
	
	const FAIStatInfoByState* pResult = mStatInfoByState.Find(State);
	return pResult;
}

void UEnemyStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UEnemyStatComponent, mMoveSpeed);
}

void UEnemyStatComponent::SetHp(float CurHp)
{
	mHP = FMath::Clamp(CurHp, 0.0f, mHPMax);
	if (true == OnChangedHpDelegate.IsBound())
	{
		OnChangedHpDelegate.Broadcast(mHP);
	}
}
