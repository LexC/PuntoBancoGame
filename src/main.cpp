#include <iostream>
#include <random>
#include <chrono>   
#include <string>
#include <vector>
#include <algorithm>
#include <cctype> 
#include <limits>
#include <thread>

using namespace std;

constexpr int DECK_COUNT = 8; // Number of decks in the shoe

// -----| Functions for the Shoe |-----

// Function to generate a single 52-card deck (Each card appears 4 times)
void createDeck(vector<int> &deck) {
    for (int i = 1; i <= 13; ++i) {
        for (int j = 0; j < 4; ++j) {
            deck.push_back(i);
        }
    }
}

// Function to generate a shoe (8 decks)
void createShoe(vector<int> &shoe) {
    shoe.reserve(DECK_COUNT * 52); 
    for (int i = 0; i < DECK_COUNT; ++i) {
        createDeck(shoe);
    }
}

// Function to shuffle the shoe
void shuffleShoe(vector<int> &shoe) {

    random_device rd;
    
    auto now = chrono::high_resolution_clock::now();
    auto timeSeed = now.time_since_epoch().count();  // Get time in nanoseconds or microseconds
    // Combine rd() with a hashed time value and add a left bit-shift for extra entropy
    auto seed = rd() ^ (hash<long long>{}(timeSeed) << 1);  
    
    mt19937 generator(seed); 
    shuffle(shoe.begin(), shoe.end(), generator); 
    
}

// Function to draw a card from the shoe
int drawCard(vector<int> &shoe) {
    int card = shoe.back();
    shoe.pop_back();
    return card;
}

// -----| Functions for the Cards |-----

// Function to get the card value (face cards and 10s are worth 0, others worth their value)
int getCardValue(int card) {
    if (card > 9) {return 0;}
    return card;
}

// Function to display the card
char displayCard(int card) {
    if (card == 1) return 'A';
    if (card == 10) return 'T';
    if (card == 11) return 'J';
    if (card == 12) return 'Q';
    if (card == 13) return 'K';
    return '0' + card; 
}

// Function to deal two cards
pair<int,int> deal2cards(vector<int> &shoe) {
    return {drawCard(shoe), drawCard(shoe)};
}

// Function to calculate the hand value
int calculateHandValue(int card1, int card2) {
    // Card values sum up and only the last digit matters (mod 10)
    return (getCardValue(card1) + getCardValue(card2)) % 10;
}

// Function to deal a third card, returns a pair with the new total and third card
pair<int, int> dealCard3(vector<int> &shoe, int total) {
    int card3 = drawCard(shoe);
    int newTotal = calculateHandValue(total, card3);
    return {newTotal, card3};
}

// Function to deal the player's third card
pair<int, int> dealPlayerCard3(vector<int> &shoe, int playerTotal) {
    if (playerTotal <= 5) {
        return dealCard3(shoe,playerTotal);
    }
    return {playerTotal, -1}; // No third card dealt
}

// Function to deal the banker's third card
pair<int, int> dealBankerCard3(vector<int> &shoe, int playerCard3, int bankerTotal) {
	if (playerCard3 == -1 && bankerTotal <= 5){
        return dealCard3(shoe,bankerTotal);
    }
    else {
        if (bankerTotal <= 2 || 
            (bankerTotal == 3 && playerCard3 != 8) ||
            (bankerTotal == 4 && playerCard3 >= 2 && playerCard3 <= 7) ||
            (bankerTotal == 5 && playerCard3 >= 4 && playerCard3 <= 7) ||
            (bankerTotal == 6 && playerCard3 == 6 || playerCard3 == 7)) {
            return dealCard3(shoe,bankerTotal);
        }
    }
    return {bankerTotal, -1}; // No third card dealt
}

// -----| Functions for the Results |-----

// Function to check for a natural (player or banker total 8 or 9)
bool checkNatural(int playerTotal, int bankerTotal) {
    return (playerTotal >= 8 || bankerTotal >= 8);
}

// Function to build a string representation of the hand, with cards separated by commas
string getHand(int card1, int card2, int card3) {
    string hand = string(1, displayCard(card1)) + "," + string(1, displayCard(card2));
    if (card3 != -1) hand += "," + string(1, displayCard(card3));
    return hand;
}

// -----| New Funtions |-----

int getNumberInput(string mTryAgain) {
    int userInput;
    
    while (true) {
        cin >> userInput;

        // Check if the input stream is in a fail state (i.e., user did not enter an integer)
        if (cin.fail()) {
            cin.clear();  // Clear the error flag on the input stream
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
            cout << mTryAgain;
            
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore any leftover input
            return userInput;  // Return the valid integer
        }
    }
}

