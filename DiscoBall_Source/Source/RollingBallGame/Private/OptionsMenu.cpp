// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingBallGame.h"
#include "OptionsMenu.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Components/Button.h"

void UOptionsMenu::NativeConstruct() {
	Super::NativeConstruct();

	if (MusicBtn) MusicBtn->OnClicked.AddDynamic(this, &UOptionsMenu::OnMusicButtonClicked);
	if (SoundBtn) SoundBtn->OnClicked.AddDynamic(this, &UOptionsMenu::OnSoundButtonClicked);

	UBP_GameInstance* MyGI = Cast<UBP_GameInstance>(GetWorld()->GetGameInstance());
	if (MyGI) {
		UpdateIcon(MyGI->MusicEnabled, true);
		UpdateIcon(MyGI->SoundEnabled, false);
	}
}

void UOptionsMenu::OnMusicButtonClicked() {
	UBP_GameInstance* MyGI = Cast<UBP_GameInstance>(GetWorld()->GetGameInstance());
	if (!MyGI) return;

	MyGI->MusicEnabled = !MyGI->MusicEnabled;
	UpdateIcon(MyGI->MusicEnabled, true);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Music: %s"), MyGI->MusicEnabled ? TEXT("ON") : TEXT("OFF")));
}

void UOptionsMenu::OnSoundButtonClicked() {
	UBP_GameInstance* MyGI = Cast<UBP_GameInstance>(GetWorld()->GetGameInstance());
	if (!MyGI) return;

	MyGI->SoundEnabled = !MyGI->SoundEnabled;
	UpdateIcon(MyGI->SoundEnabled, false);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Sound: %s"), MyGI->SoundEnabled ? TEXT("ON") : TEXT("OFF")));
}

void UOptionsMenu::UpdateIcon(bool bIsOn, bool bIsMusic)
{
	if (bIsMusic) {
		if (!MusicIcon) return;
		FSlateBrush Brush;
		Brush.SetResourceObject(bIsOn ? OnTexture : OffTexture);
		MusicIcon->SetBrush(Brush);
	}
	else {
		if (!SoundIcon) return;
		FSlateBrush Brush;
		Brush.SetResourceObject(bIsOn ? OnTexture : OffTexture);
		SoundIcon->SetBrush(Brush);
	}
}