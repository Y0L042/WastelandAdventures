#include "collision.h"



CollisionData* coll_component_create(
        ecs_world_t *world, 
        ecs_entity_t entity,
        coll_bits_t coll_bits
    )
{
    CollisionData *c_d = (CollisionData *)malloc(sizeof(CollisionData));
    c_d->coll_bits = coll_bits;
    ecs_set(world, entity, CollisionComponent, { .c_d = c_d });

    return c_d;
}



void collisiondata_initialize(CollisionData *c_d)
{
    c_d->coll_bits = 0;
}



int collisiondata_check_collision(
        CollisionData *ent_A_mask, 
        CollisionData *ent_B_layer
    )
{
    coll_bits_t mask = ent_A_mask->coll_bits;
    coll_bits_t layer = ent_B_layer->coll_bits;

    return (mask & layer);
}

coll_bits_t coll_bits_remove_layer(coll_bits_t layer_a, coll_bits_t layer_b)
{
    return (layer_a & ~layer_b);
}

coll_bits_t coll_bits_add_layer(coll_bits_t layer_a, coll_bits_t layer_b)
{
    return (layer_a | layer_b);
}


