//=============================================================================
//
// ���f������ [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"
#include "shadow.h"
#include "enemy.h"
#include "collision.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BULLET		"data/MODEL/tama.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���
#define VALUE_RUN			(4.0f)
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define PLAYER_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(7.0f)							// �v���C���[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER						g_Player;						// �v���C���[
static ID3D11ShaderResourceView*	g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static int							g_TexNo = 0;				// �e�N�X�`���ԍ�


static char* g_TextureName[] = {
	"data/TEXTURE/white.png",
};
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	g_Player.load = true;

	for (int i = 0; i < MAX_GROUP; i++)
	{
		g_Player.trans[i].pos = XMFLOAT3(0.0f, 0.0, 0.0f);
		g_Player.trans[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Player.trans[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_Player.trans[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
	}
	g_Player.trans[ALL].pos = XMFLOAT3(0.0f, PLAYER_OFFSET_Y, 0.0f);

	g_Player.spd  = 0.0f;			// �ړ��X�s�[�h�N���A
	g_Player.size = PLAYER_SIZE;	// �����蔻��̑傫��

	ResetAllStates();

	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_Player.trans[ALL].pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�

	//GetModelDiffuse(&g_Player.bullet, g_Player.diffuse);

	g_Player.use = true;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// ���f���̉������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		g_Player.state = JUMP;
	}

	// �ړ��������Ⴄ
	if (GetKeyboardPress(DIK_A))
	{	// ���ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.stopRun = FALSE;
		if (g_Player.state == IDLE)
			g_Player.state = WALK;
		if (GetKeyboardPress(DIK_LSHIFT) && (g_Player.state == WALK || g_Player.state == RUN))
		{
			g_Player.spd = VALUE_RUN;
			g_Player.state = RUN;
		}
		if (GetKeyboardPress(DIK_W))
			g_Player.targetDir = XM_PI * 3 / 4;
		else if (GetKeyboardPress(DIK_S))
			g_Player.targetDir = XM_PI / 4;
		else
			g_Player.targetDir = XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_D))
	{	// �E�ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.stopRun = FALSE;
		if (g_Player.state == IDLE)
			g_Player.state = WALK;
		if (GetKeyboardPress(DIK_LSHIFT) && (g_Player.state == WALK || g_Player.state == RUN))
		{
			g_Player.spd = VALUE_RUN;
			g_Player.state = RUN;
		}
		if (GetKeyboardPress(DIK_W))
			g_Player.targetDir = - XM_PI * 3 / 4;
		else if (GetKeyboardPress(DIK_S))
			g_Player.targetDir = - XM_PI / 4;
		else
			g_Player.targetDir = -XM_PI / 2;
	}
	if (GetKeyboardPress(DIK_W))
	{	// ��ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.stopRun = FALSE;
		if (g_Player.state == IDLE)
			g_Player.state = WALK;
		if (GetKeyboardPress(DIK_LSHIFT) && (g_Player.state == WALK || g_Player.state == RUN))
		{
			g_Player.spd = VALUE_RUN;
			g_Player.state = RUN;
		}
		if (GetKeyboardPress(DIK_A))
			g_Player.targetDir = XM_PI * 3 / 4;
		else if (GetKeyboardPress(DIK_D))
			g_Player.targetDir = XM_PI * 5/ 4;
		else
			g_Player.targetDir = XM_PI;
	}
	if (GetKeyboardPress(DIK_S))
	{	// ���ֈړ�
		g_Player.spd = VALUE_MOVE;
		g_Player.stopRun = FALSE;
		if (g_Player.state == IDLE)
			g_Player.state = WALK;
		if (GetKeyboardPress(DIK_LSHIFT) && (g_Player.state == WALK || g_Player.state == RUN))
		{
			g_Player.spd = VALUE_RUN;
			g_Player.state = RUN;
		}
		if (GetKeyboardPress(DIK_A))
			g_Player.targetDir = XM_PI / 4;
		else if (GetKeyboardPress(DIK_D))
			g_Player.targetDir = -XM_PI / 4;
		else
			g_Player.targetDir = 0.0f;
	}

	float deltaDir = g_Player.targetDir - g_Player.dir;
	if (deltaDir > XM_PI) deltaDir -= 2 * XM_PI;
	if (deltaDir < -XM_PI) deltaDir += 2 * XM_PI;
	g_Player.dir += deltaDir * ROTATION_SPEED;

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_R))
	{
		for (int i = 0; i < MAX_GROUP; i++)
		{
			g_Player.trans[i].pos.z = g_Player.trans[i].pos.x = 0.0f;
			g_Player.trans[i].rot.y = g_Player.dir = 0.0f;
		}
		g_Player.spd = 0.0f;
	}
#endif

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		g_Player.state = ATTACK;

		for (int i = 0; i < MAX_BULLET; i++)
		{
			if (g_Player.bullet[i].use == TRUE) continue;

			LoadModel(MODEL_BULLET, &g_Player.bullet[i].model);
			g_Player.bullet[i].pos = g_Player.trans[ALL].pos;	
			g_Player.bullet[i].rot = g_Player.trans[ALL].rot;
			g_Player.bullet[i].scl = XMFLOAT3(1.2f, 1.2f, 1.2f);
			g_Player.bullet[i].pos.x -= sinf(g_Player.bullet[i].rot.y) * 45.0f;
			g_Player.bullet[i].pos.z -= cosf(g_Player.bullet[i].rot.y) * 45.0f;
			g_Player.bullet[i].pos.y -= 35;
			g_Player.bullet[i].spd = 5.0f;
			g_Player.bullet[i].use = TRUE;
			g_Player.bullet[i].lifeTime = 200.0f;
			g_Player.bullet[i].size = 35.0f;
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		// ���f���̐F�����ɖ߂��Ă���
		//for (int j = 0; j < g_Player.bullet.SubsetNum; j++)
		//{
		//	SetModelDiffuse(&g_Player.bullet, j, g_Player.diffuse[j]);
		//}
	}

	//	// Key���͂���������ړ���������
	switch (g_Player.state)
	{
	case IDLE:
		HandleStopMove();
		break;
	case WALK:
	case RUN:
		if (g_Player.spd >= VALUE_MOVE * SPD_DECAY_RATE * SPD_DECAY_RATE * SPD_DECAY_RATE)
		{
			if (GetKeyboardRelease(DIK_LSHIFT) && g_Player.spd >= VALUE_MOVE)
			{
				g_Player.stopRun = TRUE;
			}
			else if (g_Player.stopRun == FALSE)
				HandlePlayerMove();
		}
		else if (g_Player.stopRun == TRUE)
		{
			PlayStopRunAnim();
		}
		else
		{
			HandleStopMove();
		}
		break;
	case JUMP:
		HandlePlayerMove();
		break;
	case ATTACK:
		PlayAttackAnim();
		break;
	default:
		break;
	}




	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos = g_Player.trans[ALL].pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);



	g_Player.spd *= 0.93f;

	UpdateBullet();

	ENEMY* enemy = GetEnemy();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use == FALSE)
			continue;
		if (CollisionBC(g_Player.trans[ALL].pos, enemy[i].pos, g_Player.size, enemy[i].size))
		{
			enemy[i].HP -= 10.0f;
		}
	}



#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Player:�� �� �� ���@Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.trans[ALL].pos.x, g_Player.trans[ALL].pos.y, g_Player.trans[ALL].pos.z);
#endif
}

void UpdateBullet(void)
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Player.bullet[i].use == FALSE) continue;

		g_Player.bullet[i].pos.x -= sinf(g_Player.bullet[i].rot.y) * g_Player.bullet[i].spd;
		g_Player.bullet[i].pos.z -= cosf(g_Player.bullet[i].rot.y) * g_Player.bullet[i].spd;
		g_Player.bullet[i].lifeTime--;

		ENEMY* enemy = GetEnemy();

		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (enemy[i].use == FALSE)
				continue;

			if (CollisionBC(g_Player.bullet[i].pos, enemy[i].pos, g_Player.bullet[i].size, enemy[i].size))
			{
				//enemy[i].use = FALSE;
				enemy[i].HP -= 5.0f;
				g_Player.bullet[i].use = FALSE;
				//aUnloadModel(&g_Player.bullet[i].model);
				//ReleaseShadow(enemy[i].shadowIdx);
			}
		}

		if (g_Player.bullet[i].lifeTime <= 0)
		{
			UnloadModel(&g_Player.bullet[i].model);
			g_Player.bullet[i].use = FALSE;
		}
	}

}

