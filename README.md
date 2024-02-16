# 42sh

42sh is a POSIX-compatible shell that draws inspiration from bash, offering versatility in reading input through various means such as strings, files, or standard input. This shell implementation encompasses fundamental shell functionalities, including control-flow mechanisms, variable handling, parameter expansion, and command substitution. Additionally, 42sh features a set of built-in commands, such as echo, cd, export, and alias, providing a comprehensive environment for shell scripting and command execution.

## Usage

To read input from a string, use the `-c` option followed by the shell script enclosed in quotes.

```bash
42sh -c "echo 'Hello, world!'"
```

To read input from a file, provide the script file as a positional argument.

```bash
42sh script.sh
```

If no other source is provided, 42sh will read commands from standard input.

```bash
42sh < script.sh
```

The command line syntax is as follows:

```bash
42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]
```

## Installation

```bash
git clone git@github.com:toutane/42sh.git
cd 42sh
```

Before building the project, ensure that you have the necessary dependencies installed.
```bash
# Example for installing dependencies on Debian/Ubuntu
sudo apt-get install autoconf autoconf-archive automake libtool
```

Run the autoreconf command to generate the configure script. Then run the configure script to create the build environment. Compile the project with make.
```bash
autoreconf --install
./configure
make
```

If you want to install the compiled binary system-wide, you can use:

```bash
sudo make install
```
<!--
### Usage of autotools

The autoreconf archive package need to be installed in order to be able
to `./configure with autotools`

On arch, you need to use the following command
`sudo pacman -S autoconf-archive`

Otherwise, you will get this error:
`./configure: line 4166: syntax error near unexpected token ,'`
`./configure: line 4166: AX_COMPILER_FLAGS(, , , -Wall -Wextra -Werror -std=c99 -pedantic)'`-->
