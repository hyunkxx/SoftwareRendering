#include "stdafx.h"
#include "..\Header\Default_Ally.h"
#include "Export_Function.h"

//Location
#include"RightLocation.h"
#include"LeftTopLocation.h"
#include"LeftLocation.h"
#include"RightTopLocation.h"
#include"BottomDirEnermy.h"
#include"Default_Enermy.h"	
#include "TankCamera.h"
#include "StaticCamera.h"
#include "AimCamera.h"
CDefault_Ally::CDefault_Ally(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_bRightLocationCount(false)
{
}

CDefault_Ally::CDefault_Ally(const CDefault_Ally & rhs)
	: CGameObject(rhs)
{
}

CDefault_Ally::~CDefault_Ally()
{
	Free();
}

HRESULT CDefault_Ally::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);

	m_pTransformCom->Set_Pos(10.f, 1.f, 10.f);
	m_pTransformHead->Set_Pos(10.f, 1.f, 10.f);
	m_pTransformPosin->Set_Pos(10.f, 1.f, 10.f);

	//UI_HP
	UI_Orgin_HP = UI_fHP = 300.f;    // tankData.fMaxHP;
	UI_fOrgin_ScaleX = UI_fScaleX = 2.f;
	UI_fScaleY = 0.2f;
	UI_fScaleZ = 1.f;

	return S_OK;
}

HRESULT CDefault_Ally::Ready_Object(void * pArg)
{

	m_EData = (EData*)pArg;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformCom->Set_Pos(m_EData->vPos.x, 1.f, m_EData->vPos.z);
	m_pTransformHead->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformHead->Set_Pos(m_EData->vPos.x, 1.f, m_EData->vPos.z);
	m_pTransformPosin->Set_Scale(1.f, 1.f, 1.f);
	m_pTransformPosin->Set_Pos(m_EData->vPos.x, 1.f, m_EData->vPos.z);

	//UI_HP
	UI_Orgin_HP = UI_fHP = 300.f;     //tankData.fMaxHP;
	UI_fOrgin_ScaleX = UI_fScaleX = 2.f;
	UI_fScaleY = 0.2f;
	UI_fScaleZ = 1.f;

	return S_OK;
}

_int CDefault_Ally::Update_Object(const _float& fTimeDelta)
{

	__super::Update_Object(fTimeDelta);
	StateCheck();
	m_fReloadTime += fTimeDelta;
	Detect(fTimeDelta);
	Basic(fTimeDelta);

	_vec3 vTrans;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vTrans);
	m_pTransformHead->Set_Pos(vTrans.x, vTrans.y, vTrans.z);
	m_pTransformPosin->Set_Pos(vTrans.x, vTrans.y, vTrans.z);
	Add_RenderGroup(RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

void CDefault_Ally::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
	Update_UI();

}

void CDefault_Ally::Render_Object(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pBody->Render(m_pTransformCom->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHead->Get_WorldMatrix());
	m_pHead->Render(m_pTransformHead->Get_WorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformPosin->Get_WorldMatrix());
	m_pPosin->Render(m_pTransformPosin->Get_WorldMatrix());

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &UI_matViewF);
	// 수업 코드 적용 m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformHP_UI->Get_WorldMatrix());

	m_pTextureF->Set_Texture(0);
	m_pRcTexF->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

