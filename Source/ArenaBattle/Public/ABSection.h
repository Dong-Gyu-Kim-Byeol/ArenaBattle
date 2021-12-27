// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABSection.generated.h"

UCLASS()
class ARENABATTLE_API AABSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABSection();

	virtual void OnConstruction(const FTransform& _rTransform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* mpMesh;

	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrivateAccess = true))
		UBoxComponent* mpTrigger;

	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		TArray<UStaticMeshComponent*> mGateMeshes;

	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrivateAccess = true))
		TArray<UBoxComponent*> mGateTriggers;

	enum class ESectionState
	{
		READY,
		BATTLE,
		COMPLETE
	};

	ESectionState mCurrentState;
	UPROPERTY(EditAnywhere, Category = State, Meta = (AllowPrivateAccess = true))
		bool mbNoBattle;

	static const float mENEMY_SPAWN_TIME;
	static const float mITEM_BOX_SPAWN_TIME;
	FTimerHandle mSpawnNPCTimerHandle;
	FTimerHandle mSpawnItemBoxTimerHandle;

	void SetState(const ESectionState _newState);
	void OperateGates(const bool _bOpen);
	UFUNCTION()
		void OnTriggerBeginOverlap(UPrimitiveComponent* _pOverlappedComponent, AActor* _pOtherActor, 
			UPrimitiveComponent* _pOtherComp, int32 _otherBodyIndex, bool _bFromSweep, 
			const FHitResult& _rSweepResult);
	UFUNCTION()
		void OnGateTriggerBeginOverlap(UPrimitiveComponent* _pOverlappedComponent, AActor* _pOtherActor,
			UPrimitiveComponent* _pOtherComp, int32 _otherBodyIndex, bool _bFromSweep,
			const FHitResult& _rSweepResult);
	
	void OnNPCSpawn();
	void OnItemBoxSpawn();

	UFUNCTION()
		void OnKeyNPCDestroyed(AActor* _pDestroyedActor);
};
