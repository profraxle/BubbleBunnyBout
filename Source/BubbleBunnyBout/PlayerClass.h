// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "BoutCentre.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerClass.generated.h"

UCLASS()
class BUBBLEBUNNYBOUT_API APlayerClass : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Combat functions
	void RotateLeftArm(const FInputActionValue& Value);
	void RotateRightArm(const FInputActionValue& Value);

	//Movement Functions
	void Move(const FInputActionValue& Value);

	//----------Movement Variables ----------
	//the current angle that the player is at, 0 to 360, changing default value alters the starting position
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float movementAngle;		
	//speed at which the player rotates around the point
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float moveSpeed;	
	//distance from the point that the players rotate around
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector moveRotateRadius;		
	//actor at the centre of the bout
	AActor* boutCentre;
	FVector boutCentreLocation;

	// Components
	USceneComponent* leftArm;
	USceneComponent* rightArm;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* inputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* leftArmRotateIA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* rightArmRotateIA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* movementIA;
};
