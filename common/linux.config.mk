# This config should make all the tweaks nessisary to build on linux

REV         = $(shell sh -c 'git rev-parse --short @{0}')
CP          = cp
WINDRES     = i686-w64-mingw32-windres
CC          = i686-w64-mingw32-gcc
AS          = i686-w64-mingw32-as
