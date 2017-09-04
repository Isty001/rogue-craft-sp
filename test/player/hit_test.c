#include <level/level.h>
#include "../unit_test.h"
#include "../fixture.h"
#include "../../src/player/inventory/inventory.h"


static ItemPrototype TEST_TOOL = {
    .item = {
        .type = TOOL,
        .tool = {
            .range = 1,
            .multipliers = {
                .defaults = {.solid = 1.1}
            }
        }
    }
};


static Inventory *create_inventory(void)
{
    Inventory *inv = inventory_new(2);
    inv->items->append(inv->items, item_clone(&TEST_TOOL));

    return inv;
}

static Player create_player(void)
{
    return (Player) {
        .level = fixture_level(),
        .inventory = create_inventory(),
        .position = {.current = point_new(1, 0), .previous = point_new(1000, 1000)},
        .state.map = {[STAMINA] = {.current = 80}}
    };
}

static LevelInteractionEvent create_event(Player *player, Point point)
{
    return (LevelInteractionEvent) {
        .player = player,
        .cell = player->level->cells[point.y][point.x],
        .point =point
    };
}

static void free_fixtures(Player *player)
{
    inventory_free(player->inventory);
    fixture_level_free(player->level);
}

MU_TEST(test_range)
{
    Player player = create_player();
    List *items = player.inventory->items;
    LevelInteractionEvent event = create_event(&player, point_new(0, 2));

    player_hit(&event);

    Item *tool = items->head(items);
    tool->tool.range = 2;

    player_hit(&event);
    mu_assert_int_eq(94, player.level->cells[0][2]->state);
    mu_assert_int_eq(6, player.state.modifiers.dealt_damage);

    free_fixtures(&player);
}

MU_TEST(test_defaults)
{
    Player player = create_player();
    Cell *cell;

    LevelInteractionEvent event = create_event(&player, point_new(0, 0));

    player_hit(&event);

    cell = player.level->cells[0][0];
    mu_assert(false == cell->in_registry, "The cell hit should be cloned");

    mu_assert_int_eq(94, cell->state);
    mu_assert_int_eq(6, player.state.modifiers.dealt_damage);

    free_fixtures(&player);
}

MU_TEST(test_material_and_tired_damage)
{
    Player player = create_player();
    List *items = player.inventory->items;
    Item *item;

    player.state.map[HUNGER].current = 20;
    player.state.map[THIRST].current = 30;
    item = items->head(items);
    item->tool.multipliers.materials[STONE] = 2;

    LevelInteractionEvent event = create_event(&player, point_new(0, 0));

    player_hit(&event);

    mu_assert_int_eq(93, player.level->cells[0][0]->state);
    mu_assert_int_eq(7, player.state.modifiers.dealt_damage);

    free_fixtures(&player);
}

MU_TEST(test_bare_hands)
{
    Player player = create_player();
    LevelInteractionEvent event = create_event(&player, point_new(0, 0));
    player.inventory->selected = 1;

    player_hit(&event);

    mu_assert_int_eq(95, player.level->cells[0][0]->state);
    mu_assert_int_eq(5, player.state.modifiers.dealt_damage);

    free_fixtures(&player);
}

MU_TEST(test_invalid_cell_type)
{
    Player player = create_player();
    LevelInteractionEvent event = create_event(&player, point_new(0, 0));
    Cell *cell = player.level->cells[0][0];
    cell->type = HOLLOW;

    player_hit(&event);
    mu_assert(cell->in_registry, "Shouldn't be touched");

    free_fixtures(&player);
}

MU_TEST(test_tool_wear_out)
{
    Player player = create_player();
    List *items = player.inventory->items;
    Item *item = items->get(items, 0);
    item->value = 10;
    item->tool.multipliers.materials[STONE] = 1.5;

    LevelInteractionEvent event = create_event(&player, point_new(0, 0));
    repeat(50, player_hit(&event))

    mu_assert_int_eq(0, items->count);

    free_fixtures(&player);
}

void run_player_hit_test(void)
{
    TEST_NAME("Hit");

    MU_RUN_TEST(test_range);
    MU_RUN_TEST(test_defaults);
    MU_RUN_TEST(test_material_and_tired_damage);
    MU_RUN_TEST(test_bare_hands);
    MU_RUN_TEST(test_invalid_cell_type);
    MU_RUN_TEST(test_tool_wear_out);

    MU_REPORT();
}
