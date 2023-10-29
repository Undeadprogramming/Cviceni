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

void generate_grid(int player)
{ // prints grid from txt file to console
    FILE *fptr;
    char check;
    int count = 0;
    if (player == 1)
    {
        fptr = fopen("player_grid.txt", "r"); // depending on player's turn prints the desired txt file
    }

    else if (player == 2)
        fptr = fopen("computer_grid.txt", "r"); // depending on player's turn prints the desired txt file

    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    check = fgetc(fptr);
    while (check != EOF)
    {
        count++;
        if (check == 'X')
        {
            printf("\033[0;31m%c", check); // prints "X" in red color
            printf("\033[0;m");            // sets color to deafult
        }
        else if (check == 'O' && count > 50)
        {
            printf("\033[0;34m%c", check); // prints "O" in blue color
            printf("\033[0;m");            // sets color to deafult
        }
        else
            printf("%c", check); // prints every other symbol but X or O

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

int parse_grid(coordinates c, int player, int type)
{ // searches txt file for a place to put "S" ships, "X" hits or "O" misses
    char fill[256];
    int line = 0;
    int column = 0;
    char check;
    FILE *fptr;
    if (player == 1)
    {
        fptr = fopen("player_grid.txt", "r+");
    }
    else
        fptr = fopen("computer_grid.txt", "r+");

    while (fgets(fill, sizeof fill, fptr) != NULL) // parses the txt file line by line
    {

        if (line == 5 + (8 - c.y) * 2) // finds the right line
        {
            check = fgetc(fptr); // parses the txt file symbol by symbol
            while (check != EOF && line == 5 + (8 - c.y) * 2)
            {
                if (column == 5 + c.x * 4) // finds the right column
                {
                    fseek(fptr, -1, SEEK_CUR); // offsets the pointer in file to put the symbol correctly
                    if (type == 1)
                    {
                        if (check != 'S')
                        {
                            fputc('S', fptr);
                        }
                    }
                    else
                    {
                        if (check == 'S')
                        {
                            fputc('X', fptr);
                            fclose(fptr);
                            return 1; // return for combat function
                        }
                        else if (check == 'X' || check == 'O')
                        {
                            fclose(fptr);
                            return 2; // return for combat function
                        }
                        else
                        {
                            fputc('O', fptr);
                            fclose(fptr);
                            return 0; // return for combat function
                        }
                    }
                    break;
                }
                else
                {
                    column++;
                }
                check = fgetc(fptr);
            }
            break;
        }
        else
        {
            line++;
        }
    }
    fclose(fptr);
    return 0;
}

void parse_enemy_grid(coordinates c, int player, int hit)
{ // same as parse grid but for different part of the file, searches txt file for a place to put, "X" hits or "O" misses
    char fill[256];
    int line = 0;
    int column = 0;
    char check;
    FILE *fptr;
    if (player == 1)
    {
        fptr = fopen("player_grid.txt", "r+");
    }
    else
        fptr = fopen("computer_grid.txt", "r+");

    while (fgets(fill, sizeof fill, fptr) != NULL)
    {
        if (line == 5 + (8 - c.y) * 2)
        {

            while (check = fgetc(fptr) != EOF && line == 5 + (8 - c.y) * 2)
            {
                if (column == 65 + c.x * 4)
                {

                    if (hit == 1)
                    {
                        fputc('X', fptr);
                        fclose(fptr);
                        return;
                    }
                    else if (hit == 2)
                    {
                        fclose(fptr);
                        return;
                    }
                    else
                    {
                        fputc('O', fptr);
                        fclose(fptr);
                        return;
                    }

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

void get_coordinates(coordinates c, char ship[14], int player)
{ // converts string of coordinates to a usable structure
    char delim[] = ",";
    char *ptr = strtok(ship, delim);

    while (ptr != NULL)
    {
        c.x = ptr[0] - 65;
        c.y = ptr[1] - 48;
        parse_grid(c, player, 1); // searches grid with converted coordinates
        ptr = strtok(NULL, delim);
    }
}

bool letter_coords(char *ship, bool letter_id)
{
    for (int i = 3; i < strlen(ship); i += 3)
    {
        if (ship[i] < 65 || ship[i] > 74) // checks if coordinates contain possible letters "A" to "J"
        {
            printf("Wrong letter input!\n");
            return true;
        }

        if (letter_id) // checks if whole ship is horizontal
        {
            if (ship[i] != ship[0])
            {
                printf("Ships cannot be placed diagonally!\n");
                return true;
            }
        }
        else
        {

            if (ship[i] > ship[i - 3] + 1 || ship[i] < ship[i - 3] - 1) // checks if coordinates increment/decrement by one eg. E4,D4 instead of E4,G4 etc.
            {
                printf("Coordinates must be adjecent letters ship i: %c ship 3: %c!\n", ship[i], ship[i - 3]);
                return true;
            }
        }
    }
    return false;
}

bool number_coords(char *ship, bool letter_id)
{
    for (int i = 4; i < strlen(ship); i += 3)
    {
        if (ship[i] < 48 || ship[i] > 57) // checks if coordinates contain possible numbers "0" to "9"
        {
            printf("Wrong number input!\n");
            return true;
        }
        if (!letter_id)
        {
            if (ship[i] != ship[1]) // checks if whole ship is vertical
            {
                printf("Ships cannot be placed diagonally!\n");
                return true;
            }
        }
        else
        {

            if (ship[i] > ship[i - 3] + 1 || ship[i] < ship[i - 3] - 1) // checks if coordinates increment/decrement by one eg. E4,E5 instead of E4,E7 etc.
            {
                printf("Coordinates must be adjecent number ship i: %c ship 4: %c!\n", ship[i], ship[i - 3]);
                return true;
            }
        }
    }
    return false;
}

bool check_ship(char *ship) // TODO NON STACKABLE SHIPS, PARSE GRID AND RETURN FALSE IF CHECK == "S"
{                           // checks if input string matches the restrictions
    bool letter_id = false;

    if (ship[0] == ship[3]) // checks if the ship is placed horizontally
    {
        letter_id = true;
    }

    if (ship[1] != ship[4] && !letter_id && strlen(ship) > 2) // checks if ship is placed vertically
    {
        printf("Ships cannot be placed diagonally!\n");
        return true;
    }
    printf("Ship: %s\n", ship);

    if (letter_coords(ship, letter_id) || number_coords(ship, letter_id))
        return true;

    return false;
}
void place_carrier(char *ship)
{ // asks for coordinates of given ship and places it on the grid
    printf("Carrier (size 5): ");
    scanf("%s", ship);

    if (check_ship(ship)) // checks input
    {
        place_carrier(ship); // recursively calls itself to ask for coordinates
        return;
    }

    if (strlen(ship) != 14) // checks if ship has correct length
    {
        printf("Wrong input!\n");
        place_carrier(ship); // recursively calls itself to ask for coordinates
    }
}

void place_battleship(char *ship)
{ // asks for coordinates of given ship and places it on the grid
    printf("Battleship (size 4): ");
    scanf("%s", ship);

    if (check_ship(ship)) // checks input
    {
        place_battleship(ship); // recursively calls itself to ask for coordinates
        return;
    }

    if (strlen(ship) != 11) // checks if ship has correct length
    {
        printf("Wrong input!");
        place_battleship(ship); // recursively calls itself to ask for coordinates
    }
}

void place_destroyer(char *ship)
{ // asks for coordinates of given ship and places it on the grid
    printf("Destroyer (size 3): ");
    scanf("%s", ship);

    if (check_ship(ship)) // checks input
    {
        place_destroyer(ship); // recursively calls itself to ask for coordinates
        return;
    }

    if (strlen(ship) != 8) // checks if ship has correct length
    {
        printf("Wrong input!");
        place_destroyer(ship); // recursively calls itself to ask for coordinates
    }
}

void place_submarine(char *ship)
{ // asks for coordinates of given ship and places it on the grid
    printf("Submarine (size 3): ");
    scanf("%s", ship);

    if (check_ship(ship)) // checks input
    {
        place_submarine(ship); // recursively calls itself to ask for coordinates
        return;
    }

    if (strlen(ship) != 8) // checks if ship has correct length
    {
        printf("Wrong input!");
        place_submarine(ship); // recursively calls itself to ask for coordinates
    }
}

void place_patrol(char *ship)
{ // asks for coordinates of given ship and places it on the grid
    printf("Patrol Boat (size 2): ");
    scanf("%s", ship);

    if (check_ship(ship)) // checks input
    {
        place_patrol(ship); // recursively calls itself to ask for coordinates
        return;
    }

    if (strlen(ship) != 5) // checks if ship has correct length
    {
        printf("Wrong input!");
        place_patrol(ship); // recursively calls itself to ask for coordinates
    }
}

void place_ships(int player)
{ // places all the ships on the grid after checking input
    coordinates c;
    char ship[14];

    printf("Choose the placement for your ships separated by commas (eg. E4,F4,G4,H4 for battleship):\n");

    place_carrier(ship);
    get_coordinates(c, ship, player);
    generate_grid(player);
    place_battleship(ship);
    get_coordinates(c, ship, player);
    generate_grid(player);
    place_destroyer(ship);
    get_coordinates(c, ship, player);
    generate_grid(player);
    place_submarine(ship);
    get_coordinates(c, ship, player);
    generate_grid(player);
    place_patrol(ship);
    get_coordinates(c, ship, player);
    generate_grid(player);
}

void wipe_grid()
{ // resets grid for a new game (copies contents of base_grid.txt)
    FILE *fptr1, *fptr2, *fptr3;
    char c;
    fptr1 = fopen("base_grid.txt", "r");
    if (fptr1 == NULL)
    {
        printf("Cannot open file base_grid.txt \n");
        exit(0);
    }

    fptr2 = fopen("player_grid.txt", "w");
    if (fptr2 == NULL)
    {
        printf("Cannot open file player_grid.txt \n");
        exit(0);
    }

    fptr3 = fopen("computer_grid.txt", "w");
    if (fptr3 == NULL)
    {
        printf("Cannot open file omputer_grid.txt \n");
        exit(0);
    }

    // Read contents from file
    c = fgetc(fptr1);
    while (c != EOF)
    {
        fputc(c, fptr3);
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }

    fclose(fptr1);
    fclose(fptr2);
    fclose(fptr3);
}
bool check_end(int player) // TODO CHECKS ENDGAME
{                          // checks if game has ended (either players ships are all destroyed)

    FILE *fptr;
    char c;

    if (player == 1)
    {
        fptr = fopen("player_grid.txt", "r");
    }
    else
        fptr = fopen("computer_grid.txt", "r");

    if (fptr == NULL)
    {
        printf("Cannot open file base_grid.txt \n");
        exit(0);
    }
    fseek(fptr, 50, SEEK_SET);

    c = fgetc(fptr);
    while (c != EOF)
    {
        if (c == 'S') // searches for "S" symbol
        {
            fclose(fptr);
            return false;
        }
        c = fgetc(fptr);
    }
    fclose(fptr);
    return true; // returns true if game is over
}

void combat(int player)
{ // funccion asks for shooting coordinates and puts "X" or "O" on the grids
    char shot[2];
    int aim;
    coordinates c;
    printf("Choose a coordinate to shoot: ");
    scanf("%s", shot);
    c.x = shot[0] - 65;
    c.y = shot[1] - 48;

    if (check_ship(shot))
    {
        combat(player); // recursively calls itself to ask for coordinates
        return;
    }

    aim = parse_grid(c, !player, 2);

    if (aim == 1) // searches the grid of opposing player to check if shot hit
    {
        parse_enemy_grid(c, player, 1); // if hit puts "X" in your imaginary opponent's grid
        printf("You hit an enemy ship!\n");
    }
    else if (aim == 2)
    {
        parse_enemy_grid(c, player, 2); // if you chose a coordinate you already tried, you are reminded of it
        printf("You already tried there genius...\n");
    }
    else
    {
        parse_enemy_grid(c, player, 0); // if missed puts "O" in your imaginary opponent's grid
        printf("You missed!\n");
    }
}

void computer_gen_x(int ship_arr[10][10], int size)
{
    coordinates c;
    c.x = rand() % 10;
    c.y = rand() % 10;
    printf("c.x: %d\nc.y: %d\n ", c.x, c.y);

    if (c.x < 5)
    {
        for (int j = 0; j < size; j++)
        {
            if (ship_arr[c.x][c.y] == 1)
            {
                printf("Collision occured, redoing generation\n");
                computer_gen_x(ship_arr, size);
                return;
            }
            c.x++;
        }
        c.x -= size;

        for (int i = 0; i < size; i++)
        {
            ship_arr[c.x][c.y] = 1;
            parse_grid(c, 0, 1); // searches grid with converted coordinates
            c.x++;
        }
    }
    else
    {
        for (int j = 0; j < size; j++)
        {
            if (ship_arr[c.x][c.y] == 1)
            {
                printf("Collision occured, redoing generation\n");
                computer_gen_x(ship_arr, size);
                return;
            }
            c.x--;
        }
        c.x += size;
        for (int i = 0; i < size; i++)
        {
            ship_arr[c.x][c.y] = 1;
            parse_grid(c, 0, 1); // searches grid with converted coordinates
            c.x--;
        }
    }
}

void computer_gen_y(int ship_arr[10][10], int size)
{
    coordinates c;
    c.x = rand() % 10;
    c.y = rand() % 10;
    printf("c.x: %d\nc.y: %d\n ", c.x, c.y);
    if (c.y < 5)
    {
        for (int j = 0; j < size; j++)
        {
            if (ship_arr[c.x][c.y] == 1)
            {
                printf("Collision occured, redoing generation\n");
                computer_gen_y(ship_arr, size);
                return;
            }
            c.y++;
        }
        c.y -= size;
        for (int i = 0; i < size; i++)
        {
            ship_arr[c.x][c.y] = 1;
            parse_grid(c, 0, 1); // searches grid with converted coordinates
            c.y++;
        }
    }
    else
    {
        for (int j = 0; j < size; j++)
        {
            if (ship_arr[c.x][c.y] == 1)
            {
                printf("Collision occured, redoing generation\n");
                computer_gen_y(ship_arr, size);
                return;
            }
            c.y--;
        }
        c.y += size;
        for (int i = 0; i < size; i++)
        {
            ship_arr[c.x][c.y] = 1;
            parse_grid(c, 0, 1); // searches grid with converted coordinates
            c.y--;
        }
    }
}

void computer_ship_gen(int ship_arr[10][10], int size)
{

    bool direction = rand() % 2;

    if (direction)
    {
        computer_gen_x(ship_arr, size);
    }
    else
    {
        computer_gen_y(ship_arr, size);
    }

    generate_grid(2);
}

void computer_ships()
{
    coordinates c;
    int ship_arr[10][10] = {0};

    computer_ship_gen(ship_arr, 5);
    computer_ship_gen(ship_arr, 4);
    computer_ship_gen(ship_arr, 3);
    computer_ship_gen(ship_arr, 3);
    computer_ship_gen(ship_arr, 2);
}

void computer_combat()
{
    coordinates c;
    int aim;
    c.x = rand() % 10;
    c.y = rand() % 10;
    printf("c.x: %d\nc.y: %d\n", c.x, c.y);

    aim = parse_grid(c, 1, 2);

    if (aim == 1) // searches the grid of opposing player to check if shot hit
    {
        parse_enemy_grid(c, 2, 1); // if hit puts "X" in your imaginary opponent's grid
        printf("Computer hit your ship!\n");
    }
    else if (aim == 2)
    {
        parse_enemy_grid(c, 2, 2); // if you chose a coordinate you already tried, you are reminded of it
        printf("Computer hit itself in its own confusion\n");
    }
    else
    {
        parse_enemy_grid(c, 2, 0); // if missed puts "O" in your imaginary opponent's grid
        printf("Computer missed!\n");
    }
}

int main()
{
    int mode_choice;
    srand(time(0));

    wipe_grid();
    printf("Welcome to Battleships!\nChoose a gamemode:\nType 1 for singleplayer\nType 2 for multiplayer\n");
    scanf("%d", &mode_choice); // choose single or multi player

    switch (mode_choice)
    {
    case 1:
        printf("Entering singleplayer mode\n");
        // computer_ships();
        printf("PLayer's turn\n"); // begins turn and places ships for the player
        generate_grid(1);
        print_info();
        place_ships(1);
        printf("Computer's turn\n"); // begins turn and places ships for player 2
        computer_ships();
        while (!check_end(1) && !check_end(2)) // starts combat untill either players fleet is destroyed
        {
            printf("PLayer 1's turn\n");
            generate_grid(1);
            combat(1);
            if (check_end(1) || check_end(2)) // checks if player 1 won the game
                break;
            printf("Computer's turn\n");
            computer_combat();
            generate_grid(2);
        }
        break;

    case 2:
        printf("Entering multiplayer mode\n");
        printf("PLayer 1's turn\n"); // begins turn and places ships for player 1
        generate_grid(1);
        print_info();
        place_ships(1);
        printf("PLayer 2's turn\n"); // begins turn and places ships for player 2
        generate_grid(2);
        print_info();
        place_ships(2);
        while (!check_end(1) && !check_end(2)) // starts combat untill either players fleet is destroyed
        {
            printf("PLayer 1's turn\n");
            generate_grid(1);
            combat(1);
            if (check_end(1) || check_end(2)) // checks if player 1 won the game
                break;
            printf("PLayer 2's turn\n");
            generate_grid(2);
            combat(0);
        }
        break;

    default:
        printf("Wrong input!");
        break;
    }

    if (check_end(1)) // checks which player won
    {
        printf("GAME OVER!\nPLAYER 2 WINS!\n");
        printf("Player 1's fleet:\n");
        generate_grid(1);
        printf("Player 2's fleet:\n");
        generate_grid(2);
    }
    else
    {
        printf("GAME OVER!\nPLAYER 1 WINS!\n");
        printf("Player 1's fleet:\n");
        generate_grid(1);
        printf("Player 2's fleet:\n");
        generate_grid(2);
    }

    return 0;
}
