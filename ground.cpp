//=============================================================================
//
// �G�l�~�[���f������ [Ground.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "ground.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_GROUND		"data/MODEL/tree.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define GROUND_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define GROUND_OFFSET_Y		(7.0f)						// �G�l�~�[�̑��������킹��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static GROUND			g_Ground[MAX_GROUND];				// �G�l�~�[

int g_Ground_load = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGround(void)
{
	for (int i = 0; i < MAX_GROUND; i++)
	{
		LoadModel(MODEL_GROUND, &g_Ground[i].model);
		g_Ground[i].load = true;

		g_Ground[i].pos = XMFLOAT3(-50.0f + i * 130.0f, -70.0f, -1720.0f - i * 130.0f);
		g_Ground[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Ground[i].scl = XMFLOAT3(12.5f, 12.5f, 12.5f);

		g_Ground[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Ground[i].size = GROUND_SIZE;	// �����蔻��̑傫��

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Ground[i].model, &g_Ground[i].diffuse[0]);

		g_Ground[i].use = true;		// true:�����Ă�

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGround(void)
{

	for (int i = 0; i < MAX_GROUND; i++)
	{
		if (g_Ground[i].load)
		{
			UnloadModel(&g_Ground[i].model);
			g_Ground[i].load = false;
		}
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGround(void)
{
	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_GROUND; i++)
	{
		if (g_Ground[i].use == true)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes

		}
	}



#ifdef _DEBUG

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGround(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_GROUND; i++)
	{
		if (g_Ground[i].use == false) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Ground[i].scl.x, g_Ground[i].scl.y, g_Ground[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Ground[i].rot.x, g_Ground[i].rot.y + XM_PI, g_Ground[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Ground[i].pos.x, g_Ground[i].pos.y, g_Ground[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Ground[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Ground[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}
