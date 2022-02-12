// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

void UTicTacToeInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	SessionInterface = Subsystem->GetSessionInterface();

	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UTicTacToeInstanceSubsystem::OnCreateSessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UTicTacToeInstanceSubsystem::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UTicTacToeInstanceSubsystem::OnJoinSessionComplete);
		SessionInterface->OnEndSessionCompleteDelegates.AddUObject(this, &UTicTacToeInstanceSubsystem::OnEndSessionComplete);
	}
}

void UTicTacToeInstanceSubsystem::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings settings;
		settings.bIsLANMatch = true;
		settings.NumPublicConnections = 2;
		settings.bShouldAdvertise = true;
		settings.bUsesPresence = true;
		settings.Set(TEXT("SESSION_NAME"), FString(TEXT("TicTacTonail")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, FName("TicTacTonail"), settings);
	}
}

void UTicTacToeInstanceSubsystem::FindSession()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_DEDICATED_ONLY, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UTicTacToeInstanceSubsystem::EndSession(FName SessionName)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->EndSession(SessionName);
	}
}

void UTicTacToeInstanceSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Session Created"));
}

void UTicTacToeInstanceSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful && SessionSearch.IsValid())
	{
		auto Results = SessionSearch->SearchResults;
		if (Results.Num() > 0)
		{
			FString SessionName;
			Results[0].Session.SessionSettings.Get(TEXT("SESSION_NAME"), SessionName);
			SessionInterface->JoinSession(0, FName(*SessionName), Results[0]);
		}
	}
}

void UTicTacToeInstanceSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
	{
		//Log
		return;
	}

	FString URL;
	if (!SessionInterface->GetResolvedConnectString(SessionName, URL))
	{
		//Log
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	APlayerController* pc = GameInstance->GetFirstLocalPlayerController();

	if (pc)
	{
		pc->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		//Log
	}
}

void UTicTacToeInstanceSubsystem::OnEndSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Session Ended"));
}