char getValidatedBet(string mTryAgain) {
    char userInput;

    while (true) {
        cin >> userInput;

        // Convert input to lowercase for case-insensitive comparison
        userInput = tolower(userInput);

        // Check if the input is valid
        if (userInput == 'p' || userInput == 'c' || userInput == 't') {
            return userInput;  // Return the valid input
        } else {
            cout << mTryAgain;
            // Clear any leftover invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void addDelay(int seconds) {
    this_thread::sleep_for(chrono::seconds(seconds));
}


// -----| Show time |-----

int main() {
    char startPlay, playAgain='y';

    string mOpening, mWager, mBet1, mBet2;
    string mDraw2player, mDraw2banker;
    string mHandPlayer1, mHandPlayer2, mHandBanker1, mHandBanker2;
    string mWinPlayer, mWinBanker, mWinTie;
    string mBetWin1, mBetWin2, mBetLose;
    string mPlayAgain, mBye;
    string mWrongInputInt, mWrongInputBet;
    
    
    mOpening = "\nFancy a quick round of Punto Banco, mate? (y/n): ";
    mWager   = "\nHow much of yer precious gold coins are ye willin' to wager? ";
    mBet1    = "\nWhere be your bet, brave one? On yourself, yours trully mighty Captain, or do you fancy the winds will call it even?";
    mBet2    = "\nMake yer pick: write 'p' for Player, 'c' for Captain, or 't' for Tie!";
    
    mDraw2player = "\nThe Captain draws another card for you.";
    mDraw2banker = "\nThe Captain draws another card for himself.";

    mHandPlayer1 = "\nYour hand:          ";
    mHandBanker1 = "\nThe Captain's hand: ";
    mHandPlayer2 = "\nYour full hand:          ";
    mHandBanker2 = "\nThe Captain's full hand: ";
    
    mWinPlayer = "\n\nA fine victory for the Player!";
    mWinBanker = "\n\nThe Captain claims victory once again!";
    mWinTie    = "\n\nAye, it's a Tie! Fortune couldn’t make up its mind this time, savvy?";

    mBetWin1 = "\nWell done, matey! Ye won the bet and now sit on a fine hoard of ";
    mBetWin2 = " gold coins!";
    mBetLose = "\nAh, tough luck, mate. Ye lost your coins this round, but there's always another sail on the horizon.";

    mPlayAgain = "\n\nFeel like trying yer luck again, my friend? (y/n): ";
    mBye = "\nA grand game, mate! 'Til we meet again—I’ve got treasures aplenty waitin’ for ye next time!\n\n\n";

    mWrongInputInt = "Ah, seems like ye fumbled the numbers. Try again, and give me a proper value of coins.\n";
    mWrongInputBet = "A wrong choice, mate! Bet again and make sure to pick from 'p', 'c', or 't'—Player, Captain, or Tie!\n";

    int whatingTime = 2; //seconds

    cout << mOpening;
    cin >> startPlay;

    while (tolower(startPlay) == 'y' && tolower(playAgain) == 'y') {
        
        int playerCard1, playerCard2, playerCard3 = -1;
        int bankerCard1, bankerCard2, bankerCard3 = -1;
        int amount0, amount1;
        char bet, outcome;
        string PHand, BHand;
        vector<int> shoe;

        cout << mWager;
        amount0 = getNumberInput(mWrongInputInt);

        cout << mBet1;
        cout << mBet2 << endl;
        bet = getValidatedBet(mWrongInputBet);
        
        // Initialize and shuffle a new shoe (deck of cards)
        createShoe(shoe);
        shuffleShoe(shoe);

        // Deal two cards to player and banker
        tie(playerCard1, playerCard2) = deal2cards(shoe);
        tie(bankerCard1, bankerCard2) = deal2cards(shoe);

        // Calculate hand totals
        int playerTotal = calculateHandValue(playerCard1, playerCard2);
        int bankerTotal = calculateHandValue(bankerCard1, bankerCard2);

        // Show player's and banker's hands before third card
        addDelay(whatingTime/5);
        cout << mHandPlayer1 << getHand(playerCard1, playerCard2, playerCard3);
        addDelay(whatingTime/2);
        cout << mHandBanker1 << getHand(bankerCard1, bankerCard2, bankerCard3);

        // Check for natural
        if (!checkNatural(playerTotal, bankerTotal)) {
            // If no natural, deal third cards to player and banker if necessary
            tie(playerTotal, playerCard3) = dealPlayerCard3(shoe, playerTotal);
            tie(bankerTotal, bankerCard3) = dealBankerCard3(shoe, playerCard3, bankerTotal);
        }

        // Final hands
        int u = 1;
        if (playerCard3 != -1){
            addDelay(whatingTime/u);u += 1;
            cout << mDraw2player;
        }
        if (bankerCard3 != -1){
            addDelay(whatingTime/u);u += 1;
            cout << mDraw2banker;
        }

        if (playerCard3 != -1){
            addDelay(whatingTime/u);u += 1;
            cout << mHandPlayer2 << getHand(playerCard1, playerCard2, playerCard3);
        }
        if (bankerCard3 != -1){
            addDelay(whatingTime/u);u += 1;
            cout << mHandBanker2 << getHand(bankerCard1, bankerCard2, bankerCard3);
        }

        if (playerTotal > bankerTotal){
            outcome = 'p';
            addDelay(whatingTime);
            cout << mWinPlayer;
        }
        else if (playerTotal < bankerTotal){
            outcome = 'c';
            addDelay(whatingTime);
            cout << mWinBanker;
        }
        else {
            outcome = 't';

            addDelay(whatingTime);
            cout << mWinTie;
        }
        
        if (outcome == bet){
            if (bet == 'p'){amount1 = amount0 * 2;}
            else if (bet == 'c'){amount1 = amount0 * (2 - 0.05);}
            else {amount1 = amount0 * 8;}
            
            cout << mBetWin1 << amount1 << mBetWin2;
        }
        else {
            cout << mBetLose;

        }

        // Ask if player wants to play again
            
        cout << mPlayAgain;
        cin >> playAgain;

    } ;

    // Thank the player for playing
    cout << mBye;

    return 0;
}
