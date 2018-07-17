



const int char_width = 5;
const int measurement_history_size = 40;
const int measurement_history_chars = measurement_history_size / 5; // cuts lower and should have bound of 8
const int DEBUG_LEVEL = 3;// 0 to 10, 0=No debug

int measurement_history[measurement_history_size];

int get_measurement() {
  float number = rand() % 8;
  return number;
}

void add_measurement(int measurement_history[], int measurement) {
  //slide measurements forgetting oldest
  for (int i = 0; i < measurement_history_size - 1; i++) {
    measurement_history[i] = measurement_history[i + 1];
  }
  measurement_history[measurement_history_size - 1] = measurement;
}






void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.blink();
  lcd.print("Start :)...");
  delay(2000);

}

void loop() {
  Serial.println("Start");

  int latest_measurement;
  latest_measurement = get_measurement();
  Serial.print("Measurement:");
  Serial.println(latest_measurement);
  add_measurement(measurement_history, latest_measurement);
  lcdprint_measurement_name_value(0, 0);
  lcdprint_measurement_plot(1, 0);

  Serial.print("Ram:");
  Serial.println(freeRam());
  delay(1000);
}


String measurement_name = "Temperature";
void lcdprint_measurement_name_value(int col, int row) {
  lcd.setCursor(row, col);
  lcd.print(measurement_name);
  lcd.print(":");
  lcd.print(measurement_history[measurement_history_size - 1]);
}



void lcdprint_measurement_plot(int col, int row) {
  for (int i = 0; i < measurement_history_chars; i++) {
    if (DEBUG_LEVEL > 5) {
      printArray(measurement_history + (char_width * i), char_width);
    }
    LCD_plot_char * plot_char = new LCD_plot_char(measurement_history + (char_width * i));
    plot_char->createChar(i);
    delete plot_char;
  }

  lcd.setCursor(row, col);
  for (int i = 0; i < measurement_history_chars; i++) {
    lcd.write(byte(i));
  }

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




