// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerHUD.h"
#include "Blueprint/UserWidget.h"

AMultiplayerHUD::AMultiplayerHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetAsset(TEXT("/Game/Blueprints/PlayerUI"));

	if (WidgetAsset.Succeeded())
	{
		PlayerWidgetClass = WidgetAsset.Class;
	}
}

// Called when the game starts or when spawned
void AMultiplayerHUD::BeginPlay()
{
	Super::BeginPlay();

	// If one player game, you can create widget here
}

// Create 2P widget and add to viewport
void AMultiplayerHUD::CreateTwoPlayerUI()
{
	UWorld* const World = GetWorld();

	if (World != nullptr)
	{
		if (PlayerWidgetClass != nullptr)
		{
			PlayerUI = CreateWidget<UUserWidget>(World, PlayerWidgetClass);

			if (PlayerUI != nullptr)
				PlayerUI->AddToViewport();
		}
	}
}
