// Copyright Epic Games, Inc. All Rights Reserved.


#include "TicTacToeGameModeBase.h"
#include "TTTPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Tile.h"

ATicTacToeGameModeBase::ATicTacToeGameModeBase() : AGameModeBase()
{
	PlayerControllerClass = ATTTPlayerController::StaticClass();

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	ended = false;
}

bool ATicTacToeGameModeBase::HasMatchEnded() const
{
	return ended;
}

bool ATicTacToeGameModeBase::HasMatchStarted() const
{
	if (ended)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Game ended"));
	}

	return !ended;
}

void ATicTacToeGameModeBase::Tick(float DeltaSeconds)
{
	if (HasMatchStarted())
	{
		HasMatchEnded();
	}
}

APlayerController* ATicTacToeGameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Hello Player " + UniqueId->ToString()));
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

void ATicTacToeGameModeBase::SetClicked()
{
	ended = true;
}

void ATicTacToeGameModeBase::OnTileClaimed(ATile* Tile, const FString OwnerName)
{
	GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, OwnerName);
}

void ATicTacToeGameModeBase::GetTiles()
{
	UGameplayStatics::GetAllActorsOfClass(GWorld, ATile::StaticClass(), Tiles);
}

void ATicTacToeGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetTiles();
	BindTileDelegates();
}

void ATicTacToeGameModeBase::BindTileDelegates()
{
	for (auto It = Tiles.CreateIterator(); It; ++It)
	{
		ATile* tile = (ATile*)*It;
		tile->OnTileClaimed.AddDynamic(this, &ATicTacToeGameModeBase::OnTileClaimed);
	}
}