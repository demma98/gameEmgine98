#include "table.h"


Table :: Table (SDL_Renderer *renderer, int max_block, const char *path, const char *sheet_path){
    this -> max_block = max_block;
    this -> path = path;
    this -> sheet_path = sheet_path;

    load();
    update(renderer);
    off_x = 0;
    off_y = 0;
}


void Table :: load(){
    FILE *f;
    int temp_x = 0;
    int temp_y = 0;
    
    f = fopen(path, "r");
    if (f != NULL){
        width = Disk().loadNumber(f);
        height = Disk().loadNumber(f);
        
        blocks =(int *) malloc(sizeof(int[width][height]));

        while (height > temp_y){
            if(temp_x == width){
                temp_x = 0;
                temp_y++;
            }
            else{
                *(blocks + temp_x + (temp_y * width)) = Disk().loadNumber(f);
                temp_x++;
            }
        }

        fclose(f);
    }
}

void Table :: save(){
    FILE *f;
    int temp_x = 0;
    int temp_y = 0;

    f = fopen(path, "w");
    if (f != NULL){
        Disk().saveNumber(f, width);
        Disk().saveNumber(f, height);
        while (height > temp_y){
            if(temp_x == width){
                temp_x = 0;
                temp_y++;
            }
            else{
                if(0 <= *(blocks + temp_x + (temp_y * width)) && *(blocks + temp_x + (temp_y * width)) < max_block)
                    Disk().saveNumber(f, *(blocks + temp_x + (temp_y * width)));
                else
                    Disk().saveNumber(f, 1);
                temp_x++;
            }
        }

        fclose(f);
    }
}

void Table :: update(SDL_Renderer *renderer){
    image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width * Block().width, height * Block().height);
    SDL_SetTextureBlendMode(image, SDL_BLENDMODE_BLEND);
    
    SDL_SetRenderTarget(renderer, image);

    for(int j = 0; j < height; j++)
        for(int i = 0; i < width; i++)
            Block().drawBlock(renderer, i, j, *(blocks + i + (j * width)), sheet_path);
    
    SDL_SetRenderTarget(renderer, NULL);
}

void Table :: render(SDL_Renderer *renderer){
    SDL_Rect r;
    r.x = off_x;
    r.y = off_y;
    SDL_QueryTexture(image, NULL, NULL, &r.w, &r.h);
    SDL_RenderCopy(renderer, image, NULL, &r);
}

void Table :: destroy(){
    free(blocks);
}


void Table :: addRows(SDL_Renderer *renderer, int n){
    height += n;
    save();
    free(blocks);
    load();
    update(renderer);
}

void Table :: addColumns(SDL_Renderer *renderer, int n){
    width += n;
    save();
    free(blocks);
    load();
    update(renderer);
}

void Table :: changeBlock(SDL_Renderer *renderer, int x, int y, int n, bool save_change){
    if(x < width && y < height){
        *(blocks + x + (y * width)) += n;
        *(blocks + x + (y * width)) %= max_block;
        SDL_SetRenderTarget(renderer, image);
        Block().drawBlock(renderer, x, y, *(blocks + x + (y * width)), sheet_path);
        SDL_SetRenderTarget(renderer, NULL);
        if(save_change)
            save();
    }
}

void Table :: setBlock(SDL_Renderer *renderer, int x, int y, int n, bool save_change){
    if(x < width && y < height){
        *(blocks + x + (y * width)) = n % max_block;
        SDL_SetRenderTarget(renderer, image);
        if(n != 0)
            Block().drawBlock(renderer, x, y, *(blocks + x + (y * width)), sheet_path);
        else{
            SDL_SetRenderDrawColor(renderer, 0, 0, 0 ,0);
            SDL_Rect temp;
            temp.x = x * Block().width;
            temp.y = y * Block().height;
            temp.w = Block().width;
            temp.h = Block().height;
            SDL_RenderFillRect(renderer, &temp);
        }
        SDL_SetRenderTarget(renderer, NULL);
        if(save_change)
            save();
    }
}


void Table :: flipVertical(SDL_Renderer *renderer, bool save_change){
    int temp, temp_i;
    for(int j = 0; j < height; ++j)
        for(int i = 0; i < width / 2; ++i){
            temp_i = width - i - 1;
            temp = *(blocks + i + (j * width));
            *(blocks + i + (j * width)) = *(blocks + temp_i + (j * width));
            *(blocks + temp_i + (j * width)) = temp;
        }
    update(renderer);
    if(save_change)
        save();
}

void Table :: flipHorizontal(SDL_Renderer *renderer, bool save_change){
    int temp, temp_j;
    for(int j = 0; j < height / 2; ++j)
        for(int i = 0; i < width; ++i){
            temp_j = height - j - 1;
            temp = *(blocks + i + (j * width));
            *(blocks + i + (j * width)) = *(blocks + i + (temp_j * width));
            *(blocks + i + (temp_j * width)) = temp;
        }
    update(renderer);
    if(save_change)
        save();
}