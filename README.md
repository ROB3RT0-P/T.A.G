# TAG - Text Adventure Game

//////////////////////////////////////////////////////////////////////////////////////////////

Copyright (c) Robert Parry - 2024

This software is provided 'as-is', without any express or implied <br />
warranty. In no event will the authors be held liable for any damages <br />
arising from the use of this software. <br />

Permission is granted to anyone to use this software for any purpose, <br />
including commercial applications, and to alter it and redistribute it <br />
freely, subject to the following restrictions: <br />

1. The origin of this software must not be misrepresented; you must not <br />
claim that you wrote the original software. If you use this software <br />
in a product, an acknowledgment in the product documentation would be <br />
appreciated but is not required. <br />

2. Altered source versions must be plainly marked as such, and must not be <br />
misrepresented as being the original software. <br />

3. This notice may not be removed or altered from any source <br />
distribution. <br />

//////////////////////////////////////////////////////////////////////////////////////////////

Peril's passage is a short demo where the user must input which passage way they want to <br />
follow in order to try and escape. My intention was to create a good base game that could be <br />
expanded upon in order to make more complex text adventures games. The main components <br />
implemented are as follows:<br />

- Game loop
- State machine
- Console
- Keyboard input handling
- Text rendering
- Texture rendering
- Parallax background rendering
- Entity manager
- Resource manager
- Audio playback
- Utils

//////////////////////////////////////////////////////////////////////////////////////////////

TODO: Aspects that I would expand upon given more time.<br />
- Expand the number of background textures so that they can be loaded at random to create <br />
    more environment variety.<br />
- Add puzzles / Riddles to progress - Stored and retrieved in a data structure and given to <br />
    the player at random.<br />
- Enemies - Can be defeated on a dice roll system.<br />
- Inventory - Player can pick up items and use them to heal etc...<br />
- Fade in and out animations to make the transitions more seemless.<br />
- Add a difficulty setting.<br />

//////////////////////////////////////////////////////////////////////////////////////////////

BUILD:<br />
MSBUILD - VS2022<br />
Directories for SDL2 are set up for Debug and Release<br />

If any issues occur with SDL2, such as missing files etc... SDL2, SDL2_mixer, SDL2_ttf <br />
and SDL2_image can be redownloaded and readded to the includes and libraries in the project <br />
settings.<br />

INSTALL:<br />
N/A<br />

USAGE:<br />
Game Controls:  [TAB] - Return <br />
                [RETURN] - Play, Pause <br />
                [ESC] - Quit <br />
                [A]~[Z] - Console input - 'left' / 'right' <br />


//////////////////////////////////////////////////////////////////////////////////////////////
