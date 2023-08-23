// DemoTexture.cpp : Implements the CDemoTexture class.
//
//
//

#include "DemoTexture.h"

// Vertex struct
struct Vertex
{
    XMFLOAT3 pos;
    // TextCoord
};


//////////////////////////////////////////////////////////////////////
// Constructors

CDemoTexture::CDemoTexture()
{
    m_pVS = NULL;
    m_pPS = NULL;
    m_pInputLayout = NULL;
    m_pVertexBuffer = NULL;
    //Terxture and Sampler
}

CDemoTexture::~CDemoTexture()
{
}


//////////////////////////////////////////////////////////////////////
// Overrides

bool CDemoTexture::LoadContent()
{
    // Compilamos el vertex shader
    ID3DBlob* pVSBuffer = NULL;
    bool res = CompileShader(L"Recursos/VertexShader.hlsl", "VS_Main", "vs_4_0", &pVSBuffer);
    if (res == false) {
        ::MessageBox(m_hWnd, L"Unable to load vertex shader", L"ERROR", MB_OK);
        return false;
    }

    // Creamos el vertex shader
    HRESULT hr;
    hr = m_pD3DDevice->CreateVertexShader(
        pVSBuffer->GetBufferPointer(),
        pVSBuffer->GetBufferSize(),
        0, &m_pVS);
    if (FAILED(hr)) {
        if (pVSBuffer)
            pVSBuffer->Release();
        return false;
    }

    // Definir dise�o de entrada
    D3D11_INPUT_ELEMENT_DESC shaderInputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        //TextCoord
    };
    UINT numLayoutElements = ARRAYSIZE(shaderInputLayout);

    // Crear dise�o de entrada
    hr = m_pD3DDevice->CreateInputLayout(
        shaderInputLayout, numLayoutElements,
        pVSBuffer->GetBufferPointer(), 
        pVSBuffer->GetBufferSize(), 
        &m_pInputLayout);
    if (FAILED(hr)) {
        return false;
    }

    // Release VS buffer
    pVSBuffer->Release();
    pVSBuffer = NULL;

    // Compilamos el pixel shader
    ID3DBlob* pPSBuffer = NULL;
    res = CompileShader(L"Recursos/PixelShader.hlsl", "PS_Main", "ps_4_0", &pPSBuffer);
    if (res == false) {
        ::MessageBox(m_hWnd, L"Unable to load pixel shader", L"ERROR", MB_OK);
        return false;
    }

    // Creamos el pixel shader
    hr = m_pD3DDevice->CreatePixelShader(
        pPSBuffer->GetBufferPointer(),
        pPSBuffer->GetBufferSize(), 
        0, &m_pPS);
    if (FAILED(hr)) {
        return false;
    }

    // Cleanup PS buffer
    pPSBuffer->Release();
    pPSBuffer = NULL;


    // Definir tri�ngulo
    Vertex vertices[] =
    {
        { XMFLOAT3(  0.4f,  0.5f, 0.0f ) /*TextCoord*/ },
        { XMFLOAT3(  0.4f, -0.5f, 0.0f ) /*TextCoord*/ },
        { XMFLOAT3( -0.4f, -0.5f, 0.0f ) /*TextCoord*/ },

        { XMFLOAT3( -0.4f, -0.5f, 0.0f ) /*TextCoord*/ },
        { XMFLOAT3( -0.4f,  0.5f, 0.0f ) /*TextCoord*/ },
        { XMFLOAT3(  0.4f,  0.5f, 0.0f ) /*TextCoord*/ },
    };

    // Descripci�n del v�rtice
    D3D11_BUFFER_DESC vertexDesc;
    ::ZeroMemory(&vertexDesc, sizeof(vertexDesc));
    vertexDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexDesc.ByteWidth = sizeof(Vertex) * 6;

    // Datos de recursos
    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(resourceData));
    resourceData.pSysMem = vertices;

    // Creamos el vertex Buffer
    hr = m_pD3DDevice->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer);
    if (FAILED(hr)) {
        return false;
    }

    // Cargamos la textura

    // Texture sampler

    return true;
}

void CDemoTexture::UnloadContent()
{
    // Cleanup
    //Terxture and Sampler
    if (m_pVS)
        m_pVS->Release();
    m_pVS = NULL;
    if (m_pPS)
        m_pPS->Release();
    m_pPS = NULL;
    if (m_pInputLayout)
        m_pInputLayout->Release();
    m_pInputLayout = NULL;
    if (m_pVertexBuffer)
        m_pVertexBuffer->Release();
    m_pVertexBuffer = NULL;
}

void CDemoTexture::Update()
{
}

void CDemoTexture::Render()
{
    // Check if D3D is ready
    if (m_pD3DContext == NULL)
        return;

    // Clear back buffer
    float color[4] = { 0.0f, 0.25f, 0.25f, 1.0f };
    m_pD3DContext->ClearRenderTargetView(m_pD3DRenderTargetView, color);


    // Stride and offset
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    // Set vertex buffer
    m_pD3DContext->IASetInputLayout(m_pInputLayout);
    m_pD3DContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set shaders
    m_pD3DContext->VSSetShader(m_pVS, 0, 0);
    m_pD3DContext->PSSetShader(m_pPS, 0, 0);

    //Set Texture
    

    // Draw triangles
    m_pD3DContext->Draw(6, 0);


    // Present back buffer to display
    m_pSwapChain->Present(0, 0);
}