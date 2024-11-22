//=============================================================================
//
// �G�l�~�[���f������ [enemy.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY		(5)					// �G�l�~�[�̐�
#define JUMP_CNT_MAX	(120)
#define	ENEMY_SIZE		(65.0f)				// �����蔻��̑傫��
#define ROTATION_SPEED				(0.18f)
enum
{
	ENEMY_IDLE,
	ENEMY_WALK,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct ENEMY
{
	float				HP;
	float				maxHP;
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)

	BOOL				jumpUp;
	int					jumpCnt;
	float				jumpYMax;
	int					state;
	bool				use;
	bool				load;
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F

	float				spd;				// �ړ��X�s�[�h
	float				size;				// �����蔻��̑傫��
	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�
	float				shadowSize;

	XMFLOAT3	move;			// �ړ����x
	float		dir;
	float		targetDir;

	float		time;			// ���`��ԗp
	int			tblNo;			// �s���f�[�^�̃e�[�u���ԍ�
	int			tblMax;			// ���̃e�[�u���̃f�[�^��

};


struct UISprite
{
	XMFLOAT3	pos;			// �ʒu
	XMFLOAT3	scl;			// �X�P�[��
	MATERIAL	material;		// �}�e���A��
	float		fWidth;			// ��
	float		fHeight;		// ����
	BOOL		bUse;			// �g�p���Ă��邩�ǂ���

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);
void UpdateHPGauge(int idx);
void DrawHPGauge(int idx);
void PlayEnemyWalkAnim(ENEMY* enemy);
HRESULT MakeVertexHPGauge(int w, int h);
