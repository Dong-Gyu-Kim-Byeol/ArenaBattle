// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterState(class UABCharacterStatComponent* _pNewCharacterState);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
private:
	TWeakObjectPtr<class UABCharacterStatComponent> mwpCurrentCharacterStat;

	UPROPERTY()
		class UProgressBar* mpHPProgressBar;
	
};
