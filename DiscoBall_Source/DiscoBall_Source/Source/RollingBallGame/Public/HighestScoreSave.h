// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "HighestScoreSave.generated.h"

/**
 *
 */
UCLASS()
class ROLLINGBALLGAME_API UHighestScoreSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 BestScore = 0;
};
