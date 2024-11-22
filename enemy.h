//=============================================================================
//
// エネミーモデル処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY		(5)					// エネミーの数
#define JUMP_CNT_MAX	(120)
#define	ENEMY_SIZE		(65.0f)				// 当たり判定の大きさ
#define ROTATION_SPEED				(0.18f)
enum
{
	ENEMY_IDLE,
	ENEMY_WALK,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ENEMY
{
	float				HP;
	float				maxHP;
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)

	BOOL				jumpUp;
	int					jumpCnt;
	float				jumpYMax;
	int					state;
	bool				use;
	bool				load;
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色

	float				spd;				// 移動スピード
	float				size;				// 当たり判定の大きさ
	int					shadowIdx;			// 影のインデックス番号
	float				shadowSize;

	XMFLOAT3	move;			// 移動速度
	float		dir;
	float		targetDir;

	float		time;			// 線形補間用
	int			tblNo;			// 行動データのテーブル番号
	int			tblMax;			// そのテーブルのデータ数

};


struct UISprite
{
	XMFLOAT3	pos;			// 位置
	XMFLOAT3	scl;			// スケール
	MATERIAL	material;		// マテリアル
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	BOOL		bUse;			// 使用しているかどうか

};

//*****************************************************************************
// プロトタイプ宣言
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
