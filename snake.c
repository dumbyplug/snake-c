#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

/*else if(event.type == SDL_MOUSEMOTION){
	SDL_GetMouseState(&rect.x, &rect.y);
	rect.x -= rect.w / 2;
	rect.y -= rect.h / 2;
}*/
char move(SDL_Rect *rect, SDL_Rect *apple, 
		int *size, int speed, int snake_facing){
	// This function move the snake to the desired location if it is possible
	int i, x, y;

	if(snake_facing == 0){
		x = rect[*size - 1].x + speed;
		y = rect[*size - 1].y;
	}
	else if(snake_facing == 1){
		x = rect[*size - 1].x;
		y = rect[*size - 1].y - speed;
	}
	else if(snake_facing == 2){
		x = rect[*size - 1].x - speed;
		y = rect[*size - 1].y;
	}

	else if(snake_facing == 3){
		x = rect[*size - 1].x;
		y = rect[*size - 1].y + speed;
	}

	// checking if the snake would leave the canvas, when the move played 
	if((x < 0) || (y < 0) ||
	   (x + rect[*size - 1].w > WINDOW_WIDTH) ||
	   (y + rect[*size - 1].h > WINDOW_HEIGHT)
	) return 1; // error code 1. snake will leave the canvas


	// checking if the snake would hit its body, when the move played
	for(i = 0; i < *size; i++){
		if((rect[i].x == x) && rect[i].y == y){
			return 2; // error code 2. the snake bump into itself
		}
	}

	if(((apple->x - 10) == x) && ((apple->y - 10) == y)){
		rect[*size].x = x;
		rect[*size].y = y;
		rect[*size].w = 40;
		rect[*size].h = 40;
		*size += 1;

		int empty_places[400];
		for(i = 0; i < 400; i++){
			empty_places[i] = (i / 20) * 100 + i % 20; 
		}
		for(i = 0; i < *size; i++){
			empty_places[(rect[i].y / 40) * 20 +(rect[i].x / 40)] = -1; 
		}
		char swapped;
		for(i = 0; i < 400 - 1; i++){
			swapped = 0;
			for(int j = 0; j < 400 - i - 1; j++){
				if(empty_places[j] > empty_places[j + 1]){
					empty_places[j] = empty_places[j] + empty_places[j + 1];
					empty_places[j + 1] = empty_places[j] - empty_places[j + 1];
					empty_places[j] = empty_places[j] - empty_places[j + 1];
					swapped = 1;
				}
			}
			if(!swapped)
				break;
		}
		int random_number = rand() % (400 - *size) + *size;

		apple->x = 10 + (empty_places[random_number] % 100) * 40; 
		apple->y = 10 + (empty_places[random_number] / 100) * 40; 
		return 0;
	}

	// delete tail. move all of body one left in array. initialize head
	for(i = 0; i < *size - 1; i++)
		rect[i] = rect[i + 1];
	rect[*size - 1].x = x;
	rect[*size - 1].y = y;
	return 0;
}

void snake_init(SDL_Rect *rect, int size){
	// placing its body parts next to each other
	for(int i = 0; i < size; i++){
		rect[i].x = 0 + 40 * i;  rect[i].y = 360;
		rect[i].w = 40;          rect[i].h = 40;
	}
}

void draw_snake(SDL_Renderer *renderer,SDL_Rect *rect, 
		SDL_Rect *eyes, int size, int snake_facing){

		SDL_SetRenderDrawColor(renderer, 8, 15, 23, 255);

		int index;
		for(index = 0; index < size; index++){
			SDL_RenderFillRect(renderer, rect + index);
		}

		if(snake_facing == 0){
			eyes[0].x = rect[size - 1].x + 30; 
			eyes[0].y = rect[size - 1].y + 10; 
			eyes[1].x = rect[size - 1].x + 30; 
			eyes[1].y = rect[size - 1].y + 25; 
		}
		else if(snake_facing == 1){
			eyes[0].x = rect[size - 1].x + 10; 
			eyes[0].y = rect[size - 1].y + 5; 
			eyes[1].x = rect[size - 1].x + 25; 
			eyes[1].y = rect[size - 1].y + 5; 
		}
		else if(snake_facing == 2){
			eyes[0].x = rect[size - 1].x + 5; 
			eyes[0].y = rect[size - 1].y + 10; 
			eyes[1].x = rect[size - 1].x + 5; 
			eyes[1].y = rect[size - 1].y + 25; 
		}
		else if(snake_facing == 3){
			eyes[0].x = rect[size - 1].x + 10; 
			eyes[0].y = rect[size - 1].y + 30; 
			eyes[1].x = rect[size - 1].x + 25; 
			eyes[1].y = rect[size - 1].y + 30; 
		}
		SDL_SetRenderDrawColor(renderer, 233, 15, 23, 255);
		SDL_RenderFillRect(renderer, &eyes[0]);
		SDL_RenderFillRect(renderer, &eyes[1]);
		
}

int main(void){
	srand(time(NULL));
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event event;

	int size = 8, speed = 40;
	SDL_Rect rect[400];
	SDL_Rect eyes[2];
	SDL_Rect apple;
	int snake_facing = 0; // 0 - right, 1 - up, 2 - left, 3 - down 

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, 
			WINDOW_HEIGHT, 0, &window, &renderer);

	snake_init(rect, size);

	eyes[0].w = 5; eyes[0].h = 5;
	eyes[1].w = 5; eyes[1].h = 5;

	apple.w = 20; apple.h = 20;
	apple.x = 10 + 120; apple.y = 10 + 120;

	int run = 1;
	int moveDelay = 0;
	char moveError = 0;
	while(run){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT)
				run = 0;
			else if(event.type == SDL_KEYDOWN){
				
			switch(event.key.keysym.sym){
			
			case SDLK_RIGHT:
				if(snake_facing != 2)
					snake_facing = 0;
				break;
			case SDLK_LEFT:
				if(snake_facing != 0)
					snake_facing = 2;
				break;
			case SDLK_UP:
				if(snake_facing != 3)
					snake_facing = 1;
				break;
			case SDLK_DOWN:
				if(snake_facing != 1)
					snake_facing = 3;
				break;
			}
			}
		}
		SDL_SetRenderDrawColor(renderer, 150, 213, 205, 255);
		SDL_RenderClear(renderer);

		if(moveDelay > 18){
			moveError = move(rect, &apple, &size, speed, snake_facing);
			moveDelay = 1;
		}
		draw_snake(renderer, rect, eyes, size, snake_facing);
		
		SDL_SetRenderDrawColor(renderer, 200, 54, 23, 255);
		SDL_RenderFillRect(renderer, &apple);
		

		SDL_RenderPresent(renderer);
		SDL_Delay(10);
		if(moveError)
			break;
		moveDelay++;
	}
	
	return 0;
}
