// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MainMenuGameMode.generated.h"

/**
 *
 */
UCLASS()
class ROLLINGBALLGAME_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	virtual void BeginPlay() override;

};
