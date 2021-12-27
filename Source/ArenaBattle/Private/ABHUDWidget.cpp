// Fill out your copyright notice in the Description page of Project Settings.


#include "ABHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ABCharacterStatComponent.h"
#include "ABPlayerState.h"


//public:

void UABHUDWidget::BindCharacterStat(class UABCharacterStatComponent* _pCharacterStat)
{
	check(_pCharacterStat != nullptr);
	mwpCurrentCharacterStat = _pCharacterStat;
	_pCharacterStat->mOnHPChanged_UABHUDWidget.AddUObject(this, &UABHUDWidget::UpdateCharacterStat);
}

void UABHUDWidget::BindPlayerState(class AABPlayerState* _pPlayerState)
{
	check(_pPlayerState != nullptr);
	mwpCurrentPlayerState = _pPlayerState;
	_pPlayerState->mOnPlayerStateChanged_UABHUDWidget.AddUObject(this, &UABHUDWidget::UpdatePlayerState);
}


//protected:

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mpHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	check(mpHPBar != nullptr);

	mpExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	check(mpExpBar != nullptr);

	mpPlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	check(mpPlayerName != nullptr);

	mpPlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	check(mpPlayerLevel != nullptr);

	mpCurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	check(mpCurrentScore != nullptr);

	mpHighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	check(mpHighScore != nullptr);
}

void UABHUDWidget::UpdateCharacterStat()
{
	check(mwpCurrentCharacterStat.IsValid());

	mpHPBar->SetPercent(mwpCurrentCharacterStat->GetHPRatio());
}

void UABHUDWidget::UpdatePlayerState()
{
	check(mwpCurrentPlayerState.IsValid());

	mpExpBar->SetPercent(mwpCurrentPlayerState->GetExpRatio());

	mpPlayerName->SetText(FText::FromString(mwpCurrentPlayerState->GetPlayerName()));
	
	mpPlayerLevel->SetText(FText::FromString(FString::FromInt(mwpCurrentPlayerState->GetCharacterLevel())));
	mpCurrentScore->SetText(FText::FromString(FString::FromInt(mwpCurrentPlayerState->GetGameScore())));
	mpHighScore->SetText(FText::FromString(FString::FromInt(mwpCurrentPlayerState->GetGameHighScore())));
}

