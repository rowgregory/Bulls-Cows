/* This is the console executable that makes use of the BullCow class
This acts as athe view in a MVC patter, and is responsible for all
user interaction. For game logic see the FBullCowGame class.
*/
#pragma once
#include <iostream>
#include <string>
#include "FBullCowGame.h"	// game class

// to make syntax Unreal friendly
using FText = std::string;
using int32 = size_t;

// function prototypes as outside a class
void PrintIntro();
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame; // instantiate a new game, which we re-use across plays

// entry 
int main() {

	bool bPlayAgain = false;
	do {
		PrintIntro();
		PlayGame();

		bPlayAgain = AskToPlayAgain(); 
	} while (bPlayAgain);

	return 0;	// exit the application
}

void PrintIntro() {
	// introduce the game
	int32 GetCurrentWordLength = BCGame.GetHiddenWordLength();
	std::cout << R"(
__    __ ____  _     ____  ____  __  __  ____ 
\ \/\/ /| ===|| |__ / (__`/ () \|  \/  || ===|
 \_/\_/ |____||____|\____)\____/|_|\/|_||____|

)";
	std::cout << std::endl;
	std::cout <<
		R"(
 .-=-.                .-=-.            .=     ,        =.
  `) )              ( (`      _  _   /'/    )\,/,/(_   \ \
 .' /                \ '.      `//-.|  (  ,\\)\//\)\/_  ) |
(  (   _.-'`::`'-._   )  )     //___\   `\\\/\\/\/\\///'  /
 \  `'`    .::.    `'`  /   ,-"~`-._ `"--'_   `"""`  _ \`'"~-,_
  `-._\`~`      `~`/_.-'    \       `-.  '_`.      .'_` \ ,-"~`/
 _,=` /            \ `=,_    `.__.-'`/   (-\        /-) |-.__,'
(    ,(^\        /^),    )     ||   |     \O)  /^\ (O/  |
 `~~` \0)\      /(0/ `~~`      `\\  |         /   `\    /
       | /`    '\ |              \\  \       /      `\ /
       |          |               `\\ `-.  /' .---.--.\
       |          |                 `\\/`~(, '()      ('
       |          |                  /(O) \\   _,.-.,_)
       /          \                 //  \\ `\'`      /
       \()      ()/               / |  ||   `""""~"`
        `._-==-_.'               /'  |__||
           `""`                        `o
           BULLS           AND                 COWS

                   A funny word game

)";

	std::cout << "Can you guess the " << GetCurrentWordLength;
	std::cout << " letter isogram I'm thinking of?\n";

	return;
}

// plays a single game to completion
void PlayGame() {
	
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();

	// loop asking for guesses while the game 
	// is NOT won and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries) {
		FText Guess = GetValidGuess(); 

		// submit valid guess to the game and receive counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls << ",";
		std::cout << " Cows = " << BullCowCount.Cows << "\n\n";
	}

	PrintGameSummary();
	return;
}

// loop continually until the user gives a cvalid guess
FText GetValidGuess() {

	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		//get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries();
		std::cout << ". Enter a word:";
		std::getline(std::cin, Guess);

		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status) {
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters.\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}

	} while (Status != EGuessStatus::OK);	// keep looping until we get no errors
	return Guess;
}


bool AskToPlayAgain() {

	std::cout << "Do you want to play again with the same hidden word (y/n)?";
	FText Response = "";
	std::getline(std::cin, Response);

	return (Response[0] == 'y' || Response[0] == 'Y'); 
}

void PrintGameSummary() {

	if (BCGame.IsGameWon()) {
		std::cout << "YOU WON!\n";
	}
	else {
		std::cout << "Better luck next time!\n";
	}
}



