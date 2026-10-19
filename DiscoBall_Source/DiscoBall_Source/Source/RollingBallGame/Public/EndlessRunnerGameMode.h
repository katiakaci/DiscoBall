// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "BP_GameInstance.h"
#include "HighestScoreSave.h"
#include "EndlessRunnerGameMode.generated.h"

class UUserWidget;
class AFloorTile;

/**
 *
 */
UENUM(BlueprintType)
enum class EDiscoColor : uint8 {
	Red     UMETA(DisplayName = "Red"),
	Green   UMETA(DisplayName = "Green"),
	Blue    UMETA(DisplayName = "Blue"),
	Yellow  UMETA(DisplayName = "Yellow")
};

UENUM(BlueprintType)
enum class EBonusType : uint8
{
	None,
	Heart,
	X2,
	Shield,
	Magnet
};

UCLASS()
class ROLLINGBALLGAME_API AEndlessRunnerGameMode : public AGameMode {
	GENERATED_BODY()

public:
	UPROPERTY()
	EBonusType LastBonusType = EBonusType::None;

	UPROPERTY()
	int32 TilesSinceLastSameBonus = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	int32 MinTilesBetweenSameBonus = 8;

	UPROPERTY()
	UHighestScoreSave* BestScoreSave;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 BestScore = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 Score = 0;

	// Bonus x2
	UPROPERTY(BlueprintReadWrite, Category = "Score")
	int32 ScoreMultiplier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	float X2Duration = 5.f;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	float X2TimeRemaining = 0.f;

	FTimerHandle ScoreMultiplierTimer;

	// Bonus Magnet
	UPROPERTY(BlueprintReadWrite, Category = "Bonus")
	bool MagnetActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus")
	float MagnetDuration = 5.f;

	UPROPERTY(BlueprintReadOnly, Category = "Bonus")
	float MagnetTimeRemaining = 0.f;

	FTimerHandle MagnetTimer;

	// Bonus Shield
	UPROPERTY(BlueprintReadWrite, Category = "Bonus")
	bool ShieldActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus")
	float ShieldDuration = 5.f;

	UPROPERTY(BlueprintReadOnly, Category = "Bonus")
	float ShieldTimeRemaining = 0.f;

	FTimerHandle ShieldTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Bonus")
	EDiscoColor CurrentActiveColor;

	UFUNCTION(BlueprintCallable, Category = "Bonus")
	void SetCurrentActiveColor(EDiscoColor NewColor);

	UFUNCTION(BlueprintPure, Category = "Bonus")
	bool ShouldAttractPickup(EDiscoColor PickupColor, FVector PickupLocation) const;

	UPROPERTY(BlueprintReadOnly, Category = "Life")
	int32 RemainingLives = 5;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	class UGameHud* GameHud;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<class UUserWidget> GameHudClass;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<class AFloorTile> FloorTileClass;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32 NumInitialFloorTiles = 20;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	FTransform NextSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	TArray<float> LaneSwitchValues;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	TArray<AFloorTile*> FloorTiles;

	UPROPERTY(EditAnywhere, Category = "Difficulty")
	int32 ScorePerSpeedStep = 1;

	UPROPERTY(EditAnywhere, Category = "Difficulty")
	float SpeedPerStep = 20.f;

	UPROPERTY(BlueprintReadOnly, Category = "Game Over")
	bool bIsGameOverTransition = false;

	UPROPERTY(EditAnywhere, Category = "Game Over")
	float GameOverDelay = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* GameOverCue;

	UFUNCTION()
	void ShowGameOverMenu();

	FTimerHandle GameOverTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	float InitialForwardSpeed = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	float MaxForwardSpeed = 3500.f;

	UPROPERTY(BlueprintReadOnly, Category = "Difficulty")
	float CurrentForwardSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 MinTilesAhead = 15;

	void EnsureEnoughTilesAhead();

	UFUNCTION(BlueprintCallable, Category = "Floor Tiles")
	void CreateInitialFloorTiles();

	UFUNCTION(BlueprintCallable, Category = "Floor Tiles")
	AFloorTile* AddFloorTile();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Bonus")
	void ResetScoreMultiplier();

	UFUNCTION(BlueprintCallable, Category = "Bonus")
	void ResetMagnet();

	UFUNCTION(BlueprintCallable, Category = "Bonus")
	void ResetShield();

	UFUNCTION(BlueprintCallable, Category = "Bonus")
	void ActivateX2();

	UFUNCTION(BlueprintCallable, Category = "Bonus")
	void ActivateMagnet();

	UFUNCTION(BlueprintCallable, Category = "Bonus")
	void ActivateShield();

	UFUNCTION(BlueprintCallable, Category = "Life")
	void AddLife();

	UFUNCTION(BlueprintCallable, Category = "Life")
	void RemoveLife();

	AEndlessRunnerGameMode();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
};
