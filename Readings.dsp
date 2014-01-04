# Microsoft Developer Studio Project File - Name="Readings" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Readings - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Readings.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Readings.mak" CFG="Readings - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Readings - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Readings - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Readings - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 shlwapi.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"C:\Program Files\Hanlin\翰林数字读物聚合平台系统\Readings_r.exe"

!ELSEIF  "$(CFG)" == "Readings - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 shlwapi.lib ws2_32.lib winmm.lib setupapi.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"C:\hanlin\Readings.exe" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "Readings - Win32 Release"
# Name "Readings - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddNetBookDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddUniversity.cpp
# End Source File
# Begin Source File

SOURCE=.\control\AdvancedBitmapButton.cpp
# End Source File
# Begin Source File

SOURCE=.\AgreeMail.cpp
# End Source File
# Begin Source File

SOURCE=.\AGREEMAILDIALOG.cpp
# End Source File
# Begin Source File

SOURCE=.\control\AutoFont.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoLoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BackupDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\base64.cpp
# End Source File
# Begin Source File

SOURCE=.\ChargeMail.cpp
# End Source File
# Begin Source File

SOURCE=.\CHARGETMAILDIALOG.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientListenerThread.cpp
# End Source File
# Begin Source File

SOURCE=.\CLientThread.cpp
# End Source File
# Begin Source File

SOURCE=.\CommLog.cpp
# End Source File
# Begin Source File

SOURCE=.\CommLogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CommView.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateReGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateRGDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Decrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogComments.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogMail.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogSMS.cpp
# End Source File
# Begin Source File

SOURCE=.\DigChinaBook.cpp
# End Source File
# Begin Source File

SOURCE=.\DigHotkey.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddStudyBuilding.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadCenterTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\DownLoadListView.cpp
# End Source File
# Begin Source File

SOURCE=.\Drawers.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawerView.cpp
# End Source File
# Begin Source File

SOURCE=.\EMailADO.cpp
# End Source File
# Begin Source File

SOURCE=.\FavorPg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileCollect.cpp
# End Source File
# Begin Source File

SOURCE=.\FileFindEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ForJmail.cpp
# End Source File
# Begin Source File

SOURCE=.\FormatWaterMark.cpp
# End Source File
# Begin Source File

SOURCE=.\GetUserInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\GFXGROUPEDIT.CPP
# End Source File
# Begin Source File

SOURCE=.\GFXOUTBARCTRL.CPP
# End Source File
# Begin Source File

SOURCE=.\GFXPOPUPMENU.CPP
# End Source File
# Begin Source File

SOURCE=.\GuidTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\HotKeyListView.cpp
# End Source File
# Begin Source File

SOURCE=.\HotKeyTree.cpp
# End Source File
# Begin Source File

SOURCE=.\HotKeyView.cpp
# End Source File
# Begin Source File

SOURCE=.\HttpDownLoadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HttpSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\HttpUtl.cpp
# End Source File
# Begin Source File

SOURCE=.\idea.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MailHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\MainBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainListView.cpp
# End Source File
# Begin Source File

SOURCE=.\ManBooks.cpp
# End Source File
# Begin Source File

SOURCE=.\MidRowSplitWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\NameTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NormalPg.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderCenterView.cpp
# End Source File
# Begin Source File

SOURCE=.\ParseTxtFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ParseWolFile.cpp
# End Source File
# Begin Source File

SOURCE=.\PathDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PersonInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PubInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Push.cpp
# End Source File
# Begin Source File

SOURCE=.\PushAdv.cpp
# End Source File
# Begin Source File

SOURCE=.\PushPg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Readings.cpp
# End Source File
# Begin Source File

SOURCE=.\Readings.rc
# End Source File
# Begin Source File

SOURCE=.\ReadingsHeart.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadingsView.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadOnLine.cpp
# End Source File
# Begin Source File

SOURCE=.\RECVMAILDIALOG.cpp
# End Source File
# Begin Source File

SOURCE=.\RefuseMail.cpp
# End Source File
# Begin Source File

SOURCE=.\REFUSETMAILDIALOG.cpp
# End Source File
# Begin Source File

SOURCE=.\RegsterDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RequestMail.cpp
# End Source File
# Begin Source File

SOURCE=.\REQUESTMAILDIALOG.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceClassify.cpp
# End Source File
# Begin Source File

