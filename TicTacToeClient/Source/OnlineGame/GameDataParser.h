// GameDataParser.h

#ifndef GAMEDATAPARSER_H
#define GAMEDATAPARSER_H
#include "axmol.h"
#include <string>
#include <vector>
#include <iostream>
#include "platform/PlatformMacros.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "network/WebSocket.h"
#include "Helper/constants.h"

USING_NS_AX;
using namespace std;

class GameDataParser {
public:
    vector<vector<string>> matrix;
    int turn;
    string state;
    
};
int countfill(vector<vector<string>>& matrix);
void PrintGameData(GameDataParser gameData);
GameDataParser parseGameData(string jsonData);
void sendMove(network::WebSocket* ws, int row, int col);
void TakeAction(GameDataParser gameData);

#endif // GAMEDATAPARSER_H

