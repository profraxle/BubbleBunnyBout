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
	
	//store all local players
	localPlayers = GetGameInstance()->GetLocalPlayers();

	//set the menu mapping context as the active mapping context
	for (ULocalPlayer* localPlayer : localPlayers) {

		if (UEnhancedInputLocalPlayerSubsystem * subsystem = ULocalPlayer::GetSubsystem <UEnhancedInputLocalPlayerSubsystem>(localPlayer)) {
			subsystem->ClearAllMappings();
			subsystem->AddMappingContext(menuMappingContext,0);
		}

	}


}

// Called every frame
void ALocalMultiplayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if both players are ready count down until game begins
	if (readyCheck[0] && readyCheck[1])
	{
		countdownTimer -= DeltaTime;
	}
	else {
		//set countdown to the countdown amount
		countdownTimer = COUNTDOWN_AMOUNT;
	}

	if (countdownTimer <= 0)
	{
		//load the gameplay level when timer is over
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), gameLevel);
	
	}

}

