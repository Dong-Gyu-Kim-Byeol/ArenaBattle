// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameplayWidget.h"
#include "Components/Button.h"
#include "ABPlayerController.h"


//protected

void UABGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mpResumeButton = ExactCast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (mpResumeButton != nullptr)
	{
		mpResumeButton->OnClicked.AddDynamic(this, &UABGameplayWidget::OnResumeClicked);
	}

	mpReturnToTitleButton = ExactCast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	check(mpReturnToTitleButton != nullptr);
	mpReturnToTitleButton->OnClicked.AddDynamic(this, &UABGameplayWidget::OnReturnToTitleClicked);

	mpRetryGameButton = ExactCast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (mpRetryGameButton != nullptr)
	{
		mpRetryGameButton->OnClicked.AddDynamic(this, &UABGameplayWidget::OnRetryGameClicked);
	}

}

void UABGameplayWidget::OnResumeClicked()
{
	AABPlayerController* pABPlayerController = ExactCast<AABPlayerController>(GetOwningPlayer());
	check(pABPlayerController != nullptr);

	RemoveFromParent();

	pABPlayerController->ChangeInputMode(true);
	pABPlayerController->SetPause(false);
}

void UABGameplayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UABGameplayWidget::OnRetryGameClicked()
{
	AABPlayerController* const pABPlayerController = ExactCast<AABPlayerController>(GetOwningPlayer());
	check(pABPlayerController != nullptr);

	pABPlayerController->RestartLevel();
}