void CDefault_Ally::StateCheck()
{
	_vec3	vPos;
	srand((unsigned int)time(NULL));
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	//RightLocation

	if (vPos.x >VTXITV* VTXCNTX / 2.f && vPos.z < VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bRightLocationCount == false)
		{
			m_bRightLocationCount = true;
		}
		m_bRightTopLocationCount = false;
		m_bLeftLocationCount = false;
		m_bLeftTopLocationCount = false;

		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"RightLocation");
		CRightLocation* LTemp = dynamic_cast<CRightLocation*>(Dest);
		m_iLocationState = LTemp->Get_LocationState();
	}
	//LeftLocation
	else if (vPos.x < (VTXITV*VTXCNTX) / 2.f && vPos.z < (VTXITV*VTXCNTZ) / 2.f)
	{

		if (m_bLeftLocationCount == false)
		{
			m_vPatrol.x = (float)((rand() % 220) + 30.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (float)(rand() % 180 + 55.f);
			m_bLeftLocationCount = true;
		}
		m_bRightTopLocationCount = false;
		m_bRightLocationCount = false;
		m_bLeftTopLocationCount = false;
		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"LeftLocation");
		CLeftLocation* LTemp = dynamic_cast<CLeftLocation*>(Dest);
		m_iLocationState = LTemp->Get_LocationState();
	}
	//RightTopLocation
	else if (vPos.x >VTXITV*VTXCNTX / 2.f && vPos.z >VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bRightTopLocationCount == false)
		{
			m_vPatrol.x = (_float)(VTXITV* VTXCNTX - rand() % 40 - 45.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (_float)(VTXITV* VTXCNTX - rand() % 40 - 45.f);

			m_bRightTopLocationCount = true;
		}
		m_bRightLocationCount = false;
		m_bLeftLocationCount = false;
		m_bLeftTopLocationCount = false;


		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"RightTopLocation");
		CRightTopLocation* LTemp = dynamic_cast<CRightTopLocation*>(Dest);
		m_iLocationState = LTemp->Get_LocationState();
	}
	//LeftTopLocation
	else if (vPos.x < VTXITV*VTXCNTX / 2.f && vPos.z > VTXITV*VTXCNTZ / 2.f)
	{
		if (m_bLeftTopLocationCount == false)
		{
			m_vPatrol.x = (VTXITV*VTXCNTX / 2.f - rand() % 13 + 5.f);
			m_vPatrol.y = 0;
			m_vPatrol.z = (rand() % 20 + VTXITV*VTXCNTZ / 2.f + 20.f);

			m_bLeftTopLocationCount = true;
		}
		m_bRightTopLocationCount = false;
		m_bLeftLocationCount = false;
		m_bRightLocationCount = false;

		CManagement* Temp = CManagement::GetInstance();
		CLayer*      Sour = Temp->Find_Layer(L"Environment");
		CGameObject* Dest = Sour->Get_GameObject(L"LeftTopLocation");
		CLeftTopLocation* LTemp = dynamic_cast<CLeftTopLocation*>(Dest);
		m_iLocationState = LTemp->Get_LocationState();
	}
}

void CDefault_Ally::Detect(_float fTimeDelta)
{

	vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);
	if (Temps.size() != 0)
	{
		for (auto& iter = Temps.begin(); iter < Temps.end(); ++iter)
		{
			CTransform*	pEnermyTransform = static_cast<CTransform*> (static_cast<CDefault_Enermy* > (*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));
			_float fEnemyCol = Dist(pEnermyTransform);

			if (fEnemyCol <= 60.f)
			{
				m_iAction = AIACTION::AIACTION_BATTLE;
				_vec3 Pos, Dir, vLook, vEHPos, TargetPos;
				m_pTransformHead->Get_Info(INFO::INFO_POS, &vEHPos);
				pEnermyTransform->Get_Info(INFO::INFO_POS, &TargetPos);
				Dir = TargetPos - vEHPos;

				m_pTransformHead->Get_Info(INFO::INFO_LOOK, &vLook);

				D3DXVec3Normalize(&vLook, &vLook);
				D3DXVec3Normalize(&Dir, &Dir);

				Left_RightCheck(Dir, vLook);
				_float Dot = D3DXVec3Dot(&vLook, &Dir);
				_float Angle = (float)acosf(Dot);
				if (isnan(Angle))
				{
					Angle = 0;
				}
				if (LeftCheck == false)
				{
					m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
					m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta);
				}
				else
				{
					m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
					m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta);
				}

				m_pTransformHead->Get_Info(INFO_POS, &Pos);
				m_pTransformHead->Get_Info(INFO_LOOK, &vLook);
				Dir = TargetPos - Pos;
				D3DXVec3Normalize(&Dir, &Dir);
				D3DXVec3Normalize(&vLook, &vLook);
				Pos += Dir* 3.f;
				if (fabs(D3DXToDegree(Angle)) < 4.f)
				{
					if (m_fReloadTime > m_fReload)
					{
						Engine::Reuse_Object(Pos, Dir, 500.f, m_pTransformPosin->Get_Angle(ROT_X), m_pTransformPosin->Get_Angle(ROT_Y), BULLET_ID::CANNONBALL);
						m_fReloadTime = 0.f;
					}
				}

			}
		}

	}

}

