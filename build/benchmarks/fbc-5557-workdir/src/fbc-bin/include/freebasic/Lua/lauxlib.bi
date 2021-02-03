#pragma once
#include "lua.bi"

extern "C"

#define LUA_ERRFILE (LUA_ERRERR+1)

type luaL_Reg
	name as const zstring ptr
	func as lua_CFunction
end type

declare sub luaL_checkversion_(byval L as lua_State ptr, byval ver as lua_Number)
#define luaL_checkversion(L)	luaL_checkversion_(L, LUA_VERSION_NUM)
declare function luaL_getmetafield(byval L as lua_State ptr, byval obj as long, byval e as const zstring ptr) as long
declare function luaL_callmeta(byval L as lua_State ptr, byval obj as long, byval e as const zstring ptr) as long
declare function luaL_tolstring(byval L as lua_State ptr, byval idx as long, byval len as uinteger ptr) as const zstring ptr
declare function luaL_argerror(byval L as lua_State ptr, byval numarg as long, byval extramsg as const zstring ptr) as long
declare function luaL_checklstring(byval L as lua_State ptr, byval numArg as long, byval l as uinteger ptr) as const zstring ptr
declare function luaL_optlstring(byval L as lua_State ptr, byval numArg as long, byval def as const zstring ptr, byval l as uinteger ptr) as const zstring ptr
declare function luaL_checknumber(byval L as lua_State ptr, byval numArg as long) as lua_Number
declare function luaL_optnumber(byval L as lua_State ptr, byval nArg as long, byval def as lua_Number) as lua_Number
declare function luaL_checkinteger(byval L as lua_State ptr, byval numArg as long) as lua_Integer
declare function luaL_optinteger(byval L as lua_State ptr, byval nArg as long, byval def as lua_Integer) as lua_Integer
declare function luaL_checkunsigned(byval L as lua_State ptr, byval numArg as long) as lua_Unsigned
declare function luaL_optunsigned(byval L as lua_State ptr, byval numArg as long, byval def as lua_Unsigned) as lua_Unsigned
declare sub luaL_checkstack(byval L as lua_State ptr, byval sz as long, byval msg as const zstring ptr)
declare sub luaL_checktype(byval L as lua_State ptr, byval narg as long, byval t as long)
declare sub luaL_checkany(byval L as lua_State ptr, byval narg as long)
declare function luaL_newmetatable(byval L as lua_State ptr, byval tname as const zstring ptr) as long
declare sub luaL_setmetatable(byval L as lua_State ptr, byval tname as const zstring ptr)
declare function luaL_testudata(byval L as lua_State ptr, byval ud as long, byval tname as const zstring ptr) as any ptr
declare function luaL_checkudata(byval L as lua_State ptr, byval ud as long, byval tname as const zstring ptr) as any ptr
declare sub luaL_where(byval L as lua_State ptr, byval lvl as long)
declare function luaL_error(byval L as lua_State ptr, byval fmt as const zstring ptr, ...) as long
declare function luaL_checkoption(byval L as lua_State ptr, byval narg as long, byval def as const zstring ptr, byval lst as const zstring const ptr const ptr) as long
declare function luaL_fileresult(byval L as lua_State ptr, byval stat as long, byval fname as const zstring ptr) as long
declare function luaL_execresult(byval L as lua_State ptr, byval stat as long) as long

#define LUA_NOREF (-2)
#define LUA_REFNIL (-1)

declare function luaL_ref(byval L as lua_State ptr, byval t as long) as long
declare sub luaL_unref(byval L as lua_State ptr, byval t as long, byval ref as long)
declare function luaL_loadfilex(byval L as lua_State ptr, byval filename as const zstring ptr, byval mode as const zstring ptr) as long

#define luaL_loadfile(L,f)	luaL_loadfilex(L,f,NULL)

declare function luaL_loadbufferx(byval L as lua_State ptr, byval buff as const zstring ptr, byval sz as uinteger, byval name as const zstring ptr, byval mode as const zstring ptr) as long
declare function luaL_loadstring(byval L as lua_State ptr, byval s as const zstring ptr) as long
declare function luaL_newstate() as lua_State ptr
declare function luaL_len(byval L as lua_State ptr, byval idx as long) as long
declare function luaL_gsub(byval L as lua_State ptr, byval s as const zstring ptr, byval p as const zstring ptr, byval r as const zstring ptr) as const zstring ptr
declare sub luaL_setfuncs(byval L as lua_State ptr, byval l as const luaL_Reg ptr, byval nup as long)
declare function luaL_getsubtable(byval L as lua_State ptr, byval idx as long, byval fname as const zstring ptr) as long
declare sub luaL_traceback(byval L as lua_State ptr, byval L1 as lua_State ptr, byval msg as const zstring ptr, byval level as long)
declare sub luaL_requiref(byval L as lua_State ptr, byval modname as const zstring ptr, byval openf as lua_CFunction, byval glb as long)

