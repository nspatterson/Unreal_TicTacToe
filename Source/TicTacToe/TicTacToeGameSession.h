// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "TicTacToeGameSession.generated.h"

/**
 * 
 */
UCLASS()
class TICTACTOE_API ATicTacToeGameSession : public AGameSession
{
	GENERATED_BODY()

	virtual void RegisterServer() override;
	
};