SOURCE=.\RowSplitWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SelPathDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerPg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetRGroupKeyWords.cpp
# End Source File
# Begin Source File

SOURCE=.\SharePg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowView.cpp
# End Source File
# Begin Source File

SOURCE=.\sms.cpp
# End Source File
# Begin Source File

SOURCE=.\sortclass.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SYNDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SynListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TipsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Transcoding.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\UDiskAuth.cpp
# End Source File
# Begin Source File

SOURCE=.\UploadShareFileList.cpp
# End Source File
# Begin Source File

SOURCE=.\UpLoadTorrent.cpp
# End Source File
# Begin Source File

SOURCE=.\UserCommDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\WaterMaker.cpp
# End Source File
# Begin Source File

SOURCE=.\control\WBButton.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\WM_Show_Dialog.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddNetBookDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddUniversity.h
# End Source File
# Begin Source File

SOURCE=.\control\AdvancedBitmapButton.h
# End Source File
# Begin Source File

SOURCE=.\AgreeMail.h
# End Source File
# Begin Source File

SOURCE=.\AGREEMAILDIALOG.h
# End Source File
# Begin Source File

SOURCE=.\control\AutoFont.h
# End Source File
# Begin Source File

SOURCE=.\AutoLoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\BackupDialog.h
# End Source File
# Begin Source File

SOURCE=.\ChargeMail.h
# End Source File
# Begin Source File

SOURCE=.\CHARGETMAILDIALOG.h
# End Source File
# Begin Source File

SOURCE=.\ClientListenerThread.h
# End Source File
# Begin Source File

SOURCE=.\CLientThread.h
# End Source File
# Begin Source File

SOURCE=.\CommLog.h
# End Source File
# Begin Source File

SOURCE=.\CommLogDlg.h
# End Source File
# Begin Source File

SOURCE=.\CommView.h
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreateReGroup.h
# End Source File
# Begin Source File

SOURCE=.\CreateRGDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DialogComments.h
# End Source File
# Begin Source File

SOURCE=.\DialogMail.h
# End Source File
# Begin Source File

SOURCE=.\DialogSMS.h
# End Source File
# Begin Source File

SOURCE=.\DigChinaBook.h
# End Source File
# Begin Source File

SOURCE=.\DigHotkey.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddStudyBuilding.h
# End Source File
# Begin Source File

SOURCE=.\DownloadCenterTreeView.h
# End Source File
# Begin Source File

SOURCE=.\DownLoadListView.h
# End Source File
# Begin Source File

SOURCE=.\Drawers.h
# End Source File
# Begin Source File

SOURCE=.\DrawerView.h
# End Source File
# Begin Source File

SOURCE=.\EMailADO.h
# End Source File
# Begin Source File

SOURCE=.\FavorPg.h
# End Source File
# Begin Source File

SOURCE=.\FileCollect.h
# End Source File
# Begin Source File

SOURCE=.\FileFindEx.h
# End Source File
# Begin Source File

SOURCE=.\ForJmail.h
# End Source File
# Begin Source File

SOURCE=.\FormarWaterMark.h
# End Source File
# Begin Source File

SOURCE=.\GetUserInfo.h
# End Source File
# Begin Source File

SOURCE=.\GFXGROUPEDIT.H
# End Source File
# Begin Source File

SOURCE=.\GFXOUTBARCTRL.H
# End Source File
# Begin Source File

SOURCE=.\GFXPOPUPMENU.H
# End Source File
# Begin Source File

SOURCE=.\GuidTreeView.h
# End Source File
# Begin Source File

SOURCE=.\HotKeyListView.h
# End Source File
# Begin Source File

SOURCE=.\HotKeyTree.h
# End Source File
# Begin Source File

SOURCE=.\HotKeyView.h
# End Source File
# Begin Source File

SOURCE=.\HttpDownLoadDlg.h
# End Source File
# Begin Source File

SOURCE=.\HttpSocket.h
# End Source File
# Begin Source File

SOURCE=.\HttpUtl.h
# End Source File
# Begin Source File

SOURCE=.\InputDlg.h
# End Source File
# Begin Source File

SOURCE=.\Mail.h
# End Source File
# Begin Source File

SOURCE=.\MailHistory.h
# End Source File
# Begin Source File

SOURCE=.\MainBar.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MainListView.h
# End Source File
# Begin Source File

SOURCE=.\ManBooks.h
# End Source File
# Begin Source File

