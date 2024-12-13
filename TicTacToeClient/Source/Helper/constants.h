
#ifndef constants_h
#define constants_h

#pragma once

static const std::string SPLASH_SCENE_BACKGROUND_PATH = "res/Splash Background.png";
static const std::string LOADING_SCREEN_PATH = "res/loading/";
static const float SPLASH_SCRENE_SHOW_TIME = 5.0;
static const float SCENE_TRANSITION_TIME = 0.6;
static const std::string BASE_LOADING_FILEPATH = "res/Base_loading.png";
static const float GAME_OVER_BACKGROUND_FADE_IN_TIME = 0.5;
static const float PAUSE_BACKGROUND_FADE_IN_TIME = 0.5;
static const float MENU_MOVE_BOUNCE_DURATION = 2.0;
static const std::string PAUSE_WINDOW = "res/Pause_back.png";

static const std::string MAIN_MENU_BACKGROUND_PATH = "res/Main Menu Background.png";
static const std::string MAIN_MENU_TITLE_PATH = "res/Game Title.png";
static const std::string MAIN_MENU_PLAY_BUTTON = "res/Play Button.png";
static const std::string MAIN_MENU_PLAY_BUTTON_PRESSED = "res/Play Button.png";
static const std::string MAIN_MENU_MULTI_PLAY_BUTTON = "res/Multi Player Button.png";
static const std::string MAIN_MENU_MULTI_PLAY_BUTTON_PRESSED = "res/Multi Player Button.png";
static const std::string MAIN_MENU_SETTINGS_BUTTON = "res/Settings Button.png";
static const std::string MAIN_MENU_SETTINGS_BUTTON_PRESSED = "res/Settings Button.png";

static const std::string MAIN_MENU_SOUND_ON_BUTTON = "res/Sound On.png";
static const std::string MAIN_MENU_SOUND_ON_BUTTON_PRESSED = "res/Sound On.png";
static const std::string MAIN_MENU_SOUND_OFF_BUTTON = "res/Sound Off.png";
static const std::string MAIN_MENU_SOUND_OFF_BUTTON_PRESSED = "res/Sound Off.png";

static const int TAG_MAIN_MENU_SINGLEPLAYER_BUTTON = 0;
static const int TAG_MAIN_MENU_MULTIPLAYER_BUTTON = 1;
static const int TAG_MAIN_MENU_ONLINE_BUTTON = 2;

static const std::string GAME_BACKGROUND_FILEPATH = "res/Main Menu Background.png";
static const std::string GAME_OVER_BACKGROUND_FILEPATH = "res/Pause Background.png";
static const std::string GRID_FILEPATH = "res/Grid.png";
static const std::string WAITING_FILEPATH = "res/waiting.png";

static const std::string OVERLAY_WINDOW_FILEPATH = "res/Pause Window.png";
static const std::string VICTORY_WINDOW_FILEPATH = "res/Victory Window.png";
static const std::string LOSE_WINDOW_FILEPATH = "res/Lose Window.png";
static const std::string HOME_BUTTON = "res/Home Button.png";
static const std::string HOME_BUTTON_PRESSED = "res/Home Button Pressed.png";
static const std::string RETRY_BUTTON = "res/Retry Button.png";
static const std::string RETRY_BUTTON_PRESSED = "res/Retry Button.png";
static const std::string PAUSE_BUTTON = "res/Pause Button.png";
static const std::string PAUSE_BUTTON_PRESSED = "res/Pause Button Pressed.png";
static const std::string RESUME_BUTTON = "res/Resume Button.png";
static const std::string RESUME_BUTTON_PRESSED = "res/Resume Button Pressed.png";

static const std::string X_PIECE_FILEPATH = "res/X.png";
static const std::string O_PIECE_FILEPATH = "res/O.png";
static const std::string X_WINNING_PIECE_FILEPATH = "res/X Win.png";
static const std::string O_WINNING_PIECE_FILEPATH = "res/O Win.png";

static const int X_PIECE = 1;
static const int O_PIECE = 0;
static const int EMPTY_PIECE = -1;
static const int PLAYER_PIECE = X_PIECE;
static const int BOT_PIECE = O_PIECE;
static const int ONLINE_PLAYER_PIECE = O_PIECE;

static const int STATE_WAITING = 8;
static const int STATE_CONNECTED = 9;
static const int STATE_ON_GOING = 10;
static const int STATE_ONLINE_PLAYING = 11;
static const int STATE_PLAYING = 12;
static const int STATE_PAUSED = 13;
static const int STATE_WON = 14;
static const int STATE_LOSE = 15;
static const int STATE_PLACING_PIECE = 16;
static const int STATE_BOT_PLAYING = 17;
static const int STATE_DRAW = 18;
static const int STATE_X_WIN = 19;
static const int STATE_O_WIN = 20;

static const int GAME_TYPE_OFFLINE = 50;
static const int GAME_TYPE_ONLINE = 51;

static const float PIECE_FADE_IN_TIME = 0.5f;

static const int COMMAND_TYPE_CONNECTED = 59;
static const int COMMAND_TYPE_PAUSE = 60;
static const int COMMAND_TYPE_MOVE = 61;
static const int COMMAND_TYPE_DISCONNECTED = 62;
static const int COMMAND_TYPE_RESULT = 63;

#endif 
