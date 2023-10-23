#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

void generate_grid()
{
    FILE *fptr;
    char check;
    fptr = fopen("base_grid.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    check = fgetc(fptr);
    while (check != EOF)
    {
        printf("%c", check);
        check = fgetc(fptr);
    }

    fclose(fptr);
    printf("\n");
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
