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

Measurement_History::Measurement_History() {
  name = "Measurement";
}
Measurement_History::Measurement_History(String measurement_name) {
  name = measurement_name;
}

void Measurement_History::add_measurement(byte measurement) {
  add_measurement(measurement, 0); // add seconds measurment
}

void Measurement_History::add_measurement(byte measurement, byte history[]) {
  for (int i = 0; i < history_size - 1; i++) {
    history[i] = history[i + 1];
  }
  history[history_size - 1] = measurement;
}

void Measurement_History::add_measurement(byte measurement, int time_scale) {
  add_measurement(measurement, history[time_scale]);
  if (time_scale < 3) {
    current_count[time_scale]++;
    current_sum[time_scale] += measurement;
    if (current_count[time_scale] >= 60) {
      current_count[time_scale] = 0;
      int current_avg = current_sum[time_scale] / 60;
      current_sum[time_scale] = 0;
      add_measurement(current_avg, time_scale + 1);
    }
  }
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

LCD_Printer::LCD_Printer(LiquidCrystal &lcd_screen){
  lcd = &lcd_screen;
}

void LCD_Printer::print_name_value(Measurement_History &measurement, int col, int row) {
  lcd->setCursor(row, col);
  lcd->print(measurement.name);
  lcd->print(":");
  lcd->print(measurement.history[0][measurement.history_size - 1]);
}

void LCD_Printer::plot(byte history[], int col, int row) {
  for (int i = 0; i < Measurement_History::history_chars; i++) {
    if (LCD_PLOTTER_DEBUG_LEVEL > 5) {
      printArray(history + (char_width * i), char_width);
    }
    LCD_plot_char * plot_char = new LCD_plot_char(history + (char_width * i));
    plot_char->createChar(i, *lcd);
    delete plot_char;
  }
  lcd->setCursor(row, col);
  for (int i = 0; i < Measurement_History::history_chars; i++) {
    lcd->write(byte(i));
  }
}

