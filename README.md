# FakeFuzzyFinder

A (almost) fuzzy finder for the command line

![Animated demo](fff.gif)

This is a tool that will take a list of text lines and filter them by a (fake) fuzzy search.

It can be used to quickly search through file lists, git branches, processes, history, etc.

NOTE: This is basically a C++ port of this: https://github.com/junegunn/fzf

I used the same ideas behind it and hopefully it shows, in a good way.

The original project is in Go, and while it claims "no dependencies", this is not 100% accurate. It will need no run-time dependencies, but it will still download additional libraries to handle the terminal input/output.

This C++ version is actually dependency free: not even ncurses are used, and interaction with the terminal is done through ANSI escape codes.


## How to install and use

- Clone this repository
- Inside of the repository type "make". G++ is needed to compile.
- Add the directory containing the executable to your `PATH`
- Set up your own key bindings, or use the included `zsh_key_bindings.zsh` or `bash_key_bindings.bash` file. The bash file is designed for compatibility with bash 3.2 but will also work with bash version >= 4.0 .

NOTE: These files need to be used with `source zsh_key_bindings.zsh` or `source bash_key_bindings.bash`

NOTE 2: Bash terminal output on older bash versions on OS X might have a visual glitch, but it will still work.


## The vim plugin

To install the vim plugin, simply copy all the contents the `fff_vim` directory to your `~/.vim/pack/vendor/start` directory.

This will create a `Files` command that you can run in normal mode with `:Files`.

By default there are no key bindings for this, but as an example, it can be set to `Ctrl+t` b
y putting this line in your ~/.vimrc:

`nnoremap <C-T> :Files<C-M>`


## Default key bindings

ZSH Only:
- `Ctrl+f` Automatic search - this will try to recognize the command (ssh, git, ps, etc.) or give the default file search

Both ZSH and Bash
- `Ctrl+t` search file names
- `Ctrl+b` search git branches
- `Ctrl+h` search history
- `Ctrl+p` search processes
- `Ctrl+o` search pod names
- `Ctrl+y` search known ssh hosts


## Adding a custom search

The executable has two ways of getting the input: by loading all data and performing the search, or by streaming the data and performing the search while still reading it (results will be updated in real time while data is loading).

NOTE: The result will be exactly what is shown, so the extract specific information from the output line (e.g. only the PID from the process list), some processing will be necessary.

- Loading all data
	This is the default method and works for most situations. If the data is relatively big everything will still work, but the start up time could be high (due to waiting for the data to be loaded)

	This works by piping the data into the executable:
		`ls -l | fff` will feed the list of files to the executable and the result will be printed to standard output.

	Example:
		`ps ax | tail +2 | cut -b 1-120 | fff | tee /dev/tty | tail -1`

	The `tee /dev/tty` is necessary when running it in the shell env, to allow copying the stdout. Without it, no output will be shown.

- Streaming data
	This is an alternative method that has no start up lag, but muight show sub-optimal results in the beginning, because data is still being loaded.

	To use this method, fff needs to read data from a stream other than stdin, otherwise no keyboard input would be possible.

	This is why fff will execute the command set in the env variable `FFF_COMMAND` and load the output from the executed command instead of through a pipe.

	Example:
		`FFF_COMMAND="find ." fff | tee /dev/tty | tail -1`



