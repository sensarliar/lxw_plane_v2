// testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
//#include <math.h>
#include "dbw_rd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
////////////////////////////////////


//#include <inttypes.h>
#include <string.h>



#define FLAG_FDJ_BIT 0
#define FLAG_WQ_BIT 1
#define FLAG_QLJ_BIT 2
#define FLAG_JY_BIT 5


struct DbwInfo dbw_info;
char dbw_info_flag = 0x39;
		//now addr_   uint8_t INFO_ADDR[4]={0x04,0xE6,0x10};//291822  addr 3B
char INFO_ADDR[4] = {0x04,0x73,0xEE,0x0};
char RCV_CMD[255];
int cmd_rcv_flag;
int cmd_ch_num;




void parse_dbw_info_GM002(void);


void dbw_info_impl_init( void ) {
  dbw_info.msg_available = FALSE;
//  dbw_info.pos_available = FALSE;
//  dbw_info.gps_nb_ovrn = 0;
  dbw_info.msg_len = 0;
	dbw_info.set_len = DBW_MAXLEN-1;
	dbw_info.set_temp_len = DBW_MAXLEN-1;
}



/**
 * parse GPGGA-nmea-messages stored in
 * dbw_info.msg_buf .
 */
void parse_dbw_info_GM002(void) {
  int i = 7;     // current position in the message, start after: GPGGA,
	int j = 0;
	int temp_len = 255;


		//now addr_   uint8_t INFO_ADDR[4];//291822  addr 3B
	if(dbw_info.msg_buf[i] != INFO_ADDR[0])
	{
	return;
	}
	i++;
		if(dbw_info.msg_buf[i] != INFO_ADDR[1])
	{
	return;
	}
		i++;
		if(dbw_info.msg_buf[i] != INFO_ADDR[2])
	{
	return;
	}
	
///crc error	
	
	if(dbw_info.msg_buf[dbw_info.msg_len-2] != 0)
	{

    return;
	}
//	i+=6;
	i=16;
	temp_len = ((dbw_info.msg_buf[i] << 8) + dbw_info.msg_buf[i+1])/8;
	if(temp_len > dbw_info.msg_len-1-20 +1)
	{
		return;
	}
	//form 18 0xA4;
	for(j=0;j<(dbw_info.msg_len-2-19);j++)
	{
		RCV_CMD[j] = dbw_info.msg_buf[19+j];
	}
	RCV_CMD[j]='\0';
	cmd_ch_num=j;
	cmd_rcv_flag = 1;


	

//END parse GGA
}


/**
 * parse_nmea_char() has a complete line.
 * Find out what type of message it is and
 * hand it to the parser for that type.
 */
void dbw_info_parse_msg( void ) {

//  if(dbw_info.msg_len > 5 && !strncmp(dbw_info.msg_buf , "GMING", 5)) {
	if(dbw_info.msg_len > 5 && !strncmp(dbw_info.msg_buf , "$TXXX", 5)) {
    dbw_info.msg_buf[dbw_info.msg_len] = 0;

		parse_dbw_info_GM002();
  //  parse_nmea_GPRMC();
  }
  else {
    
        dbw_info.msg_buf[dbw_info.msg_len] = 0;

      }

  // reset message-buffer
  dbw_info.msg_len = 0;
	dbw_info.set_len = DBW_MAXLEN-1;
	dbw_info.set_temp_len = DBW_MAXLEN-1;
}
/**
 * This is the actual parser.
 * It reads one character at a time
 * setting dbw_info.msg_available to TRUE
 * after a full line.
 */
