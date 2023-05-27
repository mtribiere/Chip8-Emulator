# Chip8-Emulator
A simple chip8 emulator for entertainement and learning purposes written in C++ using the SDL2 library.
Some roms have been included to play around with the emulator.

> /!\ Audio channels have not been implemented yet (and will probably never be)

# # Showcase
<img src="/img/tetris.gif" width="200"> <img src="/img/airplane.gif" width="200"> <img src="/img/demo.gif" width="200"> </br>


# # Compile from source
### MakeFile build
```console
user@debian:~$ mkdir build && cd build
user@debian:~$ cmake .. 
user@debian:~$ make
```
### Ninja build
```console
user@debian:~$ mkdir build && cd build
user@debian:~$ cmake -G Ninja .. 
user@debian:~$ ninja
```

# # Play 
```console
user@debian:~$ make run
```
[OR]
```console
user@debian:~$ ninja run
```
Crafted with :heart: by mtribiere
