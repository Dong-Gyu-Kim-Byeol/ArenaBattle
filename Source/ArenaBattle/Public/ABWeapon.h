// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABWeapon.generated.h"

UCLASS()
class ARENABATTLE_API AABWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type _endPlayReason) override;

public:	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* mpWeapon;

	float GetAttackRange() const;
	float GetAttackDamage() const;
	float GetAttackModifier() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float mAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float mAttackDamageMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float mAttackDamageMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float mAttackModifierMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float mAttackModifierMax;


	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
		float mAttackDamage;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
		float mAttackModifier;
};
