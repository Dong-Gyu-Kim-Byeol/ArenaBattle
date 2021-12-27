// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Blueprint/UserWidget.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UABCharacterStatComponent* _pCharacterStat);
	void BindPlayerState(class AABPlayerState* _pPlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<class UABCharacterStatComponent> mwpCurrentCharacterStat;
	TWeakObjectPtr<class AABPlayerState> mwpCurrentPlayerState;

	UPROPERTY()
		class UProgressBar* mpHPBar;
	UPROPERTY()
		class UProgressBar* mpExpBar;
	UPROPERTY()
		class UTextBlock* mpPlayerName;
	UPROPERTY()
		class UTextBlock* mpPlayerLevel;
	UPROPERTY()
		class UTextBlock* mpCurrentScore;
	UPROPERTY()
		class UTextBlock* mpHighScore;
};
