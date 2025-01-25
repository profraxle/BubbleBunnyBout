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

// Called to bind functionality to input
void APlayerClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponenet = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponenet->BindAction(leftArmRotateIA, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
		EnhancedInputComponenet->BindAction(rightArmRotateIA, ETriggerEvent::Triggered, this, &AMyPlayer::Look);
	}
}

