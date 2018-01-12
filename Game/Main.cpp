#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

//Global Declarations//
IDXGISwapChain* SwapChain;
ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DevCon;
ID3D11RenderTargetView* renderTargetView;

float red = 0.0f;
float green = 0.0f;
float blue = 0.0f;
int colormodr = 1;
int colormodg = 1;
int colormodb = 1;

LPCTSTR WndClassName = L"DX11Game";			//Define our window class name
HWND hwnd = NULL;							//Sets our windows handle to NULL

const int Width = 800;
const int Height = 600;

//Functions
bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
int messageloop();

//Windows callback procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


//DX11 methods
bool InitializeDirect3d11App(HINSTANCE hInstance);
void ReleaseObjects();
bool InitScene();
void UpdateScene();
void DrawScene();


//Main windows function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		MessageBox(0, L"Window Initialization - Failed", L"Error", MB_OK);
		return 0;
	}

	if (!InitializeDirect3d11App(hInstance))
	{
		MessageBox(0, L"Direct3D Initialization - Failed", L"Error", MB_OK);
		return 0;
	}

	if (!InitScene())
	{
		MessageBox(0, L"Scene Initialization - Failed", L"Error", MB_OK);
		return 0;
	}

	messageloop();
	ReleaseObjects();
	return 0;
}

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);					//Size of our windows class
	wc.style = CS_HREDRAW | CS_VREDRAW;				//class styles
	wc.lpfnWndProc = WndProc;						//Default windows procedure function
	wc.cbClsExtra = NULL;							//Extra bytes after our wc structure
	wc.cbWndExtra = NULL;							//Extra bytes after our windows instance
	wc.hInstance = hInstance;						//Instance to current application
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			//Title bar Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);		//Default mouse Icon
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);	//Window bg color
	wc.lpszMenuName = NULL;							//Name of the menu attached to our window
	wc.lpszClassName = WndClassName;				//Name of our windows class
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);		//Icon in your taskbar

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	hwnd = CreateWindowEx(				//Create our Extended Window
		NULL,							//Extended style
		WndClassName,					//Name of our windows class
		L"DirectX 11 Game",				//Name in the title bar of our window
		WS_OVERLAPPEDWINDOW,			//style of our window
		CW_USEDEFAULT, CW_USEDEFAULT,	//Top left corner of window
		width,							//Width of our window
		height,							//Height of our window
		NULL,							//Handle to parent window
		NULL,							//Handle to a Menu
		hInstance,						//Specifies instance of current program
		NULL							//used for an MDI client window
	);

	if (!hwnd)
	{
		MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(hwnd, ShowWnd);    //Shows our window
	UpdateWindow(hwnd);           //Its good to update our window

	return true;
}


bool InitializeDirect3d11App(HINSTANCE hInstance)
{
	HRESULT hr;

	//Describe our Buffer
	DXGI_MODE_DESC bufferDesc = { 0 };

	bufferDesc.Width = Width;
	bufferDesc.Height = Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Set the feature level to DirectX 11. Set NULL to use the highest features available
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	//Gets the max feature level supported
	D3D_FEATURE_LEVEL maxFeatureLevel; //same as: d3d11Device->GetFeatureLevel();

	//Create our SwapChain and Device
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, featureLevels, 6, D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, &maxFeatureLevel, &d3d11DevCon);

	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//Create our Render Target
	hr = d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	BackBuffer->Release();


	//Set our Render Target
	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, NULL);

	return true;
}

void ReleaseObjects()
{
	//Release the COM Objects we created
	SwapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
}
bool InitScene()
{

	return true;
}

void UpdateScene()
{
	//Update the colors of our scene
	red += colormodr * 0.00005f;
	green += colormodg * 0.00002f;
	blue += colormodb * 0.00001f;

	if (red >= 1.0f || red <= 0.0f)
		colormodr *= -1;
	if (green >= 1.0f || green <= 0.0f)
		colormodg *= -1;
	if (blue >= 1.0f || blue <= 0.0f)
		colormodb *= -1;
}

void DrawScene()
{
	//Clear our backbuffer to the updated color
	D3DXCOLOR bgColor(red, green, blue, 1.0f);

	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);

	//Present the backbuffer to the screen
	SwapChain->Present(0, 0);
}


int messageloop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);

			//Send the message to default windows procedure
			DispatchMessage(&msg);
		}
		else
		{
			//run game code
			UpdateScene();
			DrawScene();
		}
	}

	return (int)msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (MessageBox(0, L"Are you sure you want to exit?", L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
				DestroyWindow(hwnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}