//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: File-handling for OpenHoldem
//
//******************************************************************************

#define FILES_DLL_EXPORTS

#include "files.h"
#include <assert.h>

const char* k_default_ini_filename = "OpenHoldem_Preferences__feel_free_to_rename_this_file_to_whatever_you_like.INI";

CFilenames() { //!!!!! ti dll entry
  // Save directory that contains openHoldem for future use.
  // This is NOT GetCurrentDirectory(), which returns the working-directorz,
  // i.e. the working directorz of the process that started OpenHoldem.
  //  
  // If we don't get this path right and the user stars OpenHoldem
  // from a different directorz with a script we won't find our files.
  // http://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
  // http://stackoverflow.com/questions/4517425/how-to-get-program-path
  ::GetModuleFileName(NULL, _startup_path, _MAX_PATH);
  // Remove executable filename from path
  // http://pic.dhe.ibm.com/infocenter/zvm/v6r2/index.jsp?topic=%2Fcom.ibm.zvm.v620.edclv%2Fstrrchr.htm
  char *last_backslash_in_path = strrchr(_startup_path, '\\');
  if (last_backslash_in_path != NULL) {
    ++last_backslash_in_path;
    *last_backslash_in_path = '\0';
  }
  // Create logs-directoy if necessary
  CreateDirectory(LogsDirectory(), NULL);
}

CString OpenHoldemDirectory() {
  assert(_startup_path != "");
  return _startup_path;
}

CString BotlogicDirectory() {
  assert(_startup_path != "");
  CString bot_logic_dir = CString(_startup_path) + "bot_logic\\";
  return bot_logic_dir;
}

CString DefaultLogicDirectory() {
  return BotlogicDirectory() + "DefaultBot\\";
}

CString IniFilePath() {
  // We need the complete path here,
  // otherwise the file would be expected in the default location,
  // i.e. the Windows-directory.
  return OpenHoldemDirectory() + CString("\\") + IniFilename();
}

CString IniFilename() {
  WIN32_FIND_DATA find_file_data;
  HANDLE h_find;
  CString wildcard = OpenHoldemDirectory() + "\\*.INI";
  CString ini_filename;

  // Try to find "first" ini-file.
  // No matter how it is named -- it is the right one.
  // http://msdn.microsoft.com/en-us/library/windows/desktop/aa364418(v=vs.85).aspx
  h_find = FindFirstFile(wildcard, &find_file_data);
  if (h_find == INVALID_HANDLE_VALUE) {
    // No ini-file found.
    // Use default one (for saving).
    FindClose(h_find);

    return k_default_ini_filename;
  }
  ini_filename = find_file_data.cFileName;
  // Check that no more ini-files exist (potential problems).
  // http://msdn.microsoft.com/en-us/library/windows/desktop/aa364428(v=vs.85).aspx
  if (FindNextFile(h_find, &find_file_data)) {
    FindClose(h_find);
    OH_MessageBox_Error_Warning(
      "More than one ini-file in OpenHoldem-directory.\n"
      "Don't know which one to use.\n"
      "\n"
      "Going to terminate...");
    PostQuitMessage(-1);
    // Previously we returned "a_result_to_make_the_compiler_happy.ini"
    // believing the result was meaning-less and would never be used.
    // However PostQuitMessage(-1) uses a message-queue,
    // so it may take some time, and OH will continue a little bit.
    // Therefore we take the last filename now in order to 
    // not create yet another one with a funny name. ;-)
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=16229
    return ini_filename;
  }
  FindClose(h_find);
  // Exactly one ini-file found
  return ini_filename;
}

void SwitchToOpenHoldemDirectory() {
  assert(_startup_path != "");
  SetCurrentDirectory(_startup_path);
}

CString OpenPPLLibraryDirectory() {
  return BotlogicDirectory() + "OpenPPL_Library\\";
}

CString TableMapWildcard() {
  assert(_startup_path != "");
  CString wildcard;
  wildcard.Format("%sscraper\\*.tm", _startup_path);
  return wildcard;
}

