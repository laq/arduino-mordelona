#include "lcd_measurement.h"
#include <LiquidCrystal.h>

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

LCD_Printer::LCD_Printer(LiquidCrystal &lcd_screen): lcd_plotter(lcd_screen) {
  lcd = &lcd_screen;
}

void LCD_Printer::print_name_value(Measurement_History &measurement, int col, int row) {
  lcd->setCursor(row, col);
  lcd->print(measurement.name);
  lcd->print(":");
  lcd->print(measurement.history[0][measurement.history_size - 1]);
}

void LCD_Printer::plot(Measurement_History &measurement, int time_scale, int col, int row) {
  lcd_plotter.plot(measurement.history[time_scale], Measurement_History::history_size , col, row);
}






