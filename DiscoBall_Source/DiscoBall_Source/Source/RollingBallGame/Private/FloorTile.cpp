// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGame.h"
#include "FloorTile.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EndlessRunnerGameMode.h"

// Sets default values
AFloorTile::AFloorTile()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Mesh"));
	FloorMesh->AttachTo(SceneComponent);

	// fl�che pour indiquer o� attacher la prochaine tuile
	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point"));
	AttachPoint->AttachTo(SceneComponent);

	// fl�ches pour marquer la position de la voie centrale, celle de gauche et celle de droite
	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Center Lane"));
	CenterLane->AttachTo(SceneComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Lane"));
	RightLane->AttachTo(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Lane"));
	LeftLane->AttachTo(SceneComponent);

	// bo�te de collision invisible pour d�tecter les �v�nements
	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Trigger Box"));
	FloorTriggerBox->AttachTo(SceneComponent);

	// taille de la bo�te : 
	FloorTriggerBox->SetBoxExtent(FVector(32.f, 500.f, 200.f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); // File > ProjectSettings > Collision (under Engine) > Open Preset > all the names are there
}

void AFloorTile::BeginPlay() {
	Super::BeginPlay();
	RunGameMode = Cast<AEndlessRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnTriggerBoxOverlap);

	SpawnPickups();
	SpawnStars();
}

void AFloorTile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AFloorTile::OnTriggerBoxOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!RunGameMode || !OtherActor) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (OtherActor != PlayerPawn) return;

	if (DestroyHandle.IsValid()) return;

	RunGameMode->AddFloorTile();

	float DestroyDelay = 1.f;
	if (RunGameMode->bIsGameOverTransition) DestroyDelay = 4.f;

	GetWorldTimerManager().SetTimer(DestroyHandle, this, &AFloorTile::DestroyFloorTile, DestroyDelay, false);
}

void AFloorTile::DestroyFloorTile() {
	if (DestroyHandle.IsValid()) {
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}
	this->Destroy();
}

