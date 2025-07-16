// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Crazy6/Actor/Enemy/EnemyBase.h"
#include "Crazy6/Actor/Enemy/Controller/BossController.h"
#include "Crazy6/Actor/Enemy/Controller/EnemyController.h"


EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AEnemyBase* EnemyPawn = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (false == IsValid(EnemyPawn))
	{
		return EBTNodeResult::Failed;
	}

	AEnemyController* EnemyController = Cast<AEnemyController>(EnemyPawn->GetController());
	if (true == IsValid(EnemyController))
	{
		if (false == IsValid(EnemyController->GetBlackboardComponent()->GetValueAsObject("Target")))
		{
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		ABossController* BossController = Cast<ABossController>(EnemyPawn->GetController());
		if (true == IsValid(BossController))
		{
			if (false == IsValid(BossController->GetBlackboardComponent()->GetValueAsObject("Target")))
			{
				return EBTNodeResult::Failed;
			}
		}
	}

	FAIAttackEnd OnAttackEndDelegate;
	OnAttackEndDelegate.BindLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
		});
	
	EnemyPawn->SetAIState(EAIState::Attack);
	EnemyPawn->SetAttackEndDelegate(OnAttackEndDelegate);
	EnemyPawn->Attack();
	
	return EBTNodeResult::InProgress;
}
