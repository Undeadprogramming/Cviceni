#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

void generate_grid()
{
    for (int k = 0; k < 12; k++)
    {
        printf("    ");
        for (int i = 0; i < 11; i++)
        {
            printf("+");
            for (int j = 0; j < 4; j++)
            {
                printf("-");
            }
        }
        printf("+\n");
    }
}

int main()
{
    int mode_choice;
    printf("Welcome to Battleships!\nChoose a gamemode:\nType 1 for singleplayer\nType 2 for multiplayer\n");
    scanf("%d", &mode_choice);

    switch (mode_choice)
    {
    case 1:
        printf("Entering singleplayer mode\n");
        generate_grid();
        break;

    case 2:
        printf("Entering multiplayer mode\n");
        generate_grid();
        break;

    default:
        break;
    }
    return 0;
}