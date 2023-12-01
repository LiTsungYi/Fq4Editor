#include "Pch.h"
#include <Windows.h>
#include "Window/Fq4EditorWindow.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    auto windowForm = Fq4Editor::Fq4EditorWindow( hInstance );
    if ( !windowForm.Initialize( nCmdShow ) )
    {
        return 0;
    }

    return windowForm.Run();
}
