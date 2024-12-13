#include "WebSocketManager.h"


WebSocketManager* WebSocketManager::instance = nullptr;


WebSocketManager* WebSocketManager::getInstance(bool needOld) {

    if (!needOld || !instance) {
        if(instance)
            delete instance;
        instance = new WebSocketManager();
        if (!instance->init(loadUrlFromFile())) {
            delete instance;
            instance = nullptr;
        }
    }
    return instance;

}
string WebSocketManager::loadUrlFromFile()
{
    // Get the file path in the writable path
    string filePath = FileUtils::getInstance()->getWritablePath() + "/saved_url.txt";

    // Open the file and read the URL
    ifstream file(filePath);
    string url;

    if (file.is_open())
    {
        getline(file, url); // Read the URL from the file
        file.close();
    }
    else
    {
        AXLOG("Failed to open file for reading: %s", filePath.c_str());
    }

    return url;
}
//void WebSocketManager::deleteMe()
//{
//    delete this;
//    instance=nullptr;
//}
void WebSocketManager::DeleteInstance()
{
    delete instance;
    instance=nullptr;
}
WebSocketManager::WebSocketManager()
    : webSocket(nullptr) {

}

WebSocketManager::~WebSocketManager() {
    if (webSocket) {
        webSocket->close();
        delete webSocket;
    }
}


bool WebSocketManager::init(const std::string& url) {
    webSocket = new network::WebSocket();
    return webSocket->open(this, url);  // Set this instance as the delegate
}

network::WebSocket* WebSocketManager::getWebSocket() const {
    return webSocket;
}

void WebSocketManager::send(const std::string& data) {
    if (webSocket) {
        webSocket->send(data);
    }
}

void WebSocketManager::onOpen(network::WebSocket* ws) {
    AXLOG("WebSocket opened");

}

void WebSocketManager::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data) {
    std::string msg=std::string(data.bytes,data.len);
    AXLOG("WebSocket message received: %s", msg.c_str());

    GameDataParser gameData=parseGameData(msg);
//    PrintGameData(gameData);
    TakeAction(gameData);

    return;

    // Handle incoming messages here
}

void WebSocketManager::onClose(network::WebSocket* ws) {
    AXLOG("WebSocket closed");
}

void WebSocketManager::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error) {
    AXLOG("WebSocket error: %d", static_cast<int>(error));
}
