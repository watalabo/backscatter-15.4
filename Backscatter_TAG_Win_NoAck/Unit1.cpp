//---------------------------------------------------------------------------
#include	<vcl.h>
#include	<stdio.h>
#include	<Windows.h>
#include	<ustring.h>
#include	<time.h>
#include	<math.h>
#include	<dirent.h>
#include	<sys/stat.h>
#include	<locale.h>

#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
// Char Code
//---------------------------------------------------------------------------
char    CharCode[] = {
//   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, // 0
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, // 1
	 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, // 2
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, // 3
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, // 4
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, // 5
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, // 6
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0  // 7
};

//---------------------------------------------------------------------------
// Hex�e�[�u��
//---------------------------------------------------------------------------
char	HexInt[] = {
//       0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	// 0
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, // 1
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, // 2
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0, // 3
	 0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0, // 4
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	// 5
	 0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0, // 6
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0	// 7
};

//--------------------------------------------------------------------------
// ���ʕϐ�
//--------------------------------------------------------------------------
HANDLE  Com_CW_Tx  = INVALID_HANDLE_VALUE;
HANDLE  Com_Bk_Rx  = INVALID_HANDLE_VALUE;
HANDLE  Com_Co_Tx  = INVALID_HANDLE_VALUE;
char	COMlist[32][64];
#define	USB_BUF_SZ	4096
unsigned char	USB_RxBuf[USB_BUF_SZ]; 	// USB��M�o�b�t�@
unsigned char	USB_RxBufS[USB_BUF_SZ]; 	// USB��M�o�b�t�@
		char	ustr[1024], ubuf[1024];
		long	InitAlarmTime;

