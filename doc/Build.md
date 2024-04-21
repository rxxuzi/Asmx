# Configuration File Format

Version : **1.0.0-alpha**

The YAML configuration file is crucial for specifying how your project is built. The `project` key directly determines the name of the executable file generated. For example, setting `project: app` will create an executable named `app.exe`.

Ensure to include `!asmx` at the top of the file to allow the program to recognize the file as a configuration for asmx.

## Key Elements of the YAML Configuration:

- **`project`**: Specifies the base name of the executable file produced by the build process.

- **`sources`**: Lists the directories or specific files to be included in the build. Note, the inclusion of directories is non-recursive by default.

- **`ignore`**: Specifies files or directories to exclude from the build. This setting helps in omitting non-essential files from the build process.

