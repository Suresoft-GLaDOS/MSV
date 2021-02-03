#pragma once

#include once "CUnit.bi"
#include once "crt/errno.bi"

extern "C"

enum CU_ErrorCode
	CUE_SUCCESS = 0
	CUE_NOMEMORY = 1
	CUE_NOREGISTRY = 10
	CUE_REGISTRY_EXISTS = 11
	CUE_NOSUITE = 20
	CUE_NO_SUITENAME = 21
	CUE_SINIT_FAILED = 22
	CUE_SCLEAN_FAILED = 23
	CUE_DUP_SUITE = 24
	CUE_SUITE_INACTIVE = 25
	CUE_NOTEST = 30
	CUE_NO_TESTNAME = 31
	CUE_DUP_TEST = 32
	CUE_TEST_NOT_IN_SUITE = 33
	CUE_TEST_INACTIVE = 34
	CUE_FOPEN_FAILED = 40
	CUE_FCLOSE_FAILED = 41
	CUE_BAD_FILENAME = 42
	CUE_WRITE_ERROR = 43
end enum

enum CU_ErrorAction
	CUEA_IGNORE
	CUEA_FAIL
	CUEA_ABORT
end enum

declare function CU_get_error() as long
declare function CU_get_error_msg() as const zstring ptr
declare sub CU_set_error_action(byval action as long)
declare function CU_get_error_action() as long
declare sub CU_set_error(byval error as long)

end extern
