// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/SaveGame.h"
#include "ABSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UABSaveGame();

	UPROPERTY()
		int32 mLevel;
	UPROPERTY()
		int32 mExp;
	UPROPERTY()
		int32 mHighScore;
	UPROPERTY()
		FString mPlayerName;
	UPROPERTY()
		int32 mCharacterMeshIndex;
};
