#include "Motorcycle2.h"
#include "MotorcyclePlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LevelScriptActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Motorcycle.h"
#include "Engine/TargetPoint.h"
#include "OldNortGameModeBase.h"

// Sets default values
AMotorcycle2::AMotorcycle2()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetAutoActivate(true);


	//Set the Arm Location and Rotation
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 00.0f), FRotator(-25.0f, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 800.f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;
	SpringArmComponent->SetAutoActivate(true);

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(128.314102f, 183.722122f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	
	Speed = 450.0f;
	Displacement = -180.0f;

	AutoPossessPlayer = EAutoReceiveInput::Player1;
	
	FirstRespawnInWorld = nullptr;
	SecondRespawnInWorld = nullptr;
	ThirdRespawnInWorld = nullptr;
	TotalScore = 0;
	bIsDead = false;
	bIsTwoPlayerGame = false;


	RespawnLocations.Empty();

	this->Tags.Add(FName("Player1"));
}

// Called when the game starts or when spawned
void AMotorcycle2::BeginPlay()
{
	Super::BeginPlay();
	StartCollisionActorsSpawnTimer();
	FindPlayerState();
	FindRespawnLocation();
	SpawnBackLineCollisionActors();
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMotorcycle2::CollideBackLineCollisionActors);
}

// Called every frame
void AMotorcycle2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector NewLocation = GetActorLocation() + GetActorForwardVector() * (Speed * DeltaTime);
	SetActorLocation(NewLocation);

	FRotator NewRotationYaw = SpringArmComponent->GetComponentRotation();
	NewRotationYaw.Yaw = FMath::Clamp(-360.0f, NewRotationYaw.Yaw + CameraInput.X, 100.0f);
	SpringArmComponent->SetWorldRotation(NewRotationYaw);

	FRotator NewRotationPitch = SpringArmComponent->GetComponentRotation();
	NewRotationPitch.Pitch = FMath::Clamp(NewRotationPitch.Pitch + CameraInput.Y, -360.0f, 100.0f);
	SpringArmComponent->SetWorldRotation(NewRotationPitch);
}

// Called to bind functionality to input
void AMotorcycle2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("CameraPitch2", this, &AMotorcycle2::TurnCameraPitch);
	InputComponent->BindAxis("CameraYaw2", this, &AMotorcycle2::TurnCameraYaw);
	InputComponent->BindAction("Pawn2RotationRight", IE_Pressed, this, &AMotorcycle2::OnTurnRightPressed);
	InputComponent->BindAction("Pawn2RotationLeft", IE_Pressed, this, &AMotorcycle2::OnTurnLeftPressed);
}

//Camera Yaw and Pitch Axis Rotation
void AMotorcycle2::TurnCameraPitch(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void AMotorcycle2::TurnCameraYaw(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void AMotorcycle2::SpawnBackLineCollisionActors()
{
	const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * Displacement;
	//DrawDebugSphere(GetWorld(),SpawnLocation,3.0f, 12, FColor::Red,false,5.0f,0,3.0f);
	GetWorld()->SpawnActor<ABackLineCollisionActor>(CollisionActorClass, SpawnLocation, GetActorRotation());
}

void AMotorcycle2::StartCollisionActorsSpawnTimer()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnCoollisionActorTimer, this, &AMotorcycle2::SpawnBackLineCollisionActors,
	                                       SpawnCollisionActorDelay, true);
}

void AMotorcycle2::OnTurnRightPressed()
{
	if (bCanTurn)
	{
		SpawnBackLineCollisionActors();
		Turn(false);
		bCanTurn = false;
		GetWorld()->GetTimerManager().SetTimer(EnableInputTimer, this, &AMotorcycle2::EnableTurn, EnableTurnDelay,
		                                       false);
	}
}

void AMotorcycle2::OnTurnLeftPressed()
{
	if (bCanTurn)
	{
		SpawnBackLineCollisionActors();
		Turn(true);
		bCanTurn = false;
		GetWorld()->GetTimerManager().SetTimer(EnableInputTimer, this, &AMotorcycle2::EnableTurn, EnableTurnDelay,
		                                       false);
	}
}

