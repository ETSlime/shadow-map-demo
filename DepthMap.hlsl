

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

struct LightViewProjBuffer
{
    matrix ProjView[5];
    int LightIndex;
    int padding[3];
};

// �}�g���N�X�o�b�t�@
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}

cbuffer ViewBuffer : register(b1)
{
    matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

// �}�e���A���o�b�t�@
struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    int noTexSampling;
    float Dummy[2]; //16byte���E�p
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}

// ���C�g�p�o�b�t�@
struct LIGHT
{
    float4 Direction[5];
    float4 Position[5];
    float4 Diffuse[5];
    float4 Ambient[5];
    float4 Attenuation[5];
    int4 Flags[5];
    int Enable;
    int Dummy[3]; //16byte���E�p
    matrix LightViewProj;
};

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}

struct FOG
{
    float4 Distance;
    float4 FogColor;
    int Enable;
    float Dummy[3]; //16byte���E�p
};

// �t�H�O�p�o�b�t�@
cbuffer FogBuffer : register(b5)
{
    FOG Fog;
};

// �����p�o�b�t�@
cbuffer Fuchi : register(b6)
{
    int fuchi;
    int fill[3];
};


cbuffer CameraBuffer : register(b7)
{
    float4 Camera;
}

cbuffer ProjViewBuffer : register(b8)
{
    LightViewProjBuffer lightBuffer;
}

cbuffer ProjViewBuffer2 : register(b9)
{
    matrix ProjView2;
}


//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void VS(in float4 inPosition : POSITION0,
						  in float4 inNormal : NORMAL0,
						  in float4 inDiffuse : COLOR0,
						  in float2 inTexCoord : TEXCOORD0,

						  out float4 outPosition : SV_POSITION)
{
    float4 worldPosition = mul(inPosition, World);
    outPosition = mul(worldPosition, lightBuffer.ProjView[4]);
}



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================