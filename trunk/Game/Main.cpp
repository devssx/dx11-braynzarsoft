#include <windows.h>

LPCTSTR WndClassName = L"DX11Game";			//Define our window class name
HWND hwnd = NULL;							//Sets our windows handle to NULL

const int Width = 800;
const int Height = 600;

//Functions
bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
int messageloop();

//Windows callback procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


//Main windows function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		MessageBox(0, L"Window Initialization - Failed", L"Error", MB_OK);
		return 0;
	}

	messageloop();
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
			//Otherewise, keep the flow going
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