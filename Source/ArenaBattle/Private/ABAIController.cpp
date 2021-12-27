// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"


//public

const FName AABAIController::msHOME_POS_KEY(TEXT("HomePos"));
const FName AABAIController::msPATROL_POS_KEY(TEXT("PatrolPos"));
const FName AABAIController::msTARGET_KEY(TEXT("Target"));

AABAIController::AABAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetObject(TEXT("/Game/Book/AI/BB_ABCharacter.BB_ABCharacter"));
	ensure(BBAssetObject.Succeeded());
	mpBBAsset = BBAssetObject.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetObject(TEXT("/Game/Book/AI/BT_ABCharacter.BT_ABCharacter"));
	ensure(BTAssetObject.Succeeded());
	mpBTAsset = BTAssetObject.Object;
}

void AABAIController::OnPossess(APawn* _pInPawn)
{
	Super::OnPossess(_pInPawn);
}

void AABAIController::RunAI()
{
	if (UseBlackboard(mpBBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(msHOME_POS_KEY, GetPawn()->GetActorLocation());
		if (RunBehaviorTree(mpBTAsset) == false)
		{
			ABLOG(Error, TEXT("%s"), TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void AABAIController::StopAI()
{
	UBehaviorTreeComponent* pBehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (pBehaviorTreeComponent != nullptr)
	{
		pBehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