void HandlePlayerMove()
{

	CAMERA* cam = GetCamera();

	g_Player.trans[ALL].rot.y = g_Player.dir + cam->rot.y;

	// ���͂̂����������փv���C���[���������Ĉړ�������
	g_Player.trans[ALL].pos.x -= sinf(g_Player.trans[ALL].rot.y) * g_Player.spd;
	g_Player.trans[ALL].pos.z -= cosf(g_Player.trans[ALL].rot.y) * g_Player.spd;

	if (g_Player.state == WALK || g_Player.state == RUN)
	{
		PlayMoveAnim();
	}
	else if (g_Player.state == JUMP)
	{
		PlayJumpAnim();
	}


}

void HandleStopMove(void)
{
	g_Player.trans[ALL].pos.y = 0.0f;
	g_Player.twintailFluc = FALSE;
	g_Player.twintailDown = FALSE;
	g_Player.armRotDeg.z = 0.0f;
	g_Player.state = IDLE;
	ResetRotation(g_Player.trans[RIGHT_ARM].rot.x, ARM_SPD);
	ResetRotation(g_Player.trans[LEFT_ARM].rot.x, ARM_SPD);
	ResetRotation(g_Player.trans[RIGHT_LEG].rot.x, LEG_SPD);
	ResetRotation(g_Player.trans[LEFT_LEG].rot.x, LEG_SPD);
	ResetRotation(g_Player.trans[TWINTAIL].rot.x, TWINTAIL_FLUC_SPD_RUN);
	
}

void ResetRotation(float& rotationOffset, float speed)
{
	if (rotationOffset == 0.0f)
		return;
	else if (rotationOffset > 0.0f)
	{
		rotationOffset -= speed;
		if (rotationOffset < 0.0f)
			rotationOffset = 0.0f;
	}
	else if (rotationOffset < 0.0f)
	{
		rotationOffset += speed;
		if (rotationOffset > 0.0f)
			rotationOffset = 0.0f;
	}
}

void PlayAttackAnim(void)
{
	SetRotateCenter(HEAD, XMFLOAT3(0.0f, -5.0f, -1.0f));
	SetRotateCenter(BODY, XMFLOAT3(0.0f, -5.0f, -1.0f));
	SetRotateCenter(LEFT_ARM, XMFLOAT3(0.01f, -5.01f, -1.1f));
	SetRotateCenter(RIGHT_ARM, XMFLOAT3(0.01f, -5.01f, -1.1f));

	if (g_Player.bodyDir)
	{
		g_Player.attackCnt++;

		//g_Player.trans[RIGHT_ARM].rot.z = 0.0f;
		//g_Player.trans[LEFT_ARM].rot.z = 0.0f;

		g_Player.trans[HEAD].rot.x -= BODY_ROT_ATTACK_SPD_X;
		g_Player.trans[BODY].rot.x -= BODY_ROT_ATTACK_SPD_X;
		g_Player.trans[HEAD].rot.y -= BODY_ROT_ATTACK_SPD_Y;
		g_Player.trans[BODY].rot.y -= BODY_ROT_ATTACK_SPD_Y;

		g_Player.trans[LEFT_ARM].rot.y -= ARM_ROT_ATTACK_SPD_Y;
		g_Player.trans[RIGHT_ARM].rot.y -= ARM_ROT_ATTACK_SPD_Y;
		g_Player.trans[WAND].rot.y -= ARM_ROT_ATTACK_SPD_Y;
		g_Player.trans[WAND].rot.x -= WAND_ROT_ATTACK_SPD_X;
		if (g_Player.trans[BODY].rot.x <= -MAX_ATTACK_BODY_DEG)
		{
			g_Player.trans[HEAD].rot.x = -MAX_ATTACK_BODY_DEG;
			g_Player.trans[BODY].rot.x = -MAX_ATTACK_BODY_DEG;
			g_Player.trans[HEAD].rot.y += BODY_ROT_ATTACK_SPD_Y;
			g_Player.trans[BODY].rot.y += BODY_ROT_ATTACK_SPD_Y;
			g_Player.trans[LEFT_ARM].rot.y += ARM_ROT_ATTACK_SPD_Y;
			g_Player.trans[RIGHT_ARM].rot.y += ARM_ROT_ATTACK_SPD_Y;
			g_Player.trans[WAND].rot.y += ARM_ROT_ATTACK_SPD_Y;
			g_Player.trans[WAND].rot.x += WAND_ROT_ATTACK_SPD_X;
		}
		if (g_Player.attackCnt > ATTACK_BODY_WAIT)
		{
			g_Player.bodyDir = !g_Player.bodyDir;
		}
	}
	else if (g_Player.attackCnt > ATTACK_BODY_WAIT)
	{
		g_Player.trans[HEAD].rot.x += BODY_ROT_ATTACK_SPD_X;
		g_Player.trans[BODY].rot.x += BODY_ROT_ATTACK_SPD_X;
		g_Player.trans[HEAD].rot.y += BODY_ROT_ATTACK_SPD_Y;
		g_Player.trans[BODY].rot.y += BODY_ROT_ATTACK_SPD_Y;

		g_Player.trans[LEFT_ARM].rot.y += ARM_ROT_ATTACK_SPD_Y;
		g_Player.trans[RIGHT_ARM].rot.y += ARM_ROT_ATTACK_SPD_Y;
		g_Player.trans[WAND].rot.y += ARM_ROT_ATTACK_SPD_Y;
		g_Player.trans[WAND].rot.x += WAND_ROT_ATTACK_SPD_X;
		if (g_Player.trans[BODY].rot.x >= 0.0f)
		{
			g_Player.trans[BODY].rot.x = 0.0f;
			g_Player.trans[HEAD].rot.x = 0.0f;
			g_Player.trans[BODY].rot.y = 0.0f;
			g_Player.trans[HEAD].rot.y = 0.0f;
			g_Player.trans[LEFT_ARM].rot.y = 0.0f;
			g_Player.trans[RIGHT_ARM].rot.y = 0.0f;
			g_Player.trans[WAND].rot.y = 0.0f;
			g_Player.attackCnt = 0;
			g_Player.trans[WAND].rot.x = -XM_PI * 0.1f;
			g_Player.state = IDLE;
		}
	}
	else
	{

		g_Player.trans[HEAD].rot.x += BODY_ROT_ATTACK_SPD_X;
		g_Player.trans[BODY].rot.x += BODY_ROT_ATTACK_SPD_X;
		g_Player.trans[HEAD].rot.y += BODY_ROT_ATTACK_SPD_Y;
		g_Player.trans[BODY].rot.y += BODY_ROT_ATTACK_SPD_Y;

		g_Player.trans[LEFT_ARM].rot.y += ARM_ROT_ATTACK_SPD_Y;
		g_Player.trans[RIGHT_ARM].rot.y += ARM_ROT_ATTACK_SPD_Y;
		g_Player.trans[WAND].rot.y += ARM_ROT_ATTACK_SPD_Y;
		if (g_Player.trans[BODY].rot.x >= MAX_ATTACK_BODY_DEG)
		{
			g_Player.trans[BODY].rot.x = MAX_ATTACK_BODY_DEG;
			g_Player.trans[HEAD].rot.x = MAX_ATTACK_BODY_DEG;
			g_Player.trans[HEAD].rot.y -= BODY_ROT_ATTACK_SPD_Y;
			g_Player.trans[BODY].rot.y -= BODY_ROT_ATTACK_SPD_Y;
			g_Player.trans[LEFT_ARM].rot.y -= ARM_ROT_ATTACK_SPD_Y;
			g_Player.trans[RIGHT_ARM].rot.y -= ARM_ROT_ATTACK_SPD_Y;
			g_Player.trans[WAND].rot.y -= ARM_ROT_ATTACK_SPD_Y;
			g_Player.bodyDir = !g_Player.bodyDir;
		}

	}
}