//--------------------------------------------------------------------------
// Event���s
//--------------------------------------------------------------------------
void DoEvents()
{
	int	k;
	MSG	msg;

	for(k = 0; ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0 && k < 100; k++) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

//--------------------------------------------------------------------------
// USB�I�[�v��
//--------------------------------------------------------------------------
HANDLE Open_USB(char *ComNam, int Speed)
{
static	HANDLE	hCom;
	int	k;
	char	str[256];
	wchar_t	nam[256];
	DCB	dcb;

	sprintf(str, "\\\\.\\%s", ComNam);
	mbstowcs(nam, str, sizeof(str));
	// �V���A���|�[�g���J����
	hCom = CreateFile(
		nam,							// �V���A���|�[�g�̕�����
		GENERIC_READ | GENERIC_WRITE,	// �A�N�Z�X���[�h�F�ǂݏ���
		0,								// ���L���[�h�F������̓A�N�Z�X�s��
		NULL,							// �Z�L�����e�B�����F�n���h���p������
		OPEN_EXISTING,					// �쐬�t���O
		FILE_ATTRIBUTE_NORMAL,			// ����
		NULL								// �e���v���[�g�̃n���h��
		);
	if(hCom == INVALID_HANDLE_VALUE) {
		return(INVALID_HANDLE_VALUE);
	}
	PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	// �ʐM������ݒ肷��
	GetCommState(hCom, &dcb);			// DCB ���擾
	// �f�[�^
	dcb.BaudRate = Speed;
	dcb.fBinary = TRUE;					// �o�C�i�����[�h, EOF(EofChar�Ŏw��)���������Ȃ�
	dcb.ByteSize = 8;
	dcb.fParity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	// �n�[�h�E�G�A�E�t���[����
	dcb.fOutxCtsFlow = FALSE;			// CTS�n�[�h�E�F�A�t���[����FCTS������g�p���Ȃ��ꍇ��FLASE���w��
										// CTS ���������ꍇ��TRUE���w�肵��CTS�M�����Ď����܂�
	dcb.fOutxDsrFlow = FALSE;			// DSR�n�[�h�E�F�A�t���[����F�g�p���Ȃ��ꍇ��FALSE���w��
	dcb.fDtrControl = DTR_CONTROL_DISABLE;	// DTR�L��/�����F�����Ȃ�@DTR_CONTROL_DISABLE;ISABLE
	dcb.fRtsControl = RTS_CONTROL_DISABLE;	// RTS����FRTS��������Ȃ��ꍇ��RTS_CONTROL_DISABLE���w��
	// �\�t�g�E�F�A�t���[����
	dcb.fOutX = FALSE;					// ���M��XON/OFF����̗L��
	dcb.fInX = FALSE;					// ��M��XON/XOFF����̗L��
	dcb.fTXContinueOnXoff = FALSE;		// ��M�o�b�t�@�[���t��XOFF��M��̌p�����M��
	dcb.XonLim = 512;					// XON��������܂łɊi�[�ł���ŏ��o�C�g���F
	dcb.XoffLim = 512;					// XOFF��������܂łɊi�[�ł���ŏ��o�C�g���F
	dcb.XonChar = 0x11;					// ���M��XON���� ( ���M�F�r�W�B���� ) �̎w��F
	// ��ʂɁAXON�����Ƃ���11H ( �f�o�C�X����P�FDC1 )���悭�g���܂�
	dcb.XoffChar = 0x13;				// XOFF�����i���M�s�F�r�W�[�ʍ��j�̎w��F�Ȃ���FALSE
										// ��ʂɁAXOFF�����Ƃ���13H ( �f�o�C�X����3�FDC3 )���悭�g���܂�
	//���̑�
	dcb.fNull = FALSE;					// NULL�o�C�g�̔j���F�j�����遨TRUE
	dcb.fAbortOnError = TRUE;			// �G���[���̓ǂݏ�������I���F�I�����遨TRUE
	dcb.fErrorChar = FALSE;				// �p���e�B�G���[�������̃L�����N�^(ErrorChar)�u���F�Ȃ���FLALSE
	dcb.ErrorChar = 0x00;				// �p���e�B�G���[�������̒u���L�����N�^
	dcb.EofChar = 0x03;					// �f�[�^�I���ʒm�L�����N�^�F��ʂ�0x03(ETX)���悭�g���܂��B
	dcb.EvtChar = 0x02;					// �C�x���g�ʒm�L�����N�^�F��ʂ�0x02(STX)���悭�g���܂�
	SetCommState(hCom, &dcb); 			// DCB ��ݒ�
	Sleep(100);
	return(hCom);
}

//---------------------------------------------------------------------------
// USB�ɑ��M
//---------------------------------------------------------------------------
void USB_Tx(HANDLE hCom1, unsigned char *str, int len)
{
	DWORD	dwWritten; 	// �|�[�g�֏������񂾃o�C�g��

	if(hCom1 == INVALID_HANDLE_VALUE) return;

	WriteFile(hCom1, str, len, &dwWritten, NULL);
	if((int)dwWritten != len)  {
		CloseHandle(hCom1);
		hCom1 = INVALID_HANDLE_VALUE;
		return;
	}
	FlushFileBuffers(hCom1);
}

//---------------------------------------------------------------------------
// USB�����M
//---------------------------------------------------------------------------
int USB_Rx(HANDLE hCom1, int Wait)
{
	int	k;
	DWORD	dwErrors;	// �G���[���
	COMSTAT	ComStat;	// �f�o�C�X�̏��
static	DWORD	dwCount;	// ��M�f�[�^�̃o�C�g��
	DWORD	dwRead;		// �|�[�g����ǂݏo�����o�C�g��

	OVERLAPPED ovlpd;

	if(hCom1 == INVALID_HANDLE_VALUE) return(-1);

	for(k = 0; k < USB_BUF_SZ; k++) USB_RxBuf[k] = 0;
	for(k = 0; k < 32; k++) {
		ovlpd.Offset = 0;
		ovlpd.OffsetHigh = 0;
		ovlpd.hEvent = NULL;
		dwCount = 0;
		ClearCommError(hCom1, &dwErrors, &ComStat);
		dwCount = ComStat.cbInQue;
		if(dwCount >= (unsigned int)Wait) {
			if(dwCount > USB_BUF_SZ) {
				ShowMessage("ERROR:USB_Rx Over");
				CloseHandle(hCom1);
				hCom1 = INVALID_HANDLE_VALUE;
				return(-1);
			}
			ReadFile(hCom1, USB_RxBuf, dwCount, &dwRead, &ovlpd);
			if(dwCount != dwRead) {
				ShowMessage("ERROR:USB_Rx Receive");
				CloseHandle(hCom1);
				hCom1 = INVALID_HANDLE_VALUE;
				return(-1);
			}
			USB_RxBuf[dwCount] = 0;
			return(dwCount);
		}
		Sleep(10);
	}
	if(dwCount != 0) {
		ReadFile(hCom1, USB_RxBuf, dwCount, &dwRead, &ovlpd);
		if(dwCount != dwRead) {
			ShowMessage("ERROR:USB_Rx Receive");
			CloseHandle(hCom1);
			hCom1 = INVALID_HANDLE_VALUE;
			return(-1);
		}
		USB_RxBuf[dwCount] = 0;
		return(dwCount);
	}
	return(-1);
}

int USB_RxS(HANDLE hCom1)
{
		int		k;
		DWORD	dwErrors;	// �G���[���
		COMSTAT	ComStat;	// �f�o�C�X�̏��
static	DWORD	dwCount;	// ��M�f�[�^�̃o�C�g��
		DWORD	dwRead;		// �|�[�g����ǂݏo�����o�C�g��

		OVERLAPPED ovlpd;

	if(hCom1 == INVALID_HANDLE_VALUE) return(-1);


	ovlpd.Offset = 0;
	ovlpd.OffsetHigh = 0;
	ovlpd.hEvent = NULL;
	dwCount = 0;
	ClearCommError(hCom1, &dwErrors, &ComStat);
	dwCount = ComStat.cbInQue;
	if(dwCount > USB_BUF_SZ) {
		ShowMessage("ERROR:USB_Rx Over");
		CloseHandle(hCom1);
		hCom1 = INVALID_HANDLE_VALUE;
		return(-1);
	}

	USB_RxBufS[0] = 0;
	ReadFile(hCom1, USB_RxBufS, dwCount, &dwRead, &ovlpd);
	if(dwCount != dwRead) {
		ShowMessage("ERROR:USB_Rx Receive");
		CloseHandle(hCom1);
		hCom1 = INVALID_HANDLE_VALUE;
		return(-1);
	}
	USB_RxBufS[dwCount] = 0;
	return(dwCount);
}

//--------------------------------------------------------------------------
// PC�ɐڑ�����Ă���COM�|�[�g���A�f�o�C�X���𓾂�
//--------------------------------------------------------------------------
void get_COM_port()
{
	HKEY	hKey = NULL;
	DWORD	dwCount = 0;
	TCHAR	cNameBuff[256];
unsigned char	byValueBuff[256];
	DWORD	dwNameBuffSize = 0;
	DWORD	dwValueBuffSize = 0;
	DWORD	dwType = 0;
	wchar_t	wbuf[256];
	char	buf[256];
	int	k;

	for(k = 0; k < 16; k++) COMlist[k][0] = 0;
	mbstowcs(&wbuf[0], "HARDWARE\\DEVICEMAP\\SERIALCOMM",  31);
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, wbuf, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
		return;
	}
	if(RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL,
				&dwCount, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
		return;
	}
	for(k = 0; k < (int)dwCount && k < 32; k++) {
		dwNameBuffSize = sizeof(cNameBuff);
		dwValueBuffSize = sizeof(byValueBuff);
		if(RegEnumValue(hKey, k, &cNameBuff[0], &dwNameBuffSize, NULL,
			&dwType, &byValueBuff[0], &dwValueBuffSize) != ERROR_SUCCESS ) {
			ShowMessage("Error�F[RegEnumValue()]");
			return;
		}
		wcstombs(&buf[0], (wchar_t *)byValueBuff, 256);
		sprintf(COMlist[k], "%s:", buf);
		wcstombs(&buf[0], (wchar_t *)cNameBuff, 256);
		strcat(COMlist[k], buf);
	}
}

//--------------------------------------------------------------------------
// VCP�|�[�g����
//--------------------------------------------------------------------------
void GetVcpPoart(TObject *Sender)
{
	int	k;
	wchar_t	wbuf[1000];

	get_COM_port();
	Form1->CW_Tx_Port->Items->Clear();
	Form1->Backscatter_Rx_Port->Items->Clear();
	Form1->Cont_Tx_Port->Items->Clear();
	for(k = 0; k < 32 && COMlist[k][0] != 0; k++) {
		Form1->CW_Tx_Port->Items->Add(COMlist[k]);
		Form1->Backscatter_Rx_Port->Items->Add(COMlist[k]);
		Form1->Cont_Tx_Port->Items->Add(COMlist[k]);
	}
}

//---------------------------------------------------------------------------
// USB ����M
//---------------------------------------------------------------------------
char * USB_tr(HANDLE hCom1, char *TxStr, int RxWait)
{
	int		n;

	USB_Tx(hCom1, TxStr, strlen(TxStr));
	if(hCom1 == INVALID_HANDLE_VALUE) {
		CloseHandle(hCom1);
		hCom1 == INVALID_HANDLE_VALUE;
		return("ERROR USB_tr");
	}
	n = USB_Rx(hCom1, RxWait);
	USB_RxBuf[n - 2] = 0;
	if(hCom1 == INVALID_HANDLE_VALUE) {
		CloseHandle(hCom1);
		hCom1 == INVALID_HANDLE_VALUE;
		return("ERROR USB_tr");
	}
	return((char *)USB_RxBuf);
}

