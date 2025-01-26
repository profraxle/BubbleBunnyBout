// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameJimstance.h"

UMyGameJimstance::UMyGameJimstance() {

}

void UMyGameJimstance::ResetGame() {
	ResetScores();
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), victoryLevel);
}

void UMyGameJimstance::ResetScores() {
	scores = { 0, 0 };
}

void UMyGameJimstance::AwardPoint(int playerID) {
	scores[playerID]++;
}

void UMyGameJimstance::CheckForWin() {
	if (scores[0] >= scoreToWin && (!requireTwoPointLeadForWin || scores[0] - 1 > scores[1])) {
		// do shit
		ResetGame();
	}
	else if (scores[1] >= scoreToWin && (!requireTwoPointLeadForWin || scores[1] - 1 > scores[0])) {
		// do other shit
		ResetGame();
	}
	else {
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), gameLevel);
	}
}