// 1 input waarde voor alle knoppen
int knop = A0;

// ledjes (pin 1 is kapot)
int led1 = 2;
int led2 = 3;
int led3 = 4;
int led4 = 5;

// buzzer
int buzzer = 10;
int sounds[] = {200, 400, 600, 800, 1000};

// knop handling
String oude_knop = "0";
String nieuwe_knop = "0";

// beetje marge
int range_knop1[] = {420, 480};
int range_knop2[] = {250, 300};
int range_knop3[] = {140, 230};
int range_knop4[] = {80, 130};

// sequence en input (strings zijn handiger dan lists)
String sequence = "";
int count = 0;

String input = "";
int input_count = 0;


void setup () {
  Serial.begin(9600);
  pinMode(knop, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  pinMode(buzzer, OUTPUT);
  
  randomSeed(analogRead(1));

  // Genereer eerste waarde
  append_sequence();
}

void loop() {
  nieuwe_knop = get_knop(analogRead(knop));

  if (nieuwe_knop != oude_knop && nieuwe_knop != "0"){

    input += nieuwe_knop;
    input_count += 1;
    generate_sound_and_light(nieuwe_knop);

    if(input[input_count - 1] != sequence[input_count - 1]) {
      game_over();
    } else {
      Serial.print(input);
    }

    if(input_count == count){
      Serial.println("");
      append_sequence();
    }  
  }

  oude_knop = nieuwe_knop;
  delay(20);
}

String get_knop(int waarde) {
  String _knop = "0";
  
  if (waarde > range_knop1[0] && waarde < range_knop1[1]) _knop = "1";
  if (waarde > range_knop2[0] && waarde < range_knop2[1]) _knop = "2";
  if (waarde > range_knop3[0] && waarde < range_knop3[1]) _knop = "3";
  if (waarde > range_knop4[0] && waarde < range_knop4[1]) _knop = "4";

  // audiovisuele feedback
  generate_sound_and_light(_knop);

  return _knop;
}

void generate_sound_and_light(String _knop) {
  if (_knop == "-1") {
    // Game over
  } else {
    int led_pin = _knop.toInt() + 1;

    digitalWrite(led_pin, 1);
    analogWrite(buzzer, sounds[led_pin-2]);
    delay(500);
    digitalWrite(led_ping, 0); 
  }
}

void append_sequence() {
  count += 1;

  String v = String(random(4)+1);
  sequence += v;

  // audiovisuele weergave van de sequence
  play_sequence();
  Serial.println("Sequence: " + sequence);

  // reset input zodat user weer opnieuw moet beginnen
  // nadat de sequence is afgespeeld zodat inputs tijdens weg zijn
  input = "";
  input_count = 0;
}

void play_sequence() {
  for (int i = 0; i < count; i++) {
    generate_sound_and_light(sequence[i]);
  }
}

void game_over() {
  Serial.println("GAME OVER");
  generate_sound_and_light("-1");
}

void play_again() {
  sequence = "";
  count = 0;
  append_sequence();

  input = "";
  input_count = 0;
}
