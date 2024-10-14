#pragma once

struct Player
{
    char name[32] = "";
    int score = 0;
    time_t time = 0;
};


int SelectMode();
void DataEntry();
int GetPlayerCount();
void SortFile();
void Lookup();
void Reading();