#include "pch.h"
#include "Camera.h"

#include "GameTransform.h"
#include "GameObject.h"

namespace gce {


void Camera::SetMainCamera()
{
    if ( m_mainCamera ) return;
    GameManager::GetMainCameras().push_back( this );
    m_mainCamera = true;
}

void Camera::RemoveMainCamera()
{
    if ( m_mainCamera == false ) return;
    std::vector<Camera*>& mainCameras = GameManager::GetMainCameras();
    for ( auto it = mainCameras.begin(); it != mainCameras.end(); ++it )
        if ( *it == this )
            mainCameras.erase( it );
    m_mainCamera = false;
}


void Camera::Init()
{
    m_pOwner->transform.dirtyEvent.AddListener( &Camera::UpdateCamera, this );
}


void Camera::UpdateCamera( uint8 const dirty )
{
    SetMatrix( m_pOwner->transform.GetWorldMatrix() );
}


}