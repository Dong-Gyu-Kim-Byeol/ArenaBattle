// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnHPIsZeroDelegate mOnHPIsZero;
	FOnHPChangedDelegate mOnHPChanged_UABHUDWidget;
	FOnHPChangedDelegate mOnHPChanged_UABCharacterWidget;

	void SetNewLevel(const uint32 _newLevel);

	void SetDamage(const float _newDamage);
	void SetHP(const float _newHP);
	float GetAttack() const;
	float GetHPRatio() const;
	uint32 GetDropExp() const;

private:
	struct FABCharacterData* mpCurrentStatData;

	UPROPERTY(EditAnywhere, Category = State, Meta = (AllowPrivateAccess = true))
		uint32 mLevel;

	UPROPERTY(Transient/*직렬화 제외*/, VisibleInstanceOnly, Category = State, Meta = (AllowPrivateAccess = true))
		float mCurrentHP;
};