void dbw_info_parse_char( char c ) {
  //reject empty lines


  // fill the buffer, unless it's full
  if (dbw_info.msg_len < DBW_MAXLEN - 1) {

    // messages end with a linefeed
    //AD: TRUNK:       if (c == '\r' || c == '\n')
    if (dbw_info.msg_len < dbw_info.set_len-1) {
			dbw_info.msg_buf[dbw_info.msg_len] = c;
      dbw_info.msg_len ++;
    } else {
			dbw_info.msg_buf[dbw_info.msg_len] = c;
      dbw_info.msg_len ++;
			dbw_info.msg_available = TRUE;
    }
		if(dbw_info.msg_len==6)
		{
			dbw_info.set_temp_len = c;
		}
		if(dbw_info.msg_len==7)
		{
			dbw_info.set_temp_len = (dbw_info.set_temp_len << 8) + c;
			if((dbw_info.set_temp_len>=11)&&(dbw_info.set_temp_len<=230))
			{
				dbw_info.set_len = dbw_info.set_temp_len;
			}
		}
		if ( c == '$')
		{
      dbw_info.msg_buf[0] = c;
      dbw_info.msg_len =1;   
			dbw_info.set_len = DBW_MAXLEN-1;
			dbw_info.set_temp_len = DBW_MAXLEN-1;
		}

  }

  if (dbw_info.msg_len >= DBW_MAXLEN - 1)
  {
	   dbw_info.msg_available = TRUE;
  }
}




/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_brush.CreateSolidBrush(RGB(255,255,255));
	//m_brush.CreateSolidBrush(TRANSPARENT);
	
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_BTN_DEV3, m_button_dev3);
	DDX_Control(pDX, IDC_BTN_DEV2, m_button_dev2);
	DDX_Control(pDX, IDC_BTN_DEV1, m_button_dev1);
	DDX_Control(pDX, IDC_BUTTON1, m_com1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnOpen)
//	ON_BN_CLICKED(IDC_BUTTON2, OnReceive)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
	//ON_MESSAGE(ON_COM_RECEIVE, OnRecvComData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString("COM0");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString("COM1");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString("COM2");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString("COM3");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString("COM4");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString("COM5");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString("COM6");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString("COM7");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString("COM8");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString("COM9");
	
	

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	Bytes = 0;
	
	// TODO: Add extra initialization here
	//InintCom(2);
	//m_com.write("TXXXX");
	//m_com.write("m_shuju=$GPZDA,032232.00,14,05,2009,00,00*67,$GPZDA");
	//OnRecvComData();

	SetDlgItemText(IDC_JIHAO,"0007");
	SetDlgItemText(IDC_FDJ1,"1");
	SetDlgItemText(IDC_FDJ2,"0");
	
	m_bitmap_dev1.LoadBitmap(IDB_BITMAP_CLOSE);
	m_button_dev1.SetBitmap(m_bitmap_dev1);
	
	
	m_bitmap_dev2.LoadBitmap(IDB_BITMAP_CLOSE);
	m_button_dev2.SetBitmap(m_bitmap_dev2);

	m_bitmap_dev3.LoadBitmap(IDB_BITMAP_CLOSE);
	m_button_dev3.SetBitmap(m_bitmap_dev3);

	dbw_info_impl_init();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint() 
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

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		/*CRect rect;
		CPaintDC dc(this);
		GetClientRect(rect);
		dc.FillSolidRect(rect,RGB(30,144,255));*/
		
		CDialog::OnPaint();
	}
		//OnRecvComData();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

bool CTestDlg::InintCom(int port)
{
	
	if(!m_com.open(port))//打开串口
	{
		MessageBox("串口打开失败", "COM15", MB_OK);
		return false;
	}
	
	else{
		
		m_com.set_hwnd(m_hWnd);//设置窗口句柄
		((CComboBox*)GetDlgItem(IDC_COMBO5))->EnableWindow(FALSE);

	}
		
	return true;
}


char receive_buf[1024] = {0};

//char receive_buf_temp[1024] = {0};
char * recv_buf_p=receive_buf;
char dev_state=0x80;


