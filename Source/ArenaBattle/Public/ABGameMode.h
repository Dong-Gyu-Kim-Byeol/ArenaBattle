// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/GameModeBase.h"
#include "ABGameMode.generated.h"

/**
 *
 */
UCLASS()
class ARENABATTLE_API AABGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AABGameMode();

	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* _newPlayer) override;
	void AddScore(const class AABPlayerController* const _pScoredPlayer);
	int32 GetScore() const;

private:
	UPROPERTY()
		class AABGameState* mpABGameState;

	UPROPERTY()
		int32 mScoreToClear;
};
