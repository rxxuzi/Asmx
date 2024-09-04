# Asmx

Windows NASM+C Build Tool

## Description
Asmx is a build tool designed for Windows 64-bit environments, combining NASM (Netwide Assembler) and C programming. 
It streamlines the process of compiling and linking assembly and C code, making it an ideal solution for developers working on mixed-language projects.

## Features
- Automatic detection and compilation of NASM and C source files.
- Configuration via JSON file for flexible project setup.
- Integration of assembly and C code within a single build environment.
- Support for Windows 64-bit systems.
- Custom build configuration options.

## Requirements
- Windows 64-bit operating system
- NASM installed and added to the system PATH
- GCC (GNU Compiler Collection) installed and added to the system PATH

## Installation

1. Clone this repository:
    ```shell
    git clone https://github.com/rxxuzi/Asmx.git
    ```
2. Ensure NASM and GCC are properly configured in your system's PATH.

## Usage
Basic usage:

```shell
asmx <command> [options]
```
Commands:
- `build`: Build the project
- `compile`: Compile only (no linking)
- `run`: Build and run the project
- `clean`: Clean the build directory
- `gen`: Generate a default `asmx.json` configuration file

Options:
- `-o <file>`: Specify the output file name
- `-d`: Display detailed build information
- `-i <file>`: Specify a custom build configuration file
  For more information, use `asmx -h` or `asmx --help`.

## Configuration
Asmx uses a JSON configuration file (default: `asmx.json`) to specify project details. Use `asmx gen` to create a template configuration file.

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request with your proposed changes.

## License
This project is licensed under the GNU General Public License (GPL).