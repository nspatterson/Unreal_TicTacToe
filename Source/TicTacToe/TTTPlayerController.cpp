// Fill out your copyright notice in the Description page of Project Settings.


#include "TTTPlayerController.h"
#include "Tile.h"

void ATTTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("click", IE_Pressed, this, &ATTTPlayerController::HandleClick);
	bShowMouseCursor = true;
}

void ATTTPlayerController::ServerClaim_Implementation(ATile* tile)
{
	tile->ClaimTile(this);
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
