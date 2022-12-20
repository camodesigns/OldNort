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

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMotorcycle::BeginPlay()
{
	Super::BeginPlay();
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
	InputComponent->BindAxis("CameraPitch", this, &AMotorcycle::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &AMotorcycle::YawCamera);
	InputComponent->BindAction("PawnRotationRight", IE_Pressed, this, &AMotorcycle::PawnRotationRight);
	InputComponent->BindAction("PawnRotationLeft", IE_Pressed, this, &AMotorcycle::PawnRotationLeft);
}

void AMotorcycle::PawnRotationRight()
{
	FRotator NewActorRotation = GetActorRotation();
	NewActorRotation.Yaw = GetActorRotation().Yaw + 90.0f;
	SetActorRotation(NewActorRotation);
}

void AMotorcycle::PawnRotationLeft()
{
	FRotator NewActorRotation = GetActorRotation();
	NewActorRotation.Yaw = GetActorRotation().Yaw - 90.0f;
	SetActorRotation(NewActorRotation);
}

//Camera Yaw and Pitch Axis Rotation
void AMotorcycle::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void AMotorcycle::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}
