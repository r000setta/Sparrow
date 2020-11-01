#pragma once
#include "d3dApp.h"
#include "UploadBuffer.h"
#include "MathHelper.h"

#include "GeometryGenerator.h"

using Microsoft::WRL::ComPtr;

using namespace DirectX;
using namespace DirectX::PackedVector;

struct VPosData
{
    XMFLOAT3 Pos;
};

struct VColorData
{
    XMFLOAT4 Color;
};

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

struct ObjectConstants
{
    //XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
    XMFLOAT4X4 world = MathHelper::Identity4x4();
    XMFLOAT4 PulseColor;
    float Time = 0.0f;
};

struct PassConstants
{
    XMFLOAT4X4 viewProj = MathHelper::Identity4x4();
};

struct RenderItem
{
    RenderItem() = default;
    XMFLOAT4X4 World = MathHelper::Identity4x4();

    UINT objCBIndex = -1;
    D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    MeshGeometry* Geo = nullptr;

    UINT IndexCount = 0;
    UINT StartIndexLocation = 0;
    UINT BaseVertexLocation = 0;
};

class BoxApp :public D3DApp {
public:
    BoxApp(HINSTANCE hInstance) :D3DApp(hInstance) {}
    BoxApp(const BoxApp& rhs) = delete;
    BoxApp& operator=(const BoxApp& rhs) = delete;
    ~BoxApp() {}

    virtual bool Initialize() override;

protected:
    virtual void OnResize() override;
    virtual void Update(const GameTimer& gt) override;
    virtual void Draw(const GameTimer& gt) override;

    virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
    virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
    virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildBoxGeometry();
    void BuildGeometry();
    void BuildPSO();

    void BuildRenderItems();
    void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);
private:

    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

    std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
    std::unique_ptr<UploadBuffer<PassConstants>> mPassCB = nullptr;

    std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;
    std::unique_ptr<MeshGeometry> mGeo = nullptr;

    ComPtr<ID3DBlob> mvsByteCode = nullptr;
    ComPtr<ID3DBlob> mpsByteCode = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    ComPtr<ID3D12PipelineState> mPSO = nullptr;

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
    XMFLOAT4X4 mProj = MathHelper::Identity4x4();

    std::vector<std::unique_ptr<RenderItem>> mAllRitems;


    float mTheta = 1.5f * XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 5.0f;

    POINT mLastMousePos;
};