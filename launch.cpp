#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "cmath"
#include "list"

#include "level.h"


#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360

using namespace std;

void init();

void update(bool *running);
void update_frame(Level *level);
void render(SDL_Renderer *renderer, Level level);


int main(){
  bool running;

  //open window
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("nametables", 64, 64, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);

  Level level("./levels/text.txt", renderer);
  level.setTable(0, 16, "./tables/text.txt", renderer, "./img/sheet.png");

  uint32_t time_old = SDL_GetTicks(), time_new;
  
  SDL_SetRenderDrawColor(renderer, 64, 0, 64, 255);

  init();

  running = true;
  while(running){
    time_new = SDL_GetTicks();
    update(&running);
    if(time_new - time_old > 1000/60){
      update_frame(&level);
      time_old = SDL_GetTicks();
    }
    render(renderer, level);
  }

  //end
  level.destroy();

  return 0;
}

void init(){
}

void update(bool *running){
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            *running = false;
            break;
          case SDLK_r:
            init();
        }
        break;
      case SDL_QUIT:
        *running = false;
        break;
    }
  }
}

void update_frame(Level *level){
  level->update();
  if(level->tables[0].width * Block().width > SCREEN_WIDTH){
    if(level->entities[0].x > SCREEN_WIDTH/2)
      if(level->entities[0].x < level->tables[0].width * Block().width - SCREEN_WIDTH/2)
        level->tables[0].off_x = SCREEN_WIDTH/2 - level->entities[0].x;
      else
        level->tables[0].off_x = SCREEN_WIDTH - level->tables[0].width * Block().width;
    else
      level->tables[0].off_x = 0;
  }
  
  if(level->tables[0].height * Block().height > SCREEN_HEIGHT){
    if(level->entities[0].y > SCREEN_HEIGHT/2)
      if(level->entities[0].y < level->tables[0].height * Block().height - SCREEN_HEIGHT/2)
        level->tables[0].off_y = SCREEN_HEIGHT/2 - level->entities[0].y;
      else
        level->tables[0].off_y = SCREEN_HEIGHT - level->tables[0].height * Block().height;
    else
      level->tables[0].off_y = 0;
  }
}

void render(SDL_Renderer *renderer, Level level){
  SDL_RenderClear(renderer); //clear screen

  level.render(renderer);

  SDL_RenderPresent(renderer);
}