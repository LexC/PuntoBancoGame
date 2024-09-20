# Punto Banco Game

## Description
Welcome to the C++ implementation of Punto Banco, a classic casino card game. This game simulates the traditional rules where players bet on either the Player, the Banker (Captain in this case), or a Tie. It's a simple terminal-based game that you can enjoy from the comfort of your computer!

### Versions
- **Full Version**: The full version allows players to bet, track their winnings, and features dynamic text-based feedback during gameplay.
- **Simple Version**: The simple version of the game outputs only the player’s hand, banker’s hand, and the game outcome.

## Features
- Player vs. Captain (Banker) gameplay.
- Betting system where players wager gold coins (in the full version).
- Dynamic card dealing with optional third cards, following standard Punto Banco rules.
- Real-time feedback with win/loss results.

## Repository Structure

The repository is organized as follows:

```
PuntoBancoGame/
│
├── README.md           # Project documentation (this file)
├── src/                # Source files (.cpp)
│   ├── main.cpp        # Full game version
│   └── main_simple.cpp # Simple game version
└── LICENSE             # License for the project
```

## Installation & Compilation

### Prerequisites:
- A C++ compiler (like `g++` or `clang++`).
- Optional: A build toolchain like `make` for easy compilation.

### Steps to Compile:
1. Clone the repository:
    ```bash
    git clone https://github.com/LexC/PuntoBancoGame.git
    cd PuntoBancoGame
    ```

2. Compile the version of your choice:
    - For the **full version**:
      ```bash
      g++ -o punto_banco ./src/main.cpp
      ```
    - For the **simple version**:
      ```bash
      g++ -o punto_banco_simple ./src/main_simple.cpp
      ```

3. Run the game:
    - **Full version**:
      ```bash
      ./punto_banco
      ```
    - **Simple version**:
      ```bash
      ./punto_banco_simple
      ```

## How to Play
- **Full Version**: In the full version, you will be prompted to place bets, and the game will deal cards according to the standard Punto Banco rules, showing a running tally of your coins and asking if you want to play again.
  
- **Simple Version**: The simple version outputs only the player’s hand, banker’s hand, and the game outcome in a single playthrough. 

    For example, the game will display the final hands and outcome in the following format:
    ```cpp
    PlayerHand BankerHand Outcome
    ```

## Contribution
Feel free to fork this repository and submit pull requests if you want to improve the gameplay, add new features, or optimize the code.

## License
This project is licensed under the MIT License.

