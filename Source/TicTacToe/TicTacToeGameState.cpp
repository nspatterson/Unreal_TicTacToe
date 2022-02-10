// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeGameState.h"
#include "GameFramework/PlayerState.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

ATicTacToeGameState::ATicTacToeGameState()
{
	PlayerTurnName = TEXT("None");
	MatchResult = None;
}

FString ATicTacToeGameState::GetPlayersTurn()
{
	return PlayerTurnName;
}

void ATicTacToeGameState::SetStaringPlayerIndex(int32 Index)
{
	TurnIndex = FMath::Clamp(Index, 0, 1);
	PlayerTurnName = PlayerArray[TurnIndex].Get()->GetPlayerName();
	
}

void ATicTacToeGameState::SwitchPlayerTurn()
{
	TurnIndex++;

	if (TurnIndex >= PlayerArray.Num())
	{
		TurnIndex = 0;
	}

	LastPlayerTurn = PlayerTurnName;
	PlayerTurnName = PlayerArray[TurnIndex].Get()->GetPlayerName();
}

void ATicTacToeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATicTacToeGameState, TurnIndex);
	DOREPLIFETIME(ATicTacToeGameState, MatchState);
	DOREPLIFETIME(ATicTacToeGameState, PlayerTurnName);
	DOREPLIFETIME(ATicTacToeGameState, LastPlayerTurn);
	DOREPLIFETIME(ATicTacToeGameState, MatchResult);
}

bool ATicTacToeGameState::HasMatchStarted() const
{
	return MatchState != FName("WaitingForPlayers") && MatchState != FName("Abandoned") && MatchState != FName("MatchEnded");
}

bool ATicTacToeGameState::HasMatchEnded() const
{
	return MatchState == FName("Abandoned") || MatchState == ("MatchEnded");
}

void ATicTacToeGameState::SetMatchState(FName NewMatchState)
{
	MatchState = NewMatchState;
}

FName ATicTacToeGameState::GetMatchState()
{
	return MatchState;
}

void ATicTacToeGameState::OnRep_PlayerTurnName()
{
	if (OnPlayerTurnChanged.IsBound())
	{
		OnPlayerTurnChanged.Broadcast(PlayerTurnName);
	}
}

void ATicTacToeGameState::SetMatchResult(TEnumAsByte<EMatchResult> Result)
{
	MatchResult = Result;
}

TEnumAsByte<EMatchResult> ATicTacToeGameState::GetMatchResult()
{
	return MatchResult;
}

FString ATicTacToeGameState::GetWinner()
{
	return LastPlayerTurn;
}

void ATicTacToeGameState::OnRep_MatchResult()
{
	if (OnMatchResult.IsBound())
	{
		OnMatchResult.Broadcast(MatchResult);
	}
}