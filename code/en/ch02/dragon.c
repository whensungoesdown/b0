/* A little game: The Dragon's Cave
 * Uses only what this chapter covers: int variables, arithmetic,
 * printf/scanf, if/else, while, for and break */
#include <stdio.h>

int main(void)
{
    int room = 1;       /* where you are: 1 entrance, 2 fork, 3 room, 4 lair */
    int choice;         /* the player's choice */
    int has_key = 0;    /* do you have the key? 0 no, 1 yes */
    int torch = 12;     /* how many more steps the torch will last */
    int playing = 1;    /* is the game still going? 1 yes, 0 no */
    int answer;
    int won = 0;
    int i;

    printf("====== The Dragon's Cave ======\n");
    printf("Legend says a chest of gold is hidden in the dragon's cave.\n");
    printf("Torch in hand, you stand at the entrance.\n");

    while (playing) {
        /* Step 1: describe where you are and list what you can do */
        printf("\n(Your torch will last %d more steps)\n", torch);
        if (room == 1) {
            printf("You are at the entrance. It is dark. A cold wind blows.\n");
            printf("1. Go into the cave   2. Forget it, go home\n");
        } else if (room == 2) {
            printf("You reach a fork. On the left is a small wooden door.\n");
            printf("From the right comes the sound of snoring.\n");
            printf("1. Open the door   2. Go right   3. Back to the entrance\n");
        } else if (room == 3) {
            printf("A small, dusty room. ");
            if (has_key == 0) {
                printf("A rusty key hangs on the wall.\n");
            } else {
                printf("The wall is bare.\n");
            }
            printf("1. Take the key   2. Back to the fork\n");
        } else {
            printf("The dragon's lair! A big red dragon is asleep on a chest.\n");
            printf("1. Open the chest   2. Wake the dragon   3. Tiptoe back\n");
        }

        /* Step 2: read the player's choice */
        printf("> ");
        if (scanf("%d", &choice) != 1) {
            printf("I don't understand your choice. Game over.\n");
            break;
        }
        torch = torch - 1;

        /* Step 3: update the game, based on where you are and what you chose */
        if (room == 1) {
            if (choice == 1) {
                room = 2;
            } else if (choice == 2) {
                printf("You go home to bed. Treasure? Maybe tomorrow.\n");
                playing = 0;
            } else {
                printf("There is no such choice.\n");
            }
        } else if (room == 2) {
            if (choice == 1) {
                room = 3;
            } else if (choice == 2) {
                room = 4;
            } else if (choice == 3) {
                room = 1;
            } else {
                printf("There is no such choice.\n");
            }
        } else if (room == 3) {
            if (choice == 1 && has_key == 0) {
                printf("You put the key in your pocket.\n");
                has_key = 1;
            } else if (choice == 1) {
                printf("The key is already in your pocket.\n");
            } else if (choice == 2) {
                room = 2;
            } else {
                printf("There is no such choice.\n");
            }
        } else {
            if (choice == 1 && has_key == 1) {
                printf("You unlock the chest. Gold! The dragon snores on.\n");
                won = 1;
                playing = 0;
            } else if (choice == 1 || choice == 2) {
                if (choice == 1) {
                    printf("It's locked. You tug at it. CRACK! The dragon wakes.\n");
                } else {
                    printf("You poke the dragon's nose. It opens one eye.\n");
                }
                printf("The dragon says: \"I only like people who are good at math.\n");
                printf("What is 1 + 2 + ... + 100? You get 3 tries.\"\n");
                for (i = 1; i <= 3; i++) {
                    answer = 0;
                    printf("Try %d > ", i);
                    if (scanf("%d", &answer) != 1) {
                        break;
                    }
                    if (answer == 5050) {
                        printf("The dragon says: \"Not bad!\"\n");
                        printf("It moves aside and lets you take the chest.\n");
                        won = 1;
                        break;
                    } else if (answer > 5050) {
                        printf("The dragon says: \"Too big.\"\n");
                    } else {
                        printf("The dragon says: \"Too small.\"\n");
                    }
                }
                if (won == 0) {
                    printf("The dragon yawns and blows you out of the cave.\n");
                }
                playing = 0;
            } else if (choice == 3) {
                room = 2;
            } else {
                printf("There is no such choice.\n");
            }
        }

        /* when the torch burns out, the game is over too */
        if (playing && torch == 0) {
            printf("Your torch goes out... Darkness. You feel your way out\n");
            printf("of the cave, empty-handed.\n");
            playing = 0;
        }
    }

    if (won) {
        printf("\n*** You win! You took %d steps. ***\n", 12 - torch);
    } else {
        printf("\nGame over. Come back another time!\n");
    }
    return 0;
}
