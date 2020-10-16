# Pong
Made using [SDL2](https://www.libsdl.org/).
Use W to move up and S to move down.

Dependencies
---------
- [SDL_ttf](https://www.libsdl.org/projects/SDL_ttf/)
- [SDL_image](https://www.libsdl.org/projects/SDL_image/)
- [SDL_mixer](https://www.libsdl.org/projects/SDL_mixer/)

Compiling
--------
`g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include -I PATH_TO_SDL2/include && g++ *.o -o bin/release/main -s -L PATH_TO_SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -mwindows`

![](https://i.imgur.com/RNwJWHK.png)
