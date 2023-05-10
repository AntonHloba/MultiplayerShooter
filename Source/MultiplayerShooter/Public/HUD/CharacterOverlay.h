// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	void UpdatePlayerHealth(float Health, float MaxHealth);
	void DisableDefeatedMessage();
	void EnableDefeatedMessage();
private:

	UFUNCTION()
	void OnCharacterHealthChenge(float Health, float MaxHealth);
	
	UFUNCTION()
	void OnCharacterScoreChenge(float Score);

	UFUNCTION()
	void OnChareacterDefeatsChange(int32 Defeats);

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CarriedAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponType;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MatchCountdown;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TopScorePlayer;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TopScore;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* TimeBlink;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GrenadeAmount;

private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreAmount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DefeatsAmount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DefeatedMsg;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* DefeatedMsgAnim;


};
