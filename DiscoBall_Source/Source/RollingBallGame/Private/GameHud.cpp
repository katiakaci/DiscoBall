// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGame.h"
#include "GameHud.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void UGameHud::NativeConstruct() {
	if (PauseButton) PauseButton->OnClicked.AddDynamic(this, &UGameHud::OnPauseClick);

	UWidgetBlueprintLibrary::SetInputMode_GameAndUI(UGameplayStatics::GetPlayerController(GetWorld(), 0), this);
}

void UGameHud::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	AEndlessRunnerGameMode* GM = Cast<AEndlessRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GM) return;

	const bool bX2Active = (GM->ScoreMultiplier > 1);
	const bool bMagnetActive = GM->MagnetActivated;
	const bool bShieldActive = GM->ShieldActivated;

	const FVector2D TopPosition(-376.f, -44.f);
	const FVector2D SecondPosition(-376.f, 60.f);
	const FVector2D ThirdPosition(-376.f, 164.f);

	// Bonus x2
	if (X2ProgressBar && bX2Active) {
		X2ProgressBar->SetVisibility(ESlateVisibility::Visible);

		float Percent = GM->X2TimeRemaining / GM->X2Duration;
		X2ProgressBar->SetPercent(Percent);

		if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(X2ProgressBar->Slot)) {
			Slot->SetPosition(TopPosition);
		}
	}
	else X2ProgressBar->SetVisibility(ESlateVisibility::Hidden);

	// Bonus Magnet
	if (MagnetProgressBar && bMagnetActive) {
		MagnetProgressBar->SetVisibility(ESlateVisibility::Visible);

		float Percent = GM->MagnetTimeRemaining / GM->MagnetDuration;
		MagnetProgressBar->SetPercent(Percent);

		if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(MagnetProgressBar->Slot)) {
			Slot->SetPosition(bX2Active ? SecondPosition : TopPosition);
		}
	}
	else MagnetProgressBar->SetVisibility(ESlateVisibility::Hidden);

	// Bonus Shield
	if (ShieldProgressBar && bShieldActive) {
		ShieldProgressBar->SetVisibility(ESlateVisibility::Visible);

		float Percent = GM->ShieldTimeRemaining / GM->ShieldDuration;
		ShieldProgressBar->SetPercent(Percent);

		if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(ShieldProgressBar->Slot)) {
			Slot->SetPosition(bX2Active && bMagnetActive ? ThirdPosition : (bX2Active || bMagnetActive ? SecondPosition : TopPosition));
		}
	}
	else ShieldProgressBar->SetVisibility(ESlateVisibility::Hidden);
}

void UGameHud::UpdateScore(int32 NewScore) {
	if (PointsCount) {
		PointsCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewScore)));
	}
}

void UGameHud::UpdateLife(int32 RemainingLives) {
	if (!LifeBar || LifeTextures.Num() == 0) return;
	int32 LifeIndex = FMath::Clamp(RemainingLives, 0, LifeTextures.Num() - 1);
	if (LifeTextures[LifeIndex]) LifeBar->SetBrushFromTexture(LifeTextures[LifeIndex], true);
}

void UGameHud::ShowGameOverMenu() {
	if (!IsValid(GameOverWidgetClass)) return;
	SetPauseButtonVisible(false);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
	if (Widget) Widget->AddToViewport();
}

void UGameHud::OnPauseClick() {
	if (IsValid(PauseMenuWidgetClass)) {
		SetPauseButtonVisible(false);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);
		if (Widget) Widget->AddToViewport();
	}
}

void UGameHud::SetPauseButtonVisible(bool bVisible) {
	if (PauseButton) {
		PauseButton->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UGameHud::UpdateBestScore(int32 NewBestScore) {
	if (HighestScore) {
		HighestScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewBestScore)));
	}
}
