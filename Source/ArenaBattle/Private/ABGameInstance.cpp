// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameInstance.h"

//public

UABGameInstance::UABGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> sABCharacterDataTable(TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData"));
	check(sABCharacterDataTable.Succeeded());
	mpABCharacterTable = sABCharacterDataTable.Object;
	check(mpABCharacterTable->GetRowMap().Num() > 0);
}

void UABGameInstance::Init()
{
	Super::Init();
}

FABCharacterData* UABGameInstance::GetABCharacterData(const int32 _level)
{
	return mpABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(_level), TEXT(""));
}