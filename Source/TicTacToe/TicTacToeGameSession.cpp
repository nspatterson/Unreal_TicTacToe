// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeGameSession.h"
#include "TicTacToeInstanceSubsystem.h"

void ATicTacToeGameSession::RegisterServer()
{
	UE_LOG(LogTemp, Warning, TEXT("REGISTER MY DUDE"));

	UGameInstance* GameInstance = GetGameInstance();
	UTicTacToeInstanceSubsystem* ss = GameInstance->GetSubsystem<UTicTacToeInstanceSubsystem>();

	if (ss)
	{
		ss->CreateSession();
	}
}
