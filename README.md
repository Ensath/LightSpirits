Copyright (c) 2016 Noah Freed

This program is available under the "zlib license". Please see the file COPYING.

This program uses code from Will Usher's TwinklebearDev SDL 2.0 tutorial. Check it out at http://www.willusher.io/pages/sdl2/.

Sprites and other images for this program found on http://opengameart.org:

- The player character uses the "Margery Limited" sprite set by software_atelier.

- The main enemy uses the "Grue" sprite set by Jesse McCarthy, licensed under CC-BY 3.0.

- The light beam attack uses the "Lasers and beams" art by Rawdanitsu.

- The background uses "Background Night" art by Alekei, licensed under CC-BY 3.0.

- The other enemies (yet to be added) use the "16x16, 16x24, 32x32 rpg enemies--updated" sprite set by Stephen "Redshrike" Challener, licensed under CC-BY 3.0.

A description of CC-BY 3.0 and a link to the full license can be found at https://creativecommons.org/licenses/by/3.0/.

I made the sprite for the wisp.

#Light Spirits

##Build information

Based on SDL. 
Generates a window and lets the user move a character using the arrow keys and fire a beam of light with the space bar.

###To run:

- Make sure SDL 2.0 and SDL_image 2.0 are installed. 

- Download and unzip this repository.

- Go to the src directory, then run LightSpirits.

You can install SDL 2.0 with the command "sudo apt-get install libsdl2-dev", or download it from https://www.libsdl.org/download-2.0.php. 
You can install SDL_image 2.0 with the command "sudo apt-get install libsdl2-image-dev", or download it from https://www.libsdl.org/projects/SDL_image/.
If you install SDL 2.0 to somewhere other than the default directory, please adjust "SDL_INCLUDE" line in the Makefile to point to its location.

##User information

Description: This is a 2D game about a girl and a will-o'-the-wisp who fight creatures of darkness.

Controls: Move the girl around with the arrow keys. Shoot a beam of light with the space bar. Hit Escape to exit, or just close the window.

More info: Feel free to visit here for updates on the project!

##Project roadmap

- [x] Generate display

- [x] Allow movement based on keyboard input (left, right, jump)

- [ ] Find/create appropriate sprites for playable characters, enemies, and terrain

- [ ] Proper interaction with terrain

- [x] Allow attacks based on keyboard input

- [ ] Proper interaction with enemies (deal and receive damage, hitstun)

- [x] Write instructions for how to play

##Contact info

Email me at nfreed4 at gmail dot com.
