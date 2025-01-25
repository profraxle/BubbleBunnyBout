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

	leftAxisVector = FVector2D(0, 0);
	rightAxisVector = FVector2D(0, 0);
}

// Called every frame
void APlayerClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

