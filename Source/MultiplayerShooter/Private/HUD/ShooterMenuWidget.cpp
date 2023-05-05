// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/ShooterMenuWidget.h"
#include "Components/Button.h"
#include "PlayerController/ShooterMenuPlayerController.h"

void UShooterMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnJoinButtonClicked);
	}
}

void UShooterMenuWidget::OnJoinButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		AShooterMenuPlayerController* PlayerController = Cast<AShooterMenuPlayerController>(World->GetFirstPlayerController());
		if (PlayerController)
		{
			PlayerController->TryFindSessionAndJoin();
		}
	}
}
