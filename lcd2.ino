#include <Arduino.h>
#include <LiquidCrystal.h>
#include "lcd_plotter.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
int rs = 2;
int rw = 3;
int en = 4;
int d[8] = {5, 6, 7, 8, 9, 10, 11, 12};

LiquidCrystal lcd(rs, en, d[4], d[5], d[6], d[7]);
LCD_Printer lcd_printer = LCD_Printer(lcd);

//END lcd init

byte get_next_measurement() {
  byte value = rand() % 255;
  return value;
}

//simple scale to 8 buckets
byte scale_measurement(byte current_value) {
  byte scaled = current_value >> 5;
  return scaled;
}

//TO complete, should be inside LCD plot char, because the number of buckets depends on the screen line height.
int scale_measurement(int min_value, int max_value, int current_value) {
  int bucket_size = (max_value - min_value) / 9;
  int scaled = current_value / bucket_size;
  return scaled;
}

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


void update_measurement(Measurement_History &measurement_hist);
void update_measurement(Measurement_History &measurement_hist) {
  byte latest_measurement;
  latest_measurement = get_next_measurement();
  Serial.print("Measurement:");
  Serial.print(latest_measurement);
  byte scaled_measurement = scale_measurement(latest_measurement);
  Serial.print("->");
  Serial.println(scaled_measurement);
  measurement_hist.add_measurement(scaled_measurement);

}

String time_name[3] = {"sec", "min", "hour"};
void print_measurement_data(Measurement_History &measurement, int time_scale);
void print_measurement_data(Measurement_History &measurement, int time_scale) {
  lcd.setCursor(9, 1);
  lcd.print(time_name[time_scale]);
  lcd_printer.print_name_value(measurement, 0, 0);
  lcd_printer.plot(measurement.history[time_scale], 1, 0);

}

int measurments_count = 3;
Measurement_History m_temp = Measurement_History("Temperature");
Measurement_History m_humid = Measurement_History("Humidity");
Measurement_History m_light = Measurement_History("Light");

Measurement_History * measurements[] = {&m_temp, &m_humid, &m_light};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.blink();
  lcd.print("Start :)...");
  delay(2000);

}

int current_measurement = 0;
int current_time_scale = 0;
int current_plot_seconds = 0;

void loop() {
  Serial.println("Start");
  Serial.print("Ram:");
  Serial.println(freeRam());

  if (current_plot_seconds > 30) {
    current_plot_seconds = 0;
    current_measurement = (current_measurement + 1) % measurments_count;
    if (current_measurement == 0) {
      current_time_scale = (current_time_scale + 1) % 3;
    }
    lcd.clear();
  }

  for (int i = 0; i < measurments_count; i++) {
    update_measurement(*measurements[i]);
  }

  print_measurement_data(*measurements[current_measurement], current_time_scale);

  current_plot_seconds++;

  Serial.print("Ram:");
  Serial.println(freeRam());
  delay(1000);
}






