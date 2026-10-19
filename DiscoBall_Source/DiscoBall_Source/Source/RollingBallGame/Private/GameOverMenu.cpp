// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGame.h"
#include "GameOverMenu.h"

#include "GameHud.h"
#include "EndlessRunnerGameMode.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameOverMenu::NativeConstruct() {
	Super::NativeConstruct();
	if (RestartBtn) RestartBtn->OnClicked.AddDynamic(this, &UGameOverMenu::OnRestartClick);
	if (ExitBtn) ExitBtn->OnClicked.AddDynamic(this, &UGameOverMenu::OnExitClick);

	AEndlessRunnerGameMode* GameMode = Cast<AEndlessRunnerGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode) {
		int32 FinalScore = GameMode->Score;
		if (PointsCount) PointsCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), FinalScore)));
	}
}

void UGameOverMenu::OnRestartClick() {
	UWorld* World = GetWorld();
	if (World) UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));
}

void UGameOverMenu::OnExitClick() {
	UWorld* World = GetWorld();
	if (World) UGameplayStatics::OpenLevel(World, TEXT("MainMenu"));
}
