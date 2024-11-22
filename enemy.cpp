//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
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
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/neko.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define ENEMY_SHADOW_SIZE	(1.1f)						// 影の大きさ
#define ENEMY_OFFSET_Y		(7.0f)						// エネミーの足元をあわせる
#define ENEMY_SCALE			(1.8f)

#define	TEXTURE_MAX			(2)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// エネミー
static UISprite			g_HPGauge[MAX_ENEMY];
int g_Enemy_load = 0;

static ID3D11Buffer* g_VertexBuffer = NULL;	// 頂点バッファ
static char* g_TextureName[] = {
	"data/TEXTURE/EnemyHPGauge.png",
	"data/TEXTURE/EnemyHPGauge_bg.png",
};
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static INTERPOLATION_DATA g_MoveTbl0[] = {
	//座標									回転率							拡大率					時間
	{ XMFLOAT3(50.0f,  0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
	{ XMFLOAT3(250.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },/*
	{ XMFLOAT3(50.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(125.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(250.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },*/
};


static INTERPOLATION_DATA g_MoveTbl1[] = {
	//座標									回転率							拡大率							時間
	{ XMFLOAT3(450.0f,  0.0f, 55.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
	{ XMFLOAT3(250.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
};


static INTERPOLATION_DATA g_MoveTbl2[] = {
	//座標									回転率							拡大率							時間
	{ XMFLOAT3(50.0f,  0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
	{ XMFLOAT3(250.0f, 0.0f, 0.0f),	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(ENEMY_SCALE, ENEMY_SCALE, ENEMY_SCALE),	600 },
};

static INTERPOLATION_DATA g_MoveTbl3[] = {
	//座標									回転率							拡大率							時間
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
// 初期化処理
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

		g_Enemy[i].spd  = 0.0f;			// 移動スピードクリア
		g_Enemy[i].size = ENEMY_SIZE;	// 当たり判定の大きさ

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Enemy[i].model, &g_Enemy[i].diffuse[0]);


		g_Enemy[i].jumpUp = FALSE;
		g_Enemy[i].jumpCnt = 0;
		g_Enemy[i].jumpYMax = 0.9f;

		g_Enemy[i].move = XMFLOAT3(4.0f, 0.0f, 0.0f);		// 移動量

		g_Enemy[i].time = 0.0f;			// 線形補間用のタイマーをクリア
		g_Enemy[i].tblNo = 0;			// 再生する行動データテーブルNoをセット
		g_Enemy[i].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

		g_Enemy[i].shadowSize = ENEMY_SHADOW_SIZE;
		g_Enemy[i].shadowIdx = CreateShadow(g_Enemy[i].pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_Enemy[i].use = true;		// true:生きてる

		ZeroMemory(&g_HPGauge[i].material, sizeof(g_HPGauge[i].material));
		g_HPGauge[i].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_HPGauge[i].pos = XMFLOAT3(0.0f, 0.1f, 0.0f);
		g_HPGauge[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_HPGauge[i].fWidth = 0.0f;
		g_HPGauge[i].fHeight = 0.0f;
		g_HPGauge[i].bUse = TRUE;

	}

	// 0番だけ線形補間で動かしてみる
	g_Enemy[0].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	// 1番だけ線形補間で動かしてみる
	g_Enemy[1].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[1].tblNo = 1;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	// 2番だけ線形補間で動かしてみる
	g_Enemy[2].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[2].tblNo = 2;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

	// 2番だけ線形補間で動かしてみる
	g_Enemy[3].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Enemy[3].tblNo = 3;		// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[3].tblMax = sizeof(g_MoveTbl3) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット

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
// 終了処理
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
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	// エネミーを動かす場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == true)		// このエネミーが使われている？
		{								// Yes
			
				// 移動処理
			if (g_Enemy[i].tblMax > 0)	// 線形補間を実行する？
			{	// 線形補間の処理
				int nowNo = (int)g_Enemy[i].time;			// 整数分であるテーブル番号を取り出している
				int maxNo = g_Enemy[i].tblMax;				// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている
				INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Enemy[i].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
				XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
				XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
				XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
				XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

				float nowTime = g_Enemy[i].time - nowNo;	// 時間部分である少数を取り出している

				Pos *= nowTime;								// 現在の移動量を計算している
				Rot *= nowTime;								// 現在の回転量を計算している
				Scl *= nowTime;								// 現在の拡大率を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				float oldY = g_Enemy[i].pos.y;
				XMStoreFloat3(&g_Enemy[i].pos, nowPos + Pos);
				g_Enemy[i].pos.y += oldY;

				// 計算して求めた回転量を現在の移動テーブルに足している
				XMStoreFloat3(&g_Enemy[i].rot, nowRot + Rot);

				// 計算して求めた拡大率を現在の移動テーブルに足している
				XMStoreFloat3(&g_Enemy[i].scl, nowScl + Scl);

				XMVECTOR direction = XMVectorSubtract(nowPos, Pos);
				direction = XMVector3Normalize(direction);
				g_Enemy[i].targetDir = atan2(XMVectorGetZ(direction), XMVectorGetX(direction));
				g_Enemy[i].targetDir += XM_PI / 2;
				// frameを使て時間経過処理をする
				g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
				if ((int)g_Enemy[i].time >= maxNo)			// 登録テーブル最後まで移動したか？
				{
					g_Enemy[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
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
	//	// モデルの色を変更できるよ！半透明にもできるよ。
	//	for (int j = 0; j < g_Enemy[0].model.SubsetNum; j++)
	//	{
	//		SetModelDiffuse(&g_Enemy[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	// モデルの色を元に戻している
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
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == false) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Enemy[i].model);

		//DrawHPGauge(i);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);

}

void DrawHPGauge(int idx)
{
	// ライティングを無効
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA* cam = GetCamera();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (g_HPGauge[idx].bUse)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// ビューマトリックスを取得
		mtxView = XMLoadFloat4x4(&cam->mtxView);


		// なにかするところ
		// 正方行列（直交行列）を転置行列させて逆行列を作ってる版(速い)
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

		// スケールを反映
		mtxScl = XMMatrixScaling(g_HPGauge[idx].scl.x, g_HPGauge[idx].scl.y, g_HPGauge[idx].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_HPGauge[idx].pos.x, g_HPGauge[idx].pos.y, g_HPGauge[idx].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);


		// マテリアル設定
		SetMaterial(g_HPGauge[idx].material);

		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// ポリゴンの描画
		GetDeviceContext()->Draw(4, 0);
	}

	// ライティングを有効に
	SetLightEnable(TRUE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexHPGauge(int width, int height)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// 頂点バッファに値をセットする
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = width;
	float fHeight = height;

	// 頂点座標の設定
	//vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight, 0.0f);
	//vertex[1].Position = XMFLOAT3(fWidth / 2.0f, fHeight, 0.0f);
	//vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, 0.0f, 0.0f);
	//vertex[3].Position = XMFLOAT3(fWidth / 2.0f, 0.0f, 0.0f);
	vertex[0].Position = XMFLOAT3(0.0f, fHeight, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth, fHeight, 0.0f);
	vertex[2].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth, 0.0f, 0.0f);

	// 法線の設定
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 拡散光の設定
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}
