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
        }
    }
}

void DataEntry()
{
    ofstream file("high.scores", std::ios::out | std::ios::binary);
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
        system("pause");
        SelectMode();
    }
    file.close();
}

void Lookup()
{
    ofstream file("high.scores", std::ios::out | std::ios::binary);
    if (file.good())
    {
        Player newPlayer;

        std::cout << "Please enter the name of the player.\nSpaces are not allowed.\n";
        std::cout << "Name: ";
        std::cin >> newPlayer.name;
    }
    file.close();

}

void Reading()
{
    ifstream file;
    file.open("high.scores", std::ios::in | std::ios::binary);
    if (file.good())
    {
        Player newPlayer;

        while (!file.eof() && file.peek() != EOF)
        {
            file.read((char*)&newPlayer, sizeof(Player));
            std::cout << newPlayer.name << std::endl;
            std::cout << newPlayer.score << std::endl;
            std::cout << newPlayer.time;
        }
    }
    file.close();
    system("pause");
}

int main()
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
    }
}
