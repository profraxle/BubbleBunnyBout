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
	movementAngle = 0;
	moveSpeed = 1;
	moveRotateRadius = FVector(200.f, 0.f, 0.f);

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

	//Get the centre of the bout
	boutCentre = UGameplayStatics::GetActorOfClass(GetWorld(), ABoutCentre::StaticClass());
	boutCentreLocation = boutCentre->GetActorLocation();
}

// Called every frame
void APlayerClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator LookAtRotation = (GetActorLocation() - boutCentreLocation);

	//SetActorRotation();
}

//Move left and right around a point
void APlayerClass::Move(const FInputActionValue& Value)
{
	//the new location to move to, dont move on Z axis, just horizontally
	FVector newLocation = FVector(boutCentreLocation.X, boutCentreLocation.Y, GetActorLocation().Z);

	//move the current angle based on movespeed, left and right
	FVector2D inputVector = Value.Get<FVector2D>();
	if(inputVector.X > 0.f)
	{
		movementAngle += moveSpeed;
	}
	else if (inputVector.X < 0.f) {
		movementAngle -= moveSpeed;
	}
	
	//BOMBOCLAT CIRCLE
	if (movementAngle > 360.f)
	{
		movementAngle = 1.f;
	}
	else if (movementAngle <= 0.f)
	{
		movementAngle = 360.f;
	}

	//rotate around the radius 
	FVector rotateValue = moveRotateRadius.RotateAngleAxis(movementAngle, FVector(0.f, 0.f, 1));

	//update new location
	newLocation.X += rotateValue.X;
	newLocation.Y += rotateValue.Y;

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