''#define luaL_newlibtable(L,l_)	lua_createtable(L, 0, sizeof(l_)/sizeof((l_)[0]) - 1)
#define luaL_newlib(L,l_)	luaL_newlibtable(L,l_) : luaL_setfuncs(L,l_,0)
#define luaL_argcheck(L, cond,numarg,extramsg)	if (cond) = 0 then : luaL_argerror(L, (numarg), (extramsg)) : end if
#define luaL_checkstring(L,n)	(luaL_checklstring(L, (n), NULL))
#define luaL_optstring(L,n,d)	(luaL_optlstring(L, (n), (d), NULL))
#define luaL_checkint(L,n)	clng(luaL_checkinteger(L, (n)))
#define luaL_optint(L,n,d)	clng(luaL_optinteger(L, (n), (d)))
#define luaL_checklong(L,n)	clng(luaL_checkinteger(L, (n)))
#define luaL_optlong(L,n,d)	clng(luaL_optinteger(L, (n), (d)))
#define luaL_typename(L,i)	lua_typename(L, lua_type(L,(i)))
private function luaL_dofile(byval L as lua_State ptr, byval fn as const zstring ptr) as long
	function = (luaL_loadfile(L, fn) orelse lua_pcall(L, 0, LUA_MULTRET, 0))
end function
private function luaL_dostring(byval L as lua_State ptr, byval s as const zstring ptr) as long
	function = (luaL_loadstring(L, s) orelse lua_pcall(L, 0, LUA_MULTRET, 0))
end function
#define luaL_getmetatable(L,n)	(lua_getfield(L, LUA_REGISTRYINDEX, (n)))
#define luaL_opt(L,f,n,d)	iif(lua_isnoneornil(L,(n)), (d), f(L,(n)))
#define luaL_loadbuffer(L,s,sz,n)	luaL_loadbufferx(L,s,sz,n,NULL)

type luaL_Buffer
	b as zstring ptr
	size as uinteger
	n as uinteger
	L as lua_State ptr
	initb(0 to LUAL_BUFFERSIZE-1) as byte
end type

#macro luaL_addchar(B,c)
	if (B)->n >= (B)->size then
		luaL_prepbuffsize((B), 1)
	end if
	(B)->b[(B)->n] = (c)
	(B)->n += 1
#endmacro

#define luaL_addsize(B,s)	(B)->n += (s)

declare sub luaL_buffinit(byval L as lua_State ptr, byval B as luaL_Buffer ptr)
declare function luaL_prepbuffsize(byval B as luaL_Buffer ptr, byval sz as uinteger) as zstring ptr
declare sub luaL_addlstring(byval B as luaL_Buffer ptr, byval s as const zstring ptr, byval l as uinteger)
declare sub luaL_addstring(byval B as luaL_Buffer ptr, byval s as const zstring ptr)
declare sub luaL_addvalue(byval B as luaL_Buffer ptr)
declare sub luaL_pushresult(byval B as luaL_Buffer ptr)
declare sub luaL_pushresultsize(byval B as luaL_Buffer ptr, byval sz as uinteger)
declare function luaL_buffinitsize(byval L as lua_State ptr, byval B as luaL_Buffer ptr, byval sz as uinteger) as zstring ptr

#define luaL_prepbuffer(B)	luaL_prepbuffsize(B, LUAL_BUFFERSIZE)

#define LUA_FILEHANDLE "FILE*"

type luaL_Stream
	f as FILE ptr
	closef as lua_CFunction
end type

#ifdef LUA_COMPAT_MODULE
declare sub luaL_pushmodule(byval L as lua_State ptr, byval modname as const zstring ptr, byval sizehint as long)
declare sub luaL_openlib(byval L as lua_State ptr, byval libname as const zstring ptr, byval l as const luaL_Reg ptr, byval nup as long)
#define luaL_register(L,n,l)	(luaL_openlib(L,(n),(l),0))
#endif

end extern
