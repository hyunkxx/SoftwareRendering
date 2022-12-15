#include "stdafx.h"
#include "..\Header\UI_MiniMap.h"
#include "Stage.h"
#include "Tank_01.h"
#include "Export_Function.h"
#include "StaticCamera.h"
#include "TankCamera.h"
#include "AimCamera.h"
#include "TankManager.h"
#include "UI_FontMgr.h"
#include "Humvee.h"
#include "SmallTank.h"
#include "MiddleTank.h"
#include "BigTank.h"
#include "LongTank.h"
CUI_MiniMap::CUI_MiniMap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CUI_MiniMap::CUI_MiniMap(const CUI_MiniMap& rhs)
	: Engine::CGameObject(rhs)
{
}

CUI_MiniMap::~CUI_MiniMap()
{
}

HRESULT CUI_MiniMap::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_fScaleX = 80.f;
	m_fScaleY = 57.5f;
	m_fScaleZ = 0.f;

	m_fPosX = 720.f;
	m_fPosY = 542.5f;
	m_fPosZ = 0.9f;

	m_pTransform->Set_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
	m_pTransform->Set_Pos(m_fPosX - (WINCX * 0.5f), (WINCY * 0.5f) - m_fPosY, m_fPosZ);

	return S_OK;
}


_int CUI_MiniMap::Update_Object(const _float & fTimeDelta)
{
	__super::Update_Object(fTimeDelta);

	return 0;
}

void CUI_MiniMap::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	
	Add_RenderGroup(RENDER_PRIORITY, this);
	
}

void CUI_MiniMap::Render_Object(void)
{


	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		_matrix OldViewMatrix, OldProjMatrix;


		m_pGraphicDev->GetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &OldProjMatrix);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

		_matrix	ViewMatrix;
		D3DXMatrixIdentity(&ViewMatrix);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &ViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pTexture->Set_Texture(0);
		m_pRcTex->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &OldViewMatrix);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &OldProjMatrix);

	}

}

CUI_MiniMap * CUI_MiniMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_MiniMap*		pInstance = new CUI_MiniMap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("UI_Minimap Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CUI_MiniMap::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRcTex = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTex, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTexture = static_cast<CTexture*>(Clone_Prototype(L"Proto_UI_MiniMap_Tex"));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_UI_MiniMap_Tex", pComponent });

	pComponent = m_pTransform = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_MiniMap", pComponent });


	return S_OK;
}

void CUI_MiniMap::Free(void)
{
	CGameObject::Free();
}
