// Fill out your copyright notice in the Description page of Project Settings.

#include "SnakeHead.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"

#include "SnakePart.h"
#include "Interactable.h"
#include "Food.h"


// Sets default values
ASnakeHead::ASnakeHead()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	// Attach components
	SpringArmComp->SetupAttachment(MeshComponent);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	//Assign SpringArm class variables
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(0.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength	= 200.f;

	// idk it may be that i should have set it to true in Blueprints
	SpringArmComp->bUsePawnControlRotation = true;
	this->bUseControllerRotationPitch = true;
	this->bUseControllerRotationYaw = true;

	SnakeSpeed = 300.f;
	SnakeSize = 100.f;
}

// Called when the game starts or when spawned
void ASnakeHead::BeginPlay()
{
	Super::BeginPlay();
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	// add first element
	// НЕ РАБОТАЕТ!!!
	FTransform NewTransform = GetNewElementTransform(this->GetActorForwardVector(), this->GetActorLocation());
	ASnakePart* NewSnakeElement = GetWorld()->SpawnActor<ASnakePart>(SnakePartClass, NewTransform);
	SnakeElements.Add(NewSnakeElement);

}

void ASnakeHead::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

// Called every frame
void ASnakeHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

// Called to bind functionality to input
void ASnakeHead::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASnakeHead::Look);
	}
}

void ASnakeHead::Interact(AActor* Interactor)
{
	if (AFood* Food = Cast<AFood>(Interactor))
	{
		Food->Die();
		this->AddSnakeElement();
	}
}

void ASnakeHead::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; ++i)
	{
		ASnakePart* PrevElement = SnakeElements[SnakeElements.Num() - 1];
		FTransform NewElementTransform = GetNewElementTransform(
			PrevElement->GetActorForwardVector(),
			PrevElement->GetActorLocation());
		ASnakePart* NewSnakeElement = GetWorld()->SpawnActor<ASnakePart>(SnakePartClass, NewElementTransform);
		SnakeElements.Add(NewSnakeElement);
	}
}

void ASnakeHead::Move(float DeltaTime)
{
	// Move the head
	FVector MovementVector(this->GetActorForwardVector() * SnakeSpeed * DeltaTime);
	this->AddActorWorldOffset(MovementVector, true);

	// Move the rest
	for (int i = 0; i < SnakeElements.Num(); ++i)
	{
		if (i == 0)
		{
			FTransform NewTransform = GetNewElementTransform(
				this->GetActorForwardVector(),
				this->GetActorLocation());
			ASnakePart* SnakeElem = SnakeElements[i];
			// SnakeElem->SetActorLocation(NewTransform.GetLocation()); // crashes
		}
	}
}

// Returns new position for the snake element
FTransform ASnakeHead::GetNewElementTransform(FVector PrevMovementVector, FVector PrevLocation)
{
	float NewRelativeLocationX = PrevLocation.X + PrevMovementVector.X * SnakeSize;
	float NewRelativeLocationY = PrevLocation.Y + PrevMovementVector.Y * SnakeSize;
	float NewRelativeLocationZ = PrevLocation.Z + PrevMovementVector.Z * SnakeSize;
	FVector NewRelativeLocation = FVector(NewRelativeLocationX, NewRelativeLocationY, NewRelativeLocationZ);

	FVector NewLocation = PrevLocation + NewRelativeLocation;
	return FTransform(NewLocation);
}
