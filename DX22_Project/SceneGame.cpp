#include "SceneGame.h"
#include "Geometory.h"
#include "DirectXMath.h"
#include "Model.h"
#include "ShaderList.h"
#include "CameraDebug.h"
#include "CPlayer.h"
#include "Block.h"

void DrawBoxTransform(float moveX, float moveY, float moveZ,
	float scaleX, float scaleY, float scaleZ,float rotX,float rotY,float rotZ)
{
	
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(moveX, moveY, moveZ);  // 天面がグリッドよりも下に来るように移動 
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scaleX, scaleY,scaleZ); // 地面となるように、前後左右に広く、上下に狭くする 

	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(rotX);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(rotY);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(rotZ);

	DirectX::XMMATRIX mat = S * Rx * Ry * Rz * T;
	//DirectX::XMMATRIX mat = S * T;//それぞれの行列を掛け合fわせて格納 
	mat = DirectX::XMMatrixTranspose(mat);//CPUからGPUに行列を送る前に実行する処理

	DirectX::XMFLOAT4X4 fMat; // 行列の格納先 
	DirectX::XMStoreFloat4x4(&fMat, mat);//MatをfMatに格納する処理; 

	Geometory::SetWorld(fMat); // ボックスに変換行列を設定 
	Geometory::DrawBox();
}

SceneGame::SceneGame()
{
	m_pModel = new Model();

	m_pBranchModel = new Model();

	m_pBushModel = new Model();
	 
	m_pBlock = new Block({ 10.0f,8.0f,8.0f });
	m_pBlock->SetPos({ 0.0f,0.5f,-8.0f });

	m_pGaugeUI = new GaugeUI();

	/*m_wallCollision.center = { 1.0f, 1.0f, -5.0f };
	m_wallCollision.size = { 20.0f, 0.5f, 5.0f };*/

	//const fileName="Assets/Model/まけん式赤見かるび姫衣装MMD/まけん式赤見かるび姫衣装_マントなしver1.0.pmx"
	 //モデルを読み込む（ファイルパスは自分のモデルに合わせて変更）
	/*if (!m_pModel->Load(
			"Assets/Model/yousei_low_fin.fbx", 0.25f)) {

			MessageBox(NULL, "モデルの読み込みに失敗しました。", "Error", MB_OK);
	}*/

	if (!m_pBranchModel->Load("Assets/Model/LowPolyNature/Tree_02.fbx", 0.0125f))
	{
		MessageBox(NULL, "Branch_01 の読み込みに失敗しました。", "Error", MB_OK);

	}

	
	if(!m_pBushModel->Load("Assets/Model/LowPolyNature/Rock_02.fbx", 0.05f))
	{
		MessageBox(NULL, "Bush_01 の読み込みに失敗しました。", "Error", MB_OK);

	}

	//camera
	m_pCamera = new CameraDebug();


	m_pCPlayer = new CPlayer();
	m_pCPlayer->SetCamera(m_pCamera);

	// --- 3D表示設定（Zバッファ有効化）---
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, pDSV);
	SetDepthTest(true);

	
	


}

SceneGame::~SceneGame()
{
	if (m_pBlock)
	{
		delete m_pBlock;
		m_pBlock = nullptr;
	}

	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}

	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera= nullptr;
	}

	if (m_pCPlayer) {
		delete m_pCPlayer;
		m_pCPlayer = nullptr;
	}
	if (m_pBranchModel) 
	{
		delete m_pBranchModel; m_pBranchModel = nullptr;
	}
	if (m_pBushModel)
	{
		delete m_pBushModel; m_pBushModel = nullptr;
	}
	if (m_pGaugeUI)
	{
		delete m_pGaugeUI; m_pGaugeUI = nullptr;
	}

}

void SceneGame::Update()
{
	m_pCamera->Update();
	m_pCPlayer->Update();
	m_pBlock->Update();

	// --- 当たり判定 ---


	Collision::Box a = m_pCPlayer->GetCollision();
	Collision::Box b = m_pBlock->GetCollision();

	Collision::Result result = Collision::Hit(a, b);

	if (result.isHit)
	{
		if (result.dir.x != 0.0f) m_pCPlayer->Bound(CPlayer::BoundX);
		if (result.dir.y != 0.0f) m_pCPlayer->Bound(CPlayer::BoundY);
		if (result.dir.z != 0.0f) m_pCPlayer->Bound(CPlayer::BoundZ);
	}
}

