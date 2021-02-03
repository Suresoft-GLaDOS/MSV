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


'' proc parameters list declarations (called "arg" by mistake)
''
'' chng: sep/2004 written [v1ctor]


#include once "inc\fb.bi"
#include once "inc\fbint.bi"
#include once "inc\parser.bi"
#include once "inc\ast.bi"

declare function hParamDecl	_
	( _
		byval proc as FBSYMBOL ptr, _
		byval procmode as integer, _
		byval isproto as integer _
	) as FBSYMBOL ptr

declare function hParamDeclInstPtr _
	( _
		byval parent as FBSYMBOL ptr, _
		byval proc as FBSYMBOL ptr _
	) as FBSYMBOL ptr

'':::::
''Parameters=   '(' ParamDecl (',' ParamDecl)* ')' .
''
function cParameters _
	( _
		byval parent as FBSYMBOL ptr, _
		byval proc as FBSYMBOL ptr, _
		byval procmode as integer, _
		byval isproto as integer _
	) as FBSYMBOL ptr

	dim as FBSYMBOL ptr param = any

    '' method? add the instance pointer (must be done here
    '' to check for dups)
    if( symbIsMethod( proc ) ) then
    	param = symAddProcInstancePtr( parent, proc )
    else
    	param = NULL
    end if

	'' '('?
	if( lexGetToken( ) <> CHAR_LPRNT ) then
		return param
	end if

	lexSkipToken( )

	'' ')'?
	if( lexGetToken( ) = CHAR_RPRNT ) then
		lexSkipToken( )
		return param
	end if

	do
		param = hParamDecl( proc, procmode, isproto )
		if( param = NULL ) then
			exit do
		end if

		'' vararg?
		if( param->param.mode = FB_PARAMMODE_VARARG ) then
			exit do
		end if

		'' ','
	   	if( lexGetToken( ) <> CHAR_COMMA ) then
	   		exit do
	   	end if

		lexSkipToken( )
	loop

	'' ')'?
	if( lexGetToken( ) <> CHAR_RPRNT ) then
		if( errReport( FB_ERRMSG_EXPECTEDRPRNT ) = FALSE ) then
			exit function
		else
			'' error recovery: skip until ')'
			hSkipUntil( CHAR_RPRNT, TRUE )
		end if
	else
		lexSkipToken( )
	end if

	function = param

end function

'':::::
private function hParamError _
	( _
		byval proc as FBSYMBOL ptr, _
		byval pid as zstring ptr, _
		byval msgnum as FB_ERRMSG = FB_ERRMSG_ILLEGALPARAMSPECAT _
	) as integer

	function = errReportParam( proc, _
							   symbGetProcParams( proc )+1, _
							   pid, _
							   msgnum _
							 )

end function

'':::::
private sub hParamWarning _
	( _
		byval proc as FBSYMBOL ptr, _
		byval pid as zstring ptr, _
		byval msgnum as FB_ERRMSG _
	)

	errReportParamWarn( proc, _
						symbGetProcParams( proc )+1, _
						pid, _
						msgnum )

end sub

'':::::
private function hOptionalExpr _
	( _
		byval proc as FBSYMBOL ptr, _
		byval pid as zstring ptr, _
		byval mode as FB_PARAMMODE, _
		byval dtype as FB_DATATYPE, _
		byval subtype as FBSYMBOL ptr _
	) as ASTNODE ptr

    dim as ASTNODE ptr expr = any
    dim as FBSYMBOL ptr sym = any

    function = NULL

    '' not byval or byref?
    if( mode <> FB_PARAMMODE_BYVAL ) then
    	if( mode <> FB_PARAMMODE_BYREF ) then
    		exit function
    	end if
    end if

    select case as const typeGet( dtype )
    '' UDT? let SymbolInit() build a tree..
    case FB_DATATYPE_STRUCT ', FB_DATATYPE_CLASS
    	sym = symbAddTempVar( dtype, subtype, FALSE, FALSE )

    	expr = cInitializer( sym, FB_INIOPT_ISINI )
    	if( expr = NULL ) then
    		exit function
    	end if

    	'' del temp var
    	astReplaceSymbolOnTree( expr, sym, NULL )
    	symbDelVar( sym )

    '' anything else..
	case else
    	expr = cExpression( )
    	if( expr = NULL ) then
    		exit function
    	end if

    	'' check for invalid types..
    	static as ASTNODE lside

    	astBuildVAR( @lside, NULL, 0, dtype, subtype )

    	if( astCheckASSIGN( @lside, expr ) = FALSE ) then
        	exit function
		end if

    end select

	'' remove the temps from the dtors list if any was added
	astDtorListClear( )

    '' don't allow references to local symbols
    sym = astFindLocalSymbol( expr )
    if( sym <> NULL ) then
    	if( hParamError( proc, _
    					 pid, _
    					 FB_ERRMSG_INVALIDREFERENCETOLOCAL ) = FALSE ) then
    		return NULL
    	else
    		'' no error recovery, caller will take care
    		astDelTree( expr )
    		expr = NULL
    	end if
    end if

	function = expr

end function

'':::::
private function hMockParam _
	( _
		byval proc as FBSYMBOL ptr, _
		byval pmode as integer = INVALID _
	) as FBSYMBOL ptr

	dim as FBSYMBOL ptr s
	dim as integer dtype, plen

	if( pmode = INVALID ) then
		pmode = env.opt.parammode
	end if

    dtype = FB_DATATYPE_INTEGER

    select case pmode
    case FB_PARAMMODE_BYREF, FB_PARAMMODE_BYDESC
    	plen = FB_POINTERSIZE

    case FB_PARAMMODE_BYVAL
    	if( dtype = FB_DATATYPE_STRING ) then
    		plen = FB_POINTERSIZE
    	else
    		plen = symbCalcLen( dtype, NULL )
    	end if

    case FB_PARAMMODE_VARARG
    	dtype = FB_DATATYPE_INVALID
    	plen = 0
    end select

	s = symbAddProcParam( proc, _
						  NULL, _
						  NULL, _
						  dtype, NULL, plen, _
					  	  pmode, _
					  	  0, NULL )

	symbSetIsMock( s )

	function = s

end function

'':::::
'' ParamDecl      =   (BYVAL|BYREF)? ID (('(' ')')? (AS SymbolType)?)? ('=" (NUM_LIT|STR_LIT))? .
''
private function hParamDecl _
	( _
		byval proc as FBSYMBOL ptr, _
		byval proc_mode as integer, _
		byval isproto as integer _
	) as FBSYMBOL ptr

	static as zstring * FB_MAXNAMELEN+1 idTB(0 to FB_MAXARGRECLEVEL-1)
	static as integer reclevel = 0
	dim as zstring ptr id = any
	dim as ASTNODE ptr optval = any
	dim as integer dtype = any, mode = any, param_len = any
	dim as integer attrib = any
	dim as integer readid = any, dotpos = any, doskip = any, dontinit = any, use_default = any
	dim as FBSYMBOL ptr subtype = any, s = any

	function = NULL

    attrib = 0

	'' '...'?
	if( lexGetToken( ) = CHAR_DOT ) then
		if( lexGetLookAhead( 1 ) = CHAR_DOT ) then
		    lexSkipToken( )
		    lexSkipToken( )

		    ''
		    if( lexGetToken( ) <> CHAR_DOT ) then
		    	if( hParamError( proc, "..." ) = FALSE ) then
		    		exit function
		    	else
		    		'' error recovery: skip until next ')', this is the last param
		    		hSkipUntil( CHAR_RPRNT )
		    		hMockParam( proc, FB_PARAMMODE_VARARG )
		    	end if
			else
				lexSkipToken( )
		    end if

			'' not cdecl or is it the first arg?
			if( (proc_mode <> FB_FUNCMODE_CDECL) or _
				(symbGetProcParams( proc ) = 0) ) then
				if( hParamError( proc, "..." ) = FALSE ) then
					exit function
				else
					return hMockParam( proc, FB_PARAMMODE_VARARG )
				end if
			end if

			return symbAddProcParam( proc, _
									 NULL, NULL, _
						   	     	 FB_DATATYPE_INVALID, NULL, 0, _
						   	     	 FB_PARAMMODE_VARARG, _
						   	      	 0, NULL )

		'' syntax error..
		else
		    if( hParamError( proc, "..." ) = FALSE ) then
		    	exit function
		    else
		    	'' error recovery: skip until next ')', this is the last param
		    	hSkipUntil( CHAR_RPRNT )
		    	return hMockParam( proc, FB_PARAMMODE_VARARG )
		    end if
		end if
	end if

	'' (BYVAL|BYREF)?
	select case lexGetToken( )
	case FB_TK_BYVAL
		mode = FB_PARAMMODE_BYVAL
		lexSkipToken( )
	case FB_TK_BYREF
		mode = FB_PARAMMODE_BYREF
		lexSkipToken( )
	case else
		mode = INVALID
	end select

	'' only allow keywords as param names on prototypes
	readid = TRUE
	if( lexGetClass( ) <> FB_TKCLASS_IDENTIFIER ) then

		select case lexGetClass( )
		case FB_TKCLASS_KEYWORD, FB_TKCLASS_QUIRKWD
			if( isproto = FALSE ) then
				if( hParamError( proc, lexGetText( ) ) = FALSE ) then
					exit function
				else
					'' error recovery: skip until next ',' or ')' and return a mock param
					hSkipUntil( CHAR_COMMA )
					return hMockParam( proc, mode )
				end if

			else
				'' AS?
				if( lexGetToken( ) = FB_TK_AS ) then
					readid = FALSE
				end if
			end if

		case else
			if( symbGetProcParams( proc ) > 0 ) then
				if( hParamError( proc, lexGetText( ) ) = FALSE ) then
					exit function
				else
					'' error recovery: skip until next ',' or ')' and return a mock param
					hSkipUntil( CHAR_COMMA )
					return hMockParam( proc, mode )
				end if
			end if
			exit function
		end select

	end if

	''
	if( reclevel >= FB_MAXARGRECLEVEL ) then
		if( errReport( FB_ERRMSG_RECLEVELTOODEEP ) = FALSE ) then
			exit function
		else
			'' error recovery: skip until next ',' or ')' and return a mock param
			hSkipUntil( CHAR_COMMA )
			return hMockParam( proc, mode )
		end if
	end if

	id = @idTB(reclevel)
	*id = ""

	''
	if( readid ) then
		'' ID
		*id = *lexGetText( )
		dotpos = lexGetPeriodPos( )

		dtype = lexGetType( )
		hCheckSuffix( dtype )

		lexSkipToken( )

		'' ('('')')
		if( lexGetToken( ) = CHAR_LPRNT ) then
			lexSkipToken( )
			if( (mode <> INVALID) or _
				(hMatch( CHAR_RPRNT ) = FALSE) ) then
				if( hParamError( proc, id ) = FALSE ) then
					exit function
				end if
			end if

			mode = FB_PARAMMODE_BYDESC
		end if

	'' no id
	else
		dtype  = FB_DATATYPE_INVALID
	end if

	use_default = FALSE
	if( mode = INVALID ) then
		mode = env.opt.parammode
		use_default = TRUE
    	if( fbPdCheckIsSet( FB_PDCHECK_PARAMMODE ) ) then
    		hParamWarning( proc, id, FB_WARNINGMSG_NOEXPLICITPARAMMODE )
    	end if
	end if

    '' (AS SymbolType)?
    doskip = FALSE
    if( lexGetToken( ) = FB_TK_AS ) then
    	lexSkipToken( )
    	if( dtype <> FB_DATATYPE_INVALID ) then
    		if( hParamError( proc, id ) ) then
    			exit function
    		else
    			dtype = FB_DATATYPE_INVALID
    		end if
    	end if

    	reclevel += 1

    	'' if it's a proto, allow forward types in byref params
    	dim as integer options = FB_SYMBTYPEOPT_DEFAULT

    	if( mode = FB_PARAMMODE_BYREF ) then
			if( isproto ) then
				options or= FB_SYMBTYPEOPT_ALLOWFORWARD
			end if
			options and= not FB_SYMBTYPEOPT_CHECKSTRPTR
		end if

    	if( cSymbolType( dtype, subtype, _
    					 param_len, options ) = FALSE ) then
    		if( hParamError( proc, id ) = FALSE ) then
    			reclevel -= 1
    			exit function
    		else
    			'' error recovery: fake type
    			dtype = FB_DATATYPE_INTEGER
    			subtype = NULL
    			doskip = TRUE
    		end if
    	end if
    	reclevel -= 1

    else
        if( fbLangOptIsSet( FB_LANG_OPT_DEFTYPE ) = FALSE ) then
        	if( errReportNotAllowed( FB_LANG_OPT_DEFTYPE, _
        							 FB_ERRMSG_DEFTYPEONLYVALIDINLANG ) = FALSE ) then
				exit function
			else
				doskip = TRUE
			end if
        else
    		if( readid = FALSE ) then
    			if( hParamError( proc, NULL ) = FALSE ) then
    				exit function
    			else
    				doskip = TRUE
    			end if
    		end if
    	end if

    	subtype = NULL

    	attrib or= FB_SYMBATTRIB_SUFFIXED
    end if

	'' in lang FB,
	if( fbLangIsSet( FB_LANG_FB ) ) then

		'' we have to delay the true default until now, since
		'' byval/byref depends on the symbol type
		if( use_default ) then
			mode = symbGetDefaultCallConv( typeGet( dtype ), subtype )
		end if

	end if

    '' QB def-by-letter hax
    if( dtype = FB_DATATYPE_INVALID ) then
        dtype = symbGetDefType( id )
    end if

	if( doskip ) then
		hSkipUntil( CHAR_COMMA )
	end if

    '' check for invalid args
    select case as const typeGet( dtype )
    '' can't be a fixed-len string
	case FB_DATATYPE_FIXSTR, FB_DATATYPE_CHAR, FB_DATATYPE_WCHAR
		if( mode = FB_PARAMMODE_BYVAL or typeGet( dtype ) = FB_DATATYPE_FIXSTR ) then
			if( hParamError( proc, id ) = FALSE ) then
				exit function
			else
				'' error recovery: fake correct type
				dtype = typeAddrOf( dtype )
			end if
		end if

	'' can't be as ANY on non-prototypes
    case FB_DATATYPE_VOID
    	if( isproto = FALSE ) then
    		if( hParamError( proc, id ) = FALSE ) then
    			exit function
    		else
    			'' error recovery: fake correct type
    			dtype = typeAddrOf( dtype )
    		end if

    	else
    		if( mode = FB_PARAMMODE_BYVAL ) then
    			if( hParamError( proc, id ) = FALSE ) then
    				exit function
    			else
    				'' error recovery: fake correct param
    				dtype = typeAddrOf( dtype )
    			end if
    		end if
    	end if

    case FB_DATATYPE_STRUCT
    	if( isproto = FALSE ) then
    		'' contains a period?
    		if( dotpos > 0 ) then
    			if( hParamError( proc, id ) = FALSE ) then
    				exit function
    			end if
    		end if
    	end if

	case FB_DATATYPE_STRING
		if( mode = FB_PARAMMODE_BYVAL ) then
			if( fbPdCheckIsSet( FB_PDCHECK_PARAMMODE ) ) then
				hParamWarning( proc, id, FB_WARNINGMSG_BYVALASSTRING )
			end if
		end if

    end select

    '' calc len
   	param_len = symbCalcProcParamLen( typeGet( dtype ), subtype, mode )
   	if( isproto = FALSE ) then
   		if( param_len > (FB_INTEGERSIZE * 4) ) then
   			if( fbPdCheckIsSet( FB_PDCHECK_PARAMSIZE ) ) then
   				hParamWarning( proc, id, FB_WARNINGMSG_PARAMSIZETOOBIG )
   			end if
   		end if
   	end if

    '' default values
   	optval = NULL
   	dontinit = FALSE

    '' ('=' (expr | ANY))?
    if( lexGetToken( ) = FB_TK_ASSIGN ) then
    	lexSkipToken( )

    	if( mode = FB_PARAMMODE_BYDESC ) then
    		'' ANY?
    		if( lexGetToken( ) = FB_TK_ANY ) then
            	lexSkipToken( )

				dontinit = TRUE

    		else
 	   			if( hParamError( proc, id ) = FALSE ) then
 	   				exit function
 	   			else
 	   				'' error recovery: skip until next ',' or ')'
 	   				hSkipUntil( CHAR_COMMA )
 	   			end if
    		end if

    	else
        	attrib or= FB_SYMBATTRIB_OPTIONAL
			optval = hOptionalExpr( proc, id, mode, dtype, subtype )

			if( optval = NULL ) then
 	   			if( hParamError( proc, id ) = FALSE ) then
 	   				exit function
 	   			else
 	   				'' error recovery: skip until next ',' or ')' and create a def value
 	   				hSkipUntil( CHAR_COMMA )
					if( dtype <> FB_DATATYPE_STRUCT ) then
						optval = astNewCONSTz( dtype )
					else
						attrib and= not FB_SYMBATTRIB_OPTIONAL
					end if
 	   			end if
 	   		end if
    	end if

    end if

    if( isproto ) then
    	id = NULL
    end if

    s = symbAddProcParam( proc, _
    					  id, NULL, _
    					  dtype, subtype, _
    					  param_len, mode, _
    					  attrib, optval )

	if( s <> NULL ) then
		if( dontinit ) then
			symbSetDontInit( s )
		end if
	end if

	function = s

end function

