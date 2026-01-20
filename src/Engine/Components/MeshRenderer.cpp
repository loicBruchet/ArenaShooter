#include "pch.h"
#include "MeshRenderer.h"

#include <Geometry.h>
#include <FrameResources.h>

#include "GameObject.h"

namespace gce {
    void MeshRenderer::SetAlbedoTexture(Texture const* texture)
    {
        if (texture == nullptr)
            return CONSOLE_LOG("Albedo texture is null", Console::LogType::ERROR_);
            
        pMaterial->albedoTextureID = texture->GetTextureID();
        pMaterial->useTextureAlbedo = 1;
    }

    void MeshRenderer::SetMetalnessTexture(Texture const* texture)
    {
        if (texture == nullptr)
            return CONSOLE_LOG("Metalness texture is null", Console::LogType::ERROR_);
        
        pMaterial->metalnessTextureID = texture->GetTextureID();
        pMaterial->useTextureMetalness = 1;
    }

    void MeshRenderer::SetCustomPso(const D12PipelineObject* pso)
    {
        if (pso == nullptr)
            return CONSOLE_LOG("PSO is null", Console::LogType::ERROR_);
        
        pPso = pso;
        pMaterial->useTextureAlbedo = 0;
	}

    void MeshRenderer::SetRoughnessTexture(Texture const* texture)
    {
        if (texture == nullptr)
            return CONSOLE_LOG("Roughness texture is null", Console::LogType::ERROR_);
        
        pMaterial->roughnessTextureID = texture->GetTextureID();
        pMaterial->useTextureRoughness = 1;
    }

    void MeshRenderer::SetDisplacementTexture(Texture const* texture)
    {
        if (texture == nullptr)
            return CONSOLE_LOG("Displacement texture is null", Console::LogType::ERROR_);
        
        pMaterial->displacementTextureID = texture->GetTextureID();
        pMaterial->useDisplacementTexture = 1;
    }

    void MeshRenderer::SetNormalTexture(Texture const* texture)
    {
        if (texture == nullptr)
            return CONSOLE_LOG("Normal texture is null", Console::LogType::ERROR_);
        
        pMaterial->normalTextureID = texture->GetTextureID();
        pMaterial->useTextureNormal= 1;
    }

    void MeshRenderer::SetAmbientTexture(Texture const* texture)
    {
        if (texture == nullptr)
            return CONSOLE_LOG("Ambient texture is null", Console::LogType::ERROR_);
        
        pMaterial->ambientTextureID = texture->GetTextureID();
        pMaterial->useTextureAmbient = 1;
    }

    void MeshRenderer::SetSubsurfaceScattering(float32 value)
    {
        pMaterial->subsurface = value;
    }

    void MeshRenderer::SetFresnelR0(Vector3f32 const& vector)
    {
        pMaterial->fresnelR0 = vector;
    }

    void MeshRenderer::SetGeometry(Geometry* geometry)
    {
        if (geometry == nullptr)
            return CONSOLE_LOG("Geometry is null", Console::LogType::ERROR_);
        
        pGeometry = geometry;
    }

    Geometry const* MeshRenderer::GetGeometry() const
    {
        return pGeometry;
    }

    void MeshRenderer::Init()
    {
        m_pOwner->transform.dirtyEvent.AddListener( &MeshRenderer::UpdateWorld, this );
        FrameResourcesManager::AddRenderItem( *this );
        pPso = GameManager::GetDefaultPso();
        pMaterial->subsurface = 0;
        pMaterial->useDisplacementTexture = 0;
        pMaterial->useTextureAlbedo = 0;
        pMaterial->useTextureAmbient = 0;
        pMaterial->useTextureMetalness = 0;
        pMaterial->useTextureNormal = 0;
        pMaterial->useTextureRoughness = 0;
    }


void MeshRenderer::UpdateWorld( uint8 const dirty )
{
    objData.world = m_pOwner->transform.GetWorldMatrix();
    Matrix matrix = objData.world;
    matrix.Transpose();
    objData.world = matrix;
}


}
