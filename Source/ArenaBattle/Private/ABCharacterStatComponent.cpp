// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterStatComponent.h"
#include "ABGameInstance.h"


//public

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent() :
	mpCurrentStatData(nullptr),
	mLevel(1),
	mCurrentHP(0.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}


//protected

// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetNewLevel(mLevel);
}


//public

void UABCharacterStatComponent::SetNewLevel(const uint32 _newLevel)
{
	UABGameInstance* pABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(pABGameInstance != nullptr);
	
	mpCurrentStatData = pABGameInstance->GetABCharacterData(_newLevel);
	check(mpCurrentStatData != nullptr);

	mLevel = _newLevel;
	SetHP(mpCurrentStatData->MaxHP);
}

void UABCharacterStatComponent::SetDamage(const float _newDamage)
{
	check(mpCurrentStatData != nullptr);

	SetHP(FMath::Clamp<float>(mCurrentHP - _newDamage, 0.0f, mpCurrentStatData->MaxHP));
}

void UABCharacterStatComponent::SetHP(const float _newHP)
{
	mCurrentHP = _newHP;
	mOnHPChanged_UABHUDWidget.Broadcast();
	mOnHPChanged_UABCharacterWidget.Broadcast();

	if (mCurrentHP < KINDA_SMALL_NUMBER)
	{
		mCurrentHP = 0.0f;
		mOnHPIsZero.Broadcast();
	}
}

float UABCharacterStatComponent::GetAttack() const
{
	check(mpCurrentStatData != nullptr);

	return mpCurrentStatData->Attack;
}

float UABCharacterStatComponent::GetHPRatio() const
{
	check(mpCurrentStatData != nullptr);

	return (mpCurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (mCurrentHP / mpCurrentStatData->MaxHP);
}

uint32 UABCharacterStatComponent::GetDropExp() const
{
	check(mpCurrentStatData != nullptr);

	return mpCurrentStatData->DropExp;
}