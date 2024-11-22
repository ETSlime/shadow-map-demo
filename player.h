//=============================================================================
//
// ���f������ [player.h]
// Author : 
//
//=============================================================================
#pragma once

#include "Vertex.h"
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER		(1)					// �v���C���[�̐�
#define TEXTURE_MAX		(1)						// �e�N�X�`���̐�
#define	PLAYER_SIZE		(5.0f)				// �����蔻��̑傫��

#define MAX_GROUP					12
#define MAX_POLYGON					(6)						// �L���[�u�P������̖ʐ�

#define SPD_DECAY_RATE				(0.95f)
#define ROTATION_SPEED				(0.18f)
#define SHAKE_HEAD_SPD				(XM_PI * 0.008f)
#define NOD_HEAD_SPD				(XM_PI * 0.004f)
#define WINK_HEAD_SPD_X				(XM_PI * 0.001f)
#define WINK_HEAD_SPD_Z				(XM_PI * 0.002f)
#define WINK_ARM_SPD_Y				(XM_PI * 0.003f)
#define WINK_ARM_SPD_X				(XM_PI * 0.003f)
#define WINK_LEFT_ARM_SPD_Z			(XM_PI * 0.0005f)
#define WINK_LEG_SPD_Y				(XM_PI * 0.001f)
#define WINK_LEG_SPD_X				(XM_PI * 0.0013f)
#define MAX_SHAKE_HEAD_DEG			(XM_PI * 0.2f)
#define MAX_NOD_HEAD_DEG			(XM_PI * 0.15f)
#define MAX_ARM_DEG					(XM_PI * 0.12f)
#define MAX_LEG_DEG					(XM_PI * 0.07f)
#define MAX_ARM_DEG_RUN				(XM_PI * 0.15f)
#define MAX_LEG_DEG_RUN				(XM_PI * 0.1f)
#define BLINK_SPD					(0.04f)
#define WINK_SPD					(0.03f)
#define ARM_SPD						(XM_PI * 0.005f)
#define LEG_SPD						(XM_PI * 0.002f)
#define ARM_SPD_RUN					(XM_PI * 0.015f)
#define LEG_SPD_RUN					(XM_PI * 0.006f)
#define TWINTAIL_FLUC_DEG_WALK		(XM_PI * 0.03f)
#define TWINTAIL_MAX_DEG_WALK		(XM_PI * 0.05f)
#define TWINTAIL_FLUC_SPD_WALK		(XM_PI * 0.0005f)
#define TWINTAIL_FLUC_DEG_RUN		(XM_PI * 0.06f)
#define TWINTAIL_MAX_DEG_RUN		(XM_PI * 0.09f)
#define TWINTAIL_FLUC_SPD_RUN		(XM_PI * 0.0009f)
#define MAX_RUN_BODY_DEG			(XM_PI * 0.12f)
#define BODY_ROT_SPD				(XM_PI * 0.006f)
#define BODY_ROT_ATTACK_SPD_X		(XM_PI * 0.012f)
#define BODY_ROT_ATTACK_SPD_Y		(XM_PI * 0.004f)
#define ARM_ROT_ATTACK_SPD_Y		(XM_PI * 0.005f)
#define ARM_ROT_ATTACK_SPD_Z		(XM_PI * 0.004f)
#define WINK_WAIT					(30)
#define	STOP_RUN_BODY_WAIT			(55)
#define JUMP_CNT_MAX				(60)
#define STOP_RUN_ARM_WAVE_SPD		(XM_PI * 0.009f)
#define STOP_RUN_MAX_ARM_DEG		(XM_PI * 0.035f)
#define JUMP_ARM_WAVE_SPD			(XM_PI * 0.005f)
#define JUMP_LEG_WAVE_SPD			(XM_PI * 0.002f)
#define JUMP_MAX_ARM_DEG			(XM_PI * 0.065f)
#define JUMP_MAX_LEG_DEG			(XM_PI * 0.015f)

#define ATTACK_BODY_WAIT			(45)
#define MAX_ATTACK_BODY_DEG			(XM_PI * 0.15f)
#define WAND_ROT_ATTACK_SPD_X		(XM_PI * 0.012f)

#define MAX_BULLET					(155)
enum
{
	HEAD,
	TWINTAIL,
	BODY,
	SKIRT,
	LEFT_ARM,
	RIGHT_ARM,
	LEFT_LEG,
	RIGHT_LEG,
	LEFT_EYE,
	RIGHT_EYE,
	WAND,
	ALL,
};

// states
enum
{
	IDLE,
	WALK,
	RUN,
	DASH,
	ATTACK,
	JUMP,
	FALL,
	HARD_LANDING,
	HIT,
	KNOCKDOWN,
	REBOUND,
	DEFEND,
	CAST,
	DIE,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct TRANS
{
	XMFLOAT3		pos;		// �|���S���̈ʒu
	XMFLOAT3		rot;		// �|���S���̌���(��])
	XMFLOAT3		scl;		// �|���S���̑傫��(�X�P�[��)
	XMFLOAT3		rotateCenter;

	XMFLOAT4X4		mtxWorld;	// ���[���h�}�g���b�N�X
};

struct BULLET
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)
	DX11_MODEL			model;		// ���f�����

	float			spd;
	float			lifeTime;
	float			size;
	BOOL			use;
};

struct PLAYER
{
	TRANS			trans[MAX_GROUP];		// �|���S���f�[�^
	//XMFLOAT3		pos;		// �|���S���̈ʒu
	//XMFLOAT3		rot;		// �|���S���̌���(��])
	//XMFLOAT3		scl;		// �|���S���̑傫��(�X�P�[��)

	XMFLOAT4X4		mtxWorld;	// ���[���h�}�g���b�N�X

	bool			load;
	BULLET			bullet[MAX_BULLET];
	XMFLOAT4		diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	float			spd;		// �ړ��X�s�[�h
	float			dir;		// ����
	float			size;		// �����蔻��̑傫��
	int				shadowIdx;	// �e��Index
	bool			use;

	BOOL			disableInput;

	BOOL			shakeHead;
	BOOL			nodHead;
	BOOL			shakeDir;
	BOOL			nodDir;
	BOOL			blink;
	BOOL			eyeOpen;
	BOOL			wink;
	BOOL			winkEnd;
	BOOL			rightArmDir;
	BOOL			leftLegDir;
	BOOL			bodyDir;
	BOOL			armDir;
	BOOL			fluctuation;
	BOOL			twintailFluc;
	BOOL			twintailDown;
	BOOL			jump;
	BOOL			stopRun;
	BOOL			stopJump;
	int				state;
	int				walkCnt;
	int				winkWait;
	int				stopRunCnt;
	int				attackCnt;
	int				jumpCnt;
	int				jumpYMax;
	float			targetDir;

	XMFLOAT3		armRotDeg;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);

void DrawArm(void);
void DrawHead(void);
void DrawBody(void);
void DrawEye(void);
void DrawLeg(void);
void DrawWand(void);
void DrawCube(int type, float x, float y, float z, XMMATRIX mtxWorld, XMFLOAT3 rotate = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f));
XMMATRIX ComputeWorldMatrix(int part);
void ResetAllStates(void);
void HandlePlayerMove(void);
void HandleStopMove(void);
void ResetRotation(float& rotationOffset, float speed);
void PlayMoveAnim(void);
void PlayStopRunAnim(void);
void PlayJumpAnim(void);
void PlayAttackAnim(void);
void SetRotateCenter(int part, XMFLOAT3 center);
void ResetRotateCenter(void);
void ResetStopRunState(void);

void DrawBullet(void);
void UpdateBullet(void);