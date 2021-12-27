// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UABAnimInstance();
	virtual void NativeUpdateAnimation(float _deltaSeconds) override;

public:
	FOnNextAttackCheckDelegate mOnNextAttackCheck;
	FOnAttackHitCheckDelegate mOnAttackHitCheck;

	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 _newSection);
	void SetDeadAnim() { mbIsDead = true; }
	bool GetDeadAnim() const { return mbIsDead; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly/*BlueprintReadWrite*/, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float mCurrentPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool mbIsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* mpAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool mbIsDead;

	UFUNCTION()
		void AnimNotify_AttackHitCheck();
	UFUNCTION()
		void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 _section);
};
