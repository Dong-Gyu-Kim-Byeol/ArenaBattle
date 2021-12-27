// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"



//public
UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}


//protected

void UBTService_Detect::TickNode(UBehaviorTreeComponent& _rOwnerComp, uint8* _pNodeMemory, float _deltaSeconds)
{
	Super::TickNode(_rOwnerComp, _pNodeMemory, _deltaSeconds);

	APawn* pControllingPawn = _rOwnerComp.GetAIOwner()->GetPawn();
	if (pControllingPawn == nullptr)
	{
		return;
	}

	const UWorld* pWorld = pControllingPawn->GetWorld();
	const FVector center = pControllingPawn->GetActorLocation();
	const float detectRadius = 600.0f;
	if (pWorld == nullptr)
	{
		return;
	}

	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams collisionQueryParam(NAME_Name, false, pControllingPawn);
	const bool bResult = pWorld->OverlapMultiByChannel(
		overlapResults,
		center,
		FQuat::Identity,
		ECollisionChannel_TRACE_Attack,
		FCollisionShape::MakeSphere(detectRadius),
		collisionQueryParam);

	if (bResult == true)
	{
		for(auto& overlapResult : overlapResults)
		{
			AABCharacter* pABCharacter = Cast<AABCharacter>(overlapResult.GetActor());
			if (pABCharacter != nullptr && pABCharacter->GetController()->IsPlayerController() == true)
			{
				_rOwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::msTARGET_KEY, pABCharacter);
#if ENABLE_DRAW_DEBUG
				{
					const float lifeTime = 0.5f;
					DrawDebugSphere(pWorld, center, detectRadius, 16, FColor::Green, false, lifeTime);

					DrawDebugPoint(pWorld, pABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, lifeTime);
					DrawDebugLine(pWorld, pControllingPawn->GetActorLocation(), pABCharacter->GetActorLocation(), FColor::Blue, false, lifeTime);
				}
#endif
				return;
			}
		}
	}
	else
	{
		_rOwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::msTARGET_KEY, nullptr);
#if ENABLE_DRAW_DEBUG
		{
			const float lifeTime = 0.3f;
			DrawDebugSphere(pWorld, center, detectRadius, 16, FColor::Red, false, lifeTime);
		}
#endif
	}
}