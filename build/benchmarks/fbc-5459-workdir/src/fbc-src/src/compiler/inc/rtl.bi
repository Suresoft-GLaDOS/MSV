''	FreeBASIC - 32-bit BASIC Compiler.
''	Copyright (C) 2004-2010 The FreeBASIC development team.
''
''	This program is free software; you can redistribute it and/or modify
''	it under the terms of the GNU General Public License as published by
''	the Free Software Foundation; either version 2 of the License, or
''	(at your option) any later version.
''
''	This program is distributed in the hope that it will be useful,
''	but WITHOUT ANY WARRANTY; without even the implied warranty of
''	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
''	GNU General Public License for more details.
''
''	You should have received a copy of the GNU General Public License
''	along with this program; if not, write to the Free Software
''	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA.


#include once "inc\ast.bi"
#include once "inc\fbint.bi"

'' NOTE: when adding any RTL proc that will be called from rtl*.bas,
'' always update the FB_RTL_IDX enum below

#define FB_RTL_STRINIT					"fb_StrInit"
#define FB_RTL_STRDELETE				"fb_StrDelete"
#define FB_RTL_STRASSIGN				"fb_StrAssign"
#define FB_RTL_STRCONCAT 				"fb_StrConcat"
#define FB_RTL_STRCOMPARE				"fb_StrCompare"
#define FB_RTL_STRCONCATASSIGN			"fb_StrConcatAssign"
#define FB_RTL_STRALLOCTMPRES			"fb_StrAllocTempResult"
#define FB_RTL_STRALLOCTMPDESCV			"fb_StrAllocTempDescV"
#define FB_RTL_STRALLOCTMPDESCF			"fb_StrAllocTempDescF"
#define FB_RTL_STRALLOCTMPDESCZ			"fb_StrAllocTempDescZ"
#define FB_RTL_STRALLOCTMPDESCZEX		"fb_StrAllocTempDescZEx"

#define FB_RTL_INT2STR					"fb_IntToStr"
#define FB_RTL_UINT2STR					"fb_UIntToStr"
#define FB_RTL_LONGINT2STR				"fb_LongintToStr"
#define FB_RTL_ULONGINT2STR				"fb_ULongintToStr"
#define FB_RTL_FLT2STR					"fb_FloatToStr"
#define FB_RTL_DBL2STR					"fb_DoubleToStr"
#define FB_RTL_WSTR2STR					"fb_WstrToStr"

#define FB_RTL_INT2STR_QB				"fb_IntToStrQB"
#define FB_RTL_UINT2STR_QB				"fb_UIntToStrQB"
#define FB_RTL_LONGINT2STR_QB			"fb_LongintToStrQB"
#define FB_RTL_ULONGINT2STR_QB			"fb_ULongintToStrQB"
#define FB_RTL_FLT2STR_QB				"fb_FloatToStrQB"
#define FB_RTL_DBL2STR_QB				"fb_DoubleToStrQB"

#define FB_RTL_STR2INT					"valint"
#define FB_RTL_STR2UINT					"valuint"
#define FB_RTL_STR2LNG					"vallng"
#define FB_RTL_STR2ULNG					"valulng"
#define FB_RTL_STR2DBL					"val"

#define FB_RTL_MKD                      "fb_MKD"
#define FB_RTL_MKS                      "fb_MKS"
#define FB_RTL_MKI                      "fb_MKI"
#define FB_RTL_MKL                      "fb_MKL"
#define FB_RTL_MKSHORT                  "fb_MKSHORT"
#define FB_RTL_MKLONGINT                "fb_MKLONGINT"

#define FB_RTL_CVD                      "fb_CVD"
#define FB_RTL_CVS                      "fb_CVS"
#define FB_RTL_CVI                      "fb_CVI"
#define FB_RTL_CVL                      "fb_CVL"
#define FB_RTL_CVSHORT                  "fb_CVSHORT"
#define FB_RTL_CVLONGINT                "fb_CVLONGINT"

#define FB_RTL_STRMID					"fb_StrMid"
#define FB_RTL_STRASSIGNMID				"fb_StrAssignMid"
#define FB_RTL_STRFILL1					"fb_StrFill1"
#define FB_RTL_STRFILL2 				"fb_StrFill2"
#define FB_RTL_STRLEN 					"fb_StrLen"
#define FB_RTL_STRLSET 					"fb_StrLset"
#define FB_RTL_STRRSET 					"fb_StrRset"
#define FB_RTL_STRASC 					"fb_ASC"
#define FB_RTL_STRCHR 					"fb_CHR"
#define FB_RTL_STRINSTR 				"fb_StrInstr"
#define FB_RTL_STRINSTRANY				"fb_StrInstrAny"
#define FB_RTL_STRINSTRREV 				"fb_StrInstrRev"
#define FB_RTL_STRINSTRREVANY			"fb_StrInstrRevAny"
#define FB_RTL_STRTRIM 					"fb_TRIM"
#define FB_RTL_STRTRIMANY 				"fb_TrimAny"
#define FB_RTL_STRTRIMEX 				"fb_TrimEx"
#define FB_RTL_STRRTRIM 				"fb_RTRIM"
#define FB_RTL_STRRTRIMANY 				"fb_RTrimAny"
#define FB_RTL_STRRTRIMEX 				"fb_RTrimEx"
#define FB_RTL_STRLTRIM 				"fb_LTRIM"
#define FB_RTL_STRLTRIMANY 				"fb_LTrimAny"
#define FB_RTL_STRLTRIMEX 				"fb_LTrimEx"
#define FB_RTL_STRSWAP 					"fb_StrSwap"

#define FB_RTL_WSTRALLOC				"fb_WstrAlloc"
#define FB_RTL_WSTRDELETE				"fb_WstrDelete"
#define FB_RTL_WSTRASSIGN				"fb_WstrAssign"
#define FB_RTL_WSTRASSIGNWA				"fb_WstrAssignFromA"
#define FB_RTL_WSTRASSIGNAW				"fb_WstrAssignToA"
#define FB_RTL_WSTRASSIGNAW_INIT		"fb_WstrAssignToA_Init"
#define FB_RTL_WSTRCONCAT 				"fb_WstrConcat"
#define FB_RTL_WSTRCONCATWA				"fb_WstrConcatWA"
#define FB_RTL_WSTRCONCATAW				"fb_WstrConcatAW"
#define FB_RTL_WSTRCOMPARE				"fb_WstrCompare"
#define FB_RTL_WSTRCONCATASSIGN			"fb_WstrConcatAssign"

#define FB_RTL_INT2WSTR					"fb_IntToWstr"
#define FB_RTL_UINT2WSTR				"fb_UIntToWstr"
#define FB_RTL_LONGINT2WSTR				"fb_LongintToWstr"
#define FB_RTL_ULONGINT2WSTR			"fb_ULongintToWstr"
#define FB_RTL_FLT2WSTR					"fb_FloatToWstr"
#define FB_RTL_DBL2WSTR					"fb_DoubleToWstr"
#define FB_RTL_STR2WSTR					"fb_StrToWstr"

#define FB_RTL_WSTRMID					"fb_WstrMid"
#define FB_RTL_WSTRASSIGNMID			"fb_WstrAssignMid"
#define FB_RTL_WSTRFILL1				"fb_WstrFill1"
#define FB_RTL_WSTRFILL2 				"fb_WstrFill2"
#define FB_RTL_WSTRLEN 					"fb_WstrLen"
#define FB_RTL_WSTRLSET 				"fb_WstrLset"
#define FB_RTL_WSTRRSET 				"fb_WstrRset"
#define FB_RTL_WSTRASC 					"fb_WstrAsc"
#define FB_RTL_WSTRCHR 					"fb_WstrChr"
#define FB_RTL_WSTRINSTR 				"fb_WstrInstr"
#define FB_RTL_WSTRINSTRANY				"fb_WstrInstrAny"
#define FB_RTL_WSTRINSTRREV 			"fb_WstrInstrRev"
#define FB_RTL_WSTRINSTRREVANY			"fb_WstrInstrRevAny"
#define FB_RTL_WSTRTRIM 				"fb_WstrTrim"
#define FB_RTL_WSTRTRIMANY 				"fb_WstrTrimAny"
#define FB_RTL_WSTRTRIMEX 				"fb_WstrTrimEx"
#define FB_RTL_WSTRRTRIM 				"fb_WstrRTrim"
#define FB_RTL_WSTRRTRIMANY 			"fb_WstrRTrimAny"
#define FB_RTL_WSTRRTRIMEX 				"fb_WstrRTrimEx"
#define FB_RTL_WSTRLTRIM 				"fb_WstrLTrim"
#define FB_RTL_WSTRLTRIMANY 			"fb_WstrLTrimAny"
#define FB_RTL_WSTRLTRIMEX 				"fb_WstrLTrimEx"
#define FB_RTL_WSTRSWAP 				"fb_WstrSwap"

#define FB_RTL_LONGINTDIV				"__divdi3"
#define FB_RTL_ULONGINTDIV				"__udivdi3"
#define FB_RTL_LONGINTMOD				"__moddi3"
#define FB_RTL_ULONGINTMOD				"__umoddi3"
#define FB_RTL_DBL2ULONGINT				"__fixunsdfdi"

