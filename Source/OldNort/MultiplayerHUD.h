// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MultiplayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class OLDNORT_API AMultiplayerHUD : public AHUD
{
	GENERATED_BODY()
		
public:

	AMultiplayerHUD();
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Widget Blueprint Class
	TSubclassOf<class UUserWidget> PlayerWidgetClass;

	// Point to Widget Class
	UPROPERTY(BlueprintReadOnly)
	class UUserWidget* PlayerUI;

	// Widget Method
	void CreateTwoPlayerUI();
	
};
