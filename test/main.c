#include <stdlib.h>
#include <time.h>
#include "../src/color.h"
#include "../src/player/item.h"

void run_player_movement_test(void);
void run_point_test(void);
void run_util_test(void);
void run_player_manager_test(void);
void run_camera_test(void);
void run_consumable_test(void);
void run_buffer_test(void);
void run_inventory_test(void);
void run_level_interaction_test(void);
void run_cell_test(void);
void run_player_hit_test(void);


static void init(void)
{
    srand((unsigned int) time(NULL));

    item_pool_init();
    cell_pool_init();
}

static void cleanup(void)
{
    item_pool_cleanup();
    cell_pool_cleanup();
}

int main(void)
{
    init();

    run_player_movement_test();
    run_point_test();
    run_util_test();
    run_player_manager_test();
    run_camera_test();
    run_consumable_test();
    run_buffer_test();
    run_inventory_test();
    run_level_interaction_test();
    run_cell_test();
    run_player_hit_test();

    cleanup();

    return 0;
}