#define FB_RTL_ARRAYREDIM				"fb_ArrayRedimEx"
#define FB_RTL_ARRAYREDIM_OBJ			"fb_ArrayRedimObj"
#define FB_RTL_ARRAYREDIMPRESV			"fb_ArrayRedimPresvEx"
#define FB_RTL_ARRAYREDIMPRESV_OBJ		"fb_ArrayRedimPresvObj"
#define FB_RTL_ARRAYERASE				"fb_ArrayErase"
#define FB_RTL_ARRAYERASE_OBJ			"fb_ArrayEraseObj"
#define FB_RTL_ARRAYCLEAR				"fb_ArrayClear"
#define FB_RTL_ARRAYCLEAR_OBJ			"fb_ArrayClearObj"
#define FB_RTL_ARRAYLBOUND				"fb_ArrayLBound"
#define FB_RTL_ARRAYUBOUND				"fb_ArrayUBound"
#define FB_RTL_ARRAYSTRERASE			"fb_ArrayStrErase"
#define FB_RTL_ARRAYSNGBOUNDCHK			"fb_ArraySngBoundChk"
#define FB_RTL_ARRAYBOUNDCHK			"fb_ArrayBoundChk"

#define FB_RTL_NULLPTRCHK				"fb_NullPtrChk"

#define FB_RTL_CPUDETECT 				"fb_CpuDetect"
#define FB_RTL_INIT 					"fb_Init"
#define FB_RTL_INITSIGNALS 				"fb_InitSignals"
#define FB_RTL_INITCRTCTOR 				"__main"
#define FB_RTL_END 						"fb_End"
#define FB_RTL_ATEXIT					"{atexit}"

#define FB_RTL_DATARESTORE 				"fb_DataRestore"
#define FB_RTL_DATAREADSTR 				"fb_DataReadStr"
#define FB_RTL_DATAREADWSTR 			"fb_DataReadWstr"
#define FB_RTL_DATAREADBYTE 			"fb_DataReadByte"
#define FB_RTL_DATAREADSHORT 			"fb_DataReadShort"
#define FB_RTL_DATAREADINT 				"fb_DataReadInt"
#define FB_RTL_DATAREADLONGINT 			"fb_DataReadLongint"
#define FB_RTL_DATAREADUBYTE 			"fb_DataReadUByte"
#define FB_RTL_DATAREADUSHORT 			"fb_DataReadUShort"
#define FB_RTL_DATAREADUINT 			"fb_DataReadUInt"
#define FB_RTL_DATAREADPTR 				"fb_DataReadPtr"
#define FB_RTL_DATAREADULONGINT 		"fb_DataReadULongint"
#define FB_RTL_DATAREADSINGLE 			"fb_DataReadSingle"
#define FB_RTL_DATAREADDOUBLE 			"fb_DataReadDouble"

#define FB_RTL_POW 						"fb_Pow"

#define FB_RTL_PRINTVOID 				"fb_PrintVoid"
#define FB_RTL_PRINTBYTE 				"fb_PrintByte"
#define FB_RTL_PRINTUBYTE 				"fb_PrintUByte"
#define FB_RTL_PRINTSHORT 				"fb_PrintShort"
#define FB_RTL_PRINTUSHORT 				"fb_PrintUShort"
#define FB_RTL_PRINTINT 				"fb_PrintInt"
#define FB_RTL_PRINTUINT 				"fb_PrintUInt"
#define FB_RTL_PRINTLONGINT 			"fb_PrintLongint"
#define FB_RTL_PRINTULONGINT 			"fb_PrintULongint"
#define FB_RTL_PRINTSINGLE 				"fb_PrintSingle"
#define FB_RTL_PRINTDOUBLE 				"fb_PrintDouble"
#define FB_RTL_PRINTSTR 				"fb_PrintString"
#define FB_RTL_PRINTWSTR 				"fb_PrintWstr"

#define FB_RTL_LPRINTVOID 				"fb_LPrintVoid"
#define FB_RTL_LPRINTBYTE 				"fb_LPrintByte"
#define FB_RTL_LPRINTUBYTE 				"fb_LPrintUByte"
#define FB_RTL_LPRINTSHORT 				"fb_LPrintShort"
#define FB_RTL_LPRINTUSHORT 			"fb_LPrintUShort"
#define FB_RTL_LPRINTINT 				"fb_LPrintInt"
#define FB_RTL_LPRINTUINT 				"fb_LPrintUInt"
#define FB_RTL_LPRINTLONGINT 			"fb_LPrintLongint"
#define FB_RTL_LPRINTULONGINT 			"fb_LPrintULongint"
#define FB_RTL_LPRINTSINGLE 			"fb_LPrintSingle"
#define FB_RTL_LPRINTDOUBLE 			"fb_LPrintDouble"
#define FB_RTL_LPRINTSTR 				"fb_LPrintString"
#define FB_RTL_LPRINTWSTR 				"fb_LPrintWstr"

#define FB_RTL_PRINTSPC 				"fb_PrintSPC"
#define FB_RTL_PRINTTAB 				"fb_PrintTab"

#define FB_RTL_WRITEVOID 				"fb_WriteVoid"
#define FB_RTL_WRITEBYTE 				"fb_WriteByte"
#define FB_RTL_WRITEUBYTE 				"fb_WriteUByte"
#define FB_RTL_WRITESHORT 				"fb_WriteShort"
#define FB_RTL_WRITEUSHORT 				"fb_WriteUShort"
#define FB_RTL_WRITEINT 				"fb_WriteInt"
#define FB_RTL_WRITEUINT 				"fb_WriteUInt"
#define FB_RTL_WRITELONGINT 			"fb_WriteLongint"
#define FB_RTL_WRITEULONGINT 			"fb_WriteULongint"
#define FB_RTL_WRITESINGLE 				"fb_WriteSingle"
#define FB_RTL_WRITEDOUBLE 				"fb_WriteDouble"
#define FB_RTL_WRITESTR 				"fb_WriteString"
#define FB_RTL_WRITEWSTR 				"fb_WriteWstr"

#define FB_RTL_PRINTUSGINIT 			"fb_PrintUsingInit"
#define FB_RTL_PRINTUSGSTR 				"fb_PrintUsingStr"
#define FB_RTL_PRINTUSGWSTR 			"fb_PrintUsingWstr"
#define FB_RTL_PRINTUSG_SNG 			"fb_PrintUsingSingle"
#define FB_RTL_PRINTUSG_DBL 			"fb_PrintUsingDouble"
#define FB_RTL_PRINTUSG_LL  			"fb_PrintUsingLongint"
#define FB_RTL_PRINTUSG_ULL 			"fb_PrintUsingULongint"
#define FB_RTL_PRINTUSGEND 				"fb_PrintUsingEnd"
#define FB_RTL_LPRINTUSGINIT 			"fb_LPrintUsingInit"

#define FB_RTL_CONSOLEVIEW 				"fb_ConsoleView"
#define FB_RTL_CONSOLEREADXY 			"fb_ReadXY"
#define FB_RTL_COLOR					"fb_Color"
#define FB_RTL_PAGESET					"fb_PageSet"

#define FB_RTL_MEMCOPY 					"fb_MemCopy"
#define FB_RTL_MEMSWAP 					"fb_MemSwap"
#define FB_RTL_MEMCOPYCLEAR 			"fb_MemCopyClear"

#define FB_RTL_FILEOPEN 				"fb_FileOpen"
#define FB_RTL_FILEOPEN_ENCOD			"fb_FileOpenEncod"
#define FB_RTL_FILEOPEN_SHORT 			"fb_FileOpenShort"
#define FB_RTL_FILEOPEN_CONS 			"fb_FileOpenCons"
#define FB_RTL_FILEOPEN_ERR 			"fb_FileOpenErr"
#define FB_RTL_FILEOPEN_PIPE 			"fb_FileOpenPipe"
#define FB_RTL_FILEOPEN_SCRN 			"fb_FileOpenScrn"
#define FB_RTL_FILEOPEN_LPT 			"fb_FileOpenLpt"
#define FB_RTL_FILEOPEN_COM 			"fb_FileOpenCom"
#define FB_RTL_FILEOPEN_QB  			"fb_FileOpenQB"
#define FB_RTL_FILECLOSE 				"fb_FileClose"

#define FB_RTL_FILEPUT 					"fb_FilePut"
#define FB_RTL_FILEPUTLARGE 			"fb_FilePutLarge"
#define FB_RTL_FILEPUTSTR 				"fb_FilePutStr"
#define FB_RTL_FILEPUTSTRLARGE 			"fb_FilePutStrLarge"
#define FB_RTL_FILEPUTWSTR 				"fb_FilePutWstr"
#define FB_RTL_FILEPUTWSTRLARGE 		"fb_FilePutWstrLarge"
#define FB_RTL_FILEPUTARRAY 			"fb_FilePutArray"
#define FB_RTL_FILEPUTARRAYLARGE		"fb_FilePutArrayLarge"

