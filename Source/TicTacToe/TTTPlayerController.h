// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TTTPlayerController.generated.h"

class ATile;
/**
 * 
 */
UCLASS()
class TICTACTOE_API ATTTPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

	UFUNCTION( Server, Reliable)
	void ServerClaim(ATile* tile);
	void ServerClaim_Implementation(ATile* tile);

	void HandleClick();
};
