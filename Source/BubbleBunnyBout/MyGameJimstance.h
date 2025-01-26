// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameJimstance.generated.h"


UCLASS()
class BUBBLEBUNNYBOUT_API UMyGameJimstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UMyGameJimstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> scores;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int scoreToWin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool requireTwoPointLeadForWin;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UWorld> gameLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UWorld> victoryLevel;

	UFUNCTION(BlueprintCallable)
	void ResetGame();
	UFUNCTION(BlueprintCallable)
	void ResetScores();
	UFUNCTION(BlueprintCallable)
	void AwardPoint(int playerID);
	UFUNCTION(BlueprintCallable)
	void CheckForWin();

protected:
};
