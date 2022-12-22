// Fill out your copyright notice in the Description page of Project Settings.


#include "BackLineCollisionActor.h"

// Sets default values
ABackLineCollisionActor::ABackLineCollisionActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABackLineCollisionActor::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
}

// Called every frame
void ABackLineCollisionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
