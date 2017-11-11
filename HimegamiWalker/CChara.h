#pragma once

#include <DxLib.h>

#define CHARA_MAX_IDLE 10	// 待機状態画像ハンドル最大
#define CHARA_MAX_WALK 10	// 歩行状態画像ハンドル最大

enum CCHARA_PHASE
{

	PHASE_NONE,		// 未設定
	PHASE_IDLE,		// 待機
	PHASE_WALK,		// 歩行

	COUNT,			// 状態数

};

class CChara
{

public:

	CChara();
	~CChara();

	void Enable();		// 有効化

	void Update();		// 更新処理
	void Render();		// 描画処理

	void Move();		// 現在の単位速度で移動する

	int LoadTexMain(char* path, bool normalize = false);	// 画像読み込み
	int AddTexIdle(char* path, bool normalize = false);		// 画像読み込み
	int AddTexWalk(char* path, bool normalize = false);		// 画像読み込み

	int DivTexToIdle(int x, int y, int w, int h);	// 画像分割
	int DivTexToWalk(int x, int y, int w, int h);	// 画像分割

private:

	int ConvertNormalizedAlphaImage(char* path);	// α値二値化

public:

	// フラグ
	bool flg_loop_x = false;	// xでループするか
	bool flg_loop_y = false;	// yでループするか
	bool flg_pick = false;		// 掴み状態か

	// 座標
	float pos_x = 0;
	float pos_y = 0;

	// 速度
	float speed_x = 0;
	float speed_y = 0;

	// 衝突判定サイズ
	int collision_w = 0;
	int collision_h = 0;

	// 描画スケール
	float render_scale_w = 1.f;
	float render_scale_h = 1.f;

	// 移動エリア
	int area_w = 1920;
	int area_h = 1080;

	// 画像アニメーションフレーム数
	int animation_idle = 10;
	int animation_walk = 10;

private:

	// 状態
	bool m_enable = false;
	int m_frame = 0;
	int m_phase = 0;
	int m_wait = 0;

	// 画像ハンドル
	int m_handleMain = -1;				// ハンドル : 分割用
	int m_handleIdle[CHARA_MAX_IDLE];	// ハンドル : 待機状態
	int m_handleWalk[CHARA_MAX_WALK];	// ハンドル : 歩行状態

	// 画像ハンドルカウント
	int m_countIdle = 0;				// ハンドルカウント : 待機状態
	int m_countWalk = 0;				// ハンドルカウント : 歩行状態

	// 画像サイズ (テクスチャ)
	int m_tex_w = 0;
	int m_tex_h = 0;

};