//---------------------------------------------------------------------------
// Form1 Create
//---------------------------------------------------------------------------
void __fastcall TForm1::F1_Create(TObject *Sender)
{
	int		n;

	//-----------------------------------------------------------------
	// VCP�|�[�g����
	//-----------------------------------------------------------------
	GetVcpPoart(Sender);
	//-----------------------------------------------------------------
	// ComboBox ���ݒ�
	//-----------------------------------------------------------------
	// CW_Tx Interval
	Form1->Interval->Items->Clear();
	for(n =  100; n <   1000; n +=  100) Form1->Interval->Items->Add(n);
	for(n = 1000; n <= 10000; n += 1000) Form1->Interval->Items->Add(n);
	// CW_Tx Step_Freq
	Form1->Step_Freq->Items->Clear();
	for( n = 1; n <= 6; n++) Form1->Step_Freq->Items->Add(n);
	// CW_Tx Step_Num
	Form1->Step_Num->Items->Clear();
	for( n = 1; n <= 30; n++) Form1->Step_Num->Items->Add(n);
	// Alarm Interval
	Form1->AlarmInterval->Items->Clear();
	Form1->AlarmInterval->Items->Add(10);
	Form1->AlarmInterval->Items->Add(30);
	Form1->AlarmInterval->Items->Add(60);
	Form1->AlarmInterval->Items->Add(120);
	Form1->AlarmInterval->Items->Add(180);
	Form1->AlarmInterval->Items->Add(240);
	Form1->AlarmInterval->Items->Add(300);
	Form1->AlarmInterval->Items->Add(600);
	Form1->AlarmInterval->Items->Add(1200);
	Form1->AlarmInterval->Items->Add(1800);
	Form1->AlarmInterval->Items->Add(3600);
	// Init Alarm
	Form1->InitAlarm->Items->Clear();
	Form1->InitAlarm->Items->Add(60);
	Form1->InitAlarm->Items->Add(120);
	Form1->InitAlarm->Items->Add(180);
	Form1->InitAlarm->Items->Add(300);
	Form1->InitAlarm->Items->Add(600);
	Form1->InitAlarm->Items->Add(1800);
	Form1->InitAlarm->Items->Add(3600);
	Form1->InitAlarm->Items->Add(7200);
	Form1->InitAlarm->Items->Add(10080);
	Form1->InitAlarm->Items->Add(18000);
	Form1->InitAlarm->Items->Add(36000);
}

//---------------------------------------------------------------------------
// Exit
//---------------------------------------------------------------------------
void __fastcall TForm1::F1_ExitClick(TObject *Sender)
{
	FILE	*fp;


	if((fp = fopen("SetUp.txt", "w")) == 0) return;
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->CW_Tx_Port->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Backscatter_Rx_Port->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Cont_Tx_Port->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->CW_Tx_Freq->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->CW_Tx_Power->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Backscatter_Rx_Freq->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Step_Freq->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Step_Num->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Interval->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Cont_Tx_Freq->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Cont_Tx_Power->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->SrcPAN->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->SrcMAC->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->AlarmInterval->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag1_MAC->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag1_Alarm->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag2_MAC->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag2_Alarm->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag3_MAC->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag3_Alarm->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag4_MAC->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag4_Alarm->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag5_MAC->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag5_Alarm->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag6_MAC->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag6_Alarm->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag7_MAC->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag7_Alarm->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag8_MAC->Text));
	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->Tag8_Alarm->Text));

	fprintf(fp, "%s\n", (AnsiString)UTF8Encode(Form1->InitAlarm->Text));

	fclose(fp);
	Form1->Close();
}

//---------------------------------------------------------------------------
// �^�C�}�[������
//---------------------------------------------------------------------------
volatile int		Tic = 0;

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	int		n;
	FILE	*fp;

	if(Tic == 0) {
		if((fp = fopen("SetUp.txt", "r")) == 0) return;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->CW_Tx_Port->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Backscatter_Rx_Port->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Cont_Tx_Port->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->CW_Tx_Freq->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->CW_Tx_Power->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Backscatter_Rx_Freq->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Step_Freq->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Step_Num->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Interval->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Cont_Tx_Freq->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Cont_Tx_Power->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->SrcPAN->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->SrcMAC->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->AlarmInterval->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag1_MAC->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag1_Alarm->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag2_MAC->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag2_Alarm->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag3_MAC->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag3_Alarm->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag4_MAC->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag4_Alarm->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag5_MAC->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag5_Alarm->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag6_MAC->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag6_Alarm->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag7_MAC->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag7_Alarm->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag8_MAC->Text = ustr;
		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->Tag8_Alarm->Text = ustr;

		fgets(ustr, 256, fp); for(n = 0; ustr[n] != '\n' && n < 256; n++); ustr[n] = 0;
		Form1->InitAlarm->Text = ustr;

		fclose(fp);
	}
	Tic++;
}

//---------------------------------------------------------------------------
// Backscatter_Rx ��M����
//---------------------------------------------------------------------------
void Open_Com_Co_Tx()
{
	int		k;

	//--------------------------------------------------
	// Poart �ڑ� Com_Co_Tx
	//--------------------------------------------------
	if(Com_Co_Tx == INVALID_HANDLE_VALUE) {
		sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Cont_Tx_Port->Text));
		for(k = 0; ustr[k] != ':' && k < 128; k++); ustr[k] = 0;
		Com_Co_Tx = Open_USB(ustr, 230400);
		if(Com_Co_Tx == INVALID_HANDLE_VALUE) {
			ShowMessage("ERROR COM Port Open[Com_Co_Tx]");
			CloseHandle(Com_Co_Tx);
			Com_Co_Tx = INVALID_HANDLE_VALUE;
			return;
		}
		USB_Tx(Com_Co_Tx, "\n", 1);
		USB_Rx(Com_Co_Tx, 1000);
	}
}

char			Bk_Rx_Buf[4096];
volatile int	Bk_Rx_Buf_P = 0;
volatile int 	iTag1_Count = 0, iTag2_Count = 0, iTag3_Count = 0, iTag4_Count = 0;
volatile int 	iTag5_Count = 0, iTag6_Count = 0, iTag7_Count = 0, iTag8_Count = 0;
volatile int	iInitSleep = 0, iInitFlug = 0;

