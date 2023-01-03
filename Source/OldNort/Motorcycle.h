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
	UPROPERTY( EditAnywhere,BlueprintReadOnly,Category ="Character Stats")
	bool bIsTwoPlayerGame;
private:
	// Distance between de Spawner and the pawn location
	
	UPROPERTY(EditAnywhere)
	float Displacement;
	UPROPERTY(EditAnywhere)
	float Speed;
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
    //Delay for player espawm
	UPROPERTY(EditAnywhere)
	float RespawnDelay;
	
	//Condition tu run a Turn function
	bool bCanTurn = true;

	

	//Respawn Variables
	float RespawnCountdawn;
	bool bCanDisable;
	bool bCanRespawn;

public:
	UPROPERTY(EditAnywhere,Category="Spawn Locations")
	TArray<class ATargetPoint*> RespawnLocations;
	void DisablePlayer();
protected:
	UPROPERTY(EditAnywhere)
	float TurnDisplacementDistance;

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

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Character Stats")
	int32 TotalScore;

	UPROPERTY(EditAnywhere, Category="Character Stats")
	bool bIsDead;

	
	// Player Tag Name
	static  const FName MyTagName;
	// Level Name
	static const FString MyLevelName;

	
	// Timers
	FTimerHandle SpawnCoollisionActorTimer;
	FTimerHandle EnableInputTimer;
	FTimerHandle RespawnPlayerTimer;
	
	ATargetPoint* FirstRespawnInWorld;
	ATargetPoint* SecondRespawnInWorld;
	ATargetPoint* ThirdRespawnInWorld;
	ATargetPoint* FourthRespawnInWorld;


	
	class AMotorcyclePlayerState* MyPlayerState;

	
private:
	void SpawnBacklineCollisionActors();
	void StartCollisionActorsSpawnTimer();
	void OnTurnRightPressed();
	void OnTurnLeftPressed();
	void Turn(const bool bShouldTurnLeft);
	void EnableTurn();
	UFUNCTION()
	void CollideBacklineCollisionActors(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void FindRespawnLocation();
	
	void ChooseRandomSpawnPoint();
	void Respawn();
	
	

	//Player State Method
	void FindPlayerState(); 

};