#define FB_RTL_FILEGET 					"fb_FileGet"
#define FB_RTL_FILEGETLARGE 			"fb_FileGetLarge"
#define FB_RTL_FILEGETSTR 				"fb_FileGetStr"
#define FB_RTL_FILEGETSTRLARGE 			"fb_FileGetStrLarge"
#define FB_RTL_FILEGETWSTR 				"fb_FileGetWstr"
#define FB_RTL_FILEGETWSTRLARGE 		"fb_FileGetWstrLarge"
#define FB_RTL_FILEGETARRAY 			"fb_FileGetArray"
#define FB_RTL_FILEGETARRAYLARGE 		"fb_FileGetArrayLarge"

#define FB_RTL_FILEGETIOB				"fb_FileGetIOB"
#define FB_RTL_FILEGETLARGEIOB 			"fb_FileGetLargeIOB"
#define FB_RTL_FILEGETSTRIOB			"fb_FileGetStrIOB"
#define FB_RTL_FILEGETSTRLARGEIOB		"fb_FileGetStrLargeIOB"
#define FB_RTL_FILEGETWSTRIOB			"fb_FileGetWstrIOB"
#define FB_RTL_FILEGETWSTRLARGEIOB		"fb_FileGetWstrLargeIOB"
#define FB_RTL_FILEGETARRAYIOB 			"fb_FileGetArrayIOB"
#define FB_RTL_FILEGETARRAYLARGEIOB		"fb_FileGetArrayLargeIOB"

#define FB_RTL_FILETELL 				"fb_FileTell"
#define FB_RTL_FILESEEK 				"fb_FileSeek"
#define FB_RTL_FILESEEKLARGE 			"fb_FileSeekLarge"

#define FB_RTL_FILESTRINPUT 			"fb_FileStrInput"
#define FB_RTL_FILELINEINPUT 			"fb_FileLineInput"
#define FB_RTL_FILELINEINPUTWSTR		"fb_FileLineInputWstr"
#define FB_RTL_CONSOLELINEINPUT 		"fb_LineInput"
#define FB_RTL_CONSOLELINEINPUTWSTR		"fb_LineInputWstr"

#define FB_RTL_FILEINPUT 				"fb_FileInput"
#define FB_RTL_CONSOLEINPUT 			"fb_ConsoleInput"
#define FB_RTL_INPUTBYTE 				"fb_InputByte"
#define FB_RTL_INPUTUBYTE 				"fb_InputUbyte"
#define FB_RTL_INPUTSHORT 				"fb_InputShort"
#define FB_RTL_INPUTUSHORT 				"fb_InputUshort"
#define FB_RTL_INPUTINT 				"fb_InputInt"
#define FB_RTL_INPUTUINT 				"fb_InputUint"
#define FB_RTL_INPUTLONGINT 			"fb_InputLongint"
#define FB_RTL_INPUTULONGINT 			"fb_InputUlongint"
#define FB_RTL_INPUTSINGLE 				"fb_InputSingle"
#define FB_RTL_INPUTDOUBLE 				"fb_InputDouble"
#define FB_RTL_INPUTSTR 				"fb_InputString"
#define FB_RTL_INPUTWSTR 				"fb_InputWstr"

#define FB_RTL_FILELOCK 				"fb_FileLock"
#define FB_RTL_FILELOCKLARGE 			"fb_FileLockLarge"
#define FB_RTL_FILEUNLOCK 				"fb_FileUnlock"
#define FB_RTL_FILEUNLOCKLARGE 			"fb_FileUnlockLarge"
#define FB_RTL_FILERENAME 				"rename"

#define FB_RTL_WIDTH 					"fb_Width"
#define FB_RTL_WIDTHDEV 				"fb_WidthDev"
#define FB_RTL_WIDTHFILE 				"fb_WidthFile"

#define FB_RTL_ERRORTHROW 				"fb_ErrorThrowAt"
#define FB_RTL_ERRORTHROWEX 			"fb_ErrorThrowEx"
#define FB_RTL_ERRORSETHANDLER 			"fb_ErrorSetHandler"
#define FB_RTL_ERRORGETNUM 				"fb_ErrorGetNum"
#define FB_RTL_ERRORSETNUM 				"fb_ErrorSetNum"
#define FB_RTL_ERRORRESUME 				"fb_ErrorResume"
#define FB_RTL_ERRORRESUMENEXT 			"fb_ErrorResumeNext"
#define FB_RTL_ERRORSETMODNAME			"fb_ErrorSetModName"
#define FB_RTL_ERRORSETFUNCNAME			"fb_ErrorSetFuncName"

#define FB_RTL_GFXPSET 					"fb_GfxPset"
#define FB_RTL_GFXPOINT 				"fb_GfxPoint"
#define FB_RTL_GFXLINE 					"fb_GfxLine"
#define FB_RTL_GFXCIRCLE 				"fb_GfxEllipse"
#define FB_RTL_GFXPAINT 				"fb_GfxPaint"
#define FB_RTL_GFXDRAW 					"fb_GfxDraw"
#define FB_RTL_GFXDRAWSTRING			"fb_GfxDrawString"
#define FB_RTL_GFXVIEW 					"fb_GfxView"
#define FB_RTL_GFXWINDOW 				"fb_GfxWindow"
#define FB_RTL_GFXPALETTE 				"fb_GfxPalette"
#define FB_RTL_GFXPALETTEUSING 			"fb_GfxPaletteUsing"
#define FB_RTL_GFXPALETTEGET 			"fb_GfxPaletteGet"
#define FB_RTL_GFXPALETTEGETUSING 		"fb_GfxPaletteGetUsing"
#define FB_RTL_GFXPUT 					"fb_GfxPut"
#define FB_RTL_GFXGET 					"fb_GfxGet"
#define FB_RTL_GFXGETQB					"fb_GfxGetQB"
#define FB_RTL_GFXSCREENSET 			"fb_GfxScreen"
#define FB_RTL_GFXSCREENSETQB 			"fb_GfxScreenQB"
#define FB_RTL_GFXSCREENRES 			"fb_GfxScreenRes"
#define FB_RTL_GFXEVENT					"fb_GfxEvent"
#define FB_RTL_GFXIMAGECREATE			"fb_GfxImageCreate"
#define FB_RTL_GFXIMAGECREATEQB			"fb_GfxImageCreateQB"
#define FB_RTL_GFXGETMOUSE  			"fb_GfxGetMouse"
#define FB_RTL_GFXGETJOYSTICK  			"fb_GfxGetJoystick"

#define FB_RTL_GFXPUTTRANS				"fb_hPutTrans"
#define FB_RTL_GFXPUTPSET				"fb_hPutPSet"
#define FB_RTL_GFXPUTPRESET				"fb_hPutPReset"
#define FB_RTL_GFXPUTAND				"fb_hPutAnd"
#define FB_RTL_GFXPUTOR					"fb_hPutOr"
#define FB_RTL_GFXPUTXOR				"fb_hPutXor"
#define FB_RTL_GFXPUTALPHA				"fb_hPutAlpha"
#define FB_RTL_GFXPUTBLEND				"fb_hPutBlend"
#define FB_RTL_GFXPUTADD				"fb_hPutAdd"
#define FB_RTL_GFXPUTCUSTOM				"fb_hPutCustom"

#define FB_RTL_PROFILEMCOUNT            "mcount"
#define FB_RTL_PROFILEMONSTARTUP        "_monstartup"

#define FB_RTL_GOSUBPUSH                "fb_GosubPush"
#define FB_RTL_GOSUBPOP                 "fb_GosubPop"
#define FB_RTL_GOSUBRETURN              "fb_GosubReturn"
#define FB_RTL_GOSUBEXIT                "fb_GosubExit"
#define FB_RTL_SETJMP                   "fb_SetJmp"

#define FB_RTL_SGN						"{sgn}"
#define FB_RTL_SIN                      "{sin}"
#define FB_RTL_ASIN                     "{asin}"
#define FB_RTL_COS                      "{cos}"
#define FB_RTL_ACOS                     "{acos}"
#define FB_RTL_TAN                      "{tan}"
#define FB_RTL_ATAN                     "{atan}"
#define FB_RTL_SQRT                     "{sqrt}"
#define FB_RTL_LOG                      "{log}"
#define FB_RTL_EXP                      "{exp}"
#define FB_RTL_FLOOR                    "{floor}"
#define FB_RTL_ABS                    	"{abs}"
#define FB_RTL_FIX                    	"{fix}"
#define FB_RTL_FRAC                    	"{frac}"
#define FB_RTL_ATAN2                    "{atan2}"

#define FB_RTL_FTOSB                    "fb_ftosb"
#define FB_RTL_DTOSB                    "fb_dtosb"
#define FB_RTL_FTOSS                    "fb_ftoss"
#define FB_RTL_DTOSS                    "fb_dtoss"
#define FB_RTL_FTOSI                    "fb_ftosi"
#define FB_RTL_DTOSI                    "fb_dtosi"
#define FB_RTL_FTOSL                    "fb_ftosl"
#define FB_RTL_DTOSL                    "fb_dtosl"
#define FB_RTL_FTOUB                    "fb_ftoub"
#define FB_RTL_DTOUB                    "fb_dtoub"
#define FB_RTL_FTOUS                    "fb_ftous"
#define FB_RTL_DTOUS                    "fb_dtous"
#define FB_RTL_FTOUI                    "fb_ftoui"
#define FB_RTL_DTOUI                    "fb_dtoui"
#define FB_RTL_FTOUL                    "fb_ftoul"
#define FB_RTL_DTOUL                    "fb_dtoul"

