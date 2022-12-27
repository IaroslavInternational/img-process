#include "pch.h"
#include "framework.h"
#include "img-process.h"
#include "img-processDlg.h"
#include "afxdialogex.h"

#include <thread>

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

CimgprocessDlg::CimgprocessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMGPROCESS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CimgprocessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, noice_koef, noice_koef_ctrl);
}

BEGIN_MESSAGE_MAP(CimgprocessDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(btn_load,   &CimgprocessDlg::btn_load_Clicked)
	ON_BN_CLICKED(btn_to_bw,  &CimgprocessDlg::btn_to_bw_Clicked)
	ON_BN_CLICKED(btn_backup, &CimgprocessDlg::btn_backup_Clicked)
	ON_BN_CLICKED(btn_to_mono, &CimgprocessDlg::btn_to_mono_Clicked)
	ON_BN_CLICKED(btn_noise, &CimgprocessDlg::btn_noise_Clicked)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CimgprocessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	noice_koef_ctrl.SetRange(0, 255);

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

HCURSOR CimgprocessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CimgprocessDlg::btn_load_Clicked()
{	
	std::thread t(&CimgprocessDlg::UpdatePictureBox, this);
	t.join();
}

void CimgprocessDlg::btn_to_bw_Clicked()
{
	imglib::to_blackwhite(img);

	std::thread t(&CimgprocessDlg::UpdatePictureBox, this);
	t.join();
}

void CimgprocessDlg::btn_backup_Clicked()
{
	img.set_filename("..\\res\\test.bmp");
	
	std::thread t(&CimgprocessDlg::UpdatePictureBox, this);
	t.join();
}

inline void CimgprocessDlg::UpdatePictureBox()
{
	img.load();

	CStatic* pictureBox = (CStatic*)(GetDlgItem(picture_box));
	img.attach2obj(GetSafeHwnd(), pictureBox);
}

void CimgprocessDlg::btn_to_mono_Clicked()
{
	imglib::to_monochrome(img);

	std::thread t(&CimgprocessDlg::UpdatePictureBox, this);
	t.join();
}

void CimgprocessDlg::btn_noise_Clicked()
{
	imglib::add_noice(img, noice_koef_ctrl.GetPos());

	std::thread t(&CimgprocessDlg::UpdatePictureBox, this);
	t.join();
}

void CimgprocessDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
