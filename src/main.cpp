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

bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	if(((x2 < x1)&&(x1 < x2 + w2))||((x2 < x1 + w1)&&(x1 + w1 < x2 + w2))||((x1 < x2)&&(x2 < x1 + w1))||((x1 < x2 + w2)&&(x2 + w2 < x1 + w1))){
		if(((y2 < y1)&&(y1 < y2 + h2))||((y2 < y1 + h1)&&(y1+h1 < y2 + h2))||((y1 < y2)&&(y2 < y1 + h1))||((y1 < y2 + h2)&&(y2 + h2 < y1 + h1))){
			return true;
		}
	}
	return false;
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
	SDL_Texture *background = loadTexture(resPath + "Full Moon - background.png", renderer);
	SDL_Texture *player = loadTexture(resPath + "LayeredSprites.png", renderer);
	SDL_Texture *grue = loadTexture(resPath + "Grue.png", renderer);
	SDL_Texture *beam = loadTexture(resPath + "beams.png", renderer);

	//iW and iH are the clip width and height
	//We'll be drawing only clips so get a center position for the w/h of a clip
/*	int iW = 100, iH = 100;
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
*/	int pW = 24, pH = 26;
	int px = SCREEN_WIDTH / 2 - pW / 2 - 100;
	int py = SCREEN_HEIGHT - pH;
	int pyinit = py;
	int gW = 56, gH = 71;
	int gx = SCREEN_WIDTH / 2 - gW / 2 + 100;
	int gy = SCREEN_HEIGHT - gH;
	int bW = 86, bH = 50;
	int bx = px;
	int by = py;

	//Setup the clips for our image
	//SDL_Rect clips[4];
	SDL_Rect pclips[18];
	SDL_Rect gclips[4];
	SDL_Rect bclips[2];
	//Since our clips our uniform in size we can generate a list of their
	//positions using some math (the specifics of this are covered in the lesson)
	for (int i = 0; i < 4; ++i){
	/*	clips[i].x = i / 2 * iW;
		clips[i].y = i % 2 * iH;
		clips[i].w = iW;
		clips[i].h = iH;
	*/	gclips[i].x = i % 2 * gW;
		gclips[i].y = i / 2 * gH;
		gclips[i].w = gW;
		gclips[i].h = gH;
	}
	for (int i = 0; i < 18; ++i){
                pclips[i].x = i % 6 * pW;
                pclips[i].y = i / 6 * pH;
                pclips[i].w = pW;
                pclips[i].h = pH;
        }
	bclips[0].x = 0;
	bclips[0].y = 0;
	bclips[0].w = 0;
	bclips[0].h = 0;
	bclips[1].x = 360;
	bclips[1].y = 235;
	bclips[1].w = bW;
	bclips[1].h = bH;
	//Specify a default clip to start with
	//int useClip = 0;
	int pClip = 2;
	int gClip = 0;
	int bClip = 0;

	SDL_Event e;
	bool quit = false;
	bool pFaceRight = true;
	bool pAerial = false;
	bool beamActive = false;
	bool gAlive = true;
	int pVelX = 0;
	int pVelY = 0;
	int gVelX = 1;
	while (!quit){
		//Event Polling
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				quit = true;
			}
			//Use number input to select which clip should be drawn
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0){
				switch (e.key.keysym.sym){
/*					case SDLK_1:
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
*/					case SDLK_UP:
						pVelY -= 2;
						break;
					case SDLK_DOWN:
						pVelY += 1;
						if (py >= pyinit) { pAerial = false;}
						break;
					case SDLK_LEFT:
						pFaceRight = false;
						pVelX -= 1;
						break;
					case SDLK_RIGHT:
						pFaceRight = true;
						pVelX += 1;
						break;
					case SDLK_SPACE:
						beamActive = true;
						break;
					case SDLK_ESCAPE:
						quit = true;
						break;
					default:
						break;
				}
			}
			else if (e.type == SDL_KEYUP && e.key.repeat == 0){
				switch (e.key.keysym.sym){
					case SDLK_UP:
						pVelY += 2;
						break;
					case SDLK_DOWN:
						pVelY -= 1;
						break;
					case SDLK_LEFT:
						pVelX += 1;
						break;
					case SDLK_RIGHT:
						pVelX -= 1;
						break;
					case SDLK_SPACE:
						beamActive = false;
						break;
				}
			}
		}
		//Rendering
		SDL_RenderClear(renderer);
		//Draw the background
		renderTexture(background, renderer, -106, 0);
		//Draw the image
		//renderTexture(image, renderer, x, y, &clips[useClip]);
		//Update position
		px += pVelX;
		py += pVelY;
		//Fall
		py += 1;
		//Avoid entering the ground
		if (py > pyinit) {
			py = pyinit;
		}
		//Identify position and facing
		if (pVelX > 0) {
			pFaceRight = true;
		} else if (pVelX < 0) {
			pFaceRight = false;
		}
		if (py < pyinit) { 
			pAerial = true; 
		} else { 
			pAerial = false; 
		}
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
		//Draw the player
		renderTexture(player, renderer, px, py, &pclips[pClip]);
		//Update grue position, direction
		gx += gVelX;
		if (gx > 500) { 
			gVelX = -gVelX; 
			gClip = 2;
		}
		if (gx < 100) {
			gVelX = -gVelX;
			gClip = 0;
		}
		//Draw the grue if alive
		if (gAlive){
			renderTexture(grue, renderer, gx, gy, &gclips[gClip]);
		}
		//Draw the beam if active, positioned in front of the player
		if (beamActive) {
			bClip = 1;
		} else {
			bClip = 0;
		}
		by = py + pH/2 - bH/2;
		if (pFaceRight) {
			bx = px + 10;
		} else {
			bx = px - bW + pW - 10;
		}
		renderTexture(beam, renderer, bx, by, &bclips[bClip]);
		//Check collision, update state
		if (beamActive) {
			if(checkCollision(bx, by, bW, bH, gx, gy, gW, gH)){
				gAlive = false;
			}
		}
		//Update the screen
		SDL_RenderPresent(renderer);
	}
	//Clean up
	cleanup(background, renderer, window);
	//cleanup(image, renderer, window);
	cleanup(player, renderer, window);
	cleanup(grue, renderer, window);
	cleanup(beam, renderer, window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