'' the order doesn't matter but it makes more sense to follow the same
'' order as the FB_RTL_* defines above
enum FB_RTL_IDX
	FB_RTL_IDX_STRINIT
	FB_RTL_IDX_STRDELETE
	FB_RTL_IDX_STRASSIGN
	FB_RTL_IDX_STRCONCAT
	FB_RTL_IDX_STRCOMPARE
	FB_RTL_IDX_STRCONCATASSIGN
	FB_RTL_IDX_STRALLOCTMPRES
	FB_RTL_IDX_STRALLOCTMPDESCV
	FB_RTL_IDX_STRALLOCTMPDESCF
	FB_RTL_IDX_STRALLOCTMPDESCZ
	FB_RTL_IDX_STRALLOCTMPDESCZEX

	FB_RTL_IDX_INT2STR
	FB_RTL_IDX_UINT2STR
	FB_RTL_IDX_LONGINT2STR
	FB_RTL_IDX_ULONGINT2STR
	FB_RTL_IDX_FLT2STR
	FB_RTL_IDX_DBL2STR
	FB_RTL_IDX_WSTR2STR

	FB_RTL_IDX_INT2STR_QB
	FB_RTL_IDX_UINT2STR_QB
	FB_RTL_IDX_LONGINT2STR_QB
	FB_RTL_IDX_ULONGINT2STR_QB
	FB_RTL_IDX_FLT2STR_QB
	FB_RTL_IDX_DBL2STR_QB

	FB_RTL_IDX_STR2INT
	FB_RTL_IDX_STR2UINT
	FB_RTL_IDX_STR2LNG
	FB_RTL_IDX_STR2ULNG
	FB_RTL_IDX_STR2DBL

	FB_RTL_IDX_MKD
	FB_RTL_IDX_MKS
	FB_RTL_IDX_MKI
	FB_RTL_IDX_MKL
	FB_RTL_IDX_MKSHORT
	FB_RTL_IDX_MKLONGINT

	FB_RTL_IDX_CVD
	FB_RTL_IDX_CVS
	FB_RTL_IDX_CVI
	FB_RTL_IDX_CVL
	FB_RTL_IDX_CVSHORT
	FB_RTL_IDX_CVLONGINT

	FB_RTL_IDX_STRMID
	FB_RTL_IDX_STRASSIGNMID
	FB_RTL_IDX_STRFILL1
	FB_RTL_IDX_STRFILL2
	FB_RTL_IDX_STRLEN
	FB_RTL_IDX_STRLSET
	FB_RTL_IDX_STRRSET
	FB_RTL_IDX_STRASC
	FB_RTL_IDX_STRCHR
	FB_RTL_IDX_STRINSTR
	FB_RTL_IDX_STRINSTRANY
	FB_RTL_IDX_STRINSTRREV
	FB_RTL_IDX_STRINSTRREVANY
	FB_RTL_IDX_STRTRIM
	FB_RTL_IDX_STRTRIMANY
	FB_RTL_IDX_STRTRIMEX
	FB_RTL_IDX_STRRTRIM
	FB_RTL_IDX_STRRTRIMANY
	FB_RTL_IDX_STRRTRIMEX
	FB_RTL_IDX_STRLTRIM
	FB_RTL_IDX_STRLTRIMANY
	FB_RTL_IDX_STRLTRIMEX
	FB_RTL_IDX_STRSWAP

	FB_RTL_IDX_WSTRALLOC
	FB_RTL_IDX_WSTRDELETE
	FB_RTL_IDX_WSTRASSIGN
	FB_RTL_IDX_WSTRASSIGNWA
	FB_RTL_IDX_WSTRASSIGNAW
	FB_RTL_IDX_WSTRASSIGNAW_INIT
	FB_RTL_IDX_WSTRCONCAT
	FB_RTL_IDX_WSTRCONCATWA
	FB_RTL_IDX_WSTRCONCATAW
	FB_RTL_IDX_WSTRCOMPARE
	FB_RTL_IDX_WSTRCONCATASSIGN

	FB_RTL_IDX_INT2WSTR
	FB_RTL_IDX_UINT2WSTR
	FB_RTL_IDX_LONGINT2WSTR
	FB_RTL_IDX_ULONGINT2WSTR
	FB_RTL_IDX_FLT2WSTR
	FB_RTL_IDX_DBL2WSTR
	FB_RTL_IDX_STR2WSTR

	FB_RTL_IDX_WSTRMID
	FB_RTL_IDX_WSTRASSIGNMID
	FB_RTL_IDX_WSTRFILL1
	FB_RTL_IDX_WSTRFILL2
	FB_RTL_IDX_WSTRLEN
	FB_RTL_IDX_WSTRLSET
	FB_RTL_IDX_WSTRRSET
	FB_RTL_IDX_WSTRASC
	FB_RTL_IDX_WSTRCHR
	FB_RTL_IDX_WSTRINSTR
	FB_RTL_IDX_WSTRINSTRANY
	FB_RTL_IDX_WSTRINSTRREV
	FB_RTL_IDX_WSTRINSTRREVANY
	FB_RTL_IDX_WSTRTRIM
	FB_RTL_IDX_WSTRTRIMANY
	FB_RTL_IDX_WSTRTRIMEX
	FB_RTL_IDX_WSTRRTRIM
	FB_RTL_IDX_WSTRRTRIMANY
	FB_RTL_IDX_WSTRRTRIMEX
	FB_RTL_IDX_WSTRLTRIM
	FB_RTL_IDX_WSTRLTRIMANY
	FB_RTL_IDX_WSTRLTRIMEX
	FB_RTL_IDX_WSTRSWAP

	FB_RTL_IDX_LONGINTDIV
	FB_RTL_IDX_ULONGINTDIV
	FB_RTL_IDX_LONGINTMOD
	FB_RTL_IDX_ULONGINTMOD
	FB_RTL_IDX_DBL2ULONGINT

	FB_RTL_IDX_ARRAYREDIM
	FB_RTL_IDX_ARRAYREDIM_OBJ
	FB_RTL_IDX_ARRAYREDIMPRESV
	FB_RTL_IDX_ARRAYREDIMPRESV_OBJ
	FB_RTL_IDX_ARRAYERASE
	FB_RTL_IDX_ARRAYERASE_OBJ
	FB_RTL_IDX_ARRAYCLEAR
	FB_RTL_IDX_ARRAYCLEAR_OBJ
	FB_RTL_IDX_ARRAYLBOUND
	FB_RTL_IDX_ARRAYUBOUND
	FB_RTL_IDX_ARRAYSTRERASE
	FB_RTL_IDX_ARRAYSNGBOUNDCHK
	FB_RTL_IDX_ARRAYBOUNDCHK

	FB_RTL_IDX_NULLPTRCHK

	FB_RTL_IDX_CPUDETECT
	FB_RTL_IDX_INIT
	FB_RTL_IDX_INITSIGNALS
	FB_RTL_IDX_INITCRTCTOR
	FB_RTL_IDX_END
	FB_RTL_IDX_ATEXIT

	FB_RTL_IDX_DATARESTORE
	FB_RTL_IDX_DATAREADSTR
	FB_RTL_IDX_DATAREADWSTR
	FB_RTL_IDX_DATAREADBYTE
	FB_RTL_IDX_DATAREADSHORT
	FB_RTL_IDX_DATAREADINT
	FB_RTL_IDX_DATAREADLONGINT
	FB_RTL_IDX_DATAREADUBYTE
	FB_RTL_IDX_DATAREADUSHORT
	FB_RTL_IDX_DATAREADUINT
	FB_RTL_IDX_DATAREADPTR
	FB_RTL_IDX_DATAREADULONGINT
	FB_RTL_IDX_DATAREADSINGLE
	FB_RTL_IDX_DATAREADDOUBLE

	FB_RTL_IDX_POW

	FB_RTL_IDX_PRINTVOID
	FB_RTL_IDX_PRINTBYTE
	FB_RTL_IDX_PRINTUBYTE
	FB_RTL_IDX_PRINTSHORT
	FB_RTL_IDX_PRINTUSHORT
	FB_RTL_IDX_PRINTINT
	FB_RTL_IDX_PRINTUINT
	FB_RTL_IDX_PRINTLONGINT
	FB_RTL_IDX_PRINTULONGINT
	FB_RTL_IDX_PRINTSINGLE
	FB_RTL_IDX_PRINTDOUBLE
	FB_RTL_IDX_PRINTSTR
	FB_RTL_IDX_PRINTWSTR

	FB_RTL_IDX_LPRINTVOID
	FB_RTL_IDX_LPRINTBYTE
	FB_RTL_IDX_LPRINTUBYTE
	FB_RTL_IDX_LPRINTSHORT
	FB_RTL_IDX_LPRINTUSHORT
	FB_RTL_IDX_LPRINTINT
	FB_RTL_IDX_LPRINTUINT
	FB_RTL_IDX_LPRINTLONGINT
	FB_RTL_IDX_LPRINTULONGINT
	FB_RTL_IDX_LPRINTSINGLE
	FB_RTL_IDX_LPRINTDOUBLE
	FB_RTL_IDX_LPRINTSTR
	FB_RTL_IDX_LPRINTWSTR

	FB_RTL_IDX_PRINTSPC
	FB_RTL_IDX_PRINTTAB

	FB_RTL_IDX_WRITEVOID
	FB_RTL_IDX_WRITEBYTE
	FB_RTL_IDX_WRITEUBYTE
	FB_RTL_IDX_WRITESHORT
	FB_RTL_IDX_WRITEUSHORT
	FB_RTL_IDX_WRITEINT
	FB_RTL_IDX_WRITEUINT
	FB_RTL_IDX_WRITELONGINT
	FB_RTL_IDX_WRITEULONGINT
	FB_RTL_IDX_WRITESINGLE
	FB_RTL_IDX_WRITEDOUBLE
	FB_RTL_IDX_WRITESTR
	FB_RTL_IDX_WRITEWSTR

	FB_RTL_IDX_PRINTUSGINIT
	FB_RTL_IDX_PRINTUSGSTR
	FB_RTL_IDX_PRINTUSGWSTR
	FB_RTL_IDX_PRINTUSG_SNG
	FB_RTL_IDX_PRINTUSG_DBL
	FB_RTL_IDX_PRINTUSG_LL
	FB_RTL_IDX_PRINTUSG_ULL
	FB_RTL_IDX_PRINTUSGEND
	FB_RTL_IDX_LPRINTUSGINIT

	FB_RTL_IDX_CONSOLEVIEW
	FB_RTL_IDX_CONSOLEREADXY
	FB_RTL_IDX_COLOR
	FB_RTL_IDX_PAGESET

	FB_RTL_IDX_MEMCOPY
	FB_RTL_IDX_MEMSWAP
	FB_RTL_IDX_MEMCOPYCLEAR

	FB_RTL_IDX_FILEOPEN
	FB_RTL_IDX_FILEOPEN_ENCOD
	FB_RTL_IDX_FILEOPEN_SHORT
	FB_RTL_IDX_FILEOPEN_CONS
	FB_RTL_IDX_FILEOPEN_ERR
	FB_RTL_IDX_FILEOPEN_PIPE
	FB_RTL_IDX_FILEOPEN_SCRN
	FB_RTL_IDX_FILEOPEN_LPT
	FB_RTL_IDX_FILEOPEN_COM
	FB_RTL_IDX_FILEOPEN_QB
	FB_RTL_IDX_FILECLOSE

	FB_RTL_IDX_FILEPUT
	FB_RTL_IDX_FILEPUTLARGE
	FB_RTL_IDX_FILEPUTSTR
	FB_RTL_IDX_FILEPUTSTRLARGE
	FB_RTL_IDX_FILEPUTWSTR
	FB_RTL_IDX_FILEPUTWSTRLARGE
	FB_RTL_IDX_FILEPUTARRAY
	FB_RTL_IDX_FILEPUTARRAYLARGE

	FB_RTL_IDX_FILEGET
	FB_RTL_IDX_FILEGETLARGE
	FB_RTL_IDX_FILEGETSTR
	FB_RTL_IDX_FILEGETSTRLARGE
	FB_RTL_IDX_FILEGETWSTR
	FB_RTL_IDX_FILEGETWSTRLARGE
	FB_RTL_IDX_FILEGETARRAY
	FB_RTL_IDX_FILEGETARRAYLARGE

	FB_RTL_IDX_FILEGETIOB
	FB_RTL_IDX_FILEGETLARGEIOB
	FB_RTL_IDX_FILEGETSTRIOB
	FB_RTL_IDX_FILEGETSTRLARGEIOB
	FB_RTL_IDX_FILEGETWSTRIOB
	FB_RTL_IDX_FILEGETWSTRLARGEIOB
	FB_RTL_IDX_FILEGETARRAYIOB
	FB_RTL_IDX_FILEGETARRAYLARGEIOB

	FB_RTL_IDX_FILETELL
	FB_RTL_IDX_FILESEEK
	FB_RTL_IDX_FILESEEKLARGE

	FB_RTL_IDX_FILESTRINPUT
	FB_RTL_IDX_FILELINEINPUT
	FB_RTL_IDX_FILELINEINPUTWSTR
	FB_RTL_IDX_CONSOLELINEINPUT
	FB_RTL_IDX_CONSOLELINEINPUTWSTR

	FB_RTL_IDX_FILEINPUT
	FB_RTL_IDX_CONSOLEINPUT
	FB_RTL_IDX_INPUTBYTE
	FB_RTL_IDX_INPUTUBYTE
	FB_RTL_IDX_INPUTSHORT
	FB_RTL_IDX_INPUTUSHORT
	FB_RTL_IDX_INPUTINT
	FB_RTL_IDX_INPUTUINT
	FB_RTL_IDX_INPUTLONGINT
	FB_RTL_IDX_INPUTULONGINT
	FB_RTL_IDX_INPUTSINGLE
	FB_RTL_IDX_INPUTDOUBLE
	FB_RTL_IDX_INPUTSTR
	FB_RTL_IDX_INPUTWSTR

	FB_RTL_IDX_FILELOCK
	FB_RTL_IDX_FILELOCKLARGE
	FB_RTL_IDX_FILEUNLOCK
	FB_RTL_IDX_FILEUNLOCKLARGE
	FB_RTL_IDX_FILERENAME

	FB_RTL_IDX_WIDTH
	FB_RTL_IDX_WIDTHDEV
	FB_RTL_IDX_WIDTHFILE

	FB_RTL_IDX_ERRORTHROW
	FB_RTL_IDX_ERRORTHROWEX
	FB_RTL_IDX_ERRORSETHANDLER
	FB_RTL_IDX_ERRORGETNUM
	FB_RTL_IDX_ERRORSETNUM
	FB_RTL_IDX_ERRORRESUME
	FB_RTL_IDX_ERRORRESUMENEXT
	FB_RTL_IDX_ERRORSETMODNAME
	FB_RTL_IDX_ERRORSETFUNCNAME

	FB_RTL_IDX_GFXPSET
	FB_RTL_IDX_GFXPOINT
	FB_RTL_IDX_GFXLINE
	FB_RTL_IDX_GFXCIRCLE
	FB_RTL_IDX_GFXPAINT
	FB_RTL_IDX_GFXDRAW
	FB_RTL_IDX_GFXDRAWSTRING
	FB_RTL_IDX_GFXVIEW
	FB_RTL_IDX_GFXWINDOW
	FB_RTL_IDX_GFXPALETTE
	FB_RTL_IDX_GFXPALETTEUSING
	FB_RTL_IDX_GFXPALETTEGET
	FB_RTL_IDX_GFXPALETTEGETUSING
	FB_RTL_IDX_GFXPUT
	FB_RTL_IDX_GFXPUTTRANS
	FB_RTL_IDX_GFXPUTPSET
	FB_RTL_IDX_GFXPUTPRESET
	FB_RTL_IDX_GFXPUTAND
	FB_RTL_IDX_GFXPUTOR
	FB_RTL_IDX_GFXPUTXOR
	FB_RTL_IDX_GFXPUTALPHA
	FB_RTL_IDX_GFXPUTBLEND
	FB_RTL_IDX_GFXPUTADD
	FB_RTL_IDX_GFXPUTCUSTOM
	FB_RTL_IDX_GFXGET
	FB_RTL_IDX_GFXGETQB
	FB_RTL_IDX_GFXSCREENSET
	FB_RTL_IDX_GFXSCREENSETQB
	FB_RTL_IDX_GFXSCREENRES
	FB_RTL_IDX_GFXIMAGECREATE
	FB_RTL_IDX_GFXIMAGECREATEQB

	FB_RTL_IDX_PROFILEMCOUNT
	FB_RTL_IDX_PROFILEMONSTARTUP

	FB_RTL_IDX_GOSUBPUSH
	FB_RTL_IDX_GOSUBPOP
	FB_RTL_IDX_GOSUBRETURN
	FB_RTL_IDX_GOSUBEXIT
	FB_RTL_IDX_SETJMP

	FB_RTL_IDX_SGN
	FB_RTL_IDX_SIN
	FB_RTL_IDX_ASIN
	FB_RTL_IDX_COS
	FB_RTL_IDX_ACOS
	FB_RTL_IDX_TAN
	FB_RTL_IDX_ATAN
	FB_RTL_IDX_SQRT
	FB_RTL_IDX_LOG
	FB_RTL_IDX_EXP
	FB_RTL_IDX_FLOOR
	FB_RTL_IDX_ABS
	FB_RTL_IDX_FIX
	FB_RTL_IDX_FRAC
	FB_RTL_IDX_ATAN2

	FB_RTL_IDX_FTOSB
	FB_RTL_IDX_DTOSB
	FB_RTL_IDX_FTOSS
	FB_RTL_IDX_DTOSS
	FB_RTL_IDX_FTOSI
	FB_RTL_IDX_DTOSI
	FB_RTL_IDX_FTOSL
	FB_RTL_IDX_DTOSL
	FB_RTL_IDX_FTOUB
	FB_RTL_IDX_DTOUB
	FB_RTL_IDX_FTOUS
	FB_RTL_IDX_DTOUS
	FB_RTL_IDX_FTOUI
	FB_RTL_IDX_DTOUI
	FB_RTL_IDX_FTOUL
	FB_RTL_IDX_DTOUL

	FB_RTL_INDEXES
