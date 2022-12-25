// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BackLineCollisionActor.h"
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
	// Distance between de Spawner and the pawn location
	UPROPERTY(EditAnywhere)
	float Displacement;
	UPROPERTY(EditAnywhere)
	float Speed;
	float MaxSpeed;
	float MaxDeathCount;
	float HasAlive = true;
	FVector2D CameraInput;
	//Camera Pitch and Yaw rotation
	void TurnCameraPitch(float AxisValue);
	void TurnCameraYaw(float AxisValue);
	UPROPERTY(EditAnywhere)
	float EnableTurnDelay;
	UPROPERTY(EditAnywhere)
	float SpawnCollisionActorDelay;
	//Condition tu run a Turn function
	bool bCanTurn = true;

protected:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABackLineCollisionActor> CollisionActorClass;

	UPROPERTY(EditAnywhere,Category="Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;
	
	// Timers
	FTimerHandle SpawnCoollisionActorTimer;
	FTimerHandle EnableInputTimer;

private:
	void SpawnBacklineCollisionActors();
	void StartCollisionActorsSpawnTimer();
	void OnTurnRightPressed();
	void OnTurnLeftPressed();
	void Turn(const bool bShouldTurnLeft);
	void EnableTurn();
	void HasCrash();
	UFUNCTION()
	void CollideBaclineCollisionActors(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void CollidePlayer();
	//void TurnOnDelay();

};
