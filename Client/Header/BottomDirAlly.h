#pragma once
#include "GameObject.h"
#include"TankSet.h"
#include "Collisionable.h"
#include"TankManager.h"
BEGIN(Engine)

class CTransform;
class CRcTex;
class CTexture;
class CCalculator;
class CLocation;
class CTank_Body;
class CTank_Head;
class CTankPosin;
class CVoxel;
END
class CBottomDirAlly :public CGameObject, public ICollisionable
{

private:
	explicit CBottomDirAlly(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBottomDirAlly(const CBottomDirAlly& rhs);
	virtual ~CBottomDirAlly();

public:
	virtual HRESULT Ready_Object(void) override;
	HRESULT Ready_Object(void* pArg);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

	virtual const   _vec3		Get_Info(void)			override;
	virtual			void		Move_Info(_vec3 _Info)	override;
	virtual			void		OBB_Collision_EX(void)	override;
	virtual			void		Update_OBB(void)		override;
	virtual			OBB*        Get_OBB(void)			override;
	void Set_DisCountLocation();
public:

	void	StateCheck();
	_bool Get_RightLocation() { return m_bRightLocationCount; }
	_bool Get_LeftLocation() { return m_bLeftLocationCount; }
	_bool Get_RightTopLocation() { return m_bRightTopLocationCount; }
	_bool Get_LeftTopLocation() { return m_bLeftTopLocationCount; }
	TANKTYPE Get_Type() { return TTYPE; }
	_int Get_LocationCheck() { return m_iLocationCheck; }
	void  Set_LocationCheck(_int _Check) { m_iLocationCheck = _Check; }
	void  Set_Action(_int _action) { m_iAction = _action; }
	void  Set_PastLocation(_int _Past) { m_PastLocation = _Past; }
	_float Dist(CTransform* _Target);
public:

	void Basic(_float fTimeDelta);
	void Occupation(_float fTimeDelta);
	void Wait(_float fTimeDelta);
	void Enermy_In_Area(_float fTimeDelta);
	void Detect(_float fTimeDelta);
	void		ObjectCol(_bool m_Left);
	_bool  Left_RightCheck(_vec3 _vDir, _vec3 _vLook);
	void    Set_Count(_int _Count) { m_iCount += _Count; }
	void   Run(_float fTimeDelta);
	void     	ColObject(_float fTimeDelta);
	void Dead_Motion(const _float & fTimeDelta);
	void						Minus_HP_UI(_float HP_minus) { fCurHp -= HP_minus; }
	void						Plus_HP_UI(_float HP_plus) { fCurHp += HP_plus; }
	_float GetHp(void) { return fCurHp; }
private:
	HRESULT		Add_Component(void);

private:
	CCalculator*		m_pCalculatorCom = nullptr;
	//��ũ����
	CVoxel*				m_pBody = nullptr;
	CVoxel*				m_pHead = nullptr;
	CVoxel*				m_pPosin = nullptr;
	//��ũ ������
	CTransform*			m_pTransformHead = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CTransform*			m_pTransformPosin = nullptr;
	CTransform*        m_PreTransformCom = nullptr;
	EData*       m_EData;
	_bool DeadMotionCheck = false;
	_int m_iMotionCount = 0;
	_float fAccum = 0.f;
	_bool   Deadtest = false;
	_float       preDist;
	_bool		m_bonce = false;
	_float     Range = 0.f;
	_float    PreHp = 0.f;
	_float re = 0.f;
	_bool bLeft = false;
	_bool m_bTest = false;
	BULLET_ID Temp;
	_float RuleTime = 0.f;
	TANKTYPE TTYPE = MAX;
private:
	_int  m_iLocationState;
	_int  m_iAction = AIACTION::AIACTION_END;
	_bool m_bRightLocationCount = false
		, m_bRightTopLocationCount = false
		, m_bLeftLocationCount = false
		, m_bLeftTopLocationCount = false;
	_int m_iLocationCheck = LOCATIONCHECK::LOCATIONCHECK_END;
	_int m_PastLocation = LOCATIONCHECK::LOCATIONCHECK_END;
	_vec3   m_vPatrol = {};
	_bool LeftCheck = false;

	_vec3 vPatrolRange = {};
	_bool m_bPatrol = false;
	_int  TempBullet = 0;
	_bool m_bOnce = false;
	_bool ColBuild = false;
	_int  ColBuildCount = 0;
	_int m_iCount = 0;
	//��ũ ����
	_float m_fMaxHp, fCurHp, fAccel_top_speed, RotSpped, fPosinDist;
	_float m_fReloadTime, m_fReload;
	_int   m_iCannonSpeed;
public:
	static CBottomDirAlly*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CBottomDirAlly*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg);
private:
	virtual void Free(void) override;


	// HP UI
	_matrix				UI_matViewF, m_UI_ProjMatrix;
	CRcTex*				m_pRcTexF = nullptr;
	CTexture*			m_pTextureF = nullptr;
	CTransform*		m_pTransformHP_UI = nullptr;

	_float					UI_fScaleX, UI_fScaleY, UI_fScaleZ;
	_float					UI_fOrgin_ScaleX;

	// Minimap UI
	_matrix				UI_Minimap_matProj;
	CRcTex*				m_pMinimap_RcTex = nullptr;
	CTexture*			m_pMinimap_Texure = nullptr;
	CTransform*		m_pMinimap_Transform = nullptr;
	_float					m_fMinimap[TRANSFORM_FLOAT_END];

	// Shot_Sound
	CGameObject* pPlayerTank = nullptr;
	_bool				m_bSoundOn = false;
	_float				m_fSoundSize = 0.f;

public:
	void						 Update_Shot_Sound(void);
	void						 Update_UI(void);
	void						 Update_Minimap(void);
};