#pragma once
#include "GameObject.h"
#include "Voxel.h"
#include "Collisionable.h"
#include "UI_Volume.h"
BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CTankSet :
	public CGameObject, public ICollisionable
{
protected:
	explicit CTankSet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTankSet(const CTankSet& rhs);
	virtual ~CTankSet();

public:
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;


	virtual const _vec3	Get_Info(void)override;
	virtual	void		Move_Info(_vec3 _Info)override;
	virtual			void		RenderGUI(void)override {}
	virtual			void		OBB_Collision_EX(void);
	virtual			void		Update_OBB(void);
	virtual			OBB*        Get_OBB(void) { return &m_stBody; }

protected:
	virtual	HRESULT		Add_Component(void);
	virtual HRESULT		Ready_Object(void)PURE;
	virtual void		Sound_Setting(const _float & fTimeDelta)PURE;
	virtual void        Shoot_Bullet(BULLET_ID eID);
	void				Posin_Setting(const _vec3& _SetPos);
	void				Head_Setting(const _vec3& _SetPos);
	void				Body_Setting(const _vec3& _SetPos);

	virtual void		Dead_Motion(const _float& fTimeDelta);
private:
	void				Reset_Speed(void) { m_stInfo.fAccum = 0.f; }

public:
	void				Rotation_Body(ROTATION eID, _float fAngle);
	void				Rotation_Head(ROTATION eID, _float fAngle);
	void				Rotation_Posin(ROTATION eID, _float fAngle);

	_float				Plus_Advance_AccelSpeed(const _float& fTimeDelta);
	_float				Minus_Advance_AccelSpeed(const _float& fTimeDelta);

	_float				Plus_Back_AccelSpeed(const _float& fTimeDelta);
	_float				Minus_Back_AccelSpeed(const _float& fTimeDelta);

	const _vec3				Get_HitPos(void) { return m_fHitPos; }
	void				Set_Rock(_bool Rock) { m_bRock = Rock; }

	const Tank_State Get_TankInfo(void) { return  m_stInfo; }
	_bool				Get_Smoke(void) { return m_bSmoke; }
protected:
	CTransform*			m_pTransformBody = nullptr;
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformPosin = nullptr;

	Tank_State			m_stInfo;

	_bool				m_bStart = false;
	_bool				m_bRock = false;
	_bool				m_bMove = false;
	_float				m_fMoveCount = 0.f;
	_float				m_fEngineCount = 0;
	_int				m_iSound_Count = 0;
	//Voxel �߰�
	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;

	_float				m_fDT = 0.f;

	_vec3				m_fHitPos;
	//
	_int				m_iMotionCount = 0;
protected:
	virtual void Free(void) override;

	// HP UI
	_matrix				UI_matViewF;
	CRcTex*				m_pRcTexF = nullptr;
	CTexture*			m_pTextureF = nullptr;
	CTransform*		m_pTransformHP_UI = nullptr;

	_float					UI_fScaleX, UI_fScaleY, UI_fScaleZ;
	_float					UI_fOrgin_ScaleX;

	// Minimap UI
	_matrix            UI_Minimap_matProj;
	CRcTex*            m_pMinimap_RcTex = nullptr;
	CTexture*         m_pMinimap_Texure = nullptr;
	CTransform*      m_pMinimap_Transform = nullptr;
	_float               m_fMinimap[TRANSFORM_FLOAT_END];

	// Minimap Rader UI
	CRcTex*            m_pRader_RcTex = nullptr;
	CTexture*         m_pRader_Texure = nullptr;
	CTransform*      m_pRader_Transform = nullptr;
	_float               m_fRader;

	_bool			m_bSmoke = false; 
	_float			m_fSmokeTime = 10.f;
	_float			m_fSmokeCount = 0.f;
public:
	_float Get_HP(void) { return m_stInfo.fCurHP; }
	void Minus_HP(_float minus) { m_stInfo.fCurHP -= minus; }
	void Plus_HP(_float plus) { m_stInfo.fCurHP += plus; }

};