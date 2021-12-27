// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()

public:
	AABAIController();
	virtual void OnPossess(APawn* _pInPawn) override;

	void RunAI();
	void StopAI();

	static const FName msHOME_POS_KEY;
	static const FName msPATROL_POS_KEY;
	static const FName msTARGET_KEY;

private:
	UPROPERTY()
		class UBehaviorTree* mpBTAsset;
	UPROPERTY()
		class UBlackboardData* mpBBAsset;
	
};
