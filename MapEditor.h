#pragma once

#include "model.h"
#include "SingletonBase.h"

#define MAX_MODEL	500

struct CursorModel
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)

	MATERIAL			material;
	DX11_MODEL			model;
};

struct Cursor
{
	CursorModel		cursorX;
	CursorModel		cursorY;
	CursorModel		cursorZ;
};

class MapEditor : public SingletonBase<MapEditor>
{
public:
	MapEditor();
	void Init();
	void Update();
	void Draw();
	void Uninit();
	int	 AddToList(DX11_MODEL* model);

	void SetCurSelectedModelIdx(int idx) { curSelectedModelIdx = idx; }
	void ResetCurSelectedModelIdx() { curSelectedModelIdx = -1; }
	BOOL GetOnEditorCursor()  { return onEditorCursor; }
private:
	BOOL UpdateEditorSelect(CursorModel* model, int sx, int sy);
	DX11_MODEL* GetCurSelectedModel();
	void UpdateMouseDrag();
	Cursor posCursor, scaleCursor, rotateCursor;
	DX11_MODEL* modelList[MAX_MODEL];
	int	curModelCnt;
	int	curSelectedModelIdx;
	BOOL onEditorCursor;

	BOOL isDragging = FALSE;
	long startX = 0, startY = 0;
	long currentX = 0, currentY = 0;
	long deltaX = 0, deltaY = 0;
};