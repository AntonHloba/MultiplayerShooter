// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShooterGameMode.h"
#include "Character/MainCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "GameState/ShooterGameState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/ShooterPlayerController.h"
#include "PlayerState/ShooterPlayerState.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"


DEFINE_LOG_CATEGORY(LogShooterGameMode);

namespace MatchState
{
	const FName Cooldown = FName(TEXT("Cooldown"));
}

AShooterGameMode::AShooterGameMode()
{
	bDelayedStart = true;
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		OnlineSessionInterface = Subsystem->GetSessionInterface();
	}
	UE_LOG(LogShooterGameMode, Warning, TEXT("ShooterGameMode created!"));
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogShooterGameMode, Warning, TEXT("ShooterGameMode BeginPlay!"));
	CreateSession();
	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void AShooterGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f) StartMatch();
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f) SetMatchState(MatchState::Cooldown);
	}
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = WarmupTime + MatchTime + CooldownTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f) RestartGame();
	}
}

void AShooterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(*It))
		{
			ShooterPlayerController->OnMatchStateSet(MatchState);
		}
	}
}

void AShooterGameMode::PlayerEliminated(AMainCharacter* EliminatedCharacter, AShooterPlayerController* VictimController, AShooterPlayerController* AttackerController)
{
	if (!EliminatedCharacter || !AttackerController || !VictimController) return;

	AShooterPlayerState* AttackerPlayerState = AttackerController->GetPlayerState<AShooterPlayerState>();
	AShooterPlayerState* VictimPlayerState = VictimController->GetPlayerState<AShooterPlayerState>();
	if (!AttackerPlayerState || !VictimPlayerState) return;

	// Need to check if it's suicide.
	if (AttackerPlayerState != VictimPlayerState)
	{
		AttackerPlayerState->UpdateScore();
		VictimPlayerState->UpdateDefeats();
	}
	EliminatedCharacter->Eliminated();

	AShooterGameState* ShooterGameState = GetGameState<AShooterGameState>();
	if (!ShooterGameState) return;

	// Update GameState Info
	ShooterGameState->UpdateTopScorePlayerStates(AttackerPlayerState);
}

void AShooterGameMode::RequestRespawn(AMainCharacter* EliminatedCharacter, AController* EliminatedController)
{
	// Detach character from the controller and destroy.
	if (!EliminatedCharacter) return;
	EliminatedCharacter->Reset();
	EliminatedCharacter->Destroy();

	// Respawn a new character with a random reborn-spot for the controller.
	if (!EliminatedController) return;
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	const int32 PlayerStartIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);

	RestartPlayerAtPlayerStart(EliminatedController, PlayerStarts[PlayerStartIndex]);
}

void AShooterGameMode::CreateSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}

	if (OnlineSessionInterface->GetNamedSession(NAME_GameSession))
	{
		OnlineSessionInterface->DestroySession(NAME_GameSession);
		UE_LOG(LogShooterGameMode, Warning, TEXT("GameSession destroyed!"));
		return;
	}

	int32 NumPublicConnections = 2;
	FString MatchType = TEXT("FreeForAll");

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->BuildUniqueId = 1;

	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	if (OnlineSessionInterface->CreateSession(0, NAME_GameSession, *LastSessionSettings))
	{
		UE_LOG(LogShooterGameMode, Warning, TEXT("GameSession created!"));
	}
}

