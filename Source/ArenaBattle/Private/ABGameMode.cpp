// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"
#include "ABPlayerState.h"
#include "ABGameState.h"



//public

AABGameMode::AABGameMode() :
	mpABGameState(nullptr),
	mScoreToClear(2)
{
	DefaultPawnClass = AABCharacter::StaticClass();

	PlayerControllerClass = AABPlayerController::StaticClass();
	PlayerStateClass = AABPlayerState::StaticClass();

	GameStateClass = AABGameState::StaticClass();
}

void AABGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(mpABGameState == nullptr);
	mpABGameState = ExactCast<AABGameState>(GameState);
	check(mpABGameState != nullptr);
}

void AABGameMode::PostLogin(APlayerController* _newPlayer)
{
	Super::PostLogin(_newPlayer);

	AABPlayerState* pABPlayerState = Cast<AABPlayerState>(_newPlayer->PlayerState);
	check(pABPlayerState != nullptr);

	pABPlayerState->InitPlayerData();
}

void AABGameMode::AddScore(const AABPlayerController* const _pScoredPlayer)
{
	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		const AABPlayerController* const pABPlayerController = ExactCast<AABPlayerController>(it->Get());
		check(pABPlayerController != nullptr);
		if ((pABPlayerController != nullptr) && (_pScoredPlayer == pABPlayerController))
		{
			pABPlayerController->AddGameScore();
			break;
		}
	}

	mpABGameState->AddGameScore();

	if (GetScore() >= mScoreToClear)
	{
		mpABGameState->SetGameCleared();

		for (FConstPawnIterator it = GetWorld()->GetPawnIterator(); it; ++it)
		{
			(*it)->TurnOff();
		}

		for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
		{
			AABPlayerController* const pABPlayerController = ExactCast<AABPlayerController>(it->Get());
			check(pABPlayerController != nullptr);

			pABPlayerController->ShowResultUI();
		}
	}
}

int32 AABGameMode::GetScore() const
{
	return mpABGameState->GetTotalGameScore();
}