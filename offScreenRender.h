#pragma once
//=============================================================================
//
// [offScreenRender.h]
// Author : 
//
//=============================================================================
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

struct OffSCREEN_CBUFFER
{
	int			mode;
	int			padding[3];
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitOffScreenRender(void);
void UninitOffScreenRender(void);

void DrawOffScreenRender(void);
void SetOffScreenRender(void);
void SetOffScreenModeBuffer(int);


