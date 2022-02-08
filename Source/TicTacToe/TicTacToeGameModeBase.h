// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TicTacToeGameModeBase.generated.h"

class ATile;

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

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	void SetClicked();

	UFUNCTION()
	virtual void OnTileClaimed(ATile* Tile, const FString OwnerName);


protected:
	virtual void GetTiles();

	virtual void BindTileDelegates();

	virtual void BeginPlay() override;

	TArray<AActor*> Tiles;

private:
	bool ended;
};
