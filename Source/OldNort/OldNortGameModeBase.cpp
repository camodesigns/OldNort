// Copyright Epic Games, Inc. All Rights Reserved.


#include "OldNortGameModeBase.h"
#include "EngineUtils.h"
#include "Motorcycle.h"
#include "Motorcycle2.h"
#include "MultiplayerHUD.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

AOldNortGameModeBase::AOldNortGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	const FVector NewLocation1(-12943.852341f, -1905.873145f, 93.16f);
	const FVector NewLocation2(11212.004414f, -1905.873145f, 93.16f);
	const FVector NewLocation3(4116.707179f, -14970.95555f, 93.16f);
	const FRotator NewRotation1(0.0, 0.0f, 0.0f);
	const FRotator NewRotation2(0.0, 180.0f, 0.0f);
	const FRotator NewRotation3(0.0, 90.0f, 0.0f);
	RespawnSetup.RespawnTransform_1 = FTransform(NewRotation1, NewLocation1);
	RespawnSetup.RespawnTransform_2 = FTransform(NewRotation2, NewLocation2);
	RespawnSetup.RespawnTransform_3 = FTransform(NewRotation3, NewLocation3);

	DefaultPawnClass = AMotorcycle::StaticClass();
	PlayerStateClass = APlayerState::StaticClass();
	HUDClass = AMultiplayerHUD::StaticClass();

	PlayerOne = nullptr;
}

#pragma region Setup Logic

void AOldNortGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerOne = Cast<AMotorcycle>(UGameplayStatics::GetActorOfClass(this, AMotorcycle::StaticClass()));
	if (PlayerOne)
	{
		CreateRespawnPoints();
		PlayerOne->bIsTwoPlayerGame = true;
	}

	SetupTwoPlayers();
	GetWorld()->GetTimerManager().SetTimer(HUDTimer, this, &AOldNortGameModeBase::LoadTwoPlayerWidget, true);
}

void AOldNortGameModeBase::CreateRespawnPoints()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		ATargetPoint* FirstTargetpoint = World->SpawnActor<ATargetPoint>(
			ATargetPoint::StaticClass(), RespawnSetup.RespawnTransform_1, SpawnParams);
		if (FirstTargetpoint != nullptr)
		{
			FirstTargetpoint->Tags.Add(FName(TEXT("RespawnOne")));
		}
		ATargetPoint* SecondTargetPoint = World->SpawnActor<ATargetPoint>(
			ATargetPoint::StaticClass(), RespawnSetup.RespawnTransform_2, SpawnParams);
		if (SecondTargetPoint != nullptr)
		{
			SecondTargetPoint->Tags.Add(FName(TEXT("RespawnTwo")));
		}
		ATargetPoint* ThirdTargetPoint = World->SpawnActor<ATargetPoint>(
			ATargetPoint::StaticClass(), RespawnSetup.RespawnTransform_3, SpawnParams);
		if (ThirdTargetPoint != nullptr)
		{
			ThirdTargetPoint->Tags.Add(FName(TEXT("Respawnthree")));
		}
	}
}

void AOldNortGameModeBase::OnPlayerWon(const int32 PlayerIndex)
{
	// Tell players that the game is over
	// Show win/loss widget
	BP_OnPlayerWon(PlayerIndex);
	AMotorcycle* Player1=Cast<AMotorcycle>(UGameplayStatics::GetPlayerPawn(this,0));
	Player1->DisablePlayer();
	AMotorcycle2* PlayerTwo=Cast<AMotorcycle2>(UGameplayStatics::GetPlayerPawn(this,1));
	PlayerTwo->DisablePlayer();
}

#pragma endregion
void AOldNortGameModeBase::Tick(float DeltaTime)
{
}

#pragma region Player/UI Logic
// create a new APlayerController, unposses it, spawn two players, and then have player two posses the  created a playerController
void AOldNortGameModeBase::SetupTwoPlayers()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		if (PlayerOne != nullptr)
		{
			//Find Location player One
			const FVector PlayerOneLocation = PlayerOne->GetActorLocation();

			//Create offset for player two spawn, so that is  does not spawn on top of player one
			const FVector PlayerTwoSpawnLocationOffset = FVector(0.f, -150.0f, 0.0f);
			const FVector FinalSpawnLocation = PlayerOneLocation + PlayerTwoSpawnLocationOffset;
			const FRotator SpawnRotation = FRotator::ZeroRotator;

			//Create a Player Controller at index 1

			APlayerController* NewPlayerController = UGameplayStatics::CreatePlayer(World, 1);
			if (NewPlayerController != nullptr)
			{
				AMotorcycle* CreatedPlayer = Cast<AMotorcycle>(NewPlayerController->GetPawn());
				AHUD* CreatedHud = NewPlayerController->GetHUD();

				if (CreatedPlayer)
				{
					CreatedPlayer->Destroy();
				}

				if (CreatedHud)
				{
					CreatedHud->Destroy();
				}

				//Unposses  the newly created a APlayerController
				NewPlayerController->UnPossess();

				//Spawn Player Two
				//AMotorcycle2* PlayerTwo = World->SpawnActor<AMotorcycle2>(AMotorcycle2::StaticClass(), FinalSpawnLocation, SpawnRotation, SpawnParams);
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AMotorcycle2* PlayerTwo = World->SpawnActor<AMotorcycle2>(Player2, FinalSpawnLocation, SpawnRotation, SpawnParams);
				if (PlayerTwo != nullptr)
				{
					NewPlayerController->Possess(PlayerTwo);
				}
			}
		}
	}
}

void AOldNortGameModeBase::LoadTwoPlayerWidget()
{
	const APlayerController* P1Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (P1Controller)
	{
		AMultiplayerHUD* P1HUD = Cast<AMultiplayerHUD>(P1Controller->GetHUD());
		if (P1HUD)
		{
			P1HUD->CreateTwoPlayerUI();
		}
	}
}
