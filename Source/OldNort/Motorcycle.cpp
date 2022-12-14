// Fill out your copyright notice in the Description page of Project Settings.


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

	Speed = FVector(250.0f, 0.0f, 0.0f);
	MaxSpeed = 350.0f;

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

	const FVector NewLocation = GetActorLocation() + (Speed * DeltaTime);
	SetActorLocation(NewLocation);
}

// Called to bind functionality to input
void AMotorcycle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMotorcycle::RotateYaw(float RotationValue)
{
}
