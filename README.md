### CONNECT-TO

## Description
Is a Graph-Based Social Network Analysis application that enables users to explore and analyze their network of connections, interact with profiles, and discover recommendations.

## Features

- **Login, Signup, and Exit**: Start by logging in, signing up, or exiting the application.
- **Personal Profile**: View and manage your personal profile.
- **Follow and Unfollow Users**: Build and manage your network of connections.
- **Search**: Search for users, view their profiles, and follow them.
- **Connections**: Visualize how you are connected to others, both directly and through mutual connections.
- **Friends List**: View your list of followers and connections.
- **Friend Recommendations**: Receive suggestions for potential connections, including:
    - Mutual Recommendations: Suggest connections based on mutual friends.
    - Interest-Based Suggestions: Recommend users with similar interests.
    - Most Influential Users: Identify users with the most influence in the network.
    - Most Active Users: Suggest users who are the most active in the network.
- **Logout and Exit**: Safely log out or exit the application.

## Setup Instructions and Usage Instructions

### File Placement

1. Ensure `graph.cpp`, `graph.h`, and `main.cpp` are in the same folder.

### Directory Setup

1. Create two subfolders named `include` and `lib` within the folder containing the `.a` and `.h` files.
2. Place `raylib.h` in the `include` folder.
3. Place `libraylib.a` in the `lib` folder.

These folders are necessary for the compiler to locate Raylib's header and library files.

### Compilation Command

Open a terminal in the folder containing your files and run the following command:

```bash
g++ main.cpp graph.cpp -o main.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
```

#### Explanation of Flags

- `-o main.exe`: Outputs the compiled executable as `main.exe`.
- `-O1`: Optimizes the build for performance.
- `-Wall`: Enables all compiler warnings.
- `-std=c99`: Uses the C99 standard (Raylib uses a C-based API).
- `-Wno-missing-braces`: Suppresses warnings about missing braces.
- `-I include/`: Specifies the directory for Raylib's header files.
- `-L lib/`: Specifies the directory for Raylib's library files.
- `-lraylib`: Links the Raylib library.
- `-lopengl32 -lgdi32 -lwinmm`: Links necessary Windows system libraries.

### Run the Program

After successful compilation, execute the following command in the terminal:

```bash
.\main
```

### Notes

- **Check Compiler Installation**: Ensure `g++` (part of MinGW) is installed and correctly added to your system's PATH.
- **Raylib Version**: Ensure that `raylib.h` and `libraylib.a` correspond to the same version of Raylib.
- **Potential Issues**:
  - Verify the `include` and `lib` folders are correctly set up.
  - Ensure paths in the `-I` and `-L` flags match your folder structure.
  - Check that required dependencies (`opengl32`, `gdi32`, `winmm`) are available on your system.

## Technologies Used

- **C++ Libraries**:
  - `<iostream>`
  - `<fstream>`
  - `<sstream>`
  - `<vector>`
  - `<map>`
  - `<set>`
  - `<queue>`
  - `<unordered_map>`
  - `<cstring>`
  - `<algorithm>`
- **Raylib**: A simple and easy-to-use library for games and graphical applications.
- **IDE**: Visual Studio Code.
