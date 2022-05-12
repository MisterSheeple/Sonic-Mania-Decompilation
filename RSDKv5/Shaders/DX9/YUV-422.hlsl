// =======================
// VARIABLES
// =======================

SamplerState texDiffuse: register(s0);  // screen display texture

float2 pixelSize: register(c0);         // internal game resolution (usually 424x240 or smth)
float2 textureSize: register(c1);       // size of the internal framebuffer texture
float2 viewSize: register(c2);          // window viewport size

#if defined(RETRO_REV02)                // if RETRO_REV02 is defined it assumes the engine is plus/rev02 RSDKv5, else it assumes pre-plus/Rev01 RSDKv5
float2 screenDim: register(c3);         // screen dimming percent
#endif


// =======================
// STRUCTS
// =======================

struct VertexInput
{
    float4 pos      : POSITION0;
    float4 color    : COLOR0;
    float4 tex      : TEXCOORD0;
};

struct VertexOutput
{
    float4 pos      : POSITION0;
    float4 color    : COLOR0;
    float4 tex      : TEXCOORD0;
};

struct PixelInput
{
    float2 tex : TEXCOORD0;
};

// =======================
// ENTRY POINTS
// =======================

VertexOutput VSMain(VertexInput input)
{
	VertexOutput output;
	
	output.pos      = input.pos;
	output.color    = input.color;
	output.tex      = input.tex;
	
	return output;
}

float4 PSMain(PixelInput input) : SV_TARGET
{
    float4 outColor;

    float3 yuv;
    yuv.r  = tex2D(texDiffuse, input.tex.xy).r;
    yuv.gb = tex2D(texDiffuse, float2(clamp(input.tex.x / 2.0, 0, 0.499), input.tex.y)).gb;
    yuv -= float3(16.0 / 256, 0.5, 0.5);

    outColor.r = 1.164 * yuv.r + 1.596 * yuv.b;
    outColor.g = 1.164 * yuv.r - 0.392 * yuv.g - 0.813 * yuv.b;
    outColor.b = 1.164 * yuv.r + 2.017 * yuv.g;
    outColor.a = 1.0;

#if defined(RETRO_REV02) 
    return outColor * screenDim.x;
#else
    return outColor;
#endif
}