#include "lcd_plotter.h"
#include <LiquidCrystal.h>

const int LCD_PLOTTER_DEBUG_LEVEL = 3;// 0 to 10, 0=No debug

const int numRows = 2;
const int numCols = 16;
const int char_width = 5;

void printArray(byte array[], int size) {
  Serial.print("[");
  for (int i = 0; i < size; i++) {
    Serial.print(array[i]);
    Serial.print(",");
  }
  Serial.println("]");
}

LCD_plot_char::LCD_plot_char(byte plot[5]) {
  for (int i = 0; i < 5; i++) {
    int row = 8 - plot[i];
    for (int j = row; j < 8; j++) {
      plot_char[j] += get_column_value(i);
    }
  }
}

void LCD_plot_char::createChar(int lcd_save_pos, LiquidCrystal &lcd) {
  lcd.createChar(lcd_save_pos, plot_char);
}

int LCD_plot_char::get_column_value(int column) {
  //int column_value[5] = {16, 8, 4, 2, 1};
  int exponent = 4 - column;
  int power = 1 << exponent; // 2^exponent
  return power;
}

LCD_plotter::LCD_plotter(LiquidCrystal &lcd_object) {
  lcd = &lcd_object;
}

//plot_data_lenght max 40, but neither tested with less
void LCD_plotter::plot(byte plot_data[], int plot_data_length, int col, int row) {
  int plot_chars = plot_data_length / char_width;
  for (int i = 0; i < plot_chars; i++) {
    if (LCD_PLOTTER_DEBUG_LEVEL > 5) {
      printArray(plot_data + (char_width * i), char_width);
    }
    LCD_plot_char * plot_char = new LCD_plot_char(plot_data + (char_width * i));
    plot_char->createChar(i, *lcd);
    delete plot_char;
  }
  lcd->setCursor(row, col);
  for (int i = 0; i < plot_chars; i++) {
    lcd->write(byte(i));
  }
}


