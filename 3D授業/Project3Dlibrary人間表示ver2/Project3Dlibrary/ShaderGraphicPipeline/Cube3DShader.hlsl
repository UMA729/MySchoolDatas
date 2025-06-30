// ルートシグネチャの宣言
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
"CBV(b0), " \
"CBV(b1), " \
"DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL),"\
"CBV(b2), " \
"StaticSampler(s0, "\
"filter         = FILTER_MIN_MAG_MIP_LINEAR, "\
"addressU       = TEXTURE_ADDRESS_WRAP, "\
"addressV       = TEXTURE_ADDRESS_WRAP, "\
"addressW       = TEXTURE_ADDRESS_WRAP, "\
"mipLodBias     = 0.0f, "\
"maxAnisotropy  = 0, "\
"comparisonFunc = COMPARISON_NEVER, "\
"borderColor    = STATIC_BORDER_COLOR_TRANSPARENT_BLACK, "\
"minLOD         = 0.0f, "\
"maxLOD         = 3.402823466e+38f, "\
"space          = 0, "\
"visibility     = SHADER_VISIBILITY_ALL)"

//コンスタントバッファ
cbuffer cbTansMatrix : register(b0)
{
	float4x4 WVP;
};

//マテリアルバッファ
cbuffer cbMateine : register(b1)
{
    float4 diffuse;
    float4 speclar;
    float4 shininess_to_amb;
}

//ボーンバッファ
cbuffer cbBoneMatrix : register(b2)
{
    float4x4 bone_mat[128];
}

Texture2D<float4> ColorTexture : register(t0);
SamplerState ColorSmp : register(s0);

struct PSInput
{
	float4	position	: SV_POSITION;
    float4  pos         : V_POS;
	float4	color		: COLOR;
    float2  uv			: TEX_UV;
};

[RootSignature(RS)]
PSInput VSMain(float4 position : POSITION, float4 color : COLOR,float2 uv : TEX_UV,float3 tan:TAN,uint4 id:ID,float4 we:WE)
{
	PSInput	result;

    //スキンアニメーション
    float4 sp = float4(0, 0, 0, 1);           //posの同次元wを1にする
    float4 skin_pos = float4(0, 0, 0, 0);     //ボーン行列の影響を受けたpos
    float3 skin_nor = float3(0, 0, 0);        //ボーン行列の影響を受けたnor
    sp.xyz = position.xyz;
    
    for (int i = 0; i < 4;i++)
    {
        skin_pos += (mul(bone_mat[id[i]],sp) * (float4)we[i]);
        skin_nor += (mul(bone_mat[id[i]],color) * (float4) we[i]);
    }
    
    //WVP:ワールドビューパース

    skin_pos.w = 1.0f;
    
    result.position = mul(WVP, skin_pos);         //頂点位置（ピクセル化で使用する（ラスタライズ））
    result.pos      = mul(WVP, skin_pos); //頂点位置（PSMainで使用する）
    
    result.color    = mul(WVP, skin_nor.xyz); //面の向き（正規化）
    result.uv       = uv;
	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 c = input.color;
	
    //ライト
    float3 L = normalize(float3(0, 1, 0) - (input.pos.xyz / input.pos.w));
    //法線（面の向き）
    float3 N = normalize(input.color.xyz);
    
    //ライトベクトルと面の向きで内積を求め、陰影値を獲得する
    float d;
    d = dot(L, N);
    if (d < 0.0)
        d = 0.0;
    
        c = diffuse*d;
    
     //テクスチャとUVから色情報獲得
    float4 t = ColorTexture.Sample(ColorSmp, input.uv);
    c *= t;
    c.a = 1.0f;//透過なし
	return  c;
}