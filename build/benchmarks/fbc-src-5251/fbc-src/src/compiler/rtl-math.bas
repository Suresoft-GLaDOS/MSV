''	FreeBASIC - 32-bit BASIC Compiler.
''	Copyright (C) 2004-2008 The FreeBASIC development team.
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


'' intrinsic runtime lib math functions (FIX, ACOS, LOG, ...)
''
'' chng: oct/2004 written [v1ctor]


#include once "inc\fb.bi"
#include once "inc\fbint.bi"
#include once "inc\ast.bi"
#include once "inc\rtl.bi"

	dim shared as FB_RTL_PROCDEF funcdata( 0 to 19 ) = _
	{ _
		/' fb_LongintDIV ( byval x as longint, byval y as longint ) as longint '/ _
		( _
			@FB_RTL_LONGINTDIV, NULL, _
			FB_DATATYPE_LONGINT, FB_FUNCMODE_CDECL, _
			NULL, FB_RTL_OPT_NONE, _
			2, _
	 		{ _
	 			( _
					FB_DATATYPE_LONGINT, FB_PARAMMODE_BYVAL, FALSE _
	 			), _
	 			( _
					FB_DATATYPE_LONGINT, FB_PARAMMODE_BYVAL, FALSE _
	 			) _
	 		} _
		), _
		/' fb_ULongintDIV ( byval x as ulongint, byval y as ulongint ) as ulongint '/ _
		( _
			@FB_RTL_ULONGINTDIV, NULL, _
			FB_DATATYPE_ULONGINT, FB_FUNCMODE_CDECL, _
			NULL, FB_RTL_OPT_NONE, _
			2, _
	 		{ _
	 			( _
					FB_DATATYPE_ULONGINT, FB_PARAMMODE_BYVAL, FALSE _
	 			), _
	 			( _
					FB_DATATYPE_ULONGINT, FB_PARAMMODE_BYVAL, FALSE _
	 			) _
	 		} _
		), _
		/' fb_LongintMOD ( byval x as longint, byval y as longint ) as longint '/ _
		( _
			@FB_RTL_LONGINTMOD, NULL, _
			FB_DATATYPE_LONGINT, FB_FUNCMODE_CDECL, _
			NULL, FB_RTL_OPT_NONE, _
			2, _
	 		{ _
	 			( _
					FB_DATATYPE_LONGINT, FB_PARAMMODE_BYVAL, FALSE _
	 			), _
	 			( _
					FB_DATATYPE_LONGINT, FB_PARAMMODE_BYVAL, FALSE _
	 			) _
	 		} _
		), _
		/' fb_ULongintMOD ( byval x as ulongint, byval y as ulongint ) as ulongint '/ _
		( _
			@FB_RTL_ULONGINTMOD, NULL, _
			FB_DATATYPE_ULONGINT, FB_FUNCMODE_CDECL, _
			NULL, FB_RTL_OPT_NONE, _
			2, _
	 		{ _
	 			( _
					FB_DATATYPE_ULONGINT, FB_PARAMMODE_BYVAL, FALSE _
	 			), _
	 			( _
					FB_DATATYPE_ULONGINT, FB_PARAMMODE_BYVAL, FALSE _
	 			) _
	 		} _
		), _
		/' fb_Dbl2ULongint ( byval x as double ) as ulongint '/ _
		( _
			@FB_RTL_DBL2ULONGINT, NULL, _
			FB_DATATYPE_ULONGINT, FB_FUNCMODE_CDECL, _
			NULL, FB_RTL_OPT_NONE, _
			1, _
	 		{ _
	 			( _
					FB_DATATYPE_DOUBLE, FB_PARAMMODE_BYVAL, FALSE _
	 			) _
	 		} _
		), _
		/' fb_Pow CDECL ( byval x as double, byval y as double ) as double '/ _
		( _
			@FB_RTL_POW, @"pow", _
			FB_DATATYPE_DOUBLE, FB_FUNCMODE_CDECL, _
			NULL, FB_RTL_OPT_NONE, _
			2, _
	 		{ _
	 			( _
					FB_DATATYPE_DOUBLE, FB_PARAMMODE_BYVAL, FALSE _
	 			), _
	 			( _
					FB_DATATYPE_DOUBLE, FB_PARAMMODE_BYVAL, FALSE _
	 			) _
	 		} _
		), _
		/' randomize ( byval seed as double = -1.0, byval algorithm as integer = 0 ) as void '/ _
		( _
			@"randomize", @"fb_Randomize", _
			FB_DATATYPE_VOID, FB_FUNCMODE_STDCALL, _
			NULL, FB_RTL_OPT_NONE, _
			2, _
	 		{ _
	 			( _
					FB_DATATYPE_DOUBLE, FB_PARAMMODE_BYVAL, TRUE, -1.0 _
	 			), _
	 			( _
					FB_DATATYPE_INTEGER, FB_PARAMMODE_BYVAL, TRUE, 0 _
	 			) _
	 		} _
		), _
		/' rnd ( byval n as single ) as double '/ _
		( _
			@"rnd", @"fb_Rnd", _
			FB_DATATYPE_DOUBLE, FB_FUNCMODE_STDCALL, _
			NULL, FB_RTL_OPT_NONE, _
			1, _
	 		{ _
	 			( _
					FB_DATATYPE_SINGLE, FB_PARAMMODE_BYVAL, TRUE, 1.0 _
	 			) _
	 		} _
	 	), _
	 	/' EOL '/ _
	 	( _
	 		NULL _
	 	) _
	 }


'':::::
sub rtlMathModInit( )

	rtlAddIntrinsicProcs( @funcdata(0) )

end sub

'':::::
sub rtlMathModEnd( )

	'' procs will be deleted when symbEnd is called

