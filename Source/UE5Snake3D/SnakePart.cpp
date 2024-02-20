// Fill out your copyright notice in the Description page of Project Settings.

#include "SnakePart.h"

#include "Interactable.h"


// Sets default values
ASnakePart::ASnakePart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

}

// Called when the game starts or when spawned
void ASnakePart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakePart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}