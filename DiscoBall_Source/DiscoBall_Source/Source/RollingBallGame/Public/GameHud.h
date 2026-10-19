// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "EndlessRunnerGameMode.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "GameHud.generated.h"

/**
 *
 */
UCLASS()
class ROLLINGBALLGAME_API UGameHud : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Life")
	TArray<UTexture2D*> LifeTextures;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* X2ProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* MagnetProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ShieldProgressBar;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void UpdateScore(int32 NewScore);

	UFUNCTION(BlueprintCallable, Category = "Life")
	void UpdateLife(int32 RemainingLives);

	UFUNCTION(BlueprintCallable, Category = "Score")
	void UpdateBestScore(int32 NewBestScore);

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void SetPauseButtonVisible(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "Game Over")
	void ShowGameOverMenu();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ShowPlusFive(EDiscoColor CurrentActiveColor);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> GameOverWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PointsCount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* LifeBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PauseButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* HighestScore;

	UFUNCTION()
	void OnPauseClick();

	virtual void NativeConstruct() override;
};
