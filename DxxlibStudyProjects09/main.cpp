//#include "BaseController.h"
#include "GameProgress.h"

//���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	progress::GameProgress thisGameProgress;
	thisGameProgress.GameInitialize();
	thisGameProgress.GameUpdate();
	DxLib::DxLib_End();
	return 0;
}