// Fill out your copyright notice in the Description page of Project Settings.


#include "ABUIPlayerController.h"
#include "Blueprint/Userwidget.h"

void AABUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(mUIWidgetClass != nullptr);

	mpUIWidgetInstance = CreateWidget<UUserWidget>(this, mUIWidgetClass);
	check(mpUIWidgetInstance != nullptr);

	mpUIWidgetInstance->AddToViewport();

	FInputModeUIOnly mode;
	mode.SetWidgetToFocus(mpUIWidgetInstance->GetCachedWidget());
	SetInputMode(mode);
	bShowMouseCursor = true;
}