end enum

enum FB_RTL_OPT
	FB_RTL_OPT_NONE		  = &h00000000
	FB_RTL_OPT_OVER		  = &h00000001					'' overloaded
	FB_RTL_OPT_ERROR	  = &h00000002					'' returns an error
	FB_RTL_OPT_MT		  = &h00000004					'' needs the multithreaded rtlib
	FB_RTL_OPT_VBSYMB	  = &h00000008                  '' vb-only
	FB_RTL_OPT_DBGONLY	  = &h00000010                  '' -g only
	FB_RTL_OPT_OPERATOR	  = &h00000020
	FB_RTL_OPT_STRSUFFIX  = &h00000040                  '' has a $ suffix (-lang qb only)
	FB_RTL_OPT_NOQB		  = &h00000080                  '' anything but -lang qb
	FB_RTL_OPT_QBONLY	  = &h00000100                  '' -lang qb only
	FB_RTL_OPT_NOFB		  = &h00000200                  '' anything but -lang fb
	FB_RTL_OPT_FBONLY	  = &h00000400                  ''
 	FB_RTL_OPT_DUPDECL	  = &h00000800 					'' overloaded procs pointing to the same symbol
 	FB_RTL_OPT_GCCBUILTIN = &h00001000					'' GCC builtin, don't redeclare, create a wrapper
	FB_RTL_OPT_NOGCC	  = &h00002000                  '' anything but -gen gcc
