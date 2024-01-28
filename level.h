#include "SDL2/SDL.h"
#include "vector"

#ifndef LIVING_GUARD
#include "living.h"
#endif

#ifndef DISK_GUARD
#include "disk.h"
#endif

#ifndef TABLE_GUARD
#include "table.h"
#endif

#include <stdio.h>

#define LEVEL_GUARD 1


class Level{
    public:
        const char *entities_path;
        int entities_n = 2;
        std::vector <Living> entities;
        std::vector <Table> tables;
        SDL_Texture *textures[8];

        Level(const char *entities_path, SDL_Renderer *renderer);
        void update(bool update_entities);
        void update();
        void render(SDL_Renderer *renderer);
        void destroy();

        void load();
        void save();
        void setTable(int num, int max_block, const char *path, SDL_Renderer *renderer, const char *sheet_path);

        void changeEntity(int n, int change);
        void setEntity(int n, int set);
        int getEntity(int x, int y);
        void addEntity(int x, int y);
        void deleteEntity(int n);
};