void Backscatter_RX_Rcv_Proc(int Len)
{
	int		n, m, k, PktLen;
static int	RxCount = 0;
	float	Alarm;
	char	rBuf[128];
unsigned short int DstPAN, DstMAC, SrcPAN, SrcMAC;
	long	NowTime;
	struct	tm	*tm;

	for(n = 0; n < Len; n++) {
		Bk_Rx_Buf[Bk_Rx_Buf_P] =  USB_RxBufS[n];
		Bk_Rx_Buf_P++;
		if(USB_RxBufS[n] == ']') {
			Bk_Rx_Buf[Bk_Rx_Buf_P] = 0;
			Bk_Rx_Buf_P = 0;
// 0         1         2         3         4         5
// 012345678901234567890123456789012345678901234567890123456789012345678901234567890
// [53|41884FFFFFFFFF01BF01BB313231303430363130353531352E3433383820322E39372032382E3
// 52032312E3820313030392E30CD3A]
			if(Bk_Rx_Buf[0] != '[') return;
			RxCount++;
			PktLen = (Bk_Rx_Buf[1] & 0x0F) * 10 + (Bk_Rx_Buf[2] & 0x0F);
			for(k = 0, m = 4; k < PktLen; k++, m += 2) {
				rBuf[k] = HexInt[Bk_Rx_Buf[m] & 0xFF] << 4 | HexInt[Bk_Rx_Buf[m + 1] & 0xFF];
			}
			rBuf[k] = 0;
			DstPAN = rBuf[3] | rBuf[ 4] << 8;
			DstMAC = rBuf[5] | rBuf[ 6] << 8;
			SrcPAN = rBuf[7] | rBuf[ 8] << 8;
			SrcMAC = rBuf[9] | rBuf[10] << 8;
			//strcpy(ubuf, rBuf);
			for(n = 11, m = 0; n < PktLen - 2; n++, m++) {
				if(CharCode[rBuf[n]] == 0) {
					ubuf[m] = '.';
				} else {
					ubuf[m] = rBuf[n];
				}
			}
			ubuf[m] = 0;
			//ubuf[PktLen - 2] = 0;
			if((rBuf[PktLen - 1] & 0x80)  == 0) {
				sprintf(ustr, "<%d><GO>[%d][%04X][%04X][%04X][%04X][%s]",
						RxCount, (signed char)rBuf[PktLen - 2], DstPAN, DstMAC, SrcPAN, SrcMAC, ubuf);
				Form1->Edit2->Text = ustr;
			} else {
				sprintf(ustr, "<%d><NG>[%d][%04X][%04X][%04X][%04X][%s]",
						RxCount, (signed char)rBuf[PktLen - 2], DstPAN, DstMAC, SrcPAN, SrcMAC, ubuf);
				Form1->Edit2->Text = ustr;
				Form1->Edit1->Text = USB_tr(Com_Co_Tx, ustr, 9);
				return;
			}
			//--------------------------------------------------
			// �����ݒ�(�u���[�h�L���X�g)
			//--------------------------------------------------
			if(rBuf[11] == '0') {
				Open_Com_Co_Tx();
				for(n = 0; n < 3; n++) {
					time(&NowTime);
					tm = localtime(&NowTime);
					sprintf(ustr, "one FFFF T%02d%02d%02d%02d%02d%02d\n",
						tm->tm_year - 100, tm->tm_mon + 1, tm->tm_mday,
						tm->tm_hour, tm->tm_min, tm->tm_sec);
					Form1->Edit1->Text = USB_tr(Com_Co_Tx, ustr, 9);
					Sleep(100);
				}
				sprintf(ustr, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
				Form1->InitSetTime->Text = ustr;
				if(NowTime > InitAlarmTime) {
					sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->InitAlarm->Text));
					sscanf(ustr, "%d", &k);
					InitAlarmTime += k;
                    tm = localtime(&InitAlarmTime);
					sprintf(ustr, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
					Form1->InitTime->Text = ustr;
				}
			}
			//--------------------------------------------------
			// Tag1�̏��̕\��
			//--------------------------------------------------
			//[59]=[1210406143017.7626 2.97 29.5 22.1 1006.4]
			sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Tag1_MAC->Text));
			sprintf(ubuf, "%04X", SrcMAC);
			if(strcmp(ustr, ubuf) == 0) {
				sprintf(ustr, "%c%c/%c%c/%c%c:%c%c:%c%c:%c%c%c%c%c%c%c",
							rBuf[12], rBuf[13], rBuf[14], rBuf[15], rBuf[16], rBuf[17],
							rBuf[18], rBuf[19], rBuf[20], rBuf[21],
							rBuf[22], rBuf[23], rBuf[24], rBuf[25], rBuf[26], rBuf[27], rBuf[28]);
				Form1->Tag1_Tim->Text = ustr;
				strcpy(ustr, &rBuf[30]);
				ustr[4] = 0;
				Form1->Tag1_Vbat->Text = ustr;
				strcpy(ustr, &rBuf[35]);
				ustr[4] = 0;
				Form1->Tag1_Temp->Text = ustr;
				strcpy(ustr, &rBuf[40]);
				ustr[4] = 0;
				Form1->Tag1_Hum->Text = ustr;
				strcpy(ustr, &rBuf[45]);
				ustr[6] = 0;
				Form1->Tag1_Pres->Text = ustr;
				sprintf(ustr, "%d",(signed char)rBuf[PktLen - 2]);
				Form1->Tag1_Rssi->Text = ustr;
				iTag1_Count++;
				Form1->Tag1_Count->Text = iTag1_Count;
				//--------------------------------------------------
				// Tag1 �A���[���ݒ�
				//--------------------------------------------------
				if(rBuf[11] == '1') {
					tm = localtime(&InitAlarmTime);
					Open_Com_Co_Tx();
					sprintf(ustr, "one %04X A%s %s %02d%02d%02d\n", SrcMAC,
							(AnsiString)UTF8Encode(Form1->AlarmInterval->Text),
							(AnsiString)UTF8Encode(Form1->Tag1_Alarm->Text),
							tm->tm_hour, tm->tm_min, tm->tm_sec);
					Form1->Edit1->Text = USB_tr(Com_Co_Tx, ustr, 9);
				}
			}
			//--------------------------------------------------
			// Tag2�̏��̕\��
			//--------------------------------------------------
			sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Tag2_MAC->Text));
			if(strcmp(ustr, ubuf) == 0) {
				sprintf(ustr, "%c%c/%c%c/%c%c:%c%c:%c%c:%c%c%c%c%c%c%c",
							rBuf[12], rBuf[13], rBuf[14], rBuf[15], rBuf[16], rBuf[17],
							rBuf[18], rBuf[19], rBuf[20], rBuf[21],
							rBuf[22], rBuf[23], rBuf[24], rBuf[25], rBuf[26], rBuf[27], rBuf[28]);
				Form1->Tag2_Tim->Text = ustr;
				strcpy(ustr, &rBuf[30]);
				ustr[4] = 0;
				Form1->Tag2_Vbat->Text = ustr;
				strcpy(ustr, &rBuf[35]);
				ustr[4] = 0;
				Form1->Tag2_Temp->Text = ustr;
				strcpy(ustr, &rBuf[40]);
				ustr[4] = 0;
				Form1->Tag2_Hum->Text = ustr;
				strcpy(ustr, &rBuf[45]);
				ustr[6] = 0;
				Form1->Tag2_Pres->Text = ustr;
				sprintf(ustr, "%d",(signed char)rBuf[PktLen - 2]);
				Form1->Tag2_Rssi->Text = ustr;
				iTag2_Count++;
				Form1->Tag2_Count->Text = iTag2_Count;
				//--------------------------------------------------
				// Tag2 �A���[���ݒ�
				//--------------------------------------------------
				if(rBuf[11] == '1') {
					tm = localtime(&InitAlarmTime);
					Open_Com_Co_Tx();
					sprintf(ustr, "one %04X A%s %s %02d%02d%02d\n", SrcMAC,
							(AnsiString)UTF8Encode(Form1->AlarmInterval->Text),
							(AnsiString)UTF8Encode(Form1->Tag2_Alarm->Text),
							tm->tm_hour, tm->tm_min, tm->tm_sec);
					Form1->Edit1->Text = USB_tr(Com_Co_Tx, ustr, 9);
				}
			}
			//--------------------------------------------------
			// Tag3�̏��̕\��
			//--------------------------------------------------
			sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Tag3_MAC->Text));
			if(strcmp(ustr, ubuf) == 0) {
				sprintf(ustr, "%c%c/%c%c/%c%c:%c%c:%c%c:%c%c%c%c%c%c%c",
							rBuf[12], rBuf[13], rBuf[14], rBuf[15], rBuf[16], rBuf[17],
							rBuf[18], rBuf[19], rBuf[20], rBuf[21],
							rBuf[22], rBuf[23], rBuf[24], rBuf[25], rBuf[26], rBuf[27], rBuf[28]);
				Form1->Tag3_Tim->Text = ustr;
				strcpy(ustr, &rBuf[30]);
				ustr[4] = 0;
				Form1->Tag3_Vbat->Text = ustr;
				strcpy(ustr, &rBuf[35]);
				ustr[4] = 0;
				Form1->Tag3_Temp->Text = ustr;
				strcpy(ustr, &rBuf[40]);
				ustr[4] = 0;
				Form1->Tag3_Hum->Text = ustr;
				strcpy(ustr, &rBuf[45]);
				ustr[6] = 0;
				Form1->Tag3_Pres->Text = ustr;
				sprintf(ustr, "%d",(signed char)rBuf[PktLen - 2]);
				Form1->Tag3_Rssi->Text = ustr;
				iTag3_Count++;
				Form1->Tag3_Count->Text = iTag3_Count;
				//--------------------------------------------------
				// Tag3 �A���[���ݒ�
				//--------------------------------------------------
				if(rBuf[11] == '1') {
					tm = localtime(&InitAlarmTime);
					Open_Com_Co_Tx();
					sprintf(ustr, "one %04X A%s %s %02d%02d%02d\n", SrcMAC,
							(AnsiString)UTF8Encode(Form1->AlarmInterval->Text),
							(AnsiString)UTF8Encode(Form1->Tag3_Alarm->Text),
							tm->tm_hour, tm->tm_min, tm->tm_sec);
					Form1->Edit1->Text = USB_tr(Com_Co_Tx, ustr, 9);
				}
			}
			//--------------------------------------------------
			// Tag4�̏��̕\��
			//--------------------------------------------------
			sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Tag4_MAC->Text));
			if(strcmp(ustr, ubuf) == 0) {
				sprintf(ustr, "%c%c/%c%c/%c%c:%c%c:%c%c:%c%c%c%c%c%c%c",
							rBuf[12], rBuf[13], rBuf[14], rBuf[15], rBuf[16], rBuf[17],
							rBuf[18], rBuf[19], rBuf[20], rBuf[21],
							rBuf[22], rBuf[23], rBuf[24], rBuf[25], rBuf[26], rBuf[27], rBuf[28]);
				Form1->Tag4_Tim->Text = ustr;
				strcpy(ustr, &rBuf[30]);
				ustr[4] = 0;
				Form1->Tag4_Vbat->Text = ustr;
				strcpy(ustr, &rBuf[35]);
				ustr[4] = 0;
				Form1->Tag4_Temp->Text = ustr;
				strcpy(ustr, &rBuf[40]);
				ustr[4] = 0;
				Form1->Tag4_Hum->Text = ustr;
				strcpy(ustr, &rBuf[45]);
				ustr[6] = 0;
				Form1->Tag4_Pres->Text = ustr;
				sprintf(ustr, "%d",(signed char)rBuf[PktLen - 2]);
				Form1->Tag4_Rssi->Text = ustr;
				iTag4_Count++;
				Form1->Tag4_Count->Text = iTag4_Count;
				//--------------------------------------------------
				// Tag4 �A���[���ݒ�
				//--------------------------------------------------
				if(rBuf[11] == '1') {
					tm = localtime(&InitAlarmTime);
					Open_Com_Co_Tx();
					sprintf(ustr, "one %04X A%s %s %02d%02d%02d\n", SrcMAC,
							(AnsiString)UTF8Encode(Form1->AlarmInterval->Text),
							(AnsiString)UTF8Encode(Form1->Tag4_Alarm->Text),
							tm->tm_hour, tm->tm_min, tm->tm_sec);
					Form1->Edit1->Text = USB_tr(Com_Co_Tx, ustr, 9);
				}
			}
			//--------------------------------------------------
			// Tag5�̏��̕\��
			//--------------------------------------------------
			sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Tag5_MAC->Text));
			if(strcmp(ustr, ubuf) == 0) {
				sprintf(ustr, "%c%c/%c%c/%c%c:%c%c:%c%c:%c%c%c%c%c%c%c",
							rBuf[12], rBuf[13], rBuf[14], rBuf[15], rBuf[16], rBuf[17],
							rBuf[18], rBuf[19], rBuf[20], rBuf[21],
							rBuf[22], rBuf[23], rBuf[24], rBuf[25], rBuf[26], rBuf[27], rBuf[28]);
				Form1->Tag5_Tim->Text = ustr;
				strcpy(ustr, &rBuf[30]);
				ustr[4] = 0;
				Form1->Tag5_Vbat->Text = ustr;
				strcpy(ustr, &rBuf[35]);
				ustr[4] = 0;
				Form1->Tag5_Temp->Text = ustr;
				strcpy(ustr, &rBuf[40]);
				ustr[4] = 0;
				Form1->Tag5_Hum->Text = ustr;
				strcpy(ustr, &rBuf[45]);
				ustr[6] = 0;
				Form1->Tag5_Pres->Text = ustr;
				sprintf(ustr, "%d",(signed char)rBuf[PktLen - 2]);
				Form1->Tag5_Rssi->Text = ustr;
				iTag5_Count++;
				Form1->Tag5_Count->Text = iTag5_Count;
				//--------------------------------------------------
				// Tag5 �A���[���ݒ�
				//--------------------------------------------------
				if(rBuf[11] == '1') {
					tm = localtime(&InitAlarmTime);
					Open_Com_Co_Tx();
					sprintf(ustr, "one %04X A%s %s %02d%02d%02d\n", SrcMAC,
							(AnsiString)UTF8Encode(Form1->AlarmInterval->Text),
							(AnsiString)UTF8Encode(Form1->Tag5_Alarm->Text),
							tm->tm_hour, tm->tm_min, tm->tm_sec);
					Form1->Edit1->Text = USB_tr(Com_Co_Tx, ustr, 9);
				}
			}
			//--------------------------------------------------
			// Tag6�̏��̕\��
			//--------------------------------------------------
			sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Tag6_MAC->Text));
			if(strcmp(ustr, ubuf) == 0) {
				sprintf(ustr, "%c%c/%c%c/%c%c:%c%c:%c%c:%c%c%c%c%c%c%c",
							rBuf[12], rBuf[13], rBuf[14], rBuf[15], rBuf[16], rBuf[17],
							rBuf[18], rBuf[19], rBuf[20], rBuf[21],
							rBuf[22], rBuf[23], rBuf[24], rBuf[25], rBuf[26], rBuf[27], rBuf[28]);
				Form1->Tag6_Tim->Text = ustr;
				strcpy(ustr, &rBuf[30]);
				ustr[4] = 0;
				Form1->Tag6_Vbat->Text = ustr;
				strcpy(ustr, &rBuf[35]);
				ustr[4] = 0;
				Form1->Tag6_Temp->Text = ustr;
				strcpy(ustr, &rBuf[40]);
				ustr[4] = 0;
				Form1->Tag6_Hum->Text = ustr;
				strcpy(ustr, &rBuf[45]);
				ustr[6] = 0;
				Form1->Tag6_Pres->Text = ustr;
				sprintf(ustr, "%d",(signed char)rBuf[PktLen - 2]);
				Form1->Tag6_Rssi->Text = ustr;
				iTag6_Count++;
				Form1->Tag6_Count->Text = iTag6_Count;
				//--------------------------------------------------
				// Tag6 �A���[���ݒ�
				//--------------------------------------------------
				if(rBuf[11] == '1') {
					tm = localtime(&InitAlarmTime);
					Open_Com_Co_Tx();
					sprintf(ustr, "one %04X A%s %s %02d%02d%02d\n", SrcMAC,
							(AnsiString)UTF8Encode(Form1->AlarmInterval->Text),
							(AnsiString)UTF8Encode(Form1->Tag6_Alarm->Text),
							tm->tm_hour, tm->tm_min, tm->tm_sec);
					Form1->Edit1->Text = USB_tr(Com_Co_Tx, ustr, 9);
				}
			}
			//--------------------------------------------------
			// Tag7�̏��̕\��
			//--------------------------------------------------
			sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Tag7_MAC->Text));
			if(strcmp(ustr, ubuf) == 0) {
				sprintf(ustr, "%c%c/%c%c/%c%c:%c%c:%c%c:%c%c%c%c%c%c%c",
							rBuf[12], rBuf[13], rBuf[14], rBuf[15], rBuf[16], rBuf[17],
							rBuf[18], rBuf[19], rBuf[20], rBuf[21],
							rBuf[22], rBuf[23], rBuf[24], rBuf[25], rBuf[26], rBuf[27], rBuf[28]);
				Form1->Tag7_Tim->Text = ustr;
				strcpy(ustr, &rBuf[30]);
				ustr[4] = 0;
				Form1->Tag7_Vbat->Text = ustr;
				strcpy(ustr, &rBuf[35]);
				ustr[4] = 0;
				Form1->Tag7_Temp->Text = ustr;
				strcpy(ustr, &rBuf[40]);
				ustr[4] = 0;
				Form1->Tag7_Hum->Text = ustr;
				strcpy(ustr, &rBuf[45]);
				ustr[6] = 0;
				Form1->Tag7_Pres->Text = ustr;
				sprintf(ustr, "%d",(signed char)rBuf[PktLen - 2]);
				Form1->Tag7_Rssi->Text = ustr;
				iTag7_Count++;
				Form1->Tag7_Count->Text = iTag7_Count;
				//--------------------------------------------------
				// Tag7 �A���[���ݒ�
				//--------------------------------------------------
				if(rBuf[11] == '1') {
					tm = localtime(&InitAlarmTime);
					Open_Com_Co_Tx();
					sprintf(ustr, "one %04X A%s %s %02d%02d%02d\n", SrcMAC,
							(AnsiString)UTF8Encode(Form1->AlarmInterval->Text),
							(AnsiString)UTF8Encode(Form1->Tag7_Alarm->Text),
							tm->tm_hour, tm->tm_min, tm->tm_sec);
					Form1->Edit1->Text = USB_tr(Com_Co_Tx, ustr, 9);
				}
			}
			//--------------------------------------------------
			// Tag8�̏��̕\��
			//--------------------------------------------------
			sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Tag8_MAC->Text));
			if(strcmp(ustr, ubuf) == 0) {
				sprintf(ustr, "%c%c/%c%c/%c%c:%c%c:%c%c:%c%c%c%c%c%c%c",
							rBuf[12], rBuf[13], rBuf[14], rBuf[15], rBuf[16], rBuf[17],
							rBuf[18], rBuf[19], rBuf[20], rBuf[21],
							rBuf[22], rBuf[23], rBuf[24], rBuf[25], rBuf[26], rBuf[27], rBuf[28]);
				Form1->Tag8_Tim->Text = ustr;
				strcpy(ustr, &rBuf[30]);
				ustr[4] = 0;
				Form1->Tag8_Vbat->Text = ustr;
				strcpy(ustr, &rBuf[35]);
				ustr[4] = 0;
				Form1->Tag8_Temp->Text = ustr;
				strcpy(ustr, &rBuf[40]);
				ustr[4] = 0;
				Form1->Tag8_Hum->Text = ustr;
				strcpy(ustr, &rBuf[45]);
				ustr[6] = 0;
				Form1->Tag8_Pres->Text = ustr;
				sprintf(ustr, "%d",(signed char)rBuf[PktLen - 2]);
				Form1->Tag8_Rssi->Text = ustr;
				iTag8_Count++;
				Form1->Tag8_Count->Text = iTag8_Count;
				//--------------------------------------------------
				// Tag8 �A���[���ݒ�
				//--------------------------------------------------
				if(rBuf[11] == '1') {
					tm = localtime(&InitAlarmTime);
					Open_Com_Co_Tx();
					sprintf(ustr, "one %04X A%s %s %02d%02d%02d\n", SrcMAC,
							(AnsiString)UTF8Encode(Form1->AlarmInterval->Text),
							(AnsiString)UTF8Encode(Form1->Tag8_Alarm->Text),
							tm->tm_hour, tm->tm_min, tm->tm_sec);
					Form1->Edit1->Text = USB_tr(Com_Co_Tx, ustr, 9);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
// CW_Tx & Backscatter_Rx Start
//---------------------------------------------------------------------------
double	dCW_Tx_Freq, dBackscatter_Rx_Freq;
int		iCW_Tx_Power, iInterval, iStep_Freq, iStep_Num;
int		bCW_Tx_Power = - 1, bInterval = -1;
int 	iTxRxStart = 0;

void __fastcall TForm1::CW_TxRx_StartClick(TObject *Sender)
{
	int		k, n, nStep;
	double	dNow_Tx_Freq, dNow_Rx_Freq;
	long	NowTime;
	struct	tm	*tm;

	//--------------------------------------------------
	// Poart �ڑ� Com_CW_Tx
	//--------------------------------------------------
	if(Com_CW_Tx == INVALID_HANDLE_VALUE) {
		sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->CW_Tx_Port->Text));
		for(k = 0; ustr[k] != ':' && k < 128; k++); ustr[k] = 0;
		Com_CW_Tx = Open_USB(ustr, 230400);
		if(Com_CW_Tx == INVALID_HANDLE_VALUE) {
			Form1->Edit1->Text = "ERROR COM Port Open";
			CloseHandle(Com_CW_Tx);
			Com_CW_Tx = INVALID_HANDLE_VALUE;
			return;
		}
		USB_Tx(Com_CW_Tx, "\n", 1);
		USB_Rx(Com_CW_Tx, 1000);
	}
	//--------------------------------------------------
	// �������� InitTime�����l �ݒ�
	//--------------------------------------------------
	time(&NowTime);
	sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->InitAlarm->Text));
	sscanf(ustr, "%d", &k);
	InitAlarmTime = NowTime + k;
	tm = localtime(&InitAlarmTime);
	sprintf(ustr, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
	Form1->InitTime->Text = ustr;
	//--------------------------------------------------
	// Poart �ڑ� Com_Bk_Rx
	//--------------------------------------------------
	if(Com_Bk_Rx == INVALID_HANDLE_VALUE) {
		sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Backscatter_Rx_Port->Text));
		for(k = 0; ustr[k] != ':' && k < 128; k++); ustr[k] = 0;
		Com_Bk_Rx = Open_USB(ustr, 230400);
		if(Com_Bk_Rx == INVALID_HANDLE_VALUE) {
			Form1->Edit1->Text = "ERROR COM Port Open";
			CloseHandle(Com_CW_Tx);
			Com_Bk_Rx = INVALID_HANDLE_VALUE;
			return;
		}
		USB_Tx(Com_Bk_Rx, "\n", 1);
		USB_Rx(Com_Bk_Rx, 1000);
	}

	iTxRxStart = 1;
	while(iTxRxStart != 0) {
		sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Step_Freq->Text));
		sscanf(ustr, "%d", &iStep_Freq);
		sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Step_Num->Text));
		sscanf(ustr, "%d", &iStep_Num);
		sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->CW_Tx_Freq->Text));
		sscanf(ustr, "%lf", &dCW_Tx_Freq);
		sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Backscatter_Rx_Freq->Text));
		sscanf(ustr, "%lf", &dBackscatter_Rx_Freq);

		for(dNow_Tx_Freq = dCW_Tx_Freq, dNow_Rx_Freq = dBackscatter_Rx_Freq, nStep = 0;
			nStep < iStep_Num && iTxRxStart != 0;
			dNow_Tx_Freq += iStep_Freq, dNow_Rx_Freq += iStep_Freq, nStep++) {
			//--------------------------------------------------
			// CW_Tx ���g���ݒ�
			//--------------------------------------------------
			sprintf(ubuf, "freq %4.6lf\n", dNow_Tx_Freq);
			Form1->Edit1->Text = USB_tr(Com_CW_Tx, ubuf, 9);
			//DoEvents();
			//--------------------------------------------------
			// CW_Tx ���M�d�͐ݒ�
			//--------------------------------------------------
			sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->CW_Tx_Power->Text));
			sscanf(ustr, "%d", &iCW_Tx_Power);
			if(iCW_Tx_Power != bCW_Tx_Power) {
				sprintf(ubuf, "pow %d\n", iCW_Tx_Power);
				Form1->Edit1->Text = USB_tr(Com_CW_Tx, ubuf, 9);
				bCW_Tx_Power = iCW_Tx_Power;
				//DoEvents();
			}
			//--------------------------------------------------
			// CW_Tx ���ԊԊu�ݒ�
			//--------------------------------------------------
			sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Interval->Text));
			sscanf(ustr, "%d", &iInterval);
			sprintf(ubuf, "tim %1.3f\n", (float)iInterval / 1000);
			if(iInterval != bInterval) {
				Form1->Edit1->Text = USB_tr(Com_CW_Tx, ubuf, 9);
				//DoEvents();
				bInterval = iInterval;
			}
			//--------------------------------------------------
			// Backscatter_Rx ���g���ݒ�
			//--------------------------------------------------
			sprintf(ubuf, "freq %4.6lf\n", dNow_Rx_Freq);
			Form1->Edit1->Text = USB_tr(Com_Bk_Rx, ubuf, 9);
			sprintf(ubuf, "Tx:%4.6lf  Rx:%4.6lfMHz", dNow_Tx_Freq, dNow_Rx_Freq);
			Form1->NowCwFreq->Text = ubuf;
			//--------------------------------------------------
			// Backscatter_Rx Start
			//--------------------------------------------------
			Form1->Edit1->Text = USB_tr(Com_Bk_Rx, "sta\n", 9);
			DoEvents();
			//--------------------------------------------------
			// CW_Tx �J�n �� �I�� &  Backscatter_Rx ��M �҂�
			//--------------------------------------------------
			USB_Tx(Com_CW_Tx, "sta\n", 4);
			k = 0;
			while(k <= 0) {
				k = USB_RxS(Com_CW_Tx);
				n = USB_RxS(Com_Bk_Rx);
				if(n > 0) Backscatter_RX_Rcv_Proc(n);
				DoEvents();
			}
			USB_RxBuf[k - 2] = 0;
			Form1->Edit1->Text = (char *)USB_RxBuf;
		}
	}
}

