
void software_reset() {
  asm volatile ("  jmp 0");
}
void lcd(char x) {
  digitalWrite(6, HIGH);
  if (x & 0x80)  digitalWrite(11, HIGH);
  else           digitalWrite(11, LOW);
  if (x & 0x40)  digitalWrite(10, HIGH);
  else           digitalWrite(10, LOW);
  if (x & 0x20)  digitalWrite(9, HIGH);
  else           digitalWrite(9, LOW);
  if (x & 0x10)  digitalWrite(8, HIGH);
  else           digitalWrite(8, LOW);
  digitalWrite(6, LOW);
  delayMicroseconds(40);
}

void lcd2(char x) {
  lcd(x);
  lcd(x << 4);
}
/*A17*/
int array[80];
int array_time[16] = {0, 0, 10, 0, 0, 10, 0, 0, -16, -16, -16, -16, -16, -16, -16, -16}; //学籍番号
int array_str[16] = {36, 25, 29, 21, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16}; //名前格納用の配列
int array_st[16];
void setup() {
  for (int step = 0; step < 80; step++) {
    array[step] = -16;
  }

  for (int step = 0; step < 15; step++) {
    array[step] = array_str[step];
    array[step + 40] = array_time[step];
  }

  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  delay(40);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

  lcd(0x30);
  lcd(0x30);
  lcd(0x30);
  lcd(0x20);

  lcd2(0x28);
  lcd2(0x0c);
  lcd2(0x01);
  delayMicroseconds(1520);
  lcd2(0x06);

  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  Serial.begin(9600);
}

int i = 0;// 変数の定義と初期化
int s = 0;
int s1 = 0;
int s2 = 0;
int m = 0;
int m1 = 0;
int m2 = 0;
int h = 0;
int h1 = 0;
int h2 = 0;
int flag_s = 0;/*桁を制御する変数*/
int flag_m = 0;
int flag_h = 0;
int val = 0;
int val2 = 0;
int path = 0;
unsigned long time;
void loop() {


  val = digitalRead(12);
  val2 = analogRead(A1);
  path = analogRead(A0);
  
  if (val == 0 || val2 > 450) {
    digitalWrite(12, LOW);
    software_reset();
  }
  if (val == 1) {
    time = millis(); /*プログラム実行からの経過時間(ms)をtimeに返す*/
    s = time / 1000; /*毎秒1を変数sに加算する*/
  }


  s1 = s % 10;

  if (s % 10 == 0 && s > 0) {
    flag_s += 1;
  } else {
    flag_s = 0;
  }


  if (flag_s == 1) {
    s2++;
    s1 = 0;
  }

  if (s2 > 5) {
    s1 = 0;
    s2 = 0;
    m1 += 1;
    m += 1;
  } else if (m1 > 9) {
    m1 = 0;
    s1 = 0;
    s2 = 0;
    m2++;
  } else if (m2 > 5) {
    m = 0;
    m1 = 0;
    m2 = 0;
    s1 = 0;
    s2 = 0;
    h += 1;
  } else if (h > 23) {
    h = 0;
    h1 = 0;
    h2 = 0;
    m1 = 0;
    m2 = 0;
    s1 = 0;
    s2 = 0;
  }

  for (int step = 0; step < 15; step++) {
    array[step] = array_str[step];
    array[step + 40] = array_time[step];
  }

  array_time[7] = s1;
  array_time[6] = s2;
  array_time[4] = m1;
  array_time[3] = m2;
  array_time[1] = h1;
  array_time[0] = h2;

  lcd2('0' + array[i]);
  i += 1;
  if (i == 80) {
    i = 0;
  }
  if (s > 60) {
    digitalWrite(2, HIGH);
    array_time[9] = 35;
    array_time[10] = 36;
    array_time[11] = 31;
    array_time[12] = 32;

  } else if (val == 0) {
    digitalWrite(2, LOW);
  }
  delay(10);

}
