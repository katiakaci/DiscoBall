// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BP_GameInstance.h"
#include "Sound/SoundClass.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "OptionsMenu.generated.h"

/**
 *
 */
UCLASS()
class ROLLINGBALLGAME_API UOptionsMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void OnMusicButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void OnSoundButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void UpdateIcon(bool bIsOn, bool bIsMusic);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* MusicBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* SoundBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* MusicIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* SoundIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* OnTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* OffTexture;

	virtual void NativeConstruct() override;
};
