// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MotorcyclePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class OLDNORT_API AMotorcyclePlayerState : public APlayerState
{
	GENERATED_BODY()
public:

AMotorcyclePlayerState();
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoring")
	int32 TotalScore;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Scoring")
	int WinScore;
};
// Fill out your copyright notice in the Description page of Project Settings.



