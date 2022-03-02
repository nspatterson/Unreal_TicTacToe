// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Online/HTTP/Public/Http.h"

void UTicTacToeInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// TODO: If moving AWS to this class, initialize here
}

void UTicTacToeInstanceSubsystem::CreateSession()
{
	// TODO: Call from register server
}

void UTicTacToeInstanceSubsystem::FindSession()
{
	// For reference
	// https://www.davidykay.com/UE4-Hello-Http/?fbclid=IwAR0Ej3bR37vjwOEHIr60izilcGFpR38xSVHZEUNkANxZXhJ7miMntn1tV8s
	auto http = &FHttpModule::Get();
	auto request = http->CreateRequest();	

	request->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr requestptr, FHttpResponsePtr responseptr, bool bWasSuccesssful)
	{
		if(!bWasSuccesssful)
		{
			UE_LOG(LogTemp, Warning, TEXT("Game Request Failed"));
			return;
		}

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
	//request->SetURL("http://localhost:65476/weatherforecast"); // LOCAL URL
	request->SetURL("https://ctwjo8d44f.execute-api.us-east-1.amazonaws.com/dev"); // LIVE URL
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->ProcessRequest();
}

void UTicTacToeInstanceSubsystem::EndSession(FName SessionName)
{
	// TODO: Call when game ends
}