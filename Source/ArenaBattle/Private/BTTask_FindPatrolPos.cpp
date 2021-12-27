// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "ABAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& _rOwnerComp, uint8* _pNodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(_rOwnerComp, _pNodeMemory);

	APawn* pControllingPawn = _rOwnerComp.GetAIOwner()->GetPawn();
	if (pControllingPawn == nullptr) 
	{ 
		return EBTNodeResult::Failed; 
	}

	UNavigationSystemV1* pNavSystem = UNavigationSystemV1::GetNavigationSystem(pControllingPawn->GetWorld());
	if (pNavSystem == nullptr)
	{ 
		return EBTNodeResult::Failed; 
	}

	const FVector origin = _rOwnerComp.GetBlackboardComponent()->GetValueAsVector(AABAIController::msHOME_POS_KEY);
	FNavLocation nextPatrol;
	const float radius = 500.0f;
	if (pNavSystem->GetRandomPointInNavigableRadius(origin, radius, nextPatrol) == false)
	{ 
		return EBTNodeResult::Failed;
	}
	
	_rOwnerComp.GetBlackboardComponent()->SetValueAsVector(AABAIController::msPATROL_POS_KEY, nextPatrol.Location);
	return EBTNodeResult::Succeeded;
}