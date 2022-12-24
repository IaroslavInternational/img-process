
// img-process.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CimgprocessApp:
// Сведения о реализации этого класса: img-process.cpp
//

class CimgprocessApp : public CWinApp
{
public:
	CimgprocessApp();

// Переопределение
public:
	virtual BOOL InitInstance();
};

extern CimgprocessApp theApp;
