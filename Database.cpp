//
// Created by lpgau on 2026-03-24.
//

#include "Database.h"
#include <string>

DataBaseStage::DataBaseStage () :database (nullptr) {}

DataBaseStage::~DataBaseStage () { CloseDatabase (); }

//Pour creer la database et creer le dossier "data"
Sint32 DataBaseStage::OpenDatabase () {
    SDL_CreateDirectory ("data"); // creation du dossier
    Sint32 result = sqlite3_open ("data/scores", &database);
    //Si aucune database est detecter ->
    if (result != SQLITE_OK) {
        SDL_LogError(1, "failed to open the database -> scores.db %s", sqlite3_errmsg (database));//errmsg -> erreur message
        sqlite3_close (database);
        database = nullptr;
        return result;
    }
    //Si database detecter ->
    SDL_LogInfo (1, "database opened");
    result = CreateTable();
    //Si les tables sont pas creers
    if (result != SQLITE_OK) {
        SDL_LogError (1, "failed to create tables of database %s", sqlite3_errmsg (database));
        sqlite3_close (database);
        database = nullptr;
    }
    //return success ++
    return SQLITE_OK;
}

//Creation des tables
Sint32 DataBaseStage::CreateTable() {
    std::string query = "CREATE TABLE IF NOT EXISTS highscores ("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "   player_name TEXT NOT NULL,"
        "   score INTEGER NOT NULL"
        ");"
        "CREATE INDEX IF NOT EXISTS idx_score ON highscores(score DESC);";

    char *error_message = nullptr;
    const Sint32 result = sqlite3_exec (database, query.c_str(), nullptr, nullptr, &error_message);

    //Si impossible de creer la table
    if (result != SQLITE_OK) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,"failed to create tables of database %s", error_message);
        sqlite3_free (error_message);
        return result;
    }
    SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION, "Table created successfully");
    return SQLITE_OK;
}
//Pour inserer un score
Sint32 DataBaseStage::InsertScore (const ScoreRecord &score) {
    std::string query = "INSERT INTO highscores (player_name, score) "
                      "VALUES (?, ?);";
    sqlite3_stmt *prepared_statement = nullptr;
    Sint32 result = sqlite3_prepare_v2(database, query.c_str(), -1, &prepared_statement, nullptr);
    //Si impossible de preparer un statement
    if (result != SQLITE_OK) {
        SDL_LogError (SDL_LOG_CATEGORY_ERROR,"failed to create tables of database %s", sqlite3_errmsg (database));
        return result;
    }

    result = sqlite3_bind_text (prepared_statement, 1,
                              score.player_name.c_str (), -1, SQLITE_STATIC);
    //impossible de mettre le nom ->
    if (result != SQLITE_OK) {
        SDL_LogError (SDL_LOG_CATEGORY_ERROR, "Failed to bind player name: %s", sqlite3_errmsg (database));
        sqlite3_finalize (prepared_statement);
        return result;
    }
    result = sqlite3_bind_int (prepared_statement, 2, score.value);
    //Impossible de mettre le score ->
    if (result != SQLITE_OK) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to bind score value %s", sqlite3_errmsg (database));
        sqlite3_finalize (prepared_statement);
        return result;
    }

    result = sqlite3_step (prepared_statement);
    if (result != SQLITE_DONE)
    {
        SDL_LogError (SDL_LOG_CATEGORY_ERROR, "Failed to insert record: %s\n",
                      sqlite3_errmsg (database));
        sqlite3_finalize (prepared_statement);
        return result;
    }

    SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION,
                 "Score inserted successfully! Player: %s, Score: %d",
                 score.player_name.c_str (), score.value);

    sqlite3_finalize (prepared_statement);
    return SQLITE_OK;
}

//Obtenir le score maximal
Sint32 DataBaseStage::GetHighScores(std::vector<ScoreRecord> &scores, Sint32 limit) {


}


//Supprimer les scores
Sint32 DataBaseStage::DeleteAllScores() {

}

//Fermer la database
Sint32 DataBaseStage::CloseDatabase() {
    if (database)
    {
        Sint32 result = sqlite3_close (database);
        if (result == SQLITE_OK)
        {
            SDL_LogInfo (SDL_LOG_CATEGORY_APPLICATION, "Database closed");
            database = nullptr;
        }
        else
        {
            SDL_LogError (SDL_LOG_CATEGORY_ERROR,
                          "Failed to close the database %s", sqlite3_errmsg (database));
        }
        return result;
    }
    return SQLITE_OK;
}