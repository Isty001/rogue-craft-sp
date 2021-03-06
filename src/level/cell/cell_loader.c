#include <list.h>
#include "memory/memory.h"
#include "config.h"
#include "util/json.h"


static List *PROTOTYPES;


static void extend(Cell *cell, JSON_Object *json, const Cell *parent)
{
    json_get_optional_wchar(&cell->chr, json, "char", parent->chr);
    cell->style = json_get_optional_style(json, parent->style);
    cell->type = (CellType) json_get_optional_constant(json, "type", parent->type);
    cell->material = (Material) json_get_optional_constant(json, "material", parent->material);
}

static void build_new(Cell *cell, JSON_Object *json)
{
    json_get_wchar(&cell->chr, json, "char");
    cell->style = json_get_style(json);
    cell->type = (CellType) constant(json_get_string(json, "type"));
    cell->material = (Material) constant(json_get_string(json, "material"));
}

static void finalize(JSON_Object *json, CellPrototype *prototype, Cell *cell)
{
    cell->in_registry = true;
    cell->state = MATERIAL_STRENGTH[cell->material];
    prototype->cell = *cell;

    char *name = json_get_string(json, "name");
    sprintf(prototype->name, "%s", name);
}

static void build_cell(JSON_Object *json)
{
    Cell building;
    const Cell *parent;
    memset(&building, 0, sizeof(Cell));

    CellPrototype *with_name = mem_calloc(1, sizeof(CellPrototype));

    if (json_has_string(json, "extends")) {
        parent = cell_registry_get(json_get_string(json, "extends"));
        extend(&building, json, parent);
    } else {
        build_new(&building, json);
    }
    finalize(json, with_name, &building);

    PROTOTYPES->append(PROTOTYPES, with_name);
}

const Cell *cell_registry_get(char *name)
{
    CellPrototype *found = PROTOTYPES->find(PROTOTYPES, (Predicate) function(bool, (CellPrototype * cell) {
        return 0 == strcmp(name, cell->name);
    }));

    if (!found) {
        fatal("Cant find Cell [%s]", name);
    }

    return &found->cell;
}

void cell_registry_load(void)
{
    PROTOTYPES = list_new();
    PROTOTYPES->release_item = mem_dealloc;

    if (CE_LOADED != cell_cache_load(PROTOTYPES)) {
        json_parse_in_dir(env_json_resource_path(RESOURCE_CELLS), build_cell);
        cell_cache_save(PROTOTYPES);
    }
}

void cell_registry_unload(void)
{
    PROTOTYPES->free(PROTOTYPES);
}
