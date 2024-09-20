#include <iostream>
#include <random>
#include <chrono>   
#include <string>
#include <vector>
#include <algorithm>
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

// Function to determine the winner
string getResult(int playerTotal, int bankerTotal) {
    if (playerTotal > bankerTotal) return "PLAYER";
    if (bankerTotal > playerTotal) return "BANKER";
    return "TIE";
}

// Function to build a string representation of the hand, with cards separated by commas
string getHand(int card1, int card2, int card3) {
    string hand = string(1, displayCard(card1)) + "," + string(1, displayCard(card2));
    if (card3 != -1) hand += "," + string(1, displayCard(card3));
    return hand;
}

// -----| Show time |-----

int main() {

    int playerCard1, playerCard2, playerCard3 = -1;
    int bankerCard1, bankerCard2, bankerCard3 = -1;
    string PHand, BHand, Outcome;
    vector<int> shoe;

    // Initialize and shuffle a new shoe (deck of cards)
    createShoe(shoe);
    shuffleShoe(shoe);

    // Deal two cards to player and banker
    tie(playerCard1, playerCard2) = deal2cards(shoe);
    tie(bankerCard1, bankerCard2) = deal2cards(shoe);

    // Calculate hand totals
    int playerTotal = calculateHandValue(playerCard1, playerCard2);
    int bankerTotal = calculateHandValue(bankerCard1, bankerCard2);

    // Check for natural
    if (!checkNatural(playerTotal, bankerTotal)) {
        // If no natural, deal third cards to player and banker if necessary
        tie(playerTotal, playerCard3) = dealPlayerCard3(shoe, playerTotal);
        tie(bankerTotal, bankerCard3) = dealBankerCard3(shoe, playerCard3, bankerTotal);
    }

    // Prepare the display of hands
    PHand = getHand(playerCard1, playerCard2, playerCard3);
    BHand = getHand(bankerCard1, bankerCard2, bankerCard3);

    // Get the outcome
    Outcome = getResult(playerTotal, bankerTotal);
    
    // Display the player's and banker's hands and the game outcome
    cout << PHand + " " + BHand + " " + Outcome << endl;

    return 0;
}
