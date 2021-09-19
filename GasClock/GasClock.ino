int shift_ds = 2;
int shift_st_cp = 3;
int shift_sh_cp = 4;
int shift_mr = 5;
int photo_trans = 6;
int sssr_vcc = 7;
int sssr_1 = 8;
int sssr_2 = 9;
int sssr_4 = 10;
int sssr_8 = 11;

char str [8] = {'0',',','3','3','7','1','8','7'};

void setup()
{
  Serial.begin(115200);
  
  pinMode(shift_ds, OUTPUT);
  pinMode(shift_st_cp, OUTPUT);
  pinMode(shift_sh_cp, OUTPUT);
  pinMode(shift_mr, OUTPUT);
  pinMode(photo_trans, OUTPUT);
  pinMode(sssr_vcc, OUTPUT);
  pinMode(sssr_1, OUTPUT);
  pinMode(sssr_2, OUTPUT);
  pinMode(sssr_4, OUTPUT);
  pinMode(sssr_8, OUTPUT);
}



void loop()
{
  if(Serial.available() > 0)
  {
    String shitStr = Serial.readStringUntil('\n');
    if(shitStr.length() != 8)
    return;
    for(int j = 0; j < 8; j++)
    {
      if(shitStr[j] == NULL)
        str[j] = ',';
      else
        str[j] = shitStr[j];
    }
  }
  
  digitalWrite(shift_mr, HIGH);

    //output to gas clock
    for(int i = 0; i < 8; i++)
    {
      int strNum = str[i] - '0';
//      if(strNum < 0 || strNum > 9)
//        strNum = 0;

      SetShiftValues(1 << i);
      digitalWrite(photo_trans, LOW);
      if(strNum < 0 || strNum > 9)
//      if(str[i] == ',')
      {
        SetSSSRLow();
       digitalWrite(photo_trans, HIGH);
      }
      else
      {
        SetSSSRValues(strNum);
//      SetSSSRLow();
      }
      delay(2);
    }
}

void SetShiftValues(int v)
{
  digitalWrite(shift_st_cp, LOW);
  for(int i = 7; i >= 0; i--)
  {
    digitalWrite(shift_sh_cp, LOW);
    digitalWrite(shift_ds, v & (1 << i));
    digitalWrite(shift_sh_cp, HIGH);
  }
  digitalWrite(shift_st_cp, HIGH);
  digitalWrite(shift_st_cp, LOW);
}

void SetSSSRLow()
{
  digitalWrite(sssr_1, LOW);
  digitalWrite(sssr_2, LOW);
  digitalWrite(sssr_4, LOW);
  digitalWrite(sssr_8, LOW);
  digitalWrite(sssr_vcc, LOW);  
}

void SetSSSRValues(int num)
{
  SetSSSRLow();
  if(num == 0)
  {
    digitalWrite(sssr_vcc, HIGH);
    return;
  }
  else if(num == 1)
  {
    digitalWrite(sssr_vcc, HIGH);
    digitalWrite(sssr_1, HIGH);
    return;
  }
  else if(num == 2)
  {
    digitalWrite(sssr_vcc, HIGH);
    digitalWrite(sssr_2, HIGH);
    return;
  }
  else if(num == 3)
  {
    digitalWrite(sssr_vcc, HIGH);
    digitalWrite(sssr_1, HIGH);
    digitalWrite(sssr_2, HIGH);
    return;
  }
  else if(num == 4)
  {
    digitalWrite(sssr_vcc, HIGH);
    digitalWrite(sssr_4, HIGH);
    return;
  }
  else if(num == 5)
  {
    digitalWrite(sssr_vcc, HIGH);
    digitalWrite(sssr_1, HIGH);
    digitalWrite(sssr_4, HIGH);
    return;
  }
  else if(num == 6)
  {
    digitalWrite(sssr_vcc, HIGH);
    digitalWrite(sssr_2, HIGH);
    digitalWrite(sssr_4, HIGH);
    return;
  }
  else if(num == 7)
  {
    digitalWrite(sssr_vcc, HIGH);
    digitalWrite(sssr_1, HIGH);
    digitalWrite(sssr_2, HIGH);
    digitalWrite(sssr_4, HIGH);
    return;
  }
  else if(num == 8)
  {
    digitalWrite(sssr_vcc, HIGH);
    digitalWrite(sssr_8, HIGH);
    return;
  }
  else if(num == 9)
  {
    digitalWrite(sssr_vcc, HIGH);
    digitalWrite(sssr_1, HIGH);
    digitalWrite(sssr_8, HIGH);
    return;
  }
}
