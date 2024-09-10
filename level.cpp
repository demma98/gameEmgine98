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
    for(std::list<Living * >::iterator e = entities.begin(); e != entities.end(); e++){
        if(update_entities)
            switch((*e)->id){
                case 0:
                    break;

                case 1:
                    if((*e)->distanceDown(*tables.begin()) == 0.0){
                        if((*e)->tempBool){
                            if((*e)->speed_x < 1.1)
                                (*e)->speed_x += 0.1;
                        }
                        else{
                            if((*e)->speed_x > 1.1)
                                (*e)->speed_x -= 0.1;
                        }
                        if(!(*e)->nextDiagonalBlock(*tables.begin(), (*e)->tempBool, true))
                            (*e)->tempBool = !(*e)->tempBool;
                    }
                    break;
            }

            //gravity
            if((*e)->falls){
                if((*e)->distanceDown(*tables.begin()) == 0.0)
                    (*e)->coyote_time = -1;
                else{
                    if((*e)->coyote_time == -1 && (*e)->speed_y == 0.0)
                        (*e)->coyote_time = 5;
                    else if((*e)->coyote_time > 0)
                        (*e)->coyote_time -= 1;
                    else
                        if((*e)->speed_y < 2.5){
                            (*e)->speed_y += 0.8;
                            if((*e)->speed_y > 2.5)
                                (*e)->speed_y = 2.5;
                        }
                }
            }
            (*e)->update(*tables.begin());
        }
}

void Level :: render(SDL_Renderer *renderer){
    for(std::list<Living *>::iterator e = entities.begin(); e != entities.end(); e++){
        (*e)->render(renderer, textures[(*e)->id], tables.begin()->off_x, tables.begin()->off_y);
    }
    for(std::list<Table>::iterator t = tables.begin();t != tables.end(); t++)
        t->render(renderer);
}

void Level :: update(){
    update(true);
}

void Level :: destroy(){
    for(std::list<Table>::iterator t = tables.begin();t != tables.end(); t++)
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

            addEntityId(temp_x, temp_y, temp_id);
        }
    }

    fclose(f);
}

void Level :: save(){
    FILE *f = fopen(entities_path, "w");

    if(f != NULL){
        Disk().saveNumber(f, entities.size());

        for(std::list<Living *>::iterator i = entities.begin(); i != entities.end(); i++){
            Disk().saveNumber(f, (*i)->x / Block().width);
            Disk().saveNumber(f, (*i)->y / Block().height);
            Disk().saveNumber(f, (*i)->id);
        }
    }

    for(std::list<Table>::iterator t = tables.begin();t != tables.end(); t++)
        t->save();

    fclose(f);
}

void Level :: setTable(int num, int max_block, const char *path, SDL_Renderer *renderer, const char *sheet_path){
    tables.push_back(Table(renderer, max_block, path, sheet_path));
}


void Level :: changeEntity(int n, int change){
    if(n < entities.size()){
        std::list<Living *>::iterator l = entities.begin();
        for(int i = 0; l != entities.end() && i != n; i++, l++);
        (*l)->id += change;
        (*l)->id %= entities_n;
    }
    update(false);
}

void Level :: setEntity(int n, int set){
    if(n < entities.size()){
        std::list<Living *>::iterator l = entities.begin();
        for(int i = 0; l != entities.end() && i != n; i++, l++);
        (*l)->id = set % entities_n;
    }
    update(false);
}

int Level :: getEntity(int x, int y){
    int res = 0;
    for(std::list<Living *>::iterator i = entities.begin(); i != entities.end(); i++){
        if((*i)->x < x && x < (*i)->x + (*i)->width)
            if((*i)->y < y && y < (*i)->y +(*i)->height)
                return res;
        res++;
    }
    return -1;
}

void Level :: addEntityId(int x, int y, int id){
    int temp_x = x;
    int temp_y = y;
    int temp_id = id;

    switch(temp_id){
        case 0:
        entities.push_back(new Player(temp_x, temp_y, temp_id));
            break;
    }
    update(false);
}

void Level :: addEntity(int x, int y){
    addEntityId(x, y, 1);
}

void Level :: deleteEntity(int n){
    std::list<Living *>::iterator l = entities.begin();
    for(int i = 0; l != entities.end() && i != n; i++, l++);
    entities.erase(l);
}
