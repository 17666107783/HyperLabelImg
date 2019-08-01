// BezierDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bezier.h"
#include "BezierDlg.h"
#include <fstream>
#include <iostream>
#include<Common\CommonWin.h>
#include <stdio.h>
#include "atlimage.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include "ImageLayer.h"
#include "resource.h"

/*
  sunjunlishi

*/
using namespace std;
using namespace cv;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBezierDlg dialog

CBezierDlg::CBezierDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBezierDlg::IDD, pParent)
{


	m_bMouseMove = false;
	m_leftBtnDown = false;
	m_bKeyOper = false;

	m_bDeleteFile = false;
	//�ƶ����ο�
	mBMovingRect = false;

	m_nowMultiShow = 1.0;

	//�������ʼλ��
	mStartPt.x = 0;
	mStartPt.y = 0;
	//����ƶ�λ��
	mNowPt.x = 0;
	mNowPt.y = 0;
	//�µĳ�ʼ��λ��
	mInitPt.x = 0;
	mInitPt.y = 0;
	m_currentMark = -1;
}

void CBezierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBezierDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_LIST1, mListBox);
}

BEGIN_MESSAGE_MAP(CBezierDlg, CDialog)
	//{{AFX_MSG_MAP(CBezierDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()


	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_BTN_SAVE, &CBezierDlg::OnBnClickedBtnSave)
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOMMAND()

	

	ON_BN_CLICKED(IDC_BUTTON_IN, &CBezierDlg::OnBnClickedButtonIn)
	ON_WM_MOVING()
	ON_BN_CLICKED(IDC_BTN_FOLDER, &CBezierDlg::OnBnClickedBtnFolder)
	ON_LBN_SELCHANGE(IDC_LIST1, &CBezierDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BTN_DEL, &CBezierDlg::OnBnClickedBtnDel)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL CBezierDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_currentMark = -1;



	CRect rect;
	GetClientRect(rect);


	//�ؼ��㣬��������һ��
	m_points.push_back(cv::Point(500, 500));
	m_points.push_back(cv::Point(750, 500));
	m_points.push_back(cv::Point(750, 550));
	m_points.push_back(cv::Point(500, 550));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/*

	���ܣ��������ؼ����Ӧ�� �ƶ���
*/
void CBezierDlg::DrawPoint(int start,int end,CDC* pDC)
{
	
	// //��tmpNowPts��������ʾ��m_points��Ϊԭʼ���ݱ��ֲ���
	vector<Point2f> tmpNowPts;
	for (int k = 0; k < m_points.size(); ++k)
    {
		tmpNowPts.push_back(cv::Point2f(m_points[k].x*m_nowMultiShow, m_points[k].y*m_nowMultiShow));
	}


	int disX = mNowPt.x - mStartPt.x;
	int disY = mNowPt.y - mStartPt.y;
	if (!mBMovingRect)
	{
		disX = 0;
		disY = 0;
	}

	pDC->MoveTo(tmpNowPts[start].x+ disX, tmpNowPts[start].y+disY);
	for(int i = start; i<=end; i++)
	{
			CPen penStroke(PS_SOLID,1,0x007700);
			CPen *ppenPrevious=pDC->SelectObject(&penStroke);
			pDC->LineTo(tmpNowPts[i].x+disX, tmpNowPts[i].y+disY);
			if(i == end)
				pDC->LineTo(tmpNowPts[start].x+disX, tmpNowPts[start].y+disY);

			pDC->SelectObject(ppenPrevious);

			CPen penStroke1(PS_SOLID,1,0x00FFFF);
		    CPen penStroke2(PS_SOLID,1,0x0000FF);

			CPen *ppenPrevious2;
			if(m_currentMark == i)
			{
				ppenPrevious2 = pDC->SelectObject(&penStroke2);
			}
			else
			{
				ppenPrevious2 = pDC->SelectObject(&penStroke1);
			}
			

			pDC->SetBkMode(TRANSPARENT);
		
			cv::Point2d pt = cv::Point(tmpNowPts[i].x+disX, tmpNowPts[i].y+disY);
			CPoint tmpPts[4];
			int spanD = 6;
			tmpPts[0].x = pt.x - spanD;
			tmpPts[0].y = pt.y - spanD;

			tmpPts[1].x = pt.x + spanD;
			tmpPts[1].y = pt.y - spanD;
			tmpPts[2].x = pt.x + spanD;
			tmpPts[2].y = pt.y + spanD;
			tmpPts[3].x = pt.x - spanD;
			tmpPts[3].y = pt.y + spanD;
		
			
			 pDC->SelectStockObject(NULL_BRUSH);
			 pDC->Ellipse(tmpPts[0].x,tmpPts[0].y,tmpPts[2].x,tmpPts[2].y);

		
			pDC->SelectObject(ppenPrevious2);
	}
	
}
/*

���ܣ���������Ҫ��  ��Ϊ�ڽ���΢���ؼ����ʱ�򣬷�ֹ��˸�� ����˫����������
  ���� ����������ͼƬ��Ȼ����ƹؼ��㣬���Ƶ��������������������
  
*/
void CBezierDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

	}
	else
	{
		CDialog::OnPaint();

		CDC * pDC = GetDC();
		int iCanvaW = 1560;
		if(!m_nowImg.empty() && pDC)
		{

			   CImage ImageBk;
			   ImageBk.Create(iCanvaW, 960, 24);
				CImage Image;
				cv::Mat dst = m_nowImg.clone();
				cv::resize(dst, dst, cv::Size(m_nowImg.cols*m_nowMultiShow, m_nowImg.rows*m_nowMultiShow));


				Image.Create(dst.cols,dst.rows,24);
			
				int i;	int j;

				for (i = 0; i < Image.GetHeight(); i++)
				{
					int step = dst.step[0]*i;
					int jump = 0;
					for (j = 0; j < Image.GetWidth(); j++)
					{
						byte *pByte = (byte *)Image.GetPixelAddress(j,i);
						pByte[0] = (unsigned char)dst.data[step+jump+0];
						pByte[1] = (unsigned char)dst.data[step+jump+1];
						pByte[2] = (unsigned char)dst.data[step+jump+2];
		
						jump+=3;
					}
				}


				CDC   MemDC;   //���ȶ���һ����ʾ�豸���� 
				CBitmap   MemBitmap;//����һ��λͼ���� 

				pDC->SetBkMode(TRANSPARENT);
				//���������Ļ��ʾ���ݵ��ڴ���ʾ�豸 
				MemDC.CreateCompatibleDC(pDC); 
				//��ʱ�����ܻ�ͼ����Ϊû�еط���   ^_^ 
				//���潨��һ������Ļ��ʾ���ݵ�λͼ������λͼ�Ĵ�С������ô��ڵĴ�С 
				MemBitmap.CreateCompatibleBitmap(pDC, iCanvaW,960);
			
				//��λͼѡ�뵽�ڴ���ʾ�豸�� 
				//ֻ��ѡ����λͼ���ڴ���ʾ�豸���еط���ͼ������ָ����λͼ�� 
				CBitmap  *pOldBit=MemDC.SelectObject(&MemBitmap); 

				ImageBk.Draw(MemDC.m_hDC, 0, 0, iCanvaW,960);
				if (mBMovingRect || m_currentMark >=0)
				{//�����ƶ��� ���� �ƶ��������£��������治��Ҫ�ƶ�
					Image.Draw(MemDC.m_hDC, mInitPt.x, mInitPt.y,Image.GetWidth(), Image.GetHeight());
				}
				else
				Image.Draw(MemDC.m_hDC, mInitPt.x + mNowPt.x - mStartPt.x, mInitPt.y + mNowPt.y - mStartPt.y, 
					Image.GetWidth(),Image.GetHeight());

				if(m_points.size() > 0) 
				{
					//������
					DrawPoint(0,3,&MemDC);
			
				}
				//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ 
				pDC->BitBlt(0,0 ,
					iCanvaW, 960,&MemDC,0,0,SRCCOPY);


				MemDC.SelectObject(pOldBit);
				//��ͼ��ɺ������ 
				MemBitmap.DeleteObject(); 
				MemDC.DeleteDC();

				ReleaseDC(pDC);

		}

	}
}

void CBezierDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	mStartPt = point;
	mNowPt = point;
	m_leftBtnDown = true;
	// TODO: Add your message handler code here and/or call default
	m_currentMark = -1;

	// //��tmpNowPts��������ʾ��m_points��Ϊԭʼ���ݱ��ֲ���
	vector<Point2f> tmpNowPts;
	for (int k = 0; k < m_points.size(); ++k)
	{
		tmpNowPts.push_back(cv::Point2f(m_points[k].x*m_nowMultiShow, m_points[k].y*m_nowMultiShow));
	}

		double x, y;
		double t=56;
		for(int i = 0; i < tmpNowPts.size(); i++)
		{
			x = tmpNowPts[i].x - point.x, y = tmpNowPts[i].y - point.y;
			 x*=x; y*=y;
			if(x + y < t)
			{	
				m_currentMark = i;		
			}

		}

		
		//�ж��Ƿ��ƶ�����
		if (cv::pointPolygonTest(tmpNowPts, cv::Point(point.x, point.y), true) > 4)
		{
				mBMovingRect = true;
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
		}
		else
		{
				mBMovingRect = false;
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		
	
	CDialog::OnLButtonDown(nFlags, point);
	//RedrawWindow();
}

void CBezierDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//���е����ݽ��г�ʼ�����߸���
	if ( m_leftBtnDown )
	{
		//˵�������ƶ��㣬��������ͼƬ���ƶ�
		if (m_currentMark < 0 && !mBMovingRect)
		{
			//��ԭʼλ�ý��б���
			mInitPt.x = mInitPt.x + mNowPt.x - mStartPt.x;
			mInitPt.y = mInitPt.y + mNowPt.y - mStartPt.y;
		}

		if ( mBMovingRect)
		{
			for (int k = 0; k < m_points.size(); ++k)
			{
				m_points[k].x += (mNowPt.x - mStartPt.x)/m_nowMultiShow;
				m_points[k].y += (mNowPt.y - mStartPt.y) / m_nowMultiShow;
			}
			mBMovingRect = false;
		}
	}


	mNowPt.x = 0;
	mNowPt.y = 0;
	mStartPt.x = 0;
	mStartPt.y = 0;

	m_leftBtnDown = false;
	// TODO: Add your message handler code here and/or call default
	m_bMouseMove = false;
	m_currentMark = -1;

	CDialog::OnLButtonUp(nFlags, point);
}

void CBezierDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// //����߽����4������
	// //��tmpPts��������ʾ��m_points��Ϊԭʼ���ݱ��ֲ���
	//vector<Point2f> tmpPts;
	//for (int k = 0; k < m_points.size(); ++k)
	//{
	//	tmpPts.push_back(cv::Point2f(m_points[k].x*m_nowMultiShow, m_points[k].y*m_nowMultiShow));
	//}

	if (m_leftBtnDown)
	{
		mNowPt = point;
		if (m_currentMark >= 0)
		{//�ƶ��ؼ���
			m_points[m_currentMark].x = mNowPt.x/ m_nowMultiShow;
			m_points[m_currentMark].y = mNowPt.y/ m_nowMultiShow;
		}
		RedrawWindow();
	}
	
	if (mBMovingRect)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));

	}
	else
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}


	CDialog::OnMouseMove(nFlags, point);
}






/*

��һ���ļ������е�ֵ���й�0

*/
void CBezierDlg::DoGetPtsAndDraw(CString fileName)
{
	mInitPt.x = 0;
	mInitPt.y = 0;

	mStartPt.x = 0;
	mStartPt.y = 0;

	mNowPt.x = 0;
	mNowPt.y = 0;

	m_points[0].x = 10;
	m_points[0].y = 10;
	m_points[1].x = 400;
	m_points[1].y = 10;
	m_points[2].x = 400;
	m_points[2].y = 100;
	m_points[3].x = 10;
	m_points[3].y = 100;


	m_nowMultiShow = 1.0;
	   m_fileName=fileName; //�ļ�����������FilePathName��
	   m_ptsName = m_fileName;

	   m_srcImg = cv::imread(m_fileName.GetBuffer(0));
	   m_nowImg = m_srcImg.clone();
	   Invalidate();
	
		int index = m_ptsName.Replace(".jpg",".pts");
		if(index == 0)
		{
			index = m_ptsName.Replace(".png",".pts");
			if(index == 0)
			{
				index = m_ptsName.Replace(".bmp",".pts");
			}
		}

		ifstream locations(m_ptsName.GetBuffer(0), ios_base::in);
		if(locations.is_open())
		{
			string line;
			// The main file contains the references to other files
			while (!locations.eof())
			{ 
				
				for(int i=0;i<4;i++)
				{
					locations>>m_points[i].x;
					locations>>m_points[i].y;
				}
				getline(locations, line);
			}
		 
			
		}
		

}
void CBezierDlg::OnBnClickedBtnImport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePathName;
    CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("pic Files (*.jpg)|*.jpg|(*.bmp)|*.bmp|(*.png)|*.png|All Files (*.*)|*.*||"),
        NULL);
    if(dlg.DoModal()==IDOK)
    {
		DoGetPtsAndDraw(dlg.GetPathName());
		
	}
}


