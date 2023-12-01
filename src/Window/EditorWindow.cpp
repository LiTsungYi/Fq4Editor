#include "Pch.h"

#include <memory>
#include <Commdlg.h>
#include <functional>
#include <Kafka/Stream/InoutFileStream.h>
#include "../resource.h"
#include "Window/EditorWindow.h"

namespace Fq4Editor
{

EditorWindow* EditorWindow::s_instance = nullptr;


EditorWindow::~EditorWindow()
{
    CloseFile();
}

std::wstring EditorWindow::LoadStringById( int id )
{
    WCHAR buffer[ MaxStringLength ];
    ZeroMemory( buffer, MaxStringLength );
    LoadStringW( m_hInstance, id, buffer, MaxStringLength );
    return std::wstring( buffer );
}

bool EditorWindow::Initialize( int nCmdShow )
{
    m_title = LoadStringById( IDS_APP_TITLE );
    m_windowClass = LoadStringById( IDR_WINDOW );

    RegisterClass();

    // Perform application initialization:
    if ( !InitInstance( nCmdShow ) )
    {
        return false;
    }
    return true;
}

int EditorWindow::Run()
{
    m_hAccelTable = LoadAccelerators( m_hInstance, MAKEINTRESOURCE( IDR_WINDOW ) );

    MSG msg;
    while ( GetMessage( &msg, nullptr, 0, 0 ) )
    {
        if ( !TranslateAccelerator( msg.hwnd, m_hAccelTable, &msg ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }

    return ( int ) msg.wParam;
}

//
//  FUNCTION: RegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM EditorWindow::RegisterClass()
{
    EditorWindow::s_instance = this;

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof( WNDCLASSEX );

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = &EditorWindow::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon( m_hInstance, MAKEINTRESOURCE( IDI_LARGE ) );
    wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = MAKEINTRESOURCEW( IDR_WINDOW );
    wcex.lpszClassName = m_windowClass.c_str();
    wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

    return RegisterClassExW( &wcex );
}


bool EditorWindow::InitInstance( int nCmdShow )
{
    HWND hWnd = CreateWindowW( m_windowClass.c_str(), m_title.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInstance, nullptr );

    if ( hWnd == NULL )
    {
        MessageBox( NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK );
        return false;
    }

    ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

    return true;
}


LRESULT CALLBACK EditorWindow::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    auto instance = EditorWindow::s_instance;
    if ( instance != NULL )
    {
        return instance->OnWindowProcess( hWnd, message, wParam, lParam );
    }
    else
    {
        return DefWindowProc( hWnd, message, wParam, lParam );
    }
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK EditorWindow::OnWindowProcess( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch ( message )
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD( wParam );
        // Parse the menu selections:
        switch ( wmId )
        {
        case IDM_ABOUT:
            DialogBox( m_hInstance, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, &EditorWindow::About );
            break;

        case IDM_EXIT:
            DestroyWindow( hWnd );
            break;

        default:
            return OnCommand( hWnd, message, wParam, lParam );
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint( hWnd, &ps );
        // TODO: Add any drawing code that uses hdc here...
        EndPaint( hWnd, &ps );
    }
    break;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

LRESULT CALLBACK EditorWindow::OnCommand( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    int wmId = LOWORD( wParam );
    // Parse the menu selections:
    switch ( wmId )
    {
    case IDM_FILE_SAVE:
        SaveFile( hWnd );
        break;

    case IDM_FILE_OPEN:
        OpenFile( hWnd );
        break;

    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK EditorWindow::About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    UNREFERENCED_PARAMETER( lParam );
    switch ( message )
    {
    case WM_INITDIALOG:
        return ( INT_PTR ) TRUE;

    case WM_COMMAND:
        if ( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
        {
            EndDialog( hDlg, LOWORD( wParam ) );
            return ( INT_PTR ) TRUE;
        }
        break;
    }
    return ( INT_PTR ) FALSE;
}

void EditorWindow::SaveFile( HWND hWnd )
{
    if ( m_fileStream != nullptr )
    {
        OnFileSaved();
        CloseFile();
    }
}

void EditorWindow::OpenFile( HWND hWnd )
{
    OPENFILENAME ofn;
    WCHAR szFile[ 260 ];

    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize = sizeof( ofn );
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[ 0 ] = '\0';
    ofn.nMaxFile = sizeof( szFile );
    ofn.lpstrFilter = L"Fq4 Save File\0FQ4GD.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if ( GetOpenFileName( &ofn ) == TRUE )
    {
        CloseFile();

        m_fileStream.reset( new Kafka::Stream::InoutFileStream() );
        m_fileStream->OpenFile( ofn.lpstrFile, true );

        OnFileOpened();
    }
}

void EditorWindow::CloseFile()
{
    if ( m_fileStream != nullptr )
    {
        m_fileStream.reset();
    }
}

} // namespace Fq4Editor
