
#include <windows.h>
#include "TriangleApp.h"


TriangleApp theApp;


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
{
    return theApp.go();
}