



const int char_width = 5;
const int measurement_history_size = 40;
const int measurement_history_chars = measurement_history_size / 5; // cuts lower and should have bound of 8

struct Sliced_measurement_history {
  int plot_char[8][5];
};

void clean_sliced_measurement_history(Sliced_measurement_history &measurement_history) {
  for (int i = 0;  i < measurement_history_chars; i++) {
    for (int j = 0; j < char_width; j++) {
      measurement_history.plot_char[i][j] = 0;
    }
  }
}

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

Sliced_measurement_history split_measurement_history(int measurement_history[]) {
  Sliced_measurement_history history;
  clean_sliced_measurement_history(history);
  for (int i = 0;  i < measurement_history_chars; i++) {
    for (int j = 0; j < char_width; j++) {
      history.plot_char[i][j]=measurement_history[i*char_width+j];
    }
  }
  return history;
}




void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.blink();
  lcd.print("Start :)...");
  delay(1000);

}

void loop() {
  Serial.println("Start");
  int latest_measurement;
  latest_measurement = get_measurement();
  Serial.print("Measurement:");
  Serial.println(latest_measurement);
  print_measurement(latest_measurement);
  Serial.print("Ram:");
  Serial.println(freeRam());
  delay(500);
}

void print_measurement(int latest_measurement) {
  Serial.println("Printing measurement");
  add_measurement(measurement_history, latest_measurement);
  Serial.println("Added Measurement");

  Sliced_measurement_history sliced_measurement_history = split_measurement_history(measurement_history);
  Serial.println("Sliced measurement history");  



  for (int i = 0; i < measurement_history_chars; i++) {
    Serial.print("Print char plot:");
    Serial.println(i);
    printArray(sliced_measurement_history.plot_char[i], 5);
    LCD_plot_char * plot_char = new LCD_plot_char(sliced_measurement_history.plot_char[i]);
    plot_char->createChar(i);    
    delete plot_char;
  }

  lcd.setCursor(11, 0);
  lcd.print(latest_measurement);

  lcd.setCursor(0, 1);
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




