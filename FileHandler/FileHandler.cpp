#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include "FileHandler.h"

using std::string;
using std::ofstream;
using std::ifstream;

//The main UI that allows the player to choose different modes
int SelectMode()
{
    system("CLS");
    string choiceInput;
    while (true)
    {
        std::cout << "Welcome to the High Score Saving System!\n";
        std::cout << "What would you like to do?\n\n";

        std::cout << "1. Enter data entry\n";
        std::cout << "2. Lookup player high score\n";
        std::cout << "3. Read data\n";
        std::cout << "4. Close\n\n";

        std::cout << "Enter 1, 2, 3, or 4.\n";
        std::cout << "Choice: ";
        std::cin >> choiceInput;

        system("CLS");

        //Checks that the input is valid i.e. 1-4
        if (choiceInput.compare("1") == 0 || choiceInput.compare("2") == 0 || choiceInput.compare("3") == 0 || choiceInput.compare("4") == 0)
        {
            return std::stoi(choiceInput);
        }
        else
        {
            std::cout << "Sorry, that input wasn't recognised.\n\n";
            choiceInput = "0"; //Resets to stop infinite console loops
        }
    }
}

void DataEntry()
{
    //Opens file for appending
    ofstream file("high.scores", std::ios::app | std::ios::binary);
    if (file.good())
    {
        //Temp player for storing user input
        Player newPlayer;

        std::cout << "Please enter the name of the player.\nSpaces are not allowed, and max 32 characters.\n";
        std::cout << "Name: ";
        std::cin >> newPlayer.name;

        std::cout << "\nPlease enter the score of the player.\n";
        std::cout << "Score: ";
        std::cin >> newPlayer.score;

        //Sets time to current time
        newPlayer.time = time(0);

        //Writes the new player at the end of the file
        file.write((char*)&newPlayer, sizeof(Player));

        //Converts time to english
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &newPlayer.time);

        //User feedback response
        std::cout << "\nSuccessfully saved " << newPlayer.name << " with a score of " << newPlayer.score << " at " << timeStr << "\n";

        std::cin.clear(); //clear bad input flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //discard input

        //Closes file
        file.close();

        //Sorts file
        SortFile();
    }
    else 
    {
        std::cout << "Failed to open high.scores.\n";
    }
    //Closes file, incase of failed .good state
    file.close();
    system("pause");
}

//Returns the amount of player records in a file
int GetPlayerCount()
{
    int playerCount = 0;
    ifstream file;
    file.open("high.scores", std::ios::in | std::ios::binary);
    if (file.good())
    {
        Player newPlayer;
        //This loop will run for every Player in high.scores
        while (!file.eof() && file.peek() != EOF)
        {
            file.read((char*)&newPlayer, sizeof(Player));
            playerCount++;
        }
    }
    file.close();
    return playerCount;
}

void SortFile()
{
    int playerCount = GetPlayerCount();
    ifstream file;
    file.open("high.scores", std::ios::in | std::ios::binary);
    if (file.good())
    {
        //Creates an array of players, with its size based on the amount of players that exist in the file
        Player* playerArray = new Player[playerCount];

        //don't need to check for EOF as we already know how large the file is from playerCount
        //reads each player, for how many players there are
        file.read((char*)playerArray, sizeof(Player) * playerCount);

        //bubble sort players by name
        for (int i = 0; i < playerCount - 1; i++)
        {
            for (int j = 0; j < playerCount - 1; j++)
            {
                //Checks which string is 'bigger', i.e. alphabetical order
                int value = strcmp(playerArray[j].name, playerArray[j + 1].name);
                //Condition is true if first string is 'bigger', i.e. "Bob" and "Annie"
                if (value > 0)
                {
                    //Swaps values
                    Player tempPlayer;
                    tempPlayer = playerArray[j];
                    playerArray[j] = playerArray[j + 1];
                    playerArray[j+1] = tempPlayer;
                }
            }
        }
        file.close();

        //Stores new sorted player list, overwriting previous
        ofstream fileOut("high.scores", std::ios::out | std::ios::binary);
        if (fileOut.good())
        {
            fileOut.write((char*)playerArray, sizeof(Player) * playerCount);
        }

        //Closes file, frees memory
        fileOut.close();
        delete[] playerArray;
    }
    file.close();
}

void Lookup()
{
    ifstream file("high.scores", std::ios::in | std::ios::binary);
    if (file.good())
    {
        //Gets and stores name for looking up
        char nameInput[32];

        std::cout << "Please enter the name of the player.\nSpaces are not allowed, and max 32 characters.\n";
        std::cout << "Name: ";
        std::cin >> nameInput;

        //Binary search vars
        int length = GetPlayerCount();
        int beg = 0;
        int end = length - 1;
        int result = -1;

        //Creates an array of players, with its size based on the amount of players that exist in the file
        Player* playerArray = new Player[length];
        //reads each player, for how many players there are
        file.read((char*)playerArray, sizeof(Player) * length);

        //Binary search algo
        while (beg <= end)
        {
            int mid = (beg + end) / 2;
            //Checks if input comes after mid position
            if (strcmp(playerArray[mid].name, nameInput) <= 0)
            {
                beg = mid + 1;
                result = mid;
            }
            else
            {
                end = mid - 1;
            }
        }

        //checks that the binary search result is actually equal
        if (strcmp(playerArray[result].name, nameInput) != 0)
        {
            std::cout << "Sorry, that player was unable to be found." << std::endl;
        }
        else 
        {
            //Stores player based on result
            Player newPlayer;
            newPlayer = playerArray[result];

            //displays results
            std::cout << "Score: " << newPlayer.score << std::endl;
            char timeStr[26];
            ctime_s(timeStr, sizeof(timeStr), &newPlayer.time);
            std::cout << "Time: " << timeStr << std::endl << std::endl;
        }
        
        //frees memory
        delete[] playerArray;

    }

    file.close();
    system("pause");

}

void Reading()
{
    int playerCount = GetPlayerCount();
    ifstream file;
    file.open("high.scores", std::ios::in | std::ios::binary);
    if (file.good())
    {
        Player* playerArray = new Player[playerCount];
        //don't need to check for EOF as we already know how large the file is from playerCount
        file.read((char*)playerArray, sizeof(Player) * playerCount);

        //Prints each player to the console
        for (int i = 0; i < playerCount; i++)
        {
            std::cout << "Name: " << playerArray[i].name << std::endl;
            std::cout << "Score: " << playerArray[i].score << std::endl;
            //Converts from time_t to english
            char timeStr[26];
            ctime_s(timeStr, sizeof(timeStr), &playerArray[i].time);
            std::cout << "Time: " << timeStr;
            std::cout << "\n\n";
        }
        //frees memory
        delete[] playerArray;
    }
    else
    {
        std::cout << "Failed opening file. Perhaps it doesn't exist?" << std::endl;
    }
    file.close();
    system("pause");
}

int main()
{
    //Runs until broken by choice 4 (close)
    while (true)
    {
        int choice = SelectMode();


        if (choice == 1)
        {
            std::cout << "You have selected data entry mode.\n\n";
            DataEntry();
        }
        else if (choice == 2)
        {
            std::cout << "You have selected lookup mode.\n\n";
            Lookup();
        }
        else if (choice == 3)
        {
            std::cout << "You have selected read mode.\n\n";
            Reading();
        }
        else if (choice == 4)
        {
            return 0;
        }
        else
        {
            std::cout << "It appears something has gone wrong.";
            return 1;
        }
    }
}
