// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "TicTacToeGameSession.generated.h"

class FGameLiftServerSDKModule;
struct FProcessParameters;
/**
 * 
 */
UCLASS()
class TICTACTOE_API ATicTacToeGameSession : public AGameSession
{
	GENERATED_BODY()

	ATicTacToeGameSession();

	// Call Process Ready to notify Gamelift
	virtual void RegisterServer() override;

	// Override this so we can verify with AWS that incoming player has reserved session
	// Return error message to reject or empty string to accept
	virtual FString ApproveLogin(const FString& Options) override;
	
	// Override this so we can NotifyLogout for Gamelift
	// Call ProcessEnd to shut down after all players have left
	virtual void UnregisterPlayer(FName InSessionName, const FUniqueNetIdRepl& UniqueId);

	// Override this so we can NotifyLogout for Gamelift
	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer);

	// TODO: Need to create callback method to handle OnTerminate from AWS Gamelift
	// Send players back to main menu with message that server was shutdown

protected:
	FGameLiftServerSDKModule* GameLiftSdkModule;

	FProcessParameters* Params;
};
