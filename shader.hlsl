

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

struct LightViewProjBuffer
{
    matrix ProjView[5];
    int LightIndex;
    int padding[3];
};

struct MODE
{
    int mode;
    int padding[3];
};

// �}�g���N�X�o�b�t�@
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}

cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}

cbuffer ProjectionBuffer : register( b2 )
{
	matrix Projection;
}

cbuffer ProjViewBuffer : register(b8)
{
    LightViewProjBuffer ProjView;
}

cbuffer ModeBuffer : register(b10)
{
    MODE md;
}

// �}�e���A���o�b�t�@
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	int			noTexSampling;
	float		Dummy[2];//16byte���E�p
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ���C�g�p�o�b�t�@
struct LIGHT
{
	float4		Direction[5];
	float4		Position[5];
	float4		Diffuse[5];
	float4		Ambient[5];
	float4		Attenuation[5];
	int4		Flags[5];
	int			Enable;
	int			Dummy[3];//16byte���E�p
    float4x4    LightViewProj;
};

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
}

struct FOG
{
	float4		Distance;
	float4		FogColor;
	int			Enable;
	float		Dummy[3];//16byte���E�p
};

// �t�H�O�p�o�b�t�@
cbuffer FogBuffer : register( b5 )
{
	FOG			Fog;
};

// �����p�o�b�t�@
cbuffer Fuchi : register(b6)
{
	int			fuchi;
	int			fill[3];
};


cbuffer CameraBuffer : register(b7)
{
	float4 Camera;
}



//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void VertexShaderPolygon( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0,
						  out float4 outWorldPos    : POSITION0,
								out float4 outshadowCoord[5] : TEXCOORD1)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));

	outTexCoord = inTexCoord;

	outWorldPos = mul(inPosition, World);
    for (int i = 0; i < 5; ++i)
    {
        outshadowCoord[i] = mul(outWorldPos, ProjView.ProjView[i]);
    }

	outDiffuse = inDiffuse;

    //outshadowCoord = float4(Light.LightViewProj._43, Light.LightViewProj._42, Light.LightViewProj._43, Light.LightViewProj._44);
    //outshadowCoord = float4(ProjView._43, ProjView._42, ProjView._13, ProjView._44);

}



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_Texture : register( t0 );
SamplerState	g_SamplerState : register( s0 );
Texture2D g_ShadowMap[5] : register(t1);
Texture2D g_TextureSmall : register(t7);
SamplerComparisonState g_ShadowSampler : register(s1);

