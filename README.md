Curse of Boulder Dash - game based on First Star Software's "Boulder Dash" (also known as "Rockford") which was released in 1984.

Author: Arkadiusz Tomczak
Version: 1.0
Date: 30-11-2015
Language:
 - Program language: N/A
 - Variable names: Polish
 - Comments: Polish

Main character - Rockford (represented on screen as '&' character) can walk across the map. He can easily go if there is an air field (' ') 
and he also can dig throught dirt (.). In order to win he has to collect al of the diamonds (represented as '$'). He has to reach the exit 'X',
but he can enter it only after collecting all of the diamonds. He will be interrupted by stones ('O') which can fall down if there is no dirt (.) below them.
Rockford can move them (in left or right) to other fields filled with air.
Map has to be surrounded by unbreakable stone. Otherwise, it will create undefined behaviour ir Rockford will go beyond the map.

After compiling you have to draw a map in console. For example:

########################################
#.............O........................#
#.............O........................#
#.............O.................X......#
#.............O........................#
#....X...................$.............#
#........................$.............#
#........................$.............#
#......................................#
#..................  O&OO      ........#
#...X..................................#
#......................................#
#......................................#
#......................                #
########################################

Program will know that you finished drawing after you give it an empty row (just hit enter two times). After that you can move Rockford using WASD keys. Other keys are ignored.
Text User Interface is provided (implemented with ncurses library). If you want to turn it on, you have to compile it with -DTUI option. 
Without it, program will just draw the state of the map when there will be nothing on stdin.

Important note: program was written without using C structs. Becouse of that some functions have way too many parameters.
