#ifndef ROGUECRAFT_PLAYER_H
#define ROGUECRAFT_PLAYER_H


#include <stdint.h>
#include "../level/point.h"
#include "../level/level.h"


typedef int Input;

typedef enum {
    HEALTH,
    STAMINA,
    HUNGER,
    THIRST
} AttributeType;

typedef struct {
    uint16_t limit;
    uint16_t current;
} Attribute;

typedef struct player {
    Level *level;
    uint32_t eyesight;
    struct {
        Point current;
        Point previous;
    } position;
    struct {
        Cell prototype;
        Cell *occupied;
    } cell;
    struct {
        Attribute hp;
        Attribute stamina;
        Attribute hunger;
        Attribute thirst;
        Attribute *type_map[4];
    } attr;
} Player;


Player *player_new(Level *level);

void player_free(Player *player);

void player_move(Player *player, Input input);

bool player_can_see(Player *player, uint16_t y, uint16_t x);

void player_position_on_level(Player *player);


#endif
