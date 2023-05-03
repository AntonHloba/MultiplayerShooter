// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterMenuWidget.generated.h"

/**
 * 
 */

class UButton;

UCLASS()
class MULTIPLAYERSHOOTER_API UShooterMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

private:

	UFUNCTION()
	void OnJoinButtonClicked();

protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* JoinButton;

};
