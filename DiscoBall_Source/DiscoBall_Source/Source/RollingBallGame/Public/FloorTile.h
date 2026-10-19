// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "FloorTile.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;

UCLASS()
class ROLLINGBALLGAME_API AFloorTile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FloorMesh;

	// Fl�che indiquant o� attacher la prochaine tuile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* CenterLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* RightLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* LeftLane;

	// Zone de collision pour d�tecter quand le joueur entre/sort de la tuile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* FloorTriggerBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> PickupClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float NoteSpawnChance = 0.50f;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<AActor> Plus5PickupClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float Plus5SpawnChance = 0.01f;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<AActor> X2PickupClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float X2SpawnChance = 0.02f;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<AActor> HeartPickupClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float HeartSpawnChance = 0.03f;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<AActor> BonusMagnetPickupClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float BonusMagnetSpawnChance = 0.02f;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<AActor> BonusShieldPickupClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float BonusShieldSpawnChance = 0.02f;

	UPROPERTY(EditDefaultsOnly, Category = "Stars")
	TSubclassOf<AActor> StarClass;

	UPROPERTY(EditDefaultsOnly, Category = "Stars")
	int32 NumStarsToSpawn = 15;

	UPROPERTY(EditDefaultsOnly, Category = "Stars")
	float StarMaxX = 2000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Stars")
	float StarBaseY = -770.f;

	UPROPERTY(EditDefaultsOnly, Category = "Stars")
	float StarBaseZ = -2000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Stars")
	float StarJitterY = 7000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Stars")
	float StarJitterZ = 3575.f;

	UPROPERTY(EditDefaultsOnly, Category = "Stars")
	float StarScaleMin = 0.7f;

	UPROPERTY(EditDefaultsOnly, Category = "Stars")
	float StarScaleMax = 3.f;

	AFloorTile();

	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE const FTransform GetAttachTransform() const {
		return AttachPoint->GetComponentTransform();
	}

protected:
	UPROPERTY(VisibleInstanceOnly)
	class AEndlessRunnerGameMode* RunGameMode;

	UPROPERTY()
	FTimerHandle DestroyHandle;

	UFUNCTION()
	void OnTriggerBoxOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DestroyFloorTile();

	UFUNCTION()
	void SpawnPickups();

	UFUNCTION()
	void SpawnStars();

	virtual void BeginPlay() override;
};
