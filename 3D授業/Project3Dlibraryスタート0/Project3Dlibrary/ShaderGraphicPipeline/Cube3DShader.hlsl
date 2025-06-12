// ルートシグネチャの宣言
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
"CBV(b0), " \
"DescriptorTable(SRV(t0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), visibility = SHADER_VISIBILITY_ALL),"\
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

Texture2D<float4> ColorTexture : register(t0);
SamplerState ColorSmp : register(s0);

struct PSInput
{
	float4	position	: SV_POSITION;
	float4	color		: COLOR;
    float2  uv			: TEX_UV;
};

[RootSignature(RS)]
PSInput VSMain(float4 position : POSITION, float4 color : COLOR,float2 uv : TEX_UV)
{
	PSInput	result;

	result.position = mul(WVP,position);
	result.color = color;
    result.uv = uv;
	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 c = input.color;
    float4 t = ColorTexture.Sample(ColorSmp, input.uv);//テクスチャとUVから色情報獲得
    c = t;
    c.a = 1.0f;//透過なし
	return  c;
}