void PlayJumpAnim(void)
{
	if (g_Player.stopJump == FALSE)
	{
		float angle = (XM_PI / JUMP_CNT_MAX) * g_Player.jumpCnt;
		g_Player.trans[ALL].pos.y += g_Player.jumpYMax * cosf(angle);
		g_Player.jumpCnt++;

		g_Player.trans[RIGHT_ARM].rot.z -= JUMP_ARM_WAVE_SPD;
		g_Player.trans[LEFT_ARM].rot.z += JUMP_ARM_WAVE_SPD;
		g_Player.trans[RIGHT_LEG].rot.z -= JUMP_LEG_WAVE_SPD;
		g_Player.trans[LEFT_LEG].rot.z += JUMP_LEG_WAVE_SPD;
		if (g_Player.trans[RIGHT_ARM].rot.z <= -JUMP_MAX_ARM_DEG)
		{
			g_Player.trans[RIGHT_ARM].rot.z = -JUMP_MAX_ARM_DEG;
			g_Player.trans[LEFT_ARM].rot.z = JUMP_MAX_ARM_DEG;
		}
		if (g_Player.trans[RIGHT_LEG].rot.z <= -JUMP_MAX_LEG_DEG)
		{
			g_Player.trans[RIGHT_LEG].rot.z = -JUMP_MAX_LEG_DEG;
			g_Player.trans[LEFT_LEG].rot.z = JUMP_MAX_LEG_DEG;
		}
	}
	else
	{
		if (g_Player.trans[RIGHT_ARM].rot.z < 0.0f)
		{
			g_Player.trans[RIGHT_ARM].rot.z += STOP_RUN_ARM_WAVE_SPD;
			g_Player.trans[LEFT_ARM].rot.z -= STOP_RUN_ARM_WAVE_SPD;
		}
		if (g_Player.trans[RIGHT_LEG].rot.z < 0.0f)
		{
			g_Player.trans[RIGHT_LEG].rot.z += JUMP_LEG_WAVE_SPD;
			g_Player.trans[LEFT_LEG].rot.z -= JUMP_LEG_WAVE_SPD;

		}
		if (g_Player.trans[RIGHT_ARM].rot.z >= 0.0f && g_Player.trans[RIGHT_LEG].rot.z >= 0.0f)
		{
			g_Player.state = IDLE;
			g_Player.stopJump = FALSE;
		}

	}

	if (g_Player.jumpCnt > JUMP_CNT_MAX)
	{
		g_Player.stopJump = TRUE;
		g_Player.jumpCnt = 0;
	}
}

void PlayStopRunAnim(void)
{
	SetRotateCenter(HEAD, XMFLOAT3(0.0f, -5.0f, -1.0f));
	SetRotateCenter(BODY, XMFLOAT3(0.0f, -5.0f, -1.0f));
	SetRotateCenter(LEFT_ARM, XMFLOAT3(0.01f, -5.01f, -1.1f));
	SetRotateCenter(RIGHT_ARM, XMFLOAT3(0.01f, -5.01f, -1.1f));

	if (g_Player.bodyDir && g_Player.stopRunCnt > STOP_RUN_BODY_WAIT)
	{
		g_Player.trans[RIGHT_ARM].rot.z = 0.0f;
		g_Player.trans[LEFT_ARM].rot.z = 0.0f;

		g_Player.trans[HEAD].rot.x += BODY_ROT_SPD;
		g_Player.trans[BODY].rot.x += BODY_ROT_SPD;
		if (g_Player.trans[BODY].rot.x >= 0.0f)
		{
			g_Player.state = IDLE;
			ResetStopRunState();
		}
	}
	else
	{
		g_Player.stopRunCnt++;

		if (g_Player.armDir)
		{
			g_Player.trans[RIGHT_ARM].rot.z += STOP_RUN_ARM_WAVE_SPD;
			g_Player.trans[LEFT_ARM].rot.z -= STOP_RUN_ARM_WAVE_SPD;
			if (g_Player.trans[RIGHT_ARM].rot.z >= STOP_RUN_MAX_ARM_DEG)
			{
				g_Player.armDir = !g_Player.armDir;
			}
		}
		else
		{
			g_Player.trans[RIGHT_ARM].rot.z -= STOP_RUN_ARM_WAVE_SPD;
			g_Player.trans[LEFT_ARM].rot.z += STOP_RUN_ARM_WAVE_SPD;
			if (g_Player.trans[RIGHT_ARM].rot.z <= -STOP_RUN_MAX_ARM_DEG)
			{
				g_Player.armDir = !g_Player.armDir;
			}
		}

		g_Player.trans[HEAD].rot.x -= BODY_ROT_SPD;
		g_Player.trans[BODY].rot.x -= BODY_ROT_SPD;
		if (g_Player.trans[BODY].rot.x <= -MAX_RUN_BODY_DEG)
		{
			g_Player.trans[BODY].rot.x = -MAX_RUN_BODY_DEG;
			g_Player.trans[HEAD].rot.x = -MAX_RUN_BODY_DEG;
			g_Player.bodyDir = !g_Player.bodyDir;
		}

	}
}

void PlayMoveAnim(void)
{
	ResetStopRunState();

	g_Player.armRotDeg.z = XM_PI * 0.25f;

	if (g_Player.spd == VALUE_MOVE)
		g_Player.state = WALK;
	else if (g_Player.spd == VALUE_RUN)
		g_Player.state = RUN;

	g_Player.walkCnt++;

	BOOL isRun = g_Player.state == RUN;
	float armSpd = isRun ? ARM_SPD_RUN : ARM_SPD;
	float legSpd = isRun ? LEG_SPD_RUN : LEG_SPD;
	float maxArmDeg = isRun ? MAX_ARM_DEG_RUN : MAX_ARM_DEG;
	float valueRotate = isRun ? VALUE_ROTATE * 2 : VALUE_ROTATE;
	float twintailSpd = isRun ? TWINTAIL_FLUC_SPD_RUN : TWINTAIL_FLUC_SPD_WALK;
	float maxTwintailDeg = isRun ? TWINTAIL_MAX_DEG_RUN : TWINTAIL_MAX_DEG_WALK;
	float flucDeg = isRun ? TWINTAIL_FLUC_DEG_RUN : TWINTAIL_FLUC_DEG_WALK;

	if (!g_Player.rightArmDir)
	{
		g_Player.trans[RIGHT_ARM].rot.x += armSpd;
		g_Player.trans[LEFT_ARM].rot.x -= armSpd;
		g_Player.trans[RIGHT_LEG].rot.x -= legSpd;
		g_Player.trans[LEFT_LEG].rot.x += legSpd;

		if (g_Player.trans[LEFT_ARM].rot.x <= -maxArmDeg)
			g_Player.rightArmDir = !g_Player.rightArmDir;
	}
	else
	{
		g_Player.trans[RIGHT_ARM].rot.x -= armSpd;
		g_Player.trans[LEFT_ARM].rot.x += armSpd;
		g_Player.trans[RIGHT_LEG].rot.x += legSpd;
		g_Player.trans[LEFT_LEG].rot.x -= legSpd;
		if (g_Player.trans[LEFT_ARM].rot.x >= maxArmDeg)
			g_Player.rightArmDir = !g_Player.rightArmDir;
	}



	if (g_Player.walkCnt > maxArmDeg / armSpd)
	{
		g_Player.walkCnt = 0;
		g_Player.fluctuation = !g_Player.fluctuation;
	}

	if (g_Player.fluctuation)
	{
		g_Player.trans[ALL].pos.y -= valueRotate;
	}
	else
	{
		g_Player.trans[ALL].pos.y += valueRotate;
	}

	if (g_Player.twintailFluc)
	{
		if (!g_Player.twintailDown)
		{
			g_Player.trans[TWINTAIL].rot.x -= twintailSpd;
			if (g_Player.trans[TWINTAIL].rot.x <= -maxTwintailDeg)
				g_Player.twintailDown = !g_Player.twintailDown;
		}
		else
		{
			g_Player.trans[TWINTAIL].rot.x += twintailSpd;
			if (g_Player.trans[TWINTAIL].rot.x >= -flucDeg)
				g_Player.twintailDown = !g_Player.twintailDown;
		}
	}
	else
	{
		g_Player.trans[TWINTAIL].rot.x -= twintailSpd * 2;
		if (g_Player.trans[TWINTAIL].rot.x <= -flucDeg)
			g_Player.twintailFluc = TRUE;
	}
	PrintDebugProc("twintailFluc:%d twintailDown:%d\n", g_Player.twintailDown, g_Player.twintailDown);
	PrintDebugProc("twintailFluc:%f \n", g_Player.trans[TWINTAIL].rot.x);
}

