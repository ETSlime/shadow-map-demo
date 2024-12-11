//=============================================================================
//
// MapEditor処理 [MapEditor.cpp]
// Author : 
//
//=============================================================================
#include "MapEditor.h"
#include "enemy.h"
#include "debugproc.h"
#include "camera.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_POS_CUROSR			"data/MODEL/posCursor.obj"		// 読み込むモデル名

MapEditor::MapEditor()
{
	curModelCnt = 0;
	curSelectedModelIdx = -1;
	onEditorCursor = FALSE;
}

void MapEditor::Init()
{
	LoadModel(MODEL_POS_CUROSR, &posCursor.cursorX.model);
	LoadModel(MODEL_POS_CUROSR, &posCursor.cursorY.model);
	LoadModel(MODEL_POS_CUROSR, &posCursor.cursorZ.model);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	material.noTexSampling = TRUE;
	posCursor.cursorX.material = material;

	material.Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	posCursor.cursorY.material = material;
	material.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	posCursor.cursorZ.material = material;

	XMStoreFloat4x4(&posCursor.cursorX.mtxWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&posCursor.cursorY.mtxWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&posCursor.cursorZ.mtxWorld, XMMatrixIdentity());
}

void MapEditor::Update()
{
	UpdateMouseDrag();

	BOOL isOnEditorCursor = FALSE;
	isOnEditorCursor |= UpdateEditorSelect(&posCursor.cursorX, GetMousePosX(), GetMousePosY());
	isOnEditorCursor |= UpdateEditorSelect(&posCursor.cursorY, GetMousePosX(), GetMousePosY());
	isOnEditorCursor |= UpdateEditorSelect(&posCursor.cursorZ, GetMousePosX(), GetMousePosY());
	onEditorCursor = isOnEditorCursor;

	if (deltaX != 0 || deltaY != 0)
	{
		PrintDebugProc("deltaX:%f deltaY:%f\n", deltaX, deltaY);
	}

	ENEMY* enemy = GetEnemy();
	if (posCursor.cursorX.model.isCursorIn == TRUE)
	{

		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (enemy[i].model.editorIdx == curSelectedModelIdx)
			{
				enemy[i].pos.x += deltaX;
				PrintDebugProc("enemy PosX:%f", enemy[i].pos.x);
			}
		}
	}
	else if (posCursor.cursorY.model.isCursorIn == TRUE)
	{

		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (enemy[i].model.editorIdx == curSelectedModelIdx)
			{
				enemy[i].pos.y -= deltaY;
				PrintDebugProc("enemy PosY:%f", enemy[i].pos.y);
			}
		}
	}
	else if (posCursor.cursorZ.model.isCursorIn == TRUE)
	{

		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (enemy[i].model.editorIdx == curSelectedModelIdx)
			{
				enemy[i].pos.z += deltaX;
				PrintDebugProc("enemy Posz:%f", enemy[i].pos.z);
			}
		}
	}
}

void MapEditor::UpdateMouseDrag()
{

	if (IsMouseLeftTriggered()) {
		isDragging = TRUE;
		startX = GetMousePosX();
		startY = GetMousePosY();
		currentX = startX;
		currentY = startY;
		deltaX = 0;
		deltaY = 0;
	}
	else if (IsMouseLeftPressed() && isDragging) {
		
		long newX = GetMousePosX();
		long newY = GetMousePosY();
		deltaX = newX - currentX;
		deltaY = newY - currentY;
		currentX = newX;
		currentY = newY;
	}
	else if (!IsMouseLeftPressed() && isDragging) {
		isDragging = FALSE;
	}
}

