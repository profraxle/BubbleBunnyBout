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

	leftArmMarker = CreateDefaultSubobject<UStaticMeshComponent>("LeftArmMarker");
	rightArmMarker = CreateDefaultSubobject<UStaticMeshComponent>("RightArmMarker");
	leftArm->SetupAttachment(RootComponent);
	rightArm->SetupAttachment(RootComponent);
	leftArmMarker->SetupAttachment(RootComponent);
	rightArmMarker->SetupAttachment(RootComponent);
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
	leftAxisVector = FVector2D(0, 0);
	rightAxisVector = FVector2D(0, 0);
}

// Called every frame
void APlayerClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FRotator LookAtRotation = (GetActorLocation() - boutCentreLocation);

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
	// Handle left arm
	FVector leftDirectionPoint = leftArm->GetComponentLocation() + GetActorForwardVector() * leftArmDistance;
	leftDirectionPoint += GetActorRightVector() * leftAxisVector.X * leftArmRadius;
	leftDirectionPoint += GetActorUpVector() * leftAxisVector.Y * leftArmRadius;

	FVector leftDirectionVector = leftDirectionPoint - leftArm->GetComponentLocation();
	leftDirectionVector.Normalize();
	leftDirectionPoint += leftDirectionVector * leftArmExtensionDistance;

	leftArmMarker->SetWorldLocation(leftDirectionPoint);
	leftArm->SetWorldRotation(leftDirectionVector.ToOrientationQuat());

	leftAxisVector = FVector2D(0, 0);
	leftArmExtensionDistance = 0.f;

	// Handle right arm
	FVector rightDirectionPoint = rightArm->GetComponentLocation() + GetActorForwardVector() * rightArmDistance;
	rightDirectionPoint += GetActorRightVector() * rightAxisVector.X * rightArmRadius;
	rightDirectionPoint += GetActorUpVector() * rightAxisVector.Y * rightArmRadius;

	FVector rightDirectionVector = rightDirectionPoint - rightArm->GetComponentLocation();
	rightDirectionVector.Normalize();
	rightDirectionPoint += rightDirectionVector * rightArmExtensionDistance;

	rightArmMarker->SetWorldLocation(rightDirectionPoint);
	rightArm->SetWorldRotation(rightDirectionVector.ToOrientationQuat());

	rightAxisVector = FVector2D(0, 0);
	rightArmExtensionDistance = 0.f;
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
		EnhancedInputComponenet->BindAction(leftArmRaiseIA, ETriggerEvent::Triggered, this, &APlayerClass::RaiseLeftArm);
		EnhancedInputComponenet->BindAction(rightArmRaiseIA, ETriggerEvent::Triggered, this, &APlayerClass::RaiseRightArm);

	}
}

void APlayerClass::RotateLeftArm(const FInputActionValue& Value)
{
	leftAxisVector = Value.Get<FVector2D>();
	if(leftAxisVector.Length() > 1.f) leftAxisVector.Normalize();
}

void APlayerClass::RotateRightArm(const FInputActionValue& Value)
{
	rightAxisVector = Value.Get<FVector2D>();
	if (rightAxisVector.Length() > 1.f) rightAxisVector.Normalize();
}

void APlayerClass::RaiseLeftArm(const FInputActionValue& Value)
{
	leftArmExtensionDistance = Value.Get<float>() * leftArmMaxExtensionDistance;
}

void APlayerClass::RaiseRightArm(const FInputActionValue& Value)
{
	rightArmExtensionDistance = Value.Get<float>() * rightArmMaxExtensionDistance;
}



