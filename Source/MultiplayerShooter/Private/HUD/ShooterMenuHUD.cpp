// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ShooterMenuHUD.h"
#include "Blueprint/UserWidget.h"

void AShooterMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
		}
	}
}

void AShooterMenuHUD::RemoveMenuWidgetClass()
{
	MenuWidget->RemoveFromParent();
}
