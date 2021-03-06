#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <parson.h>
#include <unistd.h>
#include "util/logger.h"
#include "util/timer.h"
#include "memory/memory.h"
#include "util/color.h"
#include "item/item.h"


void run_player_movement_test(void);
void run_point_test(void);
void run_util_test(void);
void run_player_manager_test(void);
void run_camera_test(void);
void run_level_test(void);
void run_consumable_test(void);
void run_inventory_test(void);
void run_level_interaction_test(void);
void run_cell_test(void);
void run_player_hit_test(void);
void run_player_state_test(void);
void run_json_test(void);
void run_item_loader_test(void);
void run_storage_test(void);
void run_cache_test(void);
void run_cell_loader_test(void);
void run_level_loader_test(void);
void run_color_cache_test(void);
void run_grid_test(void);
void run_lighting_test(void);
void run_logger_test(void);


static void init(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    srand((unsigned int) time(NULL));
    json_set_allocation_functions((JSON_Malloc_Function) mem_alloc, mem_dealloc);

    fclose(fopen(getenv(ENV_LOG_FILE), "w"));
    log_init();
    profiler_init();
    timer_init();
    list_node_pool_init();
    item_pool_init();
    cell_pool_init();
}

static void cleanup(void)
{
    timer_cleanup();
    item_pool_cleanup();
    cell_pool_cleanup();
    list_node_pool_cleanup();
    profiler_cleanup();
    log_cleanup();

    unlink(getenv(ENV_LOG_FILE));
}

int main(int argc, char *argv[])
{
    env_setup(argc, argv);

    init();

    run_logger_test();
    run_player_movement_test();
    run_point_test();
    run_util_test();
    run_player_manager_test();
    run_camera_test();
    run_level_test();
    run_consumable_test();
    run_level_interaction_test();
    run_cell_test();
    run_player_hit_test();
    run_inventory_test();
    run_player_state_test();
    run_json_test();
    run_item_loader_test();
    run_storage_test();
    run_cache_test();
    run_cell_loader_test();
    run_level_loader_test();
    run_color_cache_test();
    run_grid_test();
    run_lighting_test();

    cleanup();
}
