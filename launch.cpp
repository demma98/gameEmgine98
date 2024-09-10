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
            break;
        }
        break;
      case SDL_QUIT:
        *running = false;
        break;
    }
  }
}

void update_frame(Level *level){
  Living *entity_0 = *level->entities.begin();
  std::list<Table>::iterator table_0 = level->tables.begin();
  level->update();
  if(table_0->width * Block().width > SCREEN_WIDTH){
    if(entity_0->x > SCREEN_WIDTH/2)
      if(entity_0->x < table_0->width * Block().width - SCREEN_WIDTH/2)
        table_0->off_x = SCREEN_WIDTH/2 - entity_0->x;
      else
        table_0->off_x = SCREEN_WIDTH - table_0->width * Block().width;
    else
      table_0->off_x = 0;
  }
  
  if(table_0->height * Block().height > SCREEN_HEIGHT){
    if(entity_0->y > SCREEN_HEIGHT/2)
      if(entity_0->y < table_0->height * Block().height - SCREEN_HEIGHT/2)
        table_0->off_y = SCREEN_HEIGHT/2 - entity_0->y;
      else
        table_0->off_y = SCREEN_HEIGHT - table_0->height * Block().height;
    else
      table_0->off_y = 0;
  }
}

void render(SDL_Renderer *renderer, Level level){
  SDL_RenderClear(renderer); //clear screen

  level.render(renderer);

  SDL_RenderPresent(renderer);
}