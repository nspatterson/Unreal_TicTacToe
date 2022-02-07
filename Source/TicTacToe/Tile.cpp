// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "TicTacToeGameModeBase.h"
#include "TTTPlayerController.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "GameFramework/PlayerState.h"

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

//void ATile::OnRep_ClaimedBy()
//{
//	FEditorScriptExecutionGuard ScriptGuard; // Supposed to allow events to fire in editor but doesn't work
//	{
//		//BPEvent_OnClaimed(ClaimedBy);
//	}
//
//	//GEngine->AddOnScreenDebugMessage(1, 2.5f, FColor::Red, TEXT("id: " + ClaimedBy));
//	
//}

void ATile::ClaimTile(AActor* claimer)
{
	if (!IsOpen())
	{
		return;
	}

	ATicTacToeGameModeBase* gm = (ATicTacToeGameModeBase*)GWorld->GetAuthGameMode();

	if (gm)
	{
		// TODO:
		// Send to GameMode for tracking
		gm->SetClicked();
	}

	// Log player network index (some reason all 0)
	ATTTPlayerController* pc = (ATTTPlayerController*)claimer;

	if (pc)
	{
		ClaimedBy = pc->GetPlayerState<APlayerState>()->GetPlayerName();

		APlayerState* ps = pc->GetPlayerState<APlayerState>();

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Claimed by: " + FString::FromInt(pc->Player->GetUniqueID())));
	}
}

bool ATile::IsOpen()
{
	return ClaimedBy.IsEmpty();
}

