#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

struct Player
{
    string name;
    int score;
    time_t time;
};

int SelectMode()
{
    string choiceInput;
    while (true)
    {
        std::cout << "What would you like to do?\n\n";

        std::cout << "1. Enter data\n";
        std::cout << "2. Read data\n\n";

        std::cout << "Enter either 1 or 2.\n";
        std::cout << "Choice: ";
        cin >> choiceInput;

        system("CLS");
        if (choiceInput.compare("1") == 0 || choiceInput.compare("2") == 0)
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
    ofstream file("high.scores", ios::out | ios::binary);
    if (file.good())
    {
        Player newPlayer;

        std::cout << "Please enter the name of the player.\n";
        std::cout << "Name: ";
        cin >> newPlayer.name;

        std::cout << "\nPlease enter the score of the player.\n";
        std::cout << "Score: ";
        cin >> newPlayer.score;

        newPlayer.time = time(0);

        file.write((char*)&newPlayer, sizeof(Player));
    }
    file.close();
}

void Reading()
{
    ifstream file("high.scores", ios::in | ios::binary);
    if (file.good())
    {
        Player newPlayer;

        file.read((char*)&newPlayer, sizeof(Player));
        cout << newPlayer.name;
    }
    file.close();
}

int main()
{
    std::cout << "Welcome to the High Score Saving System!\n";
    int choice = SelectMode();
    

    if (choice == 1)
    {
        std::cout << "You have selected data entry mode.\n\n";
        DataEntry();
    }
    else if (choice == 2)
    {
        std::cout << "You have selected read mode.";
        Reading();
    }
    else
    {
        std::cout << "It appears something has gone wrong.";
    }
}
