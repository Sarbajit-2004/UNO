#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Constants
#define MAX_CARDS 108
#define MAX_PLAYERS 4
#define INITIAL_HAND_SIZE 7
#define COLORS 4
#define VALUES 15 // Including special cards

// Card structure
typedef struct {
    char color[10];
    char value[15];
} Card;

// Function prototypes
void initializeDeck(Card deck[]);
void shuffleDeck(Card deck[]);
void dealCards(Card deck[], Card hands[][20], int *deckIndex, int numPlayers, int handSizes[]);
void displayCard(Card card);
void displayHand(Card hand[], int handSize);
int playCard(Card *pile, Card hand[], int *handSize, int chosenIndex);
int canPlay(Card pile, Card hand[], int handSize);
void drawCard(Card deck[], int *deckIndex, Card hand[], int *handSize);
int isMatch(Card pile, Card card);
void handleSpecialCard(Card *pile, Card deck[], int *deckIndex, Card hands[][20], int *handSizes, int *currentPlayer, int numPlayers, int *turnDirection);
void chooseColor(Card *pile);

// Main function
int main() {
    Card deck[MAX_CARDS];
    Card hands[MAX_PLAYERS][20]; // Player hands
    int handSizes[MAX_PLAYERS] = {INITIAL_HAND_SIZE, INITIAL_HAND_SIZE, INITIAL_HAND_SIZE, INITIAL_HAND_SIZE};
    int deckIndex = 0; // Top of the deck
    Card pile; // Card on top of the discard pile
    int currentPlayer = 0;
    int numPlayers;
    int turnDirection = 1; // 1 for clockwise, -1 for counter-clockwise
    int winner = 0;

    srand(time(0));

    // Get number of players
    do {
        printf("Enter the number of players (2-4): ");
        scanf("%d", &numPlayers);
    } while (numPlayers < 2 || numPlayers > MAX_PLAYERS);

    // Initialize and shuffle the deck
    initializeDeck(deck);
    shuffleDeck(deck);

    // Deal cards to players
    dealCards(deck, hands, &deckIndex, numPlayers, handSizes);

    // Initialize the pile
    do {
        pile = deck[deckIndex++];
    } while (strcmp(pile.value, "Wild") == 0 || strcmp(pile.value, "Wild Draw Four") == 0); // Start with a non-wild card
    printf("Starting card: ");
    displayCard(pile);

    // Game loop
    while (!winner) {
        printf("\nPlayer %d's turn:\n", currentPlayer + 1);
        displayHand(hands[currentPlayer], handSizes[currentPlayer]);

        if (canPlay(pile, hands[currentPlayer], handSizes[currentPlayer])) {
            int choice;
            printf("Choose a card to play (1-%d) or 0 to draw: ", handSizes[currentPlayer]);
            scanf("%d", &choice);
            if (choice == 0) {
                drawCard(deck, &deckIndex, hands[currentPlayer], &handSizes[currentPlayer]);
            } else if (playCard(&pile, hands[currentPlayer], &handSizes[currentPlayer], choice - 1)) {
                printf("Player %d played: ", currentPlayer + 1);
                displayCard(pile);
                handleSpecialCard(&pile, deck, &deckIndex, hands, handSizes, &currentPlayer, numPlayers, &turnDirection);
            } else {
                printf("Invalid choice. Try again.\n");
                continue;
            }
        } else {
            printf("No playable cards. Drawing a card...\n");
            drawCard(deck, &deckIndex, hands[currentPlayer], &handSizes[currentPlayer]);
        }

        // Check for a winner
        if (handSizes[currentPlayer] == 0) {
            printf("\nPlayer %d wins!\n", currentPlayer + 1);
            winner = 1;
        }

        // Move to the next player
        currentPlayer = (currentPlayer + turnDirection + numPlayers) % numPlayers;
    }

    return 0;
}

