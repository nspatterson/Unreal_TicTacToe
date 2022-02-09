// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TicTacToeGameModeBase.generated.h"

class ATile;

namespace TTTMatchState
{
	extern TICTACTOE_API const FName WaitingForPlayers;
	extern TICTACTOE_API const FName WaitingForMove;
	extern TICTACTOE_API const FName CheckingMove;
	extern TICTACTOE_API const FName Abandoned;
	extern TICTACTOE_API const FName MatchEnded;
}

//namespace TTTMatchState
//{
//	const FName WaitingForPlayers = FName(TEXT("WaitingForPlayers"));
//	const FName WaitingForMove = FName(TEXT("WaitingForMove"));
//	const FName CheckingMove = FName(TEXT("CheckingMove"));
//	const FName Abandoned = FName(TEXT("Abandoned"));
//	const FName MatchEnded = FName(TEXT("MatchEnded"));
//}


/**
 * 
 */
UCLASS()
class TICTACTOE_API ATicTacToeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	ATicTacToeGameModeBase();

public:
	virtual bool HasMatchEnded() const override;
	
	virtual bool HasMatchStarted() const override;
	
	virtual void Tick(float DeltaSeconds) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	void SetClicked();

	UFUNCTION()
	virtual void OnTileClaimed(ATile* Tile, const FString OwnerName);


protected:
	virtual void GetTiles();

	virtual void BindTileDelegates();

	virtual void BeginPlay() override;

	virtual FName GetMatchState();

	virtual void SetMatchState(FName NewMatchState);

	virtual bool ShouldStartMatch();

	virtual bool ShouldEndMatch();

	virtual void StartMatch();

	TArray<AActor*> Tiles;

	FName MatchState;

private:
	bool ended;

	int32 TurnCount;

	FString LastPlayer;
};
