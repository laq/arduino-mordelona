#include <LiquidCrystal.h>
struct Sliced_measurement_history;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
int rs = 2;
int rw = 3;
int en = 4;
int d[8] = {5, 6, 7, 8, 9, 10, 11, 12};

const int numRows = 2;
const int numCols = 16;

LiquidCrystal lcd(rs, en, d[4], d[5], d[6], d[7]);


class LCD_plot_char {
  private:
    byte plot_char[8] = {0,0,0,0,0,0,0,0};    
    
  public:
    LCD_plot_char(int plot[5]) {      
      for (int i = 0; i < 5; i++) {
        int row = 8 - plot[i];
        for (int j = row; j < 8; j++) {
          plot_char[j] += get_column_value(i);
        }
      }
    }


    void createChar(int char_num){
      lcd.createChar(char_num, plot_char);
    }

    int get_column_value(int column){
      //int column_value[5] = {16, 8, 4, 2, 1};
      short exponent = 4 - column;
      short power = 1<<exponent; // 2^exponent
      return power;
    }

    

};







