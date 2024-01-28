#include <stdio.h>

#define DISK_GUARD 1

class Disk{
    public:
        int loadNumber(FILE *f);
        void saveNumber(FILE *f, int num);
};