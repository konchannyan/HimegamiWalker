
//**************************************************************
//　インクルードファイル
//**************************************************************
#include <DxLib.h>
#include <math.h>
#include "key.h"
#include "CChara.h"


//**************************************************************
//　マクロ定義
//**************************************************************
#define CHARA_NUM 20
#define CONFIG_FILE_PATH "./config.txt"
#define HIMEGAMI_FILE_PATH "./himegami.txt"

//**************************************************************
//　名前空間
//**************************************************************
using namespace DxLib;


// メモ
//	VS2017環境
//	debugは動作が不安定
//	himegami.txtの書き替え必須


//**************************************************************
//　大域変数
//**************************************************************

// ウィンドウサイズ
int g_window_size_w = 1920;
int g_window_size_h = 1080;

// マルチディスプレイのID
int g_multi_display_id = 0;

// ウィンドウ透過色の指定 (この色は表示されません)
int g_trans_r = 0;
int g_trans_g = 254;
int g_trans_b = 0;

// マウス座標関連
int g_mouse_x;			// マウス座標
int g_mouse_y;			// マウス座標
int g_mouse_move_x;		// マウス移動量
int g_mouse_move_y;		// マウス移動量

// マウス入力関連
int g_mouse_left = 0;	// 左クリック 入力フレーム (==1でトグル動作)
int g_mouse_right = 0;	// 右クリック 入力フレーム (==1でトグル動作)
int g_mouse_center = 0;	// 中クリック 入力フレーム (==1でトグル動作)

// キャラ
CChara g_chara[CHARA_NUM];

//**************************************************************
//　デコード
//**************************************************************
void Decode(char* src, char* head, char* body)
{

	while (*src && *src != ' ' && *src != '\t')
		*head++ = *src++;

	*head = '\0';

	while (*src && (*src == ' ' || *src == '\t'))
		src++;

	if (!*src)
		return;

	while (*src && *src != ' ' && *src != '\n' && *src != '\t')
		*body++ = *src++;

	*body = '\0';

}

//**************************************************************
//　設定ファイルのデコード
//**************************************************************
void DecodeConfig(char* str)
{

	char head[256];
	char body[256];
	
	Decode(str, head, body);

	if (!strcmp(head, "WINDOW_W"))
		g_window_size_w = atoi(body);
	
	else if (!strcmp(head, "WINDOW_H"))
		g_window_size_h = atoi(body);

	else if (!strcmp(head, "MULTI_DISPLAY_ID"))
		g_multi_display_id = atoi(body);

	else if (!strcmp(head, "TRANS_COLOR_R"))
		g_trans_r = atoi(body);

	else if (!strcmp(head, "TRANS_COLOR_G"))
		g_trans_g = atoi(body);

	else if (!strcmp(head, "TRANS_COLOR_B"))
		g_trans_b = atoi(body);
	
}

//**************************************************************
//　ひめがみファイルのデコード (戻り値 : 読みこんだ数)
//**************************************************************
int DecodeHimegamiFile()
{

	int i = 0;

	char head[256];
	char body[256];

	float scale = 0.f;

	FILE *fp = nullptr;
	fopen_s(&fp, HIMEGAMI_FILE_PATH, "r");
	if (fp) {
		
		int line = 0;
		char str[1024];

		while (fgets(str, sizeof(str), fp))
		{
		
			line++;

			Decode(str, head, body);

			if (!strcmp(head, "END"))
				i++;

			else if (!strcmp(head, "HIMEGAMI")) {
				scale = 1.f;
				g_chara[i].area_w = g_window_size_w;
				g_chara[i].area_h = g_window_size_h;
			}

			else if (!strcmp(head, "SCALE")) {
				scale = atof(body);
				g_chara[i].render_scale_w = scale;
				g_chara[i].render_scale_h = scale;
			}

			else if (!strcmp(head, "COLLISION_W"))
				g_chara[i].collision_w = atoi(body) * scale;

			else if (!strcmp(head, "COLLISION_H"))
				g_chara[i].collision_h = atoi(body) * scale;

			else if (!strcmp(head, "ADD_IDLE"))
				g_chara[i].AddTexIdle(body, true);

			else if (!strcmp(head, "ADD_WALK"))
				g_chara[i].AddTexWalk(body, true);

			else if (!strcmp(head, "ANIMATION_FRAME_IDLE"))
				g_chara[i].animation_idle = atoi(body);

			else if (!strcmp(head, "ANIMATION_FRAME_WALK"))
				g_chara[i].animation_walk = atoi(body);
		
		}

		fclose(fp);

	}

	return i;

}

