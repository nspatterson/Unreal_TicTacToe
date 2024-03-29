// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTileClaimedDelegate, ATile*, Tile, const FString, OwnerName);

UCLASS()
class TICTACTOE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	FString GetClaimedBy();

	uint8 GetIndex();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnClaimed"))
	void OnRep_ClaimedBy();
	void OnRep_ClaimedBy_Implementation();

	UFUNCTION()
	void OnPlayerTurnChanged(const FString NewPlayer);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPlayerChanged"))
	void BP_OnPlayerChanged(bool IsUs);
	void BP_OnPlayerChanged_Implementation(bool IsUs);

	//This should be replicated
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ClaimedBy)
	FString ClaimedBy;

	UPROPERTY(EditAnywhere)
	uint8 Index;

public:
	// Set the tile to be owned by the actor
	void ClaimTile(AActor*);

	UFUNCTION(BlueprintCallable)
	bool IsOpen();

	UPROPERTY()
	FTileClaimedDelegate OnTileClaimed;
};
