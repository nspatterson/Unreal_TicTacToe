// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TicTacToeGameState.generated.h"

UENUM()
enum EMatchResult
{
	None		UMETA(DisplayName = "None"),
	Winner		UMETA(DisplayName = "Winning"),
	Cats		UMETA(DisplayName = "Cats"),
	Abandoned	UMETA(DisplayName = "Abandoned")
};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTileClaimedDelegate, ATile*, Tile, const FString, OwnerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerTurnChangedDelegate, const FString, PlayerTurnName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMatchResultDelegate, const TEnumAsByte<EMatchResult>&, MatchResult);

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

	virtual void SetMatchResult(TEnumAsByte<EMatchResult> Result);

	virtual TEnumAsByte<EMatchResult> GetMatchResult();

	UFUNCTION(BlueprintCallable)
	virtual FString GetWinner();

	UPROPERTY(BlueprintAssignable)
	FPlayerTurnChangedDelegate OnPlayerTurnChanged;

	UPROPERTY(BlueprintAssignable)
	FMatchResultDelegate OnMatchResult;

protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// i thihnk also change to delegate
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Turn Index"))
	void OnRep_OnTurnIndex();
	void OnRep_OnTurnIndex_Implementation();

	UFUNCTION()
	void OnRep_PlayerTurnName();

	UFUNCTION()
	void OnRep_MatchResult();

	// Player number turn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_OnTurnIndex)
	int32 TurnIndex;

	UPROPERTY(Replicated)
	FName MatchState;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PlayerTurnName)
	FString PlayerTurnName;

	UPROPERTY(Replicated)
	FString LastPlayerTurn;

	UPROPERTY(ReplicatedUsing = OnRep_MatchResult)
	TEnumAsByte<EMatchResult> MatchResult;
};
