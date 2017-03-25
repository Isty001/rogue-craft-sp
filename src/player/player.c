#include "player.h"
#include "inventory.h"


#define CHAR_BAR '|'


static void add_default_attributes(Player *player)
{
    Attribute *map = player->attributes.state;

    map[HEALTH] = (Attribute) {
        .current = 100, .max = 100,
        .name = "Health", .style  = COLOR_PAIR(COLOR_PAIR_RED_F)
    };

    map[STAMINA] = (Attribute) {
        .current = 100, .max = 100,
        .name = "Stamina", .style = COLOR_PAIR(COLOR_PAIR_GREEN_F)
    };

    map[HUNGER] = (Attribute) {
        .current = 0, .max = 100,
        .name = "Hunger", .style = COLOR_PAIR(COLOR_PAIR_YELLOW_F)
    };

    map[THIRST] = (Attribute) {
        .current = 0, .max = 100,
        .name = "Thirst", .style = COLOR_PAIR(COLOR_PAIR_BLUE_F)
    };
}

static void find_starting_point(Player *player)
{
    Level *level = player->level;
    Point point = level_rand_hollow(level);

    player->cell.previous = level->cells[point.y][point.x];
    player->position.previous = point;
    player->position.current = point;
}

Player *player_new(Level *level, Camera *camera)
{
    Player *player = allocate(sizeof(Player));
    player->level = level;
    player->cell.prototype.type = PLAYER;
    player->camera = camera;

    add_default_attributes(player);

    player->cell.prototype.style = COLOR_PAIR(COLOR_PAIR_RED_F);
    player->cell.prototype.chr = PLAYER_CHAR;

    find_starting_point(player);

    player->inventory = inventory_new(PLAYER_DEFAULT_INVENTORY_SIZE);
    player->sight = sight_new(level, player->position.current, PLAYER_DEFAULT_EYESIGHT, ALL);

    return player;
}

void player_free(Player *player)
{
    inventory_free(player->inventory);
    sight_free(player->sight);
    release(player);
}

static inline void display_attribute_bar(int width, Attribute *attr, WINDOW *win)
{
    int bar_width = width * ((double) attr->current / attr->max);

    for (int i = 0; i < width; i++) {
        if (i < bar_width) {
            styled(win, attr->style,
                   waddch(win, CHAR_BAR);
            );
        } else {
            waddch(win, ' ');
        }
    }
}

void player_attributes_display(Player *player)
{
    Attribute *attr;
    WINDOW *win = WINDOW_PLAYER_ATTRIBUTES;
    int width = getmaxx(win) - 2 * PADDING;
    int line = 0;
    wclear(win);

    for (int i = 0; i < PLAYER_ATTR_NUM; i++) {
        attr = &player->attributes.state[i];

        styled(win, attr->style,
               mvwprintw(win, ++line, PADDING, attr->name);
        );
        wprintw(win, ": %d/%d", attr->current, attr->max);
        wmove(win, ++line, PADDING);

        display_attribute_bar(width, attr, win);
        line++;
    }

    refresh_boxed(win);
}
