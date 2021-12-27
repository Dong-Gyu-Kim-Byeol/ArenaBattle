// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameState.h"



//public
AABGameState::AABGameState() :
	mTotalGameScore(0),
	mbGameCleared(false)
{
}

//public
int32 AABGameState::GetTotalGameScore() const
{
	return mTotalGameScore;
}

void AABGameState::AddGameScore()
{
	mTotalGameScore++;
}

bool AABGameState::IsGameCleared() const
{
	return mbGameCleared;
}

void AABGameState::SetGameCleared()
{
	mbGameCleared = true;
}