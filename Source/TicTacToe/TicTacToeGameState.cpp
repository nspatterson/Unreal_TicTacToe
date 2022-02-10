// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeGameState.h"
#include "GameFramework/PlayerState.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

ATicTacToeGameState::ATicTacToeGameState()
{
	PlayerTurnName = TEXT("None");
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

	PlayerTurnName = PlayerArray[TurnIndex].Get()->GetPlayerName();
}

void ATicTacToeGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATicTacToeGameState, TurnIndex);
	DOREPLIFETIME(ATicTacToeGameState, MatchState);
	DOREPLIFETIME(ATicTacToeGameState, PlayerTurnName);
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