CString ScraperDirectory() {
  assert(_startup_path != "");
  CString scraper_dir = CString(_startup_path) + "scraper\\";
  return scraper_dir;
}

CString ToolsDirectory() {
  assert(_startup_path != "");
  CString tools_dir = CString(_startup_path) + "tools\\";

  return tools_dir;
}

CString ReplaySessionDirectory() {
  assert(_startup_path != "");
  CString path;
  path.Format("%sreplay\\session_%lu\\", _startup_path, p_sessioncounter->session_id());
  return path;
}

CString ReplayBitmapFilename(int frame_number) {
  AssertRange(frame_number, 0, preferences.replay_max_frames());
  CString path;
  path.Format("%sframe%06d.bmp", ReplaySessionDirectory(), frame_number);
  return path;
}

CString ReplayHTMLFilename(int frame_number) {
  AssertRange(frame_number, 0, preferences.replay_max_frames());
  CString path;
  path.Format("%sframe%06d.htm", ReplaySessionDirectory(), frame_number);
  return path;
}

CString LogsDirectory() {
  assert(_startup_path != "");
  CString path;
  path.Format("%slogs\\", _startup_path);
  return path;
};

CString LogFilePath() {
  CString path;
  path.Format("%soh_%lu.log", LogsDirectory(), p_sessioncounter->session_id());
  return path;
}

CString MiniDumpFilename() {
  SYSTEMTIME	stLocalTime;
  char		szFileName[MAX_PATH];

  GetLocalTime(&stLocalTime);
  assert(_startup_path != "");
  sprintf_s(szFileName, MAX_PATH, "%s\\%s-%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
    _startup_path, "OpenHoldem", VERSION_TEXT, stLocalTime.wYear, stLocalTime.wMonth,
    stLocalTime.wDay, stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
    GetCurrentProcessId(), GetCurrentThreadId());
  return szFileName;
}

CString PathOfExecutable() {
  // http://msdn.microsoft.com/en-us/library/windows/desktop/ms683197(v=vs.85).aspx
  // http://stackoverflow.com/questions/124886/how-to-get-the-application-executable-name-in-windows-c-win32-or-c-cli
  TCHAR exe_path[MAX_PATH + 1];
  GetModuleFileName(0, exe_path, MAX_PATH + 1);
  return exe_path;
}

CString ExecutableFilename() {
  return FilenameWithoutPath(PathOfExecutable());
}

CString PureExecutableFilename() {
  return FilenameWithoutPathAndExtension(ExecutableFilename());
}

CString VersusPath() {
  CString result = OpenHoldemDirectory() + "versus.bin";
  return result;
}

CString CustomLibraryPath() {
  CString result = BotlogicDirectory() + "\\custom_function_library.ohf";
  return result;
}

CString ManualModePath() {
  CString result = ToolsDirectory() + "\\ManualMode.exe";
  return result;
}

bool Exists(CString filename_or_pattern) {
  // https://msdn.microsoft.com/en-us/library/windows/desktop/aa364418%28v=vs.85%29.aspx
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;
  hFind = FindFirstFile(filename_or_pattern, &FindFileData);
  if (hFind == INVALID_HANDLE_VALUE) return false;
  FindClose(hFind);
  return true;
}

CString FilenameWithoutPath(CString path) {
  int pos = path.ReverseFind('\\');
  if (pos > 0) {
    assert(pos < path.GetLength() - 1);
    return path.Mid(pos + 1);
  }
  return path;
}

CString FilenameWithoutPathAndExtension(CString path) {
  CString filename = FilenameWithoutPath(path);
  int pos = filename.ReverseFind('.');
  CString result;
  if (pos > 0) {
    result = filename.Left(pos);
  }
  else {
    result = filename;
  }
  return result;
}









#include "debug.h"
#include "afxwin.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include <time.h>
#include "..\..\Shared\CCritSec\CCritSec.h"
#include "..\..\Shared\MagicNumbers\MagicNumbers.h"