void MapEditor::Draw()
{
	if (GetRenderMode() == RENDER_MODE_SHADOW) return;

	DX11_MODEL* model = GetCurSelectedModel();

	if (model)
	{
		SetDepthEnable(FALSE);

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// カリング無効
		SetCullingMode(CULL_MODE_NONE);

		ENEMY* enemy = GetEnemy();
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			if (enemy[i].model.editorIdx == curSelectedModelIdx)
			{

				XMVECTOR minPoint = XMLoadFloat3(&enemy[i].model.boundingBox.minPoint);
				XMVECTOR maxPoint = XMLoadFloat3(&enemy[i].model.boundingBox.maxPoint);
				XMMATRIX worldMatrix = XMLoadFloat4x4(&enemy[i].mtxWorld);
				XMVECTOR corners[8];
				corners[0] = XMVectorSet(minPoint.m128_f32[0], minPoint.m128_f32[1], minPoint.m128_f32[2], 1.0f);
				corners[1] = XMVectorSet(maxPoint.m128_f32[0], minPoint.m128_f32[1], minPoint.m128_f32[2], 1.0f);
				corners[2] = XMVectorSet(minPoint.m128_f32[0], maxPoint.m128_f32[1], minPoint.m128_f32[2], 1.0f);
				corners[3] = XMVectorSet(maxPoint.m128_f32[0], maxPoint.m128_f32[1], minPoint.m128_f32[2], 1.0f);
				corners[4] = XMVectorSet(minPoint.m128_f32[0], minPoint.m128_f32[1], maxPoint.m128_f32[2], 1.0f);
				corners[5] = XMVectorSet(maxPoint.m128_f32[0], minPoint.m128_f32[1], maxPoint.m128_f32[2], 1.0f);
				corners[6] = XMVectorSet(minPoint.m128_f32[0], maxPoint.m128_f32[1], maxPoint.m128_f32[2], 1.0f);
				corners[7] = XMVectorSet(maxPoint.m128_f32[0], maxPoint.m128_f32[1], maxPoint.m128_f32[2], 1.0f);

				XMVECTOR minWorld = XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, 1.0f);
				XMVECTOR maxWorld = XMVectorSet(-FLT_MAX, -FLT_MAX, -FLT_MAX, 1.0f);

				for (int i = 0; i < 8; ++i) {
					XMVECTOR worldCorner = XMVector3TransformCoord(corners[i], worldMatrix);
					minWorld = XMVectorMin(minWorld, worldCorner);
					maxWorld = XMVectorMax(maxWorld, worldCorner);
				}
				XMVECTOR centerWorld = XMVectorScale(XMVectorAdd(minWorld, maxWorld), 0.5f);

				CAMERA* camera = GetCamera();
				XMFLOAT3 worldPosition = XMFLOAT3(enemy[i].pos.x, enemy[i].pos.y, enemy[i].pos.z);
				XMVECTOR modelPosition = XMLoadFloat3(&worldPosition);
				XMVECTOR cameraPosition = XMLoadFloat3(&camera->pos);
				XMVECTOR distanceVector = XMVectorSubtract(modelPosition, cameraPosition);
				float cameraDistance = XMVectorGetX(XMVector3Length(distanceVector));

				float projectionFactor = SCREEN_HEIGHT / tanf(camera->fov * 0.5f) * 30;

				float desiredScreenSize = 100.0f;
				float modelScale = desiredScreenSize * (cameraDistance / projectionFactor);
				mtxScl = XMMatrixScaling(modelScale, modelScale, modelScale);

				// 平移向量を追加（例：Y方向に5単位平移）
				XMVECTOR offset = XMVectorSet(0.0f, 15.0f, 0.0f, 0.0f);
				XMVECTOR newCenterWorld = XMVectorAdd(centerWorld, offset);

				mtxTranslate = XMMatrixTranslationFromVector(newCenterWorld);

				// スケールを反映
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

				// 回転を反映
				mtxRot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

				// 移動を反映
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);


				// ワールドマトリックスの設定
				SetWorldMatrix(&mtxWorld);
				XMStoreFloat4x4(&posCursor.cursorY.mtxWorld, mtxWorld);
				if (posCursor.cursorY.model.isCursorIn == TRUE)
				{
					MATERIAL material;
					ZeroMemory(&material, sizeof(material));
					material.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
					material.noTexSampling = TRUE;
					SetMaterial(material);
				}
				else
					SetMaterial(posCursor.cursorY.material);
				DrawModel(&posCursor.cursorY.model);

				offset = XMVectorSet(15.0f, 0.0f, 0.0f, 0.0f);
				newCenterWorld = XMVectorAdd(centerWorld, offset);

				mtxTranslate = XMMatrixTranslationFromVector(newCenterWorld);

				mtxWorld = XMMatrixIdentity();
				// スケールを反映
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

				// 回転を反映
				mtxRot = XMMatrixRotationRollPitchYaw(-XM_PI * 0.5f, 0.0f, 0.0f);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

				// 移動を反映
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

				SetWorldMatrix(&mtxWorld);
				XMStoreFloat4x4(&posCursor.cursorZ.mtxWorld, mtxWorld);
				if (posCursor.cursorZ.model.isCursorIn == TRUE)
				{
					MATERIAL material;
					ZeroMemory(&material, sizeof(material));
					material.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
					material.noTexSampling = TRUE;
					SetMaterial(material);
				}
				else
					SetMaterial(posCursor.cursorZ.material);
				DrawModel(&posCursor.cursorZ.model);
				

				offset = XMVectorSet(0.0f, 0.0f, 15.0f, 0.0f);
				newCenterWorld = XMVectorAdd(centerWorld, offset);

				mtxTranslate = XMMatrixTranslationFromVector(newCenterWorld);

				mtxWorld = XMMatrixIdentity();
				// スケールを反映
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

				// 回転を反映
				mtxRot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, XM_PI * 0.5f);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

				// 移動を反映
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

				SetWorldMatrix(&mtxWorld);
				XMStoreFloat4x4(&posCursor.cursorX.mtxWorld, mtxWorld);
				if (posCursor.cursorX.model.isCursorIn == TRUE)
				{
					MATERIAL material;
					ZeroMemory(&material, sizeof(material));
					material.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
					material.noTexSampling = TRUE;
					SetMaterial(material);
				}
				else
					SetMaterial(posCursor.cursorX.material);
				DrawModel(&posCursor.cursorX.model);
				break;
			}

		}

		SetDepthEnable(TRUE);
	}
}

