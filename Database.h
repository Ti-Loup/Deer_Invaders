//
// Created by lpgau on 2026-03-24.
//

#ifndef DEER_INVADERS_DATABASE_H
#define DEER_INVADERS_DATABASE_H

#include <SDL3/SDL.h>
#include <sqlite3.h>
#include <string>
#include <vector>

class Database {
};

struct ScoreRecord {
    std::string player_name;
    Sint32 value;
};

class DataBaseStage final {
    sqlite3 *database;

public:
    DataBaseStage();
    ~DataBaseStage();//destruction

    Sint32 OpenDatabase();
    Sint32 InsertScore (const ScoreRecord &score);
    Sint32 GetHighScores (std::vector<ScoreRecord> &score, Sint32 limits);
    Sint32 DeleteAllScores();

private:
    Sint32 CreateTable();
    Sint32 CloseDatabase();
};


#endif //DEER_INVADERS_DATABASE_H