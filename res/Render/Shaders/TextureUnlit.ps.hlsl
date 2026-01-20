#include "Common.hlsli"

cbuffer ObjectData : register(b0)
{
    float4x4 world;
    Material material;
}

Texture2D textures[] : register(t0);
cbuffer TexID : register(b3) { uint textureID; };
sampler textureSampler : register(s0);

float4 main(VertexOut vIn) : SV_Target
{
    if(material.useTextureAlbedo == 0) {
        return float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    return textures[textureID].Sample(textureSampler, vIn.uv);
}