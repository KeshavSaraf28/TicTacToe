#ifndef WEBSOCKETMANAGER_H
#define WEBSOCKETMANAGER_H

#include "network/WebSocket.h"  // Include the WebSocket header for Axmol
#include "platform/PlatformMacros.h"  // For logging
#include "GameDataParser.h"
#include <unistd.h>
#include <fstream>   // For std::ofstream
#include <string>    // For std::string

USING_NS_AX;

class WebSocketManager : public network::WebSocket::Delegate {
    
private:
    WebSocketManager();  // Private constructor for singleton pattern
    ~WebSocketManager();  // Private destructor
    void deleteMe();

public:
    // Get the singleton instance of WebSocketManager
    static WebSocketManager* getInstance(bool needOld=false);

    // Initialize the WebSocket connection
    bool init(const std::string& url);

    // Get the WebSocket instance
    network::WebSocket* getWebSocket() const;

    // Send data through the WebSocket
    void send(const std::string& data);
    static void DeleteInstance();

    // WebSocket delegate methods
    virtual void onOpen(network::WebSocket* ws) override;
    virtual void onMessage(network::WebSocket* ws, const network::WebSocket::Data& data) override;
    virtual void onClose(network::WebSocket* ws) override;
    virtual void onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error) override;

    
    
    // The WebSocket connection
    network::WebSocket* webSocket;

    // Singleton instance
    static WebSocketManager* instance;
    static string loadUrlFromFile();
};


#endif // WEBSOCKETMANAGER_H
