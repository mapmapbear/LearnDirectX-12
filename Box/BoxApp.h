#include "Common/d3dApp.h"
#include "Common/MathHelper.h"
#include "Common/UploadBuffer.h"
#include "FrameResource.h"
#include "Waves.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

// struct Vertex
// {
// 	XMFLOAT3 Pos;
// 	XMFLOAT4 Color;
// };
//
// struct ObjectConstants
// {
// 	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
// };

const int gNumFrameResources = 3;

struct RenderItem
{
	RenderItem() = default;

	XMFLOAT4X4 World = MathHelper::Identity4x4();

	int NumFramesDirty = gNumFrameResources;

	// 该索引指向,GPU常量缓存区对应的,当前渲染项中的物体常量缓冲区
	UINT ObjCBIndex = -1;

	// 此渲染项参与绘制的几何体,绘制一个几何体可能会用到多个渲染项
	MeshGeometry* Geo = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
};

class BoxApp : public D3DApp
{
public:
	BoxApp(HINSTANCE hInstance);
	BoxApp(const BoxApp& rhs) = delete;
	BoxApp& operator=(const BoxApp& rhs) = delete;
	~BoxApp();

	virtual bool Initialize() override;

private:
	virtual void OnResize() override;
	virtual void Update(const GameTimer& gt) override;
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);
	void UpdateCamera(const GameTimer& gt);
	void UpdateWaves(const GameTimer &gt);
	virtual void Draw(const GameTimer& gt) override;
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);

	virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

	void BuildDescriptorHeaps(); // 创建常量缓冲区描述符堆,为 mCbvHeap 赋值, RTV,DSV 描述符堆已在基类初始化
	void BuildConstantBufferViews();
	void BuildConstantBuffers(); // 为 mObjectCB 分配空间,描述符存在 mCbvHeap 中
	void BuildRootSignature(); // 根签名指定了常量缓冲区绑定到哪个着色器寄存器
	void BuildShadersAndInputLayout(); // 指定 VS, PS, mInputLayout(顶点结构体映射到VS的输入参数)
	void BuildBoxGeometry(); // 将顶点/索引上传到默认堆, 存到 mBoxGeo 中
	void BuildPSO(); // 为 mPSO 赋值
	void BuildFrameResources();
	void BuildRenderItems();
	void BuildLandGeometry();
	float GetHillsHeight(float x, float z) const;
	XMFLOAT3 GetHillsNormal(float x, float z) const;
	void BuildWavesGeometryBuffers();

private:

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr; // 根签名,指定了着色器程序所需的资源(CBV对应哪个着色器寄存器)
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr; // 常量缓冲区描述符

	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr; // 常量缓冲区, Update 中,每帧更新变换矩阵

	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;
	std::unique_ptr<MeshGeometry> mLandGeo1 = nullptr;
	std::unique_ptr<MeshGeometry> mWaveGeo = nullptr;

	ComPtr<ID3DBlob> mvsByteCode = nullptr; // 编译好的顶点着色器字节码
	ComPtr<ID3DBlob> mpsByteCode = nullptr; // 编译好的像素着色器字节码

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout; // 输入布局描述,将顶点结构体映射到VS的输入参数中

	ComPtr<ID3D12PipelineState> mPSO = nullptr; // 流水线状态对象,整合了 mRootSignature, mInputLayout, mvsByteCode, mpsByteCode

	XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	PassConstants mMainPassCB;
	XMFLOAT3 mEyePos = { 0.0f, 135.0f, 0.0f };

	std::vector<std::unique_ptr<FrameResource>> mFrameResources;
	FrameResource* mCurrFrameResources = nullptr;
	int mCurrFrameResourceIndex = 0;
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;


	std::vector<RenderItem*> mOpaqueRitems;
	std::vector<RenderItem*> mTransparentRitems;

	UINT mPassCbvOffset = 0; // 渲染过程CBV起始偏移量(最后3个),前面是3n个物体

	std::unique_ptr<Waves> mWaves;


	float mTheta = 1.5f*XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 150.0f;

	POINT mLastMousePos;
};