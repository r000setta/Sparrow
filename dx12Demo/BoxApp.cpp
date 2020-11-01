#include "BoxApp.h"

bool BoxApp::Initialize()
{
    if (!D3DApp::Initialize())
        return false;

    // Reset the command list to prep for initialization commands.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

    BuildDescriptorHeaps();
    BuildConstantBuffers();
    BuildRootSignature();
    BuildShadersAndInputLayout();
    //BuildBoxGeometry();
    BuildGeometry();
    BuildPSO();

    // Execute the initialization commands.
    ThrowIfFailed(mCommandList->Close());
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until initialization is complete.
    FlushCommandQueue();

    return true;
}

void BoxApp::OnResize()
{
    D3DApp::OnResize();
    /*mScreenViewport.TopLeftX = 0;
    mScreenViewport.TopLeftY = 0;
    mScreenViewport.Width = static_cast<float>(mClientWidth);
    mScreenViewport.Height = static_cast<float>(mClientHeight);
    mScreenViewport.MaxDepth = 1.0f;
    mScreenViewport.MinDepth = 0.0f;

    mScissorRect.left = 0;
    mScissorRect.top = 0;*/
    //mScissorRect.right = mClientWidth / 2;
    //mScissorRect.bottom = mClientHeight / 2;

    // The window resized, so update the aspect ratio and recompute the projection matrix.
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&mProj, P);
}

void BoxApp::Update(const GameTimer& gt)
{
    // Convert Spherical to Cartesian coordinates.
    float x = mRadius * sinf(mPhi) * cosf(mTheta);
    float z = mRadius * sinf(mPhi) * sinf(mTheta);
    float y = mRadius * cosf(mPhi);

    // Build the view matrix.
    XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&mView, view);

    XMMATRIX world = XMLoadFloat4x4(&mWorld);
    XMMATRIX proj = XMLoadFloat4x4(&mProj);
    //XMMATRIX worldViewProj = world * view * proj;
    XMMATRIX viewProj = view * proj;
    XMMATRIX wvp = world* view* proj;
    // Update the constant buffer with the latest worldViewProj matrix.
    ObjectConstants objConstants;
    PassConstants passConstants;

    XMStoreFloat4x4(&passConstants.viewProj, XMMatrixTranspose(viewProj));
    mPassCB->CopyData(0, passConstants);

    objConstants.Time = gt.TotalTime();
    objConstants.PulseColor = XMFLOAT4(Colors::Red);

    //XMStoreFloat4x4(&objConstants.world, XMMatrixTranspose(world));
    XMStoreFloat4x4(&objConstants.world, XMMatrixTranspose(wvp));
    mObjectCB->CopyData(0, objConstants);
}

void BoxApp::Draw(const GameTimer& gt)
{
    // Reuse the memory associated with command recording.
    // We can only reset when the associated command lists have finished execution on the GPU.
    ThrowIfFailed(mDirectCmdListAlloc->Reset());

    // A command list can be reset after it has been added to the command queue via ExecuteCommandList.
    // Reusing the command list reuses memory.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO.Get()));
    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    // Indicate a state transition on the resource usage.
    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    // Clear the back buffer and depth buffer.
    mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
    mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    // Specify the buffers we are going to render to.
    mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

    ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
    mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

    mCommandList->IASetVertexBuffers(0, 1, &mGeo->VertexBufferView());
    //mCommandList->IASetVertexBuffers(0, 1, &mBoxGeo->GetVPosBuffeView());
    //mCommandList->IASetVertexBuffers(1, 1, &mBoxGeo->GetVColorBufferView());

    mCommandList->IASetIndexBuffer(&mGeo->IndexBufferView());
    mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    int objCbvIdx = 0;
    auto handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
    handle.Offset(objCbvIdx, mCbvSrvUavDescriptorSize);
    mCommandList->SetGraphicsRootDescriptorTable(0, handle);

    int passCbvIdx = 1;
    handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
    handle.Offset(passCbvIdx, mCbvSrvUavDescriptorSize);
    mCommandList->SetGraphicsRootDescriptorTable(1, handle);


    mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

    mCommandList->SetGraphicsRootDescriptorTable(1, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

    mCommandList->DrawIndexedInstanced(mGeo->DrawArgs["box"].IndexCount,
        1,
        mGeo->DrawArgs["box"].StartIndexLocation,
        mGeo->DrawArgs["box"].BaseVertexLocation,
        0);

    mCommandList->DrawIndexedInstanced(mGeo->DrawArgs["sphere"].IndexCount,
        1,
        mGeo->DrawArgs["sphere"].StartIndexLocation,
        mGeo->DrawArgs["sphere"].BaseVertexLocation,
        0);

    // Indicate a state transition on the resource usage.
    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));


    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //bool show_demo = true;
    //if (show_demo)
    //    ImGui::ShowDemoWindow(&show_demo);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static int counter = 0;
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        //ImGui::ColorEdit4("color 2", mColor);
        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &mPhi, 0.1f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    mCommandList->SetDescriptorHeaps(1, mSrvHeap.GetAddressOf());
    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), mCommandList.Get());
    // Done recording commands.
    ThrowIfFailed(mCommandList->Close());

    // Add the command list to the queue for execution.
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // swap the back and front buffers
    ThrowIfFailed(mSwapChain->Present(0, 0));
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

    // Wait until frame commands are complete.  This waiting is inefficient and is
    // done for simplicity.  Later we will show how to organize our rendering code
    // so we do not have to wait per frame.
    FlushCommandQueue();
}

