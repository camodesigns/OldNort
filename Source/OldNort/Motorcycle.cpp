#include "Motorcycle.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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

	SpringArmComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 00.0f), FRotator(-25.0f, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 800.f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;

	Speed = 450.0f;
	MaxSpeed = 650.0f;
	Displacement = -180.0f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMotorcycle::BeginPlay()
{
	Super::BeginPlay();
	StartCollisionActorsSpawnTimer();
}

// Called every frame
void AMotorcycle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector NewLocation = GetActorLocation() + GetActorForwardVector() * (Speed * DeltaTime);
	SetActorLocation(NewLocation);

	FRotator NewRotationYaw = SpringArmComponent->GetComponentRotation();
	NewRotationYaw.Yaw = FMath::Clamp(-80.0f, NewRotationYaw.Yaw + CameraInput.X, 15.0f);
	SpringArmComponent->SetWorldRotation(NewRotationYaw);

	FRotator NewRotationPitch = SpringArmComponent->GetComponentRotation();
	NewRotationPitch.Pitch = FMath::Clamp(NewRotationPitch.Pitch + CameraInput.Y, -80.0f, 15.0f);
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
	float TurnAngle = 0.0f;
	if (bShouldTurnLeft)
	{
		const FVector NewLocation = GetActorLocation() - GetActorRightVector() * 180.0f;
		SetActorLocation(NewLocation);
		TurnAngle = -90.0f;
	}
	else
	{
		const FVector NewLocation = GetActorLocation() + GetActorRightVector() * 180.0f;
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
