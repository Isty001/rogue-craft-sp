#include <sfx/sfx.h>
#include "config.h"


static bool PLAY_STEP = false;


void player_position_on_level(Player *player)
{
    Cell ***cells = player->level->cells;
    Point current = player->position.current;
    Point previous = player->position.previous;

    cells[previous.y][previous.x] = player->cell.occupied;

    player->cell.occupied = cells[current.y][current.x];
    cells[current.y][current.x] = &player->cell.prototype;
}

static inline bool can_move_to(Player *player, Point target)
{
    Level *level = player->level;

    return
        level_in_bounds(level, target)
        &&
        SOLID != level->cells[target.y][target.x]->type;
}

EventError player_move(InputEvent *event)
{
    Direction direction;

    if (KEY_MOUSE == event->input || 0 == (direction = direction_lookup(event->input))) {
        return EE_CONTINUE;
    }

    Player *player = event->player;
    Point target = point_move(player->position.current, direction, 1);

    if (can_move_to(player, target)) {
        player->position.previous = player->position.current;
        player->position.current = target;
        player->state.modifiers.traveled++;

        if (PLAY_STEP) {
            sfx_play_step(player->level->cells[target.y][target.x]);
        }
        PLAY_STEP = !PLAY_STEP;

        player_position_on_level(player);
    }

    return EE_BREAK;
}
