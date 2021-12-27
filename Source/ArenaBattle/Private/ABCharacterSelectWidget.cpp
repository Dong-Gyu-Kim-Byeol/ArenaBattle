// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterSelectWidget.h"
#include "ABCharacterSetting.h"
#include "ABGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Engine/AssetManager.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "ABSaveGame.h"
#include "ABPlayerState.h"



//protected

void UABCharacterSelectWidget::NextCharacter(const bool _bForward)
{
	_bForward ? mCurrentMeshIndex++ : mCurrentMeshIndex--;

	if (mCurrentMeshIndex == -1)
	{
		mCurrentMeshIndex = mMaxMeshIndex - 1;
	}
	else if (mCurrentMeshIndex == mMaxMeshIndex)
	{
		mCurrentMeshIndex = 0;
	}

	const UABCharacterSetting* const pCharacterSetting = GetDefault<UABCharacterSetting>();
	check(pCharacterSetting != nullptr);

	const FSoftObjectPath& rAsset = pCharacterSetting->mCharacterAssets[mCurrentMeshIndex];

	const UABGameInstance* const pABGameInstance = GetWorld()->GetGameInstance<UABGameInstance>();
	check(pABGameInstance != nullptr);

	check(mwpTargetComponent.IsValid());

	if (mpLoadedSkeletalMesh != nullptr)
	{
		UAssetManager::GetStreamableManager().Unload(mpLoadedSkeletalMesh);
		mpLoadedSkeletalMesh = nullptr;
	}
	mpLoadedSkeletalMesh = UAssetManager::GetStreamableManager().LoadSynchronous<USkeletalMesh>(rAsset);
	check(mpLoadedSkeletalMesh != nullptr);

	mwpTargetComponent->SetSkeletalMesh(mpLoadedSkeletalMesh);
}

void UABCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mpLoadedSkeletalMesh = nullptr;

	mCurrentMeshIndex = 0;
	const UABCharacterSetting* const pCharacterSetting = GetDefault<UABCharacterSetting>();
	check(pCharacterSetting != nullptr);
	mMaxMeshIndex = pCharacterSetting->mCharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> it(GetWorld()); it; ++it)
	{
		mwpTargetComponent = it->GetSkeletalMeshComponent();
		break;
	}

	mpPrevButton = ExactCast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	check(mpPrevButton != nullptr);

	mpNextButton = ExactCast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	check(mpNextButton != nullptr);

	mpConfirmButton = ExactCast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	check(mpConfirmButton != nullptr);

	mpTextBox = ExactCast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	check(mpTextBox != nullptr);

	mpPrevButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnPrevClicked);
	mpNextButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnNextClicked);
	mpConfirmButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnConfirmClicked);
}



// private

void UABCharacterSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void UABCharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UABCharacterSelectWidget::OnConfirmClicked()
{
	const FString characterName = mpTextBox->GetText().ToString();
	if (characterName.Len() <= 0 || characterName.Len() > 10)
	{
		return;
	}

	UABSaveGame* const pNewPlayerData = NewObject<UABSaveGame>();
	check(pNewPlayerData != nullptr);

	pNewPlayerData->mPlayerName = characterName;
	pNewPlayerData->mLevel = 1;
	pNewPlayerData->mExp = 0;
	pNewPlayerData->mHighScore = 0;
	pNewPlayerData->mCharacterMeshIndex = mCurrentMeshIndex;

	const AABPlayerState* const pDefaultABPlayerState = GetDefault<AABPlayerState>();
	check(pDefaultABPlayerState != nullptr);

	if (UGameplayStatics::SaveGameToSlot(pNewPlayerData, pDefaultABPlayerState->mSaveSlotName, 0) == true)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Gameplay"));
	}
	else
	{
		checkNoEntry();
	}

	pNewPlayerData->MarkPendingKill();
}