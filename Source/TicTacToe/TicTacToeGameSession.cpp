// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeGameSession.h"
#include "TicTacToeInstanceSubsystem.h"

void ATicTacToeGameSession::RegisterServer()
{
	UE_LOG(LogTemp, Warning, TEXT("REGISTER MY DUDE"));
	// TODO: Call ProcessReady in AWS SDK to notify Gamelift server is ready to accept connections
	// Not if we need to register with online subsystm (I don't think so though)

	UGameInstance* GameInstance = GetGameInstance();
	UTicTacToeInstanceSubsystem* ss = GameInstance->GetSubsystem<UTicTacToeInstanceSubsystem>();

	if (ss)
	{
		ss->CreateSession();
	}
}

FString ATicTacToeGameSession::ApproveLogin(const FString& Options)
{
	// TODO: Implement checking with AWS SDK
	return Super::ApproveLogin(Options);
}

void ATicTacToeGameSession::UnregisterPlayer(FName InSessionName, const FUniqueNetIdRepl& UniqueId)
{
	// TODO: RemoveSession with AWS

	// TODO: If GameSession->GetPlayerNum() == 0 EndProcess
	Super::UnregisterPlayer(InSessionName, UniqueId);
}