void SceneGame::Draw()
{
	// 頂点シェーダーに渡す変換行列を作成 
	DirectX::XMFLOAT4X4 fwvp[3];
	DirectX::XMMATRIX world, view, proj;

	// --- ワールド行列 ---
	world = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&fwvp[0], DirectX::XMMatrixTranspose(world));
	

	DirectX::XMFLOAT3 m_redPos = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_branchPos = { -4.0f, 1.25f, -6.0f };
	DirectX::XMFLOAT3 m_bushPos = { -3.0f, 1.5f, -6.0f };

	DirectX::XMMATRIX worldRed = DirectX::XMMatrixTranslation(m_redPos.x, m_redPos.y, m_redPos.z);
	DirectX::XMMATRIX worldBranch = DirectX::XMMatrixTranslation(m_branchPos.x, m_branchPos.y, m_branchPos.z);
	DirectX::XMMATRIX worldBush = DirectX::XMMatrixTranslation(m_bushPos.x, m_bushPos.y, m_bushPos.z);

	//カメラのビュー・プロジェクションを取得
	fwvp[1] = m_pCamera->GetViewMatrix();
	fwvp[2] = m_pCamera->GetProjectionMatrix();

	// --- 転置してシェーダーへ送る ---
	DirectX::XMStoreFloat4x4(&fwvp[0], DirectX::XMMatrixTranspose(world));

	fwvp[1] = m_pCamera->GetViewMatrix();
	fwvp[2] = m_pCamera->GetProjectionMatrix();

	// シェーダーへ変換行列を設定 
	ShaderList::SetWVP(fwvp);

	// シェーダー設定
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

	// 赤身カルビモデル
	{
		DirectX::XMMATRIX worldRed = DirectX::XMMatrixTranslation(m_redPos.x, m_redPos.y, m_redPos.z);
		DirectX::XMStoreFloat4x4(&fwvp[0], DirectX::XMMatrixTranspose(worldRed));

		ShaderList::SetWVP(fwvp);

		m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

		for (int i = 0; i < m_pModel->GetMeshNum(); ++i) {
			Model::Mesh mesh = *m_pModel->GetMesh(i);
			Model::Material material = *m_pModel->GetMaterial(mesh.materialID);
			ShaderList::SetMaterial(material);
			m_pModel->Draw(i);
		}
	}

	// 枝モデル
	{
		DirectX::XMMATRIX worldBranch = DirectX::XMMatrixTranslation(m_branchPos.x, m_branchPos.y, m_branchPos.z);
		DirectX::XMStoreFloat4x4(&fwvp[0], DirectX::XMMatrixTranspose(worldBranch));
		
		ShaderList::SetWVP(fwvp);

		m_pBranchModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pBranchModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

		for (int i = 0; i < m_pBranchModel->GetMeshNum(); ++i) {
			Model::Mesh mesh = *m_pBranchModel->GetMesh(i);
			Model::Material material = *m_pBranchModel->GetMaterial(mesh.materialID);
			ShaderList::SetMaterial(material);
			m_pBranchModel->Draw(i);
		}
	}

	//
	{
		DirectX::XMMATRIX worldBush = DirectX::XMMatrixTranslation(m_bushPos.x, m_bushPos.y,m_bushPos.z);
		DirectX::XMStoreFloat4x4(&fwvp[0], DirectX::XMMatrixTranspose(worldBush));

		ShaderList::SetWVP(fwvp);

		m_pBushModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pBushModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

		for (int i = 0; i < m_pBushModel->GetMeshNum(); ++i) {
			Model::Mesh mesh = *m_pBushModel->GetMesh(i);
			Model::Material material = *m_pBushModel->GetMaterial(mesh.materialID);
			ShaderList::SetMaterial(material);
			m_pBushModel->Draw(i);
		}
	}


	
	m_pCPlayer->Draw();

	// シェーダー設定
	m_pBranchModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pBranchModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));


	

	Geometory::SetView(m_pCamera->GetViewMatrix());
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix());

	//地面
	static float rad = 0.05f;

	//1つ目の地面
	DrawBoxTransform(1.0f, 1.0f, -5.0f, 20.0f, 0.5f, 5.0f, 0.0f, 0.0f, 0.0f);//X,Y,Z,SX,SY,SZ,rX,rY,rZ

    //DrawBoxTransform(1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f, rad, 0.0f);


	DrawBoxTransform(1.0f, 1.5f, -7.5f, 20.0f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f);
	DrawBoxTransform(1.0f, 1.5f, -2.5f, 20.0f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f);
   /* DrawBoxTransform(-1.0f, 1.0f, 5.0f, 5.0f, 0.5f, 5.0f, 0.0f, 0.0f, 0.0f);

	DrawBoxTransform(-2.0f, 1.0f, 8.0f, 5.0f, 0.5f, 4.5f, 0.0f, 0.0f, 0.0f);*/

	rad += 0.05f;

	m_pBlock->Draw();

	// ===== ここから UI =====
	SetDepthTest(false);     
	
	m_pGaugeUI->Draw();   

	SetDepthTest(true);

}

