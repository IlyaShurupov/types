
#include "stacktrace.h"

#include "env.h"

#ifdef PLATFORM_WINDOWS

#include <Windows.h>
#include <DbgHelp.h>
#include <Winbase.h>

#include <stdio.h>

#include <iostream>

#include "common.h"

//#pragma comment(lib, "Dbghelp.lib")

void printStack() {

  typedef USHORT(WINAPI* CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
  CaptureStackBackTraceType func = (CaptureStackBackTraceType)(GetProcAddress(LoadLibrary(L"kernel32.dll"), "RtlCaptureStackBackTrace"));

  if (func == NULL)
    return; // WOE 29.SEP.2010

// Quote from Microsoft Documentation:
// ## Windows Server 2003 and Windows XP:  
// ## The sum of the FramesToSkip and FramesToCapture parameters must be less than 63.
  const int kMaxCallers = 62;

  void* callers_stack[kMaxCallers];
  unsigned short frames;
  SYMBOL_INFO* symbol;
  HANDLE         process;
  process = GetCurrentProcess();
  SymSetOptions(SYMOPT_LOAD_LINES);
  SymInitialize(process, NULL, TRUE);
  frames = (func)(0, kMaxCallers, callers_stack, NULL);
  symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
  symbol->MaxNameLen = 255;
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);


  PIMAGEHLP_LINE64 line = 0;
  line = (PIMAGEHLP_LINE64)calloc(sizeof(IMAGEHLP_LINE64), 1);
  line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
  DWORD  dwDisplacement = 0;

  const unsigned short  MAX_CALLERS_SHOWN = 16;
  frames = frames < MAX_CALLERS_SHOWN ? frames : MAX_CALLERS_SHOWN;
  for (unsigned int i = 0; i < frames; i++)
  {
    SymFromAddr(process, (DWORD64)(callers_stack[i]), 0, symbol);
    BOOL ok = SymGetLineFromAddr64(process, (DWORD64)(callers_stack[i]), &dwDisplacement, line);
    std::cout << "*** " << i << ": " << callers_stack[i] << " " << symbol->Name << " - 0x" << symbol->Address << " file - " << line->FileName << " line - " << line->LineNumber << std::endl;
  }

  free(line);
  free(symbol);
}

#endif