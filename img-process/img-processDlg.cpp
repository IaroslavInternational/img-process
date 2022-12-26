#include "pch.h"
#include "framework.h"
#include "img-process.h"
#include "img-processDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

#include <imglib.hpp>

CimgprocessDlg::CimgprocessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMGPROCESS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CimgprocessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CimgprocessDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(btn_load, &CimgprocessDlg::btn_load_Clicked)
END_MESSAGE_MAP()


// Обработчики сообщений CimgprocessDlg

BOOL CimgprocessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CimgprocessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CimgprocessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CimgprocessDlg::btn_load_Clicked()
{
	imglib::Print();
	
	CStatic* pictureBox = (CStatic*)(GetDlgItem(picture_box));
	
	CRect rect;
	pictureBox->GetClientRect(rect);
	
	CClientDC dc(pictureBox);
	
	CImage img;
	img.Load(L"C:\\Users\\HYPERPC\\Desktop\\test.bmp");
	
	CBitmap c_bmp;
	c_bmp.Attach(img.Detach());
	
	CDC memoryDC;
	memoryDC.CreateCompatibleDC(&dc);
	memoryDC.SelectObject(c_bmp);
	
	BITMAP bmp;
	c_bmp.GetBitmap(&bmp);
	
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memoryDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
}
