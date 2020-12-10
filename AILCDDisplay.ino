#include "U8glib.h"

U8GLIB_ST7920_128X64_1X u8g(5);

struct Point{
  int x;
  int y;
};

Point points[] = {
  //{0,0},{0,1},{0,2},{1,2},{1,3},{1,4},{2,4},{3,4},{3,5},{4,5},{5,5},{6,5},{6,6},{6,7},{6,8},{7,8},{8,8},{9,8},{9,9},{10,9},{11,9},{11,10},{11,11}
  {8,2},{7,2},{7,3},{7,4},{6,4},{6,5},{6,6},{6,7},{6,8},{7,8},{8,8},{9,8},{9,9}
};

Point walls[] = {
  {2,0},{2,1},{4,1},{2,2},{4,2},{9,2},{10,2},{11,2},{2,3},{3,3},{4,3},{8,3},{9,3},{10,3},{11,3},{8,4},{8,5},{8,6},{0,8},{1,8},{2,8},{4,8},{0,9},{1,9},{2,9},{4,9},{4,10},{4,11}
};

enum CellType{
  W,
  B,
  P,
  SR,
  SL,
  SU,
  SD,
  E
};

void Cost(void){
  u8g.setFont(u8g_font_10x20);
  u8g.drawStr( 88, 35, "61");
}

void Block (int x,int y){
  u8g.drawFrame(x*5+2,y*5+2,6,6);
}

void Path (int x, int y){
  u8g.drawFrame(x*5+2,y*5+2,6,6);
  u8g.drawBox(x*5+4,y*5+4, 2,2);
}

void Wall (int x, int y){
  u8g.drawBox(x*5+2,y*5+2, 6,6);
}

void Start_UP(int x, int y){
  u8g.drawLine(x*5+2,y*5 +5, x*5+7, y*5+5);
  u8g.drawBox(x*5+4,y*5+4, 2,2);
  u8g.drawFrame(x*5+2,y*5+2,6,6);
}

void Start_DOWN(int x, int y){
  u8g.drawLine(x*5+2,y*5 +4, x*5+7, y*5+4);
  u8g.drawBox(x*5+4,y*5+4, 2,2);
  u8g.drawFrame(x*5+2,y*5+2,6,6);
}

void Start_RIGHT(int x, int y){
  u8g.drawLine(x*5+4,y*5+2, x*5+4, y*5+2);
  u8g.drawBox(x*5+4,y*5+4, 2,2);
  u8g.drawFrame(x*5+2,y*5+2,6,6);
}

void Start_LEFT(int x, int y){
  u8g.drawLine(x*5 + 5,y*5+2, x*5+5, y*5+7);
  u8g.drawBox(x*5+2+2,y*5+2+2, 2,2);
  u8g.drawFrame(x*5+2,y*5+2,6,6);
}

void End (int x, int y){
  u8g.drawFrame(x*5+2,y*5+2,6,6);
  u8g.drawLine(x*5+2,y*5+2, x*5 + 5+2, y*5+5+2);
  u8g.drawLine(x*5+5+2,y*5+2, x*5+2 , y*5+5+2);
}

CellType Cells[12][12]= {
  {B,B,B,B,B,B,B,B,B,B,B,B},
  {B,B,B,B,B,B,B,B,B,B,B,B},
  {B,B,B,B,B,B,B,B,B,B,B,B},
  {B,B,B,B,B,B,B,B,B,B,B,B},
  {B,B,B,B,B,B,B,B,B,B,B,B},
  {B,B,B,B,B,B,B,B,B,B,B,B},
  {B,B,B,B,B,B,B,B,B,B,B,B},
  {B,B,B,B,B,B,B,B,B,B,B,B},
  {B,B,B,B,B,B,B,B,B,B,B,B},
  {B,B,B,B,B,B,B,B,B,B,B,B},
  {B,B,B,B,B,B,B,B,B,B,B,B},
  {B,B,B,B,B,B,B,B,B,B,B,B},
};

void drawHeading(void) {
   u8g.setFont(u8g_font_04b_03);
   u8g.drawStr( 65, 10, "SHORTEST PATH");
   u8g.drawStr( 85, 17, "COST");
}

void drawKey(void) {
   u8g.setFont(u8g_font_04b_03);
   u8g.drawStr( 75, 47, "Start = ");
   u8g.drawStr( 78, 57, "End = ");
   Start_UP(21,8);
   End(20, 10);
}

void clearLCD(){
    u8g.firstPage();  
    do {
    } while( u8g.nextPage() );
}

CellType startDir(Point p, Point n){
    if(p.x < n.x && p.y == n.y){
      return SR;
    }
    else if(p.x > n.x && p.y == n.y){
      return SL;
    }
    else if(p.x == n.x && p.y < n.y){
      return SD;
    }
    else if(p.x == n.x && p.y > n.y){
      return SU;
    }
    else{
      return P;
    }
}

void setup(void) {
  pinMode(3, OUTPUT);
  analogWrite(3, 128);
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }

  for(int i = 0; i < sizeof(walls)/sizeof(Point); i++){
    Point w = walls[i];
    Cells[w.x][w.y] = W;
  }
  
  for(int i = 0; i < sizeof(points)/sizeof(Point); i++){
    Point p = points[i];
    if(i == 0){
      Cells[p.x][p.y] = startDir(p,points[i+1]);  
    }
    else if(i == sizeof(points)/sizeof(Point)-1){
      Cells[p.x][p.y] = E;
    }
    else
      Cells[p.x][p.y] = P;
  }
  
}

void loop(void) {
  u8g.firstPage();
  do{
    for(int y = 0; y < 12; y++){
      for(int x = 0; x < 12; x++){
        if(Cells[x][y] == B)
            Block(x,y);
         else if(Cells[x][y] == W)
            Wall(x,y);
         else if(Cells[x][y] == P)
            Path(x,y);
         else if(Cells[x][y] == SR)
            Start_RIGHT(x,y);
         else if(Cells[x][y] == SD)
            Start_DOWN(x,y);
          else if(Cells[x][y] == SL)
            Start_LEFT(x,y);
         else if(Cells[x][y] == SU)
            Start_UP(x,y);   
         else if(Cells[x][y] == E)
            End(x,y);
      }
    }
    drawHeading();
    Cost();
    drawKey();
  }while(u8g.nextPage());
  delay(2000);
  clearLCD();  
}
