#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

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
    }
    else 
    {
        std::cout << "Failed to open high.scores.\n";
    }
    file.close();
    system("pause");
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

            bool isSame = true;
            for (int i = 0; i < 32; i++)
            {
                if (nameInput[i] != newPlayer.name[i])
                {
                    isSame = false;
                }
                else
                {
                    if (nameInput[i] == '\0')
                    {
                        break;
                    }
                }
            }
            if (isSame)
            {
                std::cout << newPlayer.name << std::endl;
                std::cout << newPlayer.score << std::endl;
                std::cout << newPlayer.time << std::endl << std::endl;
            }
        }
    }
    file.close();
    system("pause");

}

void Reading()
{
    ifstream file;
    file.open("high.scores", std::ios::in | std::ios::binary);
    if (file.good())
    {
        int playerCount = 0;

        Player newPlayer;
        //checks how many players are stored;
        while (!file.eof() && file.peek() != EOF)
        {
            file.read((char*)&newPlayer, sizeof(Player));
            playerCount++;
        }
        std::cout << "There are this amount of players: " << playerCount << "\n\n";

        //closes and reopens to reset file peek
        file.close();
        Player* playerArray = new Player[playerCount];
        file.open("high.scores", std::ios::in | std::ios::binary);
        if (file.good())
        {
            //don't need to check for EOF as we already know how large the file is from previous loop
            file.read((char*)playerArray, sizeof(Player) * playerCount);

            for(int i = 0; i < playerCount; i++)
            {
                std::cout << playerArray[i].name << std::endl;
                std::cout << playerArray[i].score << std::endl;
                std::cout << playerArray[i].time;
                std::cout << "\n\n";
            }

            
        }
        file.close();
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
            std::cout << "You have selected read mode.\n";
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
