// Copyright Epic Games, Inc. All Rights Reserved.


#include "TicTacToeGameModeBase.h"
#include "TTTPlayerController.h"
#include "TicTacToeGameSession.h"
#include "Kismet/GameplayStatics.h"
#include "TicTacToeGameState.h"
#include "Tile.h"

namespace TTTMatchState
{
	const FName WaitingForPlayers = FName(TEXT("WaitingForPlayers"));
	const FName WaitingForMove = FName(TEXT("WaitingForMove"));
	const FName CheckingMove = FName(TEXT("CheckingMove"));
	const FName Abandoned = FName(TEXT("Abandoned"));
	const FName MatchEnded = FName(TEXT("MatchEnded"));
}

bool CheckTiles(ATile* t1, ATile* t2, ATile* t3)
{
	if (t1->IsOpen() || t2->IsOpen() || t3->IsOpen())
	{
		return false;
	}

	return t1->GetClaimedBy() == t2->GetClaimedBy() && t1->GetClaimedBy() == t3->GetClaimedBy();
}

bool CheckRows(TArray<AActor*> * Tiles)
{
	auto tiles = *Tiles;
	for (int32 i = 0; i < 9; i+=3)
	{
		if (CheckTiles((ATile*)tiles[i], (ATile*)tiles[i + 1], (ATile*)tiles[i + 2]))
		{
			return true;
		}
	}

	return false;
}

bool CheckColumns(TArray<AActor*> * Tiles)
{
	auto tiles = *Tiles;
	for (int32 i = 0; i < 3; ++i)
	{
		if (CheckTiles((ATile*)tiles[i], (ATile*)tiles[i + 3], (ATile*)tiles[i + 6]))
		{
			return true;
		}
	}

	return false;
}

bool CheckCross(TArray<AActor*>* Tiles)
{
	auto tiles = *Tiles;
	if (CheckTiles((ATile*)tiles[0], (ATile*)tiles[4], (ATile*)tiles[8]))
	{
		return true;
	}

	if (CheckTiles((ATile*)tiles[2], (ATile*)tiles[4], (ATile*)tiles[6]))
	{
		return true;
	}

	return false;
}

bool CheckBoard(TArray<AActor*>* Tiles)
{
	if (CheckRows(Tiles))
	{
		return true;
	}

	if (CheckColumns(Tiles))
	{
		return true;
	}

	if (CheckCross(Tiles))
	{
		return true;
	}

	return false;
}

ATicTacToeGameModeBase::ATicTacToeGameModeBase() : AGameModeBase()
{
	PlayerControllerClass = ATTTPlayerController::StaticClass();
	GameStateClass = ATicTacToeGameState::StaticClass();
	GameSessionClass = ATicTacToeGameSession::StaticClass();

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	MatchState = TTTMatchState::WaitingForPlayers;
	TurnCount = 0;
	bEnded = false;
}

bool ATicTacToeGameModeBase::HasMatchEnded() const
{
	return MatchState == TTTMatchState::Abandoned || MatchState == TTTMatchState::MatchEnded;
}

bool ATicTacToeGameModeBase::HasMatchStarted() const
{
	return MatchState != TTTMatchState::WaitingForPlayers;// && MatchState != TTTMatchState::Abandoned && MatchState != TTTMatchState::MatchEnded;
}

bool ATicTacToeGameModeBase::ShouldStartMatch()
{		
	return GetNumPlayers() == 2;
}

bool ATicTacToeGameModeBase::ShouldEndMatch()
{
	if (CheckBoard(&Tiles))
	{
		//GEngine->AddOnScreenDebugMessage(1, 5, FColor::Green, LastPlayer + TEXT(" WINNER"));
		((ATicTacToeGameState*)GameState)->SetMatchResult(Winner);
		return true;
	}

	if (TurnCount == 9)
	{
		//GEngine->AddOnScreenDebugMessage(1, 5, FColor::Green, TEXT("CATS!! MEOW MEOW"));
		((ATicTacToeGameState*)GameState)->SetMatchResult(Cats);
		return true;
	}
	return false;
}

void ATicTacToeGameModeBase::StartMatch()
{
	ATicTacToeGameState* gs = (ATicTacToeGameState*)GameState;
	gs->SetStaringPlayerIndex(FMath::RandRange(0, 1));

	GameSession->HandleMatchHasStarted();
	SetMatchState(TTTMatchState::WaitingForMove);
}

void ATicTacToeGameModeBase::Tick(float DeltaSeconds)
{
	if (HasMatchStarted())
	{
		if (HasMatchEnded())
		{
			if (!PostGameTimer.IsValid())
			{
				GetWorldTimerManager().SetTimer(PostGameTimer, this, &ATicTacToeGameModeBase::RemovePlayersAfterGameEnd, 5);
			}

			return;
		}

		if (ShouldEndMatch())
		{
			GameSession->HandleMatchHasEnded();
			SetMatchState(TTTMatchState::MatchEnded);
		}
	}
	else
	{
		if (ShouldStartMatch())
		{
			StartMatch();
		}
	}
}

void ATicTacToeGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	//GEngine->AddOnScreenDebugMessage(3, 3, FColor::Purple, Options);
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	if (GetNumPlayers() == 2) {
		ErrorMessage = TEXT("Room is full NERD!!!!");
	}
}

void ATicTacToeGameModeBase::Logout(AController* Exiting)
{
	if (HasMatchStarted() && !HasMatchEnded())
	{
		((ATicTacToeGameState*)GameState)->SetMatchResult(Abandoned);
		SetMatchState(TTTMatchState::Abandoned);
	}

	Super::Logout(Exiting);
}

APlayerController* ATicTacToeGameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	ATTTPlayerController* pc = (ATTTPlayerController*)Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	if (pc && UGameplayStatics::HasOption(Options, TEXT("SessionId")))
	{
		pc->SetSessionId(UGameplayStatics::ParseOption(Options, TEXT("SessionId")));
	}
	return pc;
}

void ATicTacToeGameModeBase::SetClicked()
{
	bEnded = true;
}

void ATicTacToeGameModeBase::OnTileClaimed(ATile* Tile, const FString OwnerName)
{
	TurnCount++;
	LastPlayer = OwnerName;

	((ATicTacToeGameState*)GameState)->SwitchPlayerTurn();
}

void ATicTacToeGameModeBase::GetTiles()
{
	UGameplayStatics::GetAllActorsOfClass(GWorld, ATile::StaticClass(), Tiles);
	Tiles.Sort([](const AActor& LHS, const AActor& RHS)
	{
		return ((ATile&)LHS).GetIndex() > ((ATile&)RHS).GetIndex();
	});
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

FName ATicTacToeGameModeBase::GetMatchState()
{
	return MatchState;
}

void ATicTacToeGameModeBase::SetMatchState(FName NewMatchState)
{
	MatchState = NewMatchState;
	((ATicTacToeGameState*)GameState)->SetMatchState(MatchState);
}

void ATicTacToeGameModeBase::RemovePlayersAfterGameEnd()
{
	GameSession->ReturnToMainMenuHost();
}