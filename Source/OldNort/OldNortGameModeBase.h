// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"
#include "Motorcycle2.h"
#include "GameFramework/GameModeBase.h"
#include "OldNortGameModeBase.generated.h"

/**
 * 
 */

USTRUCT()
struct FRespawnSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform RespawnTransform_1;

	UPROPERTY()
	FTransform RespawnTransform_2;

	UPROPERTY()
	FTransform RespawnTransform_3;
};
class AMotorcycle;
UCLASS()
class OLDNORT_API AOldNortGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
private:
	
	//Method to Spawn Player two
	void SetupTwoPlayers();
	
public:
	
	//Sets Default values for this	 characters properties
	AOldNortGameModeBase();
protected:
	virtual void BeginPlay() override;

	//Player One Reference
	UPROPERTY()
	AMotorcycle* PlayerOne;

	//Level Reference
	UPROPERTY()
	ALevelScriptActor* LevelActorInstance;

	//
	UPROPERTY(EditAnywhere)
	FString MyLevelName;

	FTimerHandle HUDTimer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMotorcycle2> Player2;

	
public:
	
	//Called every frame
	virtual void Tick(float DeltaSeconds) override;

	//Load UI Method
	void LoadTwoPlayerWidget();

public:
	
	//Struct Reference
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	FRespawnSettings RespawnSetup;

	//Populate world with Respawn locations
	void CreateRespawnPoints();

	UPROPERTY(EditAnywhere)
	float ScoreToWin;
	
	void OnPlayerWon(const int32 PlayerIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnPlayerWon(const int32 PlayerIndex);
};
	