void BoxApp::OnMouseDown(WPARAM btnState, int x, int y)
{
    mLastMousePos.x = x;
    mLastMousePos.y = y;

    SetCapture(mhMainWnd);
}

void BoxApp::OnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void BoxApp::OnMouseMove(WPARAM btnState, int x, int y)
{
    if ((btnState & MK_LBUTTON) != 0)
    {
        // Make each pixel correspond to a quarter of a degree.
        float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
        float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

        // Update angles based on input to orbit camera around box.


        // Restrict the angle mPhi.
        mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
    }
    else if ((btnState & MK_RBUTTON) != 0)
    {
        // Make each pixel correspond to 0.005 unit in the scene.
        float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
        float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

        // Update the camera radius based on input.
        mRadius += dx - dy;

        // Restrict the radius.
        mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
    }

    mLastMousePos.x = x;
    mLastMousePos.y = y;
}

void BoxApp::BuildDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
    cbvHeapDesc.NumDescriptors = 2;
    cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvHeapDesc.NodeMask = 0;
    ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc,
        IID_PPV_ARGS(&mCbvHeap)));
}

void BoxApp::BuildConstantBuffers()
{
    mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice.Get(), 1, true);

    UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

    D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
    // Offset to the ith object constant buffer in the buffer.
    int boxCBufIndex = 0;
    cbAddress += boxCBufIndex * objCBByteSize;

    int heapIdx = 0;
    auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mCbvHeap->GetCPUDescriptorHandleForHeapStart());
    handle.Offset(heapIdx, mCbvSrvUavDescriptorSize);

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
    cbvDesc.BufferLocation = cbAddress;
    cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

    /*   md3dDevice->CreateConstantBufferView(
           &cbvDesc,
           mCbvHeap->GetCPUDescriptorHandleForHeapStart());*/

    md3dDevice->CreateConstantBufferView(
        &cbvDesc,
        handle);

    heapIdx = 1;

    mPassCB = std::make_unique<UploadBuffer<PassConstants>>(md3dDevice.Get(), 1, true);

    UINT passCBBytesSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));
    D3D12_GPU_VIRTUAL_ADDRESS passCBAddress = mPassCB->Resource()->GetGPUVirtualAddress();
    int passCbElementIdx = 0;
    passCBAddress += passCbElementIdx * passCBBytesSize;

    handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mCbvHeap->GetCPUDescriptorHandleForHeapStart());
    handle.Offset(heapIdx, mCbvSrvUavDescriptorSize);

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc1;
    cbvDesc1.BufferLocation = passCBAddress;
    cbvDesc1.SizeInBytes = passCBBytesSize;
    md3dDevice->CreateConstantBufferView(&cbvDesc1, handle);
}

void BoxApp::BuildRootSignature()
{
    // Shader programs typically require resources as input (constant buffers,
    // textures, samplers).  The root signature defines the resources the shader
    // programs expect.  If we think of the shader programs as a function, and
    // the input resources as function parameters, then the root signature can be
    // thought of as defining the function signature.  

    // Root parameter can be a table, root descriptor or root constants.
    CD3DX12_ROOT_PARAMETER slotRootParameter[2];
    //CD3DX12_ROOT_PARAMETER slotRootParameter[1];

    // Create a single descriptor table of CBVs.
    CD3DX12_DESCRIPTOR_RANGE cbvTable;
    cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

    CD3DX12_DESCRIPTOR_RANGE cbvTable1;
    cbvTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
    slotRootParameter[1].InitAsDescriptorTable(1, &cbvTable1);

    // A root signature is an array of root parameters.
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter, 0, nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
    ComPtr<ID3DBlob> serializedRootSig = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
        serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

    if (errorBlob != nullptr)
    {
        ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }
    ThrowIfFailed(hr);

    ThrowIfFailed(md3dDevice->CreateRootSignature(
        0,
        serializedRootSig->GetBufferPointer(),
        serializedRootSig->GetBufferSize(),
        IID_PPV_ARGS(&mRootSignature)));
}

