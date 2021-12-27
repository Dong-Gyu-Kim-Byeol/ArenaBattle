// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


//public

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& _rOwnerComp, uint8* _pNodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(_rOwnerComp, _pNodeMemory);

	AABCharacter* pABCharacter = Cast<AABCharacter>(_rOwnerComp.GetAIOwner()->GetPawn());
	ensure(pABCharacter != nullptr);

	const AABCharacter* pTarget = Cast<AABCharacter>(_rOwnerComp.GetBlackboardComponent()->GetValueAsObject(AABAIController::msTARGET_KEY));
	ensure(pTarget != nullptr);
	if (pTarget == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector lookVector = pTarget->GetActorLocation() - pABCharacter->GetActorLocation();
	lookVector.Z = 0.0f;
	const FRotator targetRot = FRotationMatrix::MakeFromX(lookVector).Rotator();
	const float interpSpeed = 5.0f;
	pABCharacter->SetActorRotation(FMath::RInterpTo(pABCharacter->GetActorRotation(), targetRot, GetWorld()->GetDeltaSeconds(), interpSpeed));

	return EBTNodeResult::Succeeded;
}
