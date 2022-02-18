// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacToeGameSession.h"
#include "TicTacToeInstanceSubsystem.h"
#include "GameLiftServerSDK.h"
#include "Kismet/GameplayStatics.h"
#include "TTTPlayerController.h"
#include "TicTacToeGameModeBase.h"

//#define WITH_GAMELIFT 1

ATicTacToeGameSession::ATicTacToeGameSession()
{
#if WITH_GAMELIFT
	UE_LOG(LogTemp, Warning, TEXT("Initializing GameLift..."));
	GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	GameLiftSdkModule->InitSDK();

	auto onGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
	{
		GameLiftSdkModule->ActivateGameSession();
	};

	Params = new FProcessParameters();
	Params->OnStartGameSession.BindLambda(onGameSession);

	// TODO: Create a method below to handle termination
	Params->OnTerminate.BindLambda([=]() { OnGameOver(); });

	Params->OnHealthCheck.BindLambda([]() {return true; });

	// Should figure out how to dynamically set this
	// https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-engines-setup-unreal.html
	Params->port = GWorld->URL.Port;
#endif
}

void ATicTacToeGameSession::RegisterServer()
{
#if WITH_GAMELIFT
	// TODO: Consider moving the gamelift dependant stuff into a custom OnlineSubSystem
	UE_LOG(LogTemp, Warning, TEXT("Registering with GameLift..."));

	GameLiftSdkModule->ProcessReady(*Params);
#endif
}

FString ATicTacToeGameSession::ApproveLogin(const FString& Options)
{
	bool bSuccess = true;
	FString Message = TEXT("");

#if WITH_GAMELIFT
	FString SessionId = UGameplayStatics::ParseOption(Options, TEXT("SessionId"));
	FGameLiftGenericOutcome Outcome = GameLiftSdkModule->AcceptPlayerSession(SessionId);
	bSuccess = Outcome.IsSuccess();
	Message = Outcome.GetError().m_errorMessage;
	UE_LOG(LogTemp, Warning, TEXT("Approved login for session: %s"), *SessionId);
#endif

	return bSuccess ? Super::ApproveLogin(Options) : Message;
}

void ATicTacToeGameSession::UnregisterPlayer(FName InSessionName, const FUniqueNetIdRepl& UniqueId)
{
	Super::UnregisterPlayer(InSessionName, UniqueId);
}

void ATicTacToeGameSession::UnregisterPlayer(const APlayerController* ExitingPlayer)
{
#if WITH_GAMELIFT
	auto pc = (ATTTPlayerController*)ExitingPlayer;
	FString SessionId = pc->GetSessionId();
	if (SessionId.Len() > 0)
	{
		GameLiftSdkModule->RemovePlayerSession(SessionId);
		UE_LOG(LogTemp, Warning, TEXT("Removed login for session: %s"), *SessionId);
	}
#endif
	Super::UnregisterPlayer(ExitingPlayer);
}

void ATicTacToeGameSession::BeginPlay()
{
#if WITH_GAMELIFT
	ATicTacToeGameModeBase* gm = (ATicTacToeGameModeBase*)GWorld->GetAuthGameMode();

	if (gm)
	{
		gm->OnGameOver.AddDynamic(this, &ATicTacToeGameSession::OnGameOver);
	}
#endif
}

void ATicTacToeGameSession::OnGameOver()
{
	GameLiftSdkModule->ProcessEnding();
	ReturnToMainMenuHost();

	RequestEngineExit(TEXT("Game is over..."));
}

