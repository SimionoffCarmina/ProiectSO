# ProiectSO
In order to use treasure manager, you have to compile using gcc -Wall -o man treasure_manager.c treasure_hub.c main.c 
You have to compile the calculate_score.c external program using gcc -Wall -o calculate_score calculate_score.c
In order to use treasure hub, you have to compile using gcc -Wall -o hub treasure_manager.c treasure_hub.c main.c

Proiect SO - Treasure manager
Functions:
1) Add -> ./p --add <HuntID>
2) List -> ./p --list <HuntID>
3) List Treasure -> ./p --view <HuntID> <TreasureID>
4) Remove Treasure -> ./p --remove_treasure <HuntID> <TreasureID>
5) Remove Hunt -> ./p --remove_hunt <HuntID>

Proiect SO - Treasure hub
Functions:
1) start_monitor
2) list_hunts
3) list_treasures
4) view_treasure
5) stop_monitor
6) calculate_score
7) exit
