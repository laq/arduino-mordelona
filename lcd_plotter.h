#include <Arduino.h>
#include <LiquidCrystal.h>

class Measurement_History {
    friend class LCD_Printer;
  public:
    const static int history_size = 40;
    const static int history_chars = history_size / 5; // cuts lower and should have bound of 8
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

class LCD_plot_char {
  private:
    byte plot_char[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int get_column_value(int column);
  public:
    LCD_plot_char(byte plot[5]);
    void createChar(int lcd_save_pos, LiquidCrystal &lcd);
};

class LCD_Printer {
  private:
    LiquidCrystal * lcd;
  public:
    LCD_Printer(LiquidCrystal &lcd_screen);
    void print_name_value(Measurement_History &measurement, int col, int row);
    void plot(byte history[], int col, int row);
    void plot(Measurement_History &measurement, int time_scale, int col, int row);
};

