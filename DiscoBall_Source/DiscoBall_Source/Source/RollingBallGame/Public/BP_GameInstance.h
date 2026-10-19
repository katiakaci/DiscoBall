// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "BP_GameInstance.generated.h"

/**
 *
 */
UCLASS()
class ROLLINGBALLGAME_API UBP_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool MusicEnabled = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool SoundEnabled = true;
};