void CBezierDlg::OnBnClickedBtnSave()
{

		char szBuf[256];
		sprintf(szBuf, "%s", m_ptsName.GetBuffer(0));
		fstream locations(m_ptsName.GetBuffer(0), ios_base::out);
		for (int i = 0; i < m_points.size(); i++)
		{
			locations << m_points[i].x << " " << m_points[i].y << "\n";
		}
		//locations << "}" << "\n";



}


BOOL CBezierDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!m_leftBtnDown)
	{
		if(m_bKeyOper)
		{
			m_bKeyOper = false;return 1;
		}
		else
		{
			return CDialog::OnEraseBkgnd(pDC);
		}
	}
	else
	{
		return 1;
	}
}



void CBezierDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nID == 61488)
	{
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(0,0,cx,cy);

		GetDlgItem(IDC_BTN_ORIGIN)->MoveWindow(cx-240,5,110,40);
		GetDlgItem(IDC_BTN_BIG)->MoveWindow(cx-120,5,110,40);

		GetDlgItem(IDC_STATIC_PIC1)->MoveWindow(cx-240,55,240,2);
		GetDlgItem(IDC_BTN_SAVE)->MoveWindow(cx-120,60,110,40);
		GetDlgItem(IDC_STATIC_PIC)->MoveWindow(cx-240,115,240,2);


	
		Invalidate();
	}
	CDialog::OnSysCommand(nID, lParam);
}



BOOL CBezierDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == 0x53&& 
			(pMsg->lParam == 0x401f0001
				|| pMsg->lParam == 0x1f0001))
		{
			//AfxMessageBox("ctrl s");
			OnBnClickedBtnSave();
		}
		char szBuf[256];
		sprintf(szBuf, "%x %x\n", pMsg->wParam, pMsg->lParam);
		OutputDebugString(szBuf);
	}
	if(pMsg->message == WM_KEYUP)
	{
			int span = 1;
			switch (pMsg->wParam)
			{
			case VK_UP:
				{
					if(m_currentMark > 0)
					{
						m_points[m_currentMark].y-=span;
						m_bKeyOper = true;
						RedrawWindow();
					}
				}
				break;
			case VK_DOWN:
				if(m_currentMark > 0)
					{
						m_points[m_currentMark].y+=span;
						m_bKeyOper = true;
						RedrawWindow();
					}
				break;
			case VK_RIGHT:
				if(m_currentMark > 0)
					{
						m_points[m_currentMark].x+=span;
						m_bKeyOper = true;
						RedrawWindow();
					}
				break;
			case VK_LEFT:
				if(m_currentMark > 0)
					{
						m_points[m_currentMark].x-=span;
						m_bKeyOper = true;
						RedrawWindow();
					}
			
				break;
			default:
				break;
			}
			

		
		
	}

	if(pMsg->wParam == VK_UP
				|| pMsg->wParam == VK_DOWN
				|| pMsg->wParam == VK_LEFT
				|| pMsg->wParam == VK_RIGHT)
			{
				if(m_currentMark > 0)
				{
					return 1;
				}
			}

	return CDialog::PreTranslateMessage(pMsg);
}






