#ifndef BUGFIX_H
#define BUGFIX_H
//Fix the random texture dissapearing in large levels
void FixRenderBugs();

//Fixes the vert location to be top of qp instead of bottom and increase angle search, so you leave the qp less often
//This was needed to get th4+ levels not to bug
//This is based of the thps4 function
//Still need some tweaking for it to work 100% for thug2+ levels
void FixVertBugs();

//Make sure chat messages don't crash the game
void FixChat();

//Fix some crashes while parsing some QB files
void FixQBParsing();
#endif