#include "menu.h"
#include "main.h"
#include "9481.h"
#include "fonts.h"
#include "string.h"
#include "stdio.h"
#include "ad9833.h"

button Freq = { {10, 10}, {310,60},  DARKRED, WHITE, "Freq ", "F", 1, 0};
button Ampl = { {10,70},  {155,120}, DARKBLUE, WHITE, "Ampl", "A", 1, 0};
button Ofs  = { {165,70}, {310,120}, DARKBLUE, WHITE, "OFS" , "O", 1, 0};
button Pha  = { {10,130}, {155,180}, DARKBLUE, WHITE, "Pha" , "P", 1, 0};
button DCyc = { {10,190}, {155,230}, DARKBLUE, WHITE, "13"  , "D", 1, 0};
button Wave = { {165,130},{310,180}, DARKBLUE, WHITE, "Sin" , "W", 1, 0};
button Onoff= { {165,190},{310,230}, DARKBLUE, WHITE, "State: OFF"  , "S", 1, 0};

window M = {0, BLACK, {"hello", " ", "world", " ", "!"}, {{10, 10},{20, 20},{30, 30}}, WHITE, {&Freq, &Ampl, &Ofs, &Pha, &Wave, &DCyc, &Onoff}, 7, 0};

button In = { {10,190}, {80,230},  GRAY, BLACK, 	"UP","i",1,0};
button Dc = { {120,190},{190,230}, GRAY, BLACK, 	"DN","d",1,0};
button Ex = { {240,190},{310,230}, GRAY, BLACK, 	"Home","H",1,0};
button val1={ {5,110}, {35,140},   DARKGRAY, WHITE, "1","1",0,0};
button val2={ {45,110}, {75,140},  WHITE, BLACK, 	"2","2",0,0};
button val3={ {85,110}, {115,140}, WHITE, BLACK, 	"3","3",0,0};
button val4={ {125,110},{155,140}, WHITE, BLACK, 	"4","4",0,0};
button val5={ {165,110},{195,140}, DARKGREEN, WHITE,"5","5",0,0};
button val6={ {205,110},{235,140}, DARKGREEN, WHITE,"6","6",0,0};
button val7={ {245,110},{275,140}, DARKGREEN, WHITE,"7","7",0,0};
button val8={ {285,110},{315,140}, DARKBLUE, WHITE, "8","8",0,0};

window F = {1, DARKRED,  {"Frequency (Hz)", ".","M", "K","I","d"}, {{100,20},{275,125},{15,80},{95,80},{215,80},{295,80}}, WHITE,
		   {&In, &Dc, &Ex, &val1, &val2, &val3, &val4, &val5, &val6, &val7, &val8},11, 6};

window A = {2, DARKBLUE,{"Amplitude (V)", "."}, {{100,20},{155,125}}, WHITE,   {&In, &Dc, &Ex, &val4, &val5},5, 2};

window O = {3, DARKBLUE,{" Offset  (V)", "."}, {{100,20},{155,125}}, WHITE,	   {&In, &Dc, &Ex, &val4, &val5},5, 2};

window P = {4, DARKBLUE,{"Phase (degree)"}, {{100,20}}, WHITE, {&In, &Dc, &Ex, &val3, &val4, &val5}, 6, 1};

window D = {5, DARKBLUE,{"Duty Cycle (%)"}, {{100,20}}, WHITE, {&In, &Dc, &Ex, &val3, &val4, &val5}, 6, 1};

double frequency = 0000001.0;
double amplitude = 1.0;
double offset = 0.0;
double duty = 50.0;    //Duty cylce in percentage
uint16_t phase = 0;
uint8_t output = 0;
uint8_t wfm = 1;
char multistring[20];

double active_param_multiplier;
window *active_window = &M;
button *active_button = &val1;
unsigned int valor1;
unsigned int valor2;


void putVal(void){
	if(active_window == &M)
	{
	sprintf(multistring, "Freq: %.1f Hz", frequency);
    strcpy(Freq.text, multistring);
    sprintf(multistring, "Amp: %1.1f V", amplitude);
    strcpy(Ampl.text, multistring);
    sprintf(multistring, "Off: %1.1f V", offset);
    strcpy(Ofs.text, multistring);
    sprintf(multistring, "Pha: %3d deg", phase);
    strcpy(Pha.text, multistring);
    sprintf(multistring, "Dcyc: %3.1f %%", duty);
    strcpy(DCyc.text, multistring);
	}

    if(active_window == &F)
    {
    frequency = val8.value1*0.1 	+	val7.value1			+	val6.value1*10		+	val5.value1*100		+
				val4.value1*1000	+	val3.value1*10000	+	val2.value1*100000	+	val1.value1*1000000;
    if(frequency>1500000){frequency=1500000;}
    }

    if(active_window == &A)
	{
	amplitude = val5.value1*0.1 + val4.value1;
	if(amplitude>9){amplitude=9;}
	}

    if(active_window == &O)
	{
	offset = val5.value1*0.1 + val4.value1;
	if(offset>9){offset=9;}
	}

    if(active_window == &P)
    {
    phase = val5.value1	+ val4.value1*10 + val3.value1*100;
    if(phase>360){phase=360;}
    }

    if(active_window == &D)
	{
	duty = val5.value1*0.1 + val4.value1 + val3.value1*10;
	}

}

void drawButton(button *B)
{
	LCD_fillRectangle(B -> pos1[0], B -> pos1[1], B -> pos2[0],B -> pos2[1], B -> BG);
	uint16_t x = ((B -> pos2[0] - B -> pos1[0] ) / 2 ) + ((B -> pos1[0]) - strlen(B -> text)*11/2);
	uint16_t y = ((B -> pos2[1] - B -> pos1[1] ) / 2 ) + (B -> pos1[1]-9);
	LCD_WriteString(x, y, B -> text, Font_11x18, B -> col, B -> BG);
}


void drawWindow(window *W)
{
	putVal();
	LCD_fillScreen(W -> BG);

	for (int i=0; i< W -> ntext; i++)
		{
		LCD_WriteString(W -> text_pos[i][0],W -> text_pos[i][1], W ->text[i], Font_11x18, W ->col,W -> BG);
		}

	for (int i=0; i< W -> nbut; i++)
	{
	drawButton(W -> listbut[i]);
	}
}

void touchPress(uint16_t x, uint16_t y)
{
	for (int i=0; i< active_window -> nbut; i++)
	{
		if 	( active_window -> listbut[i] -> pos1[0] < x && x < active_window -> listbut[i]->pos2[0])
		{
			if 	( (active_window -> listbut[i] -> pos1[1]) < y && y < (active_window -> listbut[i]->pos2[1]))
			{
				__NOP();
				takeAction(active_window -> listbut[i]);
			}
			else{};
		}
		else{};
	}


}

