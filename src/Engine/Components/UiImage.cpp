#include "pch.h"
#include "UiImage.h"
#include "GameObject.h"
#include <Event.hpp>
namespace gce
{
    void UiImage::Init()
    {
        m_pOwner->transform.dirtyEvent.AddListener(&UiImage::UpdateRect, this);
    }

    void UiImage::InitializeImage(Vector2f32 const& pos, Vector2f32 const& size, float32 const opacity)
    {
        this->size = size;
        this->position = pos;
        this->opacity = opacity;
        rectImage = new RectanglePosF(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
        
        if (btmBrush)
        {
            btmBrush->SetOpacity(opacity);
        }
    }
    
    void UiImage::SetOpacity(float32 opacity)
    {
        this->opacity = opacity;
        if (btmBrush)
        {
            btmBrush->SetOpacity(opacity);
        }
    }
    
    void UiImage::UpdateRect(uint8 dirty)
    {
        Vector3f32 const& position = m_pOwner->transform.GetWorldPosition();
        Vector3f32 const& scale = m_pOwner->transform.GetWorldScale();

        rectImage->left = position.x - 0.5f * scale.x;
        rectImage->top = position.y - 0.5f * scale.y;
        rectImage->right = position.x + 0.5f * scale.x;
        rectImage->bottom = position.y + 0.5f * scale.y;

        if (btmBrush)
        {
            Vector3f32 const brushScale = scale / Vector3f32{ btmBrush->GetSize().x, btmBrush->GetSize().y, 1.0f };
            btmBrush->SetTransformMatrix({ rectImage->left, rectImage->top, 0.0f }, brushScale, 0.0f);
            btmBrush->SetOpacity(opacity);
        }
    }
} // namespace gce