BOOL CTestDlg::OnRecvComData()
{
	int i;
	// TODO: Add your control notification handler code here
	int bytes_temp ;
	bytes_temp = m_com.read((char *)&receive_buf, 1024);
	if(bytes_temp <= 0)
	{
		return false;
	}
/*
//	i=2;
//	bytes_temp-=2;
	i=0;
	while(bytes_temp--)
	{
		receive_buf_temp[i] = receive_buf[i];
		i++;
	}
	receive_buf_temp[i]= '\0';
*/
	dev_state=receive_buf[2];

	if(bytes_temp>3)
	recv_buf_p=&(receive_buf[3]);
	GetDlgItem(IDC_RECEIVE)->SetWindowText(recv_buf_p);

	if(dev_state&0x1)
	{
	m_bitmap_dev1.Detach();
	m_bitmap_dev1.LoadBitmap(IDB_BITMAP_OPEN);
	m_button_dev1.SetBitmap(m_bitmap_dev1);
	}else
	{
	m_bitmap_dev1.Detach();
	m_bitmap_dev1.LoadBitmap(IDB_BITMAP_CLOSE);
	m_button_dev1.SetBitmap(m_bitmap_dev1);
	}

	if(dev_state&0x2)
	{
	m_bitmap_dev2.Detach();
	m_bitmap_dev2.LoadBitmap(IDB_BITMAP_OPEN);
	m_button_dev2.SetBitmap(m_bitmap_dev2);
	}else
	{
	m_bitmap_dev2.Detach();
	m_bitmap_dev2.LoadBitmap(IDB_BITMAP_CLOSE);
	m_button_dev2.SetBitmap(m_bitmap_dev2);
	}

	if(dev_state&0x4)
	{
	m_bitmap_dev3.Detach();
	m_bitmap_dev3.LoadBitmap(IDB_BITMAP_OPEN);
	m_button_dev3.SetBitmap(m_bitmap_dev3);
	}else
	{
	m_bitmap_dev3.Detach();
	m_bitmap_dev3.LoadBitmap(IDB_BITMAP_CLOSE);
	m_button_dev3.SetBitmap(m_bitmap_dev3);
	}
	
//	GetDlgItem(IDC_RECEIVE)->SetWindowText(receive_buf+1);

	return true;
}

/*BOOL CTestDlg::OnRecvComData()
{
	int bytes_temp = 1;
	bytes_temp = m_com.read((char *)&receive_buf, 1024);
//	Bytes += bytes_temp;
	if(bytes_temp <= 0)
	{
		return false;
	}
	int i=0;
	bytes_temp++;
	while((bytes_temp--)&&!dbw_info.msg_available)
	{

		if(bytes_temp > 0)
		{
			dbw_info_parse_char(receive_buf[i++]);
		}
		
	}                                            
    if (dbw_info.msg_available) {                     
      dbw_info_parse_msg();                                 
      dbw_info.msg_available = FALSE;                   
    }   
	//显示
	if(cmd_rcv_flag)
	{
	

		strBuffer=RCV_CMD;

		
		
		SetDlgItemText(IDC_Year,m_year);
						
	//#endif				
			strBuffer.Empty();

			cmd_rcv_flag = 0;

	}

	return true;
}*/

int timer_cnt_60ms=20;
float angle_roll=0;
float angle_pitch=0;
double angle_yaw=0;
char angle_buff[50];
char * angle_p;

int timer_cnt_30s = 500;

void CTestDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		OnRecvComData();

		if(timer_cnt_30s>0)
		{
			timer_cnt_30s--;
			if(timer_cnt_30s==0)
			{
				timer_cnt_30s = 500;
				CTestDlg::OnSend();
			//	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
			}
		}

		if(timer_cnt_60ms>0)
		{
			timer_cnt_60ms--;
			if(timer_cnt_60ms==0)
			{
				timer_cnt_60ms = 40;
		//		angle_roll=((int)(rand()*2000/0x7fff))/100;
		//		angle_pitch=((int)(rand()*1000/0x7fff))/100;
		//		angle_yaw=((int)(rand()*9000/RAND_MAX))/100;
		//		angle_yaw=rand()/RAND_MAX;
				angle_roll=0;
				while(angle_roll<1)
				{
				angle_roll=float(int(rand()*2000/RAND_MAX))/100;
				}
				
				angle_p = gcvt(angle_roll,4,angle_buff);
				SetDlgItemText(IDC_ROLL,angle_p);
				
				//angle_pitch=random();
				angle_pitch=0;
				while(angle_pitch<1)
				{
				angle_pitch=float(int(rand()*1000/RAND_MAX))/100;
				}

				angle_p = gcvt(angle_pitch,4,angle_buff);
				SetDlgItemText(IDC_PITCH,angle_p);

				//angle_yaw = float(rand())/RAND_MAX;
				angle_yaw=0;
				while(angle_yaw<1)
				{
				angle_yaw=float(int(rand()*9000/RAND_MAX))/100;
				}

				angle_p = gcvt(angle_yaw,4,angle_buff);
				SetDlgItemText(IDC_YAW,angle_p);
			}
		}
		
	}

	CDialog::OnTimer(nIDEvent);
}


void CTestDlg::OnOpen() 
{
	// TODO: Add your control notification handler code here
	
	
	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO5))->GetCurSel();
	
	InintCom(nIndex);
	SetTimer(1,60,0);



}

/*

void CTestDlg::OnReceive() 
{
	// TODO: Add your control notification handler code here
	int bytes_temp ;
	bytes_temp = m_com.read((char *)&receive_buf, 1024);
	if(bytes_temp <= 0)
	{
		return;
	}
	
	GetDlgItem(IDC_RECEIVE)->SetWindowText(receive_buf);

}

*/



void CTestDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
		//显示"$GMING,feijihao,1,0,1,1,1,0,1,1,1,1,11,"
	char str_tx[1024]="$GMING,";
	char *str_p;
    CString text;
	int text_len = 0;
	GetDlgItem(IDC_JIHAO)->GetWindowText(text);
	
	str_p=&str_tx[7];
	
	int i = 0;
	text_len = text.GetLength();
	while(text_len--)
	{
		*str_p=text.GetAt(i);
		str_p++;
		i++;
	}
	*str_p = ',';
	str_p++;

	GetDlgItem(IDC_FDJ1)->GetWindowText(text);
	i = 0;
	text_len = text.GetLength();
	while(text_len--)
	{
		*str_p=text.GetAt(i);
		str_p++;
		i++;
	}
	*str_p = ',';
	str_p++;

	GetDlgItem(IDC_FDJ2)->GetWindowText(text);
	i = 0;
	text_len = text.GetLength();
	while(text_len--)
	{
		*str_p=text.GetAt(i);
		str_p++;
		i++;
	}
	*str_p = ',';
	str_p++;

	GetDlgItem(IDC_ROLL)->GetWindowText(text);
	i = 0;
	text_len = text.GetLength();
	while(text_len--)
	{
		*str_p=text.GetAt(i);
		str_p++;
		i++;
	}
	*str_p = ',';
	str_p++;

	GetDlgItem(IDC_PITCH)->GetWindowText(text);
	i = 0;
	text_len = text.GetLength();
	while(text_len--)
	{
		*str_p=text.GetAt(i);
		str_p++;
		i++;
	}
	*str_p = ',';
	str_p++;

	GetDlgItem(IDC_YAW)->GetWindowText(text);
	i = 0;
	text_len = text.GetLength();
	while(text_len--)
	{
		*str_p=text.GetAt(i);
		str_p++;
		i++;
	}
	*str_p = ',';
	str_p++;

	*str_p = dev_state;
	str_p++;

	*str_p = ',';
	str_p++;

	*str_p = 0x0d;
	str_p++;
	*str_p = 0x0a;
	str_p++;
	*str_p = '\0';

	m_com.write(str_tx);
}
