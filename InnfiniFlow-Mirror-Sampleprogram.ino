#include <M5Atom.h>
#include <Adafruit_NeoPixel.h>
#define PIN        26  // LEDへの信号端子の番号
#define NUMPIXELS  180  // Infiniflow mirrorに使用しているLEDの数
int brightness = 60; // LEDの明るさ (0-255の間で設定) 明るくして同時に光るLEDが多いと合計の電流が数アンペアになります、電源の電流容量に注意してください


//変数などの設定---------------------------------------------
//M5Atom本体ボタンによるステータス変化用変数
int mode =0;

// NeoPixel初期設定
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//LEDの順序をInfiniflow mirrorで実際に見える順序に並び替えるための変数
int LEDline[] = {0, 2, 4, 5, 3, 1};

int LEDpatConvert[] = {0,5,1,4,2,3};
int sinadjust[6][6] = {
  {0,5,1,4,2,3},
  {1,0,2,5,3,4},
  {2,1,3,0,4,5},
  {3,2,4,1,5,0},
  {4,3,5,2,0,1},
  {5,4,0,3,1,2}
};

//表示パターンA
int patA[6][6]={
  {0,0,1,0,0,0},
  {0,1,1,1,0,0},
  {1,1,1,1,1,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0},
  {0,0,0,0,0,0}
};

//表示パターンB
int patB[6][6]={
  {0,0,0,0,0,1},
  {0,0,0,0,1,0},
  {0,0,0,1,0,0},
  {0,0,1,0,0,0},
  {0,1,0,0,0,0},
  {1,0,0,0,0,0}
};

//sinパターンをいれるための箱
float sinpat[6] = {0,0,0,0,0,0};

//Setup---------------------------------------------
void setup() {
  // 本体初期化（UART有効, I2C無効, 本体LED有効）
  M5.begin(true, false, false);
  //Serial.begin(9600);
  //while (!Serial);
  //Serial.println("ready");   
  // LEDを初期化
  pixels.begin();
  pixels.clear();
  pixels.show();
  pixels.setBrightness(brightness);

}

//Loop---------------------------------------------
void loop() {
  pixels.clear(); // すべてのLEDの色を0にセット
  M5.update();  //本体のボタン状態更新
  if (M5.Btn.isPressed()) { //本体のボタンを押すとmodeが切り替わる
    if (mode == 0){mode = 1;} 
    else if (mode == 1){ mode = 2;}
    else if (mode == 2){ mode = 3;}
    else if (mode == 3){ mode = 4;}
    else if (mode == 4){ mode = 5;}
    else if (mode == 5){ mode = 6;}
    else {mode = 0;}
  }


  if (mode == 0){
    for(int n=0; n<6; n++) {
    pixels.clear();
    for(int i=0; i<NUMPIXELS; i++){
      if (i % 6 == LEDline[n]) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 255));
        } else {
        }
    }
    pixels.show();
    delay(100);
    }

  } else if (mode == 1) {//6x6のパターンを並べて奥行きスクロール
      for(int n=0; n<6; n++) {
      pixels.clear();
      for(int i=0; i<NUMPIXELS; i++){
        pixels.setPixelColor(i, pixels.Color(0, 0, patA[(LEDpatConvert[i%6]+n)%6][(i/6)%6] *255));//パターンAを表示、ここをいじると色が変えられます
      }
    pixels.show();
    delay(100);
    }
  } else if (mode == 2){//6x6のパターンを並べて横スクロール
      for(int n=0; n<6; n++) {
      pixels.clear();
      for(int i=0; i<NUMPIXELS; i++){
          pixels.setPixelColor(i, pixels.Color(0, patB[LEDpatConvert[i%6]][(i/6+n+1)%6] *255, patB[LEDpatConvert[i%6]][(i/6+n)%6] *255));//パターンBを一つずらして重ね合わせ
        }
    pixels.show();
    delay(100);
    }

  } else if (mode == 3){//明るさをsin波で表示するモード
      for(int n=0; n<18; n++) {//sinカーブを18分割してずらして表示させる
      pixels.clear();
      float rad;
      rad = (float)n;
      float rad2;
      rad2 = rad*0.346;//(sin((0+rad)/18*6.24)を簡略化したもの
      //計算されたsinの値に1を足して正にし、自乗して明暗の差を大きくしている
      sinpat[0] = ((sin(rad2)+1)*(sin(rad2)+1))*64;
      sinpat[2] = ((sin(rad2+1.04)+1)*(sin(rad2+1.04)+1))*64;
      sinpat[4] = ((sin(rad2+2.08)+1)*(sin(rad2+2.08)+1))*64;
      sinpat[5] = ((sin(rad2+3.12)+1)*(sin(rad2+3.12)+1))*64;
      sinpat[3] = ((sin(rad2+4.19)+1)*(sin(rad2+4.19)+1))*64;
      sinpat[1] = ((sin(rad2+5.23)+1)*(sin(rad2+5.23)+1))*64;

      for(int i=0; i<NUMPIXELS; i++){
        pixels.setPixelColor(i, pixels.Color(0, 0, (int)sinpat[i%6]));

        }
    pixels.show();
    delay(40);
    }
  } else if (mode == 4){//明るさをsin波で表示するモード、映画のマトリクスのような表現
for(int n=0; n<18; n++) {//sinカーブを18分割してずらして表示させる
      pixels.clear();
      float rad;
      rad = (float)n;
      float rad2;
      rad2 = rad*0.346;//(sin((0+rad)/18*6.24)を簡略化したもの
      //計算されたsinの値に1を足して正にし、自乗して明暗の差を大きくしている
      sinpat[0] = ((sin(rad2)+1)*(sin(rad2)+1))*64;
      sinpat[2] = ((sin(rad2+1.04)+1)*(sin(rad2+1.04)+1))*64;
      sinpat[4] = ((sin(rad2+2.08)+1)*(sin(rad2+2.08)+1))*64;
      sinpat[5] = ((sin(rad2+3.12)+1)*(sin(rad2+3.12)+1))*64;
      sinpat[3] = ((sin(rad2+4.19)+1)*(sin(rad2+4.19)+1))*64;
      sinpat[1] = ((sin(rad2+5.23)+1)*(sin(rad2+5.23)+1))*64;

      for(int i=0; i<NUMPIXELS; i++){
        pixels.setPixelColor(i, pixels.Color(0,  (int)sinpat[(i%6+i/6)%6],0));
        }
    pixels.show();
    delay(40);
    }
  
  } else if (mode == 5){//明るさをsin波で表示するモード、スパイラル
for(int n=0; n<18; n++) {//sinカーブを18分割してずらして表示させる
      pixels.clear();
      float rad;
      rad = (float)n;
      float rad2;
      rad2 = rad*0.346;//(sin((0+rad)/18*6.24)を簡略化したもの
      //計算されたsinの値に1を足して正にし、自乗して明暗の差を大きくしている
      sinpat[0] = ((sin(rad2)+1)*(sin(rad2)+1))*64;
      sinpat[1] = ((sin(rad2+1.04)+1)*(sin(rad2+1.04)+1))*64;
      sinpat[2] = ((sin(rad2+2.08)+1)*(sin(rad2+2.08)+1))*64;
      sinpat[3] = ((sin(rad2+3.12)+1)*(sin(rad2+3.12)+1))*64;
      sinpat[4] = ((sin(rad2+4.19)+1)*(sin(rad2+4.19)+1))*64;
      sinpat[5] = ((sin(rad2+5.23)+1)*(sin(rad2+5.23)+1))*64;

      for(int i=0; i<NUMPIXELS; i++){
        pixels.setPixelColor(i, pixels.Color(0,0,(int)sinpat[sinadjust[(i/6)%6][i%6]])); 
        }
    pixels.show();
    delay(40);
    }
  
  }
  
  else{//1列だけの表示テスト
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 20, 20));
    pixels.setPixelColor(2, pixels.Color(0, 50, 50));
    pixels.setPixelColor(3, pixels.Color(0, 128, 128));
    pixels.setPixelColor(4, pixels.Color(0, 200, 200));
    pixels.setPixelColor(5, pixels.Color(0, 255, 255));


  pixels.show();
  delay(2000);
  }


}
