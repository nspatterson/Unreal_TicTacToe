// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TicTacToeGameState.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTileClaimedDelegate, ATile*, Tile, const FString, OwnerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerTurnChangedDelegate, const FString, PlayerTurnName);

/**
 * 
 */
UCLASS()
class TICTACTOE_API ATicTacToeGameState : public AGameStateBase
{
	GENERATED_BODY()
	ATicTacToeGameState();

public:

	// Returns the name of the current player
	UFUNCTION(BlueprintCallable)
	FString GetPlayersTurn();

	void SetStaringPlayerIndex(int32 Index);

	void SwitchPlayerTurn();

	virtual bool HasMatchStarted() const override;

	virtual bool HasMatchEnded() const override;

	virtual void SetMatchState(FName NewMatchState);

	virtual FName GetMatchState();

	UPROPERTY()
	FPlayerTurnChangedDelegate OnPlayerTurnChanged;

protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Turn Index"))
	void OnRep_OnTurnIndex();
	void OnRep_OnTurnIndex_Implementation();

	UFUNCTION()
	void OnRep_PlayerTurnName();

	// Player number turn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_OnTurnIndex)
	int32 TurnIndex;

	UPROPERTY(Replicated)
	FName MatchState;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PlayerTurnName)
	FString PlayerTurnName;
};
