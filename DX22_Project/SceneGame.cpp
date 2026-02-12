#include "SceneGame.h"
#include "Geometory.h"
#include "DirectXMath.h"
#include "Model.h"
#include "ShaderList.h"
#include "Sprite.h"
#include "Defines.h"
#include "Collision.h"
#include "Goal.h"


static void DrawBoxTransform(
    float moveX, float moveY, float moveZ,
    float scaleX, float scaleY, float scaleZ,
    float rotX, float rotY, float rotZ)
{
    using namespace DirectX;

    XMMATRIX T = XMMatrixTranslation(moveX, moveY, moveZ);
    XMMATRIX S = XMMatrixScaling(scaleX, scaleY, scaleZ);
    XMMATRIX Rx = XMMatrixRotationX(rotX);
    XMMATRIX Ry = XMMatrixRotationY(rotY);
    XMMATRIX Rz = XMMatrixRotationZ(rotZ);

    XMMATRIX mat = S * Rx * Ry * Rz * T;
    mat = XMMatrixTranspose(mat);

    XMFLOAT4X4 fMat;
    XMStoreFloat4x4(&fMat, mat);

    Geometory::SetWorld(fMat);
    Geometory::DrawBox();
}

SceneGame::SceneGame()
{
    m_pModel = new Model();
    m_pBranchModel = new Model();
    m_pBushModel = new Model();


    m_pCamera = new CameraDebug();
    m_pEventCamera = new CameraEvent();
    m_isEvent = true;


    m_pCPlayer = new CPlayer();
    m_pCPlayer->SetCamera(m_pCamera);
  
    m_pCamera->SetTarget(m_pCPlayer);

    m_pBlock = new Block({ 10.0f,8.0f,8.0f });
    m_pBlock->SetPos({ 0.0f,0.5f,-8.0f });

    m_pGaugeUI = new GaugeUI();

    m_pGoal = new Goal({ 64.0f,64.0f,0.0f }); 
    
    m_pEventCamera->SetEvent(
        { 0.0f, 10.0f, -30.0f },  // 開始
        { 0.0f, 5.0f, -10.0f },   // 終了
        3.0f                      // 3秒
    );
    if (!m_pBranchModel->Load("Assets/Model/LowPolyNature/Tree_02.fbx", 0.0125f))
        MessageBox(NULL, "Branch_01 の読み込みに失敗しました。", "Error", MB_OK);

    if (!m_pBushModel->Load("Assets/Model/LowPolyNature/Rock_02.fbx", 0.05f))
        MessageBox(NULL, "Bush_01 の読み込みに失敗しました。", "Error", MB_OK);

    m_pGoal->SetCamera(m_pCamera);

    m_pGoal->SetPos({ 2.0f, 2.0f, 0.0f });
}

SceneGame::~SceneGame()
{
    delete  m_pGoal;       m_pGoal = nullptr;
    delete m_pGaugeUI;     m_pGaugeUI = nullptr;
    delete m_pBlock;       m_pBlock = nullptr;
    delete m_pCPlayer;     m_pCPlayer = nullptr;
    delete m_pCamera;      m_pCamera = nullptr;
    delete m_pBushModel;   m_pBushModel = nullptr;
    delete m_pBranchModel; m_pBranchModel = nullptr;
    delete m_pModel;       m_pModel = nullptr;
    delete m_pEventCamera; m_pEventCamera = nullptr;
}
void SceneGame::Update()
{
    Camera* currentCamera = nullptr;

    if (m_isEvent)
    {
        m_pEventCamera->Update();
        currentCamera = m_pEventCamera;


        if (!m_pEventCamera->IsEvent())
        {
            m_isEvent = false;

        }
    }
    else
    {
        m_pCamera->UpdateInput();
        m_pCamera->UpdateView();
        currentCamera = m_pCamera;
    }

    m_pCPlayer->SetCamera(currentCamera);
    m_pGoal->SetCamera(currentCamera);

    m_pCPlayer->Update();
    m_pBlock->Update();

    Collision::Box a = m_pCPlayer->GetCollision();
    Collision::Box b = m_pBlock->GetCollision();
    Collision::Result r = Collision::Hit(a, b);

    if (r.isHit)
    {
        if (r.dir.x != 0.0f)      m_pCPlayer->Bound(CPlayer::BoundX);
        else if (r.dir.y != 0.0f) m_pCPlayer->Bound(CPlayer::BoundY);
        else if (r.dir.z != 0.0f) m_pCPlayer->Bound(CPlayer::BoundZ);
    }

    DirectX::XMFLOAT3 shadowPos = m_pCPlayer->GetPos();
    Collision::Box s = m_pCPlayer->GetShadowCollision();
    Collision::Result result = Collision::Hit(b, s);

    if (result.isHit)
        shadowPos.y = b.center.y + b.size.y * 0.5f;
    else
        shadowPos.y = 0.0f;

    m_pCPlayer->SetShadowPos(shadowPos);
}

void SceneGame::Draw()
{
    using namespace DirectX;

    // ===== 使用するカメラを決定 =====
    Camera* currentCamera = nullptr;

    if (m_isEvent)
        currentCamera = m_pEventCamera;
    else
        currentCamera = m_pCamera;

    RenderTarget* rtv = GetDefaultRTV();
    DepthStencil* dsv = GetDefaultDSV();
    SetRenderTargets(1, &rtv, dsv);
    SetDepthTest(true);

    D3D11_VIEWPORT vp = {};
    vp.Width = (float)SCREEN_WIDTH;
    vp.Height = (float)SCREEN_HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    GetContext()->RSSetViewports(1, &vp);

    XMFLOAT4X4 fwvp[3];

    XMStoreFloat4x4(&fwvp[0], XMMatrixTranspose(XMMatrixIdentity()));

    fwvp[1] = currentCamera->GetViewMatrix();
    fwvp[2] = currentCamera->GetProjectionMatrix();

    ShaderList::SetWVP(fwvp);

    Geometory::SetView(fwvp[1]);
    Geometory::SetProjection(fwvp[2]);

    // ========= 3D描画 =========
    m_pCPlayer->Draw();
    m_pBlock->Draw();

    // ========= UI =========
    SetDepthTest(false);

    XMFLOAT4X4 uiWVP[3];
    XMStoreFloat4x4(&uiWVP[0], XMMatrixTranspose(XMMatrixIdentity()));

    uiWVP[1] =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    XMMATRIX ortho =
        XMMatrixOrthographicOffCenterLH(
            0.0f,
            (float)SCREEN_WIDTH,
            (float)SCREEN_HEIGHT,
            0.0f,
            0.0f,
            1.0f
        );

    XMStoreFloat4x4(&uiWVP[2], XMMatrixTranspose(ortho));
    ShaderList::SetWVP(uiWVP);

    XMFLOAT4X4 view, proj;
    XMStoreFloat4x4(&view, XMMatrixTranspose(XMMatrixIdentity()));
    XMStoreFloat4x4(&proj, XMMatrixTranspose(ortho));

    Sprite::SetView(view);
    Sprite::SetProjection(proj);

    m_pGaugeUI->Draw();

    // ========= 3D Sprite =========
    SetDepthTest(true);

    Sprite::SetView(currentCamera->GetViewMatrix());
    Sprite::SetProjection(currentCamera->GetProjectionMatrix());

    m_pGoal->Draw();
}

