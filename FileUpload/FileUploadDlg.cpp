
// FileUploadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "FileUpload.h"
#include "FileUploadDlg.h"
#include "afxdialogex.h"
#include "curl\curl.h"
#include "RSA.h"
#include "base64Tool.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include<string>

using namespace std;

void ShowMsg(CString str)
{
	int   iLen = ((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_MsgBox.GetWindowTextLength();
	((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_MsgBox.SetSel(iLen, iLen, TRUE);
	((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_MsgBox.ReplaceSel("\r\n");
	((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_MsgBox.ReplaceSel(str);
	((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_MsgBox.ReplaceSel("\r\n");
}
static size_t read_callback(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	curl_off_t nread;
	size_t retcode = fread(ptr, size, nmemb, stream);
	nread = (curl_off_t)retcode;
	return retcode;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/*****************************************************全局变量*********************************************************/
CURL *GlobalCurl;
//struct curl_slist *GlobalHeaderlist = NULL;

int uploadtimes = 0;
CString pathName, fileName,extendname;
int ProcessFlag = 0;

char Paras[50];
int FileSize=0;

int UploadReadyFlag = 0;//FTP传输是否就绪

/**********************************************************************************************************************/

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileUploadDlg 对话框



CFileUploadDlg::CFileUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILEUPLOAD_DIALOG, pParent)
	, m_URL(_T("http://heer.com.cn/hospital-api/pathologyDataApi/baseInfoUpload"))
	, m_PatientName(_T("张三丰"))
	, m_PatientPhoneTel(_T("15374517221"))
	, m_PatientSex(_T("1"))
	, m_PatientAge(_T("75"))
	, m_PatientGuardName(_T("张五谷"))
	, m_PatientGuardTel(_T("18971348021"))
	, m_Relationship(_T("父子"))
	, m_PathologyGroup(_T("上皮组织肿瘤"))
	, m_DoctorName(_T("林志鹏"))
	, m_DoctorTel(_T("18207194527"))
	, m_Hospital(_T("武汉协和医院"))
	, m_ExpertNum(_T("101"))
	, m_FtpPicUrl(_T("ftp://119.96.241.98/pathology/"))
	//, m_FtpPicUrl(_T("ftp://192.168.0.112/curl/"))
	, m_FileNameUrl(_T("http://heer.com.cn/hospital-api/pathologyDataApi/pushFileName"))
	, m_UserName(_T("her"))
	, m_CoordUrl(_T("http://heer.com.cn/hospital-api/sectionLabelApi/uploadLabel"))
	, m_OrderFormNum(_T(""))
	, m_Title(_T(""))
	, m_CoordWidth(_T(""))
	, m_CoordHeight(_T(""))
	, m_XCoord(_T(""))
	, m_YCoord(_T(""))
	, m_Cipher(_T("1qaz@WSX!"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Msg, m_MsgBox);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Text(pDX, IDC_URL, m_URL);
	DDX_Text(pDX, IDC_PatientName, m_PatientName);
	DDX_Text(pDX, IDC_PatientPhoneTel, m_PatientPhoneTel);
	//  DDX_Control(pDX, IDC_PatientSex, m_PatientSex);
	DDX_Text(pDX, IDC_PatientAge, m_PatientAge);
	DDX_Text(pDX, IDC_PatientGuardName, m_PatientGuardName);
	DDX_Text(pDX, IDC_PatientGuardTel, m_PatientGuardTel);
	DDX_Text(pDX, IDC_Relationship, m_Relationship);
	DDX_Text(pDX, IDC_PathologyGroup, m_PathologyGroup);
	DDX_Text(pDX, IDC_DoctorName, m_DoctorName);
	//  DDX_Control(pDX, IDC_DoctorTel, m_DoctorTel);
	DDX_Text(pDX, IDC_Hospital, m_Hospital);
	DDX_Text(pDX, IDC_ExpertNum, m_ExpertNum);
	DDX_Text(pDX, IDC_FtpPicUrl, m_FtpPicUrl);
	DDX_Text(pDX, IDC_FileNameUrl, m_FileNameUrl);
	DDX_Text(pDX, IDC_UserName, m_UserName);
	DDX_Text(pDX, IDC_CoordUrl, m_CoordUrl);
	DDX_Text(pDX, IDC_OrderFormNum, m_OrderFormNum);
	DDX_Text(pDX, IDC_Title, m_Title);
	DDX_Text(pDX, IDC_CoordWidth, m_CoordWidth);
	DDX_Text(pDX, IDC_CoordHeight, m_CoordHeight);
	DDX_Text(pDX, IDC_XCoord, m_XCoord);
	DDX_Text(pDX, IDC_YCoord, m_YCoord);
	DDX_Text(pDX, IDC_DoctorTel, m_DoctorTel);
	DDX_Text(pDX, IDC_PatientSex, m_PatientSex);
	DDX_Text(pDX, IDC_Cipher, m_Cipher);
}

BEGIN_MESSAGE_MAP(CFileUploadDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SelectFile, &CFileUploadDlg::OnBnClickedSelectfile)
	//ON_BN_CLICKED(IDC_Upload, &CFileUploadDlg::OnBnClickedUpload)
	ON_BN_CLICKED(IDC_Debug, &CFileUploadDlg::OnBnClickedDebug)
	ON_BN_CLICKED(IDC_UploadPathologyMsg, &CFileUploadDlg::OnBnClickedUploadpathologymsg)
	ON_BN_CLICKED(IDC_UploadPathologyPic, &CFileUploadDlg::OnBnClickedUploadpathologypic)
	ON_BN_CLICKED(IDC_UploadCoord, &CFileUploadDlg::OnBnClickedUploadcoord)
	ON_BN_CLICKED(IDC_ClearMsg, &CFileUploadDlg::OnBnClickedClearmsg)
	ON_BN_CLICKED(IDC_SendFilename, &CFileUploadDlg::OnBnClickedSendFilename)
END_MESSAGE_MAP()


// CFileUploadDlg 消息处理程序

BOOL CFileUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileUploadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileUploadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileUploadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
///////////////////////////////////////////线程函数//////////////////////////////////////////
//UINT ThreadBaseMsgUpload(LPVOID pParm)
//{
//	HWND *pInfo = (HWND*)pParm;
//
//	res = curl_easy_perform(curl);
//	curl_easy_cleanup(curl);
//	return 0;
//}

/////////////////////////////////////////////////////////////////////////////////////////////
void CFileUploadDlg::OnBnClickedSelectfile()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_MsgBox.SetWindowTextA("");//先将状态窗口清空
	char* filters = _T("JPG文件(*.jpg)|*.jpg");
	//创建一个可以选择一个文件的CFileDialog  
	CFileDialog fileDlg(true, NULL, NULL,OFN_ENABLESIZING | OFN_HIDEREADONLY, NULL); 
	//显示文件对话框，获得文件名集合
	if (fileDlg.DoModal() == IDOK)
	{
		UploadReadyFlag = 1;
		//获取第一个文件的位置
		pathName = fileDlg.GetPathName();
		//循环读出每个路径并存放在数组中  

		//从字符串的后面往前遍历，如果遇到'\'则结束遍历，'\'右边的字符串则为文件名  
		int length = pathName.GetLength();
		for (int i = length - 1; i > 0; i--)
		{
			if ('\\' == pathName.GetAt(i))
			{//判断当前字符是否是'\'  
				fileName = pathName.Right(length - i - 1);
				break;//跳出循环  
			}
		}
		int n = fileName.Find('.');
		extendname = fileName.Right(fileName.GetLength()-n);
		CString str;
		/******************************************/
		int   iLen = m_MsgBox.GetWindowTextLength();
		m_MsgBox.SetSel(iLen, iLen, TRUE);
		/******************************************/
		str.Format("选中文件\r\n"+fileName);
		ShowMsg(str);
	}
}

wstring AsciiToUnicode(const string& str)
{
	// 预算-缓冲区中宽字节的长度  
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	// 给指向缓冲区的指针变量分配内存  
	wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
	// 开始向缓冲区转换字节  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
	wstring ret_str = pUnicode;
	free(pUnicode);
	return ret_str;
}

string UnicodeToUtf8(const wstring& wstr)
{
	// 预算-缓冲区中多字节的长度  
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// 给指向缓冲区的指针变量分配内存  
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
	// 开始向缓冲区转换字节  
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}

string AsciiToUtf8(const string& str)
{
	return UnicodeToUtf8(AsciiToUnicode(str));
}

char* UTF8ToEncode(const char* mbcsStr)
{
	wchar_t* wideStr;
	char* unicodeStr;
	int charLen;

	charLen = MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, NULL, 0);
	wideStr = (wchar_t*)malloc(sizeof(wchar_t)*charLen);
	MultiByteToWideChar(CP_UTF8, 0, mbcsStr, -1, wideStr, charLen);

	charLen = WideCharToMultiByte(936, 0, wideStr, -1, NULL, 0, NULL, NULL);
	unicodeStr = (char*)malloc(charLen);
	WideCharToMultiByte(936, 0, wideStr, -1, unicodeStr, charLen, NULL, NULL);

	free(wideStr);
	return unicodeStr;
}
size_t HttpMsgDebug(void *ptr, size_t size, size_t nmemb, void *stream)
{
	unsigned char*res = (unsigned char*)ptr;
	unsigned char rest[1000];
	for (int i = 0; i < 1000; i++)
	{
		rest[i] = res[i];
	}
	CString str(UTF8ToEncode((char*)rest));
	ShowMsg(str);
	return 0;
}


size_t HttpMsg(void *ptr, size_t size, size_t nmemb, void *stream)
{
	int CiphertextLength;
	int base64PlaintextLength;
	unsigned char base64Plaintext[4098] = {};
	//int PlaintextLength=0;
	unsigned char Plaintext[4098] = {};
	ZBase64 obj;
	//CString str;
	SSL_library_init();
	SSLeay_add_ssl_algorithms();
	SSL_load_error_strings();
	unsigned char*res = (unsigned char*)ptr;
	unsigned char rest[1000];
	for (int i = 0; i < 1000; i++)
	{
		if(res[i]!='\r')
			rest[i] = res[i];
		else
		{
			CiphertextLength = i-1;
			break;
		}

	}

	obj.Decode(rest, CiphertextLength, base64Plaintext, base64PlaintextLength);

	////////////////////////////////////////////////////////
	int decrypted_length = private_decrypt(base64Plaintext, base64PlaintextLength, Plaintext);
	if (decrypted_length == -1)
	{
		ShowMsg("RSA编解码失败");
	}

	/*UTF8ToEncode((char*)res);*/
	CString str(UTF8ToEncode((char*)Plaintext));
	ShowMsg(str);

	char buf1[100], buf2[100], buf3[100], buf4[100];

	sscanf(str, "%[^:]:%[^:]:%[^:]:%[^:]", buf1, buf2, buf3, buf4);
	if (strlen(buf4) > 3)
	{
		ShowMsg("订单编号更新");
		for (int i = 0; i < 50; i++)
		{
			Paras[i] = '\0';
		}
		for(int i=1;i<strlen(buf4)-2;i++)
		{ 
			Paras[i- 1] = buf4[i];
		}
		CString name(Paras);
		((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_OrderFormNum = name;
		((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->UpdateData(FALSE);
	}
	return 0;
}


int FtpProgress(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	if ((ultotal != 0)&&(UploadReadyFlag==1))
	{
		((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_Progress.SetRange(0, ultotal / 1048576);
		((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_Progress.SetPos(ulnow / 1048576);

		if (ultotal == ulnow)
		{
			UploadReadyFlag = 0;
			ShowMsg("*上传完成*");
			((CFileUploadDlg*)(AfxGetApp()->m_pMainWnd))->m_Progress.SetPos(0 / 1048576);
		}
	}
	return 0;
}
UINT ThreadFileUpload(LPVOID pParm)
{
	HWND *pInfo = (HWND*)pParm;
	///////////////////////////////--------执行操作---------//////////////////////////
	CURLcode res;
	res = curl_easy_perform(GlobalCurl);
	if (res != CURLE_OK)
	{
		CString str;
		str.Format("文件传输出现错误，请检查。。。%s", curl_easy_strerror(res));
		ShowMsg(str);
	}
	/* clean up the FTP commands list */
	//curl_slist_free_all(GlobalHeaderlist);
	/* always cleanup */
	curl_easy_cleanup(GlobalCurl);
	curl_global_cleanup();
	return 0;
}

UINT ThreadFileNameUpload(LPVOID pParm)
{
	HWND *pInfo = (HWND*)pParm;
	///////////////////////////////--------执行操作---------//////////////////////////
	CURLcode res;
	res = curl_easy_perform(GlobalCurl);

	/* always cleanup */
	curl_easy_cleanup(GlobalCurl);
	curl_global_cleanup();
	return 0;
}


void CFileUploadDlg::OnBnClickedDebug()
{
	// TODO: 在此添加控件通知处理程序代码
	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	////////////////////////-------------------------------///////////////////////
	struct curl_slist *headers = NULL; /* init to NULL is important */
	headers = curl_slist_append(headers, "Authorization:APPCODE b45c6e5ac088496bab50944797a852ee");
	headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded; charset=utf-8");
	//headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
	//headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded; charset=GB2312");

	////////////////////////-------------------------------///////////////////////
	if (curl) 
	{
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, "http://freecityid.market.alicloudapi.com/whapi/json/alicityweather/briefforecast3days");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpMsgDebug);//指定接受消息回调函数
		/* Now specify the POST data */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "cityId=2&token=677282c2f1b3d718152c4e25ed434bc4");
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		//printf(res);
		/* always cleanup */
		curl_slist_free_all(headers);//记得要释放
		curl_easy_cleanup(curl);
	}
}

void CFileUploadDlg::OnBnClickedUploadpathologymsg()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned char  encrypted[4098] = {};
	unsigned char decrypted[4098] = {};
	unsigned char base64buffer[1000];
	int EncryptLength;
	int Base64EncryptLength;
	ZBase64 Base64obj;
	char SourceBuf[1000];
	//string MsgToSend;

	ShowMsg("\r\n开始。。。。上传病理基本信息\r\n");
	ProcessFlag = 1;
	UpdateData(TRUE);

	CString str;
	CString strJson = "{";
	strJson += "\"patientName\":\""; strJson += m_PatientName;
	strJson += "\",\"patientPhone\":\""; strJson += m_PatientPhoneTel;
	strJson += "\",\"patientSex\":\""; strJson += m_PatientSex;

	strJson += "\",\"patientAge\":\""; strJson += m_PatientAge;
	strJson += "\",\"contactName\":\""; strJson += m_PatientGuardName;

	strJson += "\",\"contactPhone\":\""; strJson += m_PatientGuardTel;
	strJson += "\",\"relationship\":\""; strJson += m_Relationship;
	strJson += "\",\"pathologyName\":\""; strJson += m_PathologyGroup;
	strJson += "\",\"doctorName\":\""; strJson += m_DoctorName;
	strJson += "\",\"doctorPhone\":\""; strJson += m_DoctorTel;
	strJson += "\",\"hospitalName\":\""; strJson += m_Hospital;
	strJson += "\",\"specialistUniqueId\":\""; strJson += m_ExpertNum;
	strJson += "\"}";

	string utf8Msg = strJson.GetBuffer();
	//m_MsgBox.ReplaceSel(strJson);
	utf8Msg = AsciiToUtf8(utf8Msg);//如果json串中包含有中文,必须进行转码
	//const char *SourceBuf= utf8Msg.data();
	for (int i = 0; i < utf8Msg.length(); i++)
	{
		SourceBuf[i] = utf8Msg[i];
	}
	RSAEncypt((unsigned char*)SourceBuf, utf8Msg.length(), encrypted, EncryptLength);
	///////////////////////////////////////////////////////
	Base64obj.Encode(encrypted, EncryptLength, base64buffer, Base64EncryptLength);
	string upJson((char*)(base64buffer));
	char *MsgToSend = new char[Base64EncryptLength+1];

	for (int i = 0; i < Base64EncryptLength; i++)
	{
		MsgToSend[i] = base64buffer[i];
	}
	MsgToSend[Base64EncryptLength] = '\0';
	//str.Format("Encrypted =%s\r\n", MsgToSend);
	//m_MsgBox.ReplaceSel(str);
	//str.Format("Encrypted length =%d\r\n", EncryptLength);
	//m_MsgBox.ReplaceSel(str);
	/////////////////////////////////--------curl---------//////////////////////////
	CURL *pCurl = NULL;
	CURLcode res;
	// In windows, this will init the winsock stuff
	curl_global_init(CURL_GLOBAL_ALL);

	// get a curl handle
	pCurl = curl_easy_init();
	if (NULL != pCurl)
	{
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, HttpMsg);//指定接受消息回调函数
																// 设置超时时间为1秒
		curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 5);

		// First set the URL that is about to receive our POST. 
		// This URL can just as well be a 
		// https:// URL if that is what should receive the data.
		curl_easy_setopt(pCurl, CURLOPT_URL, m_URL);

		// 设置http发送的内容类型为JSON
		curl_slist *plist = curl_slist_append(NULL,
			"Content-Type:application/json;charset=UTF-8");
		curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, plist);

		// 设置要POST的JSON数据
		curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, MsgToSend);

		//set msg

		// Perform the request, res will get the return code
		res = curl_easy_perform(pCurl);

		//m_MsgBox.ReplaceSel("curl_easy_perform\r\n");
		//// Check for errors
		//if (res != CURLE_OK)
		//{
		//	m_MsgBox.ReplaceSel("curl_easy_perform() failed\r\n");
		//}
		// always cleanup
		curl_easy_cleanup(pCurl);
	}
	curl_global_cleanup();

}


void CFileUploadDlg::OnBnClickedUploadpathologypic()
{
	// TODO: 在此添加控件通知处理程序代码
	//CURL *curl;
	UpdateData(TRUE);
	FILE *hd_src;
	struct stat file_info;
	curl_off_t fsize;
	struct curl_slist *GlobalHeaderlist = NULL;
	/* get the file size of the local file */
	if ((pathName!="")&&(Paras[0]!='\0'))
	{
		if (stat(pathName, &file_info)) {
			ShowMsg("无法打开文件");
		}
		FileSize=file_info.st_size;
		fsize = (curl_off_t)FileSize;
		CString str;
		str.Format("文件大小%d",FileSize);
		ShowMsg(str);
		/* get a FILE * of the same file */
		hd_src = fopen(pathName, "rb");
		/* In windows, this will init the winsock stuff */
		curl_global_init(CURL_GLOBAL_ALL);
		/* get a curl handle */
		GlobalCurl = curl_easy_init();
		if (GlobalCurl) {
			/* we want to use our own read function */
			curl_easy_setopt(GlobalCurl, CURLOPT_READFUNCTION, read_callback);
			/* enable uploading */
			curl_easy_setopt(GlobalCurl, CURLOPT_UPLOAD, 1L);
			//指定接受消息回调函数
			curl_easy_setopt(GlobalCurl, CURLOPT_WRITEFUNCTION, HttpMsg);
			//设置超时时间
			curl_easy_setopt(GlobalCurl, CURLOPT_TIMEOUT, 7200);
			curl_easy_setopt(GlobalCurl, CURLOPT_CONNECTTIMEOUT, 7200);//设置超时时间
			/* specify target */
			CString name(Paras);
			curl_easy_setopt(GlobalCurl, CURLOPT_URL, m_FtpPicUrl + name +extendname);
			CString usrpasswd = m_UserName + ":" + m_Cipher;
			curl_easy_setopt(GlobalCurl, CURLOPT_USERPWD, usrpasswd);//////////////////////////////////////////////////////////
			/* pass in that last of FTP commands to run after the transfer */
			curl_easy_setopt(GlobalCurl, CURLOPT_POSTQUOTE, GlobalHeaderlist);
			/* now specify which file to upload */
			curl_easy_setopt(GlobalCurl, CURLOPT_READDATA, hd_src);
			/* Set the size of the file to upload (optional).  If you give a *_LARGE
			option you MUST make sure that the type of the passed-in argument is a
			curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
			make sure that to pass in a type 'long' argument. */
			curl_easy_setopt(GlobalCurl, CURLOPT_INFILESIZE_LARGE,
				(curl_off_t)fsize);
			curl_easy_setopt(GlobalCurl, CURLOPT_XFERINFOFUNCTION, FtpProgress);//指定回调函数获取进度
			curl_easy_setopt(GlobalCurl, CURLOPT_NOPROGRESS, 0L);//如果希望获取下载或者上传进度相关信息，就给CURLOPT_NOPROGRESS属性设置0值
			m_pThread = AfxBeginThread(ThreadFileUpload, &m_hWnd);//启用多线程
			/* Now run off and do what you've been told! */
			//res = curl_easy_perform(GlobalCurl);
			///* Check for errors */
			//if (res != CURLE_OK)
			//	fprintf(stderr, "curl_easy_perform() failed: %s\n",
			//		curl_easy_strerror(res));

			///* clean up the FTP commands list */
			//curl_slist_free_all(GlobalHeaderlist);

			///* always cleanup */
			//curl_easy_cleanup(GlobalCurl);
		}

		//curl_global_cleanup();
	}
	else
	{
		ShowMsg("未选择文件或未更新订单号！！");
	}
}


void CFileUploadDlg::OnBnClickedUploadcoord()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned char  encrypted[4098] = {};
	unsigned char decrypted[4098] = {};
	unsigned char base64buffer[1000];
	int EncryptLength;
	int Base64EncryptLength;
	ZBase64 Base64obj;
	char SourceBuf[1000];
	//string MsgToSend;

	ShowMsg("开始。。。。上传坐标");
	ProcessFlag = 1;
	UpdateData(TRUE);

	CString str;
	CString strJson = "{";
	strJson += "\"orderNo\":\""; strJson += m_OrderFormNum;
	strJson += "\",\"title\":\""; strJson += m_Title;
	strJson += "\",\"width\":\""; strJson += m_CoordWidth;
	strJson += "\",\"height\":\""; strJson += m_CoordHeight;
	strJson += "\",\"xAxis\":\""; strJson += m_XCoord;
	strJson += "\",\"yAxis\":\""; strJson += m_YCoord;
	strJson += "\"}";

	string utf8Msg = strJson.GetBuffer();
	//m_MsgBox.ReplaceSel(strJson);
	utf8Msg = AsciiToUtf8(utf8Msg);//如果json串中包含有中文,必须进行转码
								   //const char *SourceBuf= utf8Msg.data();
	for (int i = 0; i < utf8Msg.length(); i++)
	{
		SourceBuf[i] = utf8Msg[i];
	}
	RSAEncypt((unsigned char*)SourceBuf, utf8Msg.length(), encrypted, EncryptLength);
	///////////////////////////////////////////////////////
	Base64obj.Encode(encrypted, EncryptLength, base64buffer, Base64EncryptLength);
	string upJson((char*)(base64buffer));
	char *MsgToSend = new char[Base64EncryptLength + 1];

	for (int i = 0; i < Base64EncryptLength; i++)
	{
		MsgToSend[i] = base64buffer[i];
	}
	MsgToSend[Base64EncryptLength] = '\0';
	//str.Format("Encrypted =%s\r\n", MsgToSend);
	//m_MsgBox.ReplaceSel(str);
	//str.Format("Encrypted length =%d\r\n", EncryptLength);
	//m_MsgBox.ReplaceSel(str);
	/////////////////////////////////--------curl---------//////////////////////////
	CURL *pCurl = NULL;
	CURLcode res;
	// In windows, this will init the winsock stuff
	curl_global_init(CURL_GLOBAL_ALL);

	// get a curl handle
	pCurl = curl_easy_init();
	if (NULL != pCurl)
	{
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, HttpMsg);
		curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 5);
		curl_easy_setopt(pCurl, CURLOPT_URL, m_CoordUrl);
		// 设置http发送的内容类型为JSON
		curl_slist *plist = curl_slist_append(NULL,
			"Content-Type:application/json;charset=UTF-8");
		curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, plist);
		curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, MsgToSend);
		res = curl_easy_perform(pCurl);
		curl_easy_cleanup(pCurl);
	}
	curl_global_cleanup();
}


void CFileUploadDlg::OnBnClickedClearmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_MsgBox.SetWindowText("");
}


void CFileUploadDlg::OnBnClickedSendFilename()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	unsigned char  encrypted[4098] = {};
	unsigned char decrypted[4098] = {};
	unsigned char base64buffer[1000];
	int EncryptLength;
	int Base64EncryptLength;
	ZBase64 Base64obj;
	char SourceBuf[1000];

	ShowMsg("开始推送文件名,请耐心等待服务器处理图像^-^\r\n");
	ProcessFlag = 1;
	UpdateData(TRUE);

	CString fileName(Paras);
	fileName = fileName + extendname;
	CString strJson = "{";
	strJson += "\"fileName\":\""; strJson += fileName;
	strJson += "\"}";

	string utf8Msg = strJson.GetBuffer();

	utf8Msg = AsciiToUtf8(utf8Msg);//如果json串中包含有中文,必须进行转码

	for (int i = 0; i < utf8Msg.length(); i++)
	{
		SourceBuf[i] = utf8Msg[i];
	}
	RSAEncypt((unsigned char*)SourceBuf, utf8Msg.length(), encrypted, EncryptLength);
	///////////////////////////////////////////////////////
	Base64obj.Encode(encrypted, EncryptLength, base64buffer, Base64EncryptLength);
	string upJson((char*)(base64buffer));
	char *MsgToSend = new char[Base64EncryptLength + 1];

	for (int i = 0; i < Base64EncryptLength; i++)
	{
		MsgToSend[i] = base64buffer[i];
	}
	MsgToSend[Base64EncryptLength] = '\0';
	/////////////////////////////////--------curl---------//////////////////////////

	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	GlobalCurl = curl_easy_init();
	if (NULL != GlobalCurl)
	{
		curl_easy_setopt(GlobalCurl, CURLOPT_WRITEFUNCTION, HttpMsg);//指定接受消息回调函数
		curl_easy_setopt(GlobalCurl, CURLOPT_TIMEOUT, 3600);
		curl_easy_setopt(GlobalCurl, CURLOPT_CONNECTTIMEOUT, 3600);//设置超时时间
		curl_easy_setopt(GlobalCurl, CURLOPT_URL, m_FileNameUrl);

		curl_slist *plist = curl_slist_append(NULL,
			"Content-Type:application/json;charset=UTF-8");
		curl_easy_setopt(GlobalCurl, CURLOPT_HTTPHEADER, plist);
		curl_easy_setopt(GlobalCurl, CURLOPT_POSTFIELDS, MsgToSend);
		m_pThread = AfxBeginThread(ThreadFileNameUpload, &m_hWnd);//启用多线程
	}
	//curl_global_cleanup();
}



