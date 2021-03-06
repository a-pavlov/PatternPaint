############################################################################################
#      NSIS Installation Script created by NSIS Quick Setup Script Generator v1.09.18
#               Entirely Edited with NullSoft Scriptable Installation System                
#              by Vlasis K. Barkas aka Red Wine red_wine@freemail.gr Sep 2006               
#
# Modified to also install a driver, in the method suggested here:
# http://stackoverflow.com/questions/2464843/installing-drivers-from-nsis-installer-in-x64-system
############################################################################################

!include "x64.nsh"

!define APP_NAME "Pattern Paint"
!define COMP_NAME "Blinkinlabs"
!define WEB_SITE "http://blinkinlabs.com"
!define VERSION "00.1.1.00"
!define COPYRIGHT "Blinkinlabs � 2013"
!define DESCRIPTION "Application"
!define INSTALLER_NAME "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaint Windows Installer.exe"
!define MAIN_APP_EXE "PatternPaint.exe"
!define INSTALL_TYPE "SetShellVarContext current"
!define REG_ROOT "HKCU"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

SetCompressor ZLIB
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on
InstallDirRegKey "${REG_ROOT}" "${REG_APP_PATH}" ""
InstallDir "$PROGRAMFILES\Pattern Paint"

######################################################################

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!insertmacro MUI_PAGE_DIRECTORY

!ifdef REG_START_MENU
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Pattern Paint"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
!insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${MAIN_APP_EXE}"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

######################################################################

Section -MainProgram
${INSTALL_TYPE}
SetOverwrite ifnewer
SetOutPath "$INSTDIR"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\icudt53.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\icuin53.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\icuuc53.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\libgcc_s_dw2-1.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\libstdc++-6.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\libwinpthread-1.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\PatternPaint.exe"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\Qt5Core.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\Qt5Gui.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\Qt5SerialPort.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\Qt5Widgets.dll"
SetOutPath "$INSTDIR\platforms"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\platforms\qwindows.dll"
SetOutPath "$INSTDIR\imageformats"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\imageformats\qgif.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\imageformats\qjpeg.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\imageformats\qsvg.dll"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\imageformats\qtiff.dll"
SetOutPath "$INSTDIR\driver"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\driver\blinkinlabs.cat"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\driver\blinkinlabs.inf"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\driver\dpinst32.exe"
File "C:\Users\matt\My Documents\PatternPaint_Builder\PatternPaintWindows\driver\dpinst64.exe"
SectionEnd

######################################################################

Section -Icons_Reg
SetOutPath "$INSTDIR"
WriteUninstaller "$INSTDIR\uninstall.exe"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
CreateDirectory "$SMPROGRAMS\$SM_Folder"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!insertmacro MUI_STARTMENU_WRITE_END
!endif

!ifndef REG_START_MENU
CreateDirectory "$SMPROGRAMS\Pattern Paint"
CreateShortCut "$SMPROGRAMS\Pattern Paint\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\Pattern Paint\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!endif

WriteRegStr ${REG_ROOT} "${REG_APP_PATH}" "" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "$INSTDIR\uninstall.exe"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayIcon" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif
SectionEnd

######################################################################

Section Driver
${If} ${RunningX64}
       ExecWait '"$INSTDIR\driver\dpinst64.exe" /sa /sw /PATH "$INSTDIR\driver"'
	   ${Else}
       ExecWait '"$INSTDIR\driver\dpinst32.exe" /sa /sw /PATH "$INSTDIR\driver"'
	   ${EndIf}
SectionEnd
	
######################################################################

Section Uninstall
${INSTALL_TYPE}
Delete "$INSTDIR\icudt53.dll"
Delete "$INSTDIR\icuin53.dll"
Delete "$INSTDIR\icuuc53.dll"
Delete "$INSTDIR\libgcc_s_dw2-1.dll"
Delete "$INSTDIR\libstdc++-6.dll"
Delete "$INSTDIR\libwinpthread-1.dll"
Delete "$INSTDIR\PatternPaint.exe"
Delete "$INSTDIR\Qt5Core.dll"
Delete "$INSTDIR\Qt5Gui.dll"
Delete "$INSTDIR\Qt5SerialPort.dll"
Delete "$INSTDIR\Qt5Widgets.dll"
Delete "$INSTDIR\platforms\qwindows.dll"
Delete "$INSTDIR\imageformats\qgif.dll"
Delete "$INSTDIR\imageformats\qjpeg.dll"
Delete "$INSTDIR\imageformats\qsvg.dll"
Delete "$INSTDIR\imageformats\qtiff.dll"
Delete "$INSTDIR\driver\blinkinlabs.cat"
Delete "$INSTDIR\driver\blinkinlabs.inf"
Delete "$INSTDIR\driver\dpinst32.exe"
Delete "$INSTDIR\driver\dpinst64.exe"
 
RmDir "$INSTDIR\driver"
RmDir "$INSTDIR\imageformats"
RmDir "$INSTDIR\platforms"
 
Delete "$INSTDIR\uninstall.exe"
!ifdef WEB_SITE
Delete "$INSTDIR\${APP_NAME} website.url"
!endif

RmDir "$INSTDIR"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_GETFOLDER "Application" $SM_Folder
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\$SM_Folder"
!endif

!ifndef REG_START_MENU
Delete "$SMPROGRAMS\Pattern Paint\${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\Pattern Paint\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\Pattern Paint"
!endif

DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################

