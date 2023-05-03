// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterMenuHUD.generated.h"

/**
 * 
 */

UCLASS()
class MULTIPLAYERSHOOTER_API AShooterMenuHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

public:

	void RemoveMenuWidgetClass();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;

private:

	UUserWidget* MenuWidget;
};
