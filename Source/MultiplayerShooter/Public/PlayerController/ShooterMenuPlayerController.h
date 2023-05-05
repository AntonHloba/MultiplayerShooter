// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "ShooterMenuPlayerController.generated.h"

/**
 * 
 */

class AShooterMenuHUD;

UCLASS()
class MULTIPLAYERSHOOTER_API AShooterMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AShooterMenuPlayerController();

	void TryFindSessionAndJoin();

protected:

	virtual void BeginPlay() override;

private:

	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void FindGameSession();
	void JoinToGameSession();
	void ShutOffMenu();

private:

	UPROPERTY()
	AShooterMenuHUD* ShooterMenuHUD;

	IOnlineSessionPtr OnlineSessionInterface;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

};
