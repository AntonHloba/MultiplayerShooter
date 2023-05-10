// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreUpdateSignature, float, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDefeatsUpdateSignature, int32, Defeats);

UCLASS()
class MULTIPLAYERSHOOTER_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void UpdateScore();

	void UpdateDefeats();

private:
	UPROPERTY(EditAnywhere)
	float ScoreAmount = 5.f;

	UPROPERTY()
	int32 Defeats = 0;

	//UPROPERTY()
	//class AShooterPlayerController* ShooterPlayerController;

public:

	UPROPERTY(BlueprintAssignable)
	FOnScoreUpdateSignature OnScoreUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDefeatsUpdateSignature OnDefeatsUpdateDelegate;
};
