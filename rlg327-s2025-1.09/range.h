#ifndef RANGE_H
#define RANGE_H

#include <stdint.h>
#include <cstdlib>
#include <cmath>
#include "pc.h"
#include "npc.h"
#include "dungeon.h"
#include "object.h"
#include "move.h"
#include "event.h"
#include "character.h"
#include "io.h"
#include "ncurses.h"
#include "stdlib.h"


// Perform a ranged attack on a target location
void perform_ranged_attack(dungeon *d, pair_t target);

// helper function
void draw_poison_cloud(dungeon *d, pair_t center, int radius);

// Cast poison ball spell at a target location
void cast_poison_ball(dungeon *d, pair_t target);

#endif // RANGE_H
