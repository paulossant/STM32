#include "main.h"

#ifndef Menu
#define Menu

typedef struct
{
    unsigned int pos1[2]; //x1, y1
    unsigned int pos2[2]; //x2, y2
    unsigned int BG;      //background color for the button
    unsigned int col;     //textcolor for the button
    char text[20];        //text to be written in button
    char action[1];       //What the button do
    unsigned int value1;  //Value
    double value2;        //Value multiplier
} button;

typedef struct
{
    unsigned int ID;
    unsigned int BG;                // Background colour
    char text[8][20];              // Background colour is the same as window BG
    unsigned int text_pos[10][2];   //[same index as text][x1,y1]
    unsigned int col;               //textcolor for text
    button *listbut[15];
    unsigned int nbut;  			//number of buttons
    unsigned int ntext;  			//number of text
} window;

#endif



extern window M ;
extern window F ;
extern window A ;
extern window O ;
extern window P ;
extern window D ;

void drawButton(button *B);
void drawWindow(window *W);
void putVal(void);
void takeAction(button *b);
void touchPress(uint16_t x, uint16_t y);
