#include <Arduino.h>
#include <LiquidCrystal.h>
#include "lcd_plotter.h"

class Measurement_History {
    friend class LCD_Printer;
  public:
    const static int history_size = 40;
  private:
    int max_value = 255;
    int min_value = 0;
    int current_count[2] = {0, 0};
    int current_sum[2] = {0, 0};
    String name;
    byte history[3][history_size]; // 0 == seconds, 1 == minutes, 2 == hours
  public:

    Measurement_History();
    Measurement_History(String measurement_name);
    void add_measurement(byte measurement);
    void add_measurement(byte measurement, byte history[]);
    void add_measurement(byte measurement, int time_scale);
};



class LCD_Printer {
  private:
    LiquidCrystal * lcd;
    LCD_plotter lcd_plotter;
  public:
    LCD_Printer(LiquidCrystal &lcd_screen);
    void print_name_value(Measurement_History &measurement, int col, int row);
    void plot(Measurement_History &measurement, int time_scale, int col, int row);
};

