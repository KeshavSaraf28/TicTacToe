# Tic-Tac-Toe with Axmol

This project is a Tic-Tac-Toe game built using Axmol and Go, featuring both local and online multiplayer options.

## Prerequisites

Make sure you have the following installed on your system:

- **Axmol 2.1.4**
- **Xcode**
- **PowerShell**
- **Go (Golang)**
- **Android Studio**

## Reference for setup and Design Doc of the Following Project

- **[Basic Setup Doc](https://docs.superkinglabs.com/s/de830af7-f792-428f-be83-d8c50d482a64)**
- **[Tic-Tac-Toe Design Doc](https://docs.superkinglabs.com/s/78dba6ea-daf7-4e84-86b2-d2be0af84ef6)**

## Setup Instructions For Project

### Clone the Repository

Clone the project to your local machine:

```bash
git clone https://gitlab.superkinglabs.com/KeshavSaraf/tictactoe.git
cd tictactoe
```

### Client-Side Setup

1. **Build the Project(macOS):**

   Open PowerShell and navigate to the client directory. Use the following command to build the project for macOS:

   ```bash
   axmol build -p osx -a arm64 -c
   ```

2. **Run the Project:**

   - Find the `build_arm64` folder generated after the build process.
   - Open the `.xcodeproj` file inside this folder with Xcode.
   - Build and run the project in Xcode to launch the game.
   
3. **Build the Project(Android):**

   Open PowerShell and navigate to the client directory. Use the following command to build the project for Android:

   ```bash
   axmol build -p android -a arm64
   ```

4. **Run the Project:**
   - Find the `proj.android` folder generated after the build process.
   - Open Android Studio and Select to Open Proejct and Navigate to above folder and open it.
   - Build and run the project in Emulator/Physical Device to launch the game.

### Server-Side Setup

1. **Navigate to the Server Folder:**

   Open a terminal and navigate to the server directory inside the cloned repository.

2. **Run the Server:**

   Use the following command to start the server:

   ```bash
   go run .
   ```

## Usage

Once both the server and client are set up and running, you can enjoy playing Tic-Tac-Toe either in single-player mode against the AI, locally with another player, or online by connecting with other players over the network.

### Game Modes

- **Single Player:** Play against a bot.
- **Local Multiplayer:** Play with another player on the same device.
- **Online Multiplayer:** Connect with another player over the local network.

## Contributing

If you wish to contribute to this project, please fork the repository and submit a pull request with your changes.

