day 1:
        started project
        created the following functions:
        - create_room
        - if_valid_room
        - create_corridor
        - create_upstair
        - create_downstair
        - create_dungeon
        - print_dungeon
        - connect_rooms

        The bugs that I'm currently aware of and have fixed are:
        - making sure that corridors fully connect to each room; corridor would stop building at the final destination coordinate
        - making sure that rooms and corridors don't overlap the dungeon border
        - don't let rooms overlap
        - made it so that corridors don't build into rooms

week 2:
	2/11:
	Started working on the save and load functions
	Adding Player Character
	Wrote structs for holding the Dungeon, Locations of Stairs and Player Character, Nodes of Rooms, Stairs, and Tiles. Linked Lists for Rooms and Stairs.
	2/12:
	Finished Load Function and load_dungeon function
	Didn't manage to finish Save function in time;

	-- Things I'm now aware of:
	In order to make save function, I need to rewrite the structure of my dungeon generator so that I can put it inside of a struct Dungeon rather than char **dungeon.
	Save function is not functional.
	Stairs don't load for some reason; after file reading, I might be off by a byte or two, because it doesn't count correctly.


week 3/4:
    2/17/2025:
        i realized that the structure of my dungeon needed to be changed so I took ideas from sheaffers code.
        I am now using his code because I believe it was significantly more structured than mine and it would've been difficult to do from scratch.

    2/25/2025:
        i re-wrote sheaffers dijkstras algorithm for non-tunneler enemies and rendered the distance from the PC to every other tile

    2/26/2025:
        finished writing the other algorithm for the tunneler enemy

week 5:
    3/4/2025:
        I added monsters to the dungeon and monster structs and print them out into the dungeon
    3/5/2025:
        I made the movement functions for the player who moves around randomly in a valid location
        I made a 'line of sight' function
        Movement for stupid monsters -- randomly moves in a valid location; if tunneler, then can also randomly dig at a wall
        Movement for smart monsters -- randomly moves unless they've seen the PC or have had seen them before
        Movement for telepathic -- they just try shortest euclidian absolute distance as if there was no wall between them -- if they're also intelligent, basically dijkstra
        Movement for tunnelers -- dig at walls; but if they're also telepathic; dig wall straight at the PC;
        If all the above; they basically can find the shortest path to you from the perspective of a tunneler

        added switch --nummon (int) that spawns the number of monsters in the dungeon

week 6:
    3/26/2025:
        The player character can now be controlled manually and also go up and down the stairs.
        - As far I know, there's no bugs


week 7:
    4/2/2025:
        ADDED C++ SUPPORT
        I have fog of war now

week 8:
    4/9/2025:
        I have a parsable text file that somebody can edit now to add new enemies
        I added more complex enemies

week 9:
    4/16/2025:
        I added the unique monsters from the parsable text file as monster instances that can now be encountered in the dungeon.
        I am currently trying to add artifact objects to be placed within the dungeon as well.

week 10:
    4/23/2025:
        I added object parsing to the dungeon and having the character be able to equip objects like weapons and armor.
        The character should also be able to drop and pick up things from the dungeon.
    
week 11/12:
    5/9/2025:
        I added ranged combat.
        New weapon types added to the dungeon; ranged weaponry now available.
        The player can now cast spells (just poison for now)
