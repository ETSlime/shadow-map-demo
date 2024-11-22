//=============================================================================
//
// エネミーモデル処理 [Ground.cpp]
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
// マクロ定義
//*****************************************************************************
#define	MODEL_GROUND		"data/MODEL/tree.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define GROUND_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define GROUND_OFFSET_Y		(7.0f)						// エネミーの足元をあわせる


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static GROUND			g_Ground[MAX_GROUND];				// エネミー

int g_Ground_load = 0;

//=============================================================================
// 初期化処理
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

		g_Ground[i].spd  = 0.0f;			// 移動スピードクリア
		g_Ground[i].size = GROUND_SIZE;	// 当たり判定の大きさ

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Ground[i].model, &g_Ground[i].diffuse[0]);

		g_Ground[i].use = true;		// true:生きてる

	}

	return S_OK;
}

//=============================================================================
// 終了処理
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
// 更新処理
//=============================================================================
void UpdateGround(void)
{
	// エネミーを動かす場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_GROUND; i++)
	{
		if (g_Ground[i].use == true)		// このエネミーが使われている？
		{								// Yes

		}
	}



#ifdef _DEBUG

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGround(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_GROUND; i++)
	{
		if (g_Ground[i].use == false) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Ground[i].scl.x, g_Ground[i].scl.y, g_Ground[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Ground[i].rot.x, g_Ground[i].rot.y + XM_PI, g_Ground[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Ground[i].pos.x, g_Ground[i].pos.y, g_Ground[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Ground[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Ground[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}
