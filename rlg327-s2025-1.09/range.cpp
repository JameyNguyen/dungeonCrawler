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

#include <unistd.h>
#include <ctype.h>
#include <cstring>
  
  // Ranged attack function
  // MAYBE USE AN ENUM TO DIFFERENTIATE MELEE / RANGED?
  /*
  Inside object could be typedef enum { WEP_TYPE_MELEE, WEP_TYPE_RANGED } weapon_type_t;
  */
 void perform_ranged_attack(dungeon *d, pair_t target) {
  pc *pc = d->PC;

  const char *organs[] = {
    "liver", "pancreas", "heart", "brain", "eye", "arm", "leg", "intestines",
    "gall bladder", "lungs", "hand", "foot", "spinal cord", "pituitary gland",
    "thyroid", "tongue", "bladder", "diaphram", "frontal lobe", "hippocampus",
    "stomach", "pharynx", "esophagus", "trachea", "urethra", "spleen",
    "cerebellum", "ganglia", "ear", "subcutaneous tissue", "prefrontal cortex"
  };
  const char *attacks[] = {
    "shoots", "snipes", "fires at", "riddles", "pierces", "targets", "nails",
    "tags", "clips", "zaps", "pelts", "blasts", "bombards", "plinks", "darts"
  };

  if (!pc->eq[eq_slot_ranged] || pc->eq[eq_slot_ranged]->get_type() != objtype_RANGED) {
    mvprintw(0, 0, "No ranged weapon equipped! Press any key to continue.");
    refresh();
    getch();
    io_display(d);
    return;
  }

  character *target_char = d->character_map[target[dim_y]][target[dim_x]];
  if (!target_char || target_char == pc || !target_char->alive) {
    mvprintw(0, 0, "No valid target at that location! Press any key to continue.");
    refresh();
    getch();
    io_display(d);
    return;
  }

  if (!can_see(d, pc->position, target, 1, 0)) {
    mvprintw(0, 0, "Target not in line of sight! Press any key to continue.");
    refresh();
    getch();
    io_display(d);
    return;
  }

  int damage = pc->eq[eq_slot_ranged]->roll_dice();
  const char *organ = organs[rand() % (sizeof(organs) / sizeof(organs[0]))];
  const char *attack = attacks[rand() % (sizeof(attacks) / sizeof(attacks[0]))];

  io_queue_message("You %s %s%s in the %s for %d damage!",
                   attack,
                   is_unique(target_char) ? "" : "the ",
                   target_char->name,
                   organ,
                   damage);

  if (damage >= static_cast<int>(target_char->hp)) {
    io_queue_message("%s%s dies.", is_unique(target_char) ? "" : "The ", target_char->name);
    
    target_char->hp = 0;
    target_char->alive = 0;

    character_increment_dkills(pc);
    character_increment_ikills(pc,
      character_get_dkills(target_char) + character_get_ikills(target_char));

    if (target_char != d->PC) {
      d->num_monsters--;
    }

    charpair(target_char->position) = NULL;
  } else {
    target_char->hp -= damage;
  }

  io_display(d);
}

void draw_poison_cloud(dungeon *d, pair_t center, int radius) {
  for (int step = 1; step <= radius; step++) {
    for (int y = center[dim_y] - step; y <= center[dim_y] + step; y++) {
      for (int x = center[dim_x] - step; x <= center[dim_x] + step; x++) {
        if (y < 0 || y >= DUNGEON_Y || x < 0 || x >= DUNGEON_X) continue;

        int dist = abs(x - center[dim_x]) + abs(y - center[dim_y]);
        if (dist == step) {
          // Temporarily draw poison gas
          mvaddch(y + 1, x, '~' | COLOR_PAIR(COLOR_GREEN));
        }
      }
    }

    refresh();      // Show update
    usleep(100000); // Wait 100ms per step
  }

  io_display(d);  // Redraw the map after animation
}


  // Poison ball spell function
  void cast_poison_ball(dungeon *d, pair_t target) {
    pc *pc = d->PC;
  
    character *target_char = d->character_map[target[dim_y]][target[dim_x]];
    if (!target_char || target_char == pc || !target_char->alive) {
      mvprintw(0, 0, "No valid target at that location! Press any key to continue.");
      refresh();
      getch();
      io_display(d);
      return;
    }
  
    if (!can_see(d, pc->position, target, 1, 0)) {
      mvprintw(0, 0, "Target not in line of sight! Press any key to continue.");
      refresh();
      getch();
      io_display(d);
      return;
    }
  
    const int radius = 3;
    int damage = 0;
    for (int i = 0; i < num_eq_slots; i++) {
      if (i == eq_slot_weapon && !pc->eq[i]) {
        damage += pc->damage->roll(); // fallback to base unarmed damage
      } else if (pc->eq[i]) {
        damage += pc->eq[i]->roll_dice();
      }
    }

    draw_poison_cloud(d, target, radius);
  
    const char *organ = "lungs";  // Flavorful static organ
    const char *attack = "poisons";
  
    for (int y = target[dim_y] - radius; y <= target[dim_y] + radius; y++) {
      for (int x = target[dim_x] - radius; x <= target[dim_x] + radius; x++) {
        if (y < 0 || y >= DUNGEON_Y || x < 0 || x >= DUNGEON_X) continue;
  
        int dist = abs(x - target[dim_x]) + abs(y - target[dim_y]);  // Manhattan radius
        if (dist <= radius) {
          character *monster = d->character_map[y][x];
  
          if (monster && monster != pc && monster->alive) {
            io_queue_message("You %s %s%s's %s for %d damage!",
                             attack,
                             is_unique(monster) ? "" : "the ",
                             monster->name,
                             organ,
                             damage);
  
            if (damage >= static_cast<int>(monster->hp)) {
              io_queue_message("%s%s dies from the poison cloud.", is_unique(monster) ? "" : "The ", monster->name);
  
              monster->hp = 0;
              monster->alive = 0;
  
              character_increment_dkills(pc);
              character_increment_ikills(pc,
                character_get_dkills(monster) + character_get_ikills(monster));
  
              d->num_monsters--;
              charpair(monster->position) = NULL;
            } else {
              monster->hp -= damage;
            }
          }
        }
      }
    }
  
    io_display(d);
  }
  