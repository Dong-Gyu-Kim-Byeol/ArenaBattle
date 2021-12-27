// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterWidget.h"
#include "ABCharacterStatComponent.h"
#include "Components/ProgressBar.h"


//public

void UABCharacterWidget::BindCharacterState(class UABCharacterStatComponent* _pNewCharacterState)
{
	ensure(_pNewCharacterState != nullptr);

	mwpCurrentCharacterStat = _pNewCharacterState;
	_pNewCharacterState->mOnHPChanged_UABCharacterWidget.AddUObject(this, &UABCharacterWidget::UpdateHPWidget);
}


//protected

void UABCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mpHPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	ensure(mpHPProgressBar != nullptr);
	UpdateHPWidget();
}

void UABCharacterWidget::UpdateHPWidget()
{
	if (mwpCurrentCharacterStat.IsValid())
	{
		if (mpHPProgressBar != nullptr)
		{
			mpHPProgressBar->SetPercent(mwpCurrentCharacterStat->GetHPRatio());
		}
	}
}