SOURCE=.\MidRowSplitWnd.h
# End Source File
# Begin Source File

SOURCE=.\NameTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NormalPg.h
# End Source File
# Begin Source File

SOURCE=.\OrderCenterView.h
# End Source File
# Begin Source File

SOURCE=.\ParseTxtFile.h
# End Source File
# Begin Source File

SOURCE=.\ParseWolFile.h
# End Source File
# Begin Source File

SOURCE=.\PathDialog.h
# End Source File
# Begin Source File

SOURCE=.\PersonInfo.h
# End Source File
# Begin Source File

SOURCE=.\PubInfo.h
# End Source File
# Begin Source File

SOURCE=.\Push.h
# End Source File
# Begin Source File

SOURCE=.\PushAdv.h
# End Source File
# Begin Source File

SOURCE=.\PushPg.h
# End Source File
# Begin Source File

SOURCE=.\ReadControlBar.h
# End Source File
# Begin Source File

SOURCE=.\ReadDlg.h
# End Source File
# Begin Source File

SOURCE=.\Readings.h
# End Source File
# Begin Source File

SOURCE=.\ReadingsHeart.h
# End Source File
# Begin Source File

SOURCE=.\ReadingsView.h
# End Source File
# Begin Source File

SOURCE=.\ReadOnLine.h
# End Source File
# Begin Source File

SOURCE=.\RECVMAILDIALOG.h
# End Source File
# Begin Source File

SOURCE=.\RefuseMail.h
# End Source File
# Begin Source File

SOURCE=.\REFUSETMAILDIALOG.h
# End Source File
# Begin Source File

SOURCE=.\RegsterDialog.h
# End Source File
# Begin Source File

SOURCE=.\RequestMail.h
# End Source File
# Begin Source File

SOURCE=.\REQUESTMAILDIALOG.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResourceClassify.h
# End Source File
# Begin Source File

SOURCE=.\RowSplitWnd.h
# End Source File
# Begin Source File

SOURCE=.\SelPathDlg.h
# End Source File
# Begin Source File

SOURCE=.\ServerPg.h
# End Source File
# Begin Source File

SOURCE=.\SetRGroupKeyWords.h
# End Source File
# Begin Source File

SOURCE=.\SharePg.h
# End Source File
# Begin Source File

SOURCE=.\ShowDialog.h
# End Source File
# Begin Source File

SOURCE=.\ShowView.h
# End Source File
# Begin Source File

SOURCE=.\sms.h
# End Source File
# Begin Source File

SOURCE=.\sortclass.h
# End Source File
# Begin Source File

SOURCE=.\SplitWnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Struct.h
# End Source File
# Begin Source File

SOURCE=.\SYNDialog.h
# End Source File
# Begin Source File

SOURCE=.\SynListDlg.h
# End Source File
# Begin Source File

SOURCE=.\TipsDialog.h
# End Source File
# Begin Source File

SOURCE=.\Transcoding.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\UDiskAuth.h
# End Source File
# Begin Source File

SOURCE=.\UploadShareFileList.h
# End Source File
# Begin Source File

SOURCE=.\UpLoadTorrent.h
# End Source File
# Begin Source File

SOURCE=.\usb.h
# End Source File
# Begin Source File

SOURCE=.\UserCommDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\variable.h
# End Source File
# Begin Source File

SOURCE=.\WaterMaker.h
# End Source File
# Begin Source File

SOURCE=.\control\WBButton.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# Begin Source File

SOURCE=.\WM_Show_Dialog.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\pic\ICON\3dsmallbook.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\addtoorder.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\ADVCOMM.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\ADVDISABLE.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\ADVFOCUS.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\ADVSEL.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\TipDialog\alert.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\Background\bg.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\browse.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\TipDialog\btn.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\buy.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BMPLIST\cfglist.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\comments.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\comments_false.bmp
# End Source File
# Begin Source File

SOURCE=.\icon\cursor.cur
# End Source File
# Begin Source File

SOURCE=.\icon\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\icon\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\dl.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\dl_h.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\FRAME\down.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BMPLIST\download.bmp
# End Source File
# Begin Source File

SOURCE=.\icon\DRAGGING.CUR
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\dy.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\dy_check.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\dy_empty.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\email_r.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BMPLIST\filelist.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\group1.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\help.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\helpcheck.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\helpDis.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\helpsel.bmp
# End Source File
# Begin Source File

