#include "SceneGame.h"
#include "Geometory.h"
#include "DirectXMath.h"
#include "Model.h"
#include "ShaderList.h"
#include "Sprite.h"
#include "Defines.h"
#include "Collision.h"

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

    m_pCPlayer = new CPlayer();
    m_pCPlayer->SetCamera(m_pCamera);

    m_pCamera->SetTarget(m_pCPlayer);

    m_pBlock = new Block({ 10.0f,8.0f,8.0f });
    m_pBlock->SetPos({ 0.0f,0.5f,-8.0f });

    m_pGaugeUI = new GaugeUI();

    if (!m_pBranchModel->Load("Assets/Model/LowPolyNature/Tree_02.fbx", 0.0125f))
        MessageBox(NULL, "Branch_01 の読み込みに失敗しました。", "Error", MB_OK);

    if (!m_pBushModel->Load("Assets/Model/LowPolyNature/Rock_02.fbx", 0.05f))
        MessageBox(NULL, "Bush_01 の読み込みに失敗しました。", "Error", MB_OK);
}

SceneGame::~SceneGame()
{
    delete m_pGaugeUI;     m_pGaugeUI = nullptr;
    delete m_pBlock;       m_pBlock = nullptr;
    delete m_pCPlayer;     m_pCPlayer = nullptr;
    delete m_pCamera;      m_pCamera = nullptr;
    delete m_pBushModel;   m_pBushModel = nullptr;
    delete m_pBranchModel; m_pBranchModel = nullptr;
    delete m_pModel;       m_pModel = nullptr;
}

void SceneGame::Update()
{
    m_pCamera->UpdateInput();

    m_pCamera->UpdateView();

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

    m_pCamera->UpdateView();
}

void SceneGame::Draw()
{
    using namespace DirectX;

    RenderTarget* rtv = GetDefaultRTV();
    DepthStencil* dsv = GetDefaultDSV();
    SetRenderTargets(1, &rtv, dsv);
    SetDepthTest(true);

    GetContext()->RSSetState(nullptr);
    GetContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

    D3D11_VIEWPORT vp = {};
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = (float)SCREEN_WIDTH;
    vp.Height = (float)SCREEN_HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    GetContext()->RSSetViewports(1, &vp);

    XMFLOAT4X4 fwvp[3];
    XMMATRIX world = XMMatrixIdentity();

    XMStoreFloat4x4(&fwvp[0], XMMatrixTranspose(world));
    fwvp[1] = m_pCamera->GetViewMatrix();
    fwvp[2] = m_pCamera->GetProjectionMatrix();

    ShaderList::SetWVP(fwvp);

    Geometory::SetView(fwvp[1]);
    Geometory::SetProjection(fwvp[2]);

    XMFLOAT3 redPos = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 branchPos = { -4.0f, 1.25f, -6.0f };
    XMFLOAT3 bushPos = { -3.0f, 1.5f, -6.0f };

    {
        XMMATRIX w = XMMatrixTranslation(redPos.x, redPos.y, redPos.z);
        XMStoreFloat4x4(&fwvp[0], XMMatrixTranspose(w));
        ShaderList::SetWVP(fwvp);

        m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
        m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

        for (int i = 0; i < m_pModel->GetMeshNum(); ++i)
        {
            auto mesh = *m_pModel->GetMesh(i);
            auto material = *m_pModel->GetMaterial(mesh.materialID);
            ShaderList::SetMaterial(material);
            m_pModel->Draw(i);
        }
    }

    {
        XMMATRIX w = XMMatrixTranslation(branchPos.x, branchPos.y, branchPos.z);
        XMStoreFloat4x4(&fwvp[0], XMMatrixTranspose(w));
        ShaderList::SetWVP(fwvp);

        m_pBranchModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
        m_pBranchModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

        for (int i = 0; i < m_pBranchModel->GetMeshNum(); ++i)
        {
            auto mesh = *m_pBranchModel->GetMesh(i);
            auto material = *m_pBranchModel->GetMaterial(mesh.materialID);
            ShaderList::SetMaterial(material);
            m_pBranchModel->Draw(i);
        }
    }

    {
        XMMATRIX w = XMMatrixTranslation(bushPos.x, bushPos.y, bushPos.z);
        XMStoreFloat4x4(&fwvp[0], XMMatrixTranspose(w));
        ShaderList::SetWVP(fwvp);

        m_pBushModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
        m_pBushModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

        for (int i = 0; i < m_pBushModel->GetMeshNum(); ++i)
        {
            auto mesh = *m_pBushModel->GetMesh(i);
            auto material = *m_pBushModel->GetMaterial(mesh.materialID);
            ShaderList::SetMaterial(material);
            m_pBushModel->Draw(i);
        }
    }

    m_pCPlayer->Draw();

    DrawBoxTransform(1, 1, -5, 20, 0.5f, 5, 0, 0, 0);
    DrawBoxTransform(1, 1.5f, -7.5f, 20, -0.5f, -0.5f, 0, 0, 0);
    DrawBoxTransform(1, 1.5f, -2.5f, 20, -0.5f, -0.5f, 0, 0, 0);

    m_pBlock->Draw();

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

    static ID3D11RasterizerState* rsNoCull = nullptr;
    if (!rsNoCull)
    {
        D3D11_RASTERIZER_DESC rd = {};
        rd.FillMode = D3D11_FILL_SOLID;
        rd.CullMode = D3D11_CULL_NONE;
        rd.DepthClipEnable = TRUE;
        HRESULT hr = GetDevice()->CreateRasterizerState(&rd, &rsNoCull);
        if (FAILED(hr))
            MessageBox(nullptr, "CreateRasterizerState failed", "Error", MB_OK);
    }
    GetContext()->RSSetState(rsNoCull);
    GetContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

    m_pGaugeUI->Draw();

    GetContext()->RSSetState(nullptr);
    SetDepthTest(true);
}
