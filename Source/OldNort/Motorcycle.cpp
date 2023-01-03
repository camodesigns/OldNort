#include "Motorcycle.h"
#include "MotorcyclePlayerState.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LevelScriptActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Motorcycle2.h"
#include "OldNortGameModeBase.h"
#include "Engine/TargetPoint.h"


const FName AMotorcycle::MyTagName("PlayerOne");
const FString AMotorcycle::MyLevelName("Minimal_Default");
// Sets default values
AMotorcycle::AMotorcycle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	SpringArmComponent->SetupAttachment(GetRootComponent());

	//Set the Arm Location and Rotation
	SpringArmComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 00.0f), FRotator(-25.0f, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 800.f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(128.314102f, 183.722122f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	//

	Speed = 500.0f;
	Displacement = -360.0f;

	RespawnCountdawn = 3.0f;
	bCanDisable = false;
	bCanRespawn = false;
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
void AMotorcycle::BeginPlay()
{
	Super::BeginPlay();
	StartCollisionActorsSpawnTimer();
	FindPlayerState();
	FindRespawnLocation();
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMotorcycle::CollideBacklineCollisionActors);
}

// Called every frame
void AMotorcycle::Tick(float DeltaTime)
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
void AMotorcycle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("CameraPitch", this, &AMotorcycle::TurnCameraPitch);
	InputComponent->BindAxis("CameraYaw", this, &AMotorcycle::TurnCameraYaw);
	InputComponent->BindAction("PawnRotationRight", IE_Pressed, this, &AMotorcycle::OnTurnRightPressed);
	InputComponent->BindAction("PawnRotationLeft", IE_Pressed, this, &AMotorcycle::OnTurnLeftPressed);
}

//Camera Yaw and Pitch Axis Rotation
void AMotorcycle::TurnCameraPitch(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void AMotorcycle::TurnCameraYaw(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void AMotorcycle::SpawnBacklineCollisionActors()
{
	const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * Displacement;
	//DrawDebugSphere(GetWorld(),SpawnLocation,3.0f, 12, FColor::Red,false,5.0f,0,3.0f);
	GetWorld()->SpawnActor<ABackLineCollisionActor>(CollisionActorClass, SpawnLocation, GetActorRotation());
}

void AMotorcycle::StartCollisionActorsSpawnTimer()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnCoollisionActorTimer, this, &AMotorcycle::SpawnBacklineCollisionActors,
	                                       SpawnCollisionActorDelay, true);
}

void AMotorcycle::OnTurnRightPressed()
{
	if (bCanTurn)
	{
		SpawnBacklineCollisionActors();
		Turn(false);
		bCanTurn = false;
		GetWorld()->GetTimerManager().SetTimer(EnableInputTimer, this, &AMotorcycle::EnableTurn, EnableTurnDelay,
		                                       false);
	}
}

void AMotorcycle::OnTurnLeftPressed()
{
	if (bCanTurn)
	{
		SpawnBacklineCollisionActors();
		Turn(true);
		bCanTurn = false;
		GetWorld()->GetTimerManager().SetTimer(EnableInputTimer, this, &AMotorcycle::EnableTurn, EnableTurnDelay,
		                                       false);
	}
}

void AMotorcycle::Turn(const bool bShouldTurnLeft)
{
	float TurnAngle;
	if (bShouldTurnLeft)
	{
		const FVector NewLocation = GetActorLocation() - GetActorRightVector() * TurnDisplacementDistance;
		SetActorLocation(NewLocation);
		TurnAngle = -90.0f;
	}
	else
	{
		const FVector NewLocation = GetActorLocation() + GetActorRightVector() * TurnDisplacementDistance;
		SetActorLocation(NewLocation);
		TurnAngle = 90.0f;
	}
	FRotator NewActorRotation = GetActorRotation();
	NewActorRotation.Yaw = GetActorRotation().Yaw + TurnAngle;
	SetActorRotation(NewActorRotation);
	StartCollisionActorsSpawnTimer();
}

void AMotorcycle::EnableTurn()
{
	bCanTurn = true;
}

void AMotorcycle::CollideBacklineCollisionActors(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                 const FHitResult& SweepResult)
{
	if (!bIsDead && OtherActor && (OtherActor != this) && OtherComp)
	{
		bIsDead = true;
		if (GEngine)
		{
			//Print a test of collison, using a example text
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are death"));
		}

		AMotorcyclePlayerState* PlayerState2 = Cast<AMotorcyclePlayerState>(UGameplayStatics::GetPlayerState(this, 1));
		if (PlayerState2 != nullptr)
		{
			PlayerState2->TotalScore += 1;
			// Check if score is enough to win
			if (PlayerState2->TotalScore == PlayerState2->WinScore)
			{
				if (GEngine)
				{
					//Print a test of collision, using a example text
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Winner P2"));
					// If score is enough to win
					
					AOldNortGameModeBase* GameMode=Cast<AOldNortGameModeBase>(GetWorld()->GetAuthGameMode());
					if(GameMode!=nullptr)
					{
						GameMode->OnPlayerWon(1);
					}
				}
			}
			else
			{
				// Else
				// Disable player (explode, disappear, etc) -> remove collisions, movement, and visibility
				DisablePlayer();
				// Start timer to respawn
				// When timer is up, call respawn
			
				GetWorld()->GetTimerManager().SetTimer(RespawnPlayerTimer, this, &AMotorcycle::Respawn,
													   RespawnDelay, false);
			}
		}
	}
}

void AMotorcycle::FindPlayerState()
{
	const APlayerController* PlMotorcycle = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlMotorcycle)
	{
		MyPlayerState = Cast<AMotorcyclePlayerState>(PlMotorcycle->PlayerState);
	}
}

void AMotorcycle::FindRespawnLocation()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		RespawnLocations.Add(Cast<ATargetPoint>(FoundActor));
	}
}


void AMotorcycle::DisablePlayer()
{
	Speed = 0.0f;
	this->SetActorEnableCollision(false);
	this->SetActorHiddenInGame(true);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue,TEXT("You are Dead"));
}

//Choose a Random target point for the respawn
void AMotorcycle::ChooseRandomSpawnPoint()
{
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

void AMotorcycle::Respawn()
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
