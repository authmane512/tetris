![screenshot](https://raw.githubusercontent.com/authmane512/tetris/master/screenshot.png)

This project is a simple implementation of the Tetris famous game. 
I've made it mainly for learning purpose. I don't think that you
will find pleasure playing with it, but if you are a developer, you
can read the source code which is documented. Hope you will find it interesting!

You can access to the documentation here:

https://authmane512.github.io/tetris/html/files.html

To install this program on Linux, first download it:
```
git clone https://github.com/authmane512/tetris.git
cd tetris
```

Before compile it, you need SDL2 library which you can install like that on Ubuntu or Debian:
```
sudo apt install libsdl2-dev
```

Then you can compile the code:
```
make
```

Finally start the program:
```
./tetris
```

Here are some details:
- C89 standard
- Use of SDL2 library
- About 600 lines of clean code
- Works on GNU/Linux (Ubuntu or Debian)
- Installation with a Makefile
- Very fluid and reactive
- Show score at the end of game
- Only about 20 KB after complication
