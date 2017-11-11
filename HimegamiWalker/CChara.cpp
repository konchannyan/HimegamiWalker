
#include "CChara.h"

CChara::CChara()
{

}

CChara::~CChara()
{

	if (m_handleMain >= 0) {
		DeleteGraph(m_handleMain);
		m_handleMain = -1;
	}

	for (int i = 0; i < m_countIdle; i++)
		DeleteGraph(m_handleIdle[i]);
	m_countIdle = 0;

	for (int i = 0; i < m_countWalk; i++)
		DeleteGraph(m_handleWalk[i]);
	m_countWalk = 0;
	
}

void CChara::Enable()
{

	m_enable = true;
	m_phase = CCHARA_PHASE::PHASE_IDLE;
	m_wait = 1;

	if(m_countIdle > 0)
	GetGraphSize(m_handleIdle[0], &m_tex_w, &m_tex_h);

}

void CChara::Update()
{

	m_frame++;

	switch (m_phase)
	{

	case CCHARA_PHASE::PHASE_IDLE:

		break;

	case CCHARA_PHASE::PHASE_WALK:

		Move();
		break;

	default:

		break;

	}

	if (m_wait > 0) {

		m_wait--;
		if (m_wait == 0) {

			switch (m_phase)
			{

			case CCHARA_PHASE::PHASE_IDLE:

				m_wait = GetRand(120) + 60;
				m_phase = CCHARA_PHASE::PHASE_WALK;

				break;

			case CCHARA_PHASE::PHASE_WALK:

				m_wait = GetRand(60) + 60;
				m_phase = CCHARA_PHASE::PHASE_IDLE;

				break;

			default:

				break;

			}
		}

	}

}

void CChara::Render()
{

	int handle = -1;
	
	switch (m_phase)
	{

	case CCHARA_PHASE::PHASE_IDLE:

		if (m_countIdle == 0)
			return;

		handle = m_handleIdle[m_frame / animation_idle % m_countIdle];
		break;

	case CCHARA_PHASE::PHASE_WALK:

		if (m_countWalk == 0)
			return;

		handle = m_handleWalk[m_frame / animation_walk % m_countWalk];
		break;

	default:

		break;

	}

	int v = speed_x < 0 ? 1 : -1;
	int t_l = (int)pos_x - v * (m_tex_w / 2) * render_scale_w;
	int t_r = (int)pos_x + v * (m_tex_w / 2) * render_scale_w;
	int t_t = (int)pos_y - (m_tex_h / 2) * render_scale_h;
	int t_b = (int)pos_y + (m_tex_h / 2) * render_scale_h;
	DrawModiGraph(t_l, t_t, t_r, t_t, t_r, t_b, t_l, t_b, handle, true);

}

void CChara::Move()
{

	if (flg_pick)
		return;

	pos_x += speed_x;
	pos_y += speed_y;

	// xでループ
	if (flg_loop_x) {
		// 完全に隠れたら
		if (pos_x < 0 - collision_w / 2) {
			// 現れる
			pos_x += area_w + collision_w;
		}
		// 完全に隠れたら
		else if (pos_x > area_w + collision_w / 2) {
			// 現れる
			pos_x -= area_w + collision_w;
		}
	}

	// xで折り返す
	else{
		// ぶつかったら
		if (pos_x < 0 + collision_w / 2) {
			// 折り返す
			pos_x = collision_w / 2 - (pos_x - collision_w / 2);
			speed_x = -speed_x;
		}
		// ぶつかったら
		else if (pos_x > area_w - collision_w / 2) {
			// 折り返す
			pos_x = (area_w - collision_w / 2) - (pos_x -  (area_w - collision_w / 2));
			speed_x = -speed_x;
		}
	}

	// yでループ
	if (flg_loop_y) {
		// 完全に隠れたら
		if (pos_y < 0 - collision_h / 2) {
			// 現れる
			pos_y += area_h + collision_h;
		}
		// 完全に隠れたら
		else if (pos_y > area_h + collision_h / 2) {
			// 現れる
			pos_y -= area_h + collision_h;
		}
	}

	// yで折り返す
	else {
		// ぶつかったら
		if (pos_y < 0 + collision_h / 2) {
			// 折り返す
			pos_y = collision_h / 2 - (pos_y - collision_h / 2);
			speed_y = -speed_y;
		}
		// ぶつかったら
		else if (pos_y > area_h - collision_h / 2) {
			// 折り返す
			pos_y = (area_h - collision_h / 2) - (pos_y - (area_h - collision_h / 2));
			speed_y = -speed_y;
		}
	}

}

int CChara::LoadTexMain(char* path, bool normalize)
{

	if (m_handleMain >= 0) {
		DeleteGraph(m_handleMain);
		m_handleMain = -1;
	}

	m_handleMain = normalize ? ConvertNormalizedAlphaImage(path) : LoadGraph(path, false);
	return m_handleMain;

}

int CChara::AddTexIdle(char* path, bool normalize)
{

	if (m_countIdle == CHARA_MAX_IDLE)
		return -1;

	int handle = normalize ? ConvertNormalizedAlphaImage(path) : LoadGraph(path, false);
	if (handle < 0)
		return -1;

	m_handleIdle[m_countIdle++] = handle;
	return handle;

}

int CChara::AddTexWalk(char* path, bool normalize)
{

	if (m_countWalk == CHARA_MAX_IDLE)
		return -1;

	int handle = normalize ? ConvertNormalizedAlphaImage(path) : LoadGraph(path, false);
	if (handle < 0)
		return -1;

	m_handleWalk[m_countWalk++] = handle;
	return handle;

}

int CChara::DivTexToIdle(int x, int y, int w = -1, int h = -1)
{

	if (m_handleMain == -1)
		return -1;

	if (m_countIdle == CHARA_MAX_IDLE)
		return -1;

	w = w > 0 ? w : collision_w;
	h = h > 0 ? h : collision_h;

	int handle = DerivationGraph(x, y, w, h, m_handleMain);
	if (handle < 0)
		return -1;

	m_handleIdle[m_countIdle++] = handle;
	return handle;

}

int CChara::DivTexToWalk(int x, int y, int w = -1, int h = -1)
{

	if (m_handleMain == -1)
		return -1;

	if (m_countWalk == CHARA_MAX_IDLE)
		return -1;

	w = w > 0 ? w : collision_w;
	h = h > 0 ? h : collision_h;

	int handle = DerivationGraph(x, y, w, h, m_handleMain);
	if (handle < 0)
		return -1;

	m_handleWalk[m_countWalk++] = handle;
	return handle;

}

int CChara::ConvertNormalizedAlphaImage(char* path)
{

	int handle = LoadSoftImage(path);
	if (handle == -1)
		return -1;

	int w, h;
	GetSoftImageSize(handle, &w, &h);

	int ret = MakeARGB8ColorSoftImage(w, h);
	
	int r, g, b, a;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			GetPixelSoftImage(handle, x, y, &r, &g, &b, &a);
			a = (a > 127) * 255;
			DrawPixelSoftImage(ret, x, y, r, g, b, a);
		}
	}

	ret = CreateGraphFromSoftImage(ret);
	return ret;

}

