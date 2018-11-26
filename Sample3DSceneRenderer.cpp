#include "pch.h"
#include "Sample3DSceneRenderer.h"

#include "..\Common\DirectXHelper.h"

using namespace ecoin;

using namespace DirectX;
using namespace Windows::Foundation;

ULONGLONG g_TimeStamp;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

#if 0
	classTarget* f_Target = new classTarget();
	f_Target->acGen_Humanoid();
	f_Target->acSaveCloud("Human Factor1");

	classTarget* f_Target2 = new classTarget();
	f_Target2->acGen_Vehicle_Ground();
	f_Target2->acSaveCloud("Vehicle Ground1");
#endif

	g_Target = new classTarget();
	g_Target->acLoad_from_TargetID(1);
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
		);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	Cube::g_projection = perspectiveMatrix * orientationMatrix;

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { -9.75f, 1.75f, 7.75f, 0.0f };
	static const XMVECTORF32 at = { 0.75f, -0.2f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));

	XMMATRIX f_MatrixVW = XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up));

	Cube::g_view = f_MatrixVW;

#ifdef ARM_ARCH
	Cube::g_eye = Cube::BiVector(eye.v.n128_f32[0], eye.v.n128_f32[1], eye.v.n128_f32[2], eye.v.n128_f32[3]);
	Cube::g_at = Cube::BiVector(at.v.n128_f32[0], at.v.n128_f32[1], at.v.n128_f32[2], at.v.n128_f32[3]);
#else
	Cube::g_eye = Cube::BiVector(eye.v.m128_f32[0], eye.v.m128_f32[1], eye.v.m128_f32[2], eye.v.m128_f32[3]);
	Cube::g_at = Cube::BiVector(at.v.m128_f32[0], at.v.m128_f32[1], at.v.m128_f32[2], at.v.m128_f32[3]);
#endif
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		Rotate(radians);
	}
}

// Rotate the 3D cube model a set amount of radians.
void Sample3DSceneRenderer::Rotate(float radians)
{
#if 0
	if(g_VisualMiningMode == OK_MODE_VIEW)
		{
		Cube::CubeHESH* f_Hesh = Cube::g_HESH[g_SelectedHeshID];

		g_RADX = f_Hesh->m_RotateX;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADX)));
		g_RADY = f_Hesh->m_RotateY;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADY)));
		g_RADZ = f_Hesh->m_RotateZ;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADZ)));

		g_MatrixTP = XMMatrixTranspose(XMMatrixRotationY(g_RADZ));

#if 0
		int f_J = 5;
		int f_K = 5;
#endif
		}
	else
		{
		// Prepare to pass the updated model matrix to the shader
		g_RADX = ((float)Cube::randomFac() / 32767) * ((float)Cube::randomFac() / 32767) * 10;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADX)));
		g_RADY = ((float)Cube::randomFac() / 32767) * ((float)Cube::randomFac() / 32767) * 10;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADY)));
		g_RADZ = ((float)Cube::randomFac() / 32767) * ((float)Cube::randomFac() / 32767) * 10;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADZ)));
		}
#else
	if(g_VisualMiningMode == OK_MODE_VIEW)
		{
		g_RADX = 0;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADX)));
		g_RADY = 0;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADY)));
		g_RADZ = 0;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADZ)));

		g_MatrixTP = XMMatrixTranspose(XMMatrixRotationY(g_RADZ));
		}
	else
		{
		// Prepare to pass the updated model matrix to the shader
		g_RADX = 0;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADX)));
		g_RADY = 0;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADY)));
		g_RADZ = 0;
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(g_RADZ)));
		}
#endif
}

void Sample3DSceneRenderer::StartTracking()
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float positionX)
{
	if (m_tracking)
	{
		float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		Rotate(radians);
	}
}

void Sample3DSceneRenderer::StopTracking()
{
	m_tracking = false;
}

// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render(int f_Index)
{
	Cube::CubeKEY* f_KEY;

	if(g_VisualMiningMode == OK_MODE_MINE)
		{
		if(Cube::g_BicycleContainer[0].vec_Key.size() > 0)
			{
			int f_Finger = Cube::randomFac() % Cube::g_BicycleContainer[0].vec_Key.size();
			f_Finger = f_Index;
			f_KEY = Cube::g_BicycleContainer[0].vec_Key[f_Finger];
			g_vec_Cube.push_back(f_Finger);

#if 0
			MapResources(f_KEY);
#endif
			}
		else
			{
			return;
			}
		}
	else
		{
		if(Cube::g_BicycleContainer[0].vec_Key.size() > 0)
			{
#if 1
			int f_Finger = Cube::g_HESH[g_SelectedHeshID]->m_adIndex[f_Index];

			if(f_Finger < Cube::g_BicycleContainer[0].vec_Key.size())
				{
				f_KEY = Cube::g_BicycleContainer[0].vec_Key[f_Finger];
				}
			else f_KEY = Cube::g_BicycleContainer[0].vec_Key[Cube::g_BicycleContainer[0].vec_Key.size() - 1];
#else
			int f_Finger = Cube::randomFac() % Cube::g_BicycleContainer[0].vec_Key.size();
			f_KEY = Cube::g_BicycleContainer[0].vec_Key[f_Finger];
#endif
			g_vec_Cube.push_back(f_Finger);
			}
		else
			{
			return;
			}
		}

	// Loading is asynchronous. Only draw geometry after it's loaded.
	if(!m_loadingComplete)
		{
		return;
		}

	if(f_Index >= g_TargetedHesh->vec_Bicycle.size())
		{
		return;
		}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource1(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0,
		0
		);

#if 0
	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
	);
#else
	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;

	context->IASetVertexBuffers(
		0,
		1,
#if 0
		f_KEY->m_Link->m_vertexBuffer.GetAddressOf(),
#else
		g_TargetedHesh->vec_Bicycle[f_Index]->m_vertexBuffer.GetAddressOf(),
#endif
		&stride,
		&offset
		);

	context->IASetIndexBuffer(
#if 0
		m_indexBuffer.Get(),
#else
		g_TargetedHesh->vec_Bicycle[f_Index]->m_indexBuffer.Get(),
#endif
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
	);
#endif

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers1(
		0,
		1,
		m_constantBuffer.GetAddressOf(),
		nullptr,
		nullptr
		);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);

	// Draw the objects.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
		);
}

