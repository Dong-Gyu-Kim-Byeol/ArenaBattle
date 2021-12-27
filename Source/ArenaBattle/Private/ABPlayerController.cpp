// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"
#include "ABHUDWidget.h"
#include "ABPlayerState.h"
#include "ABCharacter.h"
#include "ABCharacterStatComponent.h"
#include "ABGameplayWidget.h"
#include "ABResultGameplayWidget.h"
#include "ABGameState.h"


const FInputModeGameOnly AABPlayerController::msGAME_INPUT_MODE;
const FInputModeUIOnly AABPlayerController::msUI_INPUT_MODE;

const int32 AABPlayerController::msHUD_ZORDER(1);
const int32 AABPlayerController::msMENU_ZORDER(3);


//public

AABPlayerController::AABPlayerController() :
	mpABPlayerState(nullptr),
	mpHUDWidget(nullptr),
	mpMenuWidget(nullptr),
	mpResultWidget(nullptr)
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget> sHUDWidgetClass_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	check(sHUDWidgetClass_C.Succeeded());
	mHUDWidgetClass = sHUDWidgetClass_C.Class;

	static ConstructorHelpers::FClassFinder<UABGameplayWidget> sMenuWidgetClass_C(TEXT("/Game/Book/UI/UI_Menu.UI_Menu_C"));
	check(sMenuWidgetClass_C.Succeeded());
	mMenuWidgetClass = sMenuWidgetClass_C.Class;

	static ConstructorHelpers::FClassFinder<UABResultGameplayWidget> sResultWidgetClass_C(TEXT("/Game/Book/UI/UI_Result.UI_Result_C"));
	check(sResultWidgetClass_C.Succeeded());
	mResultWidgetClass = sResultWidgetClass_C.Class;
}

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPlayerController::OnPossess(APawn* aPawn)
{
	ABLOG_S(Warning);
	Super::OnPossess(aPawn);

	mpHUDWidget = CreateWidget<UABHUDWidget>(this, mHUDWidgetClass);
	check(mpHUDWidget != nullptr);
	mpHUDWidget->AddToViewport(msHUD_ZORDER);

	mpResultWidget = CreateWidget<UABResultGameplayWidget>(this, mResultWidgetClass);
	check(mpResultWidget != nullptr);
}

UABHUDWidget* AABPlayerController::GetHUDWidget() const
{
	return mpHUDWidget;
}

void AABPlayerController::NPCKill(const AABCharacter* const _pKilledNPC) const
{
	mpABPlayerState->AddExp(_pKilledNPC->GetDropExp());
}

void AABPlayerController::AddGameScore() const
{
	mpABPlayerState->AddGameScore();
}

void AABPlayerController::ChangeInputMode(const bool _bGameMode)
{
	if (_bGameMode == true)
	{
		SetInputMode(msGAME_INPUT_MODE);
		bShowMouseCursor = false;

		if (mpMenuWidget != nullptr)
		{
			mpMenuWidget->MarkPendingKill();
			mpMenuWidget = nullptr;
		}
	}
	else
	{
		SetInputMode(msUI_INPUT_MODE);
		bShowMouseCursor = true;
	}
}

void AABPlayerController::ShowResultUI()
{
	const AABGameState* const pABGameState = ExactCast<AABGameState>(UGameplayStatics::GetGameState(this));
	check(pABGameState != nullptr);

	mpResultWidget->BindGameState(pABGameState);

	mpResultWidget->AddToViewport();
	ChangeInputMode(false);
}




//protected

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputMode(true);

	mpABPlayerState = ExactCast<AABPlayerState>(PlayerState);
	check(mpABPlayerState != nullptr);
	mpHUDWidget->BindPlayerState(mpABPlayerState);

	mpABPlayerState->mOnPlayerStateChanged_UABHUDWidget.Broadcast();
}

void AABPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AABPlayerController::OnGamePause);
}


//private

void AABPlayerController::OnGamePause()
{
	check(mpMenuWidget == nullptr);
	mpMenuWidget = CreateWidget<UABGameplayWidget>(this, mMenuWidgetClass);
	check(mpMenuWidget != nullptr);

	mpMenuWidget->AddToViewport(msMENU_ZORDER);

	SetPause(true);
	ChangeInputMode(false);
}