void BoxApp::BuildShadersAndInputLayout()
{
    HRESULT hr = S_OK;

    mvsByteCode = d3dUtil::CompileShader(L"D:\\Sparrow\\dx12Demo\\shader\\color.hlsl", nullptr, "VS", "vs_5_0");
    mpsByteCode = d3dUtil::CompileShader(L"D:\\Sparrow\\dx12Demo\\shader\\color.hlsl", nullptr, "PS", "ps_5_0");

    mInputLayout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        //{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };
}

void BoxApp::BuildBoxGeometry()
{
    /*std::array<VPosData, 8> verticesPos =
    {
        VPosData({ XMFLOAT3(-1.0f, -1.0f, -1.0f) }),
        VPosData({ XMFLOAT3(-1.0f, +1.0f, -1.0f) }),
        VPosData({ XMFLOAT3(+1.0f, +1.0f, -1.0f) }),
        VPosData({ XMFLOAT3(+1.0f, -1.0f, -1.0f) }),
        VPosData({ XMFLOAT3(-1.0f, -1.0f, +1.0f) }),
        VPosData({ XMFLOAT3(-1.0f, +1.0f, +1.0f) }),
        VPosData({ XMFLOAT3(+1.0f, +1.0f, +1.0f) }),
        VPosData({ XMFLOAT3(+1.0f, -1.0f, +1.0f) })
    };
    std::array<VColorData, 8> verticesColor =
    {
	    VColorData({ XMFLOAT4(Colors::White) }),
	    VColorData({ XMFLOAT4(Colors::Black) }),
	    VColorData({ XMFLOAT4(Colors::Red) }),
	    VColorData({ XMFLOAT4(Colors::Green) }),
	    VColorData({ XMFLOAT4(Colors::Blue) }),
	    VColorData({ XMFLOAT4(Colors::Yellow) }),
	    VColorData({ XMFLOAT4(Colors::Cyan) }),
	    VColorData({ XMFLOAT4(Colors::Magenta) })
    };*/

    //std::array<VColorData, 8> verticesColor =
    //{
    //    VColorData({XMCOLOR(Colors::White)}),
    //    VColorData({XMCOLOR(Colors::Black)}),
    //    VColorData({XMCOLOR(Colors::Red)}),
    //    VColorData({XMCOLOR(Colors::Green)}),
    //    VColorData({XMCOLOR(Colors::Blue)}),
    //    VColorData({XMCOLOR(Colors::Yellow)}),
    //    VColorData({XMCOLOR(Colors::Cyan)}),
    //    VColorData({XMCOLOR(Colors::Magenta)})
    //};

    std::array<VPosData, 5> verticesPos =
    {
        VPosData({XMFLOAT3(0.0f,+1.0f,0.0f)}),
        VPosData({XMFLOAT3(-1.0f,-1.0f,-1.0f)}),
        VPosData({XMFLOAT3(1.0f,-1.0f,-1.0f)}),
        VPosData({XMFLOAT3(1.0f,-1.0f,1.0f)}),
        VPosData({XMFLOAT3(-1.0f,-1.0f,1.0f)})
    };

    std::array<VColorData, 5> verticesColor =
    {
        VColorData({XMFLOAT4(Colors::Red)}),
        VColorData({XMFLOAT4(Colors::Green)}),
        VColorData({XMFLOAT4(Colors::Green)}),
        VColorData({XMFLOAT4(Colors::Green)}),
        VColorData({XMFLOAT4(Colors::Green)})
    };

    std::array<Vertex, 8> vertices =
    {
        Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
        Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
        Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
        Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
        Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
        Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
        Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
        Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
    };

    std::array<std::uint16_t, 36> indices =
    {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 6, 5,
        4, 7, 6,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3,
        4, 3, 7
    };


    //std::array<std::uint16_t, 18> indices =
    //{
    //    0,2,1,
    //    0,4,3,
    //    0,1,4,
    //    0,3,2,
    //    2,4,1,
    //    2,3,4
    //};

    const UINT vPosBufferByteSize = (UINT)verticesPos.size() * sizeof(VPosData);
    const UINT vColorBufferByteSize = (UINT)verticesColor.size() * sizeof(VColorData);

    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

    mBoxGeo = std::make_unique<MeshGeometry>();
    mBoxGeo->Name = "boxGeo";

    ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
    //ThrowIfFailed(D3DCreateBlob(vPosBufferByteSize, &mBoxGeo->vPosBufferCpu));
    //ThrowIfFailed(D3DCreateBlob(vColorBufferByteSize, &mBoxGeo->vColorBufferCpu));

    CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

    //CopyMemory(mBoxGeo->vPosBufferCpu->GetBufferPointer(), verticesPos.data(), vPosBufferByteSize);
    //CopyMemory(mBoxGeo->vColorBufferCpu->GetBufferPointer(), verticesColor.data(), vColorBufferByteSize);

    ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
    CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

    //mBoxGeo->vPosBufferGpu = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), verticesPos.data(), vPosBufferByteSize, mBoxGeo->vPosBufferUpload);
    //mBoxGeo->vColorBufferGpu = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), verticesColor.data(), vColorBufferByteSize, mBoxGeo->vColorBufferUpload);

    mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
        mCommandList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

    mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
        mCommandList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

    //mBoxGeo->vPosBufferByteSize = vPosBufferByteSize;
    //mBoxGeo->vColorBufferByteSize = vColorBufferByteSize;
    //mBoxGeo->vPosBufferStride = sizeof(VPosData);
    //mBoxGeo->vColorBufferStride = sizeof(VColorData);

    mBoxGeo->VertexByteStride = sizeof(Vertex);
    mBoxGeo->VertexBufferByteSize = vbByteSize;
    mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
    mBoxGeo->IndexBufferByteSize = ibByteSize;

    SubmeshGeometry submesh;
    submesh.IndexCount = (UINT)indices.size();
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;

    mBoxGeo->DrawArgs["box"] = submesh;
}

