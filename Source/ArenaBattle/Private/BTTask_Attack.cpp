// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "ABAIController.h"
#include "ABCharacter.h"


//public

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& _rOwnerComp, uint8* _pNodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(_rOwnerComp, _pNodeMemory);

	AABCharacter* pABCharacter = Cast<AABCharacter>(_rOwnerComp.GetAIOwner()->GetPawn());
	ensure(pABCharacter != nullptr);

	pABCharacter->Attack();

	return EBTNodeResult::InProgress;
}


// protected:

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& _rOwnerComp, uint8* _pNodeMemory, float _deltaSeconds)
{
	Super::TickTask(_rOwnerComp, _pNodeMemory, _deltaSeconds);

	const AABCharacter* pABCharacter = Cast<AABCharacter>(_rOwnerComp.GetAIOwner()->GetPawn());
	ensure(pABCharacter != nullptr);

	if (pABCharacter->GetIsAattacking() == false)
	{
		FinishLatentTask(_rOwnerComp, EBTNodeResult::Succeeded);
	}
}