_float CDefault_Ally::Dist(CTransform * _Target)
{
	_vec3 vTargetPos, vPos, vCol, _vAPos;
	_Target->Get_Info(INFO::INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	vCol = vPos - vTargetPos;
	_float Dist = D3DXVec3Length(&vCol);
	return Dist;
}

void CDefault_Ally::Basic(_float fTimeDelta)
{
	if (m_iAction != AIACTION::AIACTION_BATTLE)
	{
		switch (m_iAction)
		{
		case AIACTION::AIACTION_WAIT:
			Wait(fTimeDelta);
			break;
		case AIACTION::AIACTION_OCCOPATION:
			Occupation(fTimeDelta);
			break;
		case AIACTION::AIACTION_ENERMY_IN_AREA:
			Enermy_In_Area(fTimeDelta);
			break;

		}
	}
}

_bool CDefault_Ally::Left_RightCheck(_vec3 _vDir, _vec3 _vLook)
{
	_vec3 Up = { 0.f,1.f,0.f }, Temp;
	D3DXVec3Normalize(&Up, &Up);
	D3DXVec3Cross(&Temp, &_vDir, &_vLook);
	_float TempTest = D3DXVec3Dot(&Up, &Temp);

	if (TempTest > 0)
		LeftCheck = false;
	else
		LeftCheck = true;
	return LeftCheck;
}

void CDefault_Ally::Occupation(_float fTimeDelta)
{
	if (m_iAction == AIACTION::AIACTION_OCCOPATION)
	{

		_vec3  vPos, vLook;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		if (m_vPatrol.x + 3 <= vPos.x&&m_vPatrol.z + 3 <= vPos.z)
		{
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
			_vec3		vDir = m_vPatrol - vPos;

			D3DXVec3Normalize(&vLook, &vLook);
			D3DXVec3Normalize(&vDir, &vDir);

			Left_RightCheck(vDir, vLook);

			_float Dot = D3DXVec3Dot(&vLook, &vDir);
			_float AngleTemp = (float)acosf(Dot);
			if (isnan(AngleTemp))
			{
				AngleTemp = 0;
			}
			if (LeftCheck == false)
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, -AngleTemp*fTimeDelta);
				_vec3 vTrans2;
				m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
				m_pTransformCom->Move_Pos(&(vTrans2*2.f*fTimeDelta));
			}
			else
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, AngleTemp*fTimeDelta);
				_vec3 vTrans2;
				m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
				m_pTransformCom->Move_Pos(&(vTrans2*10.f*fTimeDelta));
			}
		}
	}
}

void CDefault_Ally::Wait(_float fTimeDelta)
{
	if (m_iAction == AIACTION::AIACTION_WAIT)
	{
		_vec3   vTemp, vSour, vDist, vDir, vLook, vTrans2;
		_float Angle = 0.f, Dot = 0.f;
		switch (m_PastLocation)
		{
		case LOCATIONCHECK::LOCATIONCHECK_LEFTTOP:
			vTemp = { VTXITV*VTXCNTX,0.f,m_vPatrol.z };
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vSour);
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
			vDir = vTemp - vSour;

			D3DXVec3Normalize(&vDir, &vDir);
			D3DXVec3Normalize(&vLook, &vLook);

			Left_RightCheck(vDir, vLook);
			Dot = D3DXVec3Dot(&vDir, &vLook);
			Angle = (float)acosf(Dot);
			if (isnan(Angle))
			{
				Angle = 0;
			}
			if (LeftCheck == false)
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
			}
			else
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
			}
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));
			break;

		case LOCATIONCHECK::LOCATIONCHECK_LEFT:
			vTemp = { m_vPatrol.x,0.f,VTXITV*VTXCNTZ };
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vSour);
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
			vDir = vTemp - vSour;

			D3DXVec3Normalize(&vDir, &vDir);
			D3DXVec3Normalize(&vLook, &vLook);

			Left_RightCheck(vDir, vLook);
			Dot = D3DXVec3Dot(&vDir, &vLook);
			Angle = (float)acosf(Dot);

			if (isnan(Angle))
			{
				Angle = 0;
			}
			if (LeftCheck == false)
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
			}
			else
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
			}
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
			m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));
			break;
		}
	}
}

