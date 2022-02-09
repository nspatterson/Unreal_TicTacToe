// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "TTTPlayerController.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "GameFramework/PlayerState.h"
#include "TicTacToeGameModeBase.h"
#include "GameFramework/GameStateBase.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATile, ClaimedBy);
}

void ATile::ClaimTile(AActor* claimer)
{
	if (!GWorld) return;
	if (!IsOpen()) return;

	ATTTPlayerController* pc = (ATTTPlayerController*)claimer;

	if (pc)
	{
		ClaimedBy = pc->GetPlayerState<APlayerState>()->GetPlayerName();

		if (OnTileClaimed.IsBound())
		{
			//FEditorScriptExecutionGuard ScriptGuard;
			OnTileClaimed.Broadcast(this, ClaimedBy);
		}
	}
}

bool ATile::IsOpen()
{
	return ClaimedBy.IsEmpty();
}

FString ATile::GetClaimedBy()
{
	return ClaimedBy;
}