void MapEditor::Uninit()
{
}

BOOL MapEditor::UpdateEditorSelect(CursorModel* cursorModel, int sx, int sy)
{
	CAMERA* camera = GetCamera();
	XMMATRIX P = XMLoadFloat4x4(&camera->mtxProjection);

	// Compute picking ray in view space.
	float vx = (+2.0f * sx / SCREEN_WIDTH - 1.0f) / P.r[0].m128_f32[0];
	float vy = (-2.0f * sy / SCREEN_HEIGHT + 1.0f) / P.r[1].m128_f32[1];

	// Ray definition in view space.
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	// Tranform ray to local space of Mesh.
	XMMATRIX V = XMLoadFloat4x4(&camera->mtxView);
	XMMATRIX invView = XMLoadFloat4x4(&camera->mtxInvView);

	XMMATRIX W = XMLoadFloat4x4(&cursorModel->mtxWorld);
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	// Make the ray direction unit length for the intersection tests.
	rayDir = XMVector3Normalize(rayDir);

	XMVECTOR minPoint = XMLoadFloat3(&cursorModel->model.boundingBox.minPoint);
	XMVECTOR maxPoint = XMLoadFloat3(&cursorModel->model.boundingBox.maxPoint);


	float tMin = 0.0f;
	float tMax = FLT_MAX;
	BOOL intersect = FALSE;
	for (int i = 0; i < 3; ++i)
	{
		float rayOriginComponent = XMVectorGetByIndex(rayOrigin, i);
		float rayDirComponent = XMVectorGetByIndex(rayDir, i);
		float minComponent = XMVectorGetByIndex(minPoint, i);
		float maxComponent = XMVectorGetByIndex(maxPoint, i);

		if (fabs(rayDirComponent) < 1e-8f) {
			if (rayOriginComponent < minComponent || rayOriginComponent > maxComponent)
			{
				intersect = FALSE;
				cursorModel->model.isCursorIn = FALSE;
				return FALSE;
			}
		}
		else
		{
			float t1 = (minComponent - rayOriginComponent) / rayDirComponent;
			float t2 = (maxComponent - rayOriginComponent) / rayDirComponent;

			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			tMin = max(tMin, t1);
			tMax = min(tMax, t2);

			if (tMin > tMax)
			{
				intersect = FALSE;
				cursorModel->model.isCursorIn = FALSE;
				return FALSE;
			}
		}
	}

	cursorModel->model.isCursorIn = TRUE;
	return TRUE;
}

int MapEditor::AddToList(DX11_MODEL* model)
{
	if (curModelCnt >= MAX_MODEL)
		return -1;

	modelList[curModelCnt] = model;
	model->editorIdx = curModelCnt;
	curModelCnt++;

	return curModelCnt;
}

DX11_MODEL* MapEditor::GetCurSelectedModel()
{
	if (curSelectedModelIdx == -1 || curSelectedModelIdx >= MAX_MODEL)
		return nullptr;

	return modelList[curSelectedModelIdx];
}