SOURCE=.\icon\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\icon\ICR_HAND.CUR
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_baidu.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_baiduE.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_bdwenku.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_computer.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_course.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_dangdang.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_du8.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_duxiu.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_ebook.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_fanshu.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_google.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\img_huntmine.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_huntmine.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_istation.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_local.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_mobile.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_opencourse.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_pdfpaper.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_readman.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_searchlogo.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_share.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_sina.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_sinaiask.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_td.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_type.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_verycd.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SEARCH\img_zcom.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\FRAME\left.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\ReadControlBar\leftbtn.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\liulan1.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\Logo.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\makewolf.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\ReadControlBar\menuBtn.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\TipDialog\msg.bmp
# End Source File
# Begin Source File

SOURCE=.\icon\msg.ico
# End Source File
# Begin Source File

SOURCE=.\icon\NODRAGGI.CUR
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\order.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\orderCheck.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\orderDis.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\orderSel.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\TipDialog\qst.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\rd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Readings.ico
# End Source File
# Begin Source File

SOURCE=.\res\Readings.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ReadingsDoc.ico
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\refresh.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\refreshCheck.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\refreshDis.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\refreshSel.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\regist.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\researchter.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\FRAME\right.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\ReadControlBar\rightbtn.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\search.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\searchCheck.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\searchDis.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\searchSel.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SHADOW\shadow1.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SHADOW\shadow2.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SHADOW\shadow3.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SHADOW\shadow4.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\SHADOW\shadow5.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\star.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\star.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\star_blank.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\stopsearch.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\stopsearchchk.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\stopsearchdis.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\stopsearchsel.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\stroy.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\stroyCheck.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\stroyDis.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\stroySel.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\syn.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\syn_check.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\syn_empty.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\synCheck.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\synDis.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\synSel.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\TipDialog\tip.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BMPLIST\tree.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\BUTTON\trymakewolf.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\FRAME\up.bmp
# End Source File
# Begin Source File

SOURCE=.\icon\WolfND.ico
# End Source File
# Begin Source File

SOURCE=.\icon\WolfND.rgs
# End Source File
# Begin Source File

SOURCE=.\pic\ReadControlBar\zoombtn.bmp
# End Source File
# Begin Source File

SOURCE=.\pic\MAINBAR\zst.bmp
# End Source File
# Begin Source File

SOURCE=".\pic\MAINBAR\闭合文件夹2.bmp"
# End Source File
# Begin Source File

SOURCE=".\pic\MAINBAR\闭合文件夹2_c.bmp"
# End Source File
# Begin Source File

SOURCE=".\pic\MAINBAR\闭合文件夹2_d.bmp"
# End Source File
# Begin Source File

SOURCE=".\pic\MAINBAR\闭合文件夹2_s.bmp"
# End Source File
# Begin Source File

SOURCE=".\pic\MAINBAR\打开文件1.bmp"
# End Source File
# Begin Source File

SOURCE=".\pic\MAINBAR\打开文件1_c.bmp"
# End Source File
# Begin Source File

SOURCE=".\pic\MAINBAR\打开文件1_d.bmp"
# End Source File
# Begin Source File

SOURCE=".\pic\MAINBAR\打开文件1_s.bmp"
# End Source File
# Begin Source File

SOURCE=".\pic\逛逛.bmp"
# End Source File
# Begin Source File

SOURCE=".\pic\用户登录.bmp"
# End Source File
# Begin Source File

SOURCE=".\pic\用户注册.bmp"
# End Source File
# Begin Source File

SOURCE=".\pic\找回密码.BMP"
# End Source File
# End Group
# Begin Source File

SOURCE=.\sound\Dang.WAV
# End Source File
# Begin Source File

SOURCE=.\msxml.dll
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\lib\image\vssver.scc
# End Source File
# Begin Source File

SOURCE=.\lib\image\zlib.lib
# End Source File
# Begin Source File

SOURCE=.\lib\image\Jpeg.lib
# End Source File
# Begin Source File

SOURCE=.\lib\image\ParseFile.lib
# End Source File
# Begin Source File

SOURCE=.\lib\image\png.lib
# End Source File
# Begin Source File

SOURCE=.\lib\image\Tiff.lib
# End Source File
# Begin Source File

SOURCE=.\lib\image\cximage.lib
# End Source File
# Begin Source File

SOURCE=.\lib\udt\UDTLIB.lib
# End Source File
# End Target
# End Project
# Section Readings : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
# Section Readings : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