void SetRotateCenter(int part, XMFLOAT3 center)
{
	switch (part)
	{
	case HEAD:
		g_Player.trans[RIGHT_EYE].rotateCenter.x = center.x * SIZE_WH * 2;
		g_Player.trans[RIGHT_EYE].rotateCenter.y = center.y * SIZE_WH * 2;
		g_Player.trans[RIGHT_EYE].rotateCenter.z = center.z * SIZE_WH * 2;
		g_Player.trans[LEFT_EYE].rotateCenter.x = center.x * SIZE_WH * 2;
		g_Player.trans[LEFT_EYE].rotateCenter.y = center.y * SIZE_WH * 2;
		g_Player.trans[LEFT_EYE].rotateCenter.z = center.z * SIZE_WH * 2;
		g_Player.trans[HEAD].rotateCenter.x = center.x * SIZE_WH * 2;
		g_Player.trans[HEAD].rotateCenter.y = center.y * SIZE_WH * 2;
		g_Player.trans[HEAD].rotateCenter.z = center.z * SIZE_WH * 2;
		g_Player.trans[TWINTAIL].rotateCenter.x = center.x * SIZE_WH * 2;
		g_Player.trans[TWINTAIL].rotateCenter.y = center.y * SIZE_WH * 2;
		g_Player.trans[TWINTAIL].rotateCenter.z = center.z * SIZE_WH * 2;
		break;
	default:
		g_Player.trans[part].rotateCenter.x = center.x * SIZE_WH * 2;
		g_Player.trans[part].rotateCenter.y = center.y * SIZE_WH * 2;
		g_Player.trans[part].rotateCenter.z = center.z * SIZE_WH * 2;
		break;
	}
}

void ResetRotateCenter(void)
{
	for (int i = 0; i < MAX_GROUP; i++)
	{
		g_Player.trans[i].rotateCenter.x = 0.0f;
		g_Player.trans[i].rotateCenter.y = 0.0f;
		g_Player.trans[i].rotateCenter.z = 0.0f;
	}
}

void ResetStopRunState(void)
{
	g_Player.stopRun = FALSE;
	g_Player.trans[HEAD].rot.x = 0.0f;
	g_Player.trans[BODY].rot.x = 0.0f;
	g_Player.trans[RIGHT_ARM].rot.z = 0.0f;
	g_Player.trans[LEFT_ARM].rot.z = 0.0f;
	g_Player.armDir = FALSE;
	g_Player.bodyDir = FALSE;
	g_Player.stopRunCnt = 0;
}

void ResetAllStates(void)
{
	g_Player.shakeHead = FALSE;
	g_Player.nodHead = FALSE;
	g_Player.shakeDir = FALSE;
	g_Player.nodDir = FALSE;
	g_Player.blink = FALSE;
	g_Player.eyeOpen = FALSE;
	g_Player.wink = FALSE;
	g_Player.winkEnd = FALSE;
	g_Player.rightArmDir = FALSE;
	g_Player.leftLegDir = FALSE;
	g_Player.fluctuation = FALSE;
	g_Player.twintailFluc = FALSE;
	g_Player.twintailDown = FALSE;
	g_Player.jump = FALSE;
	g_Player.bodyDir = FALSE;
	g_Player.armDir = FALSE;
	g_Player.stopRun = FALSE;
	g_Player.stopJump = FALSE;

	g_Player.state = IDLE;
	g_Player.walkCnt = 0;
	g_Player.winkWait = 0;
	g_Player.jumpCnt = 0;
	g_Player.jumpYMax = 3;
	g_Player.targetDir = 0.0f;
	g_Player.stopRunCnt = 0;
	g_Player.trans[WAND].pos = XMFLOAT3(-15.0f, -25.0f, -10.0f);
	g_Player.trans[WAND].rot = XMFLOAT3(-XM_PI * 0.1f, 0.0f, XM_PI * 0.2f);
	g_Player.trans[WAND].scl = XMFLOAT3(0.8f, 0.8f, 0.8f);
	g_Player.armRotDeg = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	//{   // shadow
	//	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//	// ���[���h�}�g���b�N�X�̏�����
	//	mtxWorld = XMMatrixIdentity();

	//	// �X�P�[���𔽉f
	//	mtxScl = XMMatrixScaling(g_Player.scl.x, 0.1f, g_Player.scl.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//	// ��]�𔽉f
	//	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// �ړ��𔽉f
	//	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y - PLAYER_OFFSET_Y, g_Player.pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ���[���h�}�g���b�N�X�̐ݒ�
	//	SetWorldMatrix(&mtxWorld);

	//	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);

	//	for (int j = 0; j < g_Player.model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Player.model, j, XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f));
	//	}

	//	// ���f���`��
	//	DrawModel(&g_Player.model);
	//}

	//{
	//	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//	// ���[���h�}�g���b�N�X�̏�����
	//	mtxWorld = XMMatrixIdentity();

	//	// �X�P�[���𔽉f
	//	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//	// ��]�𔽉f
	//	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// �ړ��𔽉f
	//	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ���[���h�}�g���b�N�X�̐ݒ�
	//	SetWorldMatrix(&mtxWorld);

	//	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);

	//	for (int j = 0; j < g_Player.model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Player.model, j, g_Player.diffuse[j]);
	//	}

	//	// ���f���`��
	//	DrawModel(&g_Player.model);
	//}

		// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	XMMATRIX mtxWorld = ComputeWorldMatrix(HEAD);

	DrawHead();
	DrawEye();
	DrawBody();
	DrawArm();
	DrawLeg();
	DrawWand();

	DrawBullet();

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

void DrawWand(void)
{
	XMMATRIX mtxWorld = ComputeWorldMatrix(WAND);

	DrawCube(WAND_BLUE, 0.0f, 0.0f, 0.0f, mtxWorld);
	DrawCube(WAND_PURPLE, 0.0f, 0.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_BLUE, 0.0f, 1.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_PURPLE, 0.0f, 1.0f, 0.0f, mtxWorld);
	DrawCube(WAND_BLUE, 0.0f, 2.0f, 0.0f, mtxWorld);
	DrawCube(WAND_PURPLE, 0.0f, 2.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_BLUE, 0.0f, 3.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_PURPLE, 0.0f, 3.0f, 0.0f, mtxWorld);
	DrawCube(WAND_BLUE, 0.0f, 4.0f, 0.0f, mtxWorld);
	DrawCube(WAND_PURPLE, 0.0f, 4.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_BLUE, 0.0f, 5.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_PURPLE, 0.0f, 5.0f, 0.0f, mtxWorld);
	DrawCube(WAND_BLUE, 0.0f, 6.0f, 0.0f, mtxWorld);
	DrawCube(WAND_PURPLE, 0.0f, 6.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_BLUE, 0.0f, 7.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_PURPLE, 0.0f, 7.0f, 0.0f, mtxWorld);
	DrawCube(WAND_BLUE, 0.0f, 8.0f, 0.0f, mtxWorld);
	DrawCube(WAND_PURPLE, 0.0f, 8.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_BLUE, 0.0f, 9.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_PURPLE, 0.0f, 9.0f, 0.0f, mtxWorld);
	DrawCube(WAND_PURPLE, 0.0f, 10.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_BLUE, 0.0f, 10.0f, 0.0f, mtxWorld);
	DrawCube(WAND_BLUE, 0.0f, 11.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_PURPLE, 0.0f, 11.0f, 0.0f, mtxWorld);
	DrawCube(WAND_PURPLE, 0.0f, 12.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.0f));
	DrawCube(WAND_BLUE, 0.0f, 12.0f, 0.0f, mtxWorld);

	DrawCube(WAND_PINK, -1.0f, 12.7f, -1.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 0.25f), XMFLOAT3(1.5f, 1.5f, 1.0f));
	DrawCube(WAND_PINK, 0.0f, 12.7f, -1.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 0.25f), XMFLOAT3(0.75f, 0.75f, 1.0f));
	DrawCube(WAND_PINK, 0.0f, 12.7f, -1.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.25f), XMFLOAT3(0.75f, 0.75f, 1.0f));
	DrawCube(WAND_PINK, 1.0f, 12.7f, -1.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.25f), XMFLOAT3(1.5f, 1.5f, 1.0f));

	XMFLOAT3 scale = XMFLOAT3(1.2f, 1.2f, 1.2f);
	DrawCube(WAND_RED, 0.0f, 13.0f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, 0.0f), scale);
	DrawCube(WAND_RED, 0.0f, 14.2f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, 0.0f), scale);
	DrawCube(WAND_RED, -1.2f, 14.2f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, 0.0f), scale);
	DrawCube(WAND_RED, 1.2f, 14.2f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, 0.0f), scale);
	DrawCube(WAND_RED, 0.0f, 15.4f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, 0.0f), scale);
	DrawCube(WAND_RED, -1.2f, 15.4f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, 0.0f), scale);
	DrawCube(WAND_RED, 1.2f, 15.4f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, 0.0f), scale);
	DrawCube(WAND_RED, -2.4f, 15.4f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, 0.0f), scale);
	DrawCube(WAND_RED, 2.4f, 15.4f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, 0.0f), scale);
	DrawCube(WAND_RED, -1.2f, 16.6f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, 0.0f), scale);
	DrawCube(WAND_RED, 1.2f, 16.6f, 0.0f, mtxWorld, XMFLOAT3(0.0f, 0.0f, 0.0f), scale);
}

