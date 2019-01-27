#pragma once
#include "FBullCowGame.h"
#include <map>

// to make syntax Unreal friendly
#define TMap std::map	
using int32 = size_t;

FBullCowGame::FBullCowGame() {	// default constructor
	Reset();
}


int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const { 
	TMap<int32, int32> WordLengthToMaxTries{ { 3, 4 }, { 4, 7 }, { 5, 10 }, { 6, 15 }, { 7, 20 } };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

void FBullCowGame::Reset() {

	const char* WordBank[7] = { "plant", "taken", "phone", "money", "clean", "aches", "blank" };	// these MUST be isograms

	const char* random;
	random = WordBank[rand() % 7];

	const FString HIDDEN_WORD = random;
	MyHiddenWord = HIDDEN_WORD;
	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}


EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const {

	if (!IsIsogram(Guess)) {	// if this guess isn't an isogram

		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess)) {	// if the guess isn't all loswercase

		return EGuessStatus::Not_Lowercase;
	}
	else if ( Guess.length() != GetHiddenWordLength() ) {	// if the guess length is wrong, return an error
		
		return EGuessStatus::Wrong_Length;
	}
	else {

		return EGuessStatus::OK;	// otherwise retun OK
	}
}


// receives a valid guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess) {

	MyCurrentTry++;

	// setup a return variable
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length();	// assuming same length as guess

	// loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++) {
		// compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++) {
			// if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar]) {
				if (MHWChar == GChar) {
					BullCowCount.Bulls++; // increment bulls if they're in the same place
				}
				else {
					BullCowCount.Cows++; // increment cows if not
				}
			}		
		}
	}
	if (BullCowCount.Bulls == WordLength) {
		bGameIsWon = true;
	}
	else {
		bGameIsWon = false;
	}
	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const {

	// places word in hash table
	// goes through each letter and marks it
	// if mark has more than one, not isogram

	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen;	// setup our map
	for (auto Letter : Word) {	// for all letters of the word

		Letter = tolower(Letter);

		if (LetterSeen[Letter]) { 
			return false; //we do NOT have an isogram
		}
		else {
			LetterSeen[Letter] = true;
		}
	}
	return true;	// for example in cases were \0 is entered
}

bool FBullCowGame::IsLowercase(FString Word) const {


	for (auto Letter : Word) {

		if (isupper(Letter)) { // could also use islower()
			return false;
		}
	}

	return true;
}
