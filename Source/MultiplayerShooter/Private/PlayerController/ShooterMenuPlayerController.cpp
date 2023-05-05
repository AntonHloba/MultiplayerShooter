// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ShooterMenuPlayerController.h"
#include "HUD/ShooterMenuHUD.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

AShooterMenuPlayerController::AShooterMenuPlayerController()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		OnlineSessionInterface = Subsystem->GetSessionInterface();
	}

	FindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete);
	JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete);
}

void AShooterMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ShooterMenuHUD = Cast<AShooterMenuHUD>(GetHUD());
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

void AShooterMenuPlayerController::ShutOffMenu()
{
	if (ShooterMenuHUD)
	{
		ShooterMenuHUD->RemoveMenuWidgetClass();
	}

	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}

void AShooterMenuPlayerController::TryFindSessionAndJoin()
{
	FindGameSession();
	//TODO: add bool to return and check;
}

void AShooterMenuPlayerController::FindGameSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSessionInterface is not valid!"));
		return;
	}

	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void AShooterMenuPlayerController::JoinToGameSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid OnlineSessionInterface!"));
		return;
	}
	// TODO: add class property SearchResults and move duplicate loop to FindSessions
	for (auto Result : SessionSearch->SearchResults)
	{
		FString MatchType;
		Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);

		if (MatchType == FString("FreeForAll"))
		{
			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

			const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
		}
	}
}

void AShooterMenuPlayerController::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sessions is not finded!"));
		return;
	}

	for (auto Result : SessionSearch->SearchResults)
	{
		FString Id = Result.GetSessionIdStr();
		FString User = Result.Session.OwningUserName;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("ID: %s, USER: %s"), *Id, *User));
			UE_LOG(LogTemp, Display, TEXT("ID: %s, USER: %s"), *Id, *User);
		}
	}

	ShutOffMenu();
	JoinToGameSession();

}

void AShooterMenuPlayerController::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid OnlineSessionInterface!"));
		return;
	}

	FString Address;
	if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Connect Address: %s"), *Address));
			UE_LOG(LogTemp, Display, TEXT("Connect Address: %s"), *Address);
		}

		ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