void CDefault_Ally::Enermy_In_Area(_float fTimeDelta)
{

	if (m_iAction == AIACTION::AIACTION_ENERMY_IN_AREA)
	{

		vector<CGameObject*>Temps = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_BDENERMY);
		vector<CGameObject*>Sours = CEnermyMgr::GetInstance()->Get_mIEnermy(OBJID::OBJID_DEFAULT_ENERMY);

		for (auto& iter = Sours.begin(); iter < Sours.end(); ++iter)
		{
			CTransform*	pDefaultEnermyTransform = static_cast<CTransform*> (static_cast<CDefault_Enermy*>(*iter)->Get_Component(L"Proto_Transform", ID_DYNAMIC));

			_vec3 vTargetPos, vLook, vPos;
			pDefaultEnermyTransform->Get_Info(INFO::INFO_POS, &vTargetPos);
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
			m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
			_vec3 vDir = vTargetPos - vPos;
			D3DXVec3Normalize(&vDir, &vDir);
			D3DXVec3Normalize(&vLook, &vLook);
			Left_RightCheck(vDir, vLook);
			_float Dot = D3DXVec3Dot(&vDir, &vLook);
			_float Angle = acosf(Dot);
			if (isnan(Angle))
			{
				Angle = 0;
			}
			if (LeftCheck == false)
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, -Angle*fTimeDelta * 3);
			}
			else
			{
				m_pTransformCom->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
				m_pTransformHead->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
				m_pTransformPosin->Rotation(ROTATION::ROT_Y, Angle*fTimeDelta * 3);
			}

		}
		_vec3 vTrans2;
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vTrans2);
		m_pTransformCom->Move_Pos(&(vTrans2*fTimeDelta*10.f));

	}
}

