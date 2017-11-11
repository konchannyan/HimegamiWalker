#pragma once

#include <DxLib.h>

#define CHARA_MAX_IDLE 10	// �ҋ@��ԉ摜�n���h���ő�
#define CHARA_MAX_WALK 10	// ���s��ԉ摜�n���h���ő�

enum CCHARA_PHASE
{

	PHASE_NONE,		// ���ݒ�
	PHASE_IDLE,		// �ҋ@
	PHASE_WALK,		// ���s

	COUNT,			// ��Ԑ�

};

class CChara
{

public:

	CChara();
	~CChara();

	void Enable();		// �L����

	void Update();		// �X�V����
	void Render();		// �`�揈��

	void Move();		// ���݂̒P�ʑ��x�ňړ�����

	int LoadTexMain(char* path, bool normalize = false);	// �摜�ǂݍ���
	int AddTexIdle(char* path, bool normalize = false);		// �摜�ǂݍ���
	int AddTexWalk(char* path, bool normalize = false);		// �摜�ǂݍ���

	int DivTexToIdle(int x, int y, int w, int h);	// �摜����
	int DivTexToWalk(int x, int y, int w, int h);	// �摜����

private:

	int ConvertNormalizedAlphaImage(char* path);	// ���l��l��

public:

	// �t���O
	bool flg_loop_x = false;	// x�Ń��[�v���邩
	bool flg_loop_y = false;	// y�Ń��[�v���邩
	bool flg_pick = false;		// �͂ݏ�Ԃ�

	// ���W
	float pos_x = 0;
	float pos_y = 0;

	// ���x
	float speed_x = 0;
	float speed_y = 0;

	// �Փ˔���T�C�Y
	int collision_w = 0;
	int collision_h = 0;

	// �`��X�P�[��
	float render_scale_w = 1.f;
	float render_scale_h = 1.f;

	// �ړ��G���A
	int area_w = 1920;
	int area_h = 1080;

	// �摜�A�j���[�V�����t���[����
	int animation_idle = 10;
	int animation_walk = 10;

private:

	// ���
	bool m_enable = false;
	int m_frame = 0;
	int m_phase = 0;
	int m_wait = 0;

	// �摜�n���h��
	int m_handleMain = -1;				// �n���h�� : �����p
	int m_handleIdle[CHARA_MAX_IDLE];	// �n���h�� : �ҋ@���
	int m_handleWalk[CHARA_MAX_WALK];	// �n���h�� : ���s���

	// �摜�n���h���J�E���g
	int m_countIdle = 0;				// �n���h���J�E���g : �ҋ@���
	int m_countWalk = 0;				// �n���h���J�E���g : ���s���

	// �摜�T�C�Y (�e�N�X�`��)
	int m_tex_w = 0;
	int m_tex_h = 0;

};