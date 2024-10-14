#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include "FileHandler.h"

using std::string;
using std::ofstream;
using std::ifstream;

struct Player
{
    char name[32] = "";
    int score = 0;
    time_t time = 0;
};

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
        if (choiceInput.compare("1") == 0 || choiceInput.compare("2") == 0 || choiceInput.compare("3") == 0 || choiceInput.compare("4") == 0)
        {
            return std::stoi(choiceInput);
        }
        else
        {
            std::cout << "Sorry, that input wasn't recognised.\n\n";
            choiceInput = "0";
        }
    }
}

void DataEntry()
{
    ofstream file("high.scores", std::ios::app | std::ios::binary);
    if (file.good())
    {
        Player newPlayer;

        std::cout << "Please enter the name of the player.\nSpaces are not allowed.\n";
        std::cout << "Name: ";
        std::cin >> newPlayer.name;

        std::cout << "\nPlease enter the score of the player.\n";
        std::cout << "Score: ";
        std::cin >> newPlayer.score;

        newPlayer.time = time(0);

        file.write((char*)&newPlayer, sizeof(Player));

        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &newPlayer.time);
        std::cout << "\nSuccessfully saved " << newPlayer.name << " with a score of " << newPlayer.score << " at " << timeStr;

        std::cout << "\n";

        std::cin.clear(); //clear bad input flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //discard input
        file.close();
        SortFile();
    }
    else 
    {
        std::cout << "Failed to open high.scores.\n";
    }
    file.close();
    system("pause");
}

int GetPlayerCount()
{
    int playerCount = 0;
    ifstream file;
    file.open("high.scores", std::ios::in | std::ios::binary);
    if (file.good())
    {
        Player newPlayer;
        //checks how many players are stored;
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
        Player* playerArray = new Player[playerCount];
        //don't need to check for EOF as we already know how large the file is from previous loop
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
                    //Swaps valyes
                    char temp[32];

                    Player tempPlayer;
                    tempPlayer = playerArray[j];
                    playerArray[j] = playerArray[j + 1];
                    playerArray[j+1] = tempPlayer;
                }
            }
        }
        file.close();

        ofstream fileOut("high.scores", std::ios::out | std::ios::binary);
        if (fileOut.good())
        {
            fileOut.write((char*)playerArray, sizeof(Player) * playerCount);
        }

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
        char nameInput[32];

        std::cout << "Please enter the name of the player.\nSpaces are not allowed.\n";
        std::cout << "Name: ";
        std::cin >> nameInput;

        Player newPlayer;

        while (!file.eof() && file.peek() != EOF)
        {
            file.read((char*)&newPlayer, sizeof(Player));

            if (strcmp(nameInput, newPlayer.name) == 0)
            {
                std::cout << newPlayer.name << std::endl;
                std::cout << newPlayer.score << std::endl;
                char timeStr[26];
                ctime_s(timeStr, sizeof(timeStr), &newPlayer.time);
                std::cout << timeStr << std::endl << std::endl;
            }
        }
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
        //don't need to check for EOF as we already know how large the file is from previous loop
        file.read((char*)playerArray, sizeof(Player) * playerCount);

        for (int i = 0; i < playerCount; i++)
        {
            std::cout << playerArray[i].name << std::endl;
            std::cout << playerArray[i].score << std::endl;
            char timeStr[26];
            ctime_s(timeStr, sizeof(timeStr), &playerArray[i].time);
            std::cout << timeStr;
            std::cout << "\n\n";
        }
        delete[] playerArray;
    }
    file.close();
    system("pause");
}

int main()
{
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
            std::cout << "You have selected lookup mode.\n";
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
