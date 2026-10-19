// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameOverMenu.generated.h"

/**
 *
 */
UCLASS()
class ROLLINGBALLGAME_API UGameOverMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Game Over")
	void OnRestartClick();

	UFUNCTION(BlueprintCallable, Category = "Game Over")
	void OnExitClick();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PointsCount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* RestartBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ExitBtn;

	virtual void NativeConstruct() override;
};
