#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

typedef struct coordinates
{
    int x;
    int y;
} coordinates;

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
void print_info()
{
    printf("\nIn battleships, there are 5 types of ships for you to place taking various amount of squares:\n");
    printf("\nName        Size\n");
    printf("Carrier     5\nBattleship  4\nDestroyer   3\nSubmarine   3\nPatrol Boat 2\n");
}

void place_ships(int player_grid[100])
{
    coordinates c;
    char ship[12];
    char delim[] = ",";

    printf("Choose the placement for your ships separated by commas (eg. E4,F4,G4,H4 for destroyer):");
    printf("Carrier (size 5): ");
    scanf("%s", ship);

    char *ptr = strtok(ship, delim);

    while (ptr != NULL)
    {
        c.x = ptr[0] - 65;
        c.y = ptr[1] - 48;
        // printf("letter '%d'\n", c.x);
        // printf("number '%d'\n", c.y);
        player_grid[(c.x - 1) * 2 + c.y] = 1;
        ptr = strtok(NULL, delim);
    }
}

int main()
{
    int mode_choice;
    int player_grid[100] = {0};
    int player_battle_grid[100] = {0};
    int computer_grid[100] = {0};
    int computer_battle_grid[100] = {0};

    char str[2] = "E4";
    char *ptr;
    long ret;
    ret = strtol(str, &ptr, 10);
    printf("strtol: %ld\n", ret);
    printf("Welcome to Battleships!\nChoose a gamemode:\nType 1 for singleplayer\nType 2 for multiplayer\n");
    scanf("%d", &mode_choice);

    switch (mode_choice)
    {
    case 1:
        printf("Entering singleplayer mode\n");
        generate_grid();
        print_info();
        place_ships(player_grid);
        break;

    case 2:
        printf("Entering multiplayer mode\n");
        generate_grid();
        print_info();
        place_ships(player_grid);
        break;

    default:
        break;
    }
    return 0;
}
