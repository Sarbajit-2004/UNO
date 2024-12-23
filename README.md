# UNO
This Uno game is a command-line implementation of the popular card game, developed in C. It replicates the core gameplay mechanics of Uno, allowing players to experience the fun and strategy of the original game in a simple console interface.

Key features include:

    Game Setup:
        A deck of 108 cards consisting of four colors (red, blue, green, and yellow) with numbers (0-9) and action cards (Reverse, Skip, Draw Two, Wild, and Wild Draw Four).
        Player initialization for 2 or more participants.

    Core Gameplay:
        Players take turns matching cards by number, color, or type with the top card on the discard pile.
        Special cards trigger actions like skipping turns, reversing play direction, or forcing opponents to draw extra cards.
        Wild cards allow players to change the current color or impose penalties.

    Custom Rules and Logic:
        Efficient card drawing and deck reshuffling logic.
        Error handling for invalid moves.
        Automated enforcement of Uno rules, including the "Uno call" mechanic.

    User Interaction:
        A text-based interface displaying the player’s hand, the top card, and available actions.
        Real-time feedback for each player’s move.

    Winning Conditions:
        The game ends when a player discards their last card. Scores may optionally be calculated based on the remaining cards in opponents’ hands.

    Optimized Performance:
        Leveraging C’s low-level capabilities for memory management and efficient operations.
        Modular code structure for scalability and maintenance.

This implementation is an excellent project for practicing advanced programming concepts, such as data structures (linked lists or arrays for cards), file handling (for saving and loading game states), and algorithms for shuffling and turn management.

Whether you’re playing with friends or using it as a solo practice tool, this Uno game provides hours of engaging fun while showcasing the power and flexibility of C programming!
