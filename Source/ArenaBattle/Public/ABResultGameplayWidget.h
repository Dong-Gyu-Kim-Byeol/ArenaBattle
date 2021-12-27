// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "ABGameplayWidget.h"
#include "ABResultGameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABResultGameplayWidget : public UABGameplayWidget
{
	GENERATED_BODY()

public:
	void BindGameState(const class AABGameState* const _pGameState);
	
protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class AABGameState> mwpCurrentGameState;
};
