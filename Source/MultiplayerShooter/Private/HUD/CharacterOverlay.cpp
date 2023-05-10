// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/MainCharacter.h"
#include "PlayerState/ShooterPlayerState.h"

bool UCharacterOverlay::Initialize()
{
	if (GetWorld())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			AMainCharacter* MainCharacter = Cast<AMainCharacter>(PlayerController->GetCharacter());
			if (MainCharacter)
			{
				MainCharacter->OnHealthChangeDelegate.AddDynamic(this, &ThisClass::OnCharacterHealthChenge);
			}
		}
		AShooterPlayerState* PlayerState = Cast<AShooterPlayerState>(GetOwningPlayerState());
		if (PlayerState)
		{
			PlayerState->OnScoreUpdateDelegate.AddDynamic(this, &ThisClass::OnCharacterScoreChenge);
			PlayerState->OnDefeatsUpdateDelegate.AddDynamic(this, &ThisClass::OnChareacterDefeatsChange);
		}
	}

	return Super::Initialize();
}

void UCharacterOverlay::UpdatePlayerHealth(float Health, float MaxHealth)
{
	HealthBar->SetPercent(Health / MaxHealth);
	const FString Text = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
	HealthText->SetText(FText::FromString(Text));
}

void UCharacterOverlay::DisableDefeatedMessage()
{
	if (DefeatedMsg)
	{
		DefeatedMsg->SetVisibility(ESlateVisibility::Hidden);
		if (IsAnimationPlaying(DefeatedMsgAnim))
		{
			StopAnimation(DefeatedMsgAnim);
		}
	}
}

void UCharacterOverlay::EnableDefeatedMessage()
{
	if (DefeatedMsg)
	{
		DefeatedMsg->SetVisibility(ESlateVisibility::Visible);
		if (DefeatedMsgAnim)
		{
			PlayAnimation(DefeatedMsgAnim);
		}
	}
}

void UCharacterOverlay::OnCharacterHealthChenge(float Health, float MaxHealth)
{
	UpdatePlayerHealth(Health, MaxHealth);
}

void UCharacterOverlay::OnCharacterScoreChenge(float Score)
{
	const FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
	ScoreAmount->SetText(FText::FromString(ScoreText));
}

void UCharacterOverlay::OnChareacterDefeatsChange(int32 Defeats)
{
	const FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
	DefeatsAmount->SetText(FText::FromString(DefeatsText));
}