void Sample3DSceneRenderer::CreateDeviceDependentResources(void)
{
	if(g_VisualMiningMode == OK_MODE_MINE)
	{
			// Load shaders asynchronously.
			auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
			auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

			// After the vertex shader file is loaded, create the shader and input layout.
			auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
				DX::ThrowIfFailed(
					m_deviceResources->GetD3DDevice()->CreateVertexShader(
						&fileData[0],
						fileData.size(),
						nullptr,
						&m_vertexShader
						)
					);

				static const D3D11_INPUT_ELEMENT_DESC vertexDesc [] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};

				DX::ThrowIfFailed(
					m_deviceResources->GetD3DDevice()->CreateInputLayout(
						vertexDesc,
						ARRAYSIZE(vertexDesc),
						&fileData[0],
						fileData.size(),
						&m_inputLayout
						)
					);
			});

			// After the pixel shader file is loaded, create the shader and constant buffer.
			auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
				DX::ThrowIfFailed(
					m_deviceResources->GetD3DDevice()->CreatePixelShader(
						&fileData[0],
						fileData.size(),
						nullptr,
						&m_pixelShader
						)
					);

				CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer) , D3D11_BIND_CONSTANT_BUFFER);
				DX::ThrowIfFailed(
					m_deviceResources->GetD3DDevice()->CreateBuffer(
						&constantBufferDesc,
						nullptr,
						&m_constantBuffer
						)
					);
			});

			// Once both shaders are loaded, create the mesh.
			auto createCubeTask = (createPSTask && createVSTask).then([this] () {

				// Load mesh vertices. Each vertex has a position and a color.
				static const VertexPositionColor cubeVertices[] = 
				{
					{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
					{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
					{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
					{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
					{XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
					{XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
					{XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
					{XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
				};

				D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
				vertexBufferData.pSysMem = cubeVertices;
				vertexBufferData.SysMemPitch = 0;
				vertexBufferData.SysMemSlicePitch = 0;
				CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
				DX::ThrowIfFailed(
					m_deviceResources->GetD3DDevice()->CreateBuffer(
						&vertexBufferDesc,
						&vertexBufferData,
						&m_vertexBuffer
						)
					);

				// Load mesh indices. Each trio of indices represents
				// a triangle to be rendered on the screen.
				// For example: 0,2,1 means that the vertices with indexes
				// 0, 2 and 1 from the vertex buffer compose the 
				// first triangle of this mesh.
				static const unsigned short cubeIndices [] =
				{
					0,2,1, // -x
					1,2,3,

					4,5,6, // +x
					5,7,6,

					0,1,5, // -y
					0,5,4,

					2,6,7, // +y
					2,7,3,

					0,4,6, // -z
					0,6,2,

					1,3,7, // +z
					1,7,5,
				};

				m_indexCount = ARRAYSIZE(cubeIndices);

				D3D11_SUBRESOURCE_DATA indexBufferData = {0};
				indexBufferData.pSysMem = cubeIndices;
				indexBufferData.SysMemPitch = 0;
				indexBufferData.SysMemSlicePitch = 0;
				CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
				DX::ThrowIfFailed(
					m_deviceResources->GetD3DDevice()->CreateBuffer(
						&indexBufferDesc,
						&indexBufferData,
						&m_indexBuffer
						)
					);
			});

			// Once the cube is loaded, the object is ready to be rendered.
			createCubeTask.then([this] () {
				m_loadingComplete = true;
			});
#if 0
		Cube::g_BicycleContainer[0].vec_Key.clear();
#endif

		Cube::g_RandomSeed.x += 15.234124;
		Cube::g_RandomSeed.y += 13.1234;
		Cube::g_RandomSeed.z += 14.32487;
	
		Cube::CubeKEY* f_KEY = new Cube::CubeKEY();
		Cube::CubeBicycle* f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiRand f_BiRand;
		Cube::BiVector f_Vector1(-0.5f);
		f_Vector1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector2(-0.5f, -0.5f, 0.5f);
		f_Vector2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector3(-0.5f, 0.5f, -0.5f);
		f_Vector3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector4(-0.5f, 0.5f, 0.5f);
		f_Vector4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector5(0.5f, -0.5f, -0.5f);
		f_Vector5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector6(0.5f, -0.5f, 0.5f);
		f_Vector6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector7(0.5f, 0.5f, -0.5f);
		f_Vector7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector8(0.5f, 0.5f, 0.5f);
		f_Vector8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_Vector1.acRun_visualminer();
		f_Vector2.acRun_visualminer();
		f_Vector3.acRun_visualminer();
		f_Vector4.acRun_visualminer();
		f_Vector5.acRun_visualminer();
		f_Vector6.acRun_visualminer();
		f_Vector7.acRun_visualminer();
		f_Vector8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_Vector1);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector2);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector3);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector4);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector5);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector6);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector7);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector8);

		Cube::BiVector f_Color1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_Color1);
		f_KEY->m_Link->vec_Color.push_back(f_Color2);
		f_KEY->m_Link->vec_Color.push_back(f_Color3);
		f_KEY->m_Link->vec_Color.push_back(f_Color4);
		f_KEY->m_Link->vec_Color.push_back(f_Color5);
		f_KEY->m_Link->vec_Color.push_back(f_Color6);
		f_KEY->m_Link->vec_Color.push_back(f_Color7);
		f_KEY->m_Link->vec_Color.push_back(f_Color8);

		static const VertexPositionColor cubeVertices[] =
		{
			{ XMFLOAT3(f_Vector1.m_X, f_Vector1.m_Y, f_Vector1.m_Z), XMFLOAT3(f_Color1.m_X, f_Color1.m_Y, f_Color1.m_Z) },
			{ XMFLOAT3(f_Vector2.m_X, f_Vector2.m_Y, f_Vector2.m_Z), XMFLOAT3(f_Color2.m_X, f_Color2.m_Y, f_Color2.m_Z) },
			{ XMFLOAT3(f_Vector3.m_X, f_Vector3.m_Y, f_Vector3.m_Z), XMFLOAT3(f_Color3.m_X, f_Color3.m_Y, f_Color3.m_Z) },
			{ XMFLOAT3(f_Vector4.m_X, f_Vector4.m_Y, f_Vector4.m_Z), XMFLOAT3(f_Color4.m_X, f_Color4.m_Y, f_Color4.m_Z) },
			{ XMFLOAT3(f_Vector5.m_X, f_Vector5.m_Y, f_Vector5.m_Z), XMFLOAT3(f_Color5.m_X, f_Color5.m_Y, f_Color5.m_Z) },
			{ XMFLOAT3(f_Vector6.m_X, f_Vector6.m_Y, f_Vector6.m_Z), XMFLOAT3(f_Color6.m_X, f_Color6.m_Y, f_Color6.m_Z) },
			{ XMFLOAT3(f_Vector7.m_X, f_Vector7.m_Y, f_Vector7.m_Z), XMFLOAT3(f_Color7.m_X, f_Color7.m_Y, f_Color7.m_Z) },
			{ XMFLOAT3(f_Vector8.m_X, f_Vector8.m_Y, f_Vector8.m_Z), XMFLOAT3(f_Color8.m_X, f_Color8.m_Y, f_Color8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = cubeVertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = cubeIndices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorAA1(-0.5f);
		f_VectorAA1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAA2(-0.5f, -0.5f, 0.5f);
		f_VectorAA2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAA3(-0.5f, 0.5f, -0.5f);
		f_VectorAA3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAA4(-0.5f, 0.5f, 0.5f);
		f_VectorAA4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAA5(0.5f, -0.5f, -0.5f);
		f_VectorAA5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAA6(0.5f, -0.5f, 0.5f);
		f_VectorAA6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAA7(0.5f, 0.5f, -0.5f);
		f_VectorAA7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAA8(0.5f, 0.5f, 0.5f);
		f_VectorAA8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorAA1.acRun_visualminer();
		f_VectorAA2.acRun_visualminer();
		f_VectorAA3.acRun_visualminer();
		f_VectorAA4.acRun_visualminer();
		f_VectorAA5.acRun_visualminer();
		f_VectorAA6.acRun_visualminer();
		f_VectorAA7.acRun_visualminer();
		f_VectorAA8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAA1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAA2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAA3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAA4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAA5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAA6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAA7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAA8);

		Cube::BiVector f_ColorAA1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAA2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAA3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAA4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAA5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAA6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAA7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAA8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorAA1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAA2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAA3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAA4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAA5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAA6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAA7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAA8);

		static const VertexPositionColor cubeVertices1[] =
		{
			{ XMFLOAT3(f_VectorAA1.m_X, f_VectorAA1.m_Y, f_VectorAA1.m_Z), XMFLOAT3(f_ColorAA1.m_X, f_ColorAA1.m_Y, f_ColorAA1.m_Z) },
			{ XMFLOAT3(f_VectorAA2.m_X, f_VectorAA2.m_Y, f_VectorAA2.m_Z), XMFLOAT3(f_ColorAA2.m_X, f_ColorAA2.m_Y, f_ColorAA2.m_Z) },
			{ XMFLOAT3(f_VectorAA3.m_X, f_VectorAA3.m_Y, f_VectorAA3.m_Z), XMFLOAT3(f_ColorAA3.m_X, f_ColorAA3.m_Y, f_ColorAA3.m_Z) },
			{ XMFLOAT3(f_VectorAA4.m_X, f_VectorAA4.m_Y, f_VectorAA4.m_Z), XMFLOAT3(f_ColorAA4.m_X, f_ColorAA4.m_Y, f_ColorAA4.m_Z) },
			{ XMFLOAT3(f_VectorAA5.m_X, f_VectorAA5.m_Y, f_VectorAA5.m_Z), XMFLOAT3(f_ColorAA5.m_X, f_ColorAA5.m_Y, f_ColorAA5.m_Z) },
			{ XMFLOAT3(f_VectorAA6.m_X, f_VectorAA6.m_Y, f_VectorAA6.m_Z), XMFLOAT3(f_ColorAA6.m_X, f_ColorAA6.m_Y, f_ColorAA6.m_Z) },
			{ XMFLOAT3(f_VectorAA7.m_X, f_VectorAA7.m_Y, f_VectorAA7.m_Z), XMFLOAT3(f_ColorAA7.m_X, f_ColorAA7.m_Y, f_ColorAA7.m_Z) },
			{ XMFLOAT3(f_VectorAA8.m_X, f_VectorAA8.m_Y, f_VectorAA8.m_Z), XMFLOAT3(f_ColorAA8.m_X, f_ColorAA8.m_Y, f_ColorAA8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData1 = { 0 };
		vertexBufferData1.pSysMem = cubeVertices1;
		vertexBufferData1.SysMemPitch = 0;
		vertexBufferData1.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc1(sizeof(cubeVertices1), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc1,
				&vertexBufferData1,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices1[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices1);

		D3D11_SUBRESOURCE_DATA indexBufferData1 = { 0 };
		indexBufferData1.pSysMem = cubeIndices1;
		indexBufferData1.SysMemPitch = 0;
		indexBufferData1.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc1(sizeof(cubeIndices1), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc1,
				&indexBufferData1,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color
		Cube::BiVector f_VectorAAA1(-0.5f);
		f_VectorAAA1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAA2(-0.5f, -0.5f, 0.5f);
		f_VectorAAA2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAA3(-0.5f, 0.5f, -0.5f);
		f_VectorAAA3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAA4(-0.5f, 0.5f, 0.5f);
		f_VectorAAA4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAA5(0.5f, -0.5f, -0.5f);
		f_VectorAAA5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAA6(0.5f, -0.5f, 0.5f);
		f_VectorAAA6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAA7(0.5f, 0.5f, -0.5f);
		f_VectorAAA7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAA8(0.5f, 0.5f, 0.5f);
		f_VectorAAA8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorAAA1.acRun_visualminer();
		f_VectorAAA2.acRun_visualminer();
		f_VectorAAA3.acRun_visualminer();
		f_VectorAAA4.acRun_visualminer();
		f_VectorAAA5.acRun_visualminer();
		f_VectorAAA6.acRun_visualminer();
		f_VectorAAA7.acRun_visualminer();
		f_VectorAAA8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAA1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAA2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAA3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAA4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAA5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAA6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAA7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAA8);

		Cube::BiVector f_ColorAAA1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAA2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAA3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAA4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAA5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAA6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAA7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAA8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorAAA1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAA2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAA3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAA4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAA5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAA6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAA7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAA8);

		static const VertexPositionColor cubeVertices2[] =
		{
			{ XMFLOAT3(f_VectorAAA1.m_X, f_VectorAAA1.m_Y, f_VectorAAA1.m_Z), XMFLOAT3(f_ColorAAA1.m_X, f_ColorAAA1.m_Y, f_ColorAAA1.m_Z) },
			{ XMFLOAT3(f_VectorAAA2.m_X, f_VectorAAA2.m_Y, f_VectorAAA2.m_Z), XMFLOAT3(f_ColorAAA2.m_X, f_ColorAAA2.m_Y, f_ColorAAA2.m_Z) },
			{ XMFLOAT3(f_VectorAAA3.m_X, f_VectorAAA3.m_Y, f_VectorAAA3.m_Z), XMFLOAT3(f_ColorAAA3.m_X, f_ColorAAA3.m_Y, f_ColorAAA3.m_Z) },
			{ XMFLOAT3(f_VectorAAA4.m_X, f_VectorAAA4.m_Y, f_VectorAAA4.m_Z), XMFLOAT3(f_ColorAAA4.m_X, f_ColorAAA4.m_Y, f_ColorAAA4.m_Z) },
			{ XMFLOAT3(f_VectorAAA5.m_X, f_VectorAAA5.m_Y, f_VectorAAA5.m_Z), XMFLOAT3(f_ColorAAA5.m_X, f_ColorAAA5.m_Y, f_ColorAAA5.m_Z) },
			{ XMFLOAT3(f_VectorAAA6.m_X, f_VectorAAA6.m_Y, f_VectorAAA6.m_Z), XMFLOAT3(f_ColorAAA6.m_X, f_ColorAAA6.m_Y, f_ColorAAA6.m_Z) },
			{ XMFLOAT3(f_VectorAAA7.m_X, f_VectorAAA7.m_Y, f_VectorAAA7.m_Z), XMFLOAT3(f_ColorAAA7.m_X, f_ColorAAA7.m_Y, f_ColorAAA7.m_Z) },
			{ XMFLOAT3(f_VectorAAA8.m_X, f_VectorAAA8.m_Y, f_VectorAAA8.m_Z), XMFLOAT3(f_ColorAAA8.m_X, f_ColorAAA8.m_Y, f_ColorAAA8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData2 = { 0 };
		vertexBufferData2.pSysMem = cubeVertices2;
		vertexBufferData2.SysMemPitch = 0;
		vertexBufferData2.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc2(sizeof(cubeVertices2), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc2,
				&vertexBufferData2,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices2[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices2);

		D3D11_SUBRESOURCE_DATA indexBufferData2 = { 0 };
		indexBufferData2.pSysMem = cubeIndices2;
		indexBufferData2.SysMemPitch = 0;
		indexBufferData2.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc2(sizeof(cubeIndices2), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc2,
				&indexBufferData2,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorAAAA1(-0.5f);
		f_VectorAAAA1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAAA2(-0.5f, -0.5f, 0.5f);
		f_VectorAAAA2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAAA3(-0.5f, 0.5f, -0.5f);
		f_VectorAAAA3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAAA4(-0.5f, 0.5f, 0.5f);
		f_VectorAAAA4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAAA5(0.5f, -0.5f, -0.5f);
		f_VectorAAAA5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAAA6(0.5f, -0.5f, 0.5f);
		f_VectorAAAA6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAAA7(0.5f, 0.5f, -0.5f);
		f_VectorAAAA7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorAAAA8(0.5f, 0.5f, 0.5f);
		f_VectorAAAA8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorAAAA1.acRun_visualminer();
		f_VectorAAAA2.acRun_visualminer();
		f_VectorAAAA3.acRun_visualminer();
		f_VectorAAAA4.acRun_visualminer();
		f_VectorAAAA5.acRun_visualminer();
		f_VectorAAAA6.acRun_visualminer();
		f_VectorAAAA7.acRun_visualminer();
		f_VectorAAAA8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAAA1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAAA2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAAA3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAAA4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAAA5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAAA6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAAA7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorAAAA8);

		Cube::BiVector f_ColorAAAA1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAAA2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAAA3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAAA4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAAA5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAAA6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAAA7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorAAAA8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorAAAA1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAAA2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAAA3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAAA4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAAA5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAAA6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAAA7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorAAAA8);

		static const VertexPositionColor cubeVertices3[] =
		{
			{ XMFLOAT3(f_VectorAAAA1.m_X, f_VectorAAAA1.m_Y, f_VectorAAAA1.m_Z), XMFLOAT3(f_ColorAAAA1.m_X, f_ColorAAAA1.m_Y, f_ColorAAAA1.m_Z) },
			{ XMFLOAT3(f_VectorAAAA2.m_X, f_VectorAAAA2.m_Y, f_VectorAAAA2.m_Z), XMFLOAT3(f_ColorAAAA2.m_X, f_ColorAAAA2.m_Y, f_ColorAAAA2.m_Z) },
			{ XMFLOAT3(f_VectorAAAA3.m_X, f_VectorAAAA3.m_Y, f_VectorAAAA3.m_Z), XMFLOAT3(f_ColorAAAA3.m_X, f_ColorAAAA3.m_Y, f_ColorAAAA3.m_Z) },
			{ XMFLOAT3(f_VectorAAAA4.m_X, f_VectorAAAA4.m_Y, f_VectorAAAA4.m_Z), XMFLOAT3(f_ColorAAAA4.m_X, f_ColorAAAA4.m_Y, f_ColorAAAA4.m_Z) },
			{ XMFLOAT3(f_VectorAAAA5.m_X, f_VectorAAAA5.m_Y, f_VectorAAAA5.m_Z), XMFLOAT3(f_ColorAAAA5.m_X, f_ColorAAAA5.m_Y, f_ColorAAAA5.m_Z) },
			{ XMFLOAT3(f_VectorAAAA6.m_X, f_VectorAAAA6.m_Y, f_VectorAAAA6.m_Z), XMFLOAT3(f_ColorAAAA6.m_X, f_ColorAAAA6.m_Y, f_ColorAAAA6.m_Z) },
			{ XMFLOAT3(f_VectorAAAA7.m_X, f_VectorAAAA7.m_Y, f_VectorAAAA7.m_Z), XMFLOAT3(f_ColorAAAA7.m_X, f_ColorAAAA7.m_Y, f_ColorAAAA7.m_Z) },
			{ XMFLOAT3(f_VectorAAAA8.m_X, f_VectorAAAA8.m_Y, f_VectorAAAA8.m_Z), XMFLOAT3(f_ColorAAAA8.m_X, f_ColorAAAA8.m_Y, f_ColorAAAA8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData3 = { 0 };
		vertexBufferData3.pSysMem = cubeVertices3;
		vertexBufferData3.SysMemPitch = 0;
		vertexBufferData3.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc3(sizeof(cubeVertices3), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc3,
				&vertexBufferData3,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices3[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices3);

		D3D11_SUBRESOURCE_DATA indexBufferData3 = { 0 };
		indexBufferData3.pSysMem = cubeIndices3;
		indexBufferData3.SysMemPitch = 0;
		indexBufferData3.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc3(sizeof(cubeIndices3), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc3,
				&indexBufferData3,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorBB1(-0.5f);
		f_VectorBB1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorBB2(-0.5f, -0.5f, 0.5f);
		f_VectorBB2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorBB3(-0.5f, 0.5f, -0.5f);
		f_VectorBB3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorBB4(-0.5f, 0.5f, 0.5f);
		f_VectorBB4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorBB5(0.5f, -0.5f, -0.5f);
		f_VectorBB5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorBB6(0.5f, -0.5f, 0.5f);
		f_VectorBB6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorBB7(0.5f, 0.5f, -0.5f);
		f_VectorBB7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorBB8(0.5f, 0.5f, 0.5f);
		f_VectorBB8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorBB1.acRun_visualminer();
		f_VectorBB2.acRun_visualminer();
		f_VectorBB3.acRun_visualminer();
		f_VectorBB4.acRun_visualminer();
		f_VectorBB5.acRun_visualminer();
		f_VectorBB6.acRun_visualminer();
		f_VectorBB7.acRun_visualminer();
		f_VectorBB8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorBB1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorBB2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorBB3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorBB4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorBB5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorBB6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorBB7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorBB8);

		Cube::BiVector f_ColorBB1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorBB2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorBB3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorBB4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorBB5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorBB6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorBB7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorBB8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorBB1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorBB2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorBB3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorBB4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorBB5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorBB6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorBB7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorBB8);

		static const VertexPositionColor cubeVertices4[] =
		{
			{ XMFLOAT3(f_VectorBB1.m_X, f_VectorBB1.m_Y, f_VectorBB1.m_Z), XMFLOAT3(f_ColorBB1.m_X, f_ColorBB1.m_Y, f_ColorBB1.m_Z) },
			{ XMFLOAT3(f_VectorBB2.m_X, f_VectorBB2.m_Y, f_VectorBB2.m_Z), XMFLOAT3(f_ColorBB2.m_X, f_ColorBB2.m_Y, f_ColorBB2.m_Z) },
			{ XMFLOAT3(f_VectorBB3.m_X, f_VectorBB3.m_Y, f_VectorBB3.m_Z), XMFLOAT3(f_ColorBB3.m_X, f_ColorBB3.m_Y, f_ColorBB3.m_Z) },
			{ XMFLOAT3(f_VectorBB4.m_X, f_VectorBB4.m_Y, f_VectorBB4.m_Z), XMFLOAT3(f_ColorBB4.m_X, f_ColorBB4.m_Y, f_ColorBB4.m_Z) },
			{ XMFLOAT3(f_VectorBB5.m_X, f_VectorBB5.m_Y, f_VectorBB5.m_Z), XMFLOAT3(f_ColorBB5.m_X, f_ColorBB5.m_Y, f_ColorBB5.m_Z) },
			{ XMFLOAT3(f_VectorBB6.m_X, f_VectorBB6.m_Y, f_VectorBB6.m_Z), XMFLOAT3(f_ColorBB6.m_X, f_ColorBB6.m_Y, f_ColorBB6.m_Z) },
			{ XMFLOAT3(f_VectorBB7.m_X, f_VectorBB7.m_Y, f_VectorBB7.m_Z), XMFLOAT3(f_ColorBB7.m_X, f_ColorBB7.m_Y, f_ColorBB7.m_Z) },
			{ XMFLOAT3(f_VectorBB8.m_X, f_VectorBB8.m_Y, f_VectorBB8.m_Z), XMFLOAT3(f_ColorBB8.m_X, f_ColorBB8.m_Y, f_ColorBB8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData4 = { 0 };
		vertexBufferData4.pSysMem = cubeVertices4;
		vertexBufferData4.SysMemPitch = 0;
		vertexBufferData4.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc4(sizeof(cubeVertices4), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc4,
				&vertexBufferData4,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices4[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices4);

		D3D11_SUBRESOURCE_DATA indexBufferData4 = { 0 };
		indexBufferData4.pSysMem = cubeIndices4;
		indexBufferData4.SysMemPitch = 0;
		indexBufferData4.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc4(sizeof(cubeIndices4), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc4,
				&indexBufferData4,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorA1(-0.5f);
		f_VectorA1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorA2(-0.5f, -0.5f, 0.5f);
		f_VectorA2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorA3(-0.5f, 0.5f, -0.5f);
		f_VectorA3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorA4(-0.5f, 0.5f, 0.5f);
		f_VectorA4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorA5(0.5f, -0.5f, -0.5f);
		f_VectorA5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorA6(0.5f, -0.5f, 0.5f);
		f_VectorA6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorA7(0.5f, 0.5f, -0.5f);
		f_VectorA7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorA8(0.5f, 0.5f, 0.5f);
		f_VectorA8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorA1.acRun_visualminer();
		f_VectorA2.acRun_visualminer();
		f_VectorA3.acRun_visualminer();
		f_VectorA4.acRun_visualminer();
		f_VectorA5.acRun_visualminer();
		f_VectorA6.acRun_visualminer();
		f_VectorA7.acRun_visualminer();
		f_VectorA8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorA1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorA2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorA3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorA4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorA5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorA6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorA7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorA8);

		Cube::BiVector f_ColorA1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorA2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorA3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorA4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorA5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorA6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorA7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorA8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorA1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorA2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorA3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorA4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorA5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorA6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorA7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorA8);

		static const VertexPositionColor cubeVertices5[] =
		{
			{ XMFLOAT3(f_VectorA1.m_X, f_VectorA1.m_Y, f_VectorA1.m_Z), XMFLOAT3(f_ColorA1.m_X, f_ColorA1.m_Y, f_ColorA1.m_Z) },
			{ XMFLOAT3(f_VectorA2.m_X, f_VectorA2.m_Y, f_VectorA2.m_Z), XMFLOAT3(f_ColorA2.m_X, f_ColorA2.m_Y, f_ColorA2.m_Z) },
			{ XMFLOAT3(f_VectorA3.m_X, f_VectorA3.m_Y, f_VectorA3.m_Z), XMFLOAT3(f_ColorA3.m_X, f_ColorA3.m_Y, f_ColorA3.m_Z) },
			{ XMFLOAT3(f_VectorA4.m_X, f_VectorA4.m_Y, f_VectorA4.m_Z), XMFLOAT3(f_ColorA4.m_X, f_ColorA4.m_Y, f_ColorA4.m_Z) },
			{ XMFLOAT3(f_VectorA5.m_X, f_VectorA5.m_Y, f_VectorA5.m_Z), XMFLOAT3(f_ColorA5.m_X, f_ColorA5.m_Y, f_ColorA5.m_Z) },
			{ XMFLOAT3(f_VectorA6.m_X, f_VectorA6.m_Y, f_VectorA6.m_Z), XMFLOAT3(f_ColorA6.m_X, f_ColorA6.m_Y, f_ColorA6.m_Z) },
			{ XMFLOAT3(f_VectorA7.m_X, f_VectorA7.m_Y, f_VectorA7.m_Z), XMFLOAT3(f_ColorA7.m_X, f_ColorA7.m_Y, f_ColorA7.m_Z) },
			{ XMFLOAT3(f_VectorA8.m_X, f_VectorA8.m_Y, f_VectorA8.m_Z), XMFLOAT3(f_ColorA8.m_X, f_ColorA8.m_Y, f_ColorA8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData5 = { 0 };
		vertexBufferData5.pSysMem = cubeVertices;
		vertexBufferData5.SysMemPitch = 0;
		vertexBufferData5.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc5(sizeof(cubeVertices5), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc5,
				&vertexBufferData5,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices5[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices5);

		D3D11_SUBRESOURCE_DATA indexBufferData5 = { 0 };
		indexBufferData5.pSysMem = cubeIndices;
		indexBufferData5.SysMemPitch = 0;
		indexBufferData5.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc5(sizeof(cubeIndices5), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc5,
				&indexBufferData5,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorB1(-0.5f);
		f_VectorB1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorB2(-0.5f, -0.5f, 0.5f);
		f_VectorB2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorB3(-0.5f, 0.5f, -0.5f);
		f_VectorB3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorB4(-0.5f, 0.5f, 0.5f);
		f_VectorB4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorB5(0.5f, -0.5f, -0.5f);
		f_VectorB5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorB6(0.5f, -0.5f, 0.5f);
		f_VectorB6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorB7(0.5f, 0.5f, -0.5f);
		f_VectorB7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorB8(0.5f, 0.5f, 0.5f);
		f_VectorB8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorB1.acRun_visualminer();
		f_VectorB2.acRun_visualminer();
		f_VectorB3.acRun_visualminer();
		f_VectorB4.acRun_visualminer();
		f_VectorB5.acRun_visualminer();
		f_VectorB6.acRun_visualminer();
		f_VectorB7.acRun_visualminer();
		f_VectorB8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorB1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorB2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorB3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorB4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorB5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorB6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorB7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorB8);

		Cube::BiVector f_ColorB1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorB2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorB3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorB4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorB5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorB6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorB7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorB8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorB1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorB2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorB3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorB4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorB5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorB6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorB7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorB8);

		static const VertexPositionColor cubeVertices6[] =
		{
			{ XMFLOAT3(f_VectorB1.m_X, f_VectorB1.m_Y, f_VectorB1.m_Z), XMFLOAT3(f_ColorB1.m_X, f_ColorB1.m_Y, f_ColorB1.m_Z) },
			{ XMFLOAT3(f_VectorB2.m_X, f_VectorB2.m_Y, f_VectorB2.m_Z), XMFLOAT3(f_ColorB2.m_X, f_ColorB2.m_Y, f_ColorB2.m_Z) },
			{ XMFLOAT3(f_VectorB3.m_X, f_VectorB3.m_Y, f_VectorB3.m_Z), XMFLOAT3(f_ColorB3.m_X, f_ColorB3.m_Y, f_ColorB3.m_Z) },
			{ XMFLOAT3(f_VectorB4.m_X, f_VectorB4.m_Y, f_VectorB4.m_Z), XMFLOAT3(f_ColorB4.m_X, f_ColorB4.m_Y, f_ColorB4.m_Z) },
			{ XMFLOAT3(f_VectorB5.m_X, f_VectorB5.m_Y, f_VectorB5.m_Z), XMFLOAT3(f_ColorB5.m_X, f_ColorB5.m_Y, f_ColorB5.m_Z) },
			{ XMFLOAT3(f_VectorB6.m_X, f_VectorB6.m_Y, f_VectorB6.m_Z), XMFLOAT3(f_ColorB6.m_X, f_ColorB6.m_Y, f_ColorB6.m_Z) },
			{ XMFLOAT3(f_VectorB7.m_X, f_VectorB7.m_Y, f_VectorB7.m_Z), XMFLOAT3(f_ColorB7.m_X, f_ColorB7.m_Y, f_ColorB7.m_Z) },
			{ XMFLOAT3(f_VectorB8.m_X, f_VectorB8.m_Y, f_VectorB8.m_Z), XMFLOAT3(f_ColorB8.m_X, f_ColorB8.m_Y, f_ColorB8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData6 = { 0 };
		vertexBufferData6.pSysMem = cubeVertices6;
		vertexBufferData6.SysMemPitch = 0;
		vertexBufferData6.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc6(sizeof(cubeVertices6), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc6,
				&vertexBufferData6,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices6[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices6);

		D3D11_SUBRESOURCE_DATA indexBufferData6 = { 0 };
		indexBufferData6.pSysMem = cubeIndices6;
		indexBufferData6.SysMemPitch = 0;
		indexBufferData6.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc6(sizeof(cubeIndices6), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc6,
				&indexBufferData6,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorC1(-0.5f);
		f_VectorC1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorC2(-0.5f, -0.5f, 0.5f);
		f_VectorC2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorC3(-0.5f, 0.5f, -0.5f);
		f_VectorC3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorC4(-0.5f, 0.5f, 0.5f);
		f_VectorC4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorC5(0.5f, -0.5f, -0.5f);
		f_VectorC5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorC6(0.5f, -0.5f, 0.5f);
		f_VectorC6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorC7(0.5f, 0.5f, -0.5f);
		f_VectorC7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorC8(0.5f, 0.5f, 0.5f);
		f_VectorC8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorC1.acRun_visualminer();
		f_VectorC2.acRun_visualminer();
		f_VectorC3.acRun_visualminer();
		f_VectorC4.acRun_visualminer();
		f_VectorC5.acRun_visualminer();
		f_VectorC6.acRun_visualminer();
		f_VectorC7.acRun_visualminer();
		f_VectorC8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorC1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorC2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorC3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorC4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorC5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorC6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorC7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorC8);

		Cube::BiVector f_ColorC1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorC2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorC3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorC4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorC5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorC6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorC7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorC8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorC1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorC2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorC3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorC4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorC5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorC6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorC7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorC8);

		static const VertexPositionColor cubeVertices7[] =
		{
			{ XMFLOAT3(f_VectorC1.m_X, f_VectorC1.m_Y, f_VectorC1.m_Z), XMFLOAT3(f_ColorC1.m_X, f_ColorC1.m_Y, f_ColorC1.m_Z) },
			{ XMFLOAT3(f_VectorC2.m_X, f_VectorC2.m_Y, f_VectorC2.m_Z), XMFLOAT3(f_ColorC2.m_X, f_ColorC2.m_Y, f_ColorC2.m_Z) },
			{ XMFLOAT3(f_VectorC3.m_X, f_VectorC3.m_Y, f_VectorC3.m_Z), XMFLOAT3(f_ColorC3.m_X, f_ColorC3.m_Y, f_ColorC3.m_Z) },
			{ XMFLOAT3(f_VectorC4.m_X, f_VectorC4.m_Y, f_VectorC4.m_Z), XMFLOAT3(f_ColorC4.m_X, f_ColorC4.m_Y, f_ColorC4.m_Z) },
			{ XMFLOAT3(f_VectorC5.m_X, f_VectorC5.m_Y, f_VectorC5.m_Z), XMFLOAT3(f_ColorC5.m_X, f_ColorC5.m_Y, f_ColorC5.m_Z) },
			{ XMFLOAT3(f_VectorC6.m_X, f_VectorC6.m_Y, f_VectorC6.m_Z), XMFLOAT3(f_ColorC6.m_X, f_ColorC6.m_Y, f_ColorC6.m_Z) },
			{ XMFLOAT3(f_VectorC7.m_X, f_VectorC7.m_Y, f_VectorC7.m_Z), XMFLOAT3(f_ColorC7.m_X, f_ColorC7.m_Y, f_ColorC7.m_Z) },
			{ XMFLOAT3(f_VectorC8.m_X, f_VectorC8.m_Y, f_VectorC8.m_Z), XMFLOAT3(f_ColorC8.m_X, f_ColorC8.m_Y, f_ColorC8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData7 = { 0 };
		vertexBufferData7.pSysMem = cubeVertices7;
		vertexBufferData7.SysMemPitch = 0;
		vertexBufferData7.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc7(sizeof(cubeVertices7), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc7,
				&vertexBufferData7,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices7[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices7);

		D3D11_SUBRESOURCE_DATA indexBufferData7 = { 0 };
		indexBufferData7.pSysMem = cubeIndices7;
		indexBufferData7.SysMemPitch = 0;
		indexBufferData7.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc7(sizeof(cubeIndices7), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc7,
				&indexBufferData7,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorD1(-0.5f);
		f_VectorD1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorD2(-0.5f, -0.5f, 0.5f);
		f_VectorD2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorD3(-0.5f, 0.5f, -0.5f);
		f_VectorD3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorD4(-0.5f, 0.5f, 0.5f);
		f_VectorD4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorD5(0.5f, -0.5f, -0.5f);
		f_VectorD5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorD6(0.5f, -0.5f, 0.5f);
		f_VectorD6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorD7(0.5f, 0.5f, -0.5f);
		f_VectorD7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorD8(0.5f, 0.5f, 0.5f);
		f_VectorD8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorD1.acRun_visualminer();
		f_VectorD2.acRun_visualminer();
		f_VectorD3.acRun_visualminer();
		f_VectorD4.acRun_visualminer();
		f_VectorD5.acRun_visualminer();
		f_VectorD6.acRun_visualminer();
		f_VectorD7.acRun_visualminer();
		f_VectorD8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorD1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorD2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorD3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorD4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorD5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorD6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorD7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorD8);

		Cube::BiVector f_ColorD1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorD2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorD3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorD4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorD5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorD6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorD7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorD8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorD1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorD2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorD3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorD4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorD5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorD6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorD7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorD8);

		static const VertexPositionColor cubeVertices8[] =
		{
			{ XMFLOAT3(f_VectorD1.m_X, f_VectorD1.m_Y, f_VectorD1.m_Z), XMFLOAT3(f_ColorD1.m_X, f_ColorD1.m_Y, f_ColorD1.m_Z) },
			{ XMFLOAT3(f_VectorD2.m_X, f_VectorD2.m_Y, f_VectorD2.m_Z), XMFLOAT3(f_ColorD2.m_X, f_ColorD2.m_Y, f_ColorD2.m_Z) },
			{ XMFLOAT3(f_VectorD3.m_X, f_VectorD3.m_Y, f_VectorD3.m_Z), XMFLOAT3(f_ColorD3.m_X, f_ColorD3.m_Y, f_ColorD3.m_Z) },
			{ XMFLOAT3(f_VectorD4.m_X, f_VectorD4.m_Y, f_VectorD4.m_Z), XMFLOAT3(f_ColorD4.m_X, f_ColorD4.m_Y, f_ColorD4.m_Z) },
			{ XMFLOAT3(f_VectorD5.m_X, f_VectorD5.m_Y, f_VectorD5.m_Z), XMFLOAT3(f_ColorD5.m_X, f_ColorD5.m_Y, f_ColorD5.m_Z) },
			{ XMFLOAT3(f_VectorD6.m_X, f_VectorD6.m_Y, f_VectorD6.m_Z), XMFLOAT3(f_ColorD6.m_X, f_ColorD6.m_Y, f_ColorD6.m_Z) },
			{ XMFLOAT3(f_VectorD7.m_X, f_VectorD7.m_Y, f_VectorD7.m_Z), XMFLOAT3(f_ColorD7.m_X, f_ColorD7.m_Y, f_ColorD7.m_Z) },
			{ XMFLOAT3(f_VectorD8.m_X, f_VectorD8.m_Y, f_VectorD8.m_Z), XMFLOAT3(f_ColorD8.m_X, f_ColorD8.m_Y, f_ColorD8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData8 = { 0 };
		vertexBufferData8.pSysMem = cubeVertices8;
		vertexBufferData8.SysMemPitch = 0;
		vertexBufferData8.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc8(sizeof(cubeVertices8), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc8,
				&vertexBufferData8,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices8[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices8);

		D3D11_SUBRESOURCE_DATA indexBufferData8 = { 0 };
		indexBufferData8.pSysMem = cubeIndices8;
		indexBufferData8.SysMemPitch = 0;
		indexBufferData8.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc8(sizeof(cubeIndices3), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc8,
				&indexBufferData8,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorE1(-0.5f);
		f_VectorE1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorE2(-0.5f, -0.5f, 0.5f);
		f_VectorE2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorE3(-0.5f, 0.5f, -0.5f);
		f_VectorE3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorE4(-0.5f, 0.5f, 0.5f);
		f_VectorE4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorE5(0.5f, -0.5f, -0.5f);
		f_VectorE5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorE6(0.5f, -0.5f, 0.5f);
		f_VectorE6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorE7(0.5f, 0.5f, -0.5f);
		f_VectorE7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorE8(0.5f, 0.5f, 0.5f);
		f_VectorE8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorE1.acRun_visualminer();
		f_VectorE2.acRun_visualminer();
		f_VectorE3.acRun_visualminer();
		f_VectorE4.acRun_visualminer();
		f_VectorE5.acRun_visualminer();
		f_VectorE6.acRun_visualminer();
		f_VectorE7.acRun_visualminer();
		f_VectorE8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorE1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorE2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorE3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorE4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorE5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorE6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorE7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorE8);

		Cube::BiVector f_ColorE1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorE2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorE3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorE4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorE5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorE6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorE7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorE8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorE1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorE2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorE3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorE4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorE5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorE6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorE7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorE8);

		static const VertexPositionColor cubeVertices9[] =
		{
			{ XMFLOAT3(f_VectorE1.m_X, f_VectorE1.m_Y, f_VectorE1.m_Z), XMFLOAT3(f_ColorE1.m_X, f_ColorE1.m_Y, f_ColorE1.m_Z) },
			{ XMFLOAT3(f_VectorE2.m_X, f_VectorE2.m_Y, f_VectorE2.m_Z), XMFLOAT3(f_ColorE2.m_X, f_ColorE2.m_Y, f_ColorE2.m_Z) },
			{ XMFLOAT3(f_VectorE3.m_X, f_VectorE3.m_Y, f_VectorE3.m_Z), XMFLOAT3(f_ColorE3.m_X, f_ColorE3.m_Y, f_ColorE3.m_Z) },
			{ XMFLOAT3(f_VectorE4.m_X, f_VectorE4.m_Y, f_VectorE4.m_Z), XMFLOAT3(f_ColorE4.m_X, f_ColorE4.m_Y, f_ColorE4.m_Z) },
			{ XMFLOAT3(f_VectorE5.m_X, f_VectorE5.m_Y, f_VectorE5.m_Z), XMFLOAT3(f_ColorE5.m_X, f_ColorE5.m_Y, f_ColorE5.m_Z) },
			{ XMFLOAT3(f_VectorE6.m_X, f_VectorE6.m_Y, f_VectorE6.m_Z), XMFLOAT3(f_ColorE6.m_X, f_ColorE6.m_Y, f_ColorE6.m_Z) },
			{ XMFLOAT3(f_VectorE7.m_X, f_VectorE7.m_Y, f_VectorE7.m_Z), XMFLOAT3(f_ColorE7.m_X, f_ColorE7.m_Y, f_ColorE7.m_Z) },
			{ XMFLOAT3(f_VectorE8.m_X, f_VectorE8.m_Y, f_VectorE8.m_Z), XMFLOAT3(f_ColorE8.m_X, f_ColorE8.m_Y, f_ColorE8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData9 = { 0 };
		vertexBufferData9.pSysMem = cubeVertices9;
		vertexBufferData9.SysMemPitch = 0;
		vertexBufferData9.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc9(sizeof(cubeVertices9), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc9,
				&vertexBufferData9,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices9[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices9);

		D3D11_SUBRESOURCE_DATA indexBufferData9 = { 0 };
		indexBufferData9.pSysMem = cubeIndices9;
		indexBufferData9.SysMemPitch = 0;
		indexBufferData9.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc9(sizeof(cubeIndices9), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc9,
				&indexBufferData9,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorF1(-0.5f);
		f_VectorF1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorF2(-0.5f, -0.5f, 0.5f);
		f_VectorF2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorF3(-0.5f, 0.5f, -0.5f);
		f_VectorF3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorF4(-0.5f, 0.5f, 0.5f);
		f_VectorF4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorF5(0.5f, -0.5f, -0.5f);
		f_VectorF5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorF6(0.5f, -0.5f, 0.5f);
		f_VectorF6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorF7(0.5f, 0.5f, -0.5f);
		f_VectorF7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorF8(0.5f, 0.5f, 0.5f);
		f_VectorF8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorF1.acRun_visualminer();
		f_VectorF2.acRun_visualminer();
		f_VectorF3.acRun_visualminer();
		f_VectorF4.acRun_visualminer();
		f_VectorF5.acRun_visualminer();
		f_VectorF6.acRun_visualminer();
		f_VectorF7.acRun_visualminer();
		f_VectorF8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorF1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorF2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorF3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorF4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorF5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorF6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorF7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorF8);

		Cube::BiVector f_ColorF1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorF2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorF3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorF4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorF5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorF6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorF7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorF8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorF1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorF2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorF3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorF4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorF5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorF6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorF7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorF8);

		static const VertexPositionColor cubeVertices10[] =
		{
			{ XMFLOAT3(f_VectorF1.m_X, f_VectorF1.m_Y, f_VectorF1.m_Z), XMFLOAT3(f_ColorF1.m_X, f_ColorF1.m_Y, f_ColorF1.m_Z) },
			{ XMFLOAT3(f_VectorF2.m_X, f_VectorF2.m_Y, f_VectorF2.m_Z), XMFLOAT3(f_ColorF2.m_X, f_ColorF2.m_Y, f_ColorF2.m_Z) },
			{ XMFLOAT3(f_VectorF3.m_X, f_VectorF3.m_Y, f_VectorF3.m_Z), XMFLOAT3(f_ColorF3.m_X, f_ColorF3.m_Y, f_ColorF3.m_Z) },
			{ XMFLOAT3(f_VectorF4.m_X, f_VectorF4.m_Y, f_VectorF4.m_Z), XMFLOAT3(f_ColorF4.m_X, f_ColorF4.m_Y, f_ColorF4.m_Z) },
			{ XMFLOAT3(f_VectorF5.m_X, f_VectorF5.m_Y, f_VectorF5.m_Z), XMFLOAT3(f_ColorF5.m_X, f_ColorF5.m_Y, f_ColorF5.m_Z) },
			{ XMFLOAT3(f_VectorF6.m_X, f_VectorF6.m_Y, f_VectorF6.m_Z), XMFLOAT3(f_ColorF6.m_X, f_ColorF6.m_Y, f_ColorF6.m_Z) },
			{ XMFLOAT3(f_VectorF7.m_X, f_VectorF7.m_Y, f_VectorF7.m_Z), XMFLOAT3(f_ColorF7.m_X, f_ColorF7.m_Y, f_ColorF7.m_Z) },
			{ XMFLOAT3(f_VectorF8.m_X, f_VectorF8.m_Y, f_VectorF8.m_Z), XMFLOAT3(f_ColorF8.m_X, f_ColorF8.m_Y, f_ColorF8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData10 = { 0 };
		vertexBufferData10.pSysMem = cubeVertices10;
		vertexBufferData10.SysMemPitch = 0;
		vertexBufferData10.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc10(sizeof(cubeVertices10), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc10,
				&vertexBufferData10,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices10[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices10);

		D3D11_SUBRESOURCE_DATA indexBufferData10 = { 0 };
		indexBufferData10.pSysMem = cubeIndices10;
		indexBufferData10.SysMemPitch = 0;
		indexBufferData10.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc10(sizeof(cubeIndices10), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc10,
				&indexBufferData10,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorG1(-0.5f);
		f_VectorG1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorG2(-0.5f, -0.5f, 0.5f);
		f_VectorG2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorG3(-0.5f, 0.5f, -0.5f);
		f_VectorG3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorG4(-0.5f, 0.5f, 0.5f);
		f_VectorG4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorG5(0.5f, -0.5f, -0.5f);
		f_VectorG5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorG6(0.5f, -0.5f, 0.5f);
		f_VectorG6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorG7(0.5f, 0.5f, -0.5f);
		f_VectorG7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorG8(0.5f, 0.5f, 0.5f);
		f_VectorG8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorG1.acRun_visualminer();
		f_VectorG2.acRun_visualminer();
		f_VectorG3.acRun_visualminer();
		f_VectorG4.acRun_visualminer();
		f_VectorG5.acRun_visualminer();
		f_VectorG6.acRun_visualminer();
		f_VectorG7.acRun_visualminer();
		f_VectorG8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorG1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorG2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorG3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorG4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorG5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorG6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorG7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorG8);

		Cube::BiVector f_ColorG1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorG2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorG3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorG4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorG5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorG6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorG7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorG8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorG1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorG2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorG3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorG4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorG5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorG6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorG7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorG8);

		static const VertexPositionColor cubeVertices11[] =
		{
			{ XMFLOAT3(f_VectorG1.m_X, f_VectorG1.m_Y, f_VectorG1.m_Z), XMFLOAT3(f_ColorG1.m_X, f_ColorG1.m_Y, f_ColorG1.m_Z) },
			{ XMFLOAT3(f_VectorG2.m_X, f_VectorG2.m_Y, f_VectorG2.m_Z), XMFLOAT3(f_ColorG2.m_X, f_ColorG2.m_Y, f_ColorG2.m_Z) },
			{ XMFLOAT3(f_VectorG3.m_X, f_VectorG3.m_Y, f_VectorG3.m_Z), XMFLOAT3(f_ColorG3.m_X, f_ColorG3.m_Y, f_ColorG3.m_Z) },
			{ XMFLOAT3(f_VectorG4.m_X, f_VectorG4.m_Y, f_VectorG4.m_Z), XMFLOAT3(f_ColorG4.m_X, f_ColorG4.m_Y, f_ColorG4.m_Z) },
			{ XMFLOAT3(f_VectorG5.m_X, f_VectorG5.m_Y, f_VectorG5.m_Z), XMFLOAT3(f_ColorG5.m_X, f_ColorG5.m_Y, f_ColorG5.m_Z) },
			{ XMFLOAT3(f_VectorG6.m_X, f_VectorG6.m_Y, f_VectorG6.m_Z), XMFLOAT3(f_ColorG6.m_X, f_ColorG6.m_Y, f_ColorG6.m_Z) },
			{ XMFLOAT3(f_VectorG7.m_X, f_VectorG7.m_Y, f_VectorG7.m_Z), XMFLOAT3(f_ColorG7.m_X, f_ColorG7.m_Y, f_ColorG7.m_Z) },
			{ XMFLOAT3(f_VectorG8.m_X, f_VectorG8.m_Y, f_VectorG8.m_Z), XMFLOAT3(f_ColorG8.m_X, f_ColorG8.m_Y, f_ColorG8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData11 = { 0 };
		vertexBufferData11.pSysMem = cubeVertices11;
		vertexBufferData11.SysMemPitch = 0;
		vertexBufferData11.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc11(sizeof(cubeVertices11), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc11,
				&vertexBufferData11,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices11[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices11);

		D3D11_SUBRESOURCE_DATA indexBufferData11 = { 0 };
		indexBufferData11.pSysMem = cubeIndices11;
		indexBufferData11.SysMemPitch = 0;
		indexBufferData11.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc11(sizeof(cubeIndices11), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc11,
				&indexBufferData11,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorH1(-0.5f);
		f_VectorH1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorH2(-0.5f, -0.5f, 0.5f);
		f_VectorH2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorH3(-0.5f, 0.5f, -0.5f);
		f_VectorH3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorH4(-0.5f, 0.5f, 0.5f);
		f_VectorH4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorH5(0.5f, -0.5f, -0.5f);
		f_VectorH5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorH6(0.5f, -0.5f, 0.5f);
		f_VectorH6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorH7(0.5f, 0.5f, -0.5f);
		f_VectorH7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorH8(0.5f, 0.5f, 0.5f);
		f_VectorH8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorH1.acRun_visualminer();
		f_VectorH2.acRun_visualminer();
		f_VectorH3.acRun_visualminer();
		f_VectorH4.acRun_visualminer();
		f_VectorH5.acRun_visualminer();
		f_VectorH6.acRun_visualminer();
		f_VectorH7.acRun_visualminer();
		f_VectorH8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorH1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorH2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorH3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorH4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorH5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorH6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorH7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorH8);

		Cube::BiVector f_ColorH1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorH2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorH3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorH4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorH5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorH6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorH7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorH8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorH1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorH2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorH3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorH4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorH5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorH6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorH7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorH8);

		static const VertexPositionColor cubeVertices12[] =
		{
			{ XMFLOAT3(f_VectorH1.m_X, f_VectorH1.m_Y, f_VectorH1.m_Z), XMFLOAT3(f_ColorH1.m_X, f_ColorH1.m_Y, f_ColorH1.m_Z) },
			{ XMFLOAT3(f_VectorH2.m_X, f_VectorH2.m_Y, f_VectorH2.m_Z), XMFLOAT3(f_ColorH2.m_X, f_ColorH2.m_Y, f_ColorH2.m_Z) },
			{ XMFLOAT3(f_VectorH3.m_X, f_VectorH3.m_Y, f_VectorH3.m_Z), XMFLOAT3(f_ColorH3.m_X, f_ColorH3.m_Y, f_ColorH3.m_Z) },
			{ XMFLOAT3(f_VectorH4.m_X, f_VectorH4.m_Y, f_VectorH4.m_Z), XMFLOAT3(f_ColorH4.m_X, f_ColorH4.m_Y, f_ColorH4.m_Z) },
			{ XMFLOAT3(f_VectorH5.m_X, f_VectorH5.m_Y, f_VectorH5.m_Z), XMFLOAT3(f_ColorH5.m_X, f_ColorH5.m_Y, f_ColorH5.m_Z) },
			{ XMFLOAT3(f_VectorH6.m_X, f_VectorH6.m_Y, f_VectorH6.m_Z), XMFLOAT3(f_ColorH6.m_X, f_ColorH6.m_Y, f_ColorH6.m_Z) },
			{ XMFLOAT3(f_VectorH7.m_X, f_VectorH7.m_Y, f_VectorH7.m_Z), XMFLOAT3(f_ColorH7.m_X, f_ColorH7.m_Y, f_ColorH7.m_Z) },
			{ XMFLOAT3(f_VectorH8.m_X, f_VectorH8.m_Y, f_VectorH8.m_Z), XMFLOAT3(f_ColorH8.m_X, f_ColorH8.m_Y, f_ColorH8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData12 = { 0 };
		vertexBufferData12.pSysMem = cubeVertices12;
		vertexBufferData12.SysMemPitch = 0;
		vertexBufferData12.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc12(sizeof(cubeVertices12), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc12,
				&vertexBufferData12,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndices12[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndices12);

		D3D11_SUBRESOURCE_DATA indexBufferData12 = { 0 };
		indexBufferData12.pSysMem = cubeIndices12;
		indexBufferData12.SysMemPitch = 0;
		indexBufferData12.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc12(sizeof(cubeIndices12), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc12,
				&indexBufferData12,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLP1(-0.5f);
		f_VectorLP1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLP2(-0.5f, -0.5f, 0.5f);
		f_VectorLP2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLP3(-0.5f, 0.5f, -0.5f);
		f_VectorLP3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLP4(-0.5f, 0.5f, 0.5f);
		f_VectorLP4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLP5(0.5f, -0.5f, -0.5f);
		f_VectorLP5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLP6(0.5f, -0.5f, 0.5f);
		f_VectorLP6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLP7(0.5f, 0.5f, -0.5f);
		f_VectorLP7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLP8(0.5f, 0.5f, 0.5f);
		f_VectorLP8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLP1.acRun_visualminer();
		f_VectorLP2.acRun_visualminer();
		f_VectorLP3.acRun_visualminer();
		f_VectorLP4.acRun_visualminer();
		f_VectorLP5.acRun_visualminer();
		f_VectorLP6.acRun_visualminer();
		f_VectorLP7.acRun_visualminer();
		f_VectorLP8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLP1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLP2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLP3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLP4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLP5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLP6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLP7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLP8);

		Cube::BiVector f_ColorLP1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLP2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLP3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLP4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLP5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLP6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLP7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLP8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLP1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLP2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLP3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLP4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLP5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLP6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLP7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLP8);

		static const VertexPositionColor cubeVerticesLP[] =
		{
			{ XMFLOAT3(f_VectorLP1.m_X, f_VectorLP1.m_Y, f_VectorLP1.m_Z), XMFLOAT3(f_ColorLP1.m_X, f_ColorLP1.m_Y, f_ColorLP1.m_Z) },
			{ XMFLOAT3(f_VectorLP2.m_X, f_VectorLP2.m_Y, f_VectorLP2.m_Z), XMFLOAT3(f_ColorLP2.m_X, f_ColorLP2.m_Y, f_ColorLP2.m_Z) },
			{ XMFLOAT3(f_VectorLP3.m_X, f_VectorLP3.m_Y, f_VectorLP3.m_Z), XMFLOAT3(f_ColorLP3.m_X, f_ColorLP3.m_Y, f_ColorLP3.m_Z) },
			{ XMFLOAT3(f_VectorLP4.m_X, f_VectorLP4.m_Y, f_VectorLP4.m_Z), XMFLOAT3(f_ColorLP4.m_X, f_ColorLP4.m_Y, f_ColorLP4.m_Z) },
			{ XMFLOAT3(f_VectorLP5.m_X, f_VectorLP5.m_Y, f_VectorLP5.m_Z), XMFLOAT3(f_ColorLP5.m_X, f_ColorLP5.m_Y, f_ColorLP5.m_Z) },
			{ XMFLOAT3(f_VectorLP6.m_X, f_VectorLP6.m_Y, f_VectorLP6.m_Z), XMFLOAT3(f_ColorLP6.m_X, f_ColorLP6.m_Y, f_ColorLP6.m_Z) },
			{ XMFLOAT3(f_VectorLP7.m_X, f_VectorLP7.m_Y, f_VectorLP7.m_Z), XMFLOAT3(f_ColorLP7.m_X, f_ColorLP7.m_Y, f_ColorLP7.m_Z) },
			{ XMFLOAT3(f_VectorLP8.m_X, f_VectorLP8.m_Y, f_VectorLP8.m_Z), XMFLOAT3(f_ColorLP8.m_X, f_ColorLP8.m_Y, f_ColorLP8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP = { 0 };
		vertexBufferDataLP.pSysMem = cubeVerticesLP;
		vertexBufferDataLP.SysMemPitch = 0;
		vertexBufferDataLP.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP(sizeof(cubeVerticesLP), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP,
				&vertexBufferDataLP,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP = { 0 };
		indexBufferDataLP.pSysMem = cubeIndices;
		indexBufferDataLP.SysMemPitch = 0;
		indexBufferDataLP.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP(sizeof(cubeIndicesLP), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP,
				&indexBufferDataLP,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLPAA1(-0.5f);
		f_VectorLPAA1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAA2(-0.5f, -0.5f, 0.5f);
		f_VectorLPAA2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAA3(-0.5f, 0.5f, -0.5f);
		f_VectorLPAA3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAA4(-0.5f, 0.5f, 0.5f);
		f_VectorLPAA4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAA5(0.5f, -0.5f, -0.5f);
		f_VectorLPAA5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAA6(0.5f, -0.5f, 0.5f);
		f_VectorLPAA6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAA7(0.5f, 0.5f, -0.5f);
		f_VectorLPAA7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAA8(0.5f, 0.5f, 0.5f);
		f_VectorLPAA8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPAA1.acRun_visualminer();
		f_VectorLPAA2.acRun_visualminer();
		f_VectorLPAA3.acRun_visualminer();
		f_VectorLPAA4.acRun_visualminer();
		f_VectorLPAA5.acRun_visualminer();
		f_VectorLPAA6.acRun_visualminer();
		f_VectorLPAA7.acRun_visualminer();
		f_VectorLPAA8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAA1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAA2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAA3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAA4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAA5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAA6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAA7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAA8);

		Cube::BiVector f_ColorLPAA1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAA2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAA3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAA4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAA5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAA6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAA7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAA8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAA1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAA2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAA3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAA4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAA5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAA6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAA7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAA8);

		static const VertexPositionColor cubeVerticesLP1[] =
		{
			{ XMFLOAT3(f_VectorLPAA1.m_X, f_VectorLPAA1.m_Y, f_VectorLPAA1.m_Z), XMFLOAT3(f_ColorLPAA1.m_X, f_ColorLPAA1.m_Y, f_ColorLPAA1.m_Z) },
			{ XMFLOAT3(f_VectorLPAA2.m_X, f_VectorLPAA2.m_Y, f_VectorLPAA2.m_Z), XMFLOAT3(f_ColorLPAA2.m_X, f_ColorLPAA2.m_Y, f_ColorLPAA2.m_Z) },
			{ XMFLOAT3(f_VectorLPAA3.m_X, f_VectorLPAA3.m_Y, f_VectorLPAA3.m_Z), XMFLOAT3(f_ColorLPAA3.m_X, f_ColorLPAA3.m_Y, f_ColorLPAA3.m_Z) },
			{ XMFLOAT3(f_VectorLPAA4.m_X, f_VectorLPAA4.m_Y, f_VectorLPAA4.m_Z), XMFLOAT3(f_ColorLPAA4.m_X, f_ColorLPAA4.m_Y, f_ColorLPAA4.m_Z) },
			{ XMFLOAT3(f_VectorLPAA5.m_X, f_VectorLPAA5.m_Y, f_VectorLPAA5.m_Z), XMFLOAT3(f_ColorLPAA5.m_X, f_ColorLPAA5.m_Y, f_ColorLPAA5.m_Z) },
			{ XMFLOAT3(f_VectorLPAA6.m_X, f_VectorLPAA6.m_Y, f_VectorLPAA6.m_Z), XMFLOAT3(f_ColorLPAA6.m_X, f_ColorLPAA6.m_Y, f_ColorLPAA6.m_Z) },
			{ XMFLOAT3(f_VectorLPAA7.m_X, f_VectorLPAA7.m_Y, f_VectorLPAA7.m_Z), XMFLOAT3(f_ColorLPAA7.m_X, f_ColorLPAA7.m_Y, f_ColorLPAA7.m_Z) },
			{ XMFLOAT3(f_VectorLPAA8.m_X, f_VectorLPAA8.m_Y, f_VectorLPAA8.m_Z), XMFLOAT3(f_ColorLPAA8.m_X, f_ColorLPAA8.m_Y, f_ColorLPAA8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP1 = { 0 };
		vertexBufferDataLP1.pSysMem = cubeVerticesLP1;
		vertexBufferDataLP1.SysMemPitch = 0;
		vertexBufferDataLP1.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP1(sizeof(cubeVerticesLP1), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP1,
				&vertexBufferDataLP1,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP1[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP1);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP1 = { 0 };
		indexBufferDataLP1.pSysMem = cubeIndicesLP1;
		indexBufferDataLP1.SysMemPitch = 0;
		indexBufferDataLP1.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP1(sizeof(cubeIndicesLP1), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP1,
				&indexBufferDataLP1,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color
		Cube::BiVector f_VectorLPAAA1(-0.5f);
		f_VectorLPAAA1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAA2(-0.5f, -0.5f, 0.5f);
		f_VectorLPAAA2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAA3(-0.5f, 0.5f, -0.5f);
		f_VectorLPAAA3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAA4(-0.5f, 0.5f, 0.5f);
		f_VectorLPAAA4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAA5(0.5f, -0.5f, -0.5f);
		f_VectorLPAAA5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAA6(0.5f, -0.5f, 0.5f);
		f_VectorLPAAA6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAA7(0.5f, 0.5f, -0.5f);
		f_VectorLPAAA7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAA8(0.5f, 0.5f, 0.5f);
		f_VectorLPAAA8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPAAA1.acRun_visualminer();
		f_VectorLPAAA2.acRun_visualminer();
		f_VectorLPAAA3.acRun_visualminer();
		f_VectorLPAAA4.acRun_visualminer();
		f_VectorLPAAA5.acRun_visualminer();
		f_VectorLPAAA6.acRun_visualminer();
		f_VectorLPAAA7.acRun_visualminer();
		f_VectorLPAAA8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAA1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAA2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAA3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAA4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAA5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAA6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAA7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAA8);

		Cube::BiVector f_ColorLPAAA1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAA2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAA3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAA4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAA5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAA6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAA7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAA8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAA1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAA2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAA3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAA4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAA5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAA6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAA7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAA8);

		static const VertexPositionColor cubeVerticesLP2[] =
		{
			{ XMFLOAT3(f_VectorLPAAA1.m_X, f_VectorLPAAA1.m_Y, f_VectorLPAAA1.m_Z), XMFLOAT3(f_ColorLPAAA1.m_X, f_ColorLPAAA1.m_Y, f_ColorLPAAA1.m_Z) },
			{ XMFLOAT3(f_VectorLPAAA2.m_X, f_VectorLPAAA2.m_Y, f_VectorLPAAA2.m_Z), XMFLOAT3(f_ColorLPAAA2.m_X, f_ColorLPAAA2.m_Y, f_ColorLPAAA2.m_Z) },
			{ XMFLOAT3(f_VectorLPAAA3.m_X, f_VectorLPAAA3.m_Y, f_VectorLPAAA3.m_Z), XMFLOAT3(f_ColorLPAAA3.m_X, f_ColorLPAAA3.m_Y, f_ColorLPAAA3.m_Z) },
			{ XMFLOAT3(f_VectorLPAAA4.m_X, f_VectorLPAAA4.m_Y, f_VectorLPAAA4.m_Z), XMFLOAT3(f_ColorLPAAA4.m_X, f_ColorLPAAA4.m_Y, f_ColorLPAAA4.m_Z) },
			{ XMFLOAT3(f_VectorLPAAA5.m_X, f_VectorLPAAA5.m_Y, f_VectorLPAAA5.m_Z), XMFLOAT3(f_ColorLPAAA5.m_X, f_ColorLPAAA5.m_Y, f_ColorLPAAA5.m_Z) },
			{ XMFLOAT3(f_VectorLPAAA6.m_X, f_VectorLPAAA6.m_Y, f_VectorLPAAA6.m_Z), XMFLOAT3(f_ColorLPAAA6.m_X, f_ColorLPAAA6.m_Y, f_ColorLPAAA6.m_Z) },
			{ XMFLOAT3(f_VectorLPAAA7.m_X, f_VectorLPAAA7.m_Y, f_VectorLPAAA7.m_Z), XMFLOAT3(f_ColorLPAAA7.m_X, f_ColorLPAAA7.m_Y, f_ColorLPAAA7.m_Z) },
			{ XMFLOAT3(f_VectorLPAAA8.m_X, f_VectorLPAAA8.m_Y, f_VectorLPAAA8.m_Z), XMFLOAT3(f_ColorLPAAA8.m_X, f_ColorLPAAA8.m_Y, f_ColorLPAAA8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP2 = { 0 };
		vertexBufferDataLP2.pSysMem = cubeVerticesLP2;
		vertexBufferDataLP2.SysMemPitch = 0;
		vertexBufferDataLP2.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP2(sizeof(cubeVerticesLP2), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP2,
				&vertexBufferDataLP2,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP2[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP2);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP2 = { 0 };
		indexBufferDataLP2.pSysMem = cubeIndicesLP2;
		indexBufferDataLP2.SysMemPitch = 0;
		indexBufferDataLP2.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP2(sizeof(cubeIndicesLP2), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP2,
				&indexBufferDataLP2,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLPAAAA1(-0.5f);
		f_VectorLPAAAA1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAAA2(-0.5f, -0.5f, 0.5f);
		f_VectorLPAAAA2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAAA3(-0.5f, 0.5f, -0.5f);
		f_VectorLPAAAA3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAAA4(-0.5f, 0.5f, 0.5f);
		f_VectorLPAAAA4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAAA5(0.5f, -0.5f, -0.5f);
		f_VectorLPAAAA5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAAA6(0.5f, -0.5f, 0.5f);
		f_VectorLPAAAA6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAAA7(0.5f, 0.5f, -0.5f);
		f_VectorLPAAAA7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPAAAA8(0.5f, 0.5f, 0.5f);
		f_VectorLPAAAA8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPAAAA1.acRun_visualminer();
		f_VectorLPAAAA2.acRun_visualminer();
		f_VectorLPAAAA3.acRun_visualminer();
		f_VectorLPAAAA4.acRun_visualminer();
		f_VectorLPAAAA5.acRun_visualminer();
		f_VectorLPAAAA6.acRun_visualminer();
		f_VectorLPAAAA7.acRun_visualminer();
		f_VectorLPAAAA8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAAA1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAAA2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAAA3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAAA4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAAA5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAAA6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAAA7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPAAAA8);

		Cube::BiVector f_ColorLPAAAA1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAAA2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAAA3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAAA4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAAA5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAAA6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAAA7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPAAAA8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAAA1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAAA2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAAA3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAAA4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAAA5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAAA6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAAA7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPAAAA8);

		static const VertexPositionColor cubeVerticesLP3[] =
		{
			{ XMFLOAT3(f_VectorLPAAAA1.m_X, f_VectorLPAAAA1.m_Y, f_VectorLPAAAA1.m_Z), XMFLOAT3(f_ColorLPAAAA1.m_X, f_ColorLPAAAA1.m_Y, f_ColorLPAAAA1.m_Z) },
			{ XMFLOAT3(f_VectorLPAAAA2.m_X, f_VectorLPAAAA2.m_Y, f_VectorLPAAAA2.m_Z), XMFLOAT3(f_ColorLPAAAA2.m_X, f_ColorLPAAAA2.m_Y, f_ColorLPAAAA2.m_Z) },
			{ XMFLOAT3(f_VectorLPAAAA3.m_X, f_VectorLPAAAA3.m_Y, f_VectorLPAAAA3.m_Z), XMFLOAT3(f_ColorLPAAAA3.m_X, f_ColorLPAAAA3.m_Y, f_ColorLPAAAA3.m_Z) },
			{ XMFLOAT3(f_VectorLPAAAA4.m_X, f_VectorLPAAAA4.m_Y, f_VectorLPAAAA4.m_Z), XMFLOAT3(f_ColorLPAAAA4.m_X, f_ColorLPAAAA4.m_Y, f_ColorLPAAAA4.m_Z) },
			{ XMFLOAT3(f_VectorLPAAAA5.m_X, f_VectorLPAAAA5.m_Y, f_VectorLPAAAA5.m_Z), XMFLOAT3(f_ColorLPAAAA5.m_X, f_ColorLPAAAA5.m_Y, f_ColorLPAAAA5.m_Z) },
			{ XMFLOAT3(f_VectorLPAAAA6.m_X, f_VectorLPAAAA6.m_Y, f_VectorLPAAAA6.m_Z), XMFLOAT3(f_ColorLPAAAA6.m_X, f_ColorLPAAAA6.m_Y, f_ColorLPAAAA6.m_Z) },
			{ XMFLOAT3(f_VectorLPAAAA7.m_X, f_VectorLPAAAA7.m_Y, f_VectorLPAAAA7.m_Z), XMFLOAT3(f_ColorLPAAAA7.m_X, f_ColorLPAAAA7.m_Y, f_ColorLPAAAA7.m_Z) },
			{ XMFLOAT3(f_VectorLPAAAA8.m_X, f_VectorLPAAAA8.m_Y, f_VectorLPAAAA8.m_Z), XMFLOAT3(f_ColorLPAAAA8.m_X, f_ColorLPAAAA8.m_Y, f_ColorLPAAAA8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP3 = { 0 };
		vertexBufferDataLP3.pSysMem = cubeVerticesLP3;
		vertexBufferDataLP3.SysMemPitch = 0;
		vertexBufferDataLP3.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP3(sizeof(cubeVerticesLP3), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP3,
				&vertexBufferDataLP3,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP3[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP3);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP3 = { 0 };
		indexBufferDataLP3.pSysMem = cubeIndicesLP3;
		indexBufferDataLP3.SysMemPitch = 0;
		indexBufferDataLP3.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP3(sizeof(cubeIndicesLP3), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP3,
				&indexBufferDataLP3,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLPBB1(-0.5f);
		f_VectorLPBB1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPBB2(-0.5f, -0.5f, 0.5f);
		f_VectorLPBB2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPBB3(-0.5f, 0.5f, -0.5f);
		f_VectorLPBB3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPBB4(-0.5f, 0.5f, 0.5f);
		f_VectorLPBB4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPBB5(0.5f, -0.5f, -0.5f);
		f_VectorLPBB5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPBB6(0.5f, -0.5f, 0.5f);
		f_VectorLPBB6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPBB7(0.5f, 0.5f, -0.5f);
		f_VectorLPBB7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPBB8(0.5f, 0.5f, 0.5f);
		f_VectorLPBB8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPBB1.acRun_visualminer();
		f_VectorLPBB2.acRun_visualminer();
		f_VectorLPBB3.acRun_visualminer();
		f_VectorLPBB4.acRun_visualminer();
		f_VectorLPBB5.acRun_visualminer();
		f_VectorLPBB6.acRun_visualminer();
		f_VectorLPBB7.acRun_visualminer();
		f_VectorLPBB8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPBB1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPBB2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPBB3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPBB4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPBB5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPBB6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPBB7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPBB8);

		Cube::BiVector f_ColorLPBB1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPBB2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPBB3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPBB4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPBB5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPBB6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPBB7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPBB8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPBB1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPBB2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPBB3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPBB4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPBB5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPBB6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPBB7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPBB8);

		static const VertexPositionColor cubeVerticesLP4[] =
		{
			{ XMFLOAT3(f_VectorLPBB1.m_X, f_VectorLPBB1.m_Y, f_VectorLPBB1.m_Z), XMFLOAT3(f_ColorLPBB1.m_X, f_ColorLPBB1.m_Y, f_ColorLPBB1.m_Z) },
			{ XMFLOAT3(f_VectorLPBB2.m_X, f_VectorLPBB2.m_Y, f_VectorLPBB2.m_Z), XMFLOAT3(f_ColorLPBB2.m_X, f_ColorLPBB2.m_Y, f_ColorLPBB2.m_Z) },
			{ XMFLOAT3(f_VectorLPBB3.m_X, f_VectorLPBB3.m_Y, f_VectorLPBB3.m_Z), XMFLOAT3(f_ColorLPBB3.m_X, f_ColorLPBB3.m_Y, f_ColorLPBB3.m_Z) },
			{ XMFLOAT3(f_VectorLPBB4.m_X, f_VectorLPBB4.m_Y, f_VectorLPBB4.m_Z), XMFLOAT3(f_ColorLPBB4.m_X, f_ColorLPBB4.m_Y, f_ColorLPBB4.m_Z) },
			{ XMFLOAT3(f_VectorLPBB5.m_X, f_VectorLPBB5.m_Y, f_VectorLPBB5.m_Z), XMFLOAT3(f_ColorLPBB5.m_X, f_ColorLPBB5.m_Y, f_ColorLPBB5.m_Z) },
			{ XMFLOAT3(f_VectorLPBB6.m_X, f_VectorLPBB6.m_Y, f_VectorLPBB6.m_Z), XMFLOAT3(f_ColorLPBB6.m_X, f_ColorLPBB6.m_Y, f_ColorLPBB6.m_Z) },
			{ XMFLOAT3(f_VectorLPBB7.m_X, f_VectorLPBB7.m_Y, f_VectorLPBB7.m_Z), XMFLOAT3(f_ColorLPBB7.m_X, f_ColorLPBB7.m_Y, f_ColorLPBB7.m_Z) },
			{ XMFLOAT3(f_VectorLPBB8.m_X, f_VectorLPBB8.m_Y, f_VectorLPBB8.m_Z), XMFLOAT3(f_ColorLPBB8.m_X, f_ColorLPBB8.m_Y, f_ColorLPBB8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP4 = { 0 };
		vertexBufferDataLP4.pSysMem = cubeVerticesLP4;
		vertexBufferDataLP4.SysMemPitch = 0;
		vertexBufferDataLP4.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP4(sizeof(cubeVerticesLP4), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP4,
				&vertexBufferDataLP4,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP4[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP4);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP4 = { 0 };
		indexBufferDataLP4.pSysMem = cubeIndicesLP4;
		indexBufferDataLP4.SysMemPitch = 0;
		indexBufferDataLP4.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP4(sizeof(cubeIndicesLP4), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP4,
				&indexBufferDataLP4,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLPA1(-0.5f);
		f_VectorLPA1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPA2(-0.5f, -0.5f, 0.5f);
		f_VectorLPA2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPA3(-0.5f, 0.5f, -0.5f);
		f_VectorLPA3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPA4(-0.5f, 0.5f, 0.5f);
		f_VectorLPA4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPA5(0.5f, -0.5f, -0.5f);
		f_VectorLPA5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPA6(0.5f, -0.5f, 0.5f);
		f_VectorLPA6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPA7(0.5f, 0.5f, -0.5f);
		f_VectorLPA7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPA8(0.5f, 0.5f, 0.5f);
		f_VectorLPA8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPA1.acRun_visualminer();
		f_VectorLPA2.acRun_visualminer();
		f_VectorLPA3.acRun_visualminer();
		f_VectorLPA4.acRun_visualminer();
		f_VectorLPA5.acRun_visualminer();
		f_VectorLPA6.acRun_visualminer();
		f_VectorLPA7.acRun_visualminer();
		f_VectorLPA8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPA1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPA2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPA3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPA4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPA5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPA6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPA7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPA8);

		Cube::BiVector f_ColorLPA1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPA2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPA3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPA4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPA5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPA6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPA7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPA8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPA1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPA2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPA3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPA4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPA5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPA6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPA7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPA8);

		static const VertexPositionColor cubeVerticesLP5[] =
		{
			{ XMFLOAT3(f_VectorLPA1.m_X, f_VectorLPA1.m_Y, f_VectorLPA1.m_Z), XMFLOAT3(f_ColorLPA1.m_X, f_ColorLPA1.m_Y, f_ColorLPA1.m_Z) },
			{ XMFLOAT3(f_VectorLPA2.m_X, f_VectorLPA2.m_Y, f_VectorLPA2.m_Z), XMFLOAT3(f_ColorLPA2.m_X, f_ColorLPA2.m_Y, f_ColorLPA2.m_Z) },
			{ XMFLOAT3(f_VectorLPA3.m_X, f_VectorLPA3.m_Y, f_VectorLPA3.m_Z), XMFLOAT3(f_ColorLPA3.m_X, f_ColorLPA3.m_Y, f_ColorLPA3.m_Z) },
			{ XMFLOAT3(f_VectorLPA4.m_X, f_VectorLPA4.m_Y, f_VectorLPA4.m_Z), XMFLOAT3(f_ColorLPA4.m_X, f_ColorLPA4.m_Y, f_ColorLPA4.m_Z) },
			{ XMFLOAT3(f_VectorLPA5.m_X, f_VectorLPA5.m_Y, f_VectorLPA5.m_Z), XMFLOAT3(f_ColorLPA5.m_X, f_ColorLPA5.m_Y, f_ColorLPA5.m_Z) },
			{ XMFLOAT3(f_VectorLPA6.m_X, f_VectorLPA6.m_Y, f_VectorLPA6.m_Z), XMFLOAT3(f_ColorLPA6.m_X, f_ColorLPA6.m_Y, f_ColorLPA6.m_Z) },
			{ XMFLOAT3(f_VectorLPA7.m_X, f_VectorLPA7.m_Y, f_VectorLPA7.m_Z), XMFLOAT3(f_ColorLPA7.m_X, f_ColorLPA7.m_Y, f_ColorLPA7.m_Z) },
			{ XMFLOAT3(f_VectorLPA8.m_X, f_VectorLPA8.m_Y, f_VectorLPA8.m_Z), XMFLOAT3(f_ColorLPA8.m_X, f_ColorLPA8.m_Y, f_ColorLPA8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP5 = { 0 };
		vertexBufferDataLP5.pSysMem = cubeVerticesLP;
		vertexBufferDataLP5.SysMemPitch = 0;
		vertexBufferDataLP5.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP5(sizeof(cubeVerticesLP5), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP5,
				&vertexBufferDataLP5,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP5[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP5);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP5 = { 0 };
		indexBufferDataLP5.pSysMem = cubeIndicesLP;
		indexBufferDataLP5.SysMemPitch = 0;
		indexBufferDataLP5.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP5(sizeof(cubeIndicesLP5), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP5,
				&indexBufferDataLP5,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLPB1(-0.5f);
		f_VectorLPB1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPB2(-0.5f, -0.5f, 0.5f);
		f_VectorLPB2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPB3(-0.5f, 0.5f, -0.5f);
		f_VectorLPB3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPB4(-0.5f, 0.5f, 0.5f);
		f_VectorLPB4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPB5(0.5f, -0.5f, -0.5f);
		f_VectorLPB5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPB6(0.5f, -0.5f, 0.5f);
		f_VectorLPB6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPB7(0.5f, 0.5f, -0.5f);
		f_VectorLPB7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPB8(0.5f, 0.5f, 0.5f);
		f_VectorLPB8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPB1.acRun_visualminer();
		f_VectorLPB2.acRun_visualminer();
		f_VectorLPB3.acRun_visualminer();
		f_VectorLPB4.acRun_visualminer();
		f_VectorLPB5.acRun_visualminer();
		f_VectorLPB6.acRun_visualminer();
		f_VectorLPB7.acRun_visualminer();
		f_VectorLPB8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPB1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPB2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPB3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPB4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPB5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPB6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPB7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPB8);

		Cube::BiVector f_ColorLPB1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPB2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPB3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPB4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPB5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPB6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPB7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPB8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPB1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPB2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPB3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPB4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPB5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPB6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPB7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPB8);

		static const VertexPositionColor cubeVerticesLP6[] =
		{
			{ XMFLOAT3(f_VectorLPB1.m_X, f_VectorLPB1.m_Y, f_VectorLPB1.m_Z), XMFLOAT3(f_ColorLPB1.m_X, f_ColorLPB1.m_Y, f_ColorLPB1.m_Z) },
			{ XMFLOAT3(f_VectorLPB2.m_X, f_VectorLPB2.m_Y, f_VectorLPB2.m_Z), XMFLOAT3(f_ColorLPB2.m_X, f_ColorLPB2.m_Y, f_ColorLPB2.m_Z) },
			{ XMFLOAT3(f_VectorLPB3.m_X, f_VectorLPB3.m_Y, f_VectorLPB3.m_Z), XMFLOAT3(f_ColorLPB3.m_X, f_ColorLPB3.m_Y, f_ColorLPB3.m_Z) },
			{ XMFLOAT3(f_VectorLPB4.m_X, f_VectorLPB4.m_Y, f_VectorLPB4.m_Z), XMFLOAT3(f_ColorLPB4.m_X, f_ColorLPB4.m_Y, f_ColorLPB4.m_Z) },
			{ XMFLOAT3(f_VectorLPB5.m_X, f_VectorLPB5.m_Y, f_VectorLPB5.m_Z), XMFLOAT3(f_ColorLPB5.m_X, f_ColorLPB5.m_Y, f_ColorLPB5.m_Z) },
			{ XMFLOAT3(f_VectorLPB6.m_X, f_VectorLPB6.m_Y, f_VectorLPB6.m_Z), XMFLOAT3(f_ColorLPB6.m_X, f_ColorLPB6.m_Y, f_ColorLPB6.m_Z) },
			{ XMFLOAT3(f_VectorLPB7.m_X, f_VectorLPB7.m_Y, f_VectorLPB7.m_Z), XMFLOAT3(f_ColorLPB7.m_X, f_ColorLPB7.m_Y, f_ColorLPB7.m_Z) },
			{ XMFLOAT3(f_VectorLPB8.m_X, f_VectorLPB8.m_Y, f_VectorLPB8.m_Z), XMFLOAT3(f_ColorLPB8.m_X, f_ColorLPB8.m_Y, f_ColorLPB8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP6 = { 0 };
		vertexBufferDataLP6.pSysMem = cubeVerticesLP6;
		vertexBufferDataLP6.SysMemPitch = 0;
		vertexBufferDataLP6.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP6(sizeof(cubeVerticesLP6), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP6,
				&vertexBufferDataLP6,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP6[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP6);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP6 = { 0 };
		indexBufferDataLP6.pSysMem = cubeIndicesLP6;
		indexBufferDataLP6.SysMemPitch = 0;
		indexBufferDataLP6.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP6(sizeof(cubeIndicesLP6), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP6,
				&indexBufferDataLP6,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLPC1(-0.5f);
		f_VectorLPC1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPC2(-0.5f, -0.5f, 0.5f);
		f_VectorLPC2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPC3(-0.5f, 0.5f, -0.5f);
		f_VectorLPC3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPC4(-0.5f, 0.5f, 0.5f);
		f_VectorLPC4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPC5(0.5f, -0.5f, -0.5f);
		f_VectorLPC5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPC6(0.5f, -0.5f, 0.5f);
		f_VectorLPC6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPC7(0.5f, 0.5f, -0.5f);
		f_VectorLPC7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPC8(0.5f, 0.5f, 0.5f);
		f_VectorLPC8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPC1.acRun_visualminer();
		f_VectorLPC2.acRun_visualminer();
		f_VectorLPC3.acRun_visualminer();
		f_VectorLPC4.acRun_visualminer();
		f_VectorLPC5.acRun_visualminer();
		f_VectorLPC6.acRun_visualminer();
		f_VectorLPC7.acRun_visualminer();
		f_VectorLPC8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPC1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPC2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPC3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPC4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPC5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPC6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPC7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPC8);

		Cube::BiVector f_ColorLPC1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPC2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPC3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPC4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPC5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPC6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPC7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPC8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPC1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPC2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPC3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPC4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPC5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPC6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPC7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPC8);

		static const VertexPositionColor cubeVerticesLP7[] =
		{
			{ XMFLOAT3(f_VectorLPC1.m_X, f_VectorLPC1.m_Y, f_VectorLPC1.m_Z), XMFLOAT3(f_ColorLPC1.m_X, f_ColorLPC1.m_Y, f_ColorLPC1.m_Z) },
			{ XMFLOAT3(f_VectorLPC2.m_X, f_VectorLPC2.m_Y, f_VectorLPC2.m_Z), XMFLOAT3(f_ColorLPC2.m_X, f_ColorLPC2.m_Y, f_ColorLPC2.m_Z) },
			{ XMFLOAT3(f_VectorLPC3.m_X, f_VectorLPC3.m_Y, f_VectorLPC3.m_Z), XMFLOAT3(f_ColorLPC3.m_X, f_ColorLPC3.m_Y, f_ColorLPC3.m_Z) },
			{ XMFLOAT3(f_VectorLPC4.m_X, f_VectorLPC4.m_Y, f_VectorLPC4.m_Z), XMFLOAT3(f_ColorLPC4.m_X, f_ColorLPC4.m_Y, f_ColorLPC4.m_Z) },
			{ XMFLOAT3(f_VectorLPC5.m_X, f_VectorLPC5.m_Y, f_VectorLPC5.m_Z), XMFLOAT3(f_ColorLPC5.m_X, f_ColorLPC5.m_Y, f_ColorLPC5.m_Z) },
			{ XMFLOAT3(f_VectorLPC6.m_X, f_VectorLPC6.m_Y, f_VectorLPC6.m_Z), XMFLOAT3(f_ColorLPC6.m_X, f_ColorLPC6.m_Y, f_ColorLPC6.m_Z) },
			{ XMFLOAT3(f_VectorLPC7.m_X, f_VectorLPC7.m_Y, f_VectorLPC7.m_Z), XMFLOAT3(f_ColorLPC7.m_X, f_ColorLPC7.m_Y, f_ColorLPC7.m_Z) },
			{ XMFLOAT3(f_VectorLPC8.m_X, f_VectorLPC8.m_Y, f_VectorLPC8.m_Z), XMFLOAT3(f_ColorLPC8.m_X, f_ColorLPC8.m_Y, f_ColorLPC8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP7 = { 0 };
		vertexBufferDataLP7.pSysMem = cubeVerticesLP7;
		vertexBufferDataLP7.SysMemPitch = 0;
		vertexBufferDataLP7.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP7(sizeof(cubeVerticesLP7), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP7,
				&vertexBufferDataLP7,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP7[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP7);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP7 = { 0 };
		indexBufferDataLP7.pSysMem = cubeIndicesLP7;
		indexBufferDataLP7.SysMemPitch = 0;
		indexBufferDataLP7.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP7(sizeof(cubeIndicesLP7), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP7,
				&indexBufferDataLP7,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLPD1(-0.5f);
		f_VectorLPD1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPD2(-0.5f, -0.5f, 0.5f);
		f_VectorLPD2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPD3(-0.5f, 0.5f, -0.5f);
		f_VectorLPD3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPD4(-0.5f, 0.5f, 0.5f);
		f_VectorLPD4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPD5(0.5f, -0.5f, -0.5f);
		f_VectorLPD5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPD6(0.5f, -0.5f, 0.5f);
		f_VectorLPD6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPD7(0.5f, 0.5f, -0.5f);
		f_VectorLPD7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPD8(0.5f, 0.5f, 0.5f);
		f_VectorLPD8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPD1.acRun_visualminer();
		f_VectorLPD2.acRun_visualminer();
		f_VectorLPD3.acRun_visualminer();
		f_VectorLPD4.acRun_visualminer();
		f_VectorLPD5.acRun_visualminer();
		f_VectorLPD6.acRun_visualminer();
		f_VectorLPD7.acRun_visualminer();
		f_VectorLPD8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPD1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPD2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPD3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPD4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPD5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPD6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPD7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPD8);

		Cube::BiVector f_ColorLPD1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPD2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPD3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPD4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPD5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPD6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPD7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPD8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPD1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPD2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPD3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPD4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPD5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPD6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPD7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPD8);

		static const VertexPositionColor cubeVerticesLP8[] =
		{
			{ XMFLOAT3(f_VectorLPD1.m_X, f_VectorLPD1.m_Y, f_VectorLPD1.m_Z), XMFLOAT3(f_ColorLPD1.m_X, f_ColorLPD1.m_Y, f_ColorLPD1.m_Z) },
			{ XMFLOAT3(f_VectorLPD2.m_X, f_VectorLPD2.m_Y, f_VectorLPD2.m_Z), XMFLOAT3(f_ColorLPD2.m_X, f_ColorLPD2.m_Y, f_ColorLPD2.m_Z) },
			{ XMFLOAT3(f_VectorLPD3.m_X, f_VectorLPD3.m_Y, f_VectorLPD3.m_Z), XMFLOAT3(f_ColorLPD3.m_X, f_ColorLPD3.m_Y, f_ColorLPD3.m_Z) },
			{ XMFLOAT3(f_VectorLPD4.m_X, f_VectorLPD4.m_Y, f_VectorLPD4.m_Z), XMFLOAT3(f_ColorLPD4.m_X, f_ColorLPD4.m_Y, f_ColorLPD4.m_Z) },
			{ XMFLOAT3(f_VectorLPD5.m_X, f_VectorLPD5.m_Y, f_VectorLPD5.m_Z), XMFLOAT3(f_ColorLPD5.m_X, f_ColorLPD5.m_Y, f_ColorLPD5.m_Z) },
			{ XMFLOAT3(f_VectorLPD6.m_X, f_VectorLPD6.m_Y, f_VectorLPD6.m_Z), XMFLOAT3(f_ColorLPD6.m_X, f_ColorLPD6.m_Y, f_ColorLPD6.m_Z) },
			{ XMFLOAT3(f_VectorLPD7.m_X, f_VectorLPD7.m_Y, f_VectorLPD7.m_Z), XMFLOAT3(f_ColorLPD7.m_X, f_ColorLPD7.m_Y, f_ColorLPD7.m_Z) },
			{ XMFLOAT3(f_VectorLPD8.m_X, f_VectorLPD8.m_Y, f_VectorLPD8.m_Z), XMFLOAT3(f_ColorLPD8.m_X, f_ColorLPD8.m_Y, f_ColorLPD8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP8 = { 0 };
		vertexBufferDataLP8.pSysMem = cubeVerticesLP8;
		vertexBufferDataLP8.SysMemPitch = 0;
		vertexBufferDataLP8.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP8(sizeof(cubeVerticesLP8), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP8,
				&vertexBufferDataLP8,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP8[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP8);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP8 = { 0 };
		indexBufferDataLP8.pSysMem = cubeIndicesLP8;
		indexBufferDataLP8.SysMemPitch = 0;
		indexBufferDataLP8.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP8(sizeof(cubeIndicesLP3), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP8,
				&indexBufferDataLP8,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLPE1(-0.5f);
		f_VectorLPE1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPE2(-0.5f, -0.5f, 0.5f);
		f_VectorLPE2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPE3(-0.5f, 0.5f, -0.5f);
		f_VectorLPE3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPE4(-0.5f, 0.5f, 0.5f);
		f_VectorLPE4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPE5(0.5f, -0.5f, -0.5f);
		f_VectorLPE5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPE6(0.5f, -0.5f, 0.5f);
		f_VectorLPE6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPE7(0.5f, 0.5f, -0.5f);
		f_VectorLPE7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPE8(0.5f, 0.5f, 0.5f);
		f_VectorLPE8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPE1.acRun_visualminer();
		f_VectorLPE2.acRun_visualminer();
		f_VectorLPE3.acRun_visualminer();
		f_VectorLPE4.acRun_visualminer();
		f_VectorLPE5.acRun_visualminer();
		f_VectorLPE6.acRun_visualminer();
		f_VectorLPE7.acRun_visualminer();
		f_VectorLPE8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPE1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPE2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPE3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPE4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPE5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPE6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPE7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPE8);

		Cube::BiVector f_ColorLPE1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPE2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPE3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPE4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPE5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPE6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPE7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPE8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPE1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPE2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPE3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPE4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPE5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPE6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPE7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPE8);

		static const VertexPositionColor cubeVerticesLP9[] =
		{
			{ XMFLOAT3(f_VectorLPE1.m_X, f_VectorLPE1.m_Y, f_VectorLPE1.m_Z), XMFLOAT3(f_ColorLPE1.m_X, f_ColorLPE1.m_Y, f_ColorLPE1.m_Z) },
			{ XMFLOAT3(f_VectorLPE2.m_X, f_VectorLPE2.m_Y, f_VectorLPE2.m_Z), XMFLOAT3(f_ColorLPE2.m_X, f_ColorLPE2.m_Y, f_ColorLPE2.m_Z) },
			{ XMFLOAT3(f_VectorLPE3.m_X, f_VectorLPE3.m_Y, f_VectorLPE3.m_Z), XMFLOAT3(f_ColorLPE3.m_X, f_ColorLPE3.m_Y, f_ColorLPE3.m_Z) },
			{ XMFLOAT3(f_VectorLPE4.m_X, f_VectorLPE4.m_Y, f_VectorLPE4.m_Z), XMFLOAT3(f_ColorLPE4.m_X, f_ColorLPE4.m_Y, f_ColorLPE4.m_Z) },
			{ XMFLOAT3(f_VectorLPE5.m_X, f_VectorLPE5.m_Y, f_VectorLPE5.m_Z), XMFLOAT3(f_ColorLPE5.m_X, f_ColorLPE5.m_Y, f_ColorLPE5.m_Z) },
			{ XMFLOAT3(f_VectorLPE6.m_X, f_VectorLPE6.m_Y, f_VectorLPE6.m_Z), XMFLOAT3(f_ColorLPE6.m_X, f_ColorLPE6.m_Y, f_ColorLPE6.m_Z) },
			{ XMFLOAT3(f_VectorLPE7.m_X, f_VectorLPE7.m_Y, f_VectorLPE7.m_Z), XMFLOAT3(f_ColorLPE7.m_X, f_ColorLPE7.m_Y, f_ColorLPE7.m_Z) },
			{ XMFLOAT3(f_VectorLPE8.m_X, f_VectorLPE8.m_Y, f_VectorLPE8.m_Z), XMFLOAT3(f_ColorLPE8.m_X, f_ColorLPE8.m_Y, f_ColorLPE8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP9 = { 0 };
		vertexBufferDataLP9.pSysMem = cubeVerticesLP9;
		vertexBufferDataLP9.SysMemPitch = 0;
		vertexBufferDataLP9.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP9(sizeof(cubeVerticesLP9), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP9,
				&vertexBufferDataLP9,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP9[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP9);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP9 = { 0 };
		indexBufferDataLP9.pSysMem = cubeIndicesLP9;
		indexBufferDataLP9.SysMemPitch = 0;
		indexBufferDataLP9.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP9(sizeof(cubeIndicesLP9), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP9,
				&indexBufferDataLP9,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLPF1(-0.5f);
		f_VectorLPF1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPF2(-0.5f, -0.5f, 0.5f);
		f_VectorLPF2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPF3(-0.5f, 0.5f, -0.5f);
		f_VectorLPF3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPF4(-0.5f, 0.5f, 0.5f);
		f_VectorLPF4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPF5(0.5f, -0.5f, -0.5f);
		f_VectorLPF5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPF6(0.5f, -0.5f, 0.5f);
		f_VectorLPF6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPF7(0.5f, 0.5f, -0.5f);
		f_VectorLPF7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPF8(0.5f, 0.5f, 0.5f);
		f_VectorLPF8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPF1.acRun_visualminer();
		f_VectorLPF2.acRun_visualminer();
		f_VectorLPF3.acRun_visualminer();
		f_VectorLPF4.acRun_visualminer();
		f_VectorLPF5.acRun_visualminer();
		f_VectorLPF6.acRun_visualminer();
		f_VectorLPF7.acRun_visualminer();
		f_VectorLPF8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPF1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPF2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPF3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPF4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPF5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPF6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPF7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPF8);

		Cube::BiVector f_ColorLPF1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPF2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPF3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPF4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPF5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPF6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPF7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPF8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPF1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPF2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPF3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPF4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPF5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPF6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPF7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPF8);

		static const VertexPositionColor cubeVerticesLP10[] =
		{
			{ XMFLOAT3(f_VectorLPF1.m_X, f_VectorLPF1.m_Y, f_VectorLPF1.m_Z), XMFLOAT3(f_ColorLPF1.m_X, f_ColorLPF1.m_Y, f_ColorLPF1.m_Z) },
			{ XMFLOAT3(f_VectorLPF2.m_X, f_VectorLPF2.m_Y, f_VectorLPF2.m_Z), XMFLOAT3(f_ColorLPF2.m_X, f_ColorLPF2.m_Y, f_ColorLPF2.m_Z) },
			{ XMFLOAT3(f_VectorLPF3.m_X, f_VectorLPF3.m_Y, f_VectorLPF3.m_Z), XMFLOAT3(f_ColorLPF3.m_X, f_ColorLPF3.m_Y, f_ColorLPF3.m_Z) },
			{ XMFLOAT3(f_VectorLPF4.m_X, f_VectorLPF4.m_Y, f_VectorLPF4.m_Z), XMFLOAT3(f_ColorLPF4.m_X, f_ColorLPF4.m_Y, f_ColorLPF4.m_Z) },
			{ XMFLOAT3(f_VectorLPF5.m_X, f_VectorLPF5.m_Y, f_VectorLPF5.m_Z), XMFLOAT3(f_ColorLPF5.m_X, f_ColorLPF5.m_Y, f_ColorLPF5.m_Z) },
			{ XMFLOAT3(f_VectorLPF6.m_X, f_VectorLPF6.m_Y, f_VectorLPF6.m_Z), XMFLOAT3(f_ColorLPF6.m_X, f_ColorLPF6.m_Y, f_ColorLPF6.m_Z) },
			{ XMFLOAT3(f_VectorLPF7.m_X, f_VectorLPF7.m_Y, f_VectorLPF7.m_Z), XMFLOAT3(f_ColorLPF7.m_X, f_ColorLPF7.m_Y, f_ColorLPF7.m_Z) },
			{ XMFLOAT3(f_VectorLPF8.m_X, f_VectorLPF8.m_Y, f_VectorLPF8.m_Z), XMFLOAT3(f_ColorLPF8.m_X, f_ColorLPF8.m_Y, f_ColorLPF8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP10 = { 0 };
		vertexBufferDataLP10.pSysMem = cubeVerticesLP10;
		vertexBufferDataLP10.SysMemPitch = 0;
		vertexBufferDataLP10.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP10(sizeof(cubeVerticesLP10), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP10,
				&vertexBufferDataLP10,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP10[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP10);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP10 = { 0 };
		indexBufferDataLP10.pSysMem = cubeIndicesLP10;
		indexBufferDataLP10.SysMemPitch = 0;
		indexBufferDataLP10.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP10(sizeof(cubeIndicesLP10), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP10,
				&indexBufferDataLP10,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLPG1(-0.5f);
		f_VectorLPG1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPG2(-0.5f, -0.5f, 0.5f);
		f_VectorLPG2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPG3(-0.5f, 0.5f, -0.5f);
		f_VectorLPG3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPG4(-0.5f, 0.5f, 0.5f);
		f_VectorLPG4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPG5(0.5f, -0.5f, -0.5f);
		f_VectorLPG5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPG6(0.5f, -0.5f, 0.5f);
		f_VectorLPG6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPG7(0.5f, 0.5f, -0.5f);
		f_VectorLPG7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPG8(0.5f, 0.5f, 0.5f);
		f_VectorLPG8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPG1.acRun_visualminer();
		f_VectorLPG2.acRun_visualminer();
		f_VectorLPG3.acRun_visualminer();
		f_VectorLPG4.acRun_visualminer();
		f_VectorLPG5.acRun_visualminer();
		f_VectorLPG6.acRun_visualminer();
		f_VectorLPG7.acRun_visualminer();
		f_VectorLPG8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPG1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPG2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPG3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPG4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPG5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPG6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPG7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPG8);

		Cube::BiVector f_ColorLPG1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPG2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPG3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPG4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPG5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPG6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPG7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPG8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPG1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPG2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPG3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPG4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPG5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPG6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPG7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPG8);

		static const VertexPositionColor cubeVerticesLP11[] =
		{
			{ XMFLOAT3(f_VectorLPG1.m_X, f_VectorLPG1.m_Y, f_VectorLPG1.m_Z), XMFLOAT3(f_ColorLPG1.m_X, f_ColorLPG1.m_Y, f_ColorLPG1.m_Z) },
			{ XMFLOAT3(f_VectorLPG2.m_X, f_VectorLPG2.m_Y, f_VectorLPG2.m_Z), XMFLOAT3(f_ColorLPG2.m_X, f_ColorLPG2.m_Y, f_ColorLPG2.m_Z) },
			{ XMFLOAT3(f_VectorLPG3.m_X, f_VectorLPG3.m_Y, f_VectorLPG3.m_Z), XMFLOAT3(f_ColorLPG3.m_X, f_ColorLPG3.m_Y, f_ColorLPG3.m_Z) },
			{ XMFLOAT3(f_VectorLPG4.m_X, f_VectorLPG4.m_Y, f_VectorLPG4.m_Z), XMFLOAT3(f_ColorLPG4.m_X, f_ColorLPG4.m_Y, f_ColorLPG4.m_Z) },
			{ XMFLOAT3(f_VectorLPG5.m_X, f_VectorLPG5.m_Y, f_VectorLPG5.m_Z), XMFLOAT3(f_ColorLPG5.m_X, f_ColorLPG5.m_Y, f_ColorLPG5.m_Z) },
			{ XMFLOAT3(f_VectorLPG6.m_X, f_VectorLPG6.m_Y, f_VectorLPG6.m_Z), XMFLOAT3(f_ColorLPG6.m_X, f_ColorLPG6.m_Y, f_ColorLPG6.m_Z) },
			{ XMFLOAT3(f_VectorLPG7.m_X, f_VectorLPG7.m_Y, f_VectorLPG7.m_Z), XMFLOAT3(f_ColorLPG7.m_X, f_ColorLPG7.m_Y, f_ColorLPG7.m_Z) },
			{ XMFLOAT3(f_VectorLPG8.m_X, f_VectorLPG8.m_Y, f_VectorLPG8.m_Z), XMFLOAT3(f_ColorLPG8.m_X, f_ColorLPG8.m_Y, f_ColorLPG8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP11 = { 0 };
		vertexBufferDataLP11.pSysMem = cubeVerticesLP11;
		vertexBufferDataLP11.SysMemPitch = 0;
		vertexBufferDataLP11.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP11(sizeof(cubeVerticesLP11), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP11,
				&vertexBufferDataLP11,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP11[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP11);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP11 = { 0 };
		indexBufferDataLP11.pSysMem = cubeIndicesLP11;
		indexBufferDataLP11.SysMemPitch = 0;
		indexBufferDataLP11.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP11(sizeof(cubeIndicesLP11), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP11,
				&indexBufferDataLP11,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();
		f_Bicycle = new Cube::CubeBicycle();

		f_KEY->m_Link = f_Bicycle;

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiVector f_VectorLPH1(-0.5f);
		f_VectorLPH1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPH2(-0.5f, -0.5f, 0.5f);
		f_VectorLPH2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPH3(-0.5f, 0.5f, -0.5f);
		f_VectorLPH3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPH4(-0.5f, 0.5f, 0.5f);
		f_VectorLPH4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPH5(0.5f, -0.5f, -0.5f);
		f_VectorLPH5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPH6(0.5f, -0.5f, 0.5f);
		f_VectorLPH6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPH7(0.5f, 0.5f, -0.5f);
		f_VectorLPH7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_VectorLPH8(0.5f, 0.5f, 0.5f);
		f_VectorLPH8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5), ((Cube::random() - 0.5) * 13.5)));

		f_VectorLPH1.acRun_visualminer();
		f_VectorLPH2.acRun_visualminer();
		f_VectorLPH3.acRun_visualminer();
		f_VectorLPH4.acRun_visualminer();
		f_VectorLPH5.acRun_visualminer();
		f_VectorLPH6.acRun_visualminer();
		f_VectorLPH7.acRun_visualminer();
		f_VectorLPH8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPH1);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPH2);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPH3);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPH4);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPH5);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPH6);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPH7);
		f_KEY->m_Link->vec_Vertex.push_back(f_VectorLPH8);

		Cube::BiVector f_ColorLPH1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPH2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPH3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPH4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPH5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPH6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPH7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_ColorLPH8(Cube::random(), Cube::random(), Cube::random());

		f_KEY->m_Link->vec_Color.push_back(f_ColorLPH1);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPH2);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPH3);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPH4);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPH5);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPH6);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPH7);
		f_KEY->m_Link->vec_Color.push_back(f_ColorLPH8);

		static const VertexPositionColor cubeVerticesLP12[] =
		{
			{ XMFLOAT3(f_VectorLPH1.m_X, f_VectorLPH1.m_Y, f_VectorLPH1.m_Z), XMFLOAT3(f_ColorLPH1.m_X, f_ColorLPH1.m_Y, f_ColorLPH1.m_Z) },
			{ XMFLOAT3(f_VectorLPH2.m_X, f_VectorLPH2.m_Y, f_VectorLPH2.m_Z), XMFLOAT3(f_ColorLPH2.m_X, f_ColorLPH2.m_Y, f_ColorLPH2.m_Z) },
			{ XMFLOAT3(f_VectorLPH3.m_X, f_VectorLPH3.m_Y, f_VectorLPH3.m_Z), XMFLOAT3(f_ColorLPH3.m_X, f_ColorLPH3.m_Y, f_ColorLPH3.m_Z) },
			{ XMFLOAT3(f_VectorLPH4.m_X, f_VectorLPH4.m_Y, f_VectorLPH4.m_Z), XMFLOAT3(f_ColorLPH4.m_X, f_ColorLPH4.m_Y, f_ColorLPH4.m_Z) },
			{ XMFLOAT3(f_VectorLPH5.m_X, f_VectorLPH5.m_Y, f_VectorLPH5.m_Z), XMFLOAT3(f_ColorLPH5.m_X, f_ColorLPH5.m_Y, f_ColorLPH5.m_Z) },
			{ XMFLOAT3(f_VectorLPH6.m_X, f_VectorLPH6.m_Y, f_VectorLPH6.m_Z), XMFLOAT3(f_ColorLPH6.m_X, f_ColorLPH6.m_Y, f_ColorLPH6.m_Z) },
			{ XMFLOAT3(f_VectorLPH7.m_X, f_VectorLPH7.m_Y, f_VectorLPH7.m_Z), XMFLOAT3(f_ColorLPH7.m_X, f_ColorLPH7.m_Y, f_ColorLPH7.m_Z) },
			{ XMFLOAT3(f_VectorLPH8.m_X, f_VectorLPH8.m_Y, f_VectorLPH8.m_Z), XMFLOAT3(f_ColorLPH8.m_X, f_ColorLPH8.m_Y, f_ColorLPH8.m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferDataLP12 = { 0 };
		vertexBufferDataLP12.pSysMem = cubeVerticesLP12;
		vertexBufferDataLP12.SysMemPitch = 0;
		vertexBufferDataLP12.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDescLP12(sizeof(cubeVerticesLP12), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDescLP12,
				&vertexBufferDataLP12,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		static const unsigned short cubeIndicesLP12[] =
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		f_KEY->m_Link->m_indexCount = ARRAYSIZE(cubeIndicesLP12);

		D3D11_SUBRESOURCE_DATA indexBufferDataLP12 = { 0 };
		indexBufferDataLP12.pSysMem = cubeIndicesLP12;
		indexBufferDataLP12.SysMemPitch = 0;
		indexBufferDataLP12.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDescLP12(sizeof(cubeIndicesLP12), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDescLP12,
				&indexBufferDataLP12,
				&f_KEY->m_Link->m_indexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);
	}
else
	{
#if 1
	Cube::g_BicycleContainer[0].vec_Key.clear();
#endif

	Cube::CubeKEY* f_KEY;
	Cube::CubeBicycle* f_Bicycle;

	for(int f_offset = 0; f_offset < Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle.size(); f_offset+=13)
		{
		f_KEY = new Cube::CubeKEY();

		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = cubeVertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();


		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 1];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices1[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData1 = { 0 };
		vertexBufferData1.pSysMem = cubeVertices1;
		vertexBufferData1.SysMemPitch = 0;
		vertexBufferData1.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc1(sizeof(cubeVertices1), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc1,
				&vertexBufferData1,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();


		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 2];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices2[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData2 = { 0 };
		vertexBufferData2.pSysMem = cubeVertices2;
		vertexBufferData2.SysMemPitch = 0;
		vertexBufferData2.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc2(sizeof(cubeVertices2), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc2,
				&vertexBufferData2,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();


		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 3];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices3[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData3 = { 0 };
		vertexBufferData3.pSysMem = cubeVertices3;
		vertexBufferData3.SysMemPitch = 0;
		vertexBufferData3.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc3(sizeof(cubeVertices3), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc3,
				&vertexBufferData3,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();


		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 4];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices4[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData4 = { 0 };
		vertexBufferData4.pSysMem = cubeVertices4;
		vertexBufferData4.SysMemPitch = 0;
		vertexBufferData4.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc4(sizeof(cubeVertices4), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc4,
				&vertexBufferData4,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();


		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 5];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices5[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData5 = { 0 };
		vertexBufferData5.pSysMem = cubeVertices5;
		vertexBufferData5.SysMemPitch = 0;
		vertexBufferData5.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc5(sizeof(cubeVertices5), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc5,
				&vertexBufferData5,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();


		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 6];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices6[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData6 = { 0 };
		vertexBufferData6.pSysMem = cubeVertices6;
		vertexBufferData6.SysMemPitch = 0;
		vertexBufferData6.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc6(sizeof(cubeVertices6), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc6,
				&vertexBufferData6,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();


		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 7];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices7[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData7 = { 0 };
		vertexBufferData7.pSysMem = cubeVertices7;
		vertexBufferData7.SysMemPitch = 0;
		vertexBufferData7.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc7(sizeof(cubeVertices7), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc7,
				&vertexBufferData7,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();


		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 8];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices8[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData8 = { 0 };
		vertexBufferData8.pSysMem = cubeVertices8;
		vertexBufferData8.SysMemPitch = 0;
		vertexBufferData8.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc8(sizeof(cubeVertices8), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc8,
				&vertexBufferData8,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();


		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 9];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices9[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData9 = { 0 };
		vertexBufferData9.pSysMem = cubeVertices9;
		vertexBufferData9.SysMemPitch = 0;
		vertexBufferData9.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc9(sizeof(cubeVertices9), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc9,
				&vertexBufferData9,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();


		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 10];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices10[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData10 = { 0 };
		vertexBufferData10.pSysMem = cubeVertices10;
		vertexBufferData10.SysMemPitch = 0;
		vertexBufferData10.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc10(sizeof(cubeVertices10), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc10,
				&vertexBufferData10,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();


		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 11];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices11[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData11 = { 0 };
		vertexBufferData11.pSysMem = cubeVertices11;
		vertexBufferData11.SysMemPitch = 0;
		vertexBufferData11.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc11(sizeof(cubeVertices11), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc11,
				&vertexBufferData11,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);

		f_KEY = new Cube::CubeKEY();

		f_Bicycle = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_offset + 12];
		f_KEY->m_Link = f_Bicycle;

		static const VertexPositionColor cubeVertices12[] =
		{
			{ XMFLOAT3(f_Bicycle->vec_Vertex[0].m_X, f_Bicycle->vec_Vertex[0].m_Y, f_Bicycle->vec_Vertex[0].m_Z), XMFLOAT3(f_Bicycle->vec_Color[0].m_X, f_Bicycle->vec_Color[0].m_Y, f_Bicycle->vec_Color[0].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[1].m_X, f_Bicycle->vec_Vertex[1].m_Y, f_Bicycle->vec_Vertex[1].m_Z), XMFLOAT3(f_Bicycle->vec_Color[1].m_X, f_Bicycle->vec_Color[1].m_Y, f_Bicycle->vec_Color[1].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[2].m_X, f_Bicycle->vec_Vertex[2].m_Y, f_Bicycle->vec_Vertex[2].m_Z), XMFLOAT3(f_Bicycle->vec_Color[2].m_X, f_Bicycle->vec_Color[2].m_Y, f_Bicycle->vec_Color[2].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[3].m_X, f_Bicycle->vec_Vertex[3].m_Y, f_Bicycle->vec_Vertex[3].m_Z), XMFLOAT3(f_Bicycle->vec_Color[3].m_X, f_Bicycle->vec_Color[3].m_Y, f_Bicycle->vec_Color[3].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[4].m_X, f_Bicycle->vec_Vertex[4].m_Y, f_Bicycle->vec_Vertex[4].m_Z), XMFLOAT3(f_Bicycle->vec_Color[4].m_X, f_Bicycle->vec_Color[4].m_Y, f_Bicycle->vec_Color[4].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[5].m_X, f_Bicycle->vec_Vertex[5].m_Y, f_Bicycle->vec_Vertex[5].m_Z), XMFLOAT3(f_Bicycle->vec_Color[5].m_X, f_Bicycle->vec_Color[5].m_Y, f_Bicycle->vec_Color[5].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[6].m_X, f_Bicycle->vec_Vertex[6].m_Y, f_Bicycle->vec_Vertex[6].m_Z), XMFLOAT3(f_Bicycle->vec_Color[6].m_X, f_Bicycle->vec_Color[6].m_Y, f_Bicycle->vec_Color[6].m_Z) },
			{ XMFLOAT3(f_Bicycle->vec_Vertex[7].m_X, f_Bicycle->vec_Vertex[7].m_Y, f_Bicycle->vec_Vertex[7].m_Z), XMFLOAT3(f_Bicycle->vec_Color[7].m_X, f_Bicycle->vec_Color[7].m_Y, f_Bicycle->vec_Color[7].m_Z) },
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData12 = { 0 };
		vertexBufferData12.pSysMem = cubeVertices12;
		vertexBufferData12.SysMemPitch = 0;
		vertexBufferData12.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc12(sizeof(cubeVertices12), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc12,
				&vertexBufferData12,
				&f_KEY->m_Link->m_vertexBuffer
			)
		);

		Cube::g_BicycleContainer[0].vec_Key.push_back(f_KEY);
		}
	}
}

struct VertexBufferType
{
	XMFLOAT3 vertex[16];
};

void Sample3DSceneRenderer::acGen_Hesh(void)
{
	   /////////////
	  /////////////
	 // getWork
	//
	g_Schedular->getWork(ESL_PATTERN_LAST);

	for(int f_Int = 0; f_Int < 26; f_Int++)
		{
		Cube::CubeKEY* f_KEY = Cube::g_BicycleContainer[0].vec_Key[f_Int];

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiRand f_BiRand;
		Cube::BiVector f_Vector1(-0.5f);
		f_Vector1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector2(-0.5f, -0.5f, 0.5f);
		f_Vector2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector3(-0.5f, 0.5f, -0.5f);
		f_Vector3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector4(-0.5f, 0.5f, 0.5f);
		f_Vector4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector5(0.5f, -0.5f, -0.5f);
		f_Vector5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector6(0.5f, -0.5f, 0.5f);
		f_Vector6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector7(0.5f, 0.5f, -0.5f);
		f_Vector7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector8(0.5f, 0.5f, 0.5f);
		f_Vector8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::BiVector f_Color1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color8(Cube::random(), Cube::random(), Cube::random());

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 5), ((Cube::random() - 0.5) * 5), ((Cube::random() - 0.5) * 5)));

		f_Vector1.acRun_visualminer();
		f_Vector2.acRun_visualminer();
		f_Vector3.acRun_visualminer();
		f_Vector4.acRun_visualminer();
		f_Vector5.acRun_visualminer();
		f_Vector6.acRun_visualminer();
		f_Vector7.acRun_visualminer();
		f_Vector8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.clear();

		f_KEY->m_Link->vec_Vertex.push_back(f_Vector1);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector2);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector3);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector4);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector5);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector6);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector7);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector8);

		f_KEY->m_Link->vec_Color.clear();

		f_KEY->m_Link->vec_Color.push_back(f_Color1);
		f_KEY->m_Link->vec_Color.push_back(f_Color2);
		f_KEY->m_Link->vec_Color.push_back(f_Color3);
		f_KEY->m_Link->vec_Color.push_back(f_Color4);
		f_KEY->m_Link->vec_Color.push_back(f_Color5);
		f_KEY->m_Link->vec_Color.push_back(f_Color6);
		f_KEY->m_Link->vec_Color.push_back(f_Color7);
		f_KEY->m_Link->vec_Color.push_back(f_Color8);
		}

	g_vec_target_Cube.clear();

	int f_PORTcntr = 0;
	int f_LimitSense = g_Target->m_Collection->m_vec_Element.size() + ((Cube::g_BicycleContainer[0].vec_Key.size() - g_Target->m_Collection->m_vec_Element.size()) * Cube::random());

	while(g_vec_target_Cube.size() < f_LimitSense)
		{
		Cube::g_Fingat = Cube::randomFac() % CUBE_TRI_SHOT_TOTAL;

		if(Cube::g_Fingat > Cube::g_BiStyle->m_AdIndexPercent)
			{
			if(g_VisualMiningMode == OK_MODE_MINE)
				{
				if(Cube::g_BicycleContainer[0].vec_Key.size() > 0)
					{
					int f_Finger = Cube::randomFac() % Cube::g_BicycleContainer[0].vec_Key.size();
					g_vec_target_Cube.push_back(f_Finger);
					}
				}
			}

		f_PORTcntr++;
		}

	  ////////////
	 // HESH   //
	// Target //
	Cube::CubeHESH* f_Hesh = Cube::g_BicycleContainer[0].getHesh();

	classTarget* f_Target = new classTarget(f_Hesh);

	g_Result = g_Target->acCompare(f_Target, 50.95f, 850.0f, true);

	if(g_Result)
		{
		g_TargetedHesh = f_Hesh;
		}
}

std::string Sample3DSceneRenderer::acGen_Hash(Cube::CubeHASH *f_Hash)
{
	//string f_NonceHEX = util
	std::string hex_str;
	std::string f_Result;
	std::string f_Input;// = f_HashdataTrn + f_PreviousBlock;// +f_NonceHEX;
	m_hasha.init(); //reset hasher state
	m_hasha.process(f_Input.begin(), f_Input.end());
	m_hasha.finish();
	picosha2::get_hash_hex_string(m_hasha, hex_str);

	m_hasha.init(); //reset hasher state
	m_hasha.process(f_Input.begin(), f_Input.end());
	m_hasha.finish();
	picosha2::get_hash_hex_string(m_hasha, hex_str);

	printf("Hash Function Input1 %s", "CoinDesk rocks!!!!!");
	printf("%s", hex_str.c_str());
	printf("Hash Function Input2 %s", "CoinDesk rocks!!!!!!");
	printf("%s", hex_str.c_str());

	return hex_str;
}

void Sample3DSceneRenderer::acGen_Hesha(void)
{
	Cube::g_Count_Muti.ac_Fire();
	Cube::g_Other_Muti.ac_Fire();
	Cube::g_Share_Muti.ac_Fire();

	g_cube_Bit_container->acRefresh();

	Cube::CubeHASH* f_Hash = new Cube::CubeHASH();

	   /////////////
	  /////////////
	 // getWork
	//
	g_Schedular->getWork(ESL_PATTERN_LAST);

	for(int f_Int = 0; f_Int < g_Target->m_Collection->m_vec_Element.size(); f_Int++)
		{
		Cube::CubeKEY* f_KEY = Cube::g_BicycleContainer[0].vec_Key[f_Int];

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiRand f_BiRand;
		Cube::BiVector f_Vector1(-0.5f);
		f_Vector1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector2(-0.5f, -0.5f, 0.5f);
		f_Vector2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector3(-0.5f, 0.5f, -0.5f);
		f_Vector3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector4(-0.5f, 0.5f, 0.5f);
		f_Vector4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector5(0.5f, -0.5f, -0.5f);
		f_Vector5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector6(0.5f, -0.5f, 0.5f);
		f_Vector6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector7(0.5f, 0.5f, -0.5f);
		f_Vector7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector8(0.5f, 0.5f, 0.5f);
		f_Vector8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::BiVector f_Color1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color8(Cube::random(), Cube::random(), Cube::random());

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 5), ((Cube::random() - 0.5) * 5), ((Cube::random() - 0.5) * 5)));

		f_Vector1.acRun_visualminer();
		f_Vector2.acRun_visualminer();
		f_Vector3.acRun_visualminer();
		f_Vector4.acRun_visualminer();
		f_Vector5.acRun_visualminer();
		f_Vector6.acRun_visualminer();
		f_Vector7.acRun_visualminer();
		f_Vector8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.clear();

		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector1.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector1.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector1.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector2.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector2.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector2.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector3.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector3.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector3.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector4.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector4.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector4.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector5.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector5.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector5.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector6.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector6.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector6.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector7.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector7.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector7.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector8.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector8.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Vector8.m_Z * 1000));

		f_KEY->m_Link->vec_Vertex.push_back(f_Vector1);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector2);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector3);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector4);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector5);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector6);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector7);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector8);

		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color1.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color1.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color1.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color2.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color2.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color2.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color3.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color3.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color3.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color4.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color4.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color4.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color5.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color5.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color5.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color6.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color6.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color6.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color7.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color7.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color7.m_Z * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color8.m_X * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color8.m_Y * 1000));
		f_Hash->m_vec_Input.push_back((unsigned int)(f_Color8.m_Z * 1000));

		f_KEY->m_Link->vec_Color.clear();

		f_KEY->m_Link->vec_Color.push_back(f_Color1);
		f_KEY->m_Link->vec_Color.push_back(f_Color2);
		f_KEY->m_Link->vec_Color.push_back(f_Color3);
		f_KEY->m_Link->vec_Color.push_back(f_Color4);
		f_KEY->m_Link->vec_Color.push_back(f_Color5);
		f_KEY->m_Link->vec_Color.push_back(f_Color6);
		f_KEY->m_Link->vec_Color.push_back(f_Color7);
		f_KEY->m_Link->vec_Color.push_back(f_Color8);
		}

	g_vec_target_Cube.clear();

	int f_PORTcntr = 0;
	int f_LimitSense = g_Target->m_Collection->m_vec_Element.size() + ((Cube::g_BicycleContainer[0].vec_Key.size() - g_Target->m_Collection->m_vec_Element.size()) * Cube::random());

	while(g_vec_target_Cube.size() < f_LimitSense)
		{
		Cube::g_Fingat = Cube::randomFac() % CUBE_TRI_SHOT_TOTAL;

		if(Cube::g_Fingat > Cube::g_BiStyle->m_AdIndexPercent)
			{
#if 0
			if(g_VisualMiningMode == OK_MODE_MINE)
				{
#endif
				if(Cube::g_BicycleContainer[0].vec_Key.size() > 0)
					{
					int f_Finger = Cube::randomFac() % Cube::g_BicycleContainer[0].vec_Key.size();
					g_vec_target_Cube.push_back(f_Finger);
					f_Hash->m_vec_Input.push_back((unsigned int)f_Finger);
					}
#if 0
				}
#endif
			}

		f_PORTcntr++;
		}

	Cube::BiVector f_ColorBCK(Cube::random(), Cube::random(), Cube::random());

	f_Hash->m_vec_Input.push_back((unsigned int)(f_ColorBCK.m_X * 1000));
	f_Hash->m_vec_Input.push_back((unsigned int)(f_ColorBCK.m_Y * 1000));
	f_Hash->m_vec_Input.push_back((unsigned int)(f_ColorBCK.m_Z * 1000));

		////////////
	   //
	  // Hash
	 //
	int f_ShareID = 1;
	int f_BlockID = 1;
	int f_JobID = 1;	//Fixme pass out jobid
	int f_EslID = 1;
	int f_TargetID = 1;
	float f_Difficulty = 13.5; //Target Mark Threshold

	  ////////////
	 // HESH   //
	// Target //
	Cube::CubeHESH* f_Hesh = Cube::g_BicycleContainer[0].getHesh();

	f_Hesh->m_Hash = f_Hash;

	f_Hesh->m_bckred = f_ColorBCK.m_X;
	f_Hesh->m_bckgreen = f_ColorBCK.m_Y;
	f_Hesh->m_bckblue = f_ColorBCK.m_Z;

	classTarget* f_Target = new classTarget(f_Hesh);

	g_Result = g_Target->acCompare(f_Target, 15.95f, f_Difficulty, true);
										//15.95

	/*while(g_MutiLock)
		{
		//WAIT
		}*/

	g_ShrMutiLock = true;

	if(g_Result)
		{
#if 0
		printf("HeshExt: %s\n", ((std::string)EncodeBase58((const std::vector<picosha2::byte_t, std::allocator<picosha2::byte_t>> &)g_Hesh_Extention)).c_str());
#endif

		char* g_sharevalue[60];

		for(int f_Count = 0; f_Count < 60; f_Count++)
			{
			g_sharevalue[f_Count] = (char*)malloc(sizeof(char) * 1024);
			}

		sprintf(g_sharevalue[0], "%f", f_Target->m_Mark);
		sprintf(g_sharevalue[1], "%s", "unknown");
		sprintf(g_sharevalue[2], "%s", "3");
		sprintf(g_sharevalue[4], "%i", f_EslID);
		sprintf(g_sharevalue[5], "%i", f_TargetID);
		sprintf(g_sharevalue[6], "%s", "jsd8932o39aeru");
		sprintf(g_sharevalue[7], "%f", f_Difficulty);

		    ////////////
		   //
		  // Hash
		 //
		char *lx = (char*)malloc(f_Hash->m_vec_Input.size() * 8 * sizeof(char));
		ag_bin2hex(lx, f_Hash->m_vec_Input, f_Hash->m_vec_Input.size());
		std::string f_InputHash(lx);

		// check GenesisBlock (assume)
		std::string f_previousblock = "00000000000000000000000000000001";
		printf("ECN::Hesh...Checking Previous BlockHesh\n");

#if 0
		Cube::g_Count_Muti.ac_Fire();
		Cube::g_Other_Muti.ac_Fire();
		Cube::g_Share_Muti.ac_Fire();
#endif

		char blokquery[1000];
		sprintf(blokquery, "SELECT id, jobid, blockledger, dated FROM block ORDER BY dated DESC LIMIT 1");
		g_SafeL->acSelectCommand(blokquery, 2, false);
		int f_blokLCount = g_Entries[ag_GetEntry(g_Entries.size() - 1)].size();

		printf("ECN::Stat::: f_TrxLCount %i\n", f_blokLCount);

		g_JobID = 1;

		//assume remote connection for windows
		if(f_blokLCount > 0)
			{
			g_JobID = atoi(g_Entries[ag_GetEntry(g_Entries.size() - 1)][0][1].c_str());
			g_JobID++;
			f_previousblock = g_Entries[ag_GetEntry(g_Entries.size() - 1)][0][2];
			}

		f_JobID = g_JobID;

		sprintf(g_sharevalue[3], "%i", f_JobID);

		printf("ECN::Stat::: g_JobID %i\n", g_JobID);

		std::string f_tx = ag_GatherTransactions(g_JobID);

		std::string f_job = f_tx + f_previousblock;

		f_InputHash += f_job;

		//hey you could add a nonce here

		g_hasha.init(); //reset hasher state
		g_hasha.process(f_InputHash.begin(), f_InputHash.end());
		g_hasha.finish();
		picosha2::get_hash_hex_string(g_hasha, f_Hash->m_OutputHash);

		f_Hesh->m_Hash = f_Hash;

		printf("ShareHash-%s\n", f_Hash->m_OutputHash.c_str());

		sprintf(g_sharevalue[8], "%i", 0);
		sprintf(g_sharevalue[9], "%i", 10);
		sprintf(g_sharevalue[10], "%s", f_Hash->m_OutputHash.c_str());
		sprintf(g_sharevalue[11], "%s", f_Hash->m_OutputHash.c_str());
		sprintf(g_sharevalue[12], "%s", g_Wallet->GetAdr().c_str());
		sprintf(g_sharevalue[13], "%f", f_Hesh->m_bckred);
		sprintf(g_sharevalue[14], "%f", f_Hesh->m_bckgreen);
		sprintf(g_sharevalue[15], "%f", f_Hesh->m_bckblue);
		sprintf(g_sharevalue[16], "%f", 0.0f);

		printf("ECN::Hesh...Sending BlockHesh\n");

		char shlquery[1000];
		sprintf(shlquery, "SELECT id FROM share WHERE shareledger = '%s'", f_Hash->m_OutputHash.c_str());// WHERE name = '%s' AND pass = MD5('%s')", i_name, i_password);
																			//Run the query
		g_SafeL->acSelectCommand(shlquery, 2, false);
		int f_ShareLCount = g_Entries[ag_GetEntry(g_Entries.size() - 1)].size();

		printf("ECN::Stat::: f_ShareLCount %i\n", f_ShareLCount);

		if(f_ShareLCount == 0)
			{
			int f_ShareUniqueID = g_SafeL->Insert("share", (const char **)g_sharefield, (const char **)g_sharevalue, 17, 2, false);

			f_ShareID = f_ShareUniqueID;
			g_ShareID = f_ShareID;

			printf("ECN::Stat::: f_ShareID %i f_shareuniqueID %i\n", f_ShareID, f_ShareUniqueID);

			ag_addGenesisTracking(f_ShareUniqueID);
				
			char* f_idquery2 = new char[150];
			sprintf(f_idquery2, "SELECT id, owner FROM share WHERE jobid = %i", g_JobID);
			g_SafeL->acSelectCommand(f_idquery2, 2, false);
			int f_PeerCount = g_Entries[ag_GetEntry(g_Entries.size() - 1)].size();

			int f_Index = 0;
			ShareContainer* f_ShareContainer = new ShareContainer();

			int f_PEERCNT = 1;
			int f_SHARECNT = 1;

			for(int f_Int = 0; f_Int < f_PeerCount; f_Int++)
				{
				std::string f_peer = g_Entries[ag_GetEntry(g_Entries.size() - 1)][f_Int][1];

				bool f_Found = false;
				for(int f_IntSearch = 0; f_IntSearch < f_ShareContainer->m_Peer.size(); f_IntSearch++)
					{
					if(f_peer.compare(f_ShareContainer->m_Peer[f_IntSearch]->m_Owner) == 0)
						{
						f_Found = true;
						f_Index = f_IntSearch;
						}
					}

				if(!f_Found)
					{
					Cube::CubePeer* f_CPeer = new Cube::CubePeer();
					f_CPeer->m_Owner = g_Entries[ag_GetEntry(g_Entries.size() - 1)][f_Int][1];
					Cube::CubeShare* f_CShare = new Cube::CubeShare();
					f_CShare->m_Owner = g_Entries[ag_GetEntry(g_Entries.size() - 1)][f_Int][1];
					f_CShare->m_ID = atoi(g_Entries[ag_GetEntry(g_Entries.size() - 1)][f_Int][0].c_str());
					f_CPeer->m_Share.push_back(f_CShare);
					f_ShareContainer->m_Peer.push_back(f_CPeer);

					if(g_ShareID == atoi(g_Entries[ag_GetEntry(g_Entries.size() - 1)][f_Int][0].c_str()))
						{
						f_PEERCNT = f_ShareContainer->m_Peer.size() - 1;
						f_SHARECNT = 1;
						}
					}
				else
					{
					Cube::CubeShare* f_CShare = new Cube::CubeShare();
					f_CShare->m_Owner = g_Entries[ag_GetEntry(g_Entries.size() - 1)][f_Int][1];
					f_CShare->m_ID = atoi(g_Entries[ag_GetEntry(g_Entries.size() - 1)][f_Int][0].c_str());
					f_ShareContainer->m_Peer[f_Index]->m_Share.push_back(f_CShare);

					if(g_ShareID == atoi(g_Entries[ag_GetEntry(g_Entries.size() - 1)][f_Int][0].c_str()))
						{
						f_PEERCNT = f_Index;
						f_SHARECNT = f_ShareContainer->m_Peer[f_Index]->m_Share.size() - 1;
						}
					}
				}

			g_PEERCNT = f_PEERCNT;
			g_SHARECNT = f_SHARECNT;

#if 1
			Ackent* f_Ack = new Ackent(ACK_TYPE_SHARE, g_ShareID, f_PEERCNT);
			f_Ack->m_ECN = 0.0f;
			f_Ack->m_Origin = 1;
			g_Ackent.push_back(f_Ack);
#endif

			g_PerShareCount++;

			char* g_heshfield[60];
			char* g_heshvalue[60];

			for(int f_Count = 0; f_Count < 60; f_Count++)
				{
				g_heshfield[f_Count] = (char*)malloc(sizeof(char) * 100);
				g_heshvalue[f_Count] = (char*)malloc(sizeof(char) * 100);
				}

			g_HeshFieldCount = 10;

			sprintf(g_heshfield[g_HeshFieldCount - 10], "mark");
			sprintf(g_heshfield[g_HeshFieldCount - 9], "assetid");
			sprintf(g_heshfield[g_HeshFieldCount - 8], "assetofficeid");
			sprintf(g_heshfield[g_HeshFieldCount - 7], "blockid");
			sprintf(g_heshfield[g_HeshFieldCount - 6], "shareid");
			sprintf(g_heshfield[g_HeshFieldCount - 5], "jobid");
			sprintf(g_heshfield[g_HeshFieldCount - 4], "eslid");
			sprintf(g_heshfield[g_HeshFieldCount - 3], "targetid");
			sprintf(g_heshfield[g_HeshFieldCount - 2], "eslcode");
			sprintf(g_heshfield[g_HeshFieldCount - 1], "difficulty");
			
			sprintf(g_heshvalue[g_HeshFieldCount - 10], "%f", f_Target->m_Mark);
			sprintf(g_heshvalue[g_HeshFieldCount - 9], "unknown");
			sprintf(g_heshvalue[g_HeshFieldCount - 8], "3");
			sprintf(g_heshvalue[g_HeshFieldCount - 7], "%i", f_BlockID);
			sprintf(g_heshvalue[g_HeshFieldCount - 6], "%i", f_ShareID);
			sprintf(g_heshvalue[g_HeshFieldCount - 5], "%i", f_JobID);
			sprintf(g_heshvalue[g_HeshFieldCount - 4], "%i", f_EslID);
			sprintf(g_heshvalue[g_HeshFieldCount - 3], "%i", 0);
			sprintf(g_heshvalue[g_HeshFieldCount - 2], "%s", "jsd8932o39aeru");
			sprintf(g_heshvalue[g_HeshFieldCount - 1], "%f", f_Difficulty);

			int f_heshID = g_SafeL->Insert("hesh", (const char **)g_heshfield, (const char **)g_heshvalue, g_HeshFieldCount, 2, false);

			for(int f_Int = 0; f_Int < g_vec_target_Cube.size(); f_Int++)
				{
				Cube::CubeKEY* f_KEY = Cube::g_BicycleContainer[0].vec_Key[f_Int];
				char* g_cubevalue[60];

				for(int f_Count = 0; f_Count < 60; f_Count++)
					{
					g_cubevalue[f_Count] = (char*)malloc(sizeof(char) * 100);
					}

				for(int f_Int2 = 0; f_Int2 < 8; f_Int2++)
					{
					sprintf(g_cubevalue[f_Int2 * 3], "%f", f_KEY->m_Link->vec_Vertex[f_Int2].m_X);
					sprintf(g_cubevalue[(f_Int2 * 3) + 1], "%f", f_KEY->m_Link->vec_Vertex[f_Int2].m_Y);
					sprintf(g_cubevalue[(f_Int2 * 3) + 2], "%f", f_KEY->m_Link->vec_Vertex[f_Int2].m_Z);
					}

				for(int f_Int3 = 0; f_Int3 < 8; f_Int3++)
					{
					sprintf(g_cubevalue[(24 + (f_Int3 * 3))], "%f", f_KEY->m_Link->vec_Color[f_Int3].m_X);
					sprintf(g_cubevalue[(24 + (f_Int3 * 3)) + 1], "%f", f_KEY->m_Link->vec_Color[f_Int3].m_Y);
					sprintf(g_cubevalue[(24 + (f_Int3 * 3)) + 2], "%f", f_KEY->m_Link->vec_Color[f_Int3].m_Z);
					}
				
				sprintf(g_cubevalue[48], "%i", f_BlockID);
				sprintf(g_cubevalue[49], "%i", f_ShareID);
				sprintf(g_cubevalue[50], "%i", f_JobID);
				sprintf(g_cubevalue[51], "%i", f_EslID);
				sprintf(g_cubevalue[52], "%i", f_TargetID);
				sprintf(g_cubevalue[53], "%s", "jsd8932o39aeru");
				sprintf(g_cubevalue[54], "%f", f_Difficulty);

				g_CubeFieldCount = 55;

				int f_adIndex = g_SafeL->Insert("cube", (const char **)g_cubefield, (const char **)g_cubevalue, g_CubeFieldCount, 2, false);

				char* g_adivalue[60];

				for(int f_Count = 0; f_Count < 3; f_Count++)
					{
					g_adivalue[f_Count] = (char*)malloc(sizeof(char) * 100);
					}

				sprintf(g_adivalue[0], "%i", f_adIndex);
				sprintf(g_adivalue[1], "%i", g_vec_target_Cube[f_Int]);
				sprintf(g_adivalue[2], "%i", f_heshID);

				g_SafeL->Insert("adindex", (const char **)g_adifield, (const char **)g_adivalue, 3, 2, false);
				}

			printf("ECN::Share Submitted.\n");

#if 0
			if(((g_Viewing == 1) || (g_RenderHesh == true)) || 1)
				{
				g_Result = false;
				}
#else
			g_Result = false;
#endif
			}
		else
			{
			printf("Duplicate Block Found.\n");
			g_Result = false;
			}

#if 0
		Cube::g_Count_Muti.ac_Free();
		Cube::g_Other_Muti.ac_Free();
		Cube::g_Share_Muti.ac_Free();
#endif
		}

	g_ShrMutiLock = false;

	g_Hesh_Extention.clear();

	Cube::g_Count_Muti.ac_Free();
	Cube::g_Other_Muti.ac_Free();
	Cube::g_Share_Muti.ac_Free();
}

void Sample3DSceneRenderer::acLoad_Hesha(string f_jobid, int f_Skip)
{
	/*g_cube_Bit_container->acRefresh();

	//Build the query
	char query[1000];
	unsigned int acc_id = 0;
	sprintf(query, "SELECT * FROM share WHERE jobid = %i", f_jobid);// WHERE name = '%s' AND pass = MD5('%s')", i_name, i_password);

	//Run the query
	g_SafeL->acSelectCommand(query);

	int f_Shareid = 1;// atoi(g_Entries[ag_GetEntry(g_Entries.size() - 1)][f_Int][0]);

	int f_Count = 0;
	while(row.IsValid())
		{
		if(f_Count == f_Skip)
			{
			//Build the query
			char hquery[1000];
			unsigned int acc_id = 0;
			sprintf(hquery, "SELECT * FROM hesh WHERE shareid = %i", f_Shareid);// WHERE name = '%s' AND pass = MD5('%s')", i_name, i_password);
																					//Run the query
			g_SafeL->acSelectCommand(hquery);

			int f_heshid = 1;// atoi(hrow[0]);

#pragma message("WARNING Poll for share width")
			for(int f_Int = 0; f_Int < g_Target->m_Collection->m_vec_Element.size(); f_Int++)
				{
				Cube::CubeKEY* f_KEY = Cube::g_BicycleContainer[0].vec_Key[f_Int];

				//Build the query
				char cquery[1000];
				unsigned int acc_id = 0;
				sprintf(cquery, "SELECT * FROM cube WHERE id = %i", f_heshid);// WHERE name = '%s' AND pass = MD5('%s')", i_name, i_password);
																							//Run the query
				g_SafeL->acSelectCommand(cquery);*/

				// Load mesh vertices. Each vertex has a position and a color.
				/*Cube::BiVector f_Vector1(atof(crow[1]), atof(crow[2]), atof(crow[3]));
				Cube::BiVector f_Vector2(atof(crow[4]), atof(crow[5]), atof(crow[6]));
				Cube::BiVector f_Vector3(atof(crow[7]), atof(crow[8]), atof(crow[9]));
				Cube::BiVector f_Vector4(atof(crow[10]), atof(crow[11]), atof(crow[12]));
				Cube::BiVector f_Vector5(atof(crow[13]), atof(crow[14]), atof(crow[15]));
				Cube::BiVector f_Vector6(atof(crow[16]), atof(crow[17]), atof(crow[18]));
				Cube::BiVector f_Vector7(atof(crow[19]), atof(crow[20]), atof(crow[21]));
				Cube::BiVector f_Vector8(atof(crow[22]), atof(crow[23]), atof(crow[24]));

				Cube::BiVector f_Color1(atof(crow[25]), atof(crow[26]), atof(crow[27]));
				Cube::BiVector f_Color2(atof(crow[28]), atof(crow[29]), atof(crow[30]));
				Cube::BiVector f_Color3(atof(crow[31]), atof(crow[32]), atof(crow[33]));
				Cube::BiVector f_Color4(atof(crow[34]), atof(crow[35]), atof(crow[36]));
				Cube::BiVector f_Color5(atof(crow[37]), atof(crow[38]), atof(crow[39]));
				Cube::BiVector f_Color6(atof(crow[40]), atof(crow[41]), atof(crow[42]));
				Cube::BiVector f_Color7(atof(crow[43]), atof(crow[44]), atof(crow[45]));
				Cube::BiVector f_Color8(atof(crow[46]), atof(crow[47]), atof(crow[48]));

				f_KEY->m_Link->vec_Vertex.clear();

				f_KEY->m_Link->vec_Vertex.push_back(f_Vector1);
				f_KEY->m_Link->vec_Vertex.push_back(f_Vector2);
				f_KEY->m_Link->vec_Vertex.push_back(f_Vector3);
				f_KEY->m_Link->vec_Vertex.push_back(f_Vector4);
				f_KEY->m_Link->vec_Vertex.push_back(f_Vector5);
				f_KEY->m_Link->vec_Vertex.push_back(f_Vector6);
				f_KEY->m_Link->vec_Vertex.push_back(f_Vector7);
				f_KEY->m_Link->vec_Vertex.push_back(f_Vector8);

				f_KEY->m_Link->vec_Color.clear();

				f_KEY->m_Link->vec_Color.push_back(f_Color1);
				f_KEY->m_Link->vec_Color.push_back(f_Color2);
				f_KEY->m_Link->vec_Color.push_back(f_Color3);
				f_KEY->m_Link->vec_Color.push_back(f_Color4);
				f_KEY->m_Link->vec_Color.push_back(f_Color5);
				f_KEY->m_Link->vec_Color.push_back(f_Color6);
				f_KEY->m_Link->vec_Color.push_back(f_Color7);
				f_KEY->m_Link->vec_Color.push_back(f_Color8);
				}

			g_vec_target_Cube.clear();

			int f_LimitSense = Cube::g_BicycleContainer[0].vec_Key.size();
			int f_cubecount = 1;
			while(g_vec_target_Cube.size() < f_LimitSense)
				{
				g_vec_target_Cube.push_back(atoi(hrow[f_cubecount]));
				f_cubecount++;
				}

			Cube::CubeHESH* f_Hesh = Cube::g_BicycleContainer[0].getHesh();
			g_TargetedHesh = f_Hesh;
			}

		f_Count++;
		row = result->NextRecord();
		}*/
}

void Sample3DSceneRenderer::MapBuffers(void)
{
	   /////////////
	  /////////////
	 // getWork
	//
	g_Schedular->getWork(ESL_PATTERN_LAST);

	for(int f_Int = 0; f_Int < g_Target->m_Collection->m_vec_Element.size(); f_Int++)
		{
		Cube::CubeKEY* f_KEY = Cube::g_BicycleContainer[0].vec_Key[f_Int];

		// Load mesh vertices. Each vertex has a position and a color.
		Cube::BiRand f_BiRand;
		Cube::BiVector f_Vector1(-0.5f);
		f_Vector1.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector2(-0.5f, -0.5f, 0.5f);
		f_Vector2.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector3(-0.5f, 0.5f, -0.5f);
		f_Vector3.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector4(-0.5f, 0.5f, 0.5f);
		f_Vector4.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector5(0.5f, -0.5f, -0.5f);
		f_Vector5.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector6(0.5f, -0.5f, 0.5f);
		f_Vector6.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector7(0.5f, 0.5f, -0.5f);
		f_Vector7.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);
		f_BiRand.acGet();
		Cube::BiVector f_Vector8(0.5f, 0.5f, 0.5f);
		f_Vector8.acPushRandSphere(Cube::g_swit, f_BiRand.m_Seed);

		Cube::BiVector f_Color1(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color2(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color3(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color4(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color5(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color6(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color7(Cube::random(), Cube::random(), Cube::random());
		Cube::BiVector f_Color8(Cube::random(), Cube::random(), Cube::random());

		Cube::agSet_offsetvec(Cube::float3(((Cube::random() - 0.5) * 5), ((Cube::random() - 0.5) * 5), ((Cube::random() - 0.5) * 5)));

		f_Vector1.acRun_visualminer();
		f_Vector2.acRun_visualminer();
		f_Vector3.acRun_visualminer();
		f_Vector4.acRun_visualminer();
		f_Vector5.acRun_visualminer();
		f_Vector6.acRun_visualminer();
		f_Vector7.acRun_visualminer();
		f_Vector8.acRun_visualminer();

		f_KEY->m_Link->vec_Vertex.clear();

		f_KEY->m_Link->vec_Vertex.push_back(f_Vector1);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector2);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector3);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector4);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector5);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector6);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector7);
		f_KEY->m_Link->vec_Vertex.push_back(f_Vector8);

		f_KEY->m_Link->vec_Color.clear();

		f_KEY->m_Link->vec_Color.push_back(f_Color1);
		f_KEY->m_Link->vec_Color.push_back(f_Color2);
		f_KEY->m_Link->vec_Color.push_back(f_Color3);
		f_KEY->m_Link->vec_Color.push_back(f_Color4);
		f_KEY->m_Link->vec_Color.push_back(f_Color5);
		f_KEY->m_Link->vec_Color.push_back(f_Color6);
		f_KEY->m_Link->vec_Color.push_back(f_Color7);
		f_KEY->m_Link->vec_Color.push_back(f_Color8);
		}

	for(int f_Int = 0; f_Int < Cube::g_BicycleContainer[0].vec_Key.size(); f_Int++)
		{
		Cube::CubeKEY* f_KEY = Cube::g_BicycleContainer[0].vec_Key[f_Int];

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		VertexBufferType* dataPtr;

		// Lock the constant buffer so it can be written to.
		HRESULT result = m_deviceResources->GetD3DDeviceContext()->Map(f_KEY->m_Link->m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
			{
			throw std::exception("GetD3DDeviceContext()->Map buffer map failed");
			}

		// Get a pointer to the data in the constant buffer.
		dataPtr = (VertexBufferType*)mappedResource.pData;

		int f_VertexCount = 0;
		for (int f_Count = 0; f_Count < 16; f_Count+=2)
			{
			//Cube::float4 f_pos(dataPtr->vertex[f_Count].x, dataPtr->vertex[f_Count].y, dataPtr->vertex[f_Count].z, 1.0f);
			//f_pos = Cube::g_model * f_pos;

			dataPtr->vertex[f_Count].x = f_KEY->m_Link->vec_Vertex[f_VertexCount].m_X;
			dataPtr->vertex[f_Count].y = f_KEY->m_Link->vec_Vertex[f_VertexCount].m_Y;
			dataPtr->vertex[f_Count].z = f_KEY->m_Link->vec_Vertex[f_VertexCount].m_Z;

			f_VertexCount++;

			dataPtr->vertex[f_Count + 1].x = f_KEY->m_Link->vec_Color[f_VertexCount].m_X;
			dataPtr->vertex[f_Count + 1].y = f_KEY->m_Link->vec_Color[f_VertexCount].m_Y;
			dataPtr->vertex[f_Count + 1].z = f_KEY->m_Link->vec_Color[f_VertexCount].m_Z;
			}

		// Unlock the constant buffer.
		m_deviceResources->GetD3DDeviceContext()->Unmap(f_KEY->m_Link->m_vertexBuffer.Get(), 0);
		}
}

void Sample3DSceneRenderer::RestoreTargetedBuffers(void)
{
	if(g_TargetedHesh)
		{
		if(g_SelectedHeshID >= Cube::g_HESH.size())
			{
			g_SelectedHeshID = Cube::g_HESH.size() - 1;
			}

		for(int f_Int = 0; f_Int < g_TargetedHesh->vec_Bicycle.size(); f_Int++)
			{
			CubeBicycle* f_Bicycle = g_TargetedHesh->vec_Bicycle[f_Int];

			f_Bicycle->m_vertexBuffer = Cube::g_BicycleContainer[0].vec_Key[f_Int]->m_Link->m_vertexBuffer;
			f_Bicycle->m_indexBuffer = Cube::g_BicycleContainer[0].vec_Key[f_Int]->m_Link->m_indexBuffer;

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			VertexBufferType* dataPtr;

			// Lock the constant buffer so it can be written to.
			HRESULT result = m_deviceResources->GetD3DDeviceContext()->Map(f_Bicycle->m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
				{
				throw std::exception("GetD3DDeviceContext()->Map buffer map failed");
				}

			// Get a pointer to the data in the constant buffer.
			dataPtr = (VertexBufferType*)mappedResource.pData;

			int f_VertexCount = 0;
			for (int f_Count = 0; f_Count < 16; f_Count+=2)
				{
				dataPtr->vertex[f_Count].x = f_Bicycle->vec_Vertex[f_VertexCount].m_X;
				dataPtr->vertex[f_Count].y = f_Bicycle->vec_Vertex[f_VertexCount].m_Y;
				dataPtr->vertex[f_Count].z = f_Bicycle->vec_Vertex[f_VertexCount].m_Z;

				f_VertexCount++;

				dataPtr->vertex[f_Count + 1].x = f_Bicycle->vec_Color[f_VertexCount].m_X;
				dataPtr->vertex[f_Count + 1].y = f_Bicycle->vec_Color[f_VertexCount].m_Y;
				dataPtr->vertex[f_Count + 1].z = f_Bicycle->vec_Color[f_VertexCount].m_Z;
				}

			// Unlock the constant buffer.
			m_deviceResources->GetD3DDeviceContext()->Unmap(f_Bicycle->m_vertexBuffer.Get(), 0);
			}
		}
}

void Sample3DSceneRenderer::RestoreBuffers(void)
{
	if(g_SelectedHeshID >= Cube::g_HESH.size())
		{
		g_SelectedHeshID = Cube::g_HESH.size() - 1;
		}

	for(int f_Int = 0; f_Int < Cube::g_BicycleContainer[0].vec_Key.size(); f_Int++)
		{
		Cube::CubeKEY* f_KEY = Cube::g_BicycleContainer[0].vec_Key[f_Int];

		f_KEY->m_Link->vec_Vertex.clear();

		for(int f_CountVertex = 0; f_CountVertex < Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_Int]->vec_Vertex.size(); f_CountVertex++)
			{
			Cube::BiVector f_Vector = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_Int]->vec_Vertex[f_CountVertex];

			f_KEY->m_Link->vec_Vertex.push_back(f_Vector);
			}

		f_KEY->m_Link->vec_Color.clear();

		for(int f_CountColor = 0; f_CountColor < Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_Int]->vec_Color.size(); f_CountColor++)
			{
			Cube::BiVector f_Color = Cube::g_HESH[g_SelectedHeshID]->vec_Bicycle[f_Int]->vec_Color[f_CountColor];

			f_KEY->m_Link->vec_Color.push_back(f_Color);
			}

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		VertexBufferType* dataPtr;

		// Lock the constant buffer so it can be written to.
		HRESULT result = m_deviceResources->GetD3DDeviceContext()->Map(f_KEY->m_Link->m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
			{
			throw std::exception("GetD3DDeviceContext()->Map buffer map failed");
			}

		// Get a pointer to the data in the constant buffer.
		dataPtr = (VertexBufferType*)mappedResource.pData;

		int f_VertexCount = 0;
		for (int f_Count = 0; f_Count < 16; f_Count+=2)
			{
			dataPtr->vertex[f_Count].x = f_KEY->m_Link->vec_Vertex[f_VertexCount].m_X;
			dataPtr->vertex[f_Count].y = f_KEY->m_Link->vec_Vertex[f_VertexCount].m_Y;
			dataPtr->vertex[f_Count].z = f_KEY->m_Link->vec_Vertex[f_VertexCount].m_Z;

			f_VertexCount++;

			dataPtr->vertex[f_Count + 1].x = f_KEY->m_Link->vec_Color[f_VertexCount].m_X;
			dataPtr->vertex[f_Count + 1].y = f_KEY->m_Link->vec_Color[f_VertexCount].m_Y;
			dataPtr->vertex[f_Count + 1].z = f_KEY->m_Link->vec_Color[f_VertexCount].m_Z;
			}

		// Unlock the constant buffer.
		m_deviceResources->GetD3DDeviceContext()->Unmap(f_KEY->m_Link->m_vertexBuffer.Get(), 0);
		}
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}

bool Sample3DSceneRenderer::MapResources(Cube::CubeKEY* f_KEY)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexBufferType* dataPtr;

	// Lock the constant buffer so it can be written to.
	HRESULT result = m_deviceResources->GetD3DDeviceContext()->Map(f_KEY->m_Link->m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		{
		throw std::exception("GetD3DDeviceContext()->Map buffer map failed");
		}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (VertexBufferType*)mappedResource.pData;

	for(int f_Count = 0; f_Count < 16; f_Count++)
		{
		Cube::float4 f_pos(dataPtr->vertex[f_Count].x, dataPtr->vertex[f_Count].y, dataPtr->vertex[f_Count].z, 1.0f);

		f_pos = Cube::g_model * f_pos;

		dataPtr->vertex[f_Count].x = (Cube::random() - 0.5) * 10;
		dataPtr->vertex[f_Count].y = (Cube::random() - 0.5) * 10;
		dataPtr->vertex[f_Count].z = (Cube::random() - 0.5) * 10;
		}

	// Unlock the constant buffer.
	m_deviceResources->GetD3DDeviceContext()->Unmap(f_KEY->m_Link->m_vertexBuffer.Get(), 0);

#if 0
	// Finanly set the constant buffer in the vertex shader with the updated values.
	m_deviceResources->GetD3DDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
#endif

	return true;
}

void Sample3DSceneRenderer::Reorientate(Cube::CubeKEY* f_KEY)
{
	for (int f_Count = 0; f_Count < f_KEY->m_Link->vec_Vertex.size(); f_Count++)
		{
		f_KEY->m_Link->vec_Vertex[f_Count].acPrepare();
		}
}