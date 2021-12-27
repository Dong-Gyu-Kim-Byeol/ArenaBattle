// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsNotDead.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "ABAnimInstance.h"


//public

UBTDecorator_IsNotDead::UBTDecorator_IsNotDead()
{
	NodeName = TEXT("IsNotDead");
}


//protected

bool UBTDecorator_IsNotDead::CalculateRawConditionValue(UBehaviorTreeComponent& _rOwnerComp, uint8* _pNodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(_rOwnerComp, _pNodeMemory);

	AABCharacter* pControllingPawn = Cast<AABCharacter>(_rOwnerComp.GetAIOwner()->GetPawn());
	ensure(pControllingPawn != nullptr);

	if (pControllingPawn->GetIsDead() == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}