void DrawEye(void)
{
	XMMATRIX mtxWorld = ComputeWorldMatrix(RIGHT_EYE);

	DrawCube(BLUE, -2.01f, -2.01f, -3.01f, mtxWorld);
	DrawCube(BLACK, -2.01f, -1.01f, -3.01f, mtxWorld);

	mtxWorld = ComputeWorldMatrix(LEFT_EYE);

	DrawCube(BLUE, 2.01f, -2.01f, -3.01f, mtxWorld);
	DrawCube(BLACK, 2.01f, -1.01f, -3.01f, mtxWorld);
}

void DrawHead(void)
{
	XMMATRIX mtxWorld = ComputeWorldMatrix(HEAD);

	DrawCube(FACE, 0, 0, 0, mtxWorld);
	DrawCube(PINK_FACE, -3.01f, -3.01f, -3.01f, mtxWorld);
	DrawCube(PINK_FACE, 3.01f, -3.01f, -3.01f, mtxWorld);
	DrawCube(WHITE, -3.99f, -1.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -3.99f, -0.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -4.0f, 1.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -4.0f, 2.01f, -4.01f, mtxWorld);
	DrawCube(WHITE3, -4.0f, -3.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, -4.0f, -4.01f, -2.01f, mtxWorld);
	DrawCube(WHITE2, -4.0f, 3.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, -4.0f, -2.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, -5.0f, -1.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, -5.0f, 0.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, -5.0f, 1.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, -5.0f, 2.01f, -3.01f, mtxWorld);
	DrawCube(WHITE3, -5.0f, 3.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, -5.0f, 2.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, -5.0f, 1.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, -5.0f, 0.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, -5.0f, -1.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, -4.0f, 4.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, -3.0f, 5.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, -2.0f, 6.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, -1.0f, 6.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, 0.0f, 6.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, 1.0f, 6.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, 2.0f, 6.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, 3.0f, 5.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, 4.0f, 4.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, 5.0f, 3.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, 5.0f, 2.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, 5.0f, 1.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, 5.0f, 0.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, 5.0f, -1.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, -5.0f, -2.01f, -2.01f, mtxWorld);
	DrawCube(WHITE4, -5.0f, -3.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, -5.0f, -1.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, -5.0f, 0.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, -5.0f, 1.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, -5.0f, 2.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, -5.0f, 3.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, -5.0f, -2.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, -4.0f, 4.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, -3.0f, 5.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, -2.0f, 6.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, -1.0f, 6.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, 0.0f, 6.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, 1.0f, 6.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, 2.0f, 6.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, 3.0f, 5.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, 4.0f, 4.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, 5.0f, 3.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, 5.0f, 2.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, 5.0f, 1.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, 5.0f, 0.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, 5.0f, -1.01f, -1.01f, mtxWorld);
	DrawCube(WHITE4, 5.0f, -2.01f, -1.01f, mtxWorld);
	DrawCube(WHITE5, -5.0f, -4.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, -5.0f, -3.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, -5.0f, -2.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, -5.0f, -1.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, -5.0f, 0.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, -5.0f, 1.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, -5.0f, 2.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, -4.0f, 3.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, -3.0f, 4.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, -2.0f, 5.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, -1.0f, 5.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, 0.0f, 5.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, 1.0f, 5.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, 2.0f, 5.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, 3.0f, 4.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, 4.0f, 3.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, 5.0f, 2.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, 5.0f, 1.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, 5.0f, 0.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, 5.0f, -1.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, 5.0f, -2.01f, 0.01f, mtxWorld);
	DrawCube(WHITE5, 5.0f, -3.01f, 0.01f, mtxWorld);
	DrawCube(WHITE6, -5.0f, -1.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, -5.0f, 0.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, -5.0f, 1.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, -5.0f, 2.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, -5.0f, -2.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, -4.0f, 3.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, -3.0f, 4.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, -2.0f, 5.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, -1.0f, 5.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, 0.0f, 5.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, 1.0f, 5.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, 2.0f, 5.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, 3.0f, 4.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, 4.0f, 3.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, 5.0f, 2.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, 5.0f, 1.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, 5.0f, 0.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, 5.0f, -1.01f, 1.01f, mtxWorld);
	DrawCube(WHITE6, 5.0f, -2.01f, 1.01f, mtxWorld);
	DrawCube(WHITE7, -5.0f, 0.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, -5.0f, 1.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, -5.0f, -1.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, -4.0f, 2.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, -3.01f, 3.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, -2.0f, 4.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, -1.0f, 4.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 0.0f, 4.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 1.0f, 4.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 2.0f, 4.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 3.01f, 3.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 4.0f, 2.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 5.0f, 1.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 5.0f, 0.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 5.0f, -1.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 4.0f, -1.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 4.0f, -2.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 3.01f, -3.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 2.0f, -3.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 1.0f, -3.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, 0.0f, -3.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, -1.0f, -3.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, -2.0f, -3.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, -3.01f, -3.01f, 2.01f, mtxWorld);
	DrawCube(WHITE7, -4.0f, -2.01f, 2.01f, mtxWorld);
	DrawCube(WHITE8, -4.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -4.0f, 1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -3.0f, 2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -2.0f, 2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -1.0f, 2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 0.0f, 2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 1.0f, 2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 2.0f, 2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 2.0f, 3.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -2.0f, 3.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -1.0f, 3.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 0.0f, 3.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 1.0f, 3.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 2.0f, 3.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 3.0f, 2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 4.0f, 1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 4.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 4.0f, -1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 3.0f, -1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 3.0f, -2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 3.0f, 1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 2.0f, 1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 1.0f, 1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 0.0f, 1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -1.0f, 1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -2.0f, 1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -3.0f, 1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 3.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 2.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 1.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 0.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -1.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -2.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -3.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 2.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 1.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 0.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -1.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -2.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -3.0f, 0.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 2.0f, -1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 1.0f, -1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 0.0f, -1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -1.0f, -1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -2.0f, -1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -3.0f, -1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -4.0f, -1.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 2.0f, -2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 1.0f, -2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, 0.0f, -2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -1.0f, -2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -2.0f, -2.01f, 3.01f, mtxWorld);
	DrawCube(WHITE8, -3.0f, -2.01f, 3.01f, mtxWorld);
	DrawCube(RED, -0.4f, -0.01f, 4.01f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 0.25f), XMFLOAT3(1.2f, 1.2f, 1.0f));
	DrawCube(RED, 0.6f, -0.01f, 4.01f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 0.25f), XMFLOAT3(1.2f, 1.2f, 1.0f));
	DrawCube(WHITE, -3.0f, 2.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -3.0f, 3.01f, -4.01f, mtxWorld);
	DrawCube(WHITE2, -3.0f, 4.01f, -3.01f, mtxWorld);
	DrawCube(WHITE, -3.0f, 1.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -3.0f, 0.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -3.99f, 0.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -4.0f, -1.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -2.0f, 2.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -2.0f, 3.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -2.0f, 4.01f, -4.01f, mtxWorld);
	DrawCube(WHITE2, -2.0f, 5.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, -1.0f, 5.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, 0.0f, 5.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, 1.0f, 5.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, 2.0f, 5.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, 3.0f, 4.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, 4.0f, 3.01f, -3.01f, mtxWorld);
	DrawCube(WHITE, -1.0f, 1.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -1.0f, 2.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -1.0f, 3.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, -1.0f, 4.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 0.0f, 0.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 0.0f, 1.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 0.0f, 2.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 0.0f, 3.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 0.0f, 4.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 1.0f, 0.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 1.0f, 1.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 1.0f, 2.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 1.0f, 3.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 1.0f, 4.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 2.0f, 0.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 2.0f, 1.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 2.0f, 2.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 2.0f, 3.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 2.0f, 4.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 3.0f, 0.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 3.0f, 1.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 3.0f, 2.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 3.0f, 3.01f, -4.01f, mtxWorld);
	DrawCube(BLUE0, 3.0f, 4.01f, -4.01f, mtxWorld);
	DrawCube(WHITE2, 3.99f, -2.01f, -3.01f, mtxWorld);
	DrawCube(WHITE, 3.99f, -1.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 3.99f, 0.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 3.99f, 1.01f, -4.01f, mtxWorld);
	DrawCube(WHITE, 3.99f, 2.01f, -4.01f, mtxWorld);
	DrawCube(PURPLE0, 3.99f, 3.01f, -4.01f, mtxWorld);
	DrawCube(WHITE2, 4.99f, 2.01f, -3.01f, mtxWorld);
	DrawCube(PINK0, 4.99f, 1.01f, -4.01f, mtxWorld);
	DrawCube(PINK0, 3.99f, 1.01f, -5.01f, mtxWorld);
	DrawCube(BLUE0, 1.99f, 3.01f, -5.01f, mtxWorld);
	DrawCube(PURPLE0, 2.99f, 2.01f, -5.01f, mtxWorld);
	DrawCube(WHITE2, 4.99f, 1.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, 4.99f, 0.01f, -3.01f, mtxWorld);
	DrawCube(WHITE2, 4.99f, -1.01f, -3.01f, mtxWorld);
	DrawCube(WHITE3, 4.99f, -2.01f, -2.01f, mtxWorld);
	DrawCube(WHITE4, 4.99f, -3.01f, -1.01f, mtxWorld);
	DrawCube(WHITE5, 4.99f, -4.01f, 0.01f, mtxWorld);
	DrawCube(WHITE3, 3.99f, -3.01f, -2.01f, mtxWorld);
	DrawCube(WHITE3, 3.99f, -4.01f, -2.01f, mtxWorld);

	DrawCube(PINK, -5.0f, 4.01f, -1.01f, mtxWorld);
	DrawCube(PINK, -4.0f, 5.01f, -1.01f, mtxWorld);
	DrawCube(PINK, 5.0f, 4.01f, -1.01f, mtxWorld);
	DrawCube(PINK, 4.0f, 5.01f, -1.01f, mtxWorld);

	mtxWorld = ComputeWorldMatrix(TWINTAIL);


	DrawCube(PINK2, -5.0f, 6.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, -6.0f, 7.01f, 0.01f, mtxWorld);
	DrawCube(PINK3, -6.0f, 7.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, -7.0f, 7.01f, 1.01f, mtxWorld);
	DrawCube(PINK2, -8.0f, 6.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, -9.0f, 5.01f, 0.01f, mtxWorld);
	DrawCube(PINK3, -9.0f, 4.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, -8.0f, 3.01f, 1.01f, mtxWorld);
	DrawCube(PINK2, -7.0f, 2.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, -7.0f, 1.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, -7.0f, 0.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, -8.0f, -1.01f, 0.01f, mtxWorld);
	DrawCube(PINK3, -9.0f, -2.01f, 1.01f, mtxWorld);
	DrawCube(PINK2, -10.0f, -3.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, -10.0f, -4.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, -10.0f, -5.01f, 0.01f, mtxWorld);
	DrawCube(PINK3, -9.0f, -6.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, -8.0f, -7.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, -8.0f, -8.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, -7.0f, -9.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, -7.0f, -10.01f, 1.01f, mtxWorld);
	DrawCube(PINK2, -6.0f, -11.01f, 0.01f, mtxWorld);

	DrawCube(BLUE, -4.0f, 6.01f, 0.01f, mtxWorld);
	DrawCube(BLUE2, -5.0f, 6.01f, 1.01f, mtxWorld);
	DrawCube(BLUE3, -6.0f, 7.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, -6.0f, 6.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, -7.0f, 6.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, -7.0f, 5.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, -8.0f, 4.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, -9.0f, 3.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, -9.0f, 2.01f, 2.01f, mtxWorld);
	DrawCube(BLUE2, -8.0f, 1.01f, 1.01f, mtxWorld);
	DrawCube(BLUE2, -8.0f, 0.01f, 1.01f, mtxWorld);
	DrawCube(BLUE2, -7.0f, -1.01f, 1.01f, mtxWorld);
	DrawCube(BLUE2, -7.0f, -2.01f, 1.01f, mtxWorld);
	DrawCube(BLUE2, -8.0f, -3.01f, 1.01f, mtxWorld);
	DrawCube(BLUE, -8.0f, -4.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, -8.0f, -5.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, -8.0f, -6.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, -7.0f, -7.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, -7.0f, -8.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, -8.0f, -9.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, -8.0f, -10.01f, 0.01f, mtxWorld);
	DrawCube(BLUE2, -9.0f, -11.01f, 1.01f, mtxWorld);

	DrawCube(PINK2, 5.0f, 6.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, 6.0f, 7.01f, 0.01f, mtxWorld);
	DrawCube(PINK3, 6.0f, 7.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, 7.0f, 7.01f, 1.01f, mtxWorld);
	DrawCube(PINK2, 8.0f, 6.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, 9.0f, 5.01f, 0.01f, mtxWorld);
	DrawCube(PINK3, 9.0f, 4.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, 8.0f, 3.01f, 1.01f, mtxWorld);
	DrawCube(PINK2, 7.0f, 2.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, 7.0f, 1.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, 7.0f, 0.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, 8.0f, -1.01f, 0.01f, mtxWorld);
	DrawCube(PINK3, 9.0f, -2.01f, 1.01f, mtxWorld);
	DrawCube(PINK2, 10.0f, -3.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, 10.0f, -4.01f, 0.01f, mtxWorld);
	DrawCube(PINK2, 10.0f, -5.01f, 0.01f, mtxWorld);
	DrawCube(PINK3, 9.0f, -6.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, 8.0f, -7.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, 8.0f, -8.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, 7.0f, -9.01f, 1.01f, mtxWorld);
	DrawCube(PINK3, 7.0f, -10.01f, 1.01f, mtxWorld);
	DrawCube(PINK2, 6.0f, -11.01f, 0.01f, mtxWorld);

	DrawCube(BLUE, 4.0f, 6.01f, 0.01f, mtxWorld);
	DrawCube(BLUE2, 5.0f, 6.01f, 1.01f, mtxWorld);
	DrawCube(BLUE3, 6.0f, 7.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, 6.0f, 6.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, 7.0f, 6.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, 7.0f, 5.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, 8.0f, 4.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, 9.0f, 3.01f, 2.01f, mtxWorld);
	DrawCube(BLUE3, 9.0f, 2.01f, 2.01f, mtxWorld);
	DrawCube(BLUE2, 8.0f, 1.01f, 1.01f, mtxWorld);
	DrawCube(BLUE2, 8.0f, 0.01f, 1.01f, mtxWorld);
	DrawCube(BLUE2, 7.0f, -1.01f, 1.01f, mtxWorld);
	DrawCube(BLUE2, 7.0f, -2.01f, 1.01f, mtxWorld);
	DrawCube(BLUE2, 8.0f, -3.01f, 1.01f, mtxWorld);
	DrawCube(BLUE, 8.0f, -4.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, 8.0f, -5.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, 8.0f, -6.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, 7.0f, -7.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, 7.0f, -8.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, 8.0f, -9.01f, 0.01f, mtxWorld);
	DrawCube(BLUE, 8.0f, -10.01f, 0.01f, mtxWorld);
	DrawCube(BLUE2, 9.0f, -11.01f, 1.01f, mtxWorld);
}