void AFloorTile::SpawnPickups()
{
	// ---------- NOTE ----------
	int32 NoteLane = -1;
	if (PickupClass && FMath::FRand() < NoteSpawnChance)
	{
		NoteLane = FMath::RandRange(0, 2);
		UArrowComponent* NoteArrow =
			NoteLane == 0 ? LeftLane :
			NoteLane == 1 ? CenterLane : RightLane;

		FVector Loc = NoteArrow->GetComponentLocation();
		Loc.Z += 75.f;
		GetWorld()->SpawnActor<AActor>(PickupClass, Loc, FRotator::ZeroRotator);
	}

	// ---------- PLUS 5 ----------
	int32 Plus5Lane = -1;
	if (Plus5PickupClass && FMath::FRand() < Plus5SpawnChance)
	{
		do {
			Plus5Lane = FMath::RandRange(0, 2);
		} while (Plus5Lane == NoteLane);

		UArrowComponent* Plus5Arrow =
			Plus5Lane == 0 ? LeftLane :
			Plus5Lane == 1 ? CenterLane : RightLane;

		FVector Loc = Plus5Arrow->GetComponentLocation();
		Loc.Z += 95.f;
		GetWorld()->SpawnActor<AActor>(Plus5PickupClass, Loc, FRotator::ZeroRotator);
	}

	// ---------- BONUSES ----------
	bool bBonusSpawned = false;
	int32 BonusLane = -1;

	float Roll = FMath::FRand();
	float Acc = 0.f;

	// ---------- HEART ----------
	Acc += HeartSpawnChance;
	if (!bBonusSpawned && Roll < Acc && HeartPickupClass &&
		(RunGameMode->LastBonusType != EBonusType::Heart ||
			RunGameMode->TilesSinceLastSameBonus >= RunGameMode->MinTilesBetweenSameBonus))
	{
		do {
			BonusLane = FMath::RandRange(0, 2);
		} while (BonusLane == NoteLane || BonusLane == Plus5Lane);

		bBonusSpawned = true;
		RunGameMode->LastBonusType = EBonusType::Heart;
		RunGameMode->TilesSinceLastSameBonus = 0;

		UArrowComponent* Arrow =
			BonusLane == 0 ? LeftLane :
			BonusLane == 1 ? CenterLane : RightLane;

		FVector Loc = Arrow->GetComponentLocation();
		Loc.Z += 75.f;
		GetWorld()->SpawnActor<AActor>(HeartPickupClass, Loc, FRotator::ZeroRotator);
	}

	// ---------- X2 ----------
	Acc += X2SpawnChance;
	if (!bBonusSpawned && Roll < Acc && X2PickupClass &&
		(RunGameMode->LastBonusType != EBonusType::X2 ||
			RunGameMode->TilesSinceLastSameBonus >= RunGameMode->MinTilesBetweenSameBonus))
	{
		do {
			BonusLane = FMath::RandRange(0, 2);
		} while (BonusLane == NoteLane || BonusLane == Plus5Lane);

		bBonusSpawned = true;
		RunGameMode->LastBonusType = EBonusType::X2;
		RunGameMode->TilesSinceLastSameBonus = 0;

		UArrowComponent* Arrow =
			BonusLane == 0 ? LeftLane :
			BonusLane == 1 ? CenterLane : RightLane;

		FVector Loc = Arrow->GetComponentLocation();
		Loc.Z += 95.f;
		GetWorld()->SpawnActor<AActor>(X2PickupClass, Loc, FRotator::ZeroRotator);
	}

	// ---------- SHIELD ----------
	Acc += BonusShieldSpawnChance;
	if (!bBonusSpawned && Roll < Acc && BonusShieldPickupClass &&
		(RunGameMode->LastBonusType != EBonusType::Shield ||
			RunGameMode->TilesSinceLastSameBonus >= RunGameMode->MinTilesBetweenSameBonus))
	{
		do {
			BonusLane = FMath::RandRange(0, 2);
		} while (BonusLane == NoteLane || BonusLane == Plus5Lane);

		bBonusSpawned = true;
		RunGameMode->LastBonusType = EBonusType::Shield;
		RunGameMode->TilesSinceLastSameBonus = 0;

		UArrowComponent* Arrow =
			BonusLane == 0 ? LeftLane :
			BonusLane == 1 ? CenterLane : RightLane;

		FVector Loc = Arrow->GetComponentLocation();
		Loc.Z += 95.f;
		GetWorld()->SpawnActor<AActor>(BonusShieldPickupClass, Loc, FRotator::ZeroRotator);
	}

	// ---------- MAGNET ----------
	Acc += BonusMagnetSpawnChance;
	if (!bBonusSpawned && Roll < Acc && BonusMagnetPickupClass &&
		(RunGameMode->LastBonusType != EBonusType::Magnet ||
			RunGameMode->TilesSinceLastSameBonus >= RunGameMode->MinTilesBetweenSameBonus))
	{
		do {
			BonusLane = FMath::RandRange(0, 2);
		} while (BonusLane == NoteLane || BonusLane == Plus5Lane);

		bBonusSpawned = true;
		RunGameMode->LastBonusType = EBonusType::Magnet;
		RunGameMode->TilesSinceLastSameBonus = 0;

		UArrowComponent* Arrow =
			BonusLane == 0 ? LeftLane :
			BonusLane == 1 ? CenterLane : RightLane;

		FVector Loc = Arrow->GetComponentLocation();
		Loc.Z += 55.f;
		GetWorld()->SpawnActor<AActor>(BonusMagnetPickupClass, Loc, FRotator::ZeroRotator);
	}
}

void AFloorTile::SpawnStars() {
	if (!StarClass) return;

	for (int32 i = 0; i < NumStarsToSpawn; i++) {
		FVector SpawnLocation = GetActorLocation();

		float Step = StarMaxX / (float)NumStarsToSpawn;
		SpawnLocation.X += 300.f + i * Step + FMath::RandRange(-80.f, 80.f);

		float Y;
		do {
			Y = StarBaseY + FMath::RandRange(-StarJitterY, StarJitterY);
		} while (Y > -700.f && Y < 620.f);
		SpawnLocation.Y += Y;

		SpawnLocation.Z += StarBaseZ + FMath::RandRange(-StarJitterZ, StarJitterZ);

		// petite rotation random
		FRotator SpawnRotation(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

		AActor* Star = GetWorld()->SpawnActor<AActor>(StarClass, SpawnLocation, SpawnRotation);
		if (Star) {
			// Taille random
			float S = FMath::FRandRange(StarScaleMin, StarScaleMax);
			Star->SetActorScale3D(FVector(0.f, S, S));
			Star->AttachRootComponentToActor(this);
		}
	}
}
