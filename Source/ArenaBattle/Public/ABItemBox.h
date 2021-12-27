// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	UPROPERTY(VisibleAnywhere, Category = Box)
		UBoxComponent* mpTrigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
		UStaticMeshComponent* mpBox;

	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* mpEffect;

	UPROPERTY(EditAnywhere, Category = Box)
		TSubclassOf<class AABWeapon> mWeaponItemClass;

private:
	UFUNCTION()
		void OnCharacterOverlap(UPrimitiveComponent* _pOverlappedComponent, AActor* _pOtherActor, 
			UPrimitiveComponent* _pOtherComp, int32 _otherBodyIndex, bool _bFromSweep, 
			const FHitResult& _rSweepResult);

	UFUNCTION()
		void OnEffectFinished(UParticleSystemComponent* _pParticleSystem);
};