void BoxApp::BuildPSO()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
    psoDesc.pRootSignature = mRootSignature.Get();
    psoDesc.VS =
    {
        reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
        mvsByteCode->GetBufferSize()
    };
    psoDesc.PS =
    {
        reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
        mpsByteCode->GetBufferSize()
    };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    //psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
    //psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = mBackBufferFormat;
    psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
    psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    psoDesc.DSVFormat = mDepthStencilFormat;
    ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}

void BoxApp::BuildGeometry()
{
    GeometryGenerator geoGen;
    GeometryGenerator::MeshData box = geoGen.CreateBox(1.5f, 1.5f, 1.5f, 3);
    GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);

    UINT boxVertexOffset = 0;
    UINT sphereVertexOffset = (UINT)box.Vertices.size();

    UINT boxIndexOffset = 0;
    UINT sphereIndexOffset = (UINT)box.Indices32.size();

    SubmeshGeometry boxSubmesh;
    boxSubmesh.IndexCount = (UINT)box.Indices32.size();
    boxSubmesh.StartIndexLocation = boxIndexOffset;
    boxSubmesh.BaseVertexLocation = boxVertexOffset;

    SubmeshGeometry sphereSubmesh;
    sphereSubmesh.IndexCount = (UINT)sphere.Indices32.size();
    sphereSubmesh.StartIndexLocation = sphereIndexOffset;
    sphereSubmesh.BaseVertexLocation = sphereVertexOffset;

    size_t totalVertexCount = box.Vertices.size() + sphere.Vertices.size();
    std::vector<Vertex> vertices(totalVertexCount);

    UINT k = 0;
    for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
    {
        vertices[k].Pos = box.Vertices[i].Position;
        vertices[k].Color = XMFLOAT4(Colors::Green);
    }

    for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
    {
        vertices[k].Pos = sphere.Vertices[i].Position;
        vertices[k].Color = XMFLOAT4(Colors::Brown);
    }

    std::vector<std::uint16_t> indices;
    indices.insert(indices.end(), box.GetIndices16().begin(), box.GetIndices16().end());
    indices.insert(indices.end(), sphere.GetIndices16().begin(), sphere.GetIndices16().end());

    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

    mGeo = std::make_unique<MeshGeometry>();
    mGeo->Name = "shapeGeo";

    ThrowIfFailed(D3DCreateBlob(vbByteSize, &mGeo->VertexBufferCPU));
    CopyMemory(mGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

    ThrowIfFailed(D3DCreateBlob(ibByteSize, &mGeo->IndexBufferCPU));
    CopyMemory(mGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

    mGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
        mCommandList.Get(), vertices.data(), vbByteSize, mGeo->VertexBufferUploader);

    mGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
        mCommandList.Get(), indices.data(), ibByteSize, mGeo->IndexBufferUploader);

    mGeo->VertexByteStride = sizeof(Vertex);
    mGeo->VertexBufferByteSize = vbByteSize;
    mGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
    mGeo->IndexBufferByteSize = ibByteSize;

    mGeo->DrawArgs["box"] = boxSubmesh;
    mGeo->DrawArgs["sphere"] = sphereSubmesh;
}