void DrawBody(void)
{
	XMMATRIX mtxWorld = ComputeWorldMatrix(BODY);

	DrawCube(RIBBON, -1.01f, -5.01f, -1.1f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 0.25f), XMFLOAT3(1.25f, 1.25f, 1.0f));
	DrawCube(RIBBON, 0.01f, -5.01f, -1.1f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 0.25f), XMFLOAT3(0.5f, 0.5f, 1.0f));
	DrawCube(RIBBON, 0.01f, -5.01f, -1.1f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.25f), XMFLOAT3(0.5f, 0.5f, 1.0f));
	DrawCube(RIBBON, 1.01f, -5.01f, -1.1f, mtxWorld, XMFLOAT3(0.0f, 0.0f, XM_PI * 1.25f), XMFLOAT3(1.25f, 1.25f, 1.0f));
	DrawCube(BLUE4, -2.01f, -4.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, -2.01f, -5.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, -1.01f, -4.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, -1.01f, -5.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, 0.01f, -4.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, 0.01f, -5.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, 1.01f, -4.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, 1.01f, -5.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, 2.01f, -4.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, 2.01f, -5.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, -1.01f, -6.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, -1.51f, -6.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, 0.01f, -6.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, 1.01f, -6.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, 1.51f, -6.01f, -1.0f, mtxWorld);

	mtxWorld = ComputeWorldMatrix(SKIRT);

	DrawCube(BLUE4, -2.01f, -7.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, 2.01f, -7.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, -2.51f, -8.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, 2.51f, -8.01f, -1.0f, mtxWorld);
	DrawCube(BLUE4, -1.01f, -7.01f, -2.0f, mtxWorld);
	DrawCube(BLUE4, 0.01f, -7.01f, -2.0f, mtxWorld);
	DrawCube(BLUE4, 1.01f, -7.01f, -2.0f, mtxWorld);
	DrawCube(BLUE4, -1.01f, -7.01f, 0.0f, mtxWorld);
	DrawCube(BLUE4, 0.01f, -7.01f, 0.0f, mtxWorld);
	DrawCube(BLUE4, 1.01f, -7.01f, 0.0f, mtxWorld);
	DrawCube(BLUE4, -2.01f, -8.01f, -2.0f, mtxWorld);
	DrawCube(BLUE4, -1.01f, -8.01f, -2.0f, mtxWorld);
	DrawCube(BLUE4, 0.01f, -8.01f, -2.0f, mtxWorld);
	DrawCube(BLUE4, 1.01f, -8.01f, -2.0f, mtxWorld);
	DrawCube(BLUE4, 2.01f, -8.01f, -2.0f, mtxWorld);
	DrawCube(BLUE4, -2.01f, -8.01f, 0.0f, mtxWorld);
	DrawCube(BLUE4, -1.01f, -8.01f, 0.0f, mtxWorld);
	DrawCube(BLUE4, 0.01f, -8.01f, 0.0f, mtxWorld);
	DrawCube(BLUE4, 1.01f, -8.01f, 0.0f, mtxWorld);
	DrawCube(BLUE4, 2.01f, -8.01f, 0.0f, mtxWorld);
}

