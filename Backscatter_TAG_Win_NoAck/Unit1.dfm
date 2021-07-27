object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 609
  ClientWidth = 1097
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = F1_Create
  PixelsPerInch = 96
  TextHeight = 13
  object Panel10: TPanel
    Left = 0
    Top = 0
    Width = 1097
    Height = 30
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object Label19: TLabel
      Left = 2
      Top = 2
      Width = 62
      Height = 26
      Align = alLeft
      Caption = ' CW_Tx '
      ExplicitHeight = 21
    end
    object Label20: TLabel
      Left = 274
      Top = 2
      Width = 125
      Height = 26
      Align = alLeft
      Caption = ' Backscatter_Rx '
      ExplicitHeight = 21
    end
    object Label21: TLabel
      Left = 609
      Top = 2
      Width = 71
      Height = 26
      Align = alLeft
      Caption = ' Cont_Tx '
      ExplicitHeight = 21
    end
    object CW_Tx_Port: TComboBox
      Left = 64
      Top = 2
      Width = 210
      Height = 25
      Align = alLeft
      BevelInner = bvLowered
      BevelKind = bkFlat
      BiDiMode = bdLeftToRight
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -17
      Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
      Font.Style = []
      ParentBiDiMode = False
      ParentFont = False
      TabOrder = 0
      Text = 'CW_Tx_Port'
    end
    object F1_Exit: TBitBtn
      Left = 995
      Top = 2
      Width = 100
      Height = 26
      Align = alRight
      Caption = 'Save'#8594'Exit'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = F1_ExitClick
    end
    object Search: TBitBtn
      Left = 890
      Top = 2
      Width = 79
      Height = 26
      Align = alLeft
      Caption = 'Port Search'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = SearchClick
    end
    object Cont_Tx_Port: TComboBox
      Left = 680
      Top = 2
      Width = 210
      Height = 25
      Align = alLeft
      BevelInner = bvLowered
      BevelKind = bkFlat
      BiDiMode = bdLeftToRight
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -17
      Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
      Font.Style = []
      ParentBiDiMode = False
      ParentFont = False
      TabOrder = 3
      Text = 'Cont_Tx_Port'
    end
    object Backscatter_Rx_Port: TComboBox
      Left = 399
      Top = 2
      Width = 210
      Height = 25
      Align = alLeft
      BevelInner = bvLowered
      BevelKind = bkFlat
      BiDiMode = bdLeftToRight
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clNavy
      Font.Height = -17
      Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
      Font.Style = []
      ParentBiDiMode = False
      ParentFont = False
      TabOrder = 4
      Text = 'Backscatter_Rx_Port'
    end
  end
  object Panel20: TPanel
    Left = 0
    Top = 126
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 1
    object Label1: TLabel
      Left = 2
      Top = 2
      Width = 155
      Height = 28
      Align = alLeft
      Caption = '  CW_Tx_Freq(MHz) '
      ExplicitHeight = 21
    end
    object Label2: TLabel
      Left = 767
      Top = 2
      Width = 218
      Height = 28
      Align = alRight
      Caption = '  Backscatter_Rx_Freq(MHz) '
      ExplicitHeight = 21
    end
    object Label3: TLabel
      Left = 267
      Top = 2
      Width = 103
      Height = 28
      Align = alLeft
      Caption = ' Power(dBm) '
      ExplicitHeight = 21
    end
    object CW_Tx_Freq: TEdit
      Left = 157
      Top = 2
      Width = 110
      Height = 28
      Align = alLeft
      TabOrder = 0
      Text = '2405.123456'
      ExplicitHeight = 29
    end
    object Backscatter_Rx_Freq: TEdit
      Left = 985
      Top = 2
      Width = 110
      Height = 28
      Align = alRight
      TabOrder = 1
      Text = '2405.123456'
      ExplicitHeight = 29
    end
    object CW_Tx_Power: TEdit
      Left = 370
      Top = 2
      Width = 30
      Height = 28
      Align = alLeft
      TabOrder = 2
      Text = '0'
    end
    object UpDown1: TUpDown
      Left = 400
      Top = 2
      Width = 20
      Height = 28
      Associate = CW_Tx_Power
      Max = 19
      TabOrder = 3
    end
    object NowCwFreq: TEdit
      Left = 465
      Top = 2
      Width = 302
      Height = 28
      Align = alRight
      TabOrder = 4
      Text = 'NowCwFreq'
      ExplicitHeight = 29
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 222
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 2
    object Label25: TLabel
      Left = 2
      Top = 2
      Width = 164
      Height = 28
      Align = alLeft
      Caption = '  Cont_Tx_Freq(MHz) '
      ExplicitHeight = 21
    end
    object Label4: TLabel
      Left = 236
      Top = 2
      Width = 103
      Height = 28
      Align = alLeft
      Caption = ' Power(dBm) '
      ExplicitHeight = 21
    end
    object Label14: TLabel
      Left = 708
      Top = 2
      Width = 67
      Height = 28
      Align = alRight
      Caption = ' SrcMAC '
      ExplicitHeight = 21
    end
    object Label15: TLabel
      Left = 599
      Top = 2
      Width = 59
      Height = 28
      Align = alRight
      Caption = 'SrcPAN '
      ExplicitHeight = 21
    end
    object Cont_Tx_Freq: TComboBox
      Left = 166
      Top = 2
      Width = 70
      Height = 29
      Align = alLeft
      TabOrder = 0
      Text = '2480'
    end
    object Cont_Tx_Power: TEdit
      Left = 339
      Top = 2
      Width = 30
      Height = 28
      Align = alLeft
      TabOrder = 1
      Text = '0'
    end
    object UpDown2: TUpDown
      Left = 369
      Top = 2
      Width = 20
      Height = 28
      Associate = Cont_Tx_Power
      Max = 19
      TabOrder = 2
    end
    object Cont_Tx_Setup: TButton
      Left = 825
      Top = 2
      Width = 270
      Height = 28
      Align = alRight
      Caption = 'Cont_Tx_Setup'
      ImageAlignment = iaRight
      TabOrder = 3
      OnClick = Cont_Tx_SetupClick
    end
    object SrcMAC: TEdit
      Left = 775
      Top = 2
      Width = 50
      Height = 28
      Align = alRight
      TabOrder = 4
      Text = 'BA01'
      ExplicitHeight = 29
    end
    object SrcPAN: TEdit
      Left = 658
      Top = 2
      Width = 50
      Height = 28
      Align = alRight
      TabOrder = 5
      Text = 'BF01'
      ExplicitHeight = 29
    end
  end
  object Panel4: TPanel
    Left = 0
    Top = 350
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 3
    object Label5: TLabel
      Left = 2
      Top = 2
      Width = 86
      Height = 28
      Align = alLeft
      Caption = 'Tag1  MAC '
      ExplicitHeight = 21
    end
    object Label6: TLabel
      Left = 138
      Top = 2
      Width = 54
      Height = 28
      Align = alLeft
      Caption = ' Alarm '
      ExplicitHeight = 21
    end
    object Label9: TLabel
      Left = 242
      Top = 2
      Width = 47
      Height = 28
      Align = alLeft
      Caption = ' Time '
      ExplicitHeight = 21
    end
    object Label10: TLabel
      Left = 504
      Top = 2
      Width = 35
      Height = 28
      Align = alLeft
      Caption = ' Bat '
      ExplicitHeight = 21
    end
    object Label11: TLabel
      Left = 579
      Top = 2
      Width = 52
      Height = 28
      Align = alLeft
      Caption = ' Temp '
      ExplicitHeight = 21
    end
    object Label12: TLabel
      Left = 671
      Top = 2
      Width = 44
      Height = 28
      Align = alLeft
      Caption = ' Hum '
      ExplicitHeight = 21
    end
    object Label13: TLabel
      Left = 755
      Top = 2
      Width = 42
      Height = 28
      Align = alLeft
      Caption = ' Pres '
      ExplicitHeight = 21
    end
    object Label22: TLabel
      Left = 852
      Top = 2
      Width = 87
      Height = 28
      Align = alLeft
      Caption = ' RSSI(dBm)'
      ExplicitHeight = 21
    end
    object Label23: TLabel
      Left = 989
      Top = 2
      Width = 53
      Height = 28
      Align = alLeft
      Caption = ' Count '
      ExplicitHeight = 21
    end
    object Tag1_Tim: TEdit
      Left = 289
      Top = 2
      Width = 215
      Height = 28
      Align = alLeft
      TabOrder = 0
      ExplicitHeight = 29
    end
    object Tag1_Vbat: TEdit
      Left = 539
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 1
      ExplicitHeight = 29
    end
    object Tag1_Temp: TEdit
      Left = 631
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 2
      ExplicitHeight = 29
    end
    object Tag1_Hum: TEdit
      Left = 715
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 3
      ExplicitHeight = 29
    end
    object Tag1_Pres: TEdit
      Left = 797
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 4
      ExplicitHeight = 29
    end
    object Tag1_Alarm: TEdit
      Left = 192
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 5
      ExplicitHeight = 29
    end
    object Tag1_Rssi: TEdit
      Left = 939
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      TabOrder = 6
      ExplicitHeight = 29
    end
    object Tag1_MAC: TEdit
      Left = 88
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 7
      Text = 'BB01'
      ExplicitHeight = 29
    end
    object Tag1_Count: TEdit
      Left = 1042
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 8
      ExplicitHeight = 29
    end
  end
  object Panel6: TPanel
    Left = 0
    Top = 286
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 4
    object Label7: TLabel
      Left = 2
      Top = 2
      Width = 201
      Height = 28
      Align = alLeft
      Caption = '  Sens Alarm Interval(Sec) '
      ExplicitHeight = 21
    end
    object Label87: TLabel
      Left = 270
      Top = 2
      Width = 129
      Height = 28
      Align = alLeft
      Caption = '  Init Alarm(Sec) '
      ExplicitHeight = 21
    end
    object Label88: TLabel
      Left = 474
      Top = 2
      Width = 39
      Height = 28
      Align = alLeft
      Caption = '  Set '
      ExplicitHeight = 21
    end
    object Label89: TLabel
      Left = 593
      Top = 2
      Width = 52
      Height = 28
      Align = alLeft
      Caption = ' '#8594'Init '
      ExplicitHeight = 21
    end
    object AlarmInterval: TComboBox
      Left = 203
      Top = 2
      Width = 67
      Height = 29
      Align = alLeft
      DropDownCount = 32
      TabOrder = 0
      Text = '60'
    end
    object ClearLog: TButton
      Left = 928
      Top = 2
      Width = 167
      Height = 28
      Align = alRight
      Caption = 'Clear Log'
      ImageAlignment = iaRight
      TabOrder = 1
      OnClick = ClearLogClick
    end
    object InitAlarm: TComboBox
      Left = 399
      Top = 2
      Width = 75
      Height = 29
      Align = alLeft
      DropDownCount = 32
      TabOrder = 2
      Text = '300'
    end
    object InitTime: TEdit
      Left = 645
      Top = 2
      Width = 80
      Height = 28
      Align = alLeft
      TabOrder = 3
      Text = 'hh:mm:ss'
      ExplicitHeight = 29
    end
    object InitSetTime: TEdit
      Left = 513
      Top = 2
      Width = 80
      Height = 28
      Align = alLeft
      TabOrder = 4
      Text = 'hh:mm:ss'
      ExplicitHeight = 29
    end
  end
  object Panel21: TPanel
    Left = 0
    Top = 158
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 5
    object Label16: TLabel
      Left = 2
      Top = 2
      Width = 136
      Height = 28
      Align = alLeft
      Caption = '  Step_Freq(MHz) '
      ExplicitHeight = 21
    end
    object Label17: TLabel
      Left = 183
      Top = 2
      Width = 96
      Height = 28
      Align = alLeft
      Caption = '  Step_Num  '
      ExplicitHeight = 21
    end
    object Label18: TLabel
      Left = 324
      Top = 2
      Width = 109
      Height = 28
      Align = alLeft
      Caption = '  Interval(mS) '
      ExplicitHeight = 21
    end
    object Step_Freq: TComboBox
      Left = 138
      Top = 2
      Width = 45
      Height = 29
      Align = alLeft
      DropDownCount = 32
      TabOrder = 0
      Text = '5'
    end
    object Step_Num: TComboBox
      Left = 279
      Top = 2
      Width = 45
      Height = 29
      Align = alLeft
      DropDownCount = 32
      TabOrder = 1
      Text = '5'
    end
    object Interval: TComboBox
      Left = 433
      Top = 2
      Width = 67
      Height = 29
      Align = alLeft
      DropDownCount = 32
      TabOrder = 2
      Text = '1000'
    end
    object CW_TxRx_Start: TButton
      Left = 555
      Top = 2
      Width = 270
      Height = 28
      Align = alRight
      Caption = 'TAG_CW_Tx And TAG_Rx Start'
      TabOrder = 3
      OnClick = CW_TxRx_StartClick
    end
    object CW_TxRx_Stop: TButton
      Left = 825
      Top = 2
      Width = 270
      Height = 28
      Align = alRight
      Caption = 'TAG_CW_Tx And TAG_Rx Stop'
      TabOrder = 4
      OnClick = CW_TxRx_StopClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 62
    Width = 1097
    Height = 32
    Align = alTop
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 6
    object Edit2: TEdit
      Left = 1
      Top = 1
      Width = 1095
      Height = 30
      Align = alClient
      TabOrder = 0
      ExplicitHeight = 29
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 30
    Width = 1097
    Height = 32
    Align = alTop
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 7
    object Edit1: TEdit
      Left = 1
      Top = 1
      Width = 1095
      Height = 30
      Align = alClient
      TabOrder = 0
      ExplicitHeight = 29
    end
  end
  object Panel5: TPanel
    Left = 0
    Top = 318
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
  end
  object Panel7: TPanel
    Left = 0
    Top = 254
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
  end
  object Panel8: TPanel
    Left = 0
    Top = 190
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
  end
  object Panel9: TPanel
    Left = 0
    Top = 382
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 11
    object Label8: TLabel
      Left = 2
      Top = 2
      Width = 86
      Height = 28
      Align = alLeft
      Caption = 'Tag2  MAC '
      ExplicitHeight = 21
    end
    object Label24: TLabel
      Left = 138
      Top = 2
      Width = 54
      Height = 28
      Align = alLeft
      Caption = ' Alarm '
      ExplicitHeight = 21
    end
    object Label26: TLabel
      Left = 242
      Top = 2
      Width = 47
      Height = 28
      Align = alLeft
      Caption = ' Time '
      ExplicitHeight = 21
    end
    object Label27: TLabel
      Left = 504
      Top = 2
      Width = 35
      Height = 28
      Align = alLeft
      Caption = ' Bat '
      ExplicitHeight = 21
    end
    object Label28: TLabel
      Left = 579
      Top = 2
      Width = 52
      Height = 28
      Align = alLeft
      Caption = ' Temp '
      ExplicitHeight = 21
    end
    object Label29: TLabel
      Left = 671
      Top = 2
      Width = 44
      Height = 28
      Align = alLeft
      Caption = ' Hum '
      ExplicitHeight = 21
    end
    object Label30: TLabel
      Left = 755
      Top = 2
      Width = 42
      Height = 28
      Align = alLeft
      Caption = ' Pres '
      ExplicitHeight = 21
    end
    object Label31: TLabel
      Left = 852
      Top = 2
      Width = 87
      Height = 28
      Align = alLeft
      Caption = ' RSSI(dBm)'
      ExplicitHeight = 21
    end
    object Label32: TLabel
      Left = 989
      Top = 2
      Width = 53
      Height = 28
      Align = alLeft
      Caption = ' Count '
      ExplicitHeight = 21
    end
    object Tag2_Tim: TEdit
      Left = 289
      Top = 2
      Width = 215
      Height = 28
      Align = alLeft
      TabOrder = 0
      ExplicitHeight = 29
    end
    object Tag2_Vbat: TEdit
      Left = 539
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 1
      ExplicitHeight = 29
    end
    object Tag2_Temp: TEdit
      Left = 631
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 2
      ExplicitHeight = 29
    end
    object Tag2_Hum: TEdit
      Left = 715
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 3
      ExplicitHeight = 29
    end
    object Tag2_Pres: TEdit
      Left = 797
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 4
      ExplicitHeight = 29
    end
    object Tag2_Alarm: TEdit
      Left = 192
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 5
      ExplicitHeight = 29
    end
    object Tag2_Rssi: TEdit
      Left = 939
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      TabOrder = 6
      ExplicitHeight = 29
    end
    object Tag2_MAC: TEdit
      Left = 88
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 7
      Text = 'BB02'
      ExplicitHeight = 29
    end
    object Tag2_Count: TEdit
      Left = 1042
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 8
      ExplicitHeight = 29
    end
  end
  object Panel11: TPanel
    Left = 0
    Top = 414
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 12
    object Label33: TLabel
      Left = 2
      Top = 2
      Width = 86
      Height = 28
      Align = alLeft
      Caption = 'Tag3  MAC '
      ExplicitHeight = 21
    end
    object Label34: TLabel
      Left = 138
      Top = 2
      Width = 54
      Height = 28
      Align = alLeft
      Caption = ' Alarm '
      ExplicitHeight = 21
    end
    object Label35: TLabel
      Left = 242
      Top = 2
      Width = 47
      Height = 28
      Align = alLeft
      Caption = ' Time '
      ExplicitHeight = 21
    end
    object Label36: TLabel
      Left = 504
      Top = 2
      Width = 35
      Height = 28
      Align = alLeft
      Caption = ' Bat '
      ExplicitHeight = 21
    end
    object Label37: TLabel
      Left = 579
      Top = 2
      Width = 52
      Height = 28
      Align = alLeft
      Caption = ' Temp '
      ExplicitHeight = 21
    end
    object Label38: TLabel
      Left = 671
      Top = 2
      Width = 44
      Height = 28
      Align = alLeft
      Caption = ' Hum '
      ExplicitHeight = 21
    end
    object Label39: TLabel
      Left = 755
      Top = 2
      Width = 42
      Height = 28
      Align = alLeft
      Caption = ' Pres '
      ExplicitHeight = 21
    end
    object Label40: TLabel
      Left = 852
      Top = 2
      Width = 87
      Height = 28
      Align = alLeft
      Caption = ' RSSI(dBm)'
      ExplicitHeight = 21
    end
    object Label41: TLabel
      Left = 989
      Top = 2
      Width = 53
      Height = 28
      Align = alLeft
      Caption = ' Count '
      ExplicitHeight = 21
    end
    object Tag3_Tim: TEdit
      Left = 289
      Top = 2
      Width = 215
      Height = 28
      Align = alLeft
      TabOrder = 0
      ExplicitHeight = 29
    end
    object Tag3_Vbat: TEdit
      Left = 539
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 1
      ExplicitHeight = 29
    end
    object Tag3_Temp: TEdit
      Left = 631
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 2
      ExplicitHeight = 29
    end
    object Tag3_Hum: TEdit
      Left = 715
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 3
      ExplicitHeight = 29
    end
    object Tag3_Pres: TEdit
      Left = 797
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 4
      ExplicitHeight = 29
    end
    object Tag3_Alarm: TEdit
      Left = 192
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 5
      ExplicitHeight = 29
    end
    object Tag3_Rssi: TEdit
      Left = 939
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      TabOrder = 6
      ExplicitHeight = 29
    end
    object Tag3_MAC: TEdit
      Left = 88
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 7
      Text = 'BB03'
      ExplicitHeight = 29
    end
    object Tag3_Count: TEdit
      Left = 1042
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 8
      ExplicitHeight = 29
    end
  end
  object Panel12: TPanel
    Left = 0
    Top = 446
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 13
    object Label42: TLabel
      Left = 2
      Top = 2
      Width = 86
      Height = 28
      Align = alLeft
      Caption = 'Tag4  MAC '
      ExplicitHeight = 21
    end
    object Label43: TLabel
      Left = 138
      Top = 2
      Width = 54
      Height = 28
      Align = alLeft
      Caption = ' Alarm '
      ExplicitHeight = 21
    end
    object Label44: TLabel
      Left = 242
      Top = 2
      Width = 47
      Height = 28
      Align = alLeft
      Caption = ' Time '
      ExplicitHeight = 21
    end
    object Label45: TLabel
      Left = 504
      Top = 2
      Width = 35
      Height = 28
      Align = alLeft
      Caption = ' Bat '
      ExplicitHeight = 21
    end
    object Label46: TLabel
      Left = 579
      Top = 2
      Width = 52
      Height = 28
      Align = alLeft
      Caption = ' Temp '
      ExplicitHeight = 21
    end
    object Label47: TLabel
      Left = 671
      Top = 2
      Width = 44
      Height = 28
      Align = alLeft
      Caption = ' Hum '
      ExplicitHeight = 21
    end
    object Label48: TLabel
      Left = 755
      Top = 2
      Width = 42
      Height = 28
      Align = alLeft
      Caption = ' Pres '
      ExplicitHeight = 21
    end
    object Label49: TLabel
      Left = 852
      Top = 2
      Width = 87
      Height = 28
      Align = alLeft
      Caption = ' RSSI(dBm)'
      ExplicitHeight = 21
    end
    object Label50: TLabel
      Left = 989
      Top = 2
      Width = 53
      Height = 28
      Align = alLeft
      Caption = ' Count '
      ExplicitHeight = 21
    end
    object Tag4_Tim: TEdit
      Left = 289
      Top = 2
      Width = 215
      Height = 28
      Align = alLeft
      TabOrder = 0
      ExplicitHeight = 29
    end
    object Tag4_Vbat: TEdit
      Left = 539
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 1
      ExplicitHeight = 29
    end
    object Tag4_Temp: TEdit
      Left = 631
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 2
      ExplicitHeight = 29
    end
    object Tag4_Hum: TEdit
      Left = 715
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 3
      ExplicitHeight = 29
    end
    object Tag4_Pres: TEdit
      Left = 797
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 4
      ExplicitHeight = 29
    end
    object Tag4_Alarm: TEdit
      Left = 192
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 5
      ExplicitHeight = 29
    end
    object Tag4_Rssi: TEdit
      Left = 939
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      TabOrder = 6
      ExplicitHeight = 29
    end
    object Tag4_MAC: TEdit
      Left = 88
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 7
      Text = 'BB04'
      ExplicitHeight = 29
    end
    object Tag4_Count: TEdit
      Left = 1042
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 8
      ExplicitHeight = 29
    end
  end
  object Panel13: TPanel
    Left = 0
    Top = 94
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 14
  end
  object Panel14: TPanel
    Left = 0
    Top = 478
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 15
    object Label51: TLabel
      Left = 2
      Top = 2
      Width = 86
      Height = 28
      Align = alLeft
      Caption = 'Tag5  MAC '
      ExplicitHeight = 21
    end
    object Label52: TLabel
      Left = 138
      Top = 2
      Width = 54
      Height = 28
      Align = alLeft
      Caption = ' Alarm '
      ExplicitHeight = 21
    end
    object Label53: TLabel
      Left = 242
      Top = 2
      Width = 47
      Height = 28
      Align = alLeft
      Caption = ' Time '
      ExplicitHeight = 21
    end
    object Label54: TLabel
      Left = 504
      Top = 2
      Width = 35
      Height = 28
      Align = alLeft
      Caption = ' Bat '
      ExplicitHeight = 21
    end
    object Label55: TLabel
      Left = 579
      Top = 2
      Width = 52
      Height = 28
      Align = alLeft
      Caption = ' Temp '
      ExplicitHeight = 21
    end
    object Label56: TLabel
      Left = 671
      Top = 2
      Width = 44
      Height = 28
      Align = alLeft
      Caption = ' Hum '
      ExplicitHeight = 21
    end
    object Label57: TLabel
      Left = 755
      Top = 2
      Width = 42
      Height = 28
      Align = alLeft
      Caption = ' Pres '
      ExplicitHeight = 21
    end
    object Label58: TLabel
      Left = 852
      Top = 2
      Width = 87
      Height = 28
      Align = alLeft
      Caption = ' RSSI(dBm)'
      ExplicitHeight = 21
    end
    object Label59: TLabel
      Left = 989
      Top = 2
      Width = 53
      Height = 28
      Align = alLeft
      Caption = ' Count '
      ExplicitHeight = 21
    end
    object Tag5_Tim: TEdit
      Left = 289
      Top = 2
      Width = 215
      Height = 28
      Align = alLeft
      TabOrder = 0
      ExplicitHeight = 29
    end
    object Tag5_Vbat: TEdit
      Left = 539
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 1
      ExplicitHeight = 29
    end
    object Tag5_Temp: TEdit
      Left = 631
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 2
      ExplicitHeight = 29
    end
    object Tag5_Hum: TEdit
      Left = 715
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 3
      ExplicitHeight = 29
    end
    object Tag5_Pres: TEdit
      Left = 797
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 4
      ExplicitHeight = 29
    end
    object Tag5_Alarm: TEdit
      Left = 192
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 5
      ExplicitHeight = 29
    end
    object Tag5_Rssi: TEdit
      Left = 939
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      TabOrder = 6
      ExplicitHeight = 29
    end
    object Tag5_MAC: TEdit
      Left = 88
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 7
      Text = 'BB05'
      ExplicitHeight = 29
    end
    object Tag5_Count: TEdit
      Left = 1042
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 8
      ExplicitHeight = 29
    end
  end
  object Panel15: TPanel
    Left = 0
    Top = 510
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 16
    object Label60: TLabel
      Left = 2
      Top = 2
      Width = 86
      Height = 28
      Align = alLeft
      Caption = 'Tag6  MAC '
      ExplicitHeight = 21
    end
    object Label61: TLabel
      Left = 138
      Top = 2
      Width = 54
      Height = 28
      Align = alLeft
      Caption = ' Alarm '
      ExplicitHeight = 21
    end
    object Label62: TLabel
      Left = 242
      Top = 2
      Width = 47
      Height = 28
      Align = alLeft
      Caption = ' Time '
      ExplicitHeight = 21
    end
    object Label63: TLabel
      Left = 504
      Top = 2
      Width = 35
      Height = 28
      Align = alLeft
      Caption = ' Bat '
      ExplicitHeight = 21
    end
    object Label64: TLabel
      Left = 579
      Top = 2
      Width = 52
      Height = 28
      Align = alLeft
      Caption = ' Temp '
      ExplicitHeight = 21
    end
    object Label65: TLabel
      Left = 671
      Top = 2
      Width = 44
      Height = 28
      Align = alLeft
      Caption = ' Hum '
      ExplicitHeight = 21
    end
    object Label66: TLabel
      Left = 755
      Top = 2
      Width = 42
      Height = 28
      Align = alLeft
      Caption = ' Pres '
      ExplicitHeight = 21
    end
    object Label67: TLabel
      Left = 852
      Top = 2
      Width = 87
      Height = 28
      Align = alLeft
      Caption = ' RSSI(dBm)'
      ExplicitHeight = 21
    end
    object Label68: TLabel
      Left = 989
      Top = 2
      Width = 53
      Height = 28
      Align = alLeft
      Caption = ' Count '
      ExplicitHeight = 21
    end
    object Tag6_Tim: TEdit
      Left = 289
      Top = 2
      Width = 215
      Height = 28
      Align = alLeft
      TabOrder = 0
      ExplicitHeight = 29
    end
    object Tag6_Vbat: TEdit
      Left = 539
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 1
      ExplicitHeight = 29
    end
    object Tag6_Temp: TEdit
      Left = 631
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 2
      ExplicitHeight = 29
    end
    object Tag6_Hum: TEdit
      Left = 715
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 3
      ExplicitHeight = 29
    end
    object Tag6_Pres: TEdit
      Left = 797
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 4
      ExplicitHeight = 29
    end
    object Tag6_Alarm: TEdit
      Left = 192
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 5
      ExplicitHeight = 29
    end
    object Tag6_Rssi: TEdit
      Left = 939
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      TabOrder = 6
      ExplicitHeight = 29
    end
    object Tag6_MAC: TEdit
      Left = 88
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 7
      Text = 'BB06'
      ExplicitHeight = 29
    end
    object Tag6_Count: TEdit
      Left = 1042
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 8
      ExplicitHeight = 29
    end
  end
  object Panel16: TPanel
    Left = 0
    Top = 542
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 17
    object Label69: TLabel
      Left = 2
      Top = 2
      Width = 86
      Height = 28
      Align = alLeft
      Caption = 'Tag7  MAC '
      ExplicitHeight = 21
    end
    object Label70: TLabel
      Left = 138
      Top = 2
      Width = 54
      Height = 28
      Align = alLeft
      Caption = ' Alarm '
      ExplicitHeight = 21
    end
    object Label71: TLabel
      Left = 242
      Top = 2
      Width = 47
      Height = 28
      Align = alLeft
      Caption = ' Time '
      ExplicitHeight = 21
    end
    object Label72: TLabel
      Left = 504
      Top = 2
      Width = 35
      Height = 28
      Align = alLeft
      Caption = ' Bat '
      ExplicitHeight = 21
    end
    object Label73: TLabel
      Left = 579
      Top = 2
      Width = 52
      Height = 28
      Align = alLeft
      Caption = ' Temp '
      ExplicitHeight = 21
    end
    object Label74: TLabel
      Left = 671
      Top = 2
      Width = 44
      Height = 28
      Align = alLeft
      Caption = ' Hum '
      ExplicitHeight = 21
    end
    object Label75: TLabel
      Left = 755
      Top = 2
      Width = 42
      Height = 28
      Align = alLeft
      Caption = ' Pres '
      ExplicitHeight = 21
    end
    object Label76: TLabel
      Left = 852
      Top = 2
      Width = 87
      Height = 28
      Align = alLeft
      Caption = ' RSSI(dBm)'
      ExplicitHeight = 21
    end
    object Label77: TLabel
      Left = 989
      Top = 2
      Width = 53
      Height = 28
      Align = alLeft
      Caption = ' Count '
      ExplicitHeight = 21
    end
    object Tag7_Tim: TEdit
      Left = 289
      Top = 2
      Width = 215
      Height = 28
      Align = alLeft
      TabOrder = 0
      ExplicitHeight = 29
    end
    object Tag7_Vbat: TEdit
      Left = 539
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 1
      ExplicitHeight = 29
    end
    object Tag7_Temp: TEdit
      Left = 631
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 2
      ExplicitHeight = 29
    end
    object Tag7_Hum: TEdit
      Left = 715
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 3
      ExplicitHeight = 29
    end
    object Tag7_Pres: TEdit
      Left = 797
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 4
      ExplicitHeight = 29
    end
    object Tag7_Alarm: TEdit
      Left = 192
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 5
      ExplicitHeight = 29
    end
    object Tag7_Rssi: TEdit
      Left = 939
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      TabOrder = 6
      ExplicitHeight = 29
    end
    object Tag7_MAC: TEdit
      Left = 88
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 7
      Text = 'BB07'
      ExplicitHeight = 29
    end
    object Tag7_Count: TEdit
      Left = 1042
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 8
      ExplicitHeight = 29
    end
  end
  object Panel17: TPanel
    Left = 0
    Top = 574
    Width = 1097
    Height = 32
    Align = alTop
    BorderWidth = 1
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 18
    object Label78: TLabel
      Left = 2
      Top = 2
      Width = 86
      Height = 28
      Align = alLeft
      Caption = 'Tag8  MAC '
      ExplicitHeight = 21
    end
    object Label79: TLabel
      Left = 138
      Top = 2
      Width = 54
      Height = 28
      Align = alLeft
      Caption = ' Alarm '
      ExplicitHeight = 21
    end
    object Label80: TLabel
      Left = 242
      Top = 2
      Width = 47
      Height = 28
      Align = alLeft
      Caption = ' Time '
      ExplicitHeight = 21
    end
    object Label81: TLabel
      Left = 504
      Top = 2
      Width = 35
      Height = 28
      Align = alLeft
      Caption = ' Bat '
      ExplicitHeight = 21
    end
    object Label82: TLabel
      Left = 579
      Top = 2
      Width = 52
      Height = 28
      Align = alLeft
      Caption = ' Temp '
      ExplicitHeight = 21
    end
    object Label83: TLabel
      Left = 671
      Top = 2
      Width = 44
      Height = 28
      Align = alLeft
      Caption = ' Hum '
      ExplicitHeight = 21
    end
    object Label84: TLabel
      Left = 755
      Top = 2
      Width = 42
      Height = 28
      Align = alLeft
      Caption = ' Pres '
      ExplicitHeight = 21
    end
    object Label85: TLabel
      Left = 852
      Top = 2
      Width = 87
      Height = 28
      Align = alLeft
      Caption = ' RSSI(dBm)'
      ExplicitHeight = 21
    end
    object Label86: TLabel
      Left = 989
      Top = 2
      Width = 53
      Height = 28
      Align = alLeft
      Caption = ' Count '
      ExplicitHeight = 21
    end
    object Tag8_Tim: TEdit
      Left = 289
      Top = 2
      Width = 215
      Height = 28
      Align = alLeft
      TabOrder = 0
      ExplicitHeight = 29
    end
    object Tag8_Vbat: TEdit
      Left = 539
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 1
      ExplicitHeight = 29
    end
    object Tag8_Temp: TEdit
      Left = 631
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 2
      ExplicitHeight = 29
    end
    object Tag8_Hum: TEdit
      Left = 715
      Top = 2
      Width = 40
      Height = 28
      Align = alLeft
      TabOrder = 3
      ExplicitHeight = 29
    end
    object Tag8_Pres: TEdit
      Left = 797
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 4
      ExplicitHeight = 29
    end
    object Tag8_Alarm: TEdit
      Left = 192
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 5
      ExplicitHeight = 29
    end
    object Tag8_Rssi: TEdit
      Left = 939
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      TabOrder = 6
      ExplicitHeight = 29
    end
    object Tag8_MAC: TEdit
      Left = 88
      Top = 2
      Width = 50
      Height = 28
      Align = alLeft
      Alignment = taCenter
      TabOrder = 7
      Text = 'BB08'
      ExplicitHeight = 29
    end
    object Tag8_Count: TEdit
      Left = 1042
      Top = 2
      Width = 55
      Height = 28
      Align = alLeft
      TabOrder = 8
      ExplicitHeight = 29
    end
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 360
    Top = 336
  end
end
