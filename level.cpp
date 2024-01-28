#include "level.h"


Level :: Level(const char *entities_path, SDL_Renderer *renderer){
    this -> entities_path = entities_path;

    //preparing textures
    textures[0] = IMG_LoadTexture(renderer, "./img/player.png");
    textures[1] = IMG_LoadTexture(renderer, "./img/enemy.png");
    
    load();
}

void Level :: update(bool update_entities){
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    for(std::vector<Living>::iterator e = entities.begin(); e != entities.end(); e++){
        if(update_entities)
            switch(e->id){
                case 0:
                    if(state[SDL_SCANCODE_RIGHT]){
                        if(e->speed_x < 1.5)
                            e->speed_x += 0.3;
                    }
                    else{
                        if(e->speed_x > 0)
                            if(e->speed_x > 0.3)
                                e->speed_x -= 0.3;
                            else
                                e->speed_x = 0;
                    }
                    if(state[SDL_SCANCODE_LEFT]){
                        if(e->speed_x > -1.5)
                            e->speed_x -= 0.3;
                    }
                    else{
                        if(e->speed_x < 0)
                            if(e->speed_x < -0.3)
                                e->speed_x += 0.3;
                            else
                                e->speed_x = 0;
                    }
                    
                    if(state[SDL_SCANCODE_DOWN]){
                        if(e->speed_y < 1.5)
                            e->speed_y += 0.3;
                    }
                    else{
                        if(e->speed_y > 0)
                            if(e->speed_y > 0.3)
                                e->speed_y -= 0.3;
                            else
                                e->speed_y = 0;
                    }
                    if(state[SDL_SCANCODE_UP]){
                        if(e->speed_y > -1.5)
                            e->speed_y -= 0.3;
                    }
                    else{
                        if(e->speed_y < 0)
                            if(e->speed_y < -0.3)
                                e->speed_y += 0.3;
                            else
                                e->speed_y = 0;
                    }
                    break;
            }
        e->update(tables[0]);
        }
}

void Level :: render(SDL_Renderer *renderer){
    for(std::vector<Living>::iterator e = entities.begin(); e != entities.end(); e++){
        e->render(renderer, textures[e->id], tables[0].off_x, tables[0].off_y);
    }
    for(std::vector<Table>::iterator t = tables.begin();t != tables.end(); t++)
        t->render(renderer);
}

void Level :: update(){
    update(true);
}

void Level :: destroy(){
    for(std::vector<Table>::iterator t = tables.begin();t != tables.end(); t++)
            t->destroy();
}


void Level :: load(){
    FILE *f = fopen(entities_path, "r");

    if(f != NULL){
        int entities_l = Disk().loadNumber(f);
        int temp_x, temp_y, temp_id;
        int i;
        for(i = 0; i < entities_l; i++){
            temp_x = Disk().loadNumber(f);
            temp_x *= Block().width;
            temp_y = Disk().loadNumber(f);
            temp_y *= Block().height;
            temp_id = Disk().loadNumber(f);
            entities.push_back(Living(temp_x, temp_y, temp_id));
        }
    }

    fclose(f);
}

void Level :: save(){
    FILE *f = fopen(entities_path, "w");

    if(f != NULL){
        Disk().saveNumber(f, entities.size());

        for(int i = 0; i < entities.size(); i++){
            Disk().saveNumber(f, entities[i].x / Block().width);
            Disk().saveNumber(f, entities[i].y / Block().height);
            Disk().saveNumber(f, entities[i].id);
        }
    }

    for(std::vector<Table>::iterator t = tables.begin();t != tables.end(); t++)
        t->save();

    fclose(f);
}

void Level :: setTable(int num, int max_block, const char *path, SDL_Renderer *renderer, const char *sheet_path){
    tables.push_back(Table(renderer, max_block, path, sheet_path));
}


void Level :: changeEntity(int n, int change){
    if(n < entities.size()){
        entities[n].id += change;
        entities[n].id %= entities_n;
    }
    update(false);
}

void Level :: setEntity(int n, int set){
    if(n < entities.size()){
        entities[n].id = set % entities_n;
    }
    update(false);
}

int Level :: getEntity(int x, int y){
    int res = 0;
    for(std::vector<Living>::iterator i = entities.begin(); i != entities.end(); i++){
        if(i->x < x && x < i->x + i->width)
            if(i->y < y && y < i->y + i->height)
                return res;
        res++;
    }
    return -1;
}

void Level :: addEntity(int x, int y){
    int temp_x = x;
    int temp_y = y;
    int temp_id = 1;
    entities.push_back(Living(temp_x, temp_y, temp_id));
    update(false);
}

void Level :: deleteEntity(int n){
    entities.erase(entities.begin() + n);
}
