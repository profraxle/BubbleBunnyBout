// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameJimstance.h"

UMyGameJimstance::UMyGameJimstance() {

}

void UMyGameJimstance::ResetGame() {
	ResetScores();
	if (UFunction* resetLevelFunction = FindFunction(TEXT("ResetLevel"))) {
		ProcessEvent(resetLevelFunction, nullptr);
	}
}

void UMyGameJimstance::ResetScores() {
	scores = { 0, 0 };
}

void UMyGameJimstance::AwardPoint(int playerID) {
	scores[playerID]++;
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, FString::Printf(TEXT("Scores are: %i, %i"), scores[0], scores[1]));
	CheckForWin();
	if (UFunction* resetLevelFunction = FindFunction(TEXT("ResetLevel"))) {
		ProcessEvent(resetLevelFunction, nullptr);
	}
	
}

void UMyGameJimstance::CheckForWin() {
	if (scores[0] >= scoreToWin && (!requireTwoPointLeadForWin || scores[0] - 1 > scores[1])) {
		// do shit
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, TEXT("Player 0 Wins"));
		ResetGame();
	}
	else if (scores[1] >= scoreToWin && (!requireTwoPointLeadForWin || scores[1] - 1 > scores[0])) {
		// do other shit
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, TEXT("Player 1 Wins"));
		ResetGame();
	}
	else {
		/*if (UFunction* resetLevelFunction = FindFunction(TEXT("ResetLevel"))) {
			ProcessEvent(resetLevelFunction, nullptr);
		}*/
	}
}