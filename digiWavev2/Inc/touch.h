/*
#define YP A2   must be an analog pin, use "An" notation!		-> 		RS -> PB10
#define XM A3   must be an analog pin, use "An" notation!		->		CS -> PB9
#define YM 7    can be a digital pin									D7 -> PB7
#define XP 4    can be a digital pin									D4 -> PB4

YP - D6
YM - RS
XP - WR
XM - D7

  pinMode(_yp, INPUT);
  pinMode(_ym, INPUT);
  pinMode(_xp, OUTPUT);
  pinMode(_xm, OUTPUT);

  int TouchScreen::readTouchX(void) {
   pinMode(_yp, INPUT);
   pinMode(_ym, INPUT);
   digitalWrite(_yp, LOW);
   digitalWrite(_ym, LOW);

   pinMode(_xp, OUTPUT);
   digitalWrite(_xp, HIGH);
   pinMode(_xm, OUTPUT);
   digitalWrite(_xm, LOW);

   return (1023-analogRead(_yp));
}


int TouchScreen::readTouchY(void) {
   pinMode(_xp, INPUT);
   pinMode(_xm, INPUT);
   digitalWrite(_xp, LOW);
   digitalWrite(_xm, LOW);

   pinMode(_yp, OUTPUT);
   digitalWrite(_yp, HIGH);
   pinMode(_ym, OUTPUT);
   digitalWrite(_ym, LOW);

   return (1023-analogRead(_xm));
}
 */

//	  HAL_Delay(100);
//	  touch_read(valores);
//	  sprintf(Buf,"y = %i", valores[0]);
//	  LCD_WriteString(10, 220, Buf, Font_7x10, WHITE, BLACK);
//	  sprintf(Buf,"x = %i", valores[1]);
//	  LCD_WriteString(90, 220, Buf, Font_7x10, WHITE, BLACK);
//	  sprintf(Buf,"z = %i", valores[2]);
//	  LCD_WriteString(180, 220, Buf, Font_7x10, WHITE, BLACK);
//	  sprintf(Buf,"dummy %i", valores[3]);
//	  LCD_WriteString(270, 220, Buf, Font_7x10, WHITE, BLACK);
//	  HAL_Delay(1000);
void touch_read(volatile uint16_t ADval[]);