void CBezierDlg::DrawThePicPoint(cv::Mat clmResult, int posX, int posY, int dstW)
{

	//��ʹ��ǰ�����ԭ�����Ƶ�ͼƬȥ��������ɾȥ����������
	CDC *pDC = GetDC();

	int biaoWidht = dstW;
	cv::Mat dst;
	if (clmResult.cols > dstW)
	{
		float ratio = clmResult.cols*1.0 / biaoWidht;
		int height = clmResult.rows / ratio;

		resize(clmResult, dst, cv::Size(biaoWidht, height));
	}
	else
	{
		dst = clmResult.clone();
	}




	CImage Image;
	Image.Create(dst.cols, dst.rows, 24);
	int i;
	int j;
	for (i = 0; i < Image.GetHeight(); i++)
	{
		int step = dst.step[0] * i;
		int jump = 0;
		for (j = 0; j < Image.GetWidth(); j++)
		{
			byte *pByte = (byte *)Image.GetPixelAddress(j, i);

			if (dst.channels() != 1)
			{
				pByte[0] = (unsigned char)dst.data[step + jump + 0];
				pByte[1] = (unsigned char)dst.data[step + jump + 1];
				pByte[2] = (unsigned char)dst.data[step + jump + 2];

				jump += 3;
			}
			else
			{
				pByte[0] = (unsigned char)dst.data[step + jump + 0];
				pByte[1] = (unsigned char)dst.data[step + jump + 0];
				pByte[2] = (unsigned char)dst.data[step + jump + 0];

				jump += 1;
			}

		}
	}

	Image.Draw(pDC->m_hDC, posX, posY);
	Image.Destroy();

	ReleaseDC(pDC);
}
void CBezierDlg::OnBnClickedButtonIn()
{

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePathName;
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("pic Files (*.jpg)|*.jpg|(*.bmp)|*.bmp|(*.png)|*.png|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		DoGetPtsAndDraw(dlg.GetPathName());
	
	}
}




void CBezierDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);

	CRect rect;
	GetWindowRect(rect);

	
	// TODO: �ڴ˴������Ϣ����������
}





int truncateUint8(int value)
{
	if (value < 0) value = 0;
	if (value > 255) value = 255;
	return value;
}

void CBezierDlg::ParseFiles(CString foldername, std::vector<std::string>& outfiles)
{

	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy(szFind, foldername);
	strcat(szFind, "\\*.*");
	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return;

	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{

			}
		}
		else
		{
			CString strTmp = foldername + "/";
			strTmp += FindFileData.cFileName;
			if(strTmp.Find(".jpg") != -1)
			outfiles.push_back(strTmp.GetBuffer(0));
		}


		if (!FindNextFile(hFind, &FindFileData))
			break;
		//
	}

	FindClose(hFind);
	
}


void CBezierDlg::OnBnClickedBtnFolder()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("������ѡ�ļ���Ŀ¼:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);

	//�����ļ�������ļ�
	vector<std::string> files;
	ParseFiles(szBuffer, files);

	mListBox.ResetContent();
	for (int k = 0; k < files.size(); ++k)
	{
		mListBox.AddString(files[k].c_str());
	}
}


void CBezierDlg::OnLbnSelchangeList1()
{
	int iCurSel = mListBox.GetCurSel();
	//�õ���ǰѡ������
	if (LB_ERR != iCurSel)
	{
		CString strTmp;
		mListBox.GetText(iCurSel, strTmp);

		//ѡ��֮��һ�й�0
		DoGetPtsAndDraw(strTmp);

		//�õ���ǰѡ����ַ���
		SetWindowText(strTmp);
	}
									
	
}


void CBezierDlg::OnBnClickedBtnDel()
{
	int iCurSel = mListBox.GetCurSel();
	//�õ���ǰѡ������
	if (LB_ERR != iCurSel)
	{
		CString strTmp;
		mListBox.GetText(iCurSel, strTmp);
		DeleteFile(strTmp);
		mListBox.DeleteString(iCurSel);
	}
}


BOOL CBezierDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (zDelta > 0)
	{
		m_nowMultiShow += 0.1;
	}
	else
	{
		m_nowMultiShow -= 0.1;
		if (m_nowMultiShow < 1) m_nowMultiShow = 1.0;

	}
	
	RedrawWindow();
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
