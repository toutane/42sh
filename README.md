## 42sh

### Usage

```bash
42sh [options] [script] [arguments...]
```

#### Options

- **-a**: Output AST in dot format
- **-c**: Execute input as a command
- **-p**: Pretty print AST
- **-v**: Verbose mode


### Usage of autotools

The autoreconf archive package need to be installed in order to be able
to `./configure with autotools`

On arch, you need to use the following command
`sudo pacman -S autoconf-archive`

Otherwise, you will get this error:
`./configure: line 4166: syntax error near unexpected token ,'`
`./configure: line 4166: AX_COMPILER_FLAGS(, , , -Wall -Wextra -Werror -std=c99 -pedantic)'`
