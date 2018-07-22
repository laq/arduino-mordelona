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

int get_next_measurement() {
  int value = rand() % 255;
  return value >> 5;
}

//TO complete, should be inside LCD plot char, because the number of buckets depends on the screen line height.
int scale_measurement(int min_value, int max_value, int current_value){
  int bucket_size = (max_value-min_value)/9;
  int scaled = current_value / bucket_size;
  return scaled;
}


void printArray(int array[], int size) {
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


class Measurement_History {
  public:
    const static int history_size = 40;
    const static int history_chars = history_size / 5; // cuts lower and should have bound of 8

    int history[history_size];
    int max_value = 255;
    int min_value = 0;
    String name = "Temperature";

    Measurement_History(String measurement_name) {
      name = measurement_name;
    }

    void add_measurement(int measurement) {
      //slide measurements forgetting oldest
      for (int i = 0; i < history_size - 1; i++) {
        history[i] = history[i + 1];
      }
      history[history_size - 1] = measurement;
    }    
};

class LCD_plot_char {
  private:
    byte plot_char[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  public:
    LCD_plot_char(int plot[5]) {
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
      short exponent = 4 - column;
      short power = 1 << exponent; // 2^exponent
      return power;
    }
};

class LCD_Printer {
  public:
    static void print_name_value(Measurement_History &measurement, int col, int row) {
      lcd.setCursor(row, col);
      lcd.print(measurement.name);
      lcd.print(":");
      lcd.print(measurement.history[measurement.history_size - 1]);
    }

    static void plot(Measurement_History &measurement, int col, int row) {
      for (int i = 0; i < measurement.history_chars; i++) {
        if (DEBUG_LEVEL > 5) {
          printArray(measurement.history + (char_width * i), char_width);
        }
        LCD_plot_char * plot_char = new LCD_plot_char(measurement.history + (char_width * i));
        plot_char->createChar(i);
        delete plot_char;
      }
      lcd.setCursor(row, col);
      for (int i = 0; i < measurement.history_chars; i++) {
        lcd.write(byte(i));
      }
    }
};

void update_measurement(Measurement_History &measurement_hist);
void update_measurement(Measurement_History &measurement_hist) {
  int latest_measurement;
  latest_measurement = get_next_measurement();
  Serial.print("Measurement:");
  Serial.println(latest_measurement);
  measurement_hist.add_measurement(latest_measurement);

}

void print_measurement_data(Measurement_History &measurement);
void print_measurement_data(Measurement_History &measurement) {
  LCD_Printer::print_name_value(measurement, 0, 0);
  LCD_Printer::plot(measurement, 1, 0);
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.blink();
  lcd.print("Start :)...");
  delay(2000);

}

Measurement_History temperature_hist = Measurement_History("Temperature");
Measurement_History humidity_hist = Measurement_History("Humidity");

int current_hist = 1;
int current_plot_seconds = 0;

void loop() {
  Serial.println("Start");

  if (current_plot_seconds > 10) {
    current_plot_seconds = 0;
    current_hist = (current_hist + 1) % 2;
    lcd.clear();  
  }

  update_measurement(temperature_hist);
  update_measurement(humidity_hist);

  if (current_hist == 0) {
    print_measurement_data(temperature_hist);
  }
  else {
    print_measurement_data(humidity_hist);
  }
  current_plot_seconds++;


  Serial.print("Ram:");
  Serial.println(freeRam());
  delay(1000);
}






