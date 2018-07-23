#include <LiquidCrystal.h>
const int DEBUG_LEVEL = 3;// 0 to 10, 0=No debug

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
int rs = 2;
int rw = 3;
int en = 4;
int d[8] = {5, 6, 7, 8, 9, 10, 11, 12};

const int numRows = 2;
const int numCols = 16;
const int char_width = 5;

LiquidCrystal lcd(rs, en, d[4], d[5], d[6], d[7]);

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


void printArray(byte array[], int size) {
  Serial.print("[");
  for (int i = 0; i < size; i++) {
    Serial.print(array[i]);
    Serial.print(",");
  }
  Serial.println("]");
}

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

String time_name[3] = {"sec", "min", "hour"};
class Measurement_History {
  public:
    const static int history_size = 40;
    const static int history_chars = history_size / 5; // cuts lower and should have bound of 8

    byte history[3][history_size]; // 0 == seconds, 1 == minutes, 2 == hours
    int current_count[2] = {0, 0};
    int current_sum[2] = {0, 0};

    int max_value = 255;
    int min_value = 0;
    String name;

    Measurement_History() {
      name = "Measurement";
    }
    Measurement_History(String measurement_name) {
      name = measurement_name;
    }

    void add_measurement(byte measurement) {
      add_measurement(measurement, 0); // add seconds measurment
    }

    void add_measurement(byte measurement, byte history[]) {
      for (int i = 0; i < history_size - 1; i++) {
        history[i] = history[i + 1];
      }
      history[history_size - 1] = measurement;
    }

    void add_measurement(byte measurement, int time_scale) {
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
};

class LCD_plot_char {
  private:
    byte plot_char[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  public:
    LCD_plot_char(byte plot[5]) {
      for (int i = 0; i < 5; i++) {
        int row = 8 - plot[i];
        for (int j = row; j < 8; j++) {
          plot_char[j] += get_column_value(i);
        }
      }
    }

    void createChar(int lcd_save_pos) {
      lcd.createChar(lcd_save_pos, plot_char);
    }

    int get_column_value(int column) {
      //int column_value[5] = {16, 8, 4, 2, 1};
      int exponent = 4 - column;
      int power = 1 << exponent; // 2^exponent
      return power;
    }
};

class LCD_Printer {
  public:
    static void print_name_value(Measurement_History &measurement, int col, int row) {
      lcd.setCursor(row, col);
      lcd.print(measurement.name);
      lcd.print(":");
      lcd.print(measurement.history[0][measurement.history_size - 1]);
    }

    static void plot(byte history[], int col, int row) {
      for (int i = 0; i < Measurement_History::history_chars; i++) {
        if (DEBUG_LEVEL > 5) {
          printArray(history + (char_width * i), char_width);
        }
        LCD_plot_char * plot_char = new LCD_plot_char(history + (char_width * i));
        plot_char->createChar(i);
        delete plot_char;
      }
      lcd.setCursor(row, col);
      for (int i = 0; i < Measurement_History::history_chars; i++) {
        lcd.write(byte(i));
      }
    }
};

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

void print_measurement_data(Measurement_History &measurement, int time_scale);
void print_measurement_data(Measurement_History &measurement, int time_scale) {
  lcd.setCursor(9, 1);
  lcd.print(time_name[time_scale]);
  LCD_Printer::print_name_value(measurement, 0, 0);
  LCD_Printer::plot(measurement.history[time_scale], 1, 0);

}

int measurments_count = 3;
Measurement_History m_temp = Measurement_History("Temperature");
Measurement_History m_humid = Measurement_History("Humidity");
Measurement_History m_light = Measurement_History("Light");

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

  update_measurement(m_temp);
  update_measurement(m_humid);
  update_measurement(m_light);

  switch (current_measurement) {
    case 0:
      print_measurement_data(m_temp, current_time_scale);
      break;
    case 1:
      print_measurement_data(m_humid, current_time_scale);
      break;
    case 2:
      print_measurement_data(m_light, current_time_scale);
      break;
  }
  current_plot_seconds++;

  Serial.print("Ram:");
  Serial.println(freeRam());
  delay(1000);
}