end enum

type FB_RTL_PARAMDEF
	dtype		as FB_DATATYPE
	mode		as FB_PARAMMODE
	isopt		as integer
	optval		as integer
	check_const as integer
end type

type FB_RTL_PROCDEF
	name		as zstring ptr
	alias		as zstring ptr
	dtype		as FB_DATATYPE
	callconv	as FB_FUNCMODE
	callback	as FBRTLCALLBACK
	options		as FB_RTL_OPT
	params		as integer
	paramTb(0 to 15) as FB_RTL_PARAMDEF
end type

type FB_RTL_MACROTOKEN
	type		as FB_DEFTOK_TYPE
	data		as any ptr
end type

type FB_RTL_MACRODEF
	name		as zstring ptr
	options		as FB_RTL_OPT
	params		as integer
	paramTb(0 to 3) as zstring ptr
	tokenTb(0 to 11) as FB_RTL_MACROTOKEN
end type

declare sub rtlInit _
	( _
	)

declare sub rtlEnd _
	( _
	)

declare sub rtlAddIntrinsicProcs _
	( _
		byval procdef as FB_RTL_PROCDEF ptr _
	)

declare sub rtlAddIntrinsicMacros _
	( _
		byval macdef as FB_RTL_MACRODEF ptr _
	)

declare function rtlProcLookup _
	( _
		byval pname as zstring ptr, _
		byval pidx as integer _
	) as FBSYMBOL ptr

declare function rtlOvlProcCall _
	( _
		byval sym as FBSYMBOL ptr, _
		byval param1 as ASTNODE ptr, _
		byval param2 as ASTNODE ptr = NULL _
	) as ASTNODE ptr

declare function rtlCalcExprLen _
	( _
		byval expr as ASTNODE ptr, _
		byval realsize as integer = TRUE _
	) as integer

declare function rtlCalcStrLen _
	( _
		byval expr as ASTNODE ptr, _
		byval dtype as integer _
	) as integer

declare function rtlStrCompare _
	( _
		byval str1 as ASTNODE ptr, _
		byval sdtype1 as integer, _
		byval str2 as ASTNODE ptr, _
		byval sdtype2 as integer _
	) as ASTNODE ptr