void takeAction(button *b){
	if (strcmp(&b -> action[0],"W")==0)
	{
		b -> value1++;
		if (b -> value1 > 3){b -> value1 = 0;}

		ad9833_waveform(b -> value1);
		add9833_DACON(1);
		if (b->value1 == 0)
		{
			sprintf(multistring, "Sin");
		}
		if (b->value1 == 1)
		{
			sprintf(multistring, "Tri");
		}
		if (b->value1 == 2)
		{
			sprintf(multistring, "Sqd");
		}
		if (b->value1 == 3)
		{
			sprintf(multistring, "PWM");
			add9833_DACON(0);
		}
		strcpy(b->text, multistring);
	}

	if (strcmp(&b -> action[0],"S")==0)
	{
		if (b -> value1)
		{
			b -> value1 = 0;
			sprintf(multistring, "State: OFF");
			add9833_DACON(0);
		}
		else
		{
			b -> value1 = 1;
			sprintf(multistring, "State: ON");
			add9833_DACON(1);
		}
		strcpy(b -> text, multistring);
	}

	if (strcmp(&b -> action[0],"F")==0)
		{
		active_window = &F;
		val8.value1 = (int)(frequency*10)%10;
		val7.value1 = (int)frequency%10;
		val6.value1 = (int)frequency/10 %10;
		val5.value1 = (int)frequency/100 %10;
		val4.value1 = (int)frequency/1000 %10;
		val3.value1 = (int)frequency/10000 %10;
		val2.value1 = (int)frequency/100000 %10;
		val1.value1 = (int)frequency/1000000 %10;
		sprintf(multistring, "%1d", val8.value1);
	    strcpy(val8.text, multistring);
	    sprintf(multistring, "%1d", val7.value1);
		strcpy(val7.text, multistring);
		sprintf(multistring, "%1d", val6.value1);
		strcpy(val6.text, multistring);
		sprintf(multistring, "%1d", val5.value1);
	    strcpy(val5.text, multistring);
	    sprintf(multistring, "%1d", val4.value1);
		strcpy(val4.text, multistring);
		sprintf(multistring, "%1d", val3.value1);
		strcpy(val3.text, multistring);
		sprintf(multistring, "%1d", val2.value1);
	    strcpy(val2.text, multistring);
	    sprintf(multistring, "%1d", val1.value1);
		strcpy(val1.text, multistring);
		}

	if (strcmp(&b -> action[0],"A")==0)
		{
		active_window = &A;
		val4.value1 =  (int) amplitude%10;
		val5.value1 =  (int) (amplitude*10)%10;
		sprintf(multistring, "%1d", val5.value1);
	    strcpy(val5.text, multistring);
	    sprintf(multistring, "%1d", val4.value1);
		strcpy(val4.text, multistring);
		}

	if (strcmp(&b -> action[0],"O")==0)
		{
		active_window = &O;
		val4.value1 =  (int) offset%10;
		val5.value1 =  (int) (offset*10)%10;
		sprintf(multistring, "%1d", val5.value1);
	    strcpy(val5.text, multistring);
	    sprintf(multistring, "%1d", val4.value1);
		strcpy(val4.text, multistring);
		}

	if (strcmp(&b -> action[0],"P")==0)
		{
		active_window = &P;
		val5.value1 = (int)phase%10;
		val4.value1 = (int)phase/10 %10;
		val3.value1 = (int)phase/100 %10;
		sprintf(multistring, "%1d", val5.value1);
		strcpy(val5.text, multistring);
		sprintf(multistring, "%1d", val4.value1);
		strcpy(val4.text, multistring);
		sprintf(multistring, "%1d", val3.value1);
		strcpy(val3.text, multistring);

		}
	if (strcmp(&b -> action[0],"D")==0)
		{
		active_window = &D;
		val5.value1 =  (int) (duty*10)%10;
		val4.value1 =  (int) duty%10;
		val3.value1 =  (int) duty/10 %10;
		sprintf(multistring, "%1d", val5.value1);
	    strcpy(val5.text, multistring);
	    sprintf(multistring, "%1d", val4.value1);
		strcpy(val4.text, multistring);
		sprintf(multistring, "%1d", val3.value1);
		strcpy(val3.text, multistring);
		}

	if (strcmp(&b -> action[0],"H")==0)		//Button to go Home
		{
		putVal();	//put correct values on the variable, before making the window switch
		active_window = &M;
		}

	if ( strcmp(&b -> action[0],"1") == 0  || strcmp(&b -> action[0],"2") == 0 || strcmp(&b -> action[0],"3") == 0
	  || strcmp(&b -> action[0],"4") == 0  || strcmp(&b -> action[0],"5") == 0 || strcmp(&b -> action[0],"6") == 0
	  || strcmp(&b -> action[0],"7") == 0  || strcmp(&b -> action[0],"8") == 0 )
		{
		active_button = b;
		}

	if (strcmp(&b -> action[0],"i")==0)		//increment active button
		{
		active_button -> value1 ++;
		if (active_button -> value1 > 9){active_button -> value1 = 9;}
	    sprintf(multistring, "%1d", active_button -> value1);
	    strcpy(active_button -> text, multistring);
	    __NOP();
		}
	if (strcmp(&b -> action[0],"d")==0)		//increment active button
		{
		active_button -> value1 --;
		if (active_button -> value1 > 9){active_button -> value1 = 0;}
	    sprintf(multistring, "%1d", active_button -> value1);
	    strcpy(active_button -> text, multistring);
		}

	drawWindow(active_window);
}