void DrawArm(void)
{
	XMMATRIX mtxWorld = ComputeWorldMatrix(RIGHT_ARM);
	DrawCube(SKIN, -3.25f, -5.25f, -1.0f, mtxWorld, g_Player.armRotDeg, XMFLOAT3(1.5f, 1.5f, 2.0f));

	mtxWorld = ComputeWorldMatrix(LEFT_ARM);

	DrawCube(SKIN, 3.25f, -5.25f, -1.0f, mtxWorld, g_Player.armRotDeg, XMFLOAT3(1.5f, 1.5f, 2.0f));

}

void DrawLeg(void)
{
	XMMATRIX mtxWorld = ComputeWorldMatrix(RIGHT_LEG);

	DrawCube(SKIN, -2.01f, -9.01f, -0.7f, mtxWorld);
	DrawCube(SKIN, -1.01f, -9.01f, -0.7f, mtxWorld);
	DrawCube(SKIN, -2.01f, -10.01f, -0.7f, mtxWorld);
	DrawCube(SKIN, -1.01f, -10.01f, -0.7f, mtxWorld);
	DrawCube(BLUE4, -2.01f, -11.01f, -0.7f, mtxWorld);
	DrawCube(BLUE4, -1.01f, -11.01f, -0.7f, mtxWorld);
	DrawCube(SKIN, -2.01f, -9.01f, -1.3f, mtxWorld);
	DrawCube(SKIN, -1.01f, -9.01f, -1.3f, mtxWorld);
	DrawCube(SKIN, -2.01f, -10.01f, -1.3f, mtxWorld);
	DrawCube(SKIN, -1.01f, -10.01f, -1.3f, mtxWorld);
	DrawCube(BLUE4, -2.01f, -11.01f, -1.3f, mtxWorld);
	DrawCube(BLUE4, -1.01f, -11.01f, -1.3f, mtxWorld);

	mtxWorld = ComputeWorldMatrix(LEFT_LEG);

	DrawCube(SKIN, 2.01f, -9.01f, -0.7f, mtxWorld);
	DrawCube(SKIN, 1.01f, -9.01f, -0.7f, mtxWorld);
	DrawCube(SKIN, 2.01f, -10.01f, -0.7f, mtxWorld);
	DrawCube(SKIN, 1.01f, -10.01f, -0.7f, mtxWorld);
	DrawCube(BLUE4, 2.01f, -11.01f, -0.7f, mtxWorld);
	DrawCube(BLUE4, 1.01f, -11.01f, -0.7f, mtxWorld);
	DrawCube(SKIN, 2.01f, -9.01f, -1.3f, mtxWorld);
	DrawCube(SKIN, 1.01f, -9.01f, -1.3f, mtxWorld);
	DrawCube(SKIN, 2.01f, -10.01f, -1.3f, mtxWorld);
	DrawCube(SKIN, 1.01f, -10.01f, -1.3f, mtxWorld);
	DrawCube(BLUE4, 2.01f, -11.01f, -1.3f, mtxWorld);
	DrawCube(BLUE4, 1.01f, -11.01f, -1.3f, mtxWorld);
}

