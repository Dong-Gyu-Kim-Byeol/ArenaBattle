// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerState.h"
#include "ABGameInstance.h"
#include "ABSaveGame.h"


//public

AABPlayerState::AABPlayerState() :
	mGameScore(0),
	mGameHighScore(0),
	mCharacterLevel(1),
	mExp(0),
	mSaveSlotName(TEXT("Player1")),
	mCharacterMeshIndex(0)
{
}

void AABPlayerState::InitPlayerData()
{
	const UABSaveGame* pABSaveGame = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(mSaveSlotName, 0));
	if (pABSaveGame == nullptr)
	{
		pABSaveGame = GetDefault<UABSaveGame>();
	}

	SetPlayerName(pABSaveGame->mPlayerName);
	SetCharacterLevel(pABSaveGame->mLevel);
	mGameHighScore = pABSaveGame->mHighScore;
	mExp = pABSaveGame->mExp;
	mCharacterMeshIndex = pABSaveGame->mCharacterMeshIndex;

	SavePlayerData();
}

void AABPlayerState::SavePlayerData()
{
	UABSaveGame* const pNewPlayerData = NewObject<UABSaveGame>();
	check(pNewPlayerData != nullptr);

	pNewPlayerData->mPlayerName = GetPlayerName();
	pNewPlayerData->mLevel = GetCharacterLevel();
	pNewPlayerData->mExp = mExp;
	pNewPlayerData->mHighScore = mGameHighScore;
	pNewPlayerData->mCharacterMeshIndex = mCharacterMeshIndex;

	const bool bSaveResult = UGameplayStatics::SaveGameToSlot(pNewPlayerData, mSaveSlotName, 0);
	checkf(bSaveResult == true, TEXT("%s, bSaveResult : %d"), TEXT("SaveGame Error!"), bSaveResult);

	pNewPlayerData->MarkPendingKill();
}

int32 AABPlayerState::GetGameScore() const
{
	return mGameScore;
}

int32 AABPlayerState::GetGameHighScore() const
{
	return mGameHighScore;
}

int32 AABPlayerState::GetCharacterLevel() const
{
	return mCharacterLevel;
}

int32 AABPlayerState::GetCharacterMeshIndex() const
{
	return mCharacterMeshIndex;
}

float AABPlayerState::GetExpRatio() const
{
	if (mpCurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	const float result = (float)mExp / (float)mpCurrentStatData->NextExp;
	ABLOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), result, mExp, mpCurrentStatData->NextExp);
	return result;
}

bool AABPlayerState::AddExp(const int32 _incomeExp)
{
	if (mpCurrentStatData->NextExp == -1)
	{
		ABLOG_S(Warning);
		return false;
	}

	bool bDidLevelUp = false;
	mExp += _incomeExp;
	if (mExp >= mpCurrentStatData->NextExp)
	{
		mExp -= mpCurrentStatData->NextExp;
		SetCharacterLevel(mCharacterLevel + 1);
		bDidLevelUp = true;
	}

	mOnPlayerStateChanged_UABHUDWidget.Broadcast();
	SavePlayerData();

	return bDidLevelUp;
}

void AABPlayerState::AddGameScore()
{
	mGameScore++;
	if (mGameScore > mGameHighScore)
	{
		mGameHighScore = mGameScore;
	}

	mOnPlayerStateChanged_UABHUDWidget.Broadcast();
	SavePlayerData();
}




//private

void AABPlayerState::SetCharacterLevel(const int32 _newCharacterLevel)
{
	UABGameInstance* pABGameInstance = Cast<UABGameInstance>(GetGameInstance());
	check(pABGameInstance != nullptr);

	mpCurrentStatData = pABGameInstance->GetABCharacterData(_newCharacterLevel);
	check(mpCurrentStatData != nullptr);

	mCharacterLevel = _newCharacterLevel;
}
