
// FileUploadDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CFileUploadDlg 对话框
class CFileUploadDlg : public CDialogEx
{
// 构造
public:
	CFileUploadDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEUPLOAD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelectfile();
	CEdit m_MsgBox;
	CProgressCtrl m_Progress;
	CString m_URL;
//	afx_msg void OnBnClickedUpload();
protected:
	CWinThread *m_pThread;
public:
	afx_msg void OnBnClickedDebug();
	afx_msg void OnBnClickedUploadpathologymsg();
	CString m_PatientName;
	CString m_PatientPhoneTel;
//	CEdit m_PatientSex;
	CString m_PatientAge;
	CString m_PatientGuardName;
	CString m_PatientGuardTel;
	CString m_Relationship;
	CString m_PathologyGroup;
	CString m_DoctorName;
//	CEdit m_DoctorTel;
	CString m_Hospital;
	CString m_ExpertNum;
	CString m_FtpPicUrl;
	CString m_FileNameUrl;
	CString m_UserName;
	CString m_CoordUrl;
	CString m_OrderFormNum;
	CString m_Title;
	CString m_CoordWidth;
	CString m_CoordHeight;
	CString m_XCoord;
	CString m_YCoord;
	CString m_DoctorTel;
	CString m_PatientSex;
	afx_msg void OnBnClickedUploadpathologypic();
	afx_msg void OnBnClickedUploadcoord();
	afx_msg void OnBnClickedClearmsg();
	afx_msg void OnBnClickedSendFilename();
	CString m_Cipher;
};