void AMotorcycle2::Turn(const bool bShouldTurnLeft)
{
	float TurnAngle;
	if (bShouldTurnLeft)
	{
		const FVector NewLocation = GetActorLocation() - GetActorRightVector() * 260.0f;
		SetActorLocation(NewLocation);
		TurnAngle = -90.0f;
	}
	else
	{
		const FVector NewLocation = GetActorLocation() + GetActorRightVector() * 260.0f;
		SetActorLocation(NewLocation);
		TurnAngle = 90.0f;
	}
	FRotator NewActorRotation = GetActorRotation();
	NewActorRotation.Yaw = GetActorRotation().Yaw + TurnAngle;
	SetActorRotation(NewActorRotation);
	StartCollisionActorsSpawnTimer();
}

void AMotorcycle2::EnableTurn()
{
	bCanTurn = true;
}

void AMotorcycle2::CollideBackLineCollisionActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	if (!bIsDead && OtherActor && (OtherActor != this) && OtherComp)
	{
		bIsDead = true;
		if (GEngine)
		{
			//Print a test of collision, using a example text
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are death Player 2"));
		}

		AMotorcyclePlayerState* PlayerState1 = Cast<AMotorcyclePlayerState>(UGameplayStatics::GetPlayerState(this, 0));
		if (PlayerState1 != nullptr)
		{
			PlayerState1->TotalScore += 1;

			// Check if score is enough to win
			if (PlayerState1->TotalScore == PlayerState1->WinScore)
			{
				if (GEngine)
				{
					//Print a test of collision, using a example text
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Winner P2"));
					// If score is enough to win
				}
				AOldNortGameModeBase* GameMode = Cast<AOldNortGameModeBase>(GetWorld()->GetAuthGameMode());
				if (GameMode != nullptr)
				{
					GameMode->OnPlayerWon(0);
				}
			}
			else
			{
				// Else
				// Disable player (explode, disappear, etc) -> remove collisions, movement, and visibility
				DisablePlayer();
				// Start timer to respawn
				// When timer is up, call respawn
				GetWorld()->GetTimerManager().SetTimer(ReSpawnplayerTimer, this, &AMotorcycle2::Respawn,
				                                       RespawnPlayerDelay, false);
			}
		}
	}
}

void AMotorcycle2::FindPlayerState()
{
	//To make sure everything has loaded correctly, Create a level Check
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 1);
	if (PlayerController)
	{
		MyPlayerState = Cast<AMotorcyclePlayerState>(PlayerController->PlayerState);
	}
}

void AMotorcycle2::FindRespawnLocation()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		RespawnLocations.Add(Cast<ATargetPoint>(FoundActor));
	}
}


void AMotorcycle2::DisablePlayer()
{
	Speed = 0.0f;
	this->SetActorEnableCollision(false);
	this->SetActorHiddenInGame(true);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue,TEXT("You are Dead"));
}

void AMotorcycle2::ChooseRandomSpawnPoint()
{
	//Inicialize point for ATargetPoint

	//Select a random value between 0-2, which is the size of the array
	const int32 RandomValue = FMath::RandRange(0, 2);

	if (RespawnLocations.IsValidIndex(RandomValue))
	{
		const ATargetPoint* LocationToRespawn = RespawnLocations[RandomValue];
		if (LocationToRespawn != nullptr)
		{
			const FVector RespawnLocation = LocationToRespawn->GetActorLocation();
			const FRotator RespawnRotation = LocationToRespawn->GetActorRotation();

			//Set player Spawn Location an Rotation
			SetActorLocation(RespawnLocation);
			SetActorRotation(RespawnRotation);

			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,TEXT("Actor Position is Set"));
		}
	}
}

void AMotorcycle2::Respawn()
{
	// Enable player -> enable collision, movement and visibility

	//Set the ReSpawn Position to a Random Position of the positions array
	ChooseRandomSpawnPoint();

	this->SetActorHiddenInGame(false);
	this->SetActorEnableCollision(true);

	//Set Speed
	Speed = 1000.0f;

	// Set is dead to false
	bIsDead = false;
}