end sub

'':::::
function rtlMathPow	_
	( _
		byval xexpr as ASTNODE ptr, _
		byval yexpr as ASTNODE ptr _
	) as ASTNODE ptr

    dim as ASTNODE ptr proc = any

	function = NULL

	''
    proc = astNewCALL( PROCLOOKUP( POW ) )

    '' byval x as double
    if( astNewARG( proc, xexpr ) = NULL ) then
 		exit function
 	end if

    '' byval y as double
    if( astNewARG( proc, yexpr ) = NULL ) then
 		exit function
 	end if

    ''
    function = proc

end function

'':::::
function rtlMathLen _
	( _
		byval expr as ASTNODE ptr, _
		byval islen as integer = TRUE _
	) as ASTNODE ptr

    dim as ASTNODE ptr proc = any
    dim as integer dtype = any, lgt = any
    dim as FBSYMBOL ptr litsym = any

	function = NULL

	dtype = astGetDataType( expr )

	'' LEN()?
	if( islen ) then
		select case as const dtype
		'' dyn-len or zstring?
		case FB_DATATYPE_STRING, FB_DATATYPE_CHAR

    		'' literal? evaluate at compile-time..
    		if( dtype = FB_DATATYPE_CHAR ) then
    			litsym = astGetStrLitSymbol( expr )
    			if( litsym <> NULL ) then
    				lgt = symbGetStrLen( litsym ) - 1
    			end if
    		else
    			litsym = NULL
    		end if

    		if( litsym = NULL ) then
    			proc = astNewCALL( PROCLOOKUP( STRLEN ) )

    			'' always calc len before pushing the param
    			lgt = rtlCalcStrLen( expr, dtype )

    			'' str as any
    			if( astNewARG( proc, expr, FB_DATATYPE_STRING ) = NULL ) then
 					exit function
 				end if

    			'' byval strlen as integer
				if( astNewARG( proc, _
							   astNewCONSTi( lgt, FB_DATATYPE_INTEGER ), _
							   FB_DATATYPE_INTEGER ) = NULL ) then
 					exit function
 				end if

				return proc
			end if

		'' wstring?
		case FB_DATATYPE_WCHAR

    		'' literal? evaluate at compile-time..
    		litsym = astGetStrLitSymbol( expr )
    		if( litsym <> NULL ) then
    			lgt = symbGetWstrLen( litsym ) - 1

    		else
    			proc = astNewCALL( PROCLOOKUP( WSTRLEN ) )

    			'' byval str as wchar ptr
    			if( astNewARG( proc, expr ) = NULL ) then
 					exit function
 				end if

 				return proc
 			end if

		'' anything else..
		case else
type_size:
			lgt = rtlCalcExprLen( expr, FALSE )

			'' handle fix-len strings (evaluated at compile-time)
			if( dtype = FB_DATATYPE_FIXSTR ) then
				if( lgt > 0 ) then
					lgt -= 1						'' less the null-term
				end if
			end if

		end select

	'' SIZEOF()
	else
		lgt = rtlCalcExprLen( expr, FALSE )

		'' wstring? multiply by sizeof(wchar) to get the
		'' number of bytes, not of chars
		if( dtype = FB_DATATYPE_WCHAR ) then
			lgt *= symbGetDataSize( FB_DATATYPE_WCHAR )
		end if

	end if

	''
	astDelTree( expr )

	function = astNewCONSTi( lgt, FB_DATATYPE_INTEGER )

end function

'':::::
function rtlMathLongintDIV _
	( _
		byval dtype as integer, _
		byval lexpr as ASTNODE ptr, _
		byval ldtype as integer, _
		byval rexpr as ASTNODE ptr, _
		byval rdtype as integer _
	) as ASTNODE ptr

    dim as ASTNODE ptr proc = any
    dim as FBSYMBOL ptr f = any

	function = NULL
	
	if( typeGet( dtype ) = FB_DATATYPE_LONGINT ) then
		f = PROCLOOKUP( LONGINTDIV )
	else
		f = PROCLOOKUP( ULONGINTDIV )
	end if

    proc = astNewCALL( f )

    ''
    if( astNewARG( proc, lexpr, ldtype ) = NULL ) then
    	exit function
    end if

    if( astNewARG( proc, rexpr, rdtype ) = NULL ) then
    	exit function
    end if

    function = proc

end function

'':::::
function rtlMathLongintMOD _
	( _
		byval dtype as integer, _
		byval lexpr as ASTNODE ptr, _
		byval ldtype as integer, _
		byval rexpr as ASTNODE ptr, _
		byval rdtype as integer _
	) as ASTNODE ptr

    dim as ASTNODE ptr proc = any
    dim as FBSYMBOL ptr f = any

	function = NULL
	
	if( typeGet( dtype ) = FB_DATATYPE_LONGINT ) then
		f = PROCLOOKUP( LONGINTMOD )
	else
		f = PROCLOOKUP( ULONGINTMOD )
	end if

    proc = astNewCALL( f )

    ''
    if( astNewARG( proc, lexpr, ldtype ) = NULL ) then
    	exit function
    end if

    if( astNewARG( proc, rexpr, rdtype ) = NULL ) then
    	exit function
    end if

    function = proc

end function

'':::::
function rtlMathFp2ULongint _
	( _
		byval expr as ASTNODE ptr, _
		byval dtype as integer _
	) as ASTNODE ptr

    dim as ASTNODE ptr proc = any
    
	function = NULL

    proc = astNewCALL( PROCLOOKUP( DBL2ULONGINT)  )

    ''
    if( astNewARG( proc, expr, dtype ) = NULL ) then
    	exit function
    end if

    function = proc

end function

