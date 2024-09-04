# CHANGELOG

## Version 2.0

### Major Changes
- Switched from YAML to JSON for configuration files.
- Introduced new command-line interface with subcommands.
- Added support for custom build configuration files.

### New Features
- `asmx gen` command to generate a default configuration file.
- `-i` option to specify custom input configuration files.
- Automatic loading of `asmx.json` if no config file is specified.

### Improvements
- Enhanced error handling and user feedback.
- Streamlined build process.

## Version 1.2

### New Features
- Implemented `libraries` support in configuration.

## Version 1.0

### Initial Release
- Basic build functionality using YAML configuration.
- Support for specifying project name, sources, and ignore patterns.
- Non-recursive directory inclusion by default.

### Key Elements of the YAML Configuration:
- `project`: Specified the base name of the executable file.
- `sources`: Listed directories or specific files for building.
- `ignore`: Specified files or directories to exclude from the build.

### Notes
- YAML configuration required `!asmx` at the top for recognition.
- Project name in config directly determined the executable name (e.g., `project: app` created `app.exe`).