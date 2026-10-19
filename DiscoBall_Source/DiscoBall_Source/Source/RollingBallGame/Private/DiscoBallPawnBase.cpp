// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGame.h"
#include "DiscoBallPawnBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "EndlessRunnerGameMode.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ADiscoBallPawnBase::ADiscoBallPawnBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	ShieldBubble = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldBubble"));
	ShieldBubble->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShieldBubble->SetVisibility(false);
}

void ADiscoBallPawnBase::BeginPlay()
{
	Super::BeginPlay();

	bIsChangingLane = false;
	bHorizontalInputHeld = false;
	CurrentLane = 1;
	NextLane = 1;

	TArray<UActorComponent*> Components = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* C : Components) {
		if (C && C->GetName() == TEXT("Ball")) {
			BallMeshComp = Cast<UStaticMeshComponent>(C);
			break;
		}
	}

	if (!BallMeshComp) {
		BallMeshComp = FindComponentByClass<UStaticMeshComponent>();
	}

	if (BallMeshComp) {
		BallMeshComp->SetSimulatePhysics(true);
		BallMeshComp->SetNotifyRigidBodyCollision(true);
		BallMeshComp->OnComponentHit.AddDynamic(this, &ADiscoBallPawnBase::OnBallHit);

		if (ShieldBubble) {
			ShieldBubble->AttachTo(BallMeshComp);
			ShieldBubble->SetAbsolute(false, false, true);
			ShieldBubble->SetRelativeScale3D(FVector(1.6f));
		}
	}

	RunGameMode = Cast<AEndlessRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ADiscoBallPawnBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!BallMeshComp || !RunGameMode) return;

	// Autorun
	const float Speed = RunGameMode->CurrentForwardSpeed;
	const float Distance = Speed * DeltaTime;
	FVector Location = BallMeshComp->GetComponentLocation();
	Location.X += Distance;
	BallMeshComp->SetWorldLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);

	// Fake roll
	const float DeltaAngleRad = Distance / BallRadius;
	const FVector RollAxis = FVector(0.f, 1.f, 0.f);
	BallMeshComp->AddWorldRotation(FQuat(RollAxis, DeltaAngleRad));

	if (ShieldBubble) {
		ShieldBubble->SetVisibility(RunGameMode->ShieldActivated);
	}
}

void ADiscoBallPawnBase::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ADiscoBallPawnBase::JumpPressed);
	InputComponent->BindAxis("MoveHorizontal", this, &ADiscoBallPawnBase::MoveHorizontal);
	InputComponent->BindAction("MoveDown", IE_Pressed, this, &ADiscoBallPawnBase::MoveDown);
}

void ADiscoBallPawnBase::JumpPressed() {
	if (!BallMeshComp || !bCanJump || !RunGameMode) return;
	if (RunGameMode->bIsGameOverTransition) return;

	BallMeshComp->AddImpulse(FVector(0.f, 0.f, JumpImpulse));
	bCanJump = false;
}

void ADiscoBallPawnBase::OnBallHit(AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit) {
	bCanJump = true;
}

void ADiscoBallPawnBase::ChangeLaneUpdate(const float Value) {
	if (!BallMeshComp || !RunGameMode || (RunGameMode->LaneSwitchValues.Num() < 3)) return;

	FVector Location = BallMeshComp->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	BallMeshComp->SetWorldLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
}

void ADiscoBallPawnBase::ChangeLaneFinished() {
	CurrentLane = NextLane;
	bIsChangingLane = false;
}

void ADiscoBallPawnBase::MoveDown() {
	if (!BallMeshComp || bCanJump) return;
	BallMeshComp->AddImpulse(FVector(0, 0, MoveDownImpulse));
}

void ADiscoBallPawnBase::MoveHorizontal(float Value) {
	const float DeadZone = 0.5f;
	const bool bInputActive = FMath::Abs(Value) > DeadZone;

	if (RunGameMode->bIsGameOverTransition) return;

	if (bHorizontalInputHeld && bInputActive) return;

	// if the input is released, we rearm the possibility to detect a new input
	if (!bInputActive) {
		bHorizontalInputHeld = false;
		return;
	}

	// input has just been pressed
	bHorizontalInputHeld = true;

	if (bIsChangingLane) return;

	if (Value > 0.f) NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
	else NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);

	//if (NextLane == CurrentLane) return;
	bIsChangingLane = true;
	ChangeLane();
}

FVector ADiscoBallPawnBase::GetBallWorldLocation() const
{
	if (BallMeshComp) return BallMeshComp->GetComponentLocation();
	return GetActorLocation();
}
