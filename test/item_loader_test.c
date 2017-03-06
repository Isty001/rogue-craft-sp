#include "unit_test.h"
#include "../src/player/item.h"
#include "../config/config.h"


static void assert_tool(ItemPrototype *prototype)
{
    Item *item = &prototype->item;
    Tool *tool = &item->tool;

    assert_string("Pickaxe", item->name);
    assert_range(range_new(1, 100), prototype->value_range);
    mu_assert_int_eq(TOOL, prototype->item.type);
    mu_assert_int_eq(COLOR_PAIR(COLOR_PAIR_GRAY_F), prototype->item.style);
//    mu_assert_int_eq('p', prototype->items.chr);

    mu_assert_int_eq(1, tool->range);
    mu_assert_double_eq(1.5, tool->multipliers.defaults.solid);
    mu_assert_double_eq(1.0, tool->multipliers.defaults.creature);

    mu_assert_double_eq(1.4, tool->multipliers.materials[STONE]);
    mu_assert_double_eq(1.1, tool->multipliers.materials[DIRT]);
}

static void assert_consumable(ItemPrototype *prototype)
{
    Item *item = &prototype->item;
    Consumable *consumable = &item->consumable;

    assert_string("Potion", item->name);
    assert_range(range_new(1, 30), prototype->value_range);
    mu_assert_int_eq(CONSUMABLE, prototype->item.type);
    mu_assert_int_eq(COLOR_PAIR(COLOR_PAIR_RED_F) | A_BOLD | A_UNDERLINE, prototype->item.style);

    mu_assert(consumable->permanent, "should be permanent");
    mu_assert_int_eq(STAMINA, consumable->attribute);
}

MU_TEST(test_load)
{
    item_load(FIXTURE_DIR"/config/items");

    mu_assert_int_eq(1, ITEM_CONSUMABLE_PROBABILITY.count);
    mu_assert_int_eq(10, ITEM_CONSUMABLE_PROBABILITY.sum);

    assert_consumable(ITEM_CONSUMABLE_PROBABILITY.items[0].value);

    mu_assert_int_eq(1, ITEM_TOOL_PROBABILITY.count);
    mu_assert_int_eq(15, ITEM_TOOL_PROBABILITY.sum);

    assert_tool(ITEM_TOOL_PROBABILITY.items[0].value);

    item_unload();
}

void run_item_loader_test(void)
{
    TEST_NAME("Item loader");

    MU_RUN_TEST(test_load);

    MU_REPORT();
}
