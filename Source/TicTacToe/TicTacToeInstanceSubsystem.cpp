// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Online/HTTP/Public/Http.h"

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
	// For reference
	// https://www.davidykay.com/UE4-Hello-Http/?fbclid=IwAR0Ej3bR37vjwOEHIr60izilcGFpR38xSVHZEUNkANxZXhJ7miMntn1tV8s
	auto http = &FHttpModule::Get();
	auto request = http->CreateRequest();	

	request->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr requestptr, FHttpResponsePtr responseptr, bool bWasSuccesssful)
	{
			// TODO:
			// 1. Create Custom FUniqueNetId class
			// 2. Parse information from web api
			// 3. Set TTTLocalPlayer Name/FUniqueNetId class
			// 4. Remove OnlineSubsession stuff
			// 5. Refactor/structure code to be more organized
			/*if (bWasSuccesssful)
			{
				GEngine->AddOnScreenDebugMessage(1, 2, FColor::Orange, "Success");
				UGameInstance* GameInstance = GetGameInstance();
				APlayerController* pc = GameInstance->GetFirstLocalPlayerController();

				if (pc)
				{
					FString URL = "192.168.0.126:7777?Name=CumLazer?Feck=ohFeck";
					pc->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(1, 2, FColor::Orange, "Fail");
			}*/

			// TODO: Uncommnent later when API side is finished
			// Reference: https://www.davidykay.com/UE4-Hello-Http/?fbclid=IwAR0Ej3bR37vjwOEHIr60izilcGFpR38xSVHZEUNkANxZXhJ7miMntn1tV8s
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(responseptr->GetContentAsString());

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				FString Address = JsonObject->GetStringField("ipAddress");
				FString Port = JsonObject->GetStringField("port");
				FString SessionId = JsonObject->GetStringField("playerSessionId");
				FString URL = (Address == "127.0.0.1" ? "192.168.0.126" : Address) + ":" + Port + "?SessionId=" + SessionId;

				UGameInstance* GameInstance = GetGameInstance();
				APlayerController* pc = GameInstance->GetFirstLocalPlayerController();

				if (pc)
				{
					pc->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(1, 2, FColor::Orange, "No Player Controller");
				}
			}
			else {
				GEngine->AddOnScreenDebugMessage(1, 2, FColor::Orange, "Failed to deserialized");
			}
	});

	request->SetVerb("GET");
	request->SetURL("http://localhost:65476/weatherforecast"); 
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->ProcessRequest();
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