#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

#include "hardheaded.h"
#include "keyboard.h"
#include "font.h"

#include "sprite.h"

#define FPS 30

#define MAX(a,b) ((a)>(b)?(a):(b))

SDL_Color red = {0xAA, 0X55, 0x00};

void renderInit(App *app){
  app->screen = SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE);


  sprite_init(&app->game.board.indy.sprite, 
	  0, 0, // origin
	  32, 64, 4, // frame size and count
	  "data/indy.png" // source
  );
  app->game.board.indy.body.sprite = &app->game.board.indy.sprite;
  app->game.board.indy.body.pos.x = 0;

  sprite_init(&app->game.board.zombie.sprite, 
	  0, 0, // origin
	  32, 32, 3, // frame size and count
	  "data/zombie.png" // source
  );
  app->game.board.zombie.body.sprite = &app->game.board.zombie.sprite;
  app->game.board.zombie.body.pos.x = 100;

  sprite_init(&app->game.board.head.sprite, 
	  0, 0, // origin
	  32, 32, 3, // frame size and count
	  "data/head.png" // source
  );
  app->game.board.head.body.sprite = &app->game.board.head.sprite;
  app->game.board.head.body.pos.x = 200;

}

void handleDelay(Uint32 start) {
  Uint32 end = SDL_GetTicks();
  int actual_delta = end - start;
  int expected_delta = 1000/FPS;
  int delay = MAX(0, expected_delta - actual_delta);
  //printf("delay %d %d %d\n", actual_delta, expected_delta, delay);
  SDL_Delay(delay);
}

void sprite_demo(App *app, Body *body) {
	  body->frame += 0.2;
	  if((int)body->frame > body->sprite->frame_count) {
		  body->frame = 0;

		  if((rand() % 10) == 0) {
			  body->action ++;
			  if((int)body->action >= ACTION_COUNT)
				  body->action = ACTION_MOVE;
		  }
	  }

	  body->angle += 1;
	  if((int)body->angle >= 360)
		  body->angle = 0;

      SDL_Rect src;
	  sprite_rotated_rect(
			  body->sprite, 
			  body->action, 
			  (int)body->frame,
			  (int)body->angle,
			  &src);

	  SDL_BlitSurface(body->sprite->rotated, &src, app->screen, &body->pos);
}


int main(int argc, char* args[]) {
  App app;
  app.state = STATE_MENU;
  memset(&app, 0, sizeof(app));

  renderInit(&app);
  init_font();

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0 ) return 1;

  while(app.state != STATE_EXIT){
	  Uint32 startTime = SDL_GetTicks();
	  bindKeyboard(&app);
	  Uint32 color = SDL_MapRGB(app.screen->format, 0x40, 0x40, 0x40 );
	  SDL_FillRect(app.screen, NULL , color);
	  text_write(app.screen, 200, 200, "teste", 0);

	  sprite_demo(&app, &app.game.board.indy.body);
	  sprite_demo(&app, &app.game.board.zombie.body);
	  sprite_demo(&app, &app.game.board.head.body);

	  SDL_UpdateRect(app.screen, 0, 0, 0, 0);
	  handleDelay(startTime);
  }
}