CDefault_Ally* CDefault_Ally::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDefault_Ally*		pInstance = new CDefault_Ally(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CDefault_Ally Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CDefault_Ally * CDefault_Ally::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CDefault_Ally*		pInstance = new CDefault_Ally(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		MSG_BOX("CDefault_Ally Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CDefault_Ally::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(m_pCalculatorCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	//탱크 몸통
	pComponent = m_pBody = CVoxel::Create(m_pGraphicDev, L"Big_ally_body");
	NULL_CHECK_RETURN(m_pBody, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelBody", pComponent });

	pComponent = m_pHead = CVoxel::Create(m_pGraphicDev, L"Big_ally_head");
	NULL_CHECK_RETURN(m_pHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_EVoxelHead", pComponent });

	pComponent = m_pPosin = CVoxel::Create(m_pGraphicDev, L"Big_ally_posin");
	NULL_CHECK_RETURN(m_pPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_VoxelPosin", pComponent });

	//탱크 움직임

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTransformHead = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHead, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_HeadTransform", pComponent });

	pComponent = m_pTransformPosin = dynamic_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformPosin, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PosinTransform", pComponent });


	pComponent = m_pRcTexF = static_cast<CRcTex*>(Clone_Prototype(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pRcTexF, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureF = static_cast<CTexture*>(Clone_Prototype(L"Proto_World_Hp_Tex"));
	NULL_CHECK_RETURN(m_pTextureF, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_World_Hp_Tex", pComponent });

	pComponent = m_pTransformHP_UI = static_cast<CTransform*>(Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformHP_UI, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform_WHP2", pComponent });

	return S_OK;
}

void CDefault_Ally::Free(void)
{
	__super::Free();
}

void CDefault_Ally::Update_UI(void)
{
	if (UI_fHP >= UI_Orgin_HP)
	{
		UI_fHP = UI_Orgin_HP;
	}
	if (UI_fHP <= 0.f)
	{
		UI_fHP = 0.f;
	}

	_float HP_Percent = (UI_fHP / UI_Orgin_HP);

	if (HP_Percent > 1.f)
	{
		HP_Percent = 1.f;
	}
	if (HP_Percent < 0.f)
	{
		HP_Percent = 0.f;
	}

	UI_fScaleX = (UI_fOrgin_ScaleX * HP_Percent);

	_vec3 vTankPos, vUI_HPF;
	// UI_ 높이 _ 키워드
	if (Engine::Get_Camera_ID() == CAMERA_ID::TANK_CAMERA)
	{
		UI_fScaleX = 2.f;
		UI_fScaleY = 0.2f;

		m_pTransformHP_UI->Set_Scale(UI_fScaleX, UI_fScaleY, UI_fScaleZ);

		m_pTransformCom->Get_Info(INFO_POS, &vTankPos);

		vUI_HPF = { vTankPos.x, vTankPos.y + 2.5f, vTankPos.z };

		Engine::Get_Camera()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &UI_matViewF);
	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::DRONE_CAMERA)
	{
		UI_fScaleX = 3.f;
		UI_fScaleY = 0.4f;

		m_pTransformHP_UI->Set_Scale(UI_fScaleX, UI_fScaleY + 1.5f, UI_fScaleZ);

		m_pTransformCom->Get_Info(INFO_POS, &vTankPos);

		vUI_HPF = { vTankPos.x, vTankPos.y + 3.5f, vTankPos.z + 3.f };

		Engine::Get_Camera()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &UI_matViewF);
	}
	else if (Engine::Get_Camera_ID() == CAMERA_ID::AIM_CAMERA)
	{
		UI_fScaleX = 2.f;
		UI_fScaleY = 0.2f;

		m_pTransformHP_UI->Set_Scale(UI_fScaleX, UI_fScaleY, UI_fScaleZ);

		m_pTransformCom->Get_Info(INFO_POS, &vTankPos);

		vUI_HPF = { vTankPos.x, vTankPos.y + 2.5f, vTankPos.z };

		Engine::Get_Camera()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &UI_matViewF);
	}

	memset(&UI_matViewF._41, 0, sizeof(_vec3));

	D3DXMatrixInverse(&UI_matViewF, 0, &UI_matViewF);

	_vec3 BillPos = vUI_HPF;

	_float fScale[ROT_END];

	fScale[ROT_X] = UI_fScaleX;
	fScale[ROT_Y] = UI_fScaleY;
	fScale[ROT_Z] = UI_fScaleZ;

	memcpy(&UI_matViewF._41, &BillPos, sizeof(_vec3));

	for (int i = 0; i < ROT_END; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			UI_matViewF(i, j) *= fScale[i];
		}
	}

	// 정말 혹시나 만약에 정말 만약을 위한 [[수업코드]]		( bool 변수로 키눌러서 축 바꾸기 가능)
#pragma region 
	/*
	UI_fScaleX = (UI_fOrgin_ScaleX * HP_Percent);
	m_pTransformHP_UI->Set_Scale(UI_fScaleX, UI_fScaleY, UI_fScaleZ);

	_vec3 UI_TankPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &UI_TankPos);
	m_pTransformHP_UI->Set_Pos(UI_TankPos.x, UI_TankPos.y + 5.f, UI_TankPos.z);

	if (Engine::Get_Camera_ID==CAMERA_ID::TANK_CAMERA)
	{
	_matrix		matWorld, matView, matBill;
	D3DXMatrixIdentity(&matWorld);
	m_pTransformHP_UI->Get_WorldMatrix(&matWorld);

	//pTankView->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	if (bXYZ[0])					// X
	{
	matBill._22 = matView._22;
	matBill._23 = matView._23;
	matBill._32 = matView._32;
	matBill._33 = matView._33;
	}
	if (bXYZ[1])						// Y
	{
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	}
	if (bXYZ[2])							// Z
	{
	matBill._11 = matView._11;
	matBill._12 = matView._12;
	matBill._21 = matView._21;
	matBill._22 = matView._22;
	}
	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pTransformHP_UI->Set_WorldMatrix(&(matBill * matWorld));
	}

	else if (Engine::Get_Camera_ID==CAMERA_ID::TOPVIEW_CAMERA)
	{
	_matrix		matWorld, matView, matBill;
	m_pTransformHP_UI->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);
	if (bXYZ[0])					// X
	{
	matBill._22 = matView._22;
	matBill._23 = matView._23;
	matBill._32 = matView._32;
	matBill._33 = matView._33;
	}
	if (bXYZ[1])						// Y
	{
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	}
	if (bXYZ[2])							// Z
	{
	matBill._11 = matView._11;
	matBill._12 = matView._12;
	matBill._21 = matView._21;
	matBill._22 = matView._22;
	}
	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pTransformHP_UI->Set_WorldMatrix(&(matBill * matWorld));

	}

	else if (static_cast<CAimCamera*>(pAimView)->Get_CameraOn())
	{
	_matrix		matWorld, matView, matBill;
	m_pTransformHP_UI->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);
	if (bXYZ[0])					// X
	{
	matBill._22 = matView._22;
	matBill._23 = matView._23;
	matBill._32 = matView._32;
	matBill._33 = matView._33;
	}
	if (bXYZ[1])						// Y
	{
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	}
	if (bXYZ[2])							// Z
	{
	matBill._11 = matView._11;
	matBill._12 = matView._12;
	matBill._21 = matView._21;
	matBill._22 = matView._22;
	}

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pTransformHP_UI->Set_WorldMatrix(&(matBill * matWorld));



	}

	*/
#pragma endregion

}
