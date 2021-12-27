// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
		void NextCharacter(const bool _bForward);

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		int32 mCurrentMeshIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		int32 mMaxMeshIndex;

	TWeakObjectPtr<USkeletalMeshComponent> mwpTargetComponent;

	USkeletalMesh* mpLoadedSkeletalMesh;

	UPROPERTY()
		class UButton* mpPrevButton;
	UPROPERTY()
		class UButton* mpNextButton;
	UPROPERTY()
		class UButton* mpConfirmButton;
	UPROPERTY()
		class UEditableTextBox* mpTextBox;

private:
	UFUNCTION()
		void OnPrevClicked();
	UFUNCTION()
		void OnNextClicked();
	UFUNCTION()
		void OnConfirmClicked();
};
