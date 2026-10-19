// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGame.h"
#include "EndlessRunnerGameMode.h"
#include "DiscoBallPawnBase.h"
#include "FloorTile.h"
#include "GameHud.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"

AEndlessRunnerGameMode::AEndlessRunnerGameMode() {
	PrimaryActorTick.bCanEverTick = true;
}

void AEndlessRunnerGameMode::BeginPlay() {
	if (UGameplayStatics::DoesSaveGameExist(TEXT("BestScoreSlot"), 0)) {
		BestScoreSave = Cast<UHighestScoreSave>(UGameplayStatics::LoadGameFromSlot(TEXT("BestScoreSlot"), 0));
	}
	else BestScoreSave = Cast<UHighestScoreSave>(UGameplayStatics::CreateSaveGameObject(UHighestScoreSave::StaticClass()));
	BestScore = BestScoreSave->BestScore;

	if (!GameHudClass) return;
	GameHud = CreateWidget<UGameHud>(GetWorld(), GameHudClass);
	GameHud->AddToViewport();
	GameHud->UpdateScore(Score);
	GameHud->UpdateBestScore(BestScore);

	CurrentForwardSpeed = InitialForwardSpeed;

	CreateInitialFloorTiles();
}

void AEndlessRunnerGameMode::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (bIsGameOverTransition) {
		CurrentForwardSpeed = FMath::FInterpTo(CurrentForwardSpeed, 0.f, DeltaSeconds, 0.6f);
	}
	else {
		EnsureEnoughTilesAhead();
		if (ScoreMultiplier > 1) X2TimeRemaining = FMath::Max(0.f, X2TimeRemaining - DeltaSeconds);
		if (MagnetActivated) MagnetTimeRemaining = FMath::Max(0.f, MagnetTimeRemaining - DeltaSeconds);
		if (ShieldActivated) ShieldTimeRemaining = FMath::Max(0.f, ShieldTimeRemaining - DeltaSeconds);
	}
}

void AEndlessRunnerGameMode::CreateInitialFloorTiles() {
	AFloorTile* Tile = AddFloorTile();
	if (Tile) {
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
	}

	for (int i = 0; i < NumInitialFloorTiles; i++) {
		AddFloorTile();
	}
}

AFloorTile* AEndlessRunnerGameMode::AddFloorTile() {
	UWorld* World = GetWorld();

	if (World) {
		TilesSinceLastSameBonus++;
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);
		if (Tile) NextSpawnPoint = Tile->GetAttachTransform();
		return Tile;
	}
	return nullptr;
}

void AEndlessRunnerGameMode::EnsureEnoughTilesAhead()
{
	if (!GetWorld()) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	float PlayerX = PlayerPawn->GetActorLocation().X;

	int32 TilesAhead = 0;

	for (AFloorTile* Tile : FloorTiles) {
		if (Tile && Tile->GetActorLocation().X > PlayerX) TilesAhead++;
	}

	while (TilesAhead < MinTilesAhead) {
		AFloorTile* NewTile = AddFloorTile();
		if (!NewTile) break;

		FloorTiles.Add(NewTile);
		TilesAhead++;
	}
}

void AEndlessRunnerGameMode::AddScore(int32 Amount) {
	if (bIsGameOverTransition) return;

	Score += (Amount * ScoreMultiplier);
	if (GameHud) {
		GameHud->UpdateScore(Score);
		if (Amount == 5) GameHud->ShowPlusFive(CurrentActiveColor);
	}
	int32 Steps = Score / ScorePerSpeedStep;
	CurrentForwardSpeed = FMath::Clamp(InitialForwardSpeed + Steps * SpeedPerStep, InitialForwardSpeed, MaxForwardSpeed);
}

void AEndlessRunnerGameMode::AddLife() {
	if (bIsGameOverTransition) return;

	RemainingLives = FMath::Min(RemainingLives + 1, 5);
	if (GameHud) GameHud->UpdateLife(RemainingLives);
}

void AEndlessRunnerGameMode::RemoveLife() {
	if (bIsGameOverTransition) return;

	RemainingLives--;
	if (GameHud) GameHud->UpdateLife(RemainingLives);
	if (RemainingLives <= 0) {
		bIsGameOverTransition = true;

		UBP_GameInstance* MyGI = Cast<UBP_GameInstance>(GetWorld()->GetGameInstance());
		if (MyGI && MyGI->SoundEnabled) UGameplayStatics::PlaySound2D(this, GameOverCue);

		GetWorldTimerManager().SetTimer(GameOverTimer, this, &AEndlessRunnerGameMode::ShowGameOverMenu, GameOverDelay, false);
	}
}

void AEndlessRunnerGameMode::ResetScoreMultiplier() {
	ScoreMultiplier = 1;
	X2TimeRemaining = 0.f;
}

void AEndlessRunnerGameMode::ResetMagnet() {
	MagnetActivated = false;
	MagnetTimeRemaining = 0.f;
}

void AEndlessRunnerGameMode::ResetShield() {
	ShieldActivated = false;
	ShieldTimeRemaining = 0.f;
}

void AEndlessRunnerGameMode::ActivateX2() {
	if (bIsGameOverTransition) return;

	ScoreMultiplier = 2;
	X2TimeRemaining = X2Duration;
	GetWorldTimerManager().ClearTimer(ScoreMultiplierTimer);
	GetWorldTimerManager().SetTimer(ScoreMultiplierTimer, this, &AEndlessRunnerGameMode::ResetScoreMultiplier, X2Duration, false);
}

void AEndlessRunnerGameMode::ActivateMagnet() {
	if (bIsGameOverTransition) return;

	MagnetActivated = true;
	MagnetTimeRemaining = MagnetDuration;
	GetWorldTimerManager().ClearTimer(MagnetTimer);
	GetWorldTimerManager().SetTimer(MagnetTimer, this, &AEndlessRunnerGameMode::ResetMagnet, MagnetDuration, false);
}

void AEndlessRunnerGameMode::ActivateShield() {
	if (bIsGameOverTransition) return;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Shield activated!")));

	ShieldActivated = true;
	ShieldTimeRemaining = ShieldDuration;
	GetWorldTimerManager().ClearTimer(ShieldTimer);
	GetWorldTimerManager().SetTimer(ShieldTimer, this, &AEndlessRunnerGameMode::ResetShield, ShieldDuration, false);
}

void AEndlessRunnerGameMode::ShowGameOverMenu() {
	if (Score > BestScore) {
		BestScore = Score;
		BestScoreSave->BestScore = BestScore;
		UGameplayStatics::SaveGameToSlot(BestScoreSave, TEXT("BestScoreSlot"), 0);
	}
	if (GameHud) GameHud->ShowGameOverMenu();
}

void AEndlessRunnerGameMode::SetCurrentActiveColor(EDiscoColor NewColor) {
	CurrentActiveColor = NewColor;
}

bool AEndlessRunnerGameMode::ShouldAttractPickup(EDiscoColor PickupColor, FVector PickupLocation) const
{
	if (!MagnetActivated || PickupColor != CurrentActiveColor) return false;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return false;

	const ADiscoBallPawnBase* DiscoPawn = Cast<ADiscoBallPawnBase>(PlayerPawn);
	if (!DiscoPawn) return false;

	const FVector PlayerLocation = DiscoPawn->GetBallWorldLocation();

	const float TileLength = 1080.f;

	const float MinX = PlayerLocation.X;         // 0 tuile derrière
	const float MaxX = PlayerLocation.X + TileLength;   // 1 tuile devant

	return PickupLocation.X >= MinX && PickupLocation.X <= MaxX;
}