//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void PixelShaderPolygon( in  float4 inPosition		: SV_POSITION,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float4 inWorldPos      : POSITION0,
						in float4 inShadowCoord[5] : TEXCOORD1,

						 out float4 outDiffuse		: SV_Target )
{
    float4 color;
	

	if (Material.noTexSampling == 0)
	{
		color = g_Texture.Sample(g_SamplerState, inTexCoord);

		color *= inDiffuse;
	}
	else
	{
		color = inDiffuse;
    }

	if (Light.Enable == 0)
	{
		color = color * Material.Diffuse;
	}
	else
	{
		float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

        for (int i = 0; i < 5; i++)
		{
			float3 lightDir;
			float light;

			if (Light.Flags[i].y == 1)
			{
                float4 ambient = color * Material.Diffuse * Light.Ambient[i];
				if (Light.Flags[i].x == 1)
				{
					lightDir = normalize(Light.Direction[i].xyz);
                    light = saturate(dot(lightDir, inNormal.xyz));
                    float backlightFactor = saturate(dot(-lightDir, inNormal.xyz));

					light = 0.5 - 0.5 * light;
					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];
                    //tempColor += color * Material.Diffuse * backlightFactor * Light.Diffuse[i];
                }
				else if (Light.Flags[i].x == 2)
				{
					lightDir = normalize(Light.Position[i].xyz - inWorldPos.xyz);
					light = dot(lightDir, inNormal.xyz);

					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];

					float distance = length(inWorldPos - Light.Position[i]);

					float att = saturate((Light.Attenuation[i].x - distance) / Light.Attenuation[i].x);
					tempColor *= att;
				}
				else
				{
					tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
				}
                float shadowFactor = 1.0f;
                if (Light.Flags[i].x == 1)
                {
                    float2 shadowTexCoord = float2(inShadowCoord[i].x, -inShadowCoord[i].y) / inShadowCoord[i].w * 0.5f + 0.5f;
                    //shadowTexCoord.xy = inShadowCoord.xy / inShadowCoord.w * 0.5f + 0.5f;
                    //shadowTexCoord.y = 1.0f - shadowTexCoord.y;
                    float currentDepth = inShadowCoord[i].z / inShadowCoord[i].w;
                    currentDepth -= 0.005f;
      //              if (shadowTexCoord.x >= 0.0f && shadowTexCoord.y >= 0.0f &&
						//shadowTexCoord.x <= 1.0f && shadowTexCoord.y <= 1.0f &&
						//currentDepth >= 0.0f && currentDepth <= 1.0f)
      //              {
      //                  tempColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
      //              }
                    //tempColor = float4(currentDepth, currentDepth, currentDepth, 1.0f);
                    //float shadowMapValue = g_ShadowMap[2].Sample(g_SamplerState, shadowTexCoord).r;
                   // tempColor = float4(shadowMapValue, shadowMapValue, shadowMapValue, 1.0f);
                    int kernelSize = 1;
                    float2 shadowMapDimensions = float2(960, 540);
                    float shadow = 0.0;
                    float2 texelSize = 1.0 / shadowMapDimensions;
                    float totalWeight = 0.0;
                    for (int x = -kernelSize; x <= kernelSize; x++)
                    {
                        for (int y = -kernelSize; y <= kernelSize; y++)
                        {
                            float weight = exp(-(x * x + y * y) / (2.0 * kernelSize * kernelSize)); // gaussian weight
                            shadow += g_ShadowMap[i].SampleCmpLevelZero(g_ShadowSampler, shadowTexCoord + float2(x, y) * texelSize, currentDepth) * weight;
                            totalWeight += weight;
                        }
                    }
                    shadowFactor = shadow / totalWeight;
                    //shadowFactor = g_ShadowMap[i].SampleCmpLevelZero(g_ShadowSampler, shadowTexCoord, currentDepth);
					//if (shadowFactor == 0.0f)
     //                   shadowFactor = 0.4f;
					
                    //shadowFactor = g_ShadowMap.SampleCmpLevelZero(g_ShadowSampler, shadowTexCoord, currentDepth);
                }

                tempColor *= shadowFactor;
				
                outColor += tempColor + ambient;
                //outColor = float4(ambient.x, ambient.x, ambient.x, 1.f);

            }
		}

		color = outColor;
		color.a = inDiffuse.a * Material.Diffuse.a;

    }

	//�t�H�O
	if (Fog.Enable == 1)
	{
		float z = inPosition.z*inPosition.w;
		float f = (Fog.Distance.y - z) / (Fog.Distance.y - Fog.Distance.x);
		f = saturate(f);
		outDiffuse = f * color + (1 - f)*Fog.FogColor;
		outDiffuse.a = color.a;
	}
	else
	{
		outDiffuse = color;
	}
	
    if (md.mode == 1)
    {

        //color.a *= 0.5f;
        float2 centeredTexcoord = inTexCoord - float2(0.5, 0.5);

        float distanceFromCenter = length(centeredTexcoord);
        float angle = distanceFromCenter * 5;

        float sinAngle = sin(angle);
        float cosAngle = cos(angle);
        float2 rotatedTexcoord;
        rotatedTexcoord.x = centeredTexcoord.x * cosAngle - centeredTexcoord.y * sinAngle;
        rotatedTexcoord.y = centeredTexcoord.x * sinAngle + centeredTexcoord.y * cosAngle;

        rotatedTexcoord += float2(0.5, 0.5);
		
        color = g_TextureSmall.Sample(g_SamplerState, rotatedTexcoord);
        outDiffuse = color;
        return;

    }
	
 

	
	//�����
	//if (fuchi == 1)
	//{
	//	float angle = dot(normalize(inWorldPos.xyz - Camera.xyz), normalize(inNormal));
	//	//if ((angle < 0.5f)&&(angle > -0.5f))
	//	if (angle > -0.3f)
	//	{
	//		outDiffuse.rb  = 1.0f;
	//		outDiffuse.g = 0.0f;			
	//	}
	//}
}
