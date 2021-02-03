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

'' pre-processor #define parsing (including macros)
''
'' chng: dec/2004 written [v1ctor]


#include once "inc\fb.bi"
#include once "inc\fbint.bi"
#include once "inc\lex.bi"
#include once "inc\parser.bi"
#include once "inc\pp.bi"
#include once "inc\list.bi"
#include once "inc\dstr.bi"

#define LEX_FLAGS (LEXCHECK_NOWHITESPC or _
				   LEXCHECK_NOSUFFIX or _
				   LEXCHECK_NODEFINE or _
				   LEXCHECK_NOQUOTES or _
				   LEXCHECK_NOSYMBOL)

type LEXPP_ARG
	union
		text		as DZSTRING
		textw		as DWSTRING
	end union
end type

type LEXPP_ARGTB
	tb(0 to FB_MAXDEFINEARGS-1) as LEXPP_ARG
end type

''::::
sub ppDefineInit( )

	listNew( @pp.argtblist, 8, len( LEXPP_ARGTB ), LIST_FLAGS_NOCLEAR )

end sub

''::::
sub ppDefineEnd( )

	listFree( @pp.argtblist )

end sub

'':::::
private function hReportMacroError _
	( _
		byval s as FBSYMBOL ptr, _
		byval errnum as integer _
	) as integer

	function = errReportEx( errnum, "expanding: " + *symbGetName( s ) )

end function

'':::::
private function hLoadMacro _
	( _
		byval s as FBSYMBOL ptr _
	) as integer

    dim as FB_DEFPARAM ptr param = any
    dim as FB_DEFTOK ptr dt = any
    dim as FBTOKEN t = any
    dim as LEXPP_ARGTB ptr argtb = any
    dim as integer prntcnt = any, num = any, doskip = any
    static as string text

	function = -1

	'' '('?
	if( lexCurrentChar( TRUE ) <> CHAR_LPRNT ) then
		'' not an error, macro can be passed as param to other macros
		exit function
	end if

	lexEatChar( )

	'' allocate a new arg list (support recursion)
	param = symbGetDefineHeadParam( s )
	if( param <> NULL ) then
		argtb = listNewNode( @pp.argtblist )
	else
		argtb = NULL
	end if

	prntcnt = 1

	'' for each arg
	num = 0
	do
		if( argtb <> NULL ) then
			DZstrZero( argtb->tb(num).text )
		end if

		'' read text until a comma or right-parentheses is found
		do
			lexNextToken( @t, LEXCHECK_NOWHITESPC or _
							  LEXCHECK_NOSUFFIX or _
							  LEXCHECK_NOQUOTES or _
							  LEXCHECK_NOPERIOD )

			select case as const t.id
			'' (
			case CHAR_LPRNT
				prntcnt += 1

			'' )
			case CHAR_RPRNT
				prntcnt -= 1
				if( prntcnt = 0 ) then
					exit do
				end if

			'' ,
			case CHAR_COMMA
				if( prntcnt = 1 ) then
					exit do
				end if

			''
			case FB_TK_EOL, FB_TK_EOF
				if( hReportMacroError( s, FB_ERRMSG_EXPECTEDRPRNT ) = FALSE ) then
					exit function
				else
					exit do
				end if
			end select

			if( argtb <> NULL ) then
	   			if( t.dtype <> FB_DATATYPE_WCHAR ) then
	    			DZstrConcatAssign( argtb->tb(num).text, t.text )
	    		else
	    			DZstrConcatAssignW( argtb->tb(num).text, t.textw )
	    		end if
	    	end if
		loop

		''
		if( argtb <> NULL ) then
			'' trim
			with argtb->tb(num)
				if( .text.data = NULL ) then
					if( hReportMacroError( s, FB_ERRMSG_EXPECTEDEXPRESSION ) = FALSE ) then
						exit function
					end if

				else
					if( (.text.data[0][0] = CHAR_SPACE) or _
						(.text.data[0][len( *.text.data )-1] = CHAR_SPACE) ) then
						DZstrAssign( .text, trim( *.text.data ) )
					end if
				end if

			end with
		end if

		'' closing parentheses?
		if( prntcnt = 0 ) then
			exit do
		end if

		'' next
		param = symbGetDefParamNext( param )
		num += 1

		'' too many args?
		if( param = NULL ) then
			if( hReportMacroError( s, FB_ERRMSG_ARGCNTMISMATCH ) = FALSE ) then
				exit function
			else
				'' error recovery: skip until next ')'
				hSkipUntil( CHAR_RPRNT, TRUE, LEX_FLAGS )
				exit do
			end if
		end if
	loop

	'' too few args?
	doskip = FALSE
	if( param <> NULL ) then
		if( symbGetDefParamNext( param ) <> NULL ) then
			if( hReportMacroError( s, FB_ERRMSG_ARGCNTMISMATCH ) = FALSE ) then
				exit function
			else
				doskip = TRUE
			end if
		end if
	end if

	''
	text = ""

	if( argtb <> NULL ) then
		if( doskip = FALSE ) then
			dt = symbGetDefineHeadToken( s )
			do while( dt <> NULL )
				select case as const symbGetDefTokType( dt )
				'' parameter?
				case FB_DEFTOK_TYPE_PARAM
					text += *argtb->tb( symbGetDefTokParamNum( dt ) ).text.data

				'' stringize parameter?
				case FB_DEFTOK_TYPE_PARAMSTR
					'' don't escape, preserve the sequencies as-is
					text += "$" + QUOTE
					text += hReplace( argtb->tb( symbGetDefTokParamNum( dt ) ).text.data, _
								  	  QUOTE, _
								  	  QUOTE + QUOTE )
					text += QUOTE

				'' ordinary text..
				case FB_DEFTOK_TYPE_TEX
					text += *symbGetDefTokText( dt )

				'' unicode text?
				case FB_DEFTOK_TYPE_TEXW
					text += str( *symbGetDefTokTextW( dt ) )
				end select

				'' next
				dt = symbGetDefTokNext( dt )
			loop
		end if

		'' free args text
		do while( num > 0 )
			num -= 1
			DZstrAssign( argtb->tb(num).text, NULL )
		loop

		listDelNode( @pp.argtblist, argtb )
	end if

	''
	if( doskip ) then
		return 0
	end if

	if( lex.ctx->deflen = 0 ) then
		DZstrAssign( lex.ctx->deftext, text )
	else
		DZstrAssign( lex.ctx->deftext, text + *lex.ctx->defptr )
	end if

	function = len( text )

end function

''::::
private function hLoadDefine _
	( _
		byval s as FBSYMBOL ptr _
	) as integer

    static as string text
    dim as integer lgt = any

    function = FALSE

	'' define has args?
	if( symbGetDefineParams( s ) > 0 ) then

		lgt = hLoadMacro( s )
		if( lgt = -1 ) then
			exit function
		end if

	'' no args
	else

		'' should we call a function to get definition text?
		if( symbGetDefineCallback( s ) <> NULL ) then
			'' call function
            if( bit( symbGetDefineFlags( s ), 0 ) = 0 ) then
				text = QUOTE + symbGetDefineCallback( s )( ) + QUOTE
            else
				text = symbGetDefineCallback( s )( )
            end if

			if( lex.ctx->deflen = 0 ) then
				DZstrAssign( lex.ctx->deftext, text )
			else
				DZstrAssign( lex.ctx->deftext, text + *lex.ctx->defptr )
			end if

            lgt = len( text )

		'' just load text as-is
		else

			'' arg-less macro?
			if( symbGetDefineIsArgless( s ) ) then
				'' '('?
				if( lexCurrentChar( TRUE ) <> CHAR_LPRNT ) then
					'' not an error, macro can be passed as param to other macros
					exit function
				end if
				lexEatChar( )

				'' ')'
				if( lexCurrentChar( TRUE ) <> CHAR_RPRNT ) then
					if( errReport( FB_ERRMSG_EXPECTEDRPRNT ) = FALSE ) then
						exit function
					end if

				else
					lexEatChar( )
				end if

			end if

			if( symbGetType( s ) <> FB_DATATYPE_WCHAR ) then
				if( lex.ctx->deflen = 0 ) then
					DZstrAssign( lex.ctx->deftext, symbGetDefineText( s ) )
				else
					DZstrAssign( lex.ctx->deftext, _
								 *symbGetDefineText( s ) + *lex.ctx->defptr )
				end if

			else
				if( lex.ctx->deflen = 0 ) then
					DZstrAssignW( lex.ctx->deftext, symbGetDefineTextW( s ) )
				else
					DZstrAssign( lex.ctx->deftext, _
								 str( *symbGetDefineTextW( s ) ) + *lex.ctx->defptr )
				end if
			end if

            lgt = symbGetLen( s )
		end if

	end if

    ''
	lex.ctx->defptr = lex.ctx->deftext.data
	lex.ctx->deflen += lgt

	'' force a re-read
	lex.ctx->currchar = cuint( INVALID )

	function = TRUE

end function

private function hLoadMacroW _
	( _
		byval s as FBSYMBOL ptr _
	) as integer

    dim as FB_DEFPARAM ptr param = any
    dim as FB_DEFTOK ptr dt = any
    dim as FBTOKEN t = any
    dim as LEXPP_ARGTB ptr argtb = any
    dim as integer prntcnt = any, lgt = any, num = any, doskip = any
    static as DWSTRING text

	function = -1

	'' '('?
	if( lexCurrentChar( TRUE ) <> CHAR_LPRNT ) then
		'' not an error, macro can be passed as param to other macros
		exit function
	end if

	lexEatChar( )

	'' allocate a new arg list (because the recursivity)
	param = symbGetDefineHeadParam( s )
	if( param <> NULL ) then
		argtb = listNewNode( @pp.argtblist )
	else
		argtb = NULL
	end if

	prntcnt = 1

	'' for each arg
	num = 0
	do
		if( argtb <> NULL ) then
			DWstrZero( argtb->tb(num).textw )
		end if

		'' read text until a comma or right-parentheses is found
		do
			lexNextToken( @t, LEXCHECK_NOWHITESPC or _
							  LEXCHECK_NOSUFFIX or _
							  LEXCHECK_NOQUOTES or _
							  LEXCHECK_NOPERIOD )

			select case as const t.id
			'' (
			case CHAR_LPRNT
				prntcnt += 1

			'' )
			case CHAR_RPRNT
				prntcnt -= 1
				if( prntcnt = 0 ) then
					exit do
				end if

			'' ,
			case CHAR_COMMA
				if( prntcnt = 1 ) then
					exit do
				end if

			''
			case FB_TK_EOL, FB_TK_EOF
				if( hReportMacroError( s, FB_ERRMSG_EXPECTEDRPRNT ) = FALSE ) then
					exit function
				else
					exit do
				end if
			end select

			if( argtb <> NULL ) then
    			if( t.dtype <> FB_DATATYPE_WCHAR ) then
    				DWstrConcatAssignA( argtb->tb(num).textw, t.text )
    			else
    				DWstrConcatAssign( argtb->tb(num).textw, t.textw )
    			end if
    		end if
		loop

		if( argtb <> NULL ) then
			'' trim
			with argtb->tb(num)
				if( .textw.data = NULL ) then
					if( hReportMacroError( s, FB_ERRMSG_EXPECTEDEXPRESSION ) = FALSE ) then
						exit function
					end if

				else
					if( (.textw.data[0][0] = CHAR_SPACE) or _
						(.textw.data[0][len( *.textw.data )-1] = CHAR_SPACE) ) then
						DWstrAssign( .textw, trim( *.textw.data ) )
					end if
				end if
			end with
		end if

		'' closing parentheses?
		if( prntcnt = 0 ) then
			exit do
		end if

		'' next
		param = symbGetDefParamNext( param )
		num += 1

		'' too many args?
		if( param = NULL ) then
			if( hReportMacroError( s, FB_ERRMSG_ARGCNTMISMATCH ) = FALSE ) then
				exit function
			else
				'' error recovery: skip until next ')'
				hSkipUntil( CHAR_RPRNT, TRUE, LEX_FLAGS )
				exit do
			end if
		end if
	loop

	'' too few args?
	doskip = FALSE
	if( param <> NULL ) then
		if( symbGetDefParamNext( param ) <> NULL ) then
			if( hReportMacroError( s, FB_ERRMSG_ARGCNTMISMATCH ) = FALSE ) then
				exit function
			else
				doskip = TRUE
			end if
		end if
	end if

	'' text = ""
	DWstrAssign( text, NULL )

	if( argtb <> NULL ) then
		if( doskip = FALSE ) then
			dt = symbGetDefineHeadToken( s )
			do while( dt <> NULL )
				select case as const symbGetDefTokType( dt )
				'' parameter?
				case FB_DEFTOK_TYPE_PARAM
					DWstrConcatAssign( text, _
								   	   argtb->tb( symbGetDefTokParamNum( dt ) ).textw.data )

				'' stringize parameter?
				case FB_DEFTOK_TYPE_PARAMSTR
					'' don't escape, preserve the sequencies as-is
					DWstrConcatAssign( text, "$" + QUOTE )
					DWstrConcatAssign( text, _
									   *hReplaceW( argtb->tb( symbGetDefTokParamNum( dt ) ).textw.data, _
										 	  	   QUOTE, _
										 	  	   QUOTE + QUOTE ) )
					DWstrConcatAssign( text, QUOTE )

				'' ordinary text..
				case FB_DEFTOK_TYPE_TEX
					DWstrConcatAssignA( text, symbGetDefTokText( dt ) )

				'' unicode text?
				case FB_DEFTOK_TYPE_TEXW
               		DWstrConcatAssign( text, symbGetDefTokTextW( dt ) )
				end select

				'' next
				dt = symbGetDefTokNext( dt )
			loop
		end if

		'' free args text
		do while( num > 0 )
			num -= 1
			DWstrAssign( argtb->tb(num).textw, NULL )
		loop

		listDelNode( @pp.argtblist, argtb )
	end if

	''
	if( doskip ) then
		return NULL
	end if

	if( lex.ctx->deflen = 0 ) then
		DWstrAssign( lex.ctx->deftextw, text.data )
	else
		DWstrAssign( lex.ctx->deftextw, *text.data + *lex.ctx->defptrw )
	end if

	function = len( *text.data )

end function

''::::
private function hLoadDefineW _
	( _
		byval s as FBSYMBOL ptr _
	) as integer

    static as DWSTRING text
    dim as integer lgt = any

    function = FALSE

	'' define has args?
	if( symbGetDefineParams( s ) > 0 ) then

		lgt = hLoadMacroW( s )
		if( lgt = -1 ) then
			exit function
		end if

	'' no args
	else

		'' should we call a function to get definition text?
		if( symbGetDefineCallback( s ) <> NULL ) then
			'' call function
            if( bit( symbGetDefineFlags( s ), 0 ) = 0 ) then
				DWstrAssignA( text, QUOTE + symbGetDefineCallback( s )( ) + QUOTE )
            else
				DWstrAssignA( text, symbGetDefineCallback( s )( ) )
            end if

			if( lex.ctx->deflen = 0 ) then
				DWstrAssign( lex.ctx->deftextw, text.data )
			else
				DWstrAssign( lex.ctx->deftextw, *text.data + *lex.ctx->defptrw )
			end if

            lgt = len( *text.data )

		'' just load text as-is
		else
			'' arg-less macro?
			if( symbGetDefineIsArgless( s ) ) then
				'' '('?
				if( lexCurrentChar( TRUE ) <> CHAR_LPRNT ) then
					'' not an error, macro can be passed as param to other macros
					exit function
				end if
				lexEatChar( )

				'' ')'
				if( lexCurrentChar( TRUE ) <> CHAR_RPRNT ) then
					if( errReport( FB_ERRMSG_EXPECTEDRPRNT ) = FALSE ) then
						exit function
					end if

				else
					lexEatChar( )
				end if
			end if

			if( symbGetType( s ) <> FB_DATATYPE_WCHAR ) then
				if( lex.ctx->deflen = 0 ) then
					DWstrAssignA( lex.ctx->deftextw, symbGetDefineText( s ) )
				else
					DWstrAssign( lex.ctx->deftextw, _
								 wstr( *symbGetDefineText( s ) ) + *lex.ctx->defptrw )
				end if

			else
				if( lex.ctx->deflen = 0 ) then
					DWstrAssign( lex.ctx->deftextw, symbGetDefineTextW( s ) )
				else
					DWstrAssign( lex.ctx->deftextw, _
								 *symbGetDefineTextW( s ) + *lex.ctx->defptrw )
				end if
			end if

            lgt = symbGetLen( s )
		end if

	end if

    ''
	lex.ctx->defptrw = lex.ctx->deftextw.data
	lex.ctx->deflen += lgt

	function = TRUE

end function

''::::
function ppDefineLoad _
	( _
		byval s as FBSYMBOL ptr _
	) as integer

	'' recursion?
	if( s = lex.ctx->currmacro ) then
		if( errReport( FB_ERRMSG_RECURSIVEMACRO ) = FALSE ) then
			return FALSE
		else
			'' error recovery: skip
			hSkipUntil( INVALID, FALSE, LEX_FLAGS )
			return TRUE
		end if
	end if

	'' only one level
	if( lex.ctx->currmacro = NULL ) then
		lex.ctx->currmacro = s
	end if

	if( env.inf.format = FBFILE_FORMAT_ASCII ) then
		function = hLoadDefine( s )
	else
		function = hLoadDefineW( s )
	end if

	'' force a re-read
	lex.ctx->currchar = cuint( INVALID )

end function

'':::::
private function hRtrimMacroText _
	( _
		byval tokhead as FB_DEFTOK ptr, _
		byval toktail as FB_DEFTOK ptr _
	) as FB_DEFTOK ptr

    '' remove the white-spaces
	do while( toktail <> NULL )
		'' not ascii text?
    	if( symbGetDefTokType( toktail ) <> FB_DEFTOK_TYPE_TEX ) then
    		exit do
    	end if

    	select case as const (*symbGetDefTokText( toktail ))[0]
    	'' space or nl?
    	case CHAR_SPACE, CHAR_TAB, CHAR_LF
    		toktail = symbDelDefineTok( toktail )

		case else
        	exit do
    	end select
    loop

    if( toktail = NULL ) then
    	function = NULL
    else
    	function = tokhead
    end if

end function

'':::::
private function hReadMacroText _
	( _
		byval args as integer, _
		byval paramhead as FB_DEFPARAM ptr, _
		byval ismultiline as integer _
	) as FB_DEFTOK ptr

	static as zstring * FB_MAXNAMELEN+1 arg
    dim as FB_DEFPARAM ptr param = any
    dim as FB_DEFTOK ptr toktail = NULL, tokhead = NULL
    dim as integer addquotes = any, nestedcnt = 0

    do
    	addquotes = FALSE

    	select case as const lexGetToken( LEX_FLAGS )
		case FB_TK_EOF
			if( ismultiline ) then
				errReport( FB_ERRMSG_EXPECTEDMACRO )
			end if

			exit do

		case FB_TK_EOL
			if( ismultiline = FALSE ) then
				exit do
			end if

    		'' multi-line, only add if it's not at the beginning
    		if( tokhead <> NULL ) then
				toktail = symbAddDefineTok( toktail, FB_DEFTOK_TYPE_TEX )
    			'' just lf
    			ZstrAssign( @toktail->text, LFCHAR )
    		end if

    		lexSkipToken( LEX_FLAGS )

    		continue do

		case FB_TK_COMMENT, FB_TK_REM
			if( ismultiline = FALSE ) then
				exit do
			end if

			do
				lexSkipToken( LEX_FLAGS )

				select case lexGetToken( LEX_FLAGS )
				case FB_TK_EOL, FB_TK_EOF
					exit do
				end select
			loop

			continue do

		case CHAR_SHARP
    		select case lexGetLookAhead( 1, (LEX_FLAGS or LEXCHECK_KWDNAMESPC) and _
    								 		(not LEXCHECK_NOWHITESPC) )
    		'' '##'?
    		case CHAR_SHARP
    			lexSkipToken( LEX_FLAGS )
    			lexSkipToken( LEX_FLAGS )
    			continue do

    		'' '#' macro?
    		case FB_TK_PP_MACRO
    			if( ismultiline ) then
    				nestedcnt += 1
    			end if

    		'' '#' endmacro?
    		case FB_TK_PP_ENDMACRO
    			if( ismultiline ) then
    				'' not nested?
    				if( nestedcnt = 0 ) then
    					lexSkipToken( LEX_FLAGS )
    					'' no LEX_FLAGS, white-spaces must be skipped
    					lexSkipToken( )

						tokhead = hRtrimMacroText( tokhead, toktail )

    					exit do
    				end if

    				nestedcnt -= 1
    			end if

    		'' '#' id?
    		case FB_TK_ID
    		    '' note: using the PP hashtb here, non-PP keyword will be ID's
    		    lexSkipToken( LEX_FLAGS )
    		    addquotes = TRUE
    		end select

    	'' white space?
    	case CHAR_SPACE, CHAR_TAB

    		'' only add if it's not at the beginning
    		if( tokhead <> NULL ) then
				toktail = symbAddDefineTok( toktail, FB_DEFTOK_TYPE_TEX )
    			'' condensed to a single white-space
    			ZstrAssign( @toktail->text, " " )
    		end if

    		lexSkipToken( LEX_FLAGS )

    		continue do

		end select

    	select case as const lexGetClass( LEX_FLAGS )
    	'' string literal? preserve quotes
    	case FB_TKCLASS_STRLITERAL

    		'' ascii?
    		if( env.inf.format = FBFILE_FORMAT_ASCII ) then
				toktail = symbAddDefineTok( toktail, FB_DEFTOK_TYPE_TEX )
				if( tokhead = NULL ) then
					tokhead = toktail
				end if

	    		ZstrAssign( @toktail->text, lexGetText( ) )

    		'' unicode..
    		else
				toktail = symbAddDefineTok( toktail, FB_DEFTOK_TYPE_TEXW )
				if( tokhead = NULL ) then
					tokhead = toktail
				end if

    			WstrAssign( @toktail->textw, lexGetTextW( ) )
    		end if

    		lexSkipToken( LEX_FLAGS )

        '' identifier? check if it's a parameter (params can be keywords too)
    	case FB_TKCLASS_IDENTIFIER, FB_TKCLASS_KEYWORD, FB_TKCLASS_QUIRKWD
			toktail = symbAddDefineTok( toktail, FB_DEFTOK_TYPE_TEX )
			if( tokhead = NULL ) then
				tokhead = toktail
			end if

    		arg = ucase( *lexGetText( ) )

    		'' look up..
    		param = hashLookup( @symb.def.paramhash, arg )

    		'' found?
    		if( param <> NULL ) then
				if( addquotes = FALSE ) then
					symbSetDefTokType( toktail, FB_DEFTOK_TYPE_PARAM )
				else
					symbSetDefTokType( toktail, FB_DEFTOK_TYPE_PARAMSTR )
				end if

				symbSetDefTokParamNum( toktail, symbGetDefParamNum( param ) )

    		'' none matched, read as-is
    		else
    			'' restore the '#'?
    			if( addquotes ) then
    			    ZstrAssign( @toktail->text, "#" )
    			    ZstrConcatAssign( @toktail->text, lexGetText( ) )
    			else
    				ZstrAssign( @toktail->text, lexGetText( ) )
    			end if
    		end if

    		lexSkipToken( LEX_FLAGS )

    	'' anything else, read as-is
    	case else
			toktail = symbAddDefineTok( toktail, FB_DEFTOK_TYPE_TEX )
			if( tokhead = NULL ) then
				tokhead = toktail
			end if

    		ZstrAssign( @toktail->text, lexGetText( ) )
    		lexSkipToken( LEX_FLAGS )

    	end select

    loop

	function = tokhead

end function

'':::::
private function hReadDefineText _
	( _
		byval sym as FBSYMBOL ptr, _
		byval defname as zstring ptr, _
		byval isargless as integer, _
		byval ismultiline as integer _
	) as integer

	dim as zstring ptr text = any
	dim as wstring ptr textw = any

	function = FALSE

    if( env.inf.format = FBFILE_FORMAT_ASCII ) then
    	'' LITERAL*
    	text = ppReadLiteral( ismultiline )

    	'' already defined? if there are no differences, do nothing..
    	if( sym <> NULL ) then
    		if( (symbGetDefineParams( sym ) > 0) or _
    			(symbGetType( sym ) <> FB_DATATYPE_CHAR) ) then

    			if( errReportEx( FB_ERRMSG_DUPDEFINITION, defname ) = FALSE ) then
    				exit function
    			end if

    		elseif( (*symbGetDefineText( sym ) <> *text) ) then
    			if( errReportEx( FB_ERRMSG_DUPDEFINITION, defname ) = FALSE ) then
    				exit function
    			end if
    		end if

    	else
    		symbAddDefine( defname, text, len( *text ), isargless )

    	end if

    '' unicode..
    else
    	'' LITERAL*
    	textw = ppReadLiteralW( ismultiline )

    	'' already defined? if there are no differences, do nothing..
    	if( sym <> NULL ) then
    		if( (symbGetDefineParams( sym ) > 0) or _
    			(symbGetType( sym ) <> FB_DATATYPE_WCHAR) ) then

    			if( errReportEx( FB_ERRMSG_DUPDEFINITION, defname ) = FALSE ) then
    				exit function
    			end if

    		elseif( (*symbGetDefineTextW( sym ) <> *textw) ) then
    			if( errReportEx( FB_ERRMSG_DUPDEFINITION, defname ) = FALSE ) then
    				exit function
    			end if
    		end if

    	else
    		symbAddDefineW( defname, textw, len( *textw ), isargless )

    	end if

    end if

    function = TRUE

end function

'':::::
'' Define			= 	DEFINE ID (!WHITESPC '(' ID (',' ID)* ')')? LITERAL+
'' 					| 	MACRO ID '(' ID (',' ID)* ')' Comment? EOL
'' 							MacroBody*
'' 						ENDMACRO .
function ppDefine _
	( _
		byval ismultiline as integer _
	) as integer

	static as zstring * FB_MAXNAMELEN+1 defname
	dim as integer params = any, isargless = any, flags = any
	dim as FB_DEFPARAM ptr paramhead = any, lastparam = any
	dim as FBSYMBOL ptr sym = any
	dim as FBSYMCHAIN ptr chain_ = any
	dim as FBSYMBOL ptr base_parent = any
	dim as FB_DEFTOK ptr tokhead = any

	'' note: using the PP hashtb here, so any non-PP keyword won't be found

	function = FALSE

	'' don't allow explicit namespaces
	chain_ = cIdentifier( base_parent, FB_IDOPT_ISDECL or FB_IDOPT_DEFAULT )
	if( errGetLast( ) <> FB_ERRMSG_OK ) then
		exit function
	end if

    flags = LEX_FLAGS
    if( ismultiline ) then
    	flags and= not LEXCHECK_NOWHITESPC
    end if

    lexEatToken( @defname, flags )
    
    if( hIsValidSymbolName( defname ) = FALSE ) then
    	exit function
    end if

    '' contains a period? (with LEX_FLAGS it won't skip white spaces)
    if( lexGetToken( flags ) = CHAR_DOT ) then
    	if( errReport( FB_ERRMSG_CANTINCLUDEPERIODS ) = FALSE ) then
    		exit function
    	end if
    end if

    if( chain_ <> NULL ) then
    	sym = chain_->sym
    	if( symbIsDefine( sym ) = FALSE ) then
    		'' defines have no dups or respect namespaces
    		if( errReportEx( FB_ERRMSG_DUPDEFINITION, @defname ) = FALSE ) then
    			exit function
    		else
    			'' error recovery: fake an id
    			defname = *hMakeTmpStr( )
    		end if
    	end if

    else
		if( errGetLast( ) <> FB_ERRMSG_OK ) then
			exit function
		end if

    	sym = NULL
    end if

    params = 0
    paramhead = NULL
    isargless = FALSE

    '' '('?
    if( lexGetToken( flags ) = CHAR_LPRNT ) then
    	lexSkipToken( LEXCHECK_NODEFINE or LEXCHECK_NOSYMBOL )

		'' not arg-less?
		if( lexGetToken( LEXCHECK_NODEFINE or LEXCHECK_NOSYMBOL ) <> CHAR_RPRNT ) then
			lastparam = NULL
			do
		    	select case as const lexGetClass( )
		    	case FB_TKCLASS_IDENTIFIER, FB_TKCLASS_KEYWORD, FB_TKCLASS_QUIRKWD
                	lastparam = symbAddDefineParam( lastparam, lexGetText( ) )

		    	case else
    				if( errReport( FB_ERRMSG_EXPECTEDIDENTIFIER ) = FALSE ) then
    					exit function
    				else
    					'' error recovery: fake a param
    					lastparam = symbAddDefineParam( lastparam, hMakeTmpStr( ) )
    				end if
		    	end select

		    	if( lastparam = NULL ) then
    				if( errReport( FB_ERRMSG_DUPDEFINITION ) = FALSE ) then
    					exit function
    				end if
		    	end if

		    	lexSkipToken( LEXCHECK_NODEFINE or LEXCHECK_NOSYMBOL )

		    	params += 1
				if( params >= FB_MAXDEFINEARGS ) then
					if( errReport( FB_ERRMSG_TOOMANYPARAMS ) = FALSE ) then
						exit function
					else
						'' error recovery: skip until next ')'
						hSkipUntil( CHAR_RPRNT, TRUE )
						return TRUE
					end if
				end if

		    	if( paramhead = NULL ) then
		    		paramhead = lastparam
		    	end if

				'' ','?
				if( lexGetToken( LEXCHECK_NODEFINE or LEXCHECK_NOSYMBOL ) <> CHAR_COMMA ) then
					exit do
				end if

		    	lexSkipToken( LEXCHECK_NODEFINE or LEXCHECK_NOSYMBOL )
			loop

		else
			isargless = TRUE
		end if

    	'' ')'
    	if( lexGetToken( LEX_FLAGS ) <> CHAR_RPRNT ) then
    		if( errReport( FB_ERRMSG_EXPECTEDRPRNT ) = FALSE ) then
    			exit function
			else
				'' error recovery: skip until next ')'
				hSkipUntil( CHAR_RPRNT, TRUE, LEX_FLAGS )
			end if

    	else
    		lexSkipToken( LEX_FLAGS and (not LEXCHECK_NOWHITESPC) )
    	end if

    else
    	if( ismultiline ) then
    		if( errReport( FB_ERRMSG_EXPECTEDLPRNT ) = FALSE ) then
    			exit function
			end if

    	else
    		if( lexGetToken( LEX_FLAGS ) = CHAR_SPACE ) then
    			'' skip white-spaces
    			lexSkipToken( LEX_FLAGS and not LEXCHECK_NOWHITESPC )
    		end if
    	end if
    end if

   	'' not a macro?
   	if( params = 0 ) then
    	return hReadDefineText( sym, @defname, isargless, ismultiline )
    end if

    '' macro..

   	'' already defined? can't check..
   	if( sym <> NULL ) then
   		if( errReportEx( FB_ERRMSG_DUPDEFINITION, defname ) = FALSE ) then
   			exit function
   		end if

   	else
   		tokhead = hReadMacroText( params, paramhead, ismultiline )
   		symbAddDefineMacro( @defname, tokhead, params, paramhead )

   	end if

   	function = TRUE

end function


