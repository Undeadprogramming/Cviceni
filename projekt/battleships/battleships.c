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
    fptr = fopen("player_grid.txt", "r");
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
    printf("Carrier     5\nBattleship  4\nDestroyer   3\nSubmarine   3\nPatrol Boat 2\n\n");
}

void parse_grid(coordinates c)
{
    char fill[256];
    int line = 0;
    int column = 0;
    FILE *fptr;
    fptr = fopen("player_grid.txt", "r+");

    while (fgets(fill, sizeof fill, fptr) != NULL) /* read a line */
    {
        if (line == 5 + (9 - c.y) * 2)
        {
            while (fgetc(fptr) != EOF && line == 5 + (9 - c.y) * 2)
            {
                if (column == 4 + c.x * 4)
                {
                    fputc('S', fptr);
                    break;
                }
                else
                {
                    column++;
                }
            }
            break;
        }
        else
        {
            line++;
        }
    }
    fclose(fptr);
}

void get_coordinates(coordinates c, char ship[12], int player_grid[100])
{
    char delim[] = ",";
    char *ptr = strtok(ship, delim);

    while (ptr != NULL)
    {
        c.x = ptr[0] - 65;
        c.y = ptr[1] - 48;
        player_grid[c.x + (c.y - 1) * 10] = 1;
        parse_grid(c);
        ptr = strtok(NULL, delim);
    }
}

void place_carrier(char *ship)
{
    printf("Carrier (size 5): ");
    scanf("%s", ship);
    if (strlen(ship) != 14)
    {
        printf("Wrong input!");
        place_carrier(ship);
    }
}

void place_battleship(char *ship)
{
    printf("Battleship (size 4): ");
    scanf("%s", ship);
    if (strlen(ship) != 11)
    {
        printf("Wrong input!");
        place_battleship(ship);
    }
}

void place_destroyer(char *ship)
{
    printf("Destroyer (size 3): ");
    scanf("%s", ship);
    if (strlen(ship) != 8)
    {
        printf("Wrong input!");
        place_destroyer(ship);
    }
}

void place_submarine(char *ship)
{
    printf("Submarine (size 3): ");
    scanf("%s", ship);
    if (strlen(ship) != 8)
    {
        printf("Wrong input!");
        place_submarine(ship);
    }
}

void place_patrol(char *ship)
{
    printf("Patrol Boat (size 2): ");
    scanf("%s", ship);
    if (strlen(ship) != 5)
    {
        printf("Wrong input!");
        place_patrol(ship);
    }
}

void place_ships(int player_grid[100])
{
    coordinates c;
    char ship[14];

    printf("Choose the placement for your ships separated by commas (eg. E4,F4,G4,H4 for battleship):\n");

    place_carrier(ship);
    get_coordinates(c, ship, player_grid);
    generate_grid();
    place_battleship(ship);
    get_coordinates(c, ship, player_grid);
    generate_grid();
    place_destroyer(ship);
    get_coordinates(c, ship, player_grid);
    generate_grid();
    place_submarine(ship);
    get_coordinates(c, ship, player_grid);
    generate_grid();
    place_patrol(ship);
    get_coordinates(c, ship, player_grid);
    generate_grid();
}

void wipe_grid()
{
    FILE *fptr1, *fptr2;
    char c;
    fptr1 = fopen("base_grid.txt", "r");
    if (fptr1 == NULL)
    {
        printf("Cannot open file %s \n", "base_grid.txt");
        exit(0);
    }

    fptr2 = fopen("player_grid.txt", "w");
    if (fptr2 == NULL)
    {
        printf("Cannot open file %s \n", "player_grid.txt");
        exit(0);
    }

    // Read contents from file
    c = fgetc(fptr1);
    while (c != EOF)
    {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }

    printf("\nContents copied to %s", "player_grid.txt");

    fclose(fptr1);
    fclose(fptr2);
}

int main()
{
    int mode_choice;
    int player_grid[100] = {0};
    int player_battle_grid[100] = {0};
    int computer_grid[100] = {0};
    int computer_battle_grid[100] = {0};

    wipe_grid();
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
        printf("Wrong input!");
        break;
    }
    return 0;
}
