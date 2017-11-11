
/*
******************************
	キー入力管理
	
	10000frame→9999frame
	0	:esc
	1	:shift
	2	:ctrl
	3	:space

	10	:↑
	11	:←
	12	:↓
	13	:→

	20	:0
	21	:1
	22	:2
	23	:3
	24	:4
	25	:5
	26	:6
	27	:7
	28	:8
	29	:9

	30	:a
	31	:b
	32	:c
	33	:d
	34	:e
	35	:f
	36	:g
	37	:h
	38	:i
	39	:j
	40	:k
	41	:l
	42	:m
	43	:n
	44	:o
	45	:p
	46	:q
	47	:r
	48	:s
	49	:t
	50	:u
	51	:v
	52	:w
	53	:x
	54	:y
	55	:z


******************************
*/

#include <DxLib.h>

#include "key.h"

char KeyBuf[256];
int Key[256];


/*
	キー入力状態更新(最大１万frameまで)
*/
void ReGetKey(void)
{
	GetHitKeyStateAll(KeyBuf);

	for(int i = 0;i<256;i++){
		int n = 0;
		switch(i){
		case  0:n = KeyBuf[KEY_INPUT_ESCAPE];
			break;
		case  1:n = KeyBuf[KEY_INPUT_LSHIFT];
			break;
		case  2:n = KeyBuf[KEY_INPUT_LCONTROL];
			break;
		case  3:n = KeyBuf[KEY_INPUT_SPACE];
			break;

		case 10:n = KeyBuf[KEY_INPUT_UP];
			break;
		case 11:n = KeyBuf[KEY_INPUT_LEFT];
			break;
		case 12:n = KeyBuf[KEY_INPUT_DOWN];
			break;
		case 13:n = KeyBuf[KEY_INPUT_RIGHT];
			break;

		case 20:n = KeyBuf[KEY_INPUT_NUMPAD0];
			break;
		case 21:n = KeyBuf[KEY_INPUT_NUMPAD1];
			break;
		case 22:n = KeyBuf[KEY_INPUT_NUMPAD2];
			break;
		case 23:n = KeyBuf[KEY_INPUT_NUMPAD3];
			break;
		case 24:n = KeyBuf[KEY_INPUT_NUMPAD4];
			break;
		case 25:n = KeyBuf[KEY_INPUT_NUMPAD5];
			break;
		case 26:n = KeyBuf[KEY_INPUT_NUMPAD6];
			break;
		case 27:n = KeyBuf[KEY_INPUT_NUMPAD7];
			break;
		case 28:n = KeyBuf[KEY_INPUT_NUMPAD8];
			break;
		case 29:n = KeyBuf[KEY_INPUT_NUMPAD9];
			break;

		case 30:n = KeyBuf[KEY_INPUT_A];
			break;
		case 31:n = KeyBuf[KEY_INPUT_B];
			break;
		case 32:n = KeyBuf[KEY_INPUT_C];
			break;
		case 33:n = KeyBuf[KEY_INPUT_D];
			break;
		case 34:n = KeyBuf[KEY_INPUT_E];
			break;
		case 35:n = KeyBuf[KEY_INPUT_F];
			break;
		case 36:n = KeyBuf[KEY_INPUT_G];
			break;
		case 37:n = KeyBuf[KEY_INPUT_H];
			break;
		case 38:n = KeyBuf[KEY_INPUT_I];
			break;
		case 39:n = KeyBuf[KEY_INPUT_J];
			break;
		case 40:n = KeyBuf[KEY_INPUT_K];
			break;
		case 41:n = KeyBuf[KEY_INPUT_L];
			break;
		case 42:n = KeyBuf[KEY_INPUT_M];
			break;
		case 43:n = KeyBuf[KEY_INPUT_N];
			break;
		case 44:n = KeyBuf[KEY_INPUT_O];
			break;
		case 45:n = KeyBuf[KEY_INPUT_P];
			break;
		case 46:n = KeyBuf[KEY_INPUT_Q];
			break;
		case 47:n = KeyBuf[KEY_INPUT_R];
			break;
		case 48:n = KeyBuf[KEY_INPUT_S];
			break;
		case 49:n = KeyBuf[KEY_INPUT_T];
			break;
		case 50:n = KeyBuf[KEY_INPUT_U];
			break;
		case 51:n = KeyBuf[KEY_INPUT_V];
			break;
		case 52:n = KeyBuf[KEY_INPUT_W];
			break;
		case 53:n = KeyBuf[KEY_INPUT_X];
			break;
		case 54:n = KeyBuf[KEY_INPUT_Y];
			break;
		case 55:n = KeyBuf[KEY_INPUT_Z];
			break;
		}
		
		if(n != 0){
			Key[i] = Key[i] + 1;
			if(Key[i] == 10000)
				Key[i] = 9999;
		}
		else
			Key[i] = 0;
	}
}

/*
	キー入力状態取得
*/
int GetKey(int k)
{
	return Key[k];
}

/*
	キー同時入力数
*/
int GetKeyHitN(void)
{
	int x = 0;
	for(int i=0;i<256;i++)
		if(Key[i] == 1)
			x++;
	return x;
}


/*
******************************
	EOF key.cpp
******************************
*/