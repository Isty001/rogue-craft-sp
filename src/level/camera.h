#ifndef ROGUECRAFT_CAMERA_H
#define ROGUECRAFT_CAMERA_H


#include "../player/config.h"


typedef struct Camera {
    Point position;
    Size size;
} Camera;


void camera_update(Player *player, WINDOW *window);

Point camera_to_level_point(Camera *camera, Point level_point);


#endif