// Function to initialize the deck
void initializeDeck(Card deck[]) {
    const char *colors[] = {"Red", "Green", "Blue", "Yellow"};
    const char *values[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Skip", "Reverse", "Draw Two", "Wild", "Wild Draw Four"};

    int index = 0;
    for (int c = 0; c < COLORS; c++) {
        for (int v = 0; v < VALUES - 2; v++) { // Non-wild cards
            strcpy(deck[index].color, colors[c]);
            strcpy(deck[index].value, values[v]);
            index++;
            if (v != 0) { // Duplicate cards (except for 0)
                strcpy(deck[index].color, colors[c]);
                strcpy(deck[index].value, values[v]);
                index++;
            }
        }
    }

    // Wild cards
    for (int i = 0; i < 4; i++) {
        strcpy(deck[index].color, "Wild");
        strcpy(deck[index].value, "Wild");
        index++;
        strcpy(deck[index].color, "Wild");
        strcpy(deck[index].value, "Wild Draw Four");
        index++;
    }
}

// Function to shuffle the deck
void shuffleDeck(Card deck[]) {
    for (int i = MAX_CARDS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Function to deal cards to players
void dealCards(Card deck[], Card hands[][20], int *deckIndex, int numPlayers, int handSizes[]) {
    for (int i = 0; i < numPlayers; i++) {
        for (int j = 0; j < INITIAL_HAND_SIZE; j++) {
            hands[i][j] = deck[(*deckIndex)++];
        }
    }
}

// Function to display a card
void displayCard(Card card) {
    printf("%s %s\n", card.color, card.value);
}

// Function to display a hand
void displayHand(Card hand[], int handSize) {
    for (int i = 0; i < handSize; i++) {
        printf("%d. ", i + 1);
        displayCard(hand[i]);
    }
}

// Function to play a card
int playCard(Card *pile, Card hand[], int *handSize, int chosenIndex) {
    if (isMatch(*pile, hand[chosenIndex])) {
        *pile = hand[chosenIndex];
        for (int i = chosenIndex; i < *handSize - 1; i++) {
            hand[i] = hand[i + 1];
        }
        (*handSize)--;
        return 1;
    }
    return 0;
}

// Function to check if a card can be played
int canPlay(Card pile, Card hand[], int handSize) {
    for (int i = 0; i < handSize; i++) {
        if (isMatch(pile, hand[i])) {
            return 1;
        }
    }
    return 0;
}

// Function to draw a card
void drawCard(Card deck[], int *deckIndex, Card hand[], int *handSize) {
    hand[(*handSize)++] = deck[(*deckIndex)++];
}

// Function to check if a card matches the pile
int isMatch(Card pile, Card card) {
    return (strcmp(pile.color, card.color) == 0 || strcmp(pile.value, card.value) == 0 || strcmp(card.color, "Wild") == 0);
}

// Function to handle special card effects
void handleSpecialCard(Card *pile, Card deck[], int *deckIndex, Card hands[][20], int *handSizes, int *currentPlayer, int numPlayers, int *turnDirection) {
    if (strcmp(pile->value, "Skip") == 0) {
        *currentPlayer = (*currentPlayer + *turnDirection + numPlayers) % numPlayers;
    } else if (strcmp(pile->value, "Reverse") == 0) {
        *turnDirection *= -1;
    } else if (strcmp(pile->value, "Draw Two") == 0) {
        int nextPlayer = (*currentPlayer + *turnDirection + numPlayers) % numPlayers;
        drawCard(deck, deckIndex, hands[nextPlayer], &handSizes[nextPlayer]);
        drawCard(deck, deckIndex, hands[nextPlayer], &handSizes[nextPlayer]);
        *currentPlayer = nextPlayer; // Skip the next player's turn
    } else if (strcmp(pile->value, "Wild") == 0 || strcmp(pile->value, "Wild Draw Four") == 0) {
        chooseColor(pile);
        if (strcmp(pile->value, "Wild Draw Four") == 0) {
            int nextPlayer = (*currentPlayer + *turnDirection + numPlayers) % numPlayers;
            drawCard(deck, deckIndex, hands[nextPlayer], &handSizes[nextPlayer]);
            drawCard(deck, deckIndex, hands[nextPlayer], &handSizes[nextPlayer]);
            drawCard(deck, deckIndex, hands[nextPlayer], &handSizes[nextPlayer]);
            drawCard(deck, deckIndex, hands[nextPlayer], &handSizes[nextPlayer]);
            *currentPlayer = nextPlayer; // Skip the next player's turn
        }
    }
}

// Function to choose a color for Wild cards
void chooseColor(Card *pile) {
    int choice;
    printf("Choose a color (1. Red, 2. Green, 3. Blue, 4. Yellow): ");
    scanf("%d", &choice);
    switch (choice) {
        case 1: strcpy(pile->color, "Red"); break;
        case 2: strcpy(pile->color, "Green"); break;
        case 3: strcpy(pile->color, "Blue"); break;
        case 4: strcpy(pile->color, "Yellow"); break;
        default: strcpy(pile->color, "Red"); break; // Default to Red
    }
}