declare function rtlWstrCompare _
	( _
		byval str1 as ASTNODE ptr, _
		byval str2 as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlStrConcat _
	( _
		byval str1 as ASTNODE ptr, _
		byval sdtype1 as integer, _
		byval str2 as ASTNODE ptr, _
		byval sdtype2 as integer _
	) as ASTNODE ptr

declare function rtlWstrConcat _
	( _
		byval str1 as ASTNODE ptr, _
		byval sdtype1 as integer, _
		byval str2 as ASTNODE ptr, _
		byval sdtype2 as integer _
	) as ASTNODE ptr

declare function rtlStrAssign _
	( _
		byval dst as ASTNODE ptr, _
		byval src as ASTNODE ptr, _
		byval is_ini as integer = FALSE, _
		byval kill_return as integer = TRUE _
	) as ASTNODE ptr

declare function rtlWstrAssign _
	( _
		byval dst as ASTNODE ptr, _
		byval src as ASTNODE ptr, _
		byval is_ini as integer = FALSE, _
		byval kill_return as integer = TRUE _
	) as ASTNODE ptr

declare function rtlStrConcatAssign _
	( _
		byval dst as ASTNODE ptr, _
		byval src as ASTNODE ptr, _
		byval kill_return as integer = TRUE _
	) as ASTNODE ptr

declare function rtlWstrConcatAssign _
	( _
		byval dst as ASTNODE ptr, _
		byval src as ASTNODE ptr, _
		byval kill_return as integer = TRUE _
	) as ASTNODE ptr

declare function rtlStrDelete _
	( _
		byval strg as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlStrAllocTmpResult _
	( _
		byval strg as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlStrAllocTmpDesc _
	( _
		byval strg as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlWstrAlloc _
	( _
		byval lenexpr as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlToStr _
	( _
		byval expr as ASTNODE ptr, _
		byval pad as integer _
	) as ASTNODE ptr

declare function rtlToWstr _
	( _
		byval expr as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlStrToVal _
	( _
		byval expr as ASTNODE ptr, _
		byval to_dtype as integer _
	) as ASTNODE ptr

declare function rtlStrMid _
	( _
		byval expr1 as ASTNODE ptr, _
		byval expr2 as ASTNODE ptr, _
		byval expr3 as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlStrAssignMid _
	( _
		byval expr1 as ASTNODE ptr, _
		byval expr2 as ASTNODE ptr, _
		byval expr3 as ASTNODE ptr, _
		byval expr4 as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlStrLRSet _
	( _
		byval dstexpr as ASTNODE ptr, _
		byval srcexpr as ASTNODE ptr, _
		byval is_rset as integer _
	) as integer

declare function rtlStrFill _
	( _
		byval expr1 as ASTNODE ptr, _
		byval expr2 as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlWstrFill _
	( _
		byval expr1 as ASTNODE ptr, _
		byval expr2 as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlStrAsc _
	( _
		byval expr as ASTNODE ptr, _
		byval posexpr as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlStrChr _
	( _
		byval args as integer, _
		exprtb() as ASTNODE ptr, _
		byval is_wstr as integer _
	) as ASTNODE ptr

declare function rtlStrInstr _
	( _
		byval nd_start as ASTNODE ptr, _
		byval nd_text as ASTNODE ptr, _
		byval nd_pattern as ASTNODE ptr, _
		byval search_any as integer _
	) as ASTNODE ptr

declare function rtlStrInstrRev _
	( _
		byval nd_start as ASTNODE ptr, _
		byval nd_text as ASTNODE ptr, _
		byval nd_pattern as ASTNODE ptr, _
		byval search_any as integer _
	) as ASTNODE ptr

declare function rtlStrTrim _
	( _
		byval nd_text as ASTNODE ptr, _
		byval nd_pattern as ASTNODE ptr, _
		byval is_any as integer _
	) as ASTNODE ptr

declare function rtlStrRTrim _
	( _
		byval nd_text as ASTNODE ptr, _
		byval nd_pattern as ASTNODE ptr, _
		byval is_any as integer _
	) as ASTNODE ptr

declare function rtlStrLTrim _
	( _
		byval nd_text as ASTNODE ptr, _
		byval nd_pattern as ASTNODE ptr, _
		byval is_any as integer _
	) as ASTNODE ptr

declare function rtlArrayRedim _
	( _
		byval s as FBSYMBOL ptr, _
		byval elementlen as integer, _
		byval dimensions as integer, _
		exprTB() as ASTNODE ptr, _
		byval dopreserve as integer, _
		byval doclear as integer _
	) as integer

declare function rtlArrayErase _
	( _
		byval arrayexpr as ASTNODE ptr, _
		byval check_access as integer = FALSE _
	) as ASTNODE ptr

declare function rtlArrayClear _
	( _
		byval arrayexpr as ASTNODE ptr, _
		byval dofill as integer, _
		byval check_access as integer = FALSE _
	) as ASTNODE ptr

declare function rtlArrayBound _
	( _
		byval sexpr as ASTNODE ptr, _
		byval dimexpr as ASTNODE ptr, _
		byval islbound as integer _
	) as ASTNODE ptr

declare function rtlArrayStrErase _
	( _
		byval arrayexpr as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlArrayBoundsCheck _
	( _
		byval idx as ASTNODE ptr, _
		byval lb as ASTNODE ptr, _
		byval rb as ASTNODE ptr, _
		byval linenum as integer, _
		byval module as zstring ptr _
	) as ASTNODE ptr

declare function rtlNullPtrCheck _
	( _
		byval p as ASTNODE ptr, _
		byval linenum as integer, _
		byval module as zstring ptr _
	) as ASTNODE ptr

declare function rtlDataRestore _
	( _
		byval label as FBSYMBOL ptr, _
		byval afternode as ASTNODE ptr = NULL _
	) as integer

declare function rtlDataRead _
	( _
		byval varexpr as ASTNODE ptr _
	) as integer

declare function rtlMathPow _
	( _
		byval xexpr as ASTNODE ptr, _
		byval yexpr as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlMathLen _
	( _
		byval expr as ASTNODE ptr, _
		byval checkstrings as integer = TRUE _
	) as ASTNODE ptr

declare function rtlMathLongintCMP _
	( _
		byval op as integer, _
		byval dtype as integer, _
		byval lexpr as ASTNODE ptr, _
		byval ldtype as integer, _
		byval rexpr as ASTNODE ptr, _
		byval rdtype as integer _
	) as ASTNODE ptr

declare function rtlMathLongintDIV _
	( _
		byval dtype as integer, _
		byval lexpr as ASTNODE ptr, _
		byval ldtype as integer, _
		byval rexpr as ASTNODE ptr, _
		byval rdtype as integer _
	) as ASTNODE ptr

declare function rtlMathLongintMUL _
	( _
		byval dtype as integer, _
		byval lexpr as ASTNODE ptr, _
		byval ldtype as integer, _
		byval rexpr as ASTNODE ptr, _
		byval rdtype as integer _
	) as ASTNODE ptr

declare function rtlMathLongintMOD _
	( _
		byval dtype as integer, _
		byval lexpr as ASTNODE ptr, _
		byval ldtype as integer, _
		byval rexpr as ASTNODE ptr, _
		byval rdtype as integer _
	) as ASTNODE ptr

declare function rtlMathFp2ULongint _
	( _
		byval expr as ASTNODE ptr, _
		byval dtype as integer _
	) as ASTNODE ptr

declare function rtlMathUop _
	( _
		byval op as integer, _
		byval expr as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlMathBop _
	( _
		byval op as integer, _
		byval lexpr as ASTNODE ptr, _
		byval rexpr as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlMathFTOI _
	( _
		byval expr as ASTNODE ptr, _
		byval to_dtype as integer _
	) as ASTNODE ptr

declare function rtlInitMain _
	( _
		_
	) as integer

declare function rtlInitApp _
	( _
		byval argc as ASTNODE ptr, _
		byval argv as ASTNODE ptr, _
		byval isdllmain as integer _
	) as ASTNODE ptr

declare function rtlExitApp _
	( _
		byval errlevel as ASTNODE ptr _
	) as integer

declare function rtlMemCopy _
	( _
		byval dst as ASTNODE ptr, _
		byval src as ASTNODE ptr, _
		byval bytes as integer _
	) as ASTNODE ptr

declare function rtlMemSwap _
	( _
		byval dst as ASTNODE ptr, _
		byval src as ASTNODE ptr _
	) as integer

declare function rtlStrSwap _
	( _
		byval str1 as ASTNODE ptr, _
		byval str2 as ASTNODE ptr _
	) as integer

declare function rtlWstrSwap _
	( _
		byval str1 as ASTNODE ptr, _
		byval str2 as ASTNODE ptr _
	) as integer

declare function rtlMemCopyClear _
	( _
		byval dstexpr as ASTNODE ptr, _
		byval dstlen as integer, _
		byval srcexpr as ASTNODE ptr, _
		byval srclen as integer _
	) as integer

declare function rtlMemNewOp _
	( _
		byval is_vector as integer, _
		byval len_expr as ASTNODE ptr, _
		byval dtype as integer, _
		byval subtype as FBSYMBOL ptr _
	) as ASTNODE ptr

declare function rtlMemDeleteOp _
	( _
		byval is_vector as integer, _
		byval ptr_expr as ASTNODE ptr, _
		byval dtype as integer, _
		byval subtype as FBSYMBOL ptr _
	) as ASTNODE ptr

declare function rtlPrint _
	( _
		byval fileexpr as ASTNODE ptr, _
		byval iscomma as integer, _
		byval issemicolon as integer, _
		byval expr as ASTNODE ptr, _
		byval islprint as integer = FALSE _
	) as integer

declare function rtlPrintSPC _
	( _
		byval fileexpr as ASTNODE ptr, _
		byval expr as ASTNODE ptr, _
		byval islprint as integer = FALSE _
	) as integer

declare function rtlPrintTab _
	( _
		byval fileexpr as ASTNODE ptr, _
		byval expr as ASTNODE ptr, _
		byval islprint as integer = FALSE _
	) as integer

declare function rtlWrite _
	( _
		byval fileexpr as ASTNODE ptr, _
		byval iscomma as integer, _
		byval expr as ASTNODE ptr _
	) as integer

declare function rtlPrintUsingInit _
	( _
		byval usingexpr as ASTNODE ptr, _
		byval islprint as integer = FALSE _
	) as integer

declare function rtlPrintUsingEnd _
	( _
		byval fileexpr as ASTNODE ptr, _
		byval islprint as integer = FALSE _
	) as integer

declare function rtlPrintUsing _
	( _
		byval fileexpr as ASTNODE ptr, _
		byval expr as ASTNODE ptr, _
		byval iscomma as integer, _
		byval issemicolon as integer, _
		byval islprint as integer = FALSE _
	) as integer

declare function rtlFileOpen _
	( _
		byval filename as ASTNODE ptr, _
		byval fmode as ASTNODE ptr, _
		byval faccess as ASTNODE ptr, _
		byval flock as ASTNODE ptr, _
		byval filenum as ASTNODE ptr, _
		byval flen as ASTNODE ptr, _
		byval fencoding as ASTNODE ptr, _
		byval isfunc as integer, _
		byval openkind as FBOPENKIND _
	) as ASTNODE ptr

declare function rtlFileOpenShort _
	( _
		byval filename as ASTNODE ptr, _
		byval fmode as ASTNODE ptr, _
		byval faccess as ASTNODE ptr, _
		byval flock as ASTNODE ptr, _
		byval filenum as ASTNODE ptr, _
		byval flen as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlFileRename _
	( _
		byval filename_new as ASTNODE ptr, _
		byval filename_old as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlWidthScreen _
	( _
		byval width_arg as ASTNODE ptr, _
		byval height_arg as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlWidthDev _
	( _
		byval device as ASTNODE ptr, _
		byval width_arg as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlWidthFile _
	( _
		byval fnum as ASTNODE ptr, _
		byval width_arg as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlColor _
	( _
		byval fore_color as ASTNODE ptr, _
		byval back_color as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlPageSet _
	( _
		byval active as ASTNODE ptr, _
		byval visible as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlFileClose _
	( _
		byval filenum as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlFileSeek _
	( _
		byval filenum as ASTNODE ptr, _
		byval newpos as ASTNODE ptr _
	) as integer

declare function rtlFileTell _
	( _
		byval filenum as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlFilePut _
	( _
		byval filenum as ASTNODE ptr, _
		byval offset as ASTNODE ptr, _
		byval src as ASTNODE ptr, _
		byval elements as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlFilePutArray _
	( _
		byval filenum as ASTNODE ptr, _
		byval offset as ASTNODE ptr, _
		byval src as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlFileGet _
	( _
		byval filenum as ASTNODE ptr, _
		byval offset as ASTNODE ptr, _
		byval dst as ASTNODE ptr, _
		byval elements as ASTNODE ptr, _
		byval iobytes as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlFileGetArray _
	( _
		byval filenum as ASTNODE ptr, _
		byval offset as ASTNODE ptr, _
		byval dst as ASTNODE ptr, _
		byval iobytes as ASTNODE ptr, _
		byval isfunc as integer _
	) as ASTNODE ptr

declare function rtlFileStrInput _
	( _
		byval bytesexpr as ASTNODE ptr, _
		byval filenum as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlFileLineInput _
	( _
		byval isfile as integer, _
		byval expr as ASTNODE ptr, _
		byval dstexpr as ASTNODE ptr, _
		byval addquestion as integer, _
		byval addnewline as integer _
	) as integer

declare function rtlFileLineInputWstr _
	( _
		byval isfile as integer, _
		byval expr as ASTNODE ptr, _
		byval dstexpr as ASTNODE ptr, _
		byval addquestion as integer, _
		byval addnewline as integer _
	) as integer

declare function rtlFileInput _
	( _
		byval isfile as integer, _
		byval expr as ASTNODE ptr, _
		byval addquestion as integer, _
		byval addnewline as integer _
	) as integer

declare function rtlFileInputGet _
	( _
		byval dstexpr as ASTNODE ptr _
	) as integer

declare function rtlFileLock _
	( _
		byval islock as integer, _
		byval filenum as ASTNODE ptr, _
		byval iniexpr as ASTNODE ptr, _
		byval endexpr as ASTNODE ptr _
	) as integer

declare function rtlErrorCheck _
	( _
		byval resexpr as ASTNODE ptr, _
		byval reslabel as FBSYMBOL ptr, _
		byval linenum as integer _
	) as integer

declare sub rtlErrorThrow _
	( _
		byval errexpr as ASTNODE ptr, _
		byval linenum as integer, _
		byval module as zstring ptr _
	)

declare sub rtlErrorSetHandler _
	( _
		byval newhandler as ASTNODE ptr, _
		byval savecurrent as integer _
	)

declare function rtlErrorGetNum _
	( _
		_
	) as ASTNODE ptr

declare sub rtlErrorSetNum _
	( _
		byval errexpr as ASTNODE ptr _
	)

declare sub rtlErrorResume _
	( _
		byval isnext as integer _
	)

declare function rtlErrorSetModName _
	( _
		byval sym as FBSYMBOL ptr, _
		byval modname as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlErrorSetFuncName _
	( _
		byval sym as FBSYMBOL ptr, _
		byval funcname as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlConsoleView _
	( _
		byval topexpr as ASTNODE ptr, _
		byval botexpr as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlConsoleReadXY _
	( _
		byval rowexpr as ASTNODE ptr, _
		byval columnexpr as ASTNODE ptr, _
		byval colorflagexpr as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlAtExit _
	( _
		byval procexpr as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlGfxPset _
	( _
		byval target as ASTNODE ptr, _
		byval targetisptr as integer, _
		byval xexpr as ASTNODE ptr, _
		byval yexpr as ASTNODE ptr, _
		byval cexpr as ASTNODE ptr, _
		byval coordtype as integer, _
		byval ispreset as integer _
	) as integer

declare function rtlGfxPoint _
	( _
		byval target as ASTNODE ptr, _
		byval targetisptr as integer, _
		byval xexpr as ASTNODE ptr, _
		byval yexpr as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlGfxLine _
	( _
		byval target as ASTNODE ptr, _
		byval targetisptr as integer, _
		byval x1expr as ASTNODE ptr, _
		byval y1expr as ASTNODE ptr, _
		byval x2expr as ASTNODE ptr, _
		byval y2expr as ASTNODE ptr, _
		byval cexpr as ASTNODE ptr, _
		byval linetype as integer, _
		byval styleexpr as ASTNODE ptr, _
		byval coordtype as integer _
	) as integer

declare function rtlGfxCircle _
	( _
		byval target as ASTNODE ptr, _
		byval targetisptr as integer, _
		byval xexpr as ASTNODE ptr, _
		byval yexpr as ASTNODE ptr, _
		byval radexpr as ASTNODE ptr, _
		byval cexpr as ASTNODE ptr, _
		byval aspexpr as ASTNODE ptr, _
		byval iniexpr as ASTNODE ptr, _
		byval endexpr as ASTNODE ptr, _
		byval fillflag as integer, _
		byval coordtype as integer _
	) as integer

declare function rtlGfxPaint _
	( _
		byval target as ASTNODE ptr, _
		byval targetisptr as integer, _
		byval xexpr as ASTNODE ptr, _
		byval yexpr as ASTNODE ptr, _
		byval pexpr as ASTNODE ptr, _
		byval bexpr as ASTNODE ptr, _
		byval coord_type as integer _
	) as integer

declare function rtlGfxDraw _
	( _
		byval target as ASTNODE ptr, _
		byval targetisptr as integer, _
		byval cexpr as ASTNODE ptr _
	) as integer

declare function rtlGfxDrawString _
	( _
		byval target as ASTNODE ptr, _
		byval targetisptr as integer, _
		byval xexpr as ASTNODE ptr, _
		byval texpr as ASTNODE ptr, _
		byval sexpr as ASTNODE ptr, _
		byval cexpr as ASTNODE ptr, _
		byval fexpr as ASTNODE ptr, _
		byval fisptr as integer, _
		byval coord_type as integer, _
		byval mode as integer, _
		byval alphaexpr as ASTNODE ptr, _
		byval funcexpr as ASTNODE ptr, _
		byval paramexpr as ASTNODE ptr _
	) as integer

declare function rtlGfxView _
	( _
		byval x1expr as ASTNODE ptr, _
		byval y1expr as ASTNODE ptr, _
		byval x2expr as ASTNODE ptr, _
		byval y2expr as ASTNODE ptr, _
		byval fillexpr as ASTNODE ptr, _
		byval bordexpr as ASTNODE ptr, _
		byval screenflag as integer _
	) as integer

declare function rtlGfxWindow _
	( _
		byval x1expr as ASTNODE ptr, _
		byval y1expr as ASTNODE ptr, _
		byval x2expr as ASTNODE ptr, _
		byval y2expr as ASTNODE ptr, _
		byval screenflag as integer _
	) as integer

declare function rtlGfxPalette _
	( _
		byval attexpr as ASTNODE ptr, _
		byval rexpr as ASTNODE ptr, _
		byval gexpr as ASTNODE ptr, _
		byval bexpr as ASTNODE ptr, _
		byval isget as integer _
	) as integer

declare function rtlGfxPaletteUsing _
	( _
		byval arrayexpr as ASTNODE ptr, _
		byval isptr as integer, _
		byval isget as integer _
	) as integer

declare function rtlGfxPut _
	( _
		byval target as ASTNODE ptr, _
		byval targetisptr as integer, _
		byval xexpr as ASTNODE ptr, _
		byval yexpr as ASTNODE ptr, _
		byval arrayexpr as ASTNODE ptr, _
		byval isptr as integer, _
		byval x1expr as ASTNODE ptr, _
		byval x2expr as ASTNODE ptr, _
		byval y1expr as ASTNODE ptr, _
		byval y2expr as ASTNODE ptr, _
		byval mode as integer, _
		byval alphaexpr as ASTNODE ptr, _
		byval funcexpr as ASTNODE ptr, _
		byval paramexpr as ASTNODE ptr, _
		byval coordtype as integer _
	) as integer

declare function rtlGfxGet _
	( _
		byval target as ASTNODE ptr, _
		byval targetisptr as integer, _
		byval x1expr as ASTNODE ptr, _
		byval y1expr as ASTNODE ptr, _
		byval x2expr as ASTNODE ptr, _
		byval y2expr as ASTNODE ptr, _
		byval arrayexpr as ASTNODE ptr, _
		byval isptr as integer, _
		byval symbol as FBSYMBOL ptr, _
		byval coordtype as integer _
	) as integer

declare function rtlGfxScreenSet _
	( _
		byval wexpr as ASTNODE ptr, _
		byval hexpr as ASTNODE ptr, _
		byval dexpr as ASTNODE ptr, _
		byval pexpr as ASTNODE ptr, _
		byval fexpr as ASTNODE ptr, _
		byval rexpr as ASTNODE ptr _
	) as integer

declare function rtlGfxScreenSetQB _
	( _
		byval mode as ASTNODE ptr, _
		byval active as ASTNODE ptr, _
		byval visible as ASTNODE ptr _
	) as integer

declare function rtlGfxImageCreate _
	( _
		byval wexpr as ASTNODE ptr, _
		byval hexpr as ASTNODE ptr, _
		byval cexpr as ASTNODE ptr, _
		byval dexpr as ASTNODE ptr, _
		byval flags as integer _
	) as ASTNODE ptr

declare function rtlGfxGetMouse _
	( _
		byval x_expr as ASTNODE ptr, _
		byval y_expr as ASTNODE ptr, _
		byval w_expr as ASTNODE ptr, _
		byval b_expr as ASTNODE ptr, _
		byval c_expr as ASTNODE ptr, _
		byval is_func as integer _
	) as ASTNODE ptr

declare function rtlGfxGetJoystick _
	( _
		byval id_expr as ASTNODE ptr, _
		byval b_expr as ASTNODE ptr, _
		a_expr() as ASTNODE ptr, _
		byval is_func as integer _
	) as ASTNODE ptr

declare function rtlGfxEvent _
	( _
		byval e_expr as ASTNODE ptr, _
		byval is_func as integer _
	) as ASTNODE ptr

declare function rtlProfileCall_mcount _
	( _
		_
	) as ASTNODE ptr

declare function rtlProfileCall_monstartup _
	( _
		_
	) as ASTNODE ptr

declare function rtlGosubPush _
	( _
		byval ctx as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlGosubPop _
	( _
		byval ctx as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlGosubReturn _
	( _
		byval ctx as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlGosubExit _
	( _
		byval ctx as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlSetJmp _
	( _
		byval buf as ASTNODE ptr _
	) as ASTNODE ptr

declare function rtlMultinput_cb _
	( _
		byval sym as FBSYMBOL ptr _
	) as integer

declare function rtlPrinter_cb _
	( _
		byval sym as FBSYMBOL ptr _
	) as integer


const FBGFX_PUTMODE_TRANS  = 0
const FBGFX_PUTMODE_PSET   = 1
const FBGFX_PUTMODE_PRESET = 2
const FBGFX_PUTMODE_AND    = 3
const FBGFX_PUTMODE_OR     = 4
const FBGFX_PUTMODE_XOR    = 5
const FBGFX_PUTMODE_ALPHA  = 6
const FBGFX_PUTMODE_ADD    = 7
const FBGFX_PUTMODE_CUSTOM = 8
const FBGFX_PUTMODE_BLEND  = 9

''
'' macros
''

#define PROCLOOKUP(id) rtlProcLookup( strptr( FB_RTL_##id ), FB_RTL_IDX_##id )



