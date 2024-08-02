#ifndef COLLISION_H
#define COLLISION_H

#include "flecs.h"
#include "components.h"

typedef int coll_bits_t;

// "I exist and can be collided with."
// This is the layers where the entity exist.
// If Entity_A does not have a CollisionLayer, then Entity_B cannot \
// collide with Entity_A. If Entity_B does not have a COllisionMask, 
// it also cannot collide with Entity_A.
// "I look at others, and can collide against them."
// This is the layers that the entity will scan for collisions.

typedef struct CollisionData {
    coll_bits_t coll_bits;
} CollisionData; 

typedef struct CollisionComponent {
    CollisionData *c_d;
} CollisionComponent, CollisionLayer, CollisionMask;



CollisionData* coll_component_create(
        ecs_world_t *world, 
        ecs_entity_t entity,
        coll_bits_t coll_bits
    );



void collisiondata_initialize(CollisionData *c_d);
int collisiondata_check_collision(
        CollisionData *ent_A_mask, 
        CollisionData *ent_B_layer
    ); 



#endif // COLLISION_H
