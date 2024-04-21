# Asmx

Windows NASM+C Build Tool

## Description
This project is a build tool that combines NASM (Netwide Assembler) and C programming language to facilitate the building of applications specifically for Windows 64-bit environments. The tool automates the compilation and linking of assembly and C code, making it ideal for developers looking to streamline their build processes on Windows platforms.

## Features
- Automatic detection and compilation of NASM and C source files based on configuration specified in a YAML file.
- Integration of assembly and C code within a single build environment.
- Support for Windows 64-bit systems.

## Requirements
- Windows 64-bit operating system.
- NASM installed on the system and added to the system PATH.
- GCC (GNU Compiler Collection) for compiling C code, also required to be in the system PATH.
- YAML file specifying build configuration.

## Installation
1. Clone this repository to your local machine using:

```shell
git clone https://github.com/rxxuzi/Asmx.git
```
2. Ensure that NASM and GCC are installed and properly configured in your system's PATH. The PATH configuration is crucial to ensure that these tools can be accessed from any command prompt.

## Usage
To build your project, prepare a YAML file (e.g., `asmx-build.yaml`) that specifies the sources, dependencies, and build instructions. Then, run the following command in the terminal:
```shell
asmx.exe build.yaml
```
This command will read the YAML configuration file and execute the build process accordingly, compiling all specified NASM and C source files and linking them into a single executable.

## Documentation

Additional documentation and detailed guides are available in the `doc/` directory of this repository. Here you can find:

- **`asmx-build.yaml`**: A sample YAML configuration file that shows how to set up your build environment. This file provides a practical example of how to specify sources and define ignore patterns.

- **`Build.md`**: A comprehensive guide to building projects with this tool, including advanced configuration options, usage tips, and troubleshooting advice.

For more information and examples, please refer to the files in the `doc/` directory.

## Contributing
Contributions to this project are welcome! Please fork the repository and submit a pull request with your proposed changes.

## License
This project is licensed under the GNU General Public License (GPL), which ensures that all derivatives of this project remain open source and freely available under the same terms.