void DrawCube(int type, float x, float y, float z, XMMATRIX mtxWorld, XMFLOAT3 rotate, XMFLOAT3 scale)
{
	XMMATRIX mtxFinal = XMMatrixIdentity();
	XMMATRIX mtxLocalScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	mtxFinal = XMMatrixMultiply(mtxFinal, mtxLocalScale);

	// �g�̂̉�]��K�p
	XMMATRIX mtxLocalRotate = XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	mtxFinal = XMMatrixMultiply(mtxFinal, mtxLocalRotate);

	XMMATRIX mtxLocalTranslate = XMMatrixTranslation(SIZE_WH * x * 2, SIZE_WH * y * 2, SIZE_WH * z * 2);
	mtxFinal = XMMatrixMultiply(mtxFinal, mtxLocalTranslate);

	mtxFinal = XMMatrixMultiply(mtxFinal, mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxFinal);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	ID3D11Buffer* vertexArrayBuffer = GetVertexArrayBuffer(type);
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexArrayBuffer, &stride, &offset);

	//// �|���S���`��
	GetDeviceContext()->Draw(4 * MAX_POLYGON, 0);
}

XMMATRIX ComputeWorldMatrix(int part)
{
	XMMATRIX mtxBodyWorld, mtxPartLocal, mtxPartWorld;

	// �g�̂̃��[���h�}�g���b�N�X�F�g�̂̃X�P�[���A��]�A���s�ړ����܂�
	mtxBodyWorld = XMMatrixIdentity();

	// �g�̂̃X�P�[����K�p
	XMMATRIX mtxBodyScale = XMMatrixScaling(g_Player.trans[ALL].scl.x, g_Player.trans[ALL].scl.y, g_Player.trans[ALL].scl.z);
	mtxBodyWorld = XMMatrixMultiply(mtxBodyWorld, mtxBodyScale);

	// �g�̂̉�]��K�p
	XMMATRIX mtxBodyRotation = XMMatrixRotationRollPitchYaw(g_Player.trans[ALL].rot.x, g_Player.trans[ALL].rot.y, g_Player.trans[ALL].rot.z);
	mtxBodyWorld = XMMatrixMultiply(mtxBodyWorld, mtxBodyRotation);

	// �g�̂̕��s�ړ���K�p
	XMMATRIX mtxBodyTranslate = XMMatrixTranslation(g_Player.trans[ALL].pos.x, g_Player.trans[ALL].pos.y, g_Player.trans[ALL].pos.z);
	mtxBodyWorld = XMMatrixMultiply(mtxBodyWorld, mtxBodyTranslate);

	// �����̃��[�J���}�g���b�N�X�F�����̉�]�I�t�Z�b�g�ƕ��s�ړ����܂�
	mtxPartLocal = XMMatrixIdentity();

	// �����̃X�P�[����K�p
	XMMATRIX mtxPartScale = XMMatrixScaling(g_Player.trans[part].scl.x, g_Player.trans[part].scl.y, g_Player.trans[part].scl.z);
	mtxPartLocal = XMMatrixMultiply(mtxPartLocal, mtxPartScale);

	XMMATRIX mtxTranslateToLocalCenter = XMMatrixTranslation(-g_Player.trans[part].rotateCenter.x, -g_Player.trans[part].rotateCenter.y, -g_Player.trans[part].rotateCenter.z);

	// �����̑��Ή�]�I�t�Z�b�g��K�p
	XMMATRIX mtxPartRotation;
	switch (part)
	{
	case TWINTAIL:
		mtxPartRotation = XMMatrixRotationRollPitchYaw(
			g_Player.trans[TWINTAIL].rot.x + g_Player.trans[HEAD].rot.x,
			g_Player.trans[TWINTAIL].rot.y + g_Player.trans[HEAD].rot.y,
			g_Player.trans[TWINTAIL].rot.z + g_Player.trans[HEAD].rot.z
		);
		break;
	case HEAD:
	case LEFT_EYE:
	case RIGHT_EYE:
		mtxPartRotation = XMMatrixRotationRollPitchYaw(
			g_Player.trans[HEAD].rot.x,
			g_Player.trans[HEAD].rot.y,
			g_Player.trans[HEAD].rot.z
		);
		break;
	case BODY:
		mtxPartRotation = XMMatrixRotationRollPitchYaw(
			g_Player.trans[BODY].rot.x,
			g_Player.trans[BODY].rot.y,
			g_Player.trans[BODY].rot.z
		);
		break;
	case LEFT_ARM:
		mtxPartRotation = XMMatrixRotationRollPitchYaw(
			g_Player.trans[LEFT_ARM].rot.x,
			g_Player.trans[LEFT_ARM].rot.y,
			g_Player.trans[LEFT_ARM].rot.z
		);
		break;
	case RIGHT_ARM:
		mtxPartRotation = XMMatrixRotationRollPitchYaw(
			g_Player.trans[RIGHT_ARM].rot.x,
			g_Player.trans[RIGHT_ARM].rot.y,
			g_Player.trans[RIGHT_ARM].rot.z
		);
		break;
	case LEFT_LEG:
		mtxPartRotation = XMMatrixRotationRollPitchYaw(
			g_Player.trans[LEFT_LEG].rot.x,
			g_Player.trans[LEFT_LEG].rot.y,
			g_Player.trans[LEFT_LEG].rot.z
		);
		break;
	case RIGHT_LEG:
		mtxPartRotation = XMMatrixRotationRollPitchYaw(
			g_Player.trans[RIGHT_LEG].rot.x,
			g_Player.trans[RIGHT_LEG].rot.y,
			g_Player.trans[RIGHT_LEG].rot.z
		);
		break;
	case WAND:
		mtxPartRotation = XMMatrixRotationRollPitchYaw(
			g_Player.trans[WAND].rot.x,
			g_Player.trans[WAND].rot.y,
			g_Player.trans[WAND].rot.z
		);
		break;
	default:
		return mtxBodyWorld;
	}

	XMMATRIX mtxTranslateBack = XMMatrixTranslation(g_Player.trans[part].rotateCenter.x, g_Player.trans[part].rotateCenter.y, g_Player.trans[part].rotateCenter.z);

	XMMATRIX mtxPartRotateTransform = XMMatrixMultiply(mtxTranslateToLocalCenter, mtxPartRotation);
	mtxPartRotateTransform = XMMatrixMultiply(mtxPartRotateTransform, mtxTranslateBack);

	mtxPartLocal = XMMatrixMultiply(mtxPartLocal, mtxPartRotateTransform);

	// �����̑��Ε��s�ړ��i�g�̂ɑ΂���ʒu�j��K�p
	XMMATRIX mtxPartTranslate = XMMatrixTranslation(g_Player.trans[part].pos.x, g_Player.trans[part].pos.y, g_Player.trans[part].pos.z);
	mtxPartLocal = XMMatrixMultiply(mtxPartLocal, mtxPartTranslate);

	// �����̍ŏI�I�ȃ��[���h�}�g���b�N�X���v�Z
	mtxPartWorld = XMMatrixMultiply(mtxPartLocal, mtxBodyWorld);

	// �����̃��[���h�}�g���b�N�X�̌��ʂ�ۑ�
	XMStoreFloat4x4(&g_Player.trans[part].mtxWorld, mtxPartWorld);

	return mtxPartWorld;
}

void DrawBullet(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Player.bullet[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Player.bullet[i].scl.x, g_Player.bullet[i].scl.y, g_Player.bullet[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Player.bullet[i].rot.x, g_Player.bullet[i].rot.y + XM_PI, g_Player.bullet[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Player.bullet[i].pos.x, g_Player.bullet[i].pos.y, g_Player.bullet[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Player.bullet[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Player.bullet[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}
//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