//---------------------------------------------------------------------------
// CW_Tx & Backscatter_Rx Stop
//---------------------------------------------------------------------------
void __fastcall TForm1::CW_TxRx_StopClick(TObject *Sender)
{
	iTxRxStart = 0;
}

//---------------------------------------------------------------------------
// Cont_Tx_Setup
//---------------------------------------------------------------------------
void __fastcall TForm1::Cont_Tx_SetupClick(TObject *Sender)
{
int		k;

	//--------------------------------------------------
	// Poart �ڑ� Com_Co_Tx
	//--------------------------------------------------
	if(Com_Co_Tx == INVALID_HANDLE_VALUE) {
		sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Cont_Tx_Port->Text));
		for(k = 0; ustr[k] != ':' && k < 128; k++); ustr[k] = 0;
		Com_Co_Tx = Open_USB(ustr, 230400);
		if(Com_Co_Tx == INVALID_HANDLE_VALUE) {
			Form1->Edit1->Text = "ERROR COM Port Open";
			CloseHandle(Com_Co_Tx);
			Com_Co_Tx = INVALID_HANDLE_VALUE;
			return;
		}
		USB_Tx(Com_Co_Tx, "\n", 1);
		USB_Rx(Com_Co_Tx, 1000);
	}
	//--------------------------------------------------
	//  RF_Close
	//--------------------------------------------------
	Form1->Edit1->Text = USB_tr(Com_Co_Tx, "rfc\n", 4);
	DoEvents();
	//--------------------------------------------------
	// Cont_Tx ���g���ݒ�
	//--------------------------------------------------
	sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Cont_Tx_Freq->Text));
	sprintf(ubuf, "fre %s\n", ustr);
	Form1->Edit1->Text = USB_tr(Com_Co_Tx, ubuf, 10);
	DoEvents();
	//--------------------------------------------------
	// Cont_Tx ���M�d�͐ݒ�
	//--------------------------------------------------
	sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Cont_Tx_Power->Text));
	sprintf(ubuf, "pow %s\n", ustr);
	Form1->Edit1->Text = USB_tr(Com_Co_Tx, ubuf, 9);
	DoEvents();
	//--------------------------------------------------
	// PAN_ID �ݒ�(Dst & Src)
	//--------------------------------------------------
	sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->SrcPAN->Text));
	sprintf(ubuf, "dstp %s\n", ustr);
	Form1->Edit1->Text = USB_tr(Com_Co_Tx, ubuf, 10);
	DoEvents();
	sprintf(ubuf, "srcp %s\n", ustr);
	Form1->Edit1->Text = USB_tr(Com_Co_Tx, ubuf, 10);
	DoEvents();
	//--------------------------------------------------
	// SrcMAC �ݒ�
	//--------------------------------------------------
	sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->SrcMAC->Text));
	sprintf(ubuf, "srcm %s\n", ustr);
	Form1->Edit1->Text = USB_tr(Com_Co_Tx, ubuf, 10);
	DoEvents();
	//--------------------------------------------------
	//  RF_Open
	//--------------------------------------------------
	Form1->Edit1->Text = USB_tr(Com_Co_Tx, "rfo\n", 9);

	//CloseHandle(Com_Co_Tx);
	//Com_Co_Tx = INVALID_HANDLE_VALUE;
}

