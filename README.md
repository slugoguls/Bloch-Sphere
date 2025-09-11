# Bloch Sphere Visualization

This project provides a visualization of the Bloch Sphere, allowing you to explore quantum states and apply Pauli gates.

## How to Run

To run the application without a full development setup:

1.  **Download the Executable Package:**
    Download the latest `mygame-X.X.X-win64.zip` file from the [releases/main branch](YOUR_GITHUB_RELEASES_OR_MAIN_BRANCH_LINK_HERE).

2.  **Extract the Contents:**
    Unzip the downloaded file to a location of your choice.

3.  **Run the Executable:**
    Navigate to the extracted folder and double-click `mygame.exe` to launch the application.

### Important Note on Antivirus Detection

You might encounter a "virus detected" message from your antivirus software when downloading or running the executable. This is very likely a **false positive**.

Modern antivirus programs use heuristic analysis, which can sometimes flag legitimate software (especially unsigned executables or those interacting with system resources like graphics drivers) as suspicious.

**Rest assured, this project's code is open-source and available for your review.** You can inspect the entire codebase to verify its safety. If you are concerned, you can:
*   Temporarily disable your antivirus (at your own risk) to run the application.
*   Add an exception for the `mygame.exe` file or the extracted folder in your antivirus settings.
*   Compile the project from source yourself (refer to `CMakeLists.txt` for build instructions).

## Building from Source

If you wish to build the project from source, you will need CMake and a C++ compiler (e.g., MSVC with Visual Studio).

1.  Clone the repository: `git clone https://github.com/your_username/Bloch-Sphere.git` (Replace `your_username` with the actual GitHub username)
2.  Navigate to the `Bloch-Sphere` directory.
3.  Create a build directory: `cmake -B build`
4.  Build the project: `cmake --build build --config Release`
5.  The executable will be found in `build/Release/mygame.exe`.

## Features

*   Interactive Bloch Sphere visualization.
*   Control of quantum state (theta and phi angles).
*   Application of Pauli X, Y, Z gates.
*   Real-time display of the quantum state in Dirac notation.

## Contributing

Feel free to fork the repository, make improvements, and submit pull requests.

## License

[Specify your license here, e.g., MIT License]