//=============================================================================
//
// �G�l�~�[���f������ [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "enemy.h"
#include "shadow.h"
#include "camera.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/neko.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define ENEMY_SHADOW_SIZE	(1.1f)						// �e�̑傫��
#define ENEMY_OFFSET_Y		(7.0f)						// �G�l�~�[�̑��������킹��
#define ENEMY_SCALE			(1.8f)

#define	TEXTURE_MAX			(2)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// �G�l�~�[
static UISprite			g_HPGauge[MAX_ENEMY];
int g_Enemy_load = 0;

static ID3D11Buffer* g_VertexBuffer = NULL;	// ���_�o�b�t�@
static char* g_TextureName[] = {
	"data/TEXTURE/EnemyHPGauge.png",
	"data/TEXTURE/EnemyHPGauge_bg.png",
};
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static INTERPOLATION_DATA g_MoveTbl0[] = {
	//���W									��]��							�g�嗦					����
	{ XMFLOAT3(50.0f,  0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
	{ XMFLOAT3(250.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },/*
	{ XMFLOAT3(50.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(125.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(250.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },*/
};


static INTERPOLATION_DATA g_MoveTbl1[] = {
	//���W									��]��							�g�嗦							����
	{ XMFLOAT3(450.0f,  0.0f, 55.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
	{ XMFLOAT3(250.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
};


static INTERPOLATION_DATA g_MoveTbl2[] = {
	//���W									��]��							�g�嗦							����
	{ XMFLOAT3(50.0f,  0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
	{ XMFLOAT3(250.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
};

static INTERPOLATION_DATA g_MoveTbl3[] = {
	//���W									��]��							�g�嗦							����
	{ XMFLOAT3(471.0f,  0.0f, -437.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
	{ XMFLOAT3(752.0f, 0.0f, 742.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
};


static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,
	g_MoveTbl3,

};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	MakeVertexHPGauge(35, 5);

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		g_Enemy[i].load = true;

		g_Enemy[i].maxHP = 20.0f;
		g_Enemy[i].HP = 20.0f;

		g_Enemy[i].pos = XMFLOAT3(-50.0f + i * 30.0f, -70.0f, 20.0f);
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE);

		g_Enemy[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Enemy[i].size = ENEMY_SIZE;	// �����蔻��̑傫��

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Enemy[i].model, &g_Enemy[i].diffuse[0]);


		g_Enemy[i].jumpUp = FALSE;
		g_Enemy[i].jumpCnt = 0;
		g_Enemy[i].jumpYMax = 0.9f;

		g_Enemy[i].move = XMFLOAT3(4.0f, 0.0f, 0.0f);		// �ړ���

		g_Enemy[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		g_Enemy[i].shadowSize = ENEMY_SHADOW_SIZE;
		g_Enemy[i].shadowIdx = CreateShadow(g_Enemy[i].pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_Enemy[i].use = true;		// true:�����Ă�

		ZeroMemory(&g_HPGauge[i].material, sizeof(g_HPGauge[i].material));
		g_HPGauge[i].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_HPGauge[i].pos = XMFLOAT3(0.0f, 0.1f, 0.0f);
		g_HPGauge[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_HPGauge[i].fWidth = 0.0f;
		g_HPGauge[i].fHeight = 0.0f;
		g_HPGauge[i].bUse = TRUE;

	}

	// 0�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 1�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[1].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[1].tblNo = 1;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 2�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[2].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[2].tblNo = 2;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 2�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[3].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Enemy[3].tblNo = 3;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[3].tblMax = sizeof(g_MoveTbl3) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	g_Enemy[4].scl = XMFLOAT3(ENEMY_SCALE * 5, ENEMY_SCALE * 5, ENEMY_SCALE * 5);
	g_Enemy[4].pos.x += 233;
	g_Enemy[4].pos.z -= 333;
	g_Enemy[4].rot.y = 2;
	g_Enemy[4].size = ENEMY_SIZE * 5;
	g_Enemy[4].pos.y -= 35;
	g_Enemy[4].maxHP = 80;
	g_Enemy[4].HP = 80;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].load)
		{
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = false;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == true)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes
			
				// �ړ�����
			if (g_Enemy[i].tblMax > 0)	// ���`��Ԃ����s����H
			{	// ���`��Ԃ̏���
				int nowNo = (int)g_Enemy[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Enemy[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Enemy[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

				float nowTime = g_Enemy[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
				Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
				Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				float oldY = g_Enemy[i].pos.y;
				XMStoreFloat3(&g_Enemy[i].pos, nowPos + Pos);
				g_Enemy[i].pos.y += oldY;

				// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

				// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
				XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);

				XMVECTOR direction = XMVectorSubtract(nowPos, Pos);
				direction = XMVector3Normalize(direction);
				g_Enemy[i].targetDir = atan2(XMVectorGetZ(direction), XMVectorGetX(direction));
				g_Enemy[i].targetDir += XM_PI / 2;
				// frame���g�Ď��Ԍo�ߏ���������
				g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
				if ((int)g_Enemy[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Enemy[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
				}
			}

			float deltaDir = g_Enemy[i].targetDir - g_Enemy[i].dir;
			if (deltaDir > XM_PI) deltaDir -= 2 * XM_PI;
			if (deltaDir < -XM_PI) deltaDir += 2 * XM_PI;
			g_Enemy[i].dir += deltaDir * ROTATION_SPEED;
			if (i != 4)
				g_Enemy[i].rot.y = g_Enemy[i].dir;

			PlayEnemyWalkAnim(&g_Enemy[i]);
			if (g_Enemy[i].state == ENEMY_WALK)
			{
				PlayEnemyWalkAnim(&g_Enemy[i]);
			}

			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y = (-40.0f - ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);

			UpdateHPGauge(i);

			if (g_Enemy[i].HP <= 0.0f)
				g_Enemy[i].use = FALSE;

		}
	}




#ifdef _DEBUG

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	// ���f���̐F��ύX�ł����I�������ɂ��ł����B
	//	for (int j = 0; j < g_Enemy[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Enemy[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// ���f���̐F�����ɖ߂��Ă���
	//	for (int j = 0; j < g_Enemy[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Enemy[0].model, j, g_Enemy[0].diffuse[j]);
	//	}
	//}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_Enemy[1].state = ENEMY_WALK;
	}
#endif

}

void UpdateHPGauge(int idx)
{
	g_HPGauge[idx].pos = g_Enemy[idx].pos;
	g_HPGauge[idx].pos.y += ENEMY_SIZE;
	g_HPGauge[idx].pos.x -= 15.0f;
}

void PlayEnemyWalkAnim(ENEMY* enemy)
{
	float angle = (XM_PI / JUMP_CNT_MAX) * enemy->jumpCnt;
	enemy->jumpCnt++;

	if (enemy->jumpUp == FALSE)
	{
		enemy->scl.y += 0.008f;
		enemy->pos.y += enemy->jumpYMax * cosf(angle);
		enemy->shadowSize -= 0.0085f;
		//SetShadowSize(enemy->shadowIdx, enemy->shadowSize, enemy->shadowSize);
	}
	else
	{
		enemy->scl.y -= 0.008f;
		enemy->pos.y -= enemy->jumpYMax * cosf(angle);
		enemy->shadowSize += 0.0085f;
		//SetShadowSize(enemy->shadowIdx, enemy->shadowSize, enemy->shadowSize);
	}

	if (enemy->jumpCnt > JUMP_CNT_MAX * 0.5f)
	{
		enemy->jumpUp = !enemy->jumpUp;
		enemy->jumpCnt = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Enemy[i].model);

		//DrawHPGauge(i);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);

}

void DrawHPGauge(int idx)
{
	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA* cam = GetCamera();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (g_HPGauge[idx].bUse)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �r���[�}�g���b�N�X���擾
		mtxView = XMLoadFloat4x4(&cam->mtxView);


		// �Ȃɂ�����Ƃ���
		// �����s��i�����s��j��]�u�s�񂳂��ċt�s�������Ă��(����)
		mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
		mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
		mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

		mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
		mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
		mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

		mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
		mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
		mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];

		int ratio = g_Enemy[idx].HP / g_Enemy[idx].maxHP;
		MakeVertexHPGauge(35.0f * ratio, 5.0f);

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_HPGauge[idx].scl.x, g_HPGauge[idx].scl.y, g_HPGauge[idx].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_HPGauge[idx].pos.x, g_HPGauge[idx].pos.y, g_HPGauge[idx].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);


		// �}�e���A���ݒ�
		SetMaterial(g_HPGauge[idx].material);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// �|���S���̕`��
		GetDeviceContext()->Draw(4, 0);
	}

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexHPGauge(int width, int height)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// ���_�o�b�t�@�ɒl���Z�b�g����
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = width;
	float fHeight = height;

	// ���_���W�̐ݒ�
	//vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight, 0.0f);
	//vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight, 0.0f);
	//vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, 0.0f, 0.0f);
	//vertex[3].Position = XMFLOAT3(fWidth / 2.0f, 0.0f, 0.0f);
	vertex[0].Position = XMFLOAT3(0.0f, fHeight, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth, fHeight, 0.0f);
	vertex[2].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth, 0.0f, 0.0f);

	// �@���̐ݒ�
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// �g�U���̐ݒ�
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}
