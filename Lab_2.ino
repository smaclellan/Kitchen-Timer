volatile int count=0;
volatile int countEnabled=0;
int speakerPin = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(speakerPin, OUTPUT);
  
  attachInterrupt(0, resetCounter, FALLING);
  
  // digitalWrite(latchpin, LOW);
  // shiftOut(datapin, clockpin, LSBFIRST, code);
  // digitalWrite(latchpin, HIGH);
}

void displayNumber(int number) {
  int datapin=4, latchpin=5, clockpin=6;
  int datapin2=7, latchpin2=8, clockpin2=9;

  pinMode(datapin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(clockpin, OUTPUT);

  pinMode(datapin2, OUTPUT);
  pinMode(latchpin2, OUTPUT);
  pinMode(clockpin2, OUTPUT);
  
  int firstDigit=number/10;
  int secondDigit=number%10;
  int codes[10]={0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11110110};

  //first display
  digitalWrite(latchpin, LOW);
  shiftOut(datapin, clockpin, LSBFIRST, codes[firstDigit]);
  digitalWrite(latchpin, HIGH);

  //second display
  digitalWrite(latchpin2, LOW);
  shiftOut(datapin2, clockpin2, LSBFIRST, codes[secondDigit]);
  digitalWrite(latchpin2, HIGH);
  
}

void resetCounter() {
  count=60;
  countEnabled=1;
  displayNumber(count);
  
  cli();
  TCCR1A=0;
  TCCR1B=0;
  TCNT1=0;

  TCCR1B |= 0b101;
  OCR1A = 15650;

  TCCR1B |= 0b1000;
  TIMSK1 |= 0b10;

  EIMSK |= 1;
  EICRA |= 0b11;

  sei();
  
}

ISR(TIMER1_COMPA_vect){
  if(countEnabled==1){
    count -= 1;
    displayNumber(count);
    if(count==0){   
      countEnabled=0;
      int timer=0;
      while(timer<2500){
        digitalWrite(speakerPin, HIGH);
        delay(50);
        digitalWrite(speakerPin, LOW);
        delay(50);
        timer+=1;
      }
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = analogRead(0);
  if(reading>475){
    resetCounter();
  }
}
