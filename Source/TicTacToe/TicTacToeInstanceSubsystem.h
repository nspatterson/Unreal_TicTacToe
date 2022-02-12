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

protected:
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	virtual void OnFindSessionsComplete(bool bWasSuccessful);

	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	virtual void OnEndSessionComplete(FName SessionName, bool bWasSuccessful);

protected:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
};
