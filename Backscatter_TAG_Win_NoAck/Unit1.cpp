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
// Hexテーブル
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
// 共通変数
//--------------------------------------------------------------------------
HANDLE  Com_CW_Tx  = INVALID_HANDLE_VALUE;
HANDLE  Com_Bk_Rx  = INVALID_HANDLE_VALUE;
HANDLE  Com_Co_Tx  = INVALID_HANDLE_VALUE;
char	COMlist[32][64];
#define	USB_BUF_SZ	4096
unsigned char	USB_RxBuf[USB_BUF_SZ]; 	// USB受信バッファ
unsigned char	USB_RxBufS[USB_BUF_SZ]; 	// USB受信バッファ
		char	ustr[1024], ubuf[1024];
		long	InitAlarmTime;

//--------------------------------------------------------------------------
// Event実行
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
// USBオープン
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
	// シリアルポートを開ける
	hCom = CreateFile(
		nam,							// シリアルポートの文字列
		GENERIC_READ | GENERIC_WRITE,	// アクセスモード：読み書き
		0,								// 共有モード：他からはアクセス不可
		NULL,							// セキュリティ属性：ハンドル継承せず
		OPEN_EXISTING,					// 作成フラグ
		FILE_ATTRIBUTE_NORMAL,			// 属性
		NULL								// テンプレートのハンドル
		);
	if(hCom == INVALID_HANDLE_VALUE) {
		return(INVALID_HANDLE_VALUE);
	}
	PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	// 通信属性を設定する
	GetCommState(hCom, &dcb);			// DCB を取得
	// データ
	dcb.BaudRate = Speed;
	dcb.fBinary = TRUE;					// バイナリモード, EOF(EofCharで指定)をﾁｴｯｸしない
	dcb.ByteSize = 8;
	dcb.fParity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	// ハードウエア・フロー制御
	dcb.fOutxCtsFlow = FALSE;			// CTSハードウェアフロー制御：CTS制御を使用しない場合はFLASEを指定
										// CTS 制御をする場合はTRUEを指定してCTS信号を監視します
	dcb.fOutxDsrFlow = FALSE;			// DSRハードウェアフロー制御：使用しない場合はFALSEを指定
	dcb.fDtrControl = DTR_CONTROL_DISABLE;	// DTR有効/無効：無効なら　DTR_CONTROL_DISABLE;ISABLE
	dcb.fRtsControl = RTS_CONTROL_DISABLE;	// RTS制御：RTS制御をしない場合はRTS_CONTROL_DISABLEを指定
	// ソフトウェアフロー制御
	dcb.fOutX = FALSE;					// 送信時XON/OFF制御の有無
	dcb.fInX = FALSE;					// 受信時XON/XOFF制御の有無
	dcb.fTXContinueOnXoff = FALSE;		// 受信バッファー満杯＆XOFF受信後の継続送信可否
	dcb.XonLim = 512;					// XONが送られるまでに格納できる最小バイト数：
	dcb.XoffLim = 512;					// XOFFが送られるまでに格納できる最小バイト数：
	dcb.XonChar = 0x11;					// 送信時XON文字 ( 送信可：ビジィ解除 ) の指定：
	// 一般に、XON文字として11H ( デバイス制御１：DC1 )がよく使われます
	dcb.XoffChar = 0x13;				// XOFF文字（送信不可：ビジー通告）の指定：なし→FALSE
										// 一般に、XOFF文字として13H ( デバイス制御3：DC3 )がよく使われます
	//その他
	dcb.fNull = FALSE;					// NULLバイトの破棄：破棄する→TRUE
	dcb.fAbortOnError = TRUE;			// エラー時の読み書き操作終了：終了する→TRUE
	dcb.fErrorChar = FALSE;				// パリティエラー発生時のキャラクタ(ErrorChar)置換：なし→FLALSE
	dcb.ErrorChar = 0x00;				// パリティエラー発生時の置換キャラクタ
	dcb.EofChar = 0x03;					// データ終了通知キャラクタ：一般に0x03(ETX)がよく使われます。
	dcb.EvtChar = 0x02;					// イベント通知キャラクタ：一般に0x02(STX)がよく使われます
	SetCommState(hCom, &dcb); 			// DCB を設定
	Sleep(100);
	return(hCom);
}

