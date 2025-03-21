#include <SDL2/SDL.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

/*else if(event.type == SDL_MOUSEMOTION){
	SDL_GetMouseState(&rect.x, &rect.y);
	rect.x -= rect.w / 2;
	rect.y -= rect.h / 2;
}*/
int move(SDL_Rect *rect, int size, int x, int y){
	// This function move the snake to the desired location if it is possible
	int i;

	// checking if the snake would leave the canvas, when the move played 
	if((x < 0) || (y < 0) ||
	   (x + rect[size - 1].w > WINDOW_WIDTH) ||
	   (y + rect[size - 1].h > WINDOW_HEIGHT)
	) return 1; // error code 1. snake will leave the canvas

	// checking if the snake would hit its body, when the move played
	for(i = 0; i < size; i++){
		if((rect[i].x == x) && rect[i].y == y){
			return 2; // error code 2. the snake bump into itself
		}
	}

	// delete tail. move all of body one left in array. initialize head
	for(i = 0; i < size - 1; i++)
		rect[i] = rect[i + 1];
	rect[size - 1].x = x;
	rect[size - 1].y = y;
	return 0;
}

void snake_init(SDL_Rect *rect, int size){
	for(int i = 0; i < size; i++){
		rect[i].x = 0 + 40 * i;  rect[i].y = 360;
		rect[i].w = 40;          rect[i].h = 40;
	}
}

int main(void){
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, 
			WINDOW_HEIGHT, 0, &window, &renderer);

	int size = 8;
	SDL_Rect rect[400];
	snake_init(rect, size);

	int speed = 40;

	
	SDL_Event event;
		

	int run = 1;
	int index;
	while(run){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT)
				run = 0;
			else if(event.type == SDL_KEYDOWN){
				
			switch(event.key.keysym.sym){
			
			case SDLK_RIGHT:
				move(rect, size, rect[size - 1].x + speed, rect[size - 1].y);
				break;
			case SDLK_LEFT:
				move(rect, size, rect[size - 1].x - speed, rect[size - 1].y);
				break;
			case SDLK_UP:
				move(rect, size, rect[size - 1].x, rect[size - 1].y - speed);
				break;
			case SDLK_DOWN:
				move(rect, size, rect[size - 1].x, rect[size - 1].y + speed);
				break;
			}	
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		
		for(index = 0; index < size; index++){
			SDL_RenderFillRect(renderer, rect + index);
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}
	
	return 0;
}
