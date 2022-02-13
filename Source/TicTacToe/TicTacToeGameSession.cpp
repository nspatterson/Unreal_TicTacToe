// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeGameSession.h"
#include "TicTacToeInstanceSubsystem.h"
#include "GameLiftServerSDK.h"

ATicTacToeGameSession::ATicTacToeGameSession()
{
#if WITH_GAMELIFT
	GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	GameLiftSdkModule->InitSDK();

	auto onGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
	{
		GameLiftSdkModule->ActivateGameSession();
	};

	Params = new FProcessParameters();
	Params->OnStartGameSession.BindLambda(onGameSession);

	// TODO: Create a method below to handle termination
	Params->OnTerminate.BindLambda([=]() {GameLiftSdkModule->ProcessEnding(); });

	Params->OnHealthCheck.BindLambda([]() {return true; });

	// Should figure out how to dynamically set this
	// https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-engines-setup-unreal.html
	Params->port = 7777;
#endif
}

void ATicTacToeGameSession::RegisterServer()
{
	UE_LOG(LogTemp, Warning, TEXT("REGISTER MY DUDE"));

#if WITH_GAMELIFT
	// TODO: Consider moving the gamelift dependant stuff into a custom OnlineSubSystem
	GameLiftSdkModule->ProcessReady(*Params);
#endif

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
