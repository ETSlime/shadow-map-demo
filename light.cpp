//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include "player.h"
#include "input.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(1.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(3000.0f)										// �r���[���ʂ�FarZ�l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];
static LightViewProjBuffer g_LightViewProj;
static FOG		g_Fog;

static BOOL		g_FogEnable = FALSE;


//=============================================================================
// ����������
//=============================================================================
void InitLight(void)
{

	//���C�g������
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		g_Light[i].Direction = XMFLOAT3( 0.0f, -1.0f, 0.0f );
		g_Light[i].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		g_Light[i].Ambient   = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
		g_Light[i].Attenuation = 100.0f;	// ��������
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ���C�g�̃^�C�v
		g_Light[i].Enable = FALSE;			// ON / OFF
		SetLight(i, &g_Light[i]);

		g_LightViewProj.ProjView[i] = XMMatrixIdentity();
	}


	g_Light[1].Enable = FALSE;									// ���̃��C�g��ON
	g_Light[0].Enable = TRUE;

	// �t�H�O�̏������i���̌��ʁj
	g_Fog.FogStart = 100.0f;									// ���_���炱�̋��������ƃt�H�O��������n�߂�
	g_Fog.FogEnd   = 250.0f;									// �����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
	g_Fog.FogColor = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );		// �t�H�O�̐F
	SetFog(&g_Fog);
	SetFogEnable(g_FogEnable);				// ���̏ꏊ���`�F�b�N���� shadow

}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateLight(void)
{
	PLAYER* player = GetPlayer();

	// ���s�����̐ݒ�i���E���Ƃ炷���j
	g_Light[1].Direction = XMFLOAT3(-1.0f, -15.0f, -13.0f);		// ���̌���
	g_Light[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	// ���̐F
	g_Light[1].Type = LIGHT_TYPE_DIRECTIONAL;					// ���s����

	g_Light[1].Ambient = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	g_Light[1].Position = XMFLOAT3(600.0f, 500.0f, 250.0f);

	//g_Light[0].Direction = XMFLOAT3(-1.0f, -15.0f, -13.0f);		// ���̌���
	g_Light[0].Direction = XMFLOAT3(1.1f, -1.0f, 0.0f);		// ���̌���
	g_Light[0].Diffuse = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);	// ���̐F
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;					// ���s����
	//g_Light[0].Position = XMFLOAT3(-400.0f, 200.0f, 50.0f);

	g_Light[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f); // player->trans[ALL].pos;
	g_Light[0].Position.y += 11;
	g_Light[0].Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	SetLight(0, &g_Light[0]);
	XMFLOAT3 targetPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 lightUp = { 0.0f, 1.0f, 0.0f };
	XMVECTOR pos = XMLoadFloat3(&g_Light[0].Position);
	XMVECTOR lightDir = XMLoadFloat3(&g_Light[0].Direction);
	XMMATRIX lightView = XMMatrixLookAtLH(
		XMLoadFloat3(&g_Light[0].Position),
		pos + lightDir,
		XMLoadFloat3(&lightUp)
	);
	XMMATRIX lightProj = XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_WIDTH, VIEW_NEAR_Z, VIEW_FAR_Z);
	g_LightViewProj.ProjView[0] = XMMatrixTranspose(lightView * lightProj);


	//targetPosition = XMFLOAT3(player->trans[ALL].pos.x, player->trans[ALL].pos.y, player->trans[ALL].pos.z);
	XMVECTOR dir = XMVector3Normalize(
		XMVectorSubtract(XMLoadFloat3(&targetPosition), XMLoadFloat3(&g_Light[1].Position))
	);
	XMFLOAT3 lightDirection;
	XMStoreFloat3(&lightDirection, dir);
	g_Light[1].Direction = lightDirection;
	lightUp = { 0.0f, 1.0f, 0.0f };
	//lightView = XMMatrixLookAtLH(
	//	XMLoadFloat3(&g_Light[1].Position),
	//	XMLoadFloat3(&targetPosition),
	//	XMLoadFloat3(&lightUp)
	//);
	lightView = XMMatrixLookAtLH(
		XMLoadFloat3(&g_Light[1].Position),
		XMLoadFloat3(&targetPosition),
		XMLoadFloat3(&lightUp)
	);
	lightProj = XMMatrixOrthographicLH(SCREEN_WIDTH * 1.2f, SCREEN_HEIGHT * 1.2f, VIEW_NEAR_Z, VIEW_FAR_Z);
	g_LightViewProj.ProjView[1] = XMMatrixTranspose(lightView * lightProj);

	SetLight(1, &g_Light[1]);									// ����Őݒ肵�Ă���

	if (GetKeyboardTrigger(DIK_L))
	{
		if (g_Light[0].Enable == TRUE)
			g_Light[0].Enable = FALSE;
		else
			g_Light[0].Enable = TRUE;

		if (g_Light[1].Enable == TRUE)
			g_Light[1].Enable = FALSE;
		else
			g_Light[1].Enable = TRUE;

	}
}

void SetLightViewProjBuffer(int lightIdx)
{
	g_LightViewProj.LightIndex = lightIdx;
	//SetLightProjView2(&g_LightViewProj.ProjView[lightIdx]);
	SetLightProjView(&g_LightViewProj);
}


//=============================================================================
// ���C�g�̐ݒ�
// Type�ɂ���ăZ�b�g���郁���o�[�ϐ����ς���Ă���
//=============================================================================
void SetLightData(int index, LIGHT *light)
{
	SetLight(index, light);
}


LIGHT *GetLightData(int index)
{
	return(&g_Light[index]);
}


//=============================================================================
// �t�H�O�̐ݒ�
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}


BOOL	GetFogEnable(void)
{
	return(g_FogEnable);
}


