#!/bin/bash
./compile.sh debug
leaks -atExit -- bin/maindebug
rm bin/maindebug
rm -r bin/maindebug.dSYM