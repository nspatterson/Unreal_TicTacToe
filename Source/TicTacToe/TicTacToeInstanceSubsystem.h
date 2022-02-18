// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "TicTacToeInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TICTACTOE_API UTicTacToeInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	virtual void CreateSession();

	UFUNCTION(BlueprintCallable)
	virtual void FindSession();

	virtual void EndSession(FName SessionName);
};
