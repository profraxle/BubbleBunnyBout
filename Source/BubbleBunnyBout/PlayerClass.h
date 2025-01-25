// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
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

	FVector2D leftAxisVector;
	FVector2D rightAxisVector;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Combat functions
	void RotateLeftArm(const FInputActionValue& Value);
	void RotateRightArm(const FInputActionValue& Value);
	void RaiseLeftArm(const FInputActionValue& Value);
	void RaiseRightArm(const FInputActionValue& Value);

	// Arms
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* leftArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* rightArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* leftArmMarker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* rightArmMarker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float leftArmDistance = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float leftArmRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float leftArmMaxExtensionDistance = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rightArmMaxExtensionDistance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float leftArmExtensionDistance = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rightArmExtensionDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rightArmDistance = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rightArmRadius = 100.f;


	// Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* inputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* leftArmRotateIA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* rightArmRotateIA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* leftArmRaiseIA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* rightArmRaiseIA;

};
