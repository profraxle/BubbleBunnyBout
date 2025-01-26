// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerClass.h"
#include "EnhancedInputComponent.h"
#include "MyGameJimstance.h"

// Sets default values
APlayerClass::APlayerClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	leftArm = CreateDefaultSubobject<USceneComponent>("LeftArm");
	rightArm = CreateDefaultSubobject<USceneComponent>("RightArm");
	leftArmMarker = CreateDefaultSubobject<UStaticMeshComponent>("LeftArmMarker");
	rightArmMarker = CreateDefaultSubobject<UStaticMeshComponent>("RightArmMarker");
	popVfx = CreateDefaultSubobject<UParticleSystemComponent>("PoppingVFX");
	leftArm->SetupAttachment(RootComponent);
	rightArm->SetupAttachment(RootComponent);
	leftArmMarker->SetupAttachment(RootComponent);
	rightArmMarker->SetupAttachment(RootComponent);
	popVfx->SetupAttachment(RootComponent);


	//Init movement vars
	movementAngle = 0;
	moveSpeed = 1;
	moveRotateRadius = FVector(200.f, 0.f, 0.f);

	//Init animation vars
	isMovingLeft = false;
	isMovingRight = false;
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

		ULocalPlayer* LP = Cast<ULocalPlayer>(PlayerController->Player);

		playerID = LP->GetControllerId();


		if (playerID == 1) {
			movementAngle = 180;
		}

		//Get the centre of the bout
		boutCentre = UGameplayStatics::GetActorOfClass(GetWorld(), ABoutCentre::StaticClass());
		if (IsValid(boutCentre)) boutCentreLocation = boutCentre->GetActorLocation();
		else boutCentreLocation = FVector(0, 0, 0);


		leftAxisVector = FVector2D(0, 0);
		rightAxisVector = FVector2D(0, 0);

		//the new location to move to, dont move on Z axis, just horizontally
		FVector newLocation = FVector(boutCentreLocation.X, boutCentreLocation.Y, GetActorLocation().Z);

		//rotate around the radius 
		FVector rotateValue = moveRotateRadius.RotateAngleAxis(movementAngle, FVector(0.f, 0.f, 1));

		//update new location
		newLocation.X += rotateValue.X;
		newLocation.Y += rotateValue.Y;

		//set new actor location
		SetActorLocation(newLocation);

		popVfx->SetActive(false);
	}
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
	//leftArmMarker->SetWorldRotation(leftDirectionVector.ToOrientationQuat());
	leftArm->SetWorldRotation(leftDirectionVector.ToOrientationQuat());

	leftAxisVector = FVector2D(0, 0);
	leftArmExtensionDistance = 0.f;

	// Handle right arm
	FVector rightDirectionPoint = rightArm->GetComponentLocation() + GetActorForwardVector() * rightArmDistance;
	rightDirectionPoint += GetActorRightVector() * rightAxisVector.X * rightArmRadius;
	rightDirectionPoint += GetActorUpVector() * rightAxisVector.Y * rightArmRadius;

	FVector rightDirectionVector = rightDirectionPoint - rightArm->GetComponentLocation();
	rightDirectionVector.Normalize();
	rightDirectionVector.Z = 0;
	rightDirectionPoint += rightDirectionVector * rightArmExtensionDistance;

	rightArmMarker->SetWorldLocation(rightDirectionPoint);
	rightArmMarker->SetWorldRotation(rightDirectionVector.ToOrientationQuat());
	rightArm->SetWorldRotation(rightDirectionVector.ToOrientationQuat());

	rightAxisVector = FVector2D(0, 0);
	rightArmExtensionDistance = 0.f;

	if (enemy) {
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(enemy->GetActorLocation().X, enemy->GetActorLocation().Y, GetActorLocation().Z)));
	}
	// Death
	if (dying) {
		deathTimeElapsed += DeltaTime;
		if (deathTimeElapsed >= 1.f) {
			deathTimeElapsed = 0.f;
			dying = false;
			Cast<UMyGameJimstance>(GetGameInstance())->CheckForWin();
			GetMesh()->SetVisibility(true);
			GetMesh()->SetRelativeScale3D(FVector(2.75f));
			popVfx->SetActive(false);
			return;
		}
		else if (deathTimeElapsed >= 0.15f) {
			GetMesh()->SetVisibility(false);
			if (!popped && !popVfx->IsActive()) {
				popped = true;
				popVfx->SetActive(true);
				popVfx->Activate(true);
			}
			return;
		}
		float scaleOffset = 2.75f + (deathTimeElapsed * 5.f);
		GetMesh()->SetRelativeScale3D(FVector(scaleOffset, scaleOffset, 2.75f));
	}
}

//Move left and right around a point
void APlayerClass::Move(const FInputActionValue& Value)
{
	//the new location to move to, dont move on Z axis, just horizontally
	FVector newLocation = FVector(boutCentreLocation.X, boutCentreLocation.Y, GetActorLocation().Z);

	//move the current angle based on movespeed, left and right
	FVector2D inputVector = Value.Get<FVector2D>();
	if(inputVector.X > 0.f)				//Left
	{
		movementAngle += moveSpeed;
		isMovingLeft = true;
		isMovingRight = false;
	}
	else if (inputVector.X < 0.f) {		//Right
		movementAngle -= moveSpeed;
		isMovingLeft = false;
		isMovingRight = true;
	}
	
	//BOMBOCLAT CIRCLE
	if (playerID == 1) {
		if (movementAngle > 270 - moveAngleConstraint)
		{
			movementAngle = 270 - moveAngleConstraint;
		}
		else if (movementAngle < 90 + moveAngleConstraint)
		{
			movementAngle = 90 + moveAngleConstraint;
		}
	}
	else {
		if (movementAngle > 90 - moveAngleConstraint)
		{
			movementAngle = 90 - moveAngleConstraint;
		}
		else if (movementAngle < -90 + moveAngleConstraint)
		{
			movementAngle = -90 + moveAngleConstraint;
		}
	}

	//rotate around the radius 
	FVector rotateValue = moveRotateRadius.RotateAngleAxis(movementAngle, FVector(0.f, 0.f, 1));

	//update new location
	newLocation.X += rotateValue.X;
	newLocation.Y += rotateValue.Y;

	//set new actor location
	SetActorLocation(newLocation);
	
}

//if there is no input left or right movement wise
void APlayerClass::StopMove(const FInputActionValue& Value)
{
	isMovingLeft = false;
	isMovingRight = false;
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
		EnhancedInputComponenet->BindAction(movementIA, ETriggerEvent::None, this, &APlayerClass::StopMove);
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

void APlayerClass::Die()
{
	if (!dying) {
		if (!Cast<APlayerClass>(UGameplayStatics::GetPlayerControllerFromID(this, 1 - playerID)->GetPawn())->dying) {
			Cast<UMyGameJimstance>(GetGameInstance())->AwardPoint(1 - playerID);
		}
	}
	dying = true;
}



