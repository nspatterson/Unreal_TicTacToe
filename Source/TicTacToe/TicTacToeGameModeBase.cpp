// Copyright Epic Games, Inc. All Rights Reserved.


#include "TicTacToeGameModeBase.h"
#include "TTTPlayerController.h"

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
