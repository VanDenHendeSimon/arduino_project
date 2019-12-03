int knop = A0;
int buzzer = 2;

String oude_knop = "0";
String nieuwe_knop = "0";

// beetje marge
int range_knop1[] = {420, 480};
int range_knop2[] = {250, 300};
int range_knop3[] = {140, 230};
int range_knop4[] = {80, 130};

// sequence
int count = 0;
String sequence = "";
String input = "";
int input_count = 0;


void setup () {
  Serial.begin(9600);
  pinMode(knop, INPUT);
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
  
  return _knop;
}

void append_sequence() {
  count += 1;

  String v = String(random(4)+1);
  sequence += v;
  Serial.println("Sequence: " + sequence);

  // reset input zodat user weer opnieuw moet beginnen
  input = "";
  input_count = 0;
}

void game_over() {
  Serial.println("GAME OVER");

//  digitalWrite(buzzer, 1);
  delay(1000);
  digitalWrite(buzzer, 0);
}

void play_again() {
  sequence = "";
  count = 0;
  append_sequence();

  input = "";
  input_count = 0;
}
