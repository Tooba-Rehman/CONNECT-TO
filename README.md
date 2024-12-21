# CONNECT-TO
Graph-Based Social Network Analysis
The execution guideline you provided outlines steps to organize files and compile a program using Raylib. Here are some improvements and clarifications:

*Execution Guideline:*
1. **File Placement**:  
   - Ensure `graph.cpp`, `graph.h`, and `main.cpp` are in the same folder.
   
2. **Directory Setup**:  
   - Create two subfolders named `include` and `lib` within the folder containing the `.cpp` and `.h` files.
   - Place `raylib.h` in the `include` folder.
   - Place `libraylib.a` in the `lib` folder.  
     *These folders are necessary for the compiler to find Raylib's header and library files.*

3. **Compilation Command**:  
   Open a terminal in the folder containing your files and run the following command:
   ```bash
   g++ main.cpp graph.cpp -o main.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
   ```
   - **Explanation**:
     - `-o main.exe`: Outputs the compiled executable as `main.exe`.
     - `-O1`: Optimizes the build for performance.
     - `-Wall`: Enables all compiler warnings.
     - `-std=c99`: Uses the C99 standard (Raylib uses a C-based API).
     - `-Wno-missing-braces`: Suppresses warnings about missing braces.
     - `-I include/`: Specifies the directory for Raylib's header files.
     - `-L lib/`: Specifies the directory for Raylib's library files.
     - `-lraylib`: Links the Raylib library.
     - `-lopengl32 -lgdi32 -lwinmm`: Links necessary Windows system libraries.

4. **Run the Program**:  
   After successful compilation, execute the following command in the terminal:
   ```bash
   .\main
   ```

### Notes:
1. **Check Compiler Installation**: Ensure `g++` (part of MinGW) is installed and correctly added to your system's PATH.
2. **Raylib Version**: Ensure that `raylib.h` and `libraylib.a` correspond to the same version of Raylib.
3. **Potential Issues**: If compilation fails:
   - Check that the `include` and `lib` folders are in the correct locations.
   - Ensure the paths in the `-I` and `-L` flags match your folder structure.
   - Verify that the required dependencies (`opengl32`, `gdi32`, `winmm`) are available on your system.
