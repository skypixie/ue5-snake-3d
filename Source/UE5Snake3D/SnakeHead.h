// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Interactable.h"

#include "SnakeHead.generated.h"


class UInputAction;
class UInputMappingContext;
class IInteractable;
class ASnakePart;


UCLASS()
class UE5SNAKE3D_API ASnakeHead : public APawn, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnakeHead();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY()
	TArray<ASnakePart*> SnakeElements;

	UPROPERTY(EditAnywhere)
	float SnakeSpeed;

	UPROPERTY(EditAnywhere)
	float SnakeSize;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakePart> SnakePartClass;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UFUNCTION()
	void Look(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact(AActor* Interactor) override;

	void AddSnakeElement(int ElementsNum = 1);

	void Move(float DeltaTime);

	FTransform GetNewElementTransform(FVector PrevMovementVector, FVector PrevLocation);
};