//---------------------------------------------------------------------------
// USBに送信
//---------------------------------------------------------------------------
void USB_Tx(HANDLE hCom1, unsigned char *str, int len)
{
	DWORD	dwWritten; 	// ポートへ書き込んだバイト数

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
// USBから受信
//---------------------------------------------------------------------------
int USB_Rx(HANDLE hCom1, int Wait)
{
	int	k;
	DWORD	dwErrors;	// エラー情報
	COMSTAT	ComStat;	// デバイスの状態
static	DWORD	dwCount;	// 受信データのバイト数
	DWORD	dwRead;		// ポートから読み出したバイト数

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
		DWORD	dwErrors;	// エラー情報
		COMSTAT	ComStat;	// デバイスの状態
static	DWORD	dwCount;	// 受信データのバイト数
		DWORD	dwRead;		// ポートから読み出したバイト数

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
// PCに接続されているCOMポート名、デバイス名を得る
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
			ShowMessage("Error：[RegEnumValue()]");
			return;
		}
		wcstombs(&buf[0], (wchar_t *)byValueBuff, 256);
		sprintf(COMlist[k], "%s:", buf);
		wcstombs(&buf[0], (wchar_t *)cNameBuff, 256);
		strcat(COMlist[k], buf);
	}
}

//--------------------------------------------------------------------------
// VCPポート検索
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
// USB 送受信
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
	// VCPポート検索
	//-----------------------------------------------------------------
	GetVcpPoart(Sender);
	//-----------------------------------------------------------------
	// ComboBox 候補設定
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
// タイマー割込み
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
// Backscatter_Rx 受信処理
//---------------------------------------------------------------------------
void Open_Com_Co_Tx()
{
	int		k;

	//--------------------------------------------------
	// Poart 接続 Com_Co_Tx
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
			// 時刻設定(ブロードキャスト)
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
			// Tag1の情報の表示
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
				// Tag1 アラーム設定
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
			// Tag2の情報の表示
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
				// Tag2 アラーム設定
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
			// Tag3の情報の表示
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
				// Tag3 アラーム設定
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
			// Tag4の情報の表示
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
				// Tag4 アラーム設定
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
			// Tag5の情報の表示
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
				// Tag5 アラーム設定
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
			// Tag6の情報の表示
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
				// Tag6 アラーム設定
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
			// Tag7の情報の表示
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
				// Tag7 アラーム設定
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
			// Tag8の情報の表示
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
				// Tag8 アラーム設定
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
	// Poart 接続 Com_CW_Tx
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
	// 時刻同期 InitTime初期値 設定
	//--------------------------------------------------
	time(&NowTime);
	sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->InitAlarm->Text));
	sscanf(ustr, "%d", &k);
	InitAlarmTime = NowTime + k;
	tm = localtime(&InitAlarmTime);
	sprintf(ustr, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
	Form1->InitTime->Text = ustr;
	//--------------------------------------------------
	// Poart 接続 Com_Bk_Rx
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
			// CW_Tx 周波数設定
			//--------------------------------------------------
			sprintf(ubuf, "freq %4.6lf\n", dNow_Tx_Freq);
			Form1->Edit1->Text = USB_tr(Com_CW_Tx, ubuf, 9);
			//DoEvents();
			//--------------------------------------------------
			// CW_Tx 送信電力設定
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
			// CW_Tx 時間間隔設定
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
			// Backscatter_Rx 周波数設定
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
			// CW_Tx 開始 → 終了 &  Backscatter_Rx 受信 待ち
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
	// Poart 接続 Com_Co_Tx
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
	// Cont_Tx 周波数設定
	//--------------------------------------------------
	sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Cont_Tx_Freq->Text));
	sprintf(ubuf, "fre %s\n", ustr);
	Form1->Edit1->Text = USB_tr(Com_Co_Tx, ubuf, 10);
	DoEvents();
	//--------------------------------------------------
	// Cont_Tx 送信電力設定
	//--------------------------------------------------
	sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->Cont_Tx_Power->Text));
	sprintf(ubuf, "pow %s\n", ustr);
	Form1->Edit1->Text = USB_tr(Com_Co_Tx, ubuf, 9);
	DoEvents();
	//--------------------------------------------------
	// PAN_ID 設定(Dst & Src)
	//--------------------------------------------------
	sprintf(ustr, "%s", (AnsiString)UTF8Encode(Form1->SrcPAN->Text));
	sprintf(ubuf, "dstp %s\n", ustr);
	Form1->Edit1->Text = USB_tr(Com_Co_Tx, ubuf, 10);
	DoEvents();
	sprintf(ubuf, "srcp %s\n", ustr);
	Form1->Edit1->Text = USB_tr(Com_Co_Tx, ubuf, 10);
	DoEvents();
	//--------------------------------------------------
	// SrcMAC 設定
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
// VCPポート検索
//---------------------------------------------------------------------------
void __fastcall TForm1::SearchClick(TObject *Sender)
{
	GetVcpPoart(Sender);
}
//---------------------------------------------------------------------------

