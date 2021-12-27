// Fill out your copyright notice in the Description page of Project Settings.


#include "ABResultGameplayWidget.h"
#include "Components/TextBlock.h"
#include "ABGameState.h"



//public
void UABResultGameplayWidget::BindGameState(const class AABGameState* const _pGameState)
{
	check(_pGameState != nullptr);

	mwpCurrentGameState = _pGameState;
}



//protected

void UABResultGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(mwpCurrentGameState.IsValid());

	UTextBlock* pResult = ExactCast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	check(pResult != nullptr);
	const FString& rResult = mwpCurrentGameState->IsGameCleared() ? TEXT("Mission Complete") : TEXT("Mission Failed");
	pResult->SetText(FText::FromString(rResult));

	UTextBlock* pTotalScore = ExactCast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	check(pTotalScore != nullptr);
	pTotalScore->SetText(FText::FromString(FString::FromInt(mwpCurrentGameState->GetTotalGameScore())));
}