#include <Arduino.h>
#include <LiquidCrystal.h>

class LCD_plot_char {
  private:
    byte plot_char[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int get_column_value(int column);
  public:
    LCD_plot_char(byte plot[5]);
    void createChar(int lcd_save_pos, LiquidCrystal &lcd);
};

class LCD_plotter {
  private:
    LiquidCrystal * lcd;
  public:
    LCD_plotter(LiquidCrystal &lcd_pointer);
    void plot(byte plot_data[], int plot_data_length, int col, int row);
};

