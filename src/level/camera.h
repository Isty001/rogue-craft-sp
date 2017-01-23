#ifndef ROGUECRAFT_CAMERA_H
#define ROGUECRAFT_CAMERA_H


#include "../player/player.h"


typedef struct camera {
    Point position;
    Size size;
} Camera;


void camera_update(Camera *camera, Player *player, WINDOW *window);

Point camera_adjust_level_point(Camera *camera, Point level_point);


#endif
