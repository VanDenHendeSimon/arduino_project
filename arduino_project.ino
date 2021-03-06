// 1 input waarde voor alle knoppen
int knop_input = A0;

// ledjes (pin 1 is kapot)
int leds[] = {2, 3, 4, 5};

// buzzer
int buzzer = 9;
int sounds[] = {1000, 2000, 3000, 4000};

// knop handling
String oude_knop = "0";
String nieuwe_knop = "0";

// beetje marge
int range_knop1[] = {450, 550};
int range_knop2[] = {300, 440};
int range_knop3[] = {220, 299};
int range_knop4[] = {150, 219};

// sequence en input (strings zijn handiger dan lists)
String sequence = "";
int count = 0;

String input = "";
int input_count = 0;

bool gameOver = false;

void setup () {
  Serial.begin(9600);

  // pinModes
  pinMode(knop_input, INPUT);
  pinMode(buzzer, OUTPUT);
  for(int i=0; i<4; i++) {
    pinMode(leds[i], OUTPUT);
  }

  start_game(false);
}

void loop() {
  nieuwe_knop = get_knop(analogRead(knop_input));

  if(!gameOver) {
    if (nieuwe_knop != oude_knop && nieuwe_knop != "0"){
      process_input(nieuwe_knop);
  
      if(input[input_count - 1] != sequence[input_count - 1]) {
        game_over();
      } else {
        // Proceed with the game
        if(input_count == count) {
          Serial.println("");
          generate_sound_and_light("-2");
          append_sequence();
        }
      }
    }
  // when the game is over
  } else {
    // restart with button
    if(analogRead(knop_input) > 0) {
      start_game(true);
    }
    // restart with terminal input
    if(Serial.available()) {
      byte serial_input = Serial.read();
      if(serial_input == 121) {
        start_game(true);
      } else if(serial_input == 110) {
        // Acknowledge user input
        Serial.println("n");
        generate_sound_and_light("-3");
      }
    }
  }

  oude_knop = nieuwe_knop;
  delay(20);
}

String get_knop(int waarde) {
  String knop = "0";
  
  if (waarde > range_knop1[0] && waarde < range_knop1[1]) knop = "1";
  if (waarde > range_knop2[0] && waarde < range_knop2[1]) knop = "2";
  if (waarde > range_knop3[0] && waarde < range_knop3[1]) knop = "3";
  if (waarde > range_knop4[0] && waarde < range_knop4[1]) knop = "4";

  return knop;
}

void process_input(String knop) {
  input += knop;
  input_count += 1;

  // audiovisuele feedback
  generate_sound_and_light(knop);
  Serial.print(knop);
}

void generate_sound_and_light(String knop) {
  if (knop == "-1") {
    // Game over
     play_game_over();
  } else if(knop == "-2") {
    // Correct sequence
    play_right_answer();
  } else if(knop == "-3") {
    // Player doesnt want to play again
    acknowledge_input(4);
  }
  else {
    // Some button was pressed
    play_input(knop);
  }
}

void all_leds_off() {
  for(int i=0; i<4; i++) {
    digitalWrite(leds[i], 0);
  }
}

void all_leds_on() {
  for(int i=0; i<4; i++) {
    digitalWrite(leds[i], 1);
  }
}

void off_one_at_a_time() {
  // een voor een uit
  for(int i=0; i<4; i++) {
    digitalWrite(leds[i], 0);
    play_sound(i+2, 10000);
  }
}

void play_right_answer() {
  // Efkes lampkes uit, Geen geluid
  for(int i=0; i<4; i++) {
    digitalWrite(leds[i], 0);
  }
  delay(200);
}

void play_game_over() {
  // Flikker 6x + geluid als delay
  for(int i=0; i<6; i++) {
    all_leds_off();
    play_sound(3, 30000);

    all_leds_on();
    play_sound(4, 30000);
  }
  acknowledge_input(2);
}

void play_input(String knop) {
  int pin = led_pin(knop);

  digitalWrite(pin, 1);
  play_sound(pin, 30000); // 30.000 is the total delay in microseconds
  digitalWrite(pin, 0);
  delay(250);
}

void acknowledge_input(int amount) {
  for(int j=0; j<amount; j++) {
    all_leds_on();
    delay(20);
    off_one_at_a_time();
    delay(50);
  }
}

void play_sound(int pin, int duration) {
  // Duration in microseconds
  int pulse_duration = sounds[pin-2];
  int amount = duration / pulse_duration;

  for(int i=0; i<amount; i++) {
    digitalWrite(buzzer, 1);
    delayMicroseconds(pulse_duration);

    digitalWrite(buzzer, 0);
    delayMicroseconds(pulse_duration); 
  }
}

void append_sequence() {
  count += 1;

  String v = String(random(4)+1);
  sequence += v;

  // audiovisuele weergave van de sequence
  play_sequence();

  // reset input zodat user weer opnieuw moet beginnen
  // nadat de sequence is afgespeeld zodat inputs tijdens weg zijn
  input = "";
  input_count = 0;
}

void play_sequence() {
  Serial.println("Sequence: " + sequence);
  Serial.print("Input   : ");

  for (int i = 0; i < count; i++) {
    generate_sound_and_light(String(sequence[i]));
  }
}

void game_over() {
  Serial.println("\nGAME OVER");
  gameOver = true;
  generate_sound_and_light("-1");

  Serial.print("Do you want to play again? [y/n] >> ");
}

int led_pin(String knop) {
  return knop.toInt() + 1;
}

void start_game(bool nieuw) {
  generate_sound_and_light("-3");
  delay(500);

  if(nieuw) {
    Serial.println("y");
    Serial.println("Nieuw spel is begonnen!");

    sequence = "";
    count = 0;
  
    input = "";
    input_count = 0;
    gameOver = false;
  }

  randomSeed(analogRead(1));
  append_sequence();
}
