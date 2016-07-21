// Copyright (c) 2016 Noah Freed
// This program is available under the "zlib license". Please see the file COPYING.
// This program uses code from Will Usher's TwinklebearDev SDL 2.0 tutorial. Check it out at http://www.willusher.io/pages/sdl2/.

#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "res_path.h"
#include "cleanup.h"

//Screen attributes
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

/*
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}
/*
 * Loads an image into a texture on the rendering device
 * @param file The image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
}
/*
 * Draw an SDL_Texture to an SDL_Renderer at some destination rect
 * taking a clip of the texture if desired
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param dst The destination rectangle to render the texture too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr){
	SDL_RenderCopy(ren, tex, clip, &dst);
}
/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of the texture's
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}

int main(int, char**){
	//Start up SDL and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	//Setup our window and renderer
	SDL_Window *window = SDL_CreateWindow("Light Spirits", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		SDL_Quit();
		return 1;
	}
	const std::string resPath = getResourcePath("images");
	SDL_Texture *image = loadTexture(resPath + "image.png", renderer);
	if (image == nullptr){
		cleanup(image, renderer, window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	SDL_Texture *player = loadTexture(resPath + "LayeredSprites.png", renderer);

	//iW and iH are the clip width and height
	//We'll be drawing only clips so get a center position for the w/h of a clip
	int iW = 100, iH = 100;
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
	int pW = 24, pH = 26;
	int px = SCREEN_WIDTH / 2 - pW/ 2;
	int py = SCREEN_HEIGHT / 2 - pH / 2;
	int pyinit = py;

	//Setup the clips for our image
	SDL_Rect clips[4];
	SDL_Rect pclips[18];
	//Since our clips our uniform in size we can generate a list of their
	//positions using some math (the specifics of this are covered in the lesson)
	for (int i = 0; i < 4; ++i){
		clips[i].x = i / 2 * iW;
		clips[i].y = i % 2 * iH;
		clips[i].w = iW;
		clips[i].h = iH;
	}
	for (int i = 0; i < 18; ++i){
                pclips[i].x = i % 6 * pW;
                pclips[i].y = i / 6 * pH;
                pclips[i].w = pW;
                pclips[i].h = pH;
        }
	//Specify a default clip to start with
	int useClip = 0;
	int pClip = 0;

	SDL_Event e;
	bool quit = false;
	bool pFaceRight = false;
	bool pAerial = false;
	while (!quit){
		//Event Polling
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				quit = true;
			}
			//Use number input to select which clip should be drawn
			if (e.type == SDL_KEYDOWN){
				switch (e.key.keysym.sym){
					case SDLK_1:
					case SDLK_KP_1:
						useClip = 0;
						break;
					case SDLK_2:
					case SDLK_KP_2:
						useClip = 1;
						break;
					case SDLK_3:
					case SDLK_KP_3:
						useClip = 2;
						break;
					case SDLK_4:
					case SDLK_KP_4:
						useClip = 3;
						break;
					case SDLK_UP:
						py -= 1;
						if (py < pyinit) { pAerial = true;}
						break;
					case SDLK_DOWN:
						py += 1;
						if (py >= pyinit) { pAerial = false;}
						break;
					case SDLK_LEFT:
						pFaceRight = false;
						px -= 1;
						break;
					case SDLK_RIGHT:
						pFaceRight = true;
						px += 1;
						break;
					case SDLK_ESCAPE:
						quit = true;
						break;
					default:
						break;
				}
			}
		}
		//Rendering
		SDL_RenderClear(renderer);
		//Draw the image
		renderTexture(image, renderer, x, y, &clips[useClip]);
		if (pFaceRight) {
			if (pAerial) {
				pClip = 5;
			} else {
				pClip = 2;
			}	
		} else {
			if (pAerial) {
				pClip = 4;
			} else {
				pClip = 0;
			}
		}
		renderTexture(player, renderer, px, py, &pclips[pClip]);
		//Update the screen
		SDL_RenderPresent(renderer);
	}
	//Clean up
	cleanup(image, renderer, window);
	cleanup(player, renderer, window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
