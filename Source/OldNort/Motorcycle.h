// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Motorcycle.generated.h"

UCLASS()
class OLDNORT_API AMotorcycle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMotorcycle();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	float Speed;
	float MaxSpeed;
	FVector2D CameraInput;
	//Pawn  yaw Rotation
	void PawnRotationLeft();
	void PawnRotationRight();
	//Camera Pitch and Yaw rotation
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

protected:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComponent;
};
