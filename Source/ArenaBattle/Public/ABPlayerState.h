// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AABPlayerState();

public:
	FString mSaveSlotName;

	FOnPlayerStateChangedDelegate mOnPlayerStateChanged_UABHUDWidget;

	void InitPlayerData();
	void SavePlayerData();

	int32 GetGameScore() const;
	int32 GetGameHighScore() const;
	int32 GetCharacterLevel() const;
	int32 GetCharacterMeshIndex() const;

	float GetExpRatio() const;
	bool AddExp(const int32 _incomeExp);
	void AddGameScore();

protected:
	UPROPERTY(Transient)
		int32 mGameScore;
	UPROPERTY(Transient)
		int32 mGameHighScore;

	UPROPERTY(Transient)
		int32 mCharacterLevel;

	UPROPERTY(Transient)
		int32 mExp;

	UPROPERTY(Transient)
		int32 mCharacterMeshIndex;

private:
	void SetCharacterLevel(const int32 _newCharacterLevel);
	const struct FABCharacterData* mpCurrentStatData;

};
