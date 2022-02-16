// Fill out your copyright notice in the Description page of Project Settings.


#include "TTTPlayerController.h"
#include "TicTacToeGameState.h"
#include "GameFramework/PlayerState.h"
#include "Tile.h"

void ATTTPlayerController::SetSessionId(const FString& NewSessionId)
{
	SessionId = NewSessionId;
}

FString ATTTPlayerController::GetSessionId() const
{
	return SessionId;
}

void ATTTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("click", IE_Pressed, this, &ATTTPlayerController::HandleClick);
	bShowMouseCursor = true;
}

void ATTTPlayerController::ServerClaim_Implementation(ATile* tile)
{
	ATicTacToeGameState* gs = (ATicTacToeGameState*)GWorld->GetGameState();
	if (gs->HasMatchStarted() && PlayerState->GetPlayerName() == gs->GetPlayersTurn())
	{
		tile->ClaimTile(this);
	}
}

void ATTTPlayerController::HandleClick()
{
	FHitResult hr;

	if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hr))
	{
		ATile* tile = (ATile*)hr.GetActor();
		if (tile)
		{
			if (HasAuthority() && tile->IsOpen())
			{
				tile->ClaimTile(this);
			}
			else
			{
				ServerClaim(tile);
			}
		}
	}
}