//**************************************************************
//　WinMain
//**************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//************************************************
	//　初期化設定ファイルの読み込み
	//************************************************
	FILE *fp = nullptr;
	fopen_s(&fp, CONFIG_FILE_PATH, "r");
	if (fp) {
		char str[1024];
		while (fgets(str, sizeof(str), fp))
			DecodeConfig(str);
		fclose(fp);
	}

	//************************************************
	//　DxLib 初期化
	//************************************************

	// DxLib 初期化 前設定
	SetOutApplicationLogValidFlag(false);					// ログを生成しない
	SetMainWindowText("Himegami Walker");					// ウインドウのタイトル
	ChangeWindowMode(true);									// ウインドウモード
	SetGraphMode(g_window_size_w, g_window_size_h, 32);		// 画面サイズ
	SetWindowStyleMode(2);									// スタイル
	SetUseBackBufferTransColorFlag(true);					// ウィンドウを透明にします
	SetWindowSizeChangeEnableFlag(false);					// サイズ変更の許可	
	SetAlwaysRunFlag(true);									// フォーカスが無くても動作
	SetTransColor(g_trans_r, g_trans_g, g_trans_b);			// 透過色 
	SetBackgroundColor(g_trans_r, g_trans_g, g_trans_b);	// 背景色
	SetUseDisplayIndex(g_multi_display_id);					// マルチディスプレイの指定
	SetDoubleStartValidFlag(true);							// 多重起動の許可

	// DxLib 初期化
	if (DxLib_Init() == -1) return -1;	// 初期化 
	SetDrawScreen(DX_SCREEN_BACK);		// 裏画面に描画

	// ウィンドウの最前面化
	auto whnd = GetMainWindowHandle();	// ウィンドウのハンドル
	SetWindowPos(whnd, HWND_TOPMOST,	// HWND_TOPMOST -> HWND_NOTOPMOST で最前面化解除
		0, 0, 0, 0,						// 座標・サイズ変更しない
		SWP_NOMOVE | SWP_NOSIZE);		// 座標(SWP_NOMOVE)・サイズ(SWP_NOSIZE)


	//************************************************
	//　リソース読みこみ
	//************************************************

	int count_chara = DecodeHimegamiFile();

	for (int i = 0; i < count_chara; i++)
	{
		g_chara[i].pos_x = GetRand(g_window_size_w - g_chara[i].collision_w) + g_chara[i].collision_w / 2;
		g_chara[i].pos_y = GetRand(g_window_size_h - g_chara[i].collision_h) + g_chara[i].collision_h / 2;
		g_chara[i].speed_x = 5;
		g_chara[i].Enable();
	}

	int pick_index = -1;

	//************************************************
	//　メインループ
	//************************************************
	int frame = 0;	// 総フレーム数
	bool flg_loop = true;
	while (flg_loop && ProcessMessage() == 0)
	{
		
		frame++;

		// マウス
		{

			// 座標
			int mouse_x, mouse_y;
			GetMousePoint(&mouse_x, &mouse_y);
			g_mouse_move_x = mouse_x - g_mouse_x;
			g_mouse_move_y = mouse_y - g_mouse_y;
			g_mouse_x = mouse_x;
			g_mouse_y = mouse_y;

			// クリック
			g_mouse_left = GetMouseInput() & MOUSE_INPUT_LEFT ? g_mouse_left + 1 : 0;
			g_mouse_right = GetMouseInput() & MOUSE_INPUT_RIGHT ? g_mouse_right + 1 : 0;
			g_mouse_center = GetMouseInput() & MOUSE_INPUT_MIDDLE ? g_mouse_center + 1 : 0;

		}

		// キー入力更新
		ReGetKey();

		// Esc -> 終了
		if (GetKey(0) == 1)
			flg_loop = false;	// or break


		// pick end
		if (g_mouse_left == 0 && pick_index != -1) {
			g_chara[pick_index].flg_pick = false;
			pick_index = -1;
		}

		// pick up
		if (g_mouse_left == 1)
		{
			float best_index = -1;
			float best_rad = -1;
			const float max_rad = 100;
			for (int i = 0; i < count_chara; i++) {
				float dx = g_chara[i].pos_x - g_mouse_x;
				float dy = g_chara[i].pos_y - g_mouse_y;
				float dr = sqrt(dx * dx + dy * dy);
				if (dr < max_rad) {
					if (best_index == -1) {
						best_index = i;
						best_rad = dr;
					}
					else {
						if (best_rad > dr) {
							best_index = i;
							best_rad = dr;
						}
					}
				}
			}
			pick_index = best_index;
			g_chara[pick_index].flg_pick = true;
		}

		// pick move
		if (pick_index != -1) {
			g_chara[pick_index].pos_x += g_mouse_move_x;
			g_chara[pick_index].pos_y += g_mouse_move_y;
		}

		// update & render
		for (int i = 0; i < count_chara; i++) {
			g_chara[i].Update();
			g_chara[i].Render();
		}

		// swap & clear
		ScreenFlip();
		ClearDrawScreen();

	}


	//************************************************
	//　DxLib 解放
	//************************************************
	DxLib_End();

	return 0;

}