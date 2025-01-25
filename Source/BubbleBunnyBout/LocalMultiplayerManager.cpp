// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerManager.h"

// Sets default values
ALocalMultiplayerManager::ALocalMultiplayerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ALocalMultiplayerManager::BeginPlay()
{
	Super::BeginPlay();

	//set both players ready check to false
	readyCheck = { false,false };
	
	//create the second player
	if (GetGameInstance()->GetLocalPlayers().Num() < 2) {
		FString Error;
		GetGameInstance()->CreateLocalPlayer(-1, Error, true);
	}
	
	localPlayers = GetGameInstance()->GetLocalPlayers();

	for (ULocalPlayer* localPlayer : localPlayers) {

		if (UEnhancedInputLocalPlayerSubsystem * subsystem = ULocalPlayer::GetSubsystem <UEnhancedInputLocalPlayerSubsystem>(localPlayer)) {
			subsystem->AddMappingContext(menuMappingContext,0);
		}

	}


}

// Called every frame
void ALocalMultiplayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (readyCheck[0] && readyCheck[1])
	{
		countdownTimer -= DeltaTime;
	}
	else {
		countdownTimer = COUNTDOWN_AMOUNT;
	}

	if (countdownTimer <= 0)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), gameLevel);
	
	}

}

