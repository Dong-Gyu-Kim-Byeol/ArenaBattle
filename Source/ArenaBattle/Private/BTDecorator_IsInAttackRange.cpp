// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


//public

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("IsInAttackRange");
}


//protected

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& _rOwnerComp, uint8* _pNodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(_rOwnerComp, _pNodeMemory);

	AABCharacter* pControllingPawn = Cast<AABCharacter>(_rOwnerComp.GetAIOwner()->GetPawn());
	ensure(pControllingPawn != nullptr);

	AABCharacter* pTarget = Cast<AABCharacter>(_rOwnerComp.GetBlackboardComponent()->GetValueAsObject(AABAIController::msTARGET_KEY));
	if (pTarget == nullptr)
	{
		return false;
	}

	bResult = (pControllingPawn->GetDistanceTo(pTarget) <= pControllingPawn->GetFinalAttackRange());
	return bResult;
}
