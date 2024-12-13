#include "GameDataParser.h"
#include "OnlineGame/OnlineGameScene.h"



GameDataParser parseGameData( string jsonData) {
        rapidjson::Document document;
        document.Parse(jsonData.c_str());
        
        if (document.HasParseError()) {
            AXLOG("JSON parsing error: %d", document.GetParseError());
            return {};
        }
        
        GameDataParser gameData=*(new GameDataParser());
        if (document.HasMember("board") && document["board"].IsArray()) {
            const rapidjson::Value& boardArray = document["board"];

            if (boardArray.Size() == 3) {
                // Resize the matrix to 3x3
                gameData.matrix.resize(3, vector<string>(3, ""));
                
                for (rapidjson::SizeType i = 0; i < boardArray.Size(); ++i) {
                    const rapidjson::Value& rowArray = boardArray[i];
                    if (rowArray.IsArray() && rowArray.Size() == 3) {
                        for (rapidjson::SizeType j = 0; j < rowArray.Size(); ++j) {
                            if (rowArray[j].IsString()) {
                                gameData.matrix[i][j] = rowArray[j].GetString();
                            }
                        }}}}}
        
        if (document.HasMember("turn") && document["turn"].IsInt()) {
            gameData.turn = document["turn"].GetInt();
        }
        
        if (document.HasMember("state") && document["state"].IsString()) {
            gameData.state = document["state"].GetString();
        }
        
        return gameData;
    }
void sendMove(network::WebSocket* ws, int row, int col) {
        
        // Create a JSON document and set it as an object
        rapidjson::Document document;
        document.SetObject();
        
        // Get the allocator for the document
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        
        // Add "Row" and "Col" to the document
        document.AddMember("Row", row, allocator);
        document.AddMember("Col", col, allocator);
        
        // Convert document to a string
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        
        // Get the JSON string
        string jsonData = buffer.GetString();
        AXLOG("Sending JSON: %s", jsonData.c_str());
        
        // Send the JSON string through WebSocket
        ws->send(jsonData);
    return;
    }
void PrintGameData(GameDataParser gameData)
{
    AXLOG("Game Matrix:");
    for (const auto& row : gameData.matrix) {
        string rowStr;
        for (const auto& cell : row) {
            rowStr += cell + "{ } ";
        }
        AXLOG("%s",rowStr.c_str());
        
    }
    
    AXLOG("Current turn: %d", gameData.turn);
    AXLOG("Current game state: %s", gameData.state.c_str());
    return;
    
}
//int countfill(vector<vector<string>>& matrix)
//{
//    int count=0;
//    for(int i=0;i<3;i++)
//    {
//        for(int j=0;i<3;j++)
//        {
//            if(matrix[i][j]!="") count++;
//        }
//    }
//    return count;
//}
void TakeAction(GameDataParser gameData)
{
//        Scene* currentScene = Director::getInstance()->getRunningScene();
//        OnlineGameScene* myScene = dynamic_cast<OnlineGameScene*>(currentScene->getChildren()[1]);
        OnlineGameScene* myScene=OnlineGameScene::getInstance();
        if (myScene) {
////                 Call a function on the current scene
            myScene->onMessage(gameData);
        }
     
}
