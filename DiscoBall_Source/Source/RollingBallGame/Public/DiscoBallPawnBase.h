// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"
#include "DiscoBallPawnBase.generated.h"

class UStaticMeshComponent;

UCLASS()
class ROLLINGBALLGAME_API ADiscoBallPawnBase : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleInstanceOnly)
	class AEndlessRunnerGameMode* RunGameMode;

public:
	ADiscoBallPawnBase();
	virtual void Tick(float DeltaSeconds) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
	UStaticMeshComponent* ShieldBubble;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 CurrentLane = 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 NextLane = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fake roll")
	float BallRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fake roll")
	FVector RollAxis = FVector(0.f, 1.f, 0.f);

	UFUNCTION(BlueprintImplementableEvent, Category = "Lane")
	void ChangeLane();

	UFUNCTION(BlueprintCallable, Category = "Lane")
	void ChangeLaneUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category = "Lane")
	void ChangeLaneFinished();

	UFUNCTION(BlueprintPure, Category = "Runner")
	FVector GetBallWorldLocation() const;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	void JumpPressed();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

	UPROPERTY()
	UUserWidget* PauseMenuWidget = nullptr;

	UPROPERTY(Transient)
	UStaticMeshComponent* BallMeshComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	float RollTorque = 50000000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	float JumpImpulse = 350000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	float MoveDownImpulse = -600000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball")
	bool bCanJump = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lane")
	bool bIsChangingLane = false;

	UPROPERTY(VisibleAnywhere, Category = "Lane")
	bool bHorizontalInputHeld = false;

	UFUNCTION()
	void MoveHorizontal(float Value);

	UFUNCTION()
	void MoveDown();

	UFUNCTION()
	void OnBallHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
