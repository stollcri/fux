# Fux

Fux is a command line tool to automatically fix mistakes in the last command you typed. This utility works with `fc`, a tool which enables editing and reexecution reviously run commands, to automatically correct previous commnds. Once fux is installed (`make && make install`) create an alias to fc where fux is the designated editor (e.g. `alias fc=`fc -e /usr/local/bin/fux`). Then, typing `fc` will automatically fix and re-run the last command.

If there is too low of a probablitity that the suggested command is correct, then fux will reissue the command as a comment (it will be preceeded with "#"). Then, (On a Mac) press the up-arrow, then ctrl-a, and delete the comment marker.

Fux uses a list of example commands to make its corrections, so adding commands is as simple as adding a new example to the configuration. When there are two values, such as `sudo apt-get install *` and `sudo apt-get install * --only-upgrade`, which vary only at the end, the shorter of the two should be listed first. This will ensure that the shorter is picked by default (when the parts unique to the longer version are not present, `--only-upgrade` in this case). It also reads from the standard input, so it is possible to run `echo "this command fails" | fux` to see how it will be corrected.

Fux is insipred by ["The Fuck"](https://github.com/nvbn/thefuck); the difference is that fux is written in pure C rather than Python, and is designed to use a more sophisticated algorithm (rather than following a lot of heuristics). If you want to use a heuristic based approach, or prefer a Python program, then check out ["The Fuck"](https://github.com/nvbn/thefuck).
