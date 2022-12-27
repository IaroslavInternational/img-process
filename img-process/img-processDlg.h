
// img-processDlg.h: файл заголовка
//

#pragma once

#include <imglib.hpp>

class CimgprocessDlg : public CDialogEx
{
public:
	CimgprocessDlg(CWnd* pParent = nullptr);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMGPROCESS_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	HICON m_hIcon;
protected:
	virtual BOOL    OnInitDialog();
	afx_msg void    OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void btn_load_Clicked();
	afx_msg void btn_to_mono_Clicked();
	inline  void UpdatePictureBox();
private:
	imglib::Image img{ "..\\res\\test.bmp" };
public:
	afx_msg void btn_backup_Clicked();
};