//---------------------------------------------------------------------------
//  Clear Log
//---------------------------------------------------------------------------
void __fastcall TForm1::ClearLogClick(TObject *Sender)
{
	iTag1_Count = 0;
	iTag2_Count = 0;
	iTag3_Count = 0;
	iTag4_Count = 0;
	iTag5_Count = 0;
	iTag6_Count = 0;
	iTag7_Count = 0;
	iTag8_Count = 0;

	Form1->Tag1_Tim->Text = "";
	Form1->Tag1_Vbat->Text = "";
	Form1->Tag1_Temp->Text = "";
	Form1->Tag1_Hum->Text = "";
	Form1->Tag1_Pres->Text = "";
	Form1->Tag1_Rssi->Text = "";
	Form1->Tag1_Count->Text = "";

	Form1->Tag2_Tim->Text = "";
	Form1->Tag2_Vbat->Text = "";
	Form1->Tag2_Temp->Text = "";
	Form1->Tag2_Hum->Text = "";
	Form1->Tag2_Pres->Text = "";
	Form1->Tag2_Rssi->Text = "";
	Form1->Tag2_Count->Text = "";

	Form1->Tag3_Tim->Text = "";
	Form1->Tag3_Vbat->Text = "";
	Form1->Tag3_Temp->Text = "";
	Form1->Tag3_Hum->Text = "";
	Form1->Tag3_Pres->Text = "";
	Form1->Tag3_Rssi->Text = "";
	Form1->Tag3_Count->Text = "";

	Form1->Tag4_Tim->Text = "";
	Form1->Tag4_Vbat->Text = "";
	Form1->Tag4_Temp->Text = "";
	Form1->Tag4_Hum->Text = "";
	Form1->Tag4_Pres->Text = "";
	Form1->Tag4_Rssi->Text = "";
	Form1->Tag4_Count->Text = "";

	Form1->Tag5_Tim->Text = "";
	Form1->Tag5_Vbat->Text = "";
	Form1->Tag5_Temp->Text = "";
	Form1->Tag5_Hum->Text = "";
	Form1->Tag5_Pres->Text = "";
	Form1->Tag5_Rssi->Text = "";
	Form1->Tag5_Count->Text = "";

	Form1->Tag6_Tim->Text = "";
	Form1->Tag6_Vbat->Text = "";
	Form1->Tag6_Temp->Text = "";
	Form1->Tag6_Hum->Text = "";
	Form1->Tag6_Pres->Text = "";
	Form1->Tag6_Rssi->Text = "";
	Form1->Tag6_Count->Text = "";

	Form1->Tag7_Tim->Text = "";
	Form1->Tag7_Vbat->Text = "";
	Form1->Tag7_Temp->Text = "";
	Form1->Tag7_Hum->Text = "";
	Form1->Tag7_Pres->Text = "";
	Form1->Tag7_Rssi->Text = "";
	Form1->Tag7_Count->Text = "";

	Form1->Tag8_Tim->Text = "";
	Form1->Tag8_Vbat->Text = "";
	Form1->Tag8_Temp->Text = "";
	Form1->Tag8_Hum->Text = "";
	Form1->Tag8_Pres->Text = "";
	Form1->Tag8_Rssi->Text = "";
	Form1->Tag8_Count->Text = "";
}

//---------------------------------------------------------------------------
// VCP�|�[�g����
//---------------------------------------------------------------------------
void __fastcall TForm1::SearchClick(TObject *Sender)
{
	GetVcpPoart(Sender);
}
//---------------------------------------------------------------------------

