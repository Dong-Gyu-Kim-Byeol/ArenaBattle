// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsNotDead.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UBTDecorator_IsNotDead : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsNotDead();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& _rOwnerComp, uint8* _pNodeMemory) const override;

};
