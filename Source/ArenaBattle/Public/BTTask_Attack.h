// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _rOwnerComp, uint8* _pNodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& _rOwnerComp, uint8* _pNodeMemory, float _deltaSeconds) override;
};
