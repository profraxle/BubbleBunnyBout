// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerClass.h"
#include "EnhancedInputComponent.h"

// Sets default values
APlayerClass::APlayerClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	leftArm = CreateDefaultSubobject<USceneComponent>("LeftArm");
	rightArm = CreateDefaultSubobject<USceneComponent>("RightArm");

	//Init movement vars
	angle = 0;
	moveSpeed = 1;
}

// Called when the game starts or when spawned
void APlayerClass::BeginPlay()
{
	Super::BeginPlay();
	
	//Setup input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(inputMappingContext, 0);
		}
	}
}

// Called every frame
void APlayerClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Move left and right around a point
void APlayerClass::Move(const FInputActionValue& Value)
{
	//the new location to move to
	FVector newLocation = FVector(0.f,0.f,0.f);

	//the radius to move around, temporary for now, will be an actor they move around
	FVector radius = FVector(200.f,0.f,0.f);

	//move the current angle based on movespeed
	angle+=moveSpeed;

	//BOMBOCLAT CIRCLE
	if (angle > 360.f)
	{
		angle = 1.f;
	}else if (angle <= 0.f)
	{
		angle = 360.f;
	}

	//rotate around the radius 
	FVector rotateValue = radius.RotateAngleAxis(angle,FVector(0.f,0.f,moveSpeed));

	//update new location
	newLocation.X += rotateValue.X;
	newLocation.Y += rotateValue.Y;
	newLocation.Z += rotateValue.Z;

	//set new actor location
	SetActorLocation(newLocation);
}

// Called to bind functionality to input
void APlayerClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponenet = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponenet->BindAction(rightArmRotateIA, ETriggerEvent::Triggered, this, &APlayerClass::RotateRightArm);
		EnhancedInputComponenet->BindAction(leftArmRotateIA, ETriggerEvent::Triggered, this, &APlayerClass::RotateLeftArm);
		EnhancedInputComponenet->BindAction(movementIA, ETriggerEvent::Triggered, this, &APlayerClass::Move);
	}
}

void APlayerClass::RotateLeftArm(const FInputActionValue& Value)
{
	FVector2D rotVector = Value.Get<FVector2D>();
	leftArm->AddLocalRotation(FRotator(rotVector.X, rotVector.Y, 0));
}

void APlayerClass::RotateRightArm(const FInputActionValue& Value)
{
}



