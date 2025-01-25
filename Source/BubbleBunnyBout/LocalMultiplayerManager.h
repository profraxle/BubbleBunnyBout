// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "LocalMultiplayerManager.generated.h"

#define COUNTDOWN_AMOUNT 3

UCLASS()
class BUBBLEBUNNYBOUT_API ALocalMultiplayerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALocalMultiplayerManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* menuMappingContext;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<bool> readyCheck;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<ULocalPlayer*>localPlayers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float countdownTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UWorld> gameLevel;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
