// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 *
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AABPlayerController();

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	class UABHUDWidget* GetHUDWidget() const;

	void NPCKill(const class AABCharacter* const _pKilledNPC) const;
	void AddGameScore() const;

	void ChangeInputMode(const bool _bGameMode);

	void ShowResultUI();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UABHUDWidget> mHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UABGameplayWidget> mMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UABResultGameplayWidget> mResultWidgetClass;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	static const int32 msHUD_ZORDER;
	static const int32 msMENU_ZORDER;

	UPROPERTY()
		class UABHUDWidget* mpHUDWidget;

	UPROPERTY()
		class UABGameplayWidget* mpMenuWidget;
	static const FInputModeGameOnly msGAME_INPUT_MODE;
	static const FInputModeUIOnly msUI_INPUT_MODE;

	UPROPERTY()
		class UABResultGameplayWidget* mpResultWidget;

	UPROPERTY()
		class AABPlayerState* mpABPlayerState;

	void OnGamePause();
};
