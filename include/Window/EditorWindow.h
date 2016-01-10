#pragma once

#include <windows.h>
#include <string>
#include <memory>
#include <Kafka/Applications/IConsole.h>

namespace Kafka
{

class InoutFileStream;

} // namespace Kafka

namespace Fq4Editor
{

class EditorWindow : Kafka::IConsole
{
private:
    static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
    static INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );

public:
    EditorWindow( HINSTANCE hInstance )
        : m_hInstance( hInstance )
        , m_hAccelTable( nullptr )
    {}

    virtual ~EditorWindow();

    bool Initialize( int nCmdShow );
    int Run();

protected:
    virtual LRESULT CALLBACK OnWindowProcess( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
    virtual LRESULT CALLBACK OnCommand( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
    std::wstring LoadStringById( int id );
    ATOM RegisterClass();
    bool InitInstance( int nCmdShow );

    void SaveFile( HWND hWnd );
    void OpenFile( HWND hWnd );
    void CloseFile();

    virtual void OnFileOpened()
    {}

    virtual void OnFileSaved()
    {}

private:
    static const int MaxStringLength = 100;
    static EditorWindow* s_instance;

    std::wstring m_title;
    std::wstring m_windowClass;
    HINSTANCE m_hInstance;
    HACCEL m_hAccelTable;

protected:
    std::shared_ptr<Kafka::InoutFileStream> m_fileStream;

};

} // namespace Fq4Editor
