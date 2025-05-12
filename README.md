# claPlusPlus

A simple and flexible C++ library for parsing command-line arguments.

This library allows you to define, set, and retrieve command-line options such as short and long names, help messages, default values, environment variables, and more.

## Features

- Supports short (`-s`) and long (`--long`) argument names.
- Ability to define custom help messages for user-friendly output.
- Option to specify if an argument is required or optional.
- Supports default values for arguments.
- Supports environment variables for argument values.
- Option to retrieve values from environment variables automatically (like: APPNAME_ARG1).

## Installation

1. Clone the repository to your local machine:
   ```bash
   git clone https://github.com/csboo/claplusplus
   ```
2. Add the library to your project by including the relevant header [Parser.hpp](include/Parser.hpp) and linking the corresponding source file.

## Example Usage
- see [example](src/example.cpp)

- run your app like this:
```bash
./my_program --file example.txt --verbose true
```
```bash
export FILE_PATH="example.txt"
./my_program --file $FILE_PATH
```
