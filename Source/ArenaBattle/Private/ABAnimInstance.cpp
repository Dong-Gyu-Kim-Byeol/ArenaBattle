// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance() :
	mCurrentPawnSpeed(0.0f),
	mbIsInAir(false),
	mbIsDead(false)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> sAttackMontage(TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	ensure(sAttackMontage.Succeeded());
	mpAttackMontage = sAttackMontage.Object;
}

void UABAnimInstance::NativeUpdateAnimation(float _deltaSeconds)
{
	Super::NativeUpdateAnimation(_deltaSeconds);

	APawn* pPawn = TryGetPawnOwner();
	if (::IsValid(pPawn) == false) { return; }
	
	if (mbIsDead == false)
	{
		mCurrentPawnSpeed = pPawn->GetVelocity().Size();

		ACharacter* pCharacter = Cast<ACharacter>(pPawn);
		if (pCharacter != nullptr)
		{
			mbIsInAir = pCharacter->GetMovementComponent()->IsFalling();
		}
	}
}

void UABAnimInstance::PlayAttackMontage()
{
	ABCHECK(mbIsDead == false);
	Montage_Play(mpAttackMontage, 1.0f);
}

void UABAnimInstance::JumpToAttackMontageSection(int32 _newSection)
{
	ABCHECK(mbIsDead == false);
	ABCHECK(Montage_IsPlaying(mpAttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(_newSection), mpAttackMontage);
}

//private

void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	mOnAttackHitCheck.Broadcast();
}

void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	mOnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 _section)
{
	ABCHECK(FMath::IsWithinInclusive(_section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), _section));
}