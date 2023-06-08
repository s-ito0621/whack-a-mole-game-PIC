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

void main(){                            
    Pic();                              //PICの初期設定
    intrInit();                         //タイマー0割り込み設定
    int n,m,l,d,i;
    unsigined char num[20];
    num[0] = 0b00111111;                //0~9を表示
    num[1] = 0b00000110;
    num[2] = 0b01011011;
    num[3] = 0b01001111;
    num[4] = 0b01100110;
    num[5] = 0b01101101;
    num[6] = 0b01111101;
    num[7] = 0b00100111;
    num[8] = 0b01111111;
    num[9] = 0b01101111;
    
    num[10] = 0b10111111;               //0. ~ 9. を表示 　小数点付き
    num[11] = 0b10000110;
    num[12] = 0b11011011;
    num[13] = 0b11001111;
    num[14] = 0b11100110;
    num[15] = 0b11101101;
    num[16] = 0b11111101;
    num[17] = 0b10100111;
    num[18] = 0b11111111;
    num[19] = 0b11101111;

    while(1){
        PORTA = 0b00001110;             //startの状態(30.0)を表示
        PORTB = num[0];
        __delay_ms(1);
        PORTA = 0b00001101;
        PROTB = num[10];
        __delay_ms(1);
        PORTA = 0b00001011;
        PORTB = num[3];
        __delay_ms(1);
    }
    /*スタートの処理
    0番のタクトスイッチが押されRA5の入力がOFFになる時に開始する*/
    if(RA5 ==0){                        //チャタリング対策で微小な時間中ずっとOFFになっているときにスタート処理をする
        int tmp = 0;
        for (int k = 0;k<100;k++){
            if(RA5 ==0){
                tmp++;
            }
        }
        
        if(tmp == 100){                 //上記の処理をクリアした場合に，カウントスタート
            timer = 0;                  //カウントを初期化して数え始める
            intr_conter = 0;
            int count = 0;
            while (1)
            {
                d = 300-timer;          //30秒からカウントダウン
                l = d/100;              //タイマー表示の１０の位
                m = d/10 -l*10;         //タイマー表示の1の位
                n = d%10;               //タイマー表示の少数第１位の位

                if(l!=0){               //30.0~10.0秒の時の表示
                    PORTA = 0b00001110; //ダイナミック転倒
                    PORTB = num[n];
                    __delay_ms(1);
                    PORTA = 0b00001101;
                    PORTB =num[m+10];
                    __delay_ms(1);
                    PORTA = 0b00001011;
                    PORTB = num[l];
                    __delay_ms(1);
                }
                else if(l == 0 && d != 0){  //9.9~0.0秒の時の表示
                    PORTA = 0b00001110;     //ダイナミック点灯
                    PORTB = num[n];
                    __delay_ms(1);
                    PORTA = 0b00001101;
                    PORTB = num[m+10]
                    __delay_ms(1);
                }
                if (d ==0)break;            //カウントダウンが終了したらbreakして次の処理へ
            }
            PORTA = 0b01011100;             //ゲーム終了のブザーを3秒鳴らす
            __delay_ms(3000);   

            while(1){                       //点数のカウント．表示をする
                l = count/100;              //点数の３桁目の数
                m = count/10 -l*10;         //点数の2桁目の数
                n = count%10;               //点数の1桁目の数
                PORTA = 0b00001110;         //ダイナミック点灯で点数表示
                PORTB = num[n];
                __delay_ms(1);
                PORTA = 0b00001101;
                PORTB = num[m];
                __delay_ms(1);
                PORTA = 0b00001011;
                PORTB =num[l]
                __delay_ms(1);


                /*点数のカウント処理
                モグラたたきの点数はもう１つのPICでカウントしており，最後に結果が送られてくるのでその処理を行う．点数の回数だけもう１つのPICでRA5への入力をOFFにする*/
                if(RA5 == 0){                       //点数の送信によって入力がOFFになったかの確認の処理                        
                    int tmp2 = 0;
                    while(1){
                        for(int k = 0; k<100; k++){
                            if(RA5 ==0){            //チャタリング対策
                                tmp2++;
                            }
                        }
                        while(1){
                            if(RA5 == 1)break;      //入力がOFFじゃない(点数を受け取り終わったら)次の処理
                        }
                        if(tmp2 = 100){             //上の処理をクリアすると点数を1つ数える
                            count++;
                            break;
                        }else{                      //処理をクリアしてない場合，点数をすべて受け取り終わったのでbreakする
                            break;
                        }
                    }
                }
                if(timer >= 400)break;              //上の点数の送信，カウント処理はブザーが鳴った後の7秒間続ける
            }
        }

    }

}