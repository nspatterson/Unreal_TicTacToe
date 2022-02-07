// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class TICTACTOE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnClaimed"))
	void BPEvent_OnClaimed(int32 claimer);
	void BPEvent_OnClaimed_Implementation(int32 claimer);*/

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnClaimed"))
	void OnRep_ClaimedBy();
	void OnRep_ClaimedBy_Implementation();

	//This should be replicated
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ClaimedBy)
	FString ClaimedBy;

	UPROPERTY(EditAnywhere)
	uint8 Index;

public:
	// Set the tile to be owned by the actor
	void ClaimTile(AActor*);

	bool IsOpen();
};
