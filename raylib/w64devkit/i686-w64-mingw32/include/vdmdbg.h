/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _VDMDBG_
#define _VDMDBG_

#ifdef __cplusplus
extern "C" {
#endif

#include <pshpack4.h>

#define STATUS_VDM_EVENT STATUS_SEGMENT_NOTIFICATION

#ifndef DBG_SEGLOAD
#define DBG_SEGLOAD 0
#define DBG_SEGMOVE 1
#define DBG_SEGFREE 2
#define DBG_MODLOAD 3
#define DBG_MODFREE 4
#define DBG_SINGLESTEP 5
#define DBG_BREAK 6
#define DBG_GPFAULT 7
#define DBG_DIVOVERFLOW 8
#define DBG_INSTRFAULT 9
#define DBG_TASKSTART 10
#define DBG_TASKSTOP 11
#define DBG_DLLSTART 12
#define DBG_DLLSTOP 13
#define DBG_ATTACH 14
#define DBG_TOOLHELP 15
#define DBG_STACKFAULT 16
#define DBG_WOWINIT 17
#define DBG_TEMPBP 18
#define DBG_MODMOVE 19
#define DBG_INIT 20
#define DBG_GPFAULT2 21
#endif

#define VDMEVENT_NEEDS_INTERACTIVE 0x8000
#define VDMEVENT_VERBOSE 0x4000
#define VDMEVENT_PE 0x2000
#define VDMEVENT_ALLFLAGS 0xe000

#define VDMEVENT_V86 0x0001
#define VDMEVENT_PM16 0x0002

#define VDMCONTEXT_i386 0x00010000
#define VDMCONTEXT_i486 0x00010000

#define VDMCONTEXT_CONTROL (VDMCONTEXT_i386 | __MSABI_LONG(0x00000001))
#define VDMCONTEXT_INTEGER (VDMCONTEXT_i386 | __MSABI_LONG(0x00000002))
#define VDMCONTEXT_SEGMENTS (VDMCONTEXT_i386 | __MSABI_LONG(0x00000004))
#define VDMCONTEXT_FLOATING_POINT (VDMCONTEXT_i386 | __MSABI_LONG(0x00000008))
#define VDMCONTEXT_DEBUG_REGISTERS (VDMCONTEXT_i386 | __MSABI_LONG(0x00000010))
#define VDMCONTEXT_EXTENDED_REGISTERS (VDMCONTEXT_i386 | __MSABI_LONG(0x00000020))

#define VDMCONTEXT_FULL (VDMCONTEXT_CONTROL | VDMCONTEXT_INTEGER | VDMCONTEXT_SEGMENTS)

#ifdef _X86_

  typedef struct _CONTEXT VDMCONTEXT;
  typedef struct _LDT_ENTRY VDMLDT_ENTRY;
#else

#define SIZE_OF_80387_REGISTERS 80

  typedef struct _FLOATING_SAVE_AREA {
    ULONG ControlWord;
    ULONG StatusWord;
    ULONG TagWord;
    ULONG ErrorOffset;
    ULONG ErrorSelector;
    ULONG DataOffset;
    ULONG DataSelector;
    UCHAR RegisterArea[SIZE_OF_80387_REGISTERS];
    ULONG Cr0NpxState;
  } FLOATING_SAVE_AREA;

  typedef struct _VDMCONTEXT {
    ULONG ContextFlags;

    ULONG Dr0;
    ULONG Dr1;
    ULONG Dr2;
    ULONG Dr3;
    ULONG Dr6;
    ULONG Dr7;

    FLOATING_SAVE_AREA FloatSave;
    ULONG SegGs;
    ULONG SegFs;
    ULONG SegEs;
    ULONG SegDs;
    ULONG Edi;
    ULONG Esi;
    ULONG Ebx;
    ULONG Edx;
    ULONG Ecx;
    ULONG Eax;
    ULONG Ebp;
    ULONG Eip;
    ULONG SegCs;
    ULONG EFlags;
    ULONG Esp;
    ULONG SegSs;
  } VDMCONTEXT;

  typedef struct _VDMLDT_ENTRY {
    USHORT LimitLow;
    USHORT BaseLow;
    union {
      struct {
	UCHAR BaseMid;
	UCHAR Flags1;
	UCHAR Flags2;
	UCHAR BaseHi;
      } Bytes;
      struct {
	ULONG BaseMid : 8;
	ULONG Type : 5;
	ULONG Dpl : 2;
	ULONG Pres : 1;
	ULONG LimitHi : 4;
	ULONG Sys : 1;
	ULONG Reserved_0 : 1;
	ULONG Default_Big : 1;
	ULONG Granularity : 1;
	ULONG BaseHi : 8;
      } Bits;
    } HighWord;
  } VDMLDT_ENTRY;
#endif

  typedef VDMCONTEXT *LPVDMCONTEXT;
  typedef VDMLDT_ENTRY *LPVDMLDT_ENTRY;

#define VDMCONTEXT_TO_PROGRAM_COUNTER(Context) (PVOID)((Context)->Eip)

#define VDMCONTEXT_LENGTH (sizeof(VDMCONTEXT))
#define VDMCONTEXT_ALIGN (sizeof(ULONG))
#define VDMCONTEXT_ROUND (VDMCONTEXT_ALIGN - 1)

#define V86FLAGS_CARRY 0x00001
#define V86FLAGS_PARITY 0x00004
#define V86FLAGS_AUXCARRY 0x00010
#define V86FLAGS_ZERO 0x00040
#define V86FLAGS_SIGN 0x00080
#define V86FLAGS_TRACE 0x00100
#define V86FLAGS_INTERRUPT 0x00200
#define V86FLAGS_DIRECTION 0x00400
#define V86FLAGS_OVERFLOW 0x00800
#define V86FLAGS_IOPL 0x03000
#define V86FLAGS_IOPL_BITS 0x12
#define V86FLAGS_RESUME 0x10000
#define V86FLAGS_V86 0x20000
#define V86FLAGS_ALIGNMENT 0x40000

#define MAX_MODULE_NAME 8 + 1
#define MAX_PATH16 255

  typedef struct _SEGMENT_NOTE {
    WORD Selector1;
    WORD Selector2;
    WORD Segment;
    CHAR Module[MAX_MODULE_NAME+1];
    CHAR FileName[MAX_PATH16+1];
    WORD Type;
    DWORD Length;
  } SEGMENT_NOTE;

  typedef struct _IMAGE_NOTE {
    CHAR Module[MAX_MODULE_NAME+1];
    CHAR FileName[MAX_PATH16+1];
    WORD hModule;
    WORD hTask;
  } IMAGE_NOTE;

  typedef struct {
    DWORD dwSize;
    char szModule[MAX_MODULE_NAME+1];
    HANDLE hModule;
    WORD wcUsage;
    char szExePath[MAX_PATH16+1];
    WORD wNext;
  } MODULEENTRY,*LPMODULEENTRY;

#define SN_CODE 0
#define SN_DATA 1
#define SN_V86 2

  typedef struct _TEMP_BP_NOTE {
    WORD Seg;
    DWORD Offset;
    WINBOOL bPM;
  } TEMP_BP_NOTE;

  typedef struct _VDM_SEGINFO {
    WORD Selector;
    WORD SegNumber;
    DWORD Length;
    WORD Type;
    CHAR ModuleName[MAX_MODULE_NAME];
    CHAR FileName[MAX_PATH16];
  } VDM_SEGINFO;

#define GLOBAL_ALL 0
#define GLOBAL_LRU 1
#define GLOBAL_FREE 2

#define GT_UNKNOWN 0
#define GT_DGROUP 1
#define GT_DATA 2
#define GT_CODE 3
#define GT_TASK 4
#define GT_RESOURCE 5
#define GT_MODULE 6
#define GT_FREE 7
#define GT_INTERNAL 8
#define GT_SENTINEL 9
#define GT_BURGERMASTER 10

#define GD_USERDEFINED 0
#define GD_CURSORCOMPONENT 1
#define GD_BITMAP 2
#define GD_ICONCOMPONENT 3
#define GD_MENU 4
#define GD_DIALOG 5
#define GD_STRING 6
#define GD_FONTDIR 7
#define GD_FONT 8
#define GD_ACCELERATORS 9
#define GD_RCDATA 10
#define GD_ERRTABLE 11
#define GD_CURSOR 12
#define GD_ICON 14
#define GD_NAMETABLE 15
#define GD_MAX_RESOURCE 15

  typedef struct {
    DWORD dwSize;
    DWORD dwAddress;
    DWORD dwBlockSize;
    HANDLE hBlock;
    WORD wcLock;
    WORD wcPageLock;
    WORD wFlags;
    WINBOOL wHeapPresent;
    HANDLE hOwner;
    WORD wType;
    WORD wData;
    DWORD dwNext;
    DWORD dwNextAlt;
  } GLOBALENTRY,*LPGLOBALENTRY;

  typedef DWORD (CALLBACK *DEBUGEVENTPROC)(LPDEBUG_EVENT,LPVOID);

#define W1(x) ((USHORT)(x.ExceptionInformation[0]))
#define W2(x) ((USHORT)(x.ExceptionInformation[0] >> 16))
#define W3(x) ((USHORT)(x.ExceptionInformation[1]))
#define W4(x) ((USHORT)(x.ExceptionInformation[1] >> 16))
#define DW3(x) (x.ExceptionInformation[2])
#define DW4(x) (x.ExceptionInformation[3])

#include <poppack.h>

  WINBOOL WINAPI VDMProcessException(LPDEBUG_EVENT lpDebugEvent);
  WINBOOL WINAPI VDMGetThreadSelectorEntry(HANDLE hProcess,HANDLE hThread,WORD wSelector,LPVDMLDT_ENTRY lpSelectorEntry);
  ULONG WINAPI VDMGetPointer(HANDLE hProcess,HANDLE hThread,WORD wSelector,DWORD dwOffset,WINBOOL fProtMode);
  WINBOOL WINAPI VDMGetContext(HANDLE hProcess,HANDLE hThread,LPVDMCONTEXT lpVDMContext);
  WINBOOL WINAPI VDMSetContext(HANDLE hProcess,HANDLE hThread,LPVDMCONTEXT lpVDMContext);
  WINBOOL WINAPI VDMGetSelectorModule(HANDLE hProcess,HANDLE hThread,WORD wSelector,PUINT lpSegmentNumber,LPSTR lpModuleName,UINT nNameSize,LPSTR lpModulePath,UINT nPathSize);
  WINBOOL WINAPI VDMGetModuleSelector(HANDLE hProcess,HANDLE hThread,UINT wSegmentNumber,LPSTR lpModuleName,LPWORD lpSelector);
  WINBOOL WINAPI VDMModuleFirst(HANDLE hProcess,HANDLE hThread,LPMODULEENTRY lpModuleEntry,DEBUGEVENTPROC lpEventProc,LPVOID lpData);
  WINBOOL WINAPI VDMModuleNext(HANDLE hProcess,HANDLE hThread,LPMODULEENTRY lpModuleEntry,DEBUGEVENTPROC lpEventProc,LPVOID lpData);
  WINBOOL WINAPI VDMGlobalFirst(HANDLE hProcess,HANDLE hThread,LPGLOBALENTRY lpGlobalEntry,WORD wFlags,DEBUGEVENTPROC lpEventProc,LPVOID lpData);
  WINBOOL WINAPI VDMGlobalNext(HANDLE hProcess,HANDLE hThread,LPGLOBALENTRY lpGlobalEntry,WORD wFlags,DEBUGEVENTPROC lpEventProc,LPVOID lpData);

  typedef WINBOOL (WINAPI *PROCESSENUMPROC)(DWORD dwProcessId,DWORD dwAttributes,LPARAM lpUserDefined);
  typedef WINBOOL (WINAPI *TASKENUMPROC)(DWORD dwThreadId,WORD hMod16,WORD hTask16,LPARAM lpUserDefined);
  typedef WINBOOL (WINAPI *TASKENUMPROCEX)(DWORD dwThreadId,WORD hMod16,WORD hTask16,PSZ pszModName,PSZ pszFileName,LPARAM lpUserDefined);

#define WOW_SYSTEM (DWORD)0x0001

  INT WINAPI VDMEnumProcessWOW(PROCESSENUMPROC fp,LPARAM lparam);
  INT WINAPI VDMEnumTaskWOW(DWORD dwProcessId,TASKENUMPROC fp,LPARAM lparam);
  INT WINAPI VDMEnumTaskWOWEx(DWORD dwProcessId,TASKENUMPROCEX fp,LPARAM lparam);
  WINBOOL WINAPI VDMTerminateTaskWOW(DWORD dwProcessId,WORD htask);
  WINBOOL WINAPI VDMStartTaskInWOW(DWORD dwProcessId,LPSTR lpCommandLine,WORD wShow);
  WINBOOL WINAPI VDMKillWOW(VOID);
  WINBOOL WINAPI VDMDetectWOW(VOID);
  WINBOOL WINAPI VDMBreakThread(HANDLE hProcess,HANDLE hThread);
  DWORD WINAPI VDMGetDbgFlags(HANDLE hProcess);
  WINBOOL WINAPI VDMSetDbgFlags(HANDLE hProcess,DWORD dwFlags);

#define VDMDBG_BREAK_DOSTASK 0x00000001
#define VDMDBG_BREAK_WOWTASK 0x00000002
#define VDMDBG_BREAK_LOADDLL 0x00000004
#define VDMDBG_BREAK_EXCEPTIONS 0x00000008
#define VDMDBG_BREAK_DEBUGGER 0x00000010
#define VDMDBG_TRACE_HISTORY 0x00000080

  WINBOOL WINAPI VDMIsModuleLoaded(LPSTR szPath);
  WINBOOL WINAPI VDMGetSegmentInfo(WORD Selector,ULONG Offset,WINBOOL bProtectMode,VDM_SEGINFO *pSegInfo);
  WINBOOL WINAPI VDMGetSymbol(LPSTR szModule,WORD SegNumber,DWORD Offset,WINBOOL bProtectMode,WINBOOL bNextSymbol,LPSTR szSymbolName,PDWORD pDisplacement);
  WINBOOL WINAPI VDMGetAddrExpression(LPSTR szModule,LPSTR szSymbol,PWORD Selector,PDWORD Offset,PWORD Type);

#define VDMADDR_V86 2
#define VDMADDR_PM16 4
#define VDMADDR_PM32 16

  typedef WINBOOL (WINAPI *VDMPROCESSEXCEPTIONPROC)(LPDEBUG_EVENT);
  typedef WINBOOL (WINAPI *VDMGETTHREADSELECTORENTRYPROC)(HANDLE,HANDLE,DWORD,LPVDMLDT_ENTRY);
  typedef ULONG (WINAPI *VDMGETPOINTERPROC)(HANDLE,HANDLE,WORD,DWORD,WINBOOL);
  typedef WINBOOL (WINAPI *VDMGETCONTEXTPROC)(HANDLE,HANDLE,LPVDMCONTEXT);
  typedef WINBOOL (WINAPI *VDMSETCONTEXTPROC)(HANDLE,HANDLE,LPVDMCONTEXT);
  typedef WINBOOL (WINAPI *VDMKILLWOWPROC)(VOID);
  typedef WINBOOL (WINAPI *VDMDETECTWOWPROC)(VOID);
  typedef WINBOOL (WINAPI *VDMBREAKTHREADPROC)(HANDLE);
  typedef WINBOOL (WINAPI *VDMGETSELECTORMODULEPROC)(HANDLE,HANDLE,WORD,PUINT,LPSTR,UINT,LPSTR,UINT);
  typedef WINBOOL (WINAPI *VDMGETMODULESELECTORPROC)(HANDLE,HANDLE,UINT,LPSTR,LPWORD);
  typedef WINBOOL (WINAPI *VDMMODULEFIRSTPROC)(HANDLE,HANDLE,LPMODULEENTRY,DEBUGEVENTPROC,LPVOID);
  typedef WINBOOL (WINAPI *VDMMODULENEXTPROC)(HANDLE,HANDLE,LPMODULEENTRY,DEBUGEVENTPROC,LPVOID);
  typedef WINBOOL (WINAPI *VDMGLOBALFIRSTPROC)(HANDLE,HANDLE,LPGLOBALENTRY,WORD,DEBUGEVENTPROC,LPVOID);
  typedef WINBOOL (WINAPI *VDMGLOBALNEXTPROC)(HANDLE,HANDLE,LPGLOBALENTRY,WORD,DEBUGEVENTPROC,LPVOID);
  typedef INT (WINAPI *VDMENUMPROCESSWOWPROC)(PROCESSENUMPROC,LPARAM);
  typedef INT (WINAPI *VDMENUMTASKWOWPROC)(DWORD,TASKENUMPROC,LPARAM);
  typedef INT (WINAPI *VDMENUMTASKWOWEXPROC)(DWORD,TASKENUMPROCEX,LPARAM);
  typedef WINBOOL (WINAPI *VDMTERMINATETASKINWOWPROC)(DWORD,WORD);
  typedef WINBOOL (WINAPI *VDMSTARTTASKINWOWPROC)(DWORD,LPSTR,WORD);
  typedef DWORD (WINAPI *VDMGETDBGFLAGSPROC)(HANDLE);
  typedef WINBOOL (WINAPI *VDMSETDBGFLAGSPROC)(HANDLE,DWORD);
  typedef WINBOOL (WINAPI *VDMISMODULELOADEDPROC)(LPSTR);
  typedef WINBOOL (WINAPI *VDMGETSEGMENTINFOPROC)(WORD,ULONG,WINBOOL,VDM_SEGINFO);
  typedef WINBOOL (WINAPI *VDMGETSYMBOLPROC)(LPSTR,WORD,DWORD,WINBOOL,WINBOOL,LPSTR,PDWORD);
  typedef WINBOOL (WINAPI *VDMGETADDREXPRESSIONPROC)(LPSTR,LPSTR,PWORD,PDWORD,PWORD);

#ifdef __cplusplus
}
#endif
#endif
