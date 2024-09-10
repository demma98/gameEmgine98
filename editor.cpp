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

void update(bool *running, int *change_jump, SDL_Renderer *renderer, Level *level, int *mode, int *entity);
void update_frame();
void render(SDL_Renderer *renderer, Level level);


int main(){
  bool running;
  int change_jump = 1;
  int mode = 0;
  int entity = 0;

  //open window
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("nametables", 64, 64, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);

  Level level("./levels/text.txt", renderer);
  level.setTable(0, 16, "./tables/text.txt", renderer, "./img/sheet.png");
  level.update();

  uint32_t time_old = SDL_GetTicks(), time_new;

  init();

  running = true;
  while(running){
    time_new = SDL_GetTicks();
    update(&running, &change_jump, renderer, &level, &mode, &entity);
    if(time_new - time_old > 1000/60){
      update_frame();
      time_old = SDL_GetTicks();
    }
    render(renderer, level);
  }

  level.save();
  level.destroy();
  //end

  return 0;
}

void init(){
}

void update(bool *running, int *change_jump, SDL_Renderer *renderer, Level *level, int *mode, int *entity){
  SDL_Event event;
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  std::list<Table>::iterator t = level->tables.begin();
  for(int i = 0; i != *mode - 1 && t != level->tables.end(); i++, t++);
  while (SDL_PollEvent(&event)) {
    switch (event.type) {

      case SDL_KEYDOWN:
        state = SDL_GetKeyboardState(nullptr);
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            *running = false;
            break;
          case SDLK_r:
            init();
            break;
          case SDLK_UP:
            if(*mode != 0){
              if(state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
                t->addRows(renderer, -1);
              else
                t->off_y += 8;
            }
            break;
          case SDLK_DOWN:
            if(*mode != 0){
              if(state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
                t->addRows(renderer, 1);
              else
                t->off_y -= 8;
            }
            break;
          case SDLK_LEFT:
            if(*mode != 0){
              if(state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
                t->addColumns(renderer, -1);
              else
                t->off_x += 8;
            }
            break;
          case SDLK_RIGHT:
            if(*mode != 0){
              if(state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
                t->addColumns(renderer, 1);
              else
                t->off_x -= 8;
            }
            break;

          case SDLK_z:
            t->flipVertical(renderer, true);
            break;
          case SDLK_x:
            t->flipHorizontal(renderer, true);
            break;
            
          case SDLK_a:
            *mode -= 1;
            if(*mode < 0)
              *mode = level->tables.size();
            break;
          case SDLK_s:
            *mode += 1;
            if(*mode > level->tables.size())
              *mode = 0;
            break;
          
          case SDLK_1:
            *change_jump = 1;
            break;
          case SDLK_2:
            *change_jump = 2;
            break;
          case SDLK_3:
            *change_jump = 3;
            break;
          case SDLK_4:
            *change_jump = 4;
            break;
          case SDLK_5:
            *change_jump = 5;
            break;
          case SDLK_6:
            *change_jump = 6;
            break;
          case SDLK_7:
            *change_jump = 7;
            break;
          case SDLK_8:
            *change_jump = 8;
            break;
          case SDLK_9:
            *change_jump = 9;
            break;
          case SDLK_0:
            *change_jump = 10;
            break;

        }
        break;

      case SDL_MOUSEBUTTONDOWN:
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        switch (event.button.button) {
          case SDL_BUTTON_LEFT:
            if(*mode != 0)
              t->changeBlock(renderer, (mouse_x - t->off_x) / Block().width, (mouse_y - t->off_y) / Block().height, *change_jump, true);
            else{
              int i = level->getEntity(mouse_x, mouse_y);
              if(i != -1)
                level->changeEntity(i, 1);
              else{
                level->addEntity(mouse_x, mouse_y);
              }
            }
            break;
          case SDL_BUTTON_RIGHT:
            if(*mode != 0)
              t->setBlock(renderer, (mouse_x - t->off_x) / Block().width, (mouse_y - t->off_y) / Block().height, 0, true);
            else{
              int i = level->getEntity(mouse_x, mouse_y);
              if(i != -1)
                level->deleteEntity(i);
            }
            break;
        }

        break;

      case SDL_QUIT:
        *running = false;
        break;
    }
  }
}

void update_frame(){
  const Uint8* state = SDL_GetKeyboardState(nullptr);
}

void render(SDL_Renderer *renderer, Level level){
  SDL_SetRenderDrawColor(renderer, 64, 0, 64, 255);
  SDL_RenderClear(renderer); //clear screen

  level.render(renderer);

  SDL_RenderPresent(renderer);
}