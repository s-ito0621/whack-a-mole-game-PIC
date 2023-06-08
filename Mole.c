#include <pic.h>
#include <xc.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define _LEGACY_HEADERS
#define _XTAL_FREQ 8000000
#pragma config FOSC = INTOSCIO
#pragma config WDTE = OFF
#pragma config PWRTE = ON 
#pragma config MCLRE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CCPMX = RB3
#pragma config CP = OFF
#pragma config FOMEN = OFF
#pragma config IESO = OFF

int timer = 0;
int intr_conter = 0;
unsigned int setrandom = 0;

/*タイマー0割り込み関数の動作周波数は8MHz 1命令サイクルは4クロック周期である． プリスケーラを32倍にしたので，0.08μsec*4*4*32=4.096msで1回呼び出される関数*/
static void __interrupt() isr(void){　  //タイマー0割り込み関数
    intr_conter++;
    if(intr_conter == 25){              //4.096ms*25 = 100ms →0.1秒毎にif文の内部処理
        timer++;                        //timer = 10 で1秒
        intr_conter = 0;
    }
    INTCONbits.TMR0IF = 0;

}

void intrInit(){                        //タイマー0割り込み設定の関数
    OPTION_REG = 0b10000100;            //プリスケーラを32に設定
    TMR0 = 0x00
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1
}

void Pic(){                             //PICの初期設定の関数
    OSCCON = 0b01110000;                //動作周波数8MHz
    TRISA = 0b0010000;                  //RA5のみ入力ピン 残りは出力ピン 
    TRISB = 0x00;                       //PORTBは出力ピン
    PORTA = 0x00;                       //PORTAは全てOFF状態
    PORTB = 0x00;                       //PORTBは全てOFF状態
    ANSEL = 0;                          //アナログ制御
}

/*LEDを点灯&LEDが光ったところのスイッチが押されたか確認する関数
0~5を引く数として渡すとそのLEDが点灯し，そのLEDが光っている間に対応するスイッチを押すとLEDが消えて1をreturnする
押せていなかったら0をreturn，つまりもぐらがたたけたら1点追加という処理を行う．
(スイッチのポート,LEDのポート)は(RA0,RB0),(RA1,RB1)，...(RA5,RB5)である
sw()は点灯時間が2s，点が入った後の消灯時間が1s
sw2()は点灯時間が1s,点が入った後の消灯時間が0.5s
sw3()は消灯時間が0.5s,点が入った後の消灯時間が0.25s
sw()→sw2(),sw3()になるにつれて，消灯のタイミングが早くなり難易度が上がる*/
int sw(int n){                      //LEDを点灯&LEDが光ったところのスイッチが押されたか確認する関数(難易度:easy)
    int i = 0;
    int count = 0;
    PORTB = 0x00;

    if(n == 0){                     //nが0の時
        int tmp0 = 0;
        RB0 = 1;                    //0番のLED点灯
        while(RB0 == 1){            //点灯中
            if(RA0 == 0){           //対応するスイッチが押されたら
                RB0 = 0;            //1秒消灯
                __delay_ms(1000);
                count++;            //加点する
            }
            else if(RA0 != 0){      //対応するスイッチが押されなかったら
                tmp0++;             //tmp0に1ms間隔で+1していく
                __delay_ms(1);
            }
            if (tmp0 > 1999)break;  //tmp0が2000以上，２秒間押されなかったらbreakする
            
        }
    }
    if(n ==1){                      //nが1のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB1 = 1;
        while(RB1 == 1){
            if(RA1 == 0){
                RB1 = 0;
                __delay_ms(1000);
                count++;
            }
            else if(RA1 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 1999)break;
            
        }
    }
    if(n ==2){                      //nが2のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB2 = 1;
        while(RB2 == 1){
            if(RA2 == 0){
                RB2 = 0;
                __delay_ms(1000);
                count++;
            }
            else if(RA2 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 1999)break;
            
        }
    }
    if(n ==3){                      //nが3のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB3 = 1;
        while(RB3 == 1){
            if(RA3 == 0){
                RB3 = 0;
                __delay_ms(1000);
                count++;
            }
            else if(RA3 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 1999)break;
            
        }
    }
    if(n ==4){                      //nが4のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB4 = 1;
        while(RB4 == 1){
            if(RA4 == 0){
                RB4 = 0;
                __delay_ms(1000);
                count++;
            }
            else if(RA4 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 1999)break;
            
        }
    }
    if(n ==5){                      //nが5のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB5 = 1;
        while(RB5 == 1){
            if(RA5 == 0){
                RB5 = 0;
                __delay_ms(1000);
                count++;
            }
            else if(RA5 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 1999)break;
            
        }
    }
    return count;
}

int sw2(int n){                      //LEDを点灯&LEDが光ったところのスイッチが押されたか確認する関数(難易度:normal)
    int i = 0;
    int count = 0;
    PORTB = 0x00;

    if(n == 0){                     //nが0の時
        int tmp0 = 0;
        RB0 = 1;                    //0番のLED点灯
        while(RB0 == 1){            //点灯中
            if(RA0 == 0){           //対応するスイッチが押されたら
                RB0 = 0;            //0.5秒消灯
                __delay_ms(500);
                count++;            //加点する
            }
            else if(RA0 != 0){      //対応するスイッチが押されなかったら
                tmp0++;             //tmp0に1ms間隔で+1していく
                __delay_ms(1);
            }
            if (tmp0 > 999)break;  //tmp0が1000以上，1秒間押されなかったらbreakする
            
        }
    }
    if(n ==1){                      //nが1のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB1 = 1;
        while(RB1 == 1){
            if(RA1 == 0){
                RB1 = 0;
                __delay_ms(500);
                count++;
            }
            else if(RA1 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 999)break;
            
        }
    }
    if(n ==2){                      //nが2のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB2 = 1;
        while(RB2 == 1){
            if(RA2 == 0){
                RB2 = 0;
                __delay_ms(500);
                count++;
            }
            else if(RA2 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 999)break;
            
        }
    }
    if(n ==3){                      //nが3のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB3 = 1;
        while(RB3 == 1){
            if(RA3 == 0){
                RB3 = 0;
                __delay_ms(500);
                count++;
            }
            else if(RA3 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 999)break;
            
        }
    }
    if(n ==4){                      //nが4のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB4 = 1;
        while(RB4 == 1){
            if(RA4 == 0){
                RB4 = 0;
                __delay_ms(500);
                count++;
            }
            else if(RA4 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 999)break;
            
        }
    }
    if(n ==5){                      //nが5のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB5 = 1;
        while(RB5 == 1){
            if(RA5 == 0){
                RB5 = 0;
                __delay_ms(500);
                count++;
            }
            else if(RA5 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 999)break;
            
        }
    }
    return count;
}

int sw3(int n){                      //LEDを点灯&LEDが光ったところのスイッチが押されたか確認する関数(難易度:hard)
    int i = 0;
    int count = 0;
    PORTB = 0x00;

    if(n == 0){                     //nが0の時
        int tmp0 = 0;
        RB0 = 1;                    //0番のLED点灯
        while(RB0 == 1){            //点灯中
            if(RA0 == 0){           //対応するスイッチが押されたら
                RB0 = 0;            //0.25秒消灯
                __delay_ms(250);
                count++;            //加点する
            }
            else if(RA0 != 0){      //対応するスイッチが押されなかったら
                tmp0++;             //tmp0に1ms間隔で+1していく
                __delay_ms(1);
            }
            if (tmp0 > 499)break;  //tmp0が500以上，1秒間押されなかったらbreakする
            
        }
    }
    if(n ==1){                      //nが1のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB1 = 1;
        while(RB1 == 1){
            if(RA1 == 0){
                RB1 = 0;
                __delay_ms(250);
                count++;
            }
            else if(RA1 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 499)break;
            
        }
    }
    if(n ==2){                      //nが2のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB2 = 1;
        while(RB2 == 1){
            if(RA2 == 0){
                RB2 = 0;
                __delay_ms(250);
                count++;
            }
            else if(RA2 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 499)break;
            
        }
    }
    if(n ==3){                      //nが3のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB3 = 1;
        while(RB3 == 1){
            if(RA3 == 0){
                RB3 = 0;
                __delay_ms(250);
                count++;
            }
            else if(RA3 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 499)break;
            
        }
    }
    if(n ==4){                      //nが4のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB4 = 1;
        while(RB4 == 1){
            if(RA4 == 0){
                RB4 = 0;
                __delay_ms(250);
                count++;
            }
            else if(RA4 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 499)break;
            
        }
    }
    if(n ==5){                      //nが5のときの処理　(nが0のときと同じ処理)
        int tmp0 = 0;
        RB5 = 1;
        while(RB5 == 1){
            if(RA5 == 0){
                RB5 = 0;
                __delay_ms(250);
                count++;
            }
            else if(RA5 != 0){
                tmp0++;
                __delay_ms(1);
            }
            if (tmp0 > 499)break;
            
        }
    }
    return count;
}

void main(void){                            //main関数
    int i,tmp;
    int count;

    intrInit();                             //タイマー0割り込み設定
    Pic();                                  //PICの初期設定

    RB7 = 1;                                //RB7 もう１つのPICとつながっているポートはON

    while(1){
        if(RA0 == 0){                       //0番のタクトスイッチが押されたらスタート
            count = 0;
            timer = 0;
            RB7 = 0;                        //RB7をOFFにしてもう１つのPICもスタートさせる
            srand(setrandom);               //seed値をsetrandomにする(setrandomは4.096msごとに+1されるunsigned int型)
            intr_counter = 0;               //時間のカウントスタート
            while(1){
                if(timer <= 100){           //スタートから10秒の処理(30~20秒)
                    i =rand() % 6;          //出現するモグラを選択
                    if(tmp == i) {          //2回連続で同じモグラになる場合，隣のモグラを選択
                        if(i != 5){
                            i = i+1;
                        }
                        else{
                            i = i-1;
                        }
                    }
                    count = count +sw(i);   //sw()にモグラの番号を渡し，点数のカウントを行う．
                    tmp = i;
                }
                if(timer > 100 && timer < 200){//以下スタートから10秒~20秒の処理
                    i =rand() % 6;
                    if(tmp = i){
                        if(i != 5){
                            i = i;1;
                        }else{
                            i = i-1;
                        }
                    }
                    count = count +sw2(i);
                    tmp = i;
                }
                if(timer > 200 && timer < 300){//以下スタートから20秒~30秒の処理
                    i =rand() % 6;
                    if(tmp = i){
                        if(i != 5){
                            i = i;1;
                        }else{
                            i = i-1;
                        }
                    }
                    count = count +sw3(i);
                    tmp = i;
                }
                if(timer >= 300)break;          //30秒経過で終了

            }
            PORTB = 0b10000000;
            while(1){
                if(timer >= 335)break;          //3.5秒松
            }
            for (int k = 0;k<count;k++){        //点数の回数だけPORTBをON/OFFを繰り返し，もう１つのPICへ送信
                PORTB = 0b00000000;
                __delay_ms(50);
                PORTB = 0b11111111;
                __delay_ms(50);
            }
            PORTB = 0b10000000;
            while(1){                           //送信し終えたら40秒経過するまで待つ
                if(timer >= 400)break;
            }
        }else{
            PORTB = 0b10000000;
        }
    }
}