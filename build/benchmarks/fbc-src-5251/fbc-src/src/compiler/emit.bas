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


'' emit abstract interface
''
'' chng: jun/2005 written [v1ctor]
''

#include once "inc\fb.bi"
#include once "inc\fbint.bi"
#include once "inc\reg.bi"
#include once "inc\ir.bi"
#include once "inc\rtl.bi"
#include once "inc\emit.bi"
#include once "inc\symb.bi"

declare function emitGasX86_ctor	_
	( _
	) as integer

'' globals
	dim shared emit as EMITCTX

'':::::
#macro hCallCtor( backend )
	select case backend
	case FB_BACKEND_GAS
		emitGasX86_ctor( )
	end select
#endmacro

'':::::
function emitInit _
	( _
		byval backend as FB_BACKEND _
	) as integer

	if( emit.inited ) then
		return TRUE
	end if

	''
	hCallCtor( backend )

	''
	flistNew( @emit.nodeTB, EMIT_INITNODES, len( EMIT_NODE ) )

	flistNew( @emit.vregTB, EMIT_INITVREGNODES, len( IRVREG ) )

	''
	emit.inited = TRUE
	emit.pos = 0

	''
	function = emit.vtbl.init( )

end function

'':::::
sub emitEnd static

	if( emit.inited = FALSE ) then
		exit sub
	end if

	emit.vtbl.end( )

	emit.inited = FALSE

end sub

'':::::
sub emitWriteStr _
	( _
		byval s as zstring ptr, _
		byval addtab as integer _
	)

    static as string ostr

	if( addtab ) then
		ostr = TABCHAR
		ostr += *s
	else
		ostr = *s
	end if

	ostr += NEWLINE

	if( put( #env.outf.num, , ostr ) <> 0 ) then
		'' ...
	end if

end sub

'':::::
sub emitReset( ) static
	dim as integer c

	flistReset( @emit.nodeTB )
	flistReset( @emit.vregTB )

	emit.curnode = NULL

	'' reset reg usage
	for c = 0 to EMIT_REGCLASSES-1
		EMIT_REGCLEARALL( c )
	next

end sub

'':::::
private function hOptSYMOP _
	( _
		byval p as EMIT_NODE ptr, _
		byval n as EMIT_NODE ptr _
	) as EMIT_NODE ptr

	select case n->sop.op
	case EMIT_OP_LABEL
		if( p <> NULL ) then
			'' convert "jxx foo \n foo:" to "foo:"
			if( p->class = EMIT_NODECLASS_BRC ) then
				select case p->brc.op
				case EMIT_OP_BRANCH, EMIT_OP_JUMP
					if( p->brc.sym = n->sop.sym ) then
						p->class = EMIT_NODECLASS_NOP
					end if
				end select
			end if
		end if

		'' don't count the label so "jmp foo \n bar: \n foo:" could be handled
		return p
	end select

	function = n

end function

'':::::
private sub hPeepHoleOpt( )
    dim as EMIT_NODE ptr n = any, p = any

	p = NULL
	n = flistGetHead( @emit.nodeTB )
	do while( n <> NULL )

		select case as const n->class
		case EMIT_NODECLASS_SOP
			p = hOptSYMOP( p, n )

		case EMIT_NODECLASS_DBG
			'' don't count debugging nodes, they won't gen any code

		case EMIT_NODECLASS_LIT
			'' don't count literal text, unless it's inline asm
			if( n->lit.isasm ) then
				p = n
			end if

		case else
			p = n
		end select

		n = flistGetNext( n )
	loop

end sub

'':::::
sub emitFlush( )
    dim as EMIT_NODE ptr n = any

    hPeepHoleOpt( )

	n = flistGetHead( @emit.nodeTB )
	do while( n <> NULL )

		emit.curnode = n

		select case as const n->class
		case EMIT_NODECLASS_NOP

		case EMIT_NODECLASS_BOP
			cast( EMIT_BOPCB, emit.opFnTb[n->bop.op] )( n->bop.dvreg, _
												   		n->bop.svreg )

		case EMIT_NODECLASS_UOP
			cast( EMIT_UOPCB, emit.opFnTb[n->uop.op ] )( n->uop.dvreg )

		case EMIT_NODECLASS_REL
			cast( EMIT_RELCB, emit.opFnTb[n->rel.op] )( n->rel.rvreg, _
												   		n->rel.label, _
												   		n->rel.dvreg, _
												   		n->rel.svreg )

		case EMIT_NODECLASS_STK
			cast( EMIT_STKCB, emit.opFnTb[n->stk.op] )( n->stk.vreg, _
												   n->stk.extra )

		case EMIT_NODECLASS_BRC
			cast( EMIT_BRCCB, emit.opFnTb[n->brc.op] )( n->brc.vreg, _
												   		n->brc.sym, _
												   		n->brc.extra )

		case EMIT_NODECLASS_SOP
			cast( EMIT_SOPCB, emit.opFnTb[n->sop.op] )( n->sop.sym )

		case EMIT_NODECLASS_LIT
			cast( EMIT_LITCB, emit.opFnTb[EMIT_OP_LIT] )( n->lit.text )

			ZstrFree( n->lit.text )

		case EMIT_NODECLASS_JTB
			cast( EMIT_JTBCB, emit.opFnTb[EMIT_OP_JMPTB] )( n->jtb.dtype, _
													   		n->jtb.text )

			ZstrFree( n->jtb.text )

		case EMIT_NODECLASS_MEM
			cast( EMIT_MEMCB, emit.opFnTb[n->mem.op] )( n->mem.dvreg, _
												   		n->mem.svreg, _
												   		n->mem.bytes, _
												   		n->mem.extra )

		case EMIT_NODECLASS_DBG
			cast( EMIT_DBGCB, emit.opFnTb[n->dbg.op] )( n->dbg.sym, _
												   		n->dbg.lnum, _
												   		n->dbg.pos )

		end select

		n = flistGetNext( n )
	loop

end sub

'':::::
function emitGetRegClass _
	( _
		byval dclass as integer _
	) as REGCLASS ptr

	function = emit.regTB(dclass)

end function

'':::::
sub emitWriteInfoSection _
	( _
		byval liblist as TLIST ptr, _
		byval libpathlist as TLIST ptr _
	)

	static as string byte_fld
	static as string zstr_fld
	dim as integer header_emitted = FALSE

	'' this must follow the fbObjInfoWriteObj() format

#macro hWriteStr( value )
	emitWriteStr( zstr_fld & QUOTE & value & $"\0" & QUOTE )
#endmacro

#macro hWriteByte( value )
	emitWriteStr( byte_fld & value )
#endmacro

#macro hEmitInfoHeader( )
	scope
		dim as zstring ptr sec = emit.vtbl.getSectionString( IR_SECTION_INFO, 0 )
		if( sec <> NULL ) then
			emitWriteStr( *sec )
		end if

		if( header_emitted = FALSE ) then
			header_emitted = TRUE
			hWriteByte( FB_INFOSEC_VERSION )
		end if
	end scope
#endmacro

	if( len( byte_fld ) = 0 ) then
		byte_fld = *emit.vtbl.getTypeString( FB_DATATYPE_BYTE ) + " "
		zstr_fld = *emit.vtbl.getTypeString( FB_DATATYPE_CHAR ) + " "
	end if

	'' libraries
	dim as FBS_LIB ptr nlib = listGetHead( liblist )
	if( nlib <> NULL ) then
		hEmitInfoHeader( )

        hWriteByte( FB_INFOSEC_LIB )
		do
            '' never add a default one
            if( nlib->isdefault = FALSE ) then
            	hWriteByte( len( *nlib->name ) )
            	hWriteStr( *nlib->name )
            end if

			nlib = listGetNext( nlib )
		loop while( nlib <> NULL )

		hWriteByte( 0 )
	end if

	'' paths
	dim as FBS_LIB ptr npath = listGetHead( libpathlist )
	if( npath <> NULL ) then
		hEmitInfoHeader( )

        hWriteByte( FB_INFOSEC_PTH )
		do
            '' never add a default one
            if( npath->isdefault = FALSE ) then
            	dim as zstring ptr txt = hEscape( *npath->name )
            	hWriteByte( len( *npath->name ) )
            	hWriteStr( *txt )
            end if

			npath = listGetNext( npath )
		loop while( npath <> NULL )

		hWriteByte( 0 )
	end if

	'' any important cmd-line option? (like -lang, -mt)
	if( env.clopt.multithreaded or (env.clopt.lang <> FB_LANG_FB) ) then
		hEmitInfoHeader( )

        hWriteByte( FB_INFOSEC_CMD )

    	'' not the default lang?
    	if( env.clopt.lang <> FB_LANG_FB ) then
            hWriteByte( len( "-lang" ) )
            hWriteStr( "-lang" )
            hWriteStr( fbGetLangName( env.clopt.lang ) )
    	end if

    	'' MT?
    	if( env.clopt.multithreaded ) then
            hWriteByte( len( "-mt" ) )
            hWriteStr( "-mt" )
		end if

		hWriteByte( 0 )
	end if

	if( header_emitted ) then
		hWriteByte( FB_INFOSEC_EOL )
	end if

end sub

''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
'' node creation
''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

'':::::
private function hNewVR _
	( _
		byval v as IRVREG ptr _
	) as IRVREG ptr

    dim as IRVREG ptr n = any
    dim as integer dclass = any

	if( v = NULL ) then
		return NULL
	end if

	n = flistNewItem( @emit.vregTB )

	n->typ = v->typ
	astGetFullType( n ) = v->dtype
	n->sym = v->sym
	n->ofs = v->ofs
	n->mult = v->mult
	n->value = v->value

	n->regFamily = v->regFamily
	n->vector = v->vector

	if( v->typ = IR_VREGTYPE_REG ) then
		dclass = symbGetDataClass( v->dtype )
		n->reg = emit.regTB(dclass)->getRealReg( emit.regTB(dclass), v->reg )
		assert( n->reg <> INVALID )
		EMIT_REGSETUSED( dclass, n->reg )
	end if

	'' recursive
	n->vaux = hNewVR( v->vaux )
	n->vidx = hNewVR( v->vidx )

	function = n

end function

'':::::
private function hNewNode _
	( _
		byval class_ as EMIT_NODECLASS_ENUM, _
		byval updatepos as integer = TRUE _
	) as EMIT_NODE ptr static

	dim as EMIT_NODE ptr n
	dim as integer i

	n = flistNewItem( @emit.nodeTB )

	n->class = class_

	'' save register's state
	for i = 0 to EMIT_REGCLASSES-1
		n->regFreeTB(i) = emit.regTB(i)->regctx.freeTB
	next

	if( updatepos ) then
		emit.pos += 1
	end if

	function = n

end function

'':::::
private function hNewBOP _
	( _
		byval op as integer, _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	dim as EMIT_NODE ptr n

	n = hNewNode( EMIT_NODECLASS_BOP )

	n->bop.op = op
	n->bop.dvreg = hNewVR( dvreg )
	n->bop.svreg = hNewVR( svreg )

	function = n

end function

'':::::
private function hNewUOP _
	( _
		byval op as integer, _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	dim as EMIT_NODE ptr n

	n = hNewNode( EMIT_NODECLASS_UOP )

	n->uop.op = op
	n->uop.dvreg = hNewVR( dvreg )

	function = n

end function

'':::::
private function hNewREL _
	( _
		byval op as integer, _
		byval rvreg as IRVREG ptr, _
		byval label as FBSYMBOL ptr, _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	dim as EMIT_NODE ptr n

	n = hNewNode( EMIT_NODECLASS_REL )

	n->rel.op = op
	n->rel.rvreg = hNewVR( rvreg )
	n->rel.label = label
	n->rel.dvreg = hNewVR( dvreg )
	n->rel.svreg = hNewVR( svreg )

	function = n

end function

'':::::
private function hNewSTK _
	( _
		byval op as integer, _
		byval vreg as IRVREG ptr, _
		byval extra as integer = 0 _
	) as EMIT_NODE ptr static

	dim as EMIT_NODE ptr n

	n = hNewNode( EMIT_NODECLASS_STK )

	n->stk.op = op
	n->stk.vreg = hNewVR( vreg )
	n->stk.extra = extra

	function = n

end function

'':::::
private function hNewBRANCH _
	( _
		byval op as integer, _
		byval vreg as IRVREG ptr, _
		byval sym as FBSYMBOL ptr, _
		byval extra as integer = 0 _
	) as EMIT_NODE ptr static

	dim as EMIT_NODE ptr n

	n = hNewNode( EMIT_NODECLASS_BRC )

	n->brc.op = op
	n->brc.sym = sym
	n->brc.vreg = hNewVR( vreg )
	n->brc.extra = extra

	function = n

end function

'':::::
private function hNewSYMOP _
	( _
		byval op as integer, _
		byval sym as FBSYMBOL ptr _
	) as EMIT_NODE ptr static

	dim as EMIT_NODE ptr n

	n = hNewNode( EMIT_NODECLASS_SOP, FALSE )

	n->sop.op = op
	n->sop.sym = sym

	function = n

end function

'':::::
private function hNewLIT _
	( _
		byval text as zstring ptr, _
		byval isasm as integer _
	) as EMIT_NODE ptr static

	dim as EMIT_NODE ptr n

	n = hNewNode( EMIT_NODECLASS_LIT, isasm )

	n->lit.isasm = isasm
	n->lit.text = ZstrAllocate( len( *text ) )
	*n->lit.text = *text

	function = n

end function

'':::::
private function hNewJMPTB _
	( _
		byval dtype as integer, _
		byval text as zstring ptr _
	) as EMIT_NODE ptr static

	dim as EMIT_NODE ptr n

	n = hNewNode( EMIT_NODECLASS_JTB, FALSE )

	n->jtb.dtype = dtype
	n->jtb.text = ZstrAllocate( len( *text ) )
	*n->jtb.text = *text

	function = n

end function

'':::::
private function hNewMEM _
	( _
		byval op as integer, _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr, _
		byval bytes as integer, _
		byval extra as integer = 0 _
	) as EMIT_NODE ptr static

	dim as EMIT_NODE ptr n

	n = hNewNode( EMIT_NODECLASS_MEM )

	n->mem.op = op
	n->mem.dvreg = hNewVR( dvreg )
	n->mem.svreg = hNewVR( svreg )
	n->mem.bytes = bytes
	n->mem.extra = extra

	function = n

end function

'':::::
private function hNewDBG _
	( _
		byval op as integer, _
		byval sym as FBSYMBOL ptr, _
		byval lnum as integer = 0, _
		byval pos_ as integer = 0 _
	) as EMIT_NODE ptr static

	dim as EMIT_NODE ptr n

	n = hNewNode( EMIT_NODECLASS_DBG, FALSE )

	n->dbg.op = op
	n->dbg.sym = sym
	n->dbg.lnum = lnum
	n->dbg.pos = pos_

	function = n

end function

''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
'' load & store
''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

'':::::
function emitLOAD _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT

		select case as const svreg->dtype
		'' longint?
		case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
			function = hNewBOP( EMIT_OP_LOADL2L, dvreg, svreg )

		'' float?
		case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
			function = hNewBOP( EMIT_OP_LOADF2L, dvreg, svreg )

		case else
			function = hNewBOP( EMIT_OP_LOADI2L, dvreg, svreg )
		end select

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE

		select case as const svreg->dtype
		'' longint?
		case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
			function = hNewBOP( EMIT_OP_LOADL2F, dvreg, svreg )

		'' float?
		case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
			function = hNewBOP( EMIT_OP_LOADF2F, dvreg, svreg )

		case else
			function = hNewBOP( EMIT_OP_LOADI2F, dvreg, svreg )
		end select

	case else

		select case as const svreg->dtype
		'' longint?
		case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
			function = hNewBOP( EMIT_OP_LOADL2I, dvreg, svreg )

		'' float?
		case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
			function = hNewBOP( EMIT_OP_LOADF2I, dvreg, svreg )

		case else
			function = hNewBOP( EMIT_OP_LOADI2I, dvreg, svreg )
		end select

	end select

end function

'':::::
function emitSTORE _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT

		select case as const svreg->dtype
		'' longint?
		case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
			function = hNewBOP( EMIT_OP_STORL2L, dvreg, svreg )

		'' float?
		case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
			function = hNewBOP( EMIT_OP_STORF2L, dvreg, svreg )

		case else
			function = hNewBOP( EMIT_OP_STORI2L, dvreg, svreg )
		end select

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE

		select case as const svreg->dtype
		'' longint?
		case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
			function = hNewBOP( EMIT_OP_STORL2F, dvreg, svreg )

		'' float?
		case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
			function = hNewBOP( EMIT_OP_STORF2F, dvreg, svreg )

		case else
			function = hNewBOP( EMIT_OP_STORI2F, dvreg, svreg )
		end select

	case else

		select case as const svreg->dtype
		'' longint?
		case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
			function = hNewBOP( EMIT_OP_STORL2I, dvreg, svreg )

		'' float?
		case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
			function = hNewBOP( EMIT_OP_STORF2I, dvreg, svreg )

		case else
			function = hNewBOP( EMIT_OP_STORI2I, dvreg, svreg )
		end select

	end select

end function

''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
'' BOP
''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

'':::::
function emitMOV _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewBOP( EMIT_OP_MOVL, dvreg, svreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewBOP( EMIT_OP_MOVF, dvreg, svreg )

	case else
		function = hNewBOP( EMIT_OP_MOVI, dvreg, svreg )
	end select

end function

'':::::
function emitADD _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewBOP( EMIT_OP_ADDL, dvreg, svreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewBOP( EMIT_OP_ADDF, dvreg, svreg )

	case else
		function = hNewBOP( EMIT_OP_ADDI, dvreg, svreg )
	end select

end function

'':::::
function emitSUB _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewBOP( EMIT_OP_SUBL, dvreg, svreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewBOP( EMIT_OP_SUBF, dvreg, svreg )

	case else
		function = hNewBOP( EMIT_OP_SUBI, dvreg, svreg )
	end select

end function

'':::::
function emitMUL _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewBOP( EMIT_OP_MULL, dvreg, svreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewBOP( EMIT_OP_MULF, dvreg, svreg )

	case else
		if( symbIsSigned( dvreg->dtype ) ) then
			function = hNewBOP( EMIT_OP_SMULI, dvreg, svreg )
		else
			function = hNewBOP( EMIT_OP_MULI, dvreg, svreg )
		end if
	end select

end function

'':::::
function emitDIV _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewBOP( EMIT_OP_DIVF, dvreg, svreg )

end function

'':::::
function emitINTDIV _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewBOP( EMIT_OP_DIVI, dvreg, svreg )

end function

'':::::
function emitMOD _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewBOP( EMIT_OP_MODI, dvreg, svreg )

end function

'':::::
function emitSHL _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewBOP( EMIT_OP_SHLL, dvreg, svreg )

	case else
		function = hNewBOP( EMIT_OP_SHLI, dvreg, svreg )
	end select

end function

'':::::
function emitSHR _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewBOP( EMIT_OP_SHRL, dvreg, svreg )

	case else
		function = hNewBOP( EMIT_OP_SHRI, dvreg, svreg )
	end select

end function

'':::::
function emitAND _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewBOP( EMIT_OP_ANDL, dvreg, svreg )

	case else
		function = hNewBOP( EMIT_OP_ANDI, dvreg, svreg )
	end select

end function

'':::::
function emitOR _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewBOP( EMIT_OP_ORL, dvreg, svreg )

	case else
		function = hNewBOP( EMIT_OP_ORI, dvreg, svreg )
	end select

end function

'':::::
function emitXOR _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewBOP( EMIT_OP_XORL, dvreg, svreg )

	case else
		function = hNewBOP( EMIT_OP_XORI, dvreg, svreg )
	end select

end function

'':::::
function emitEQV _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewBOP( EMIT_OP_EQVL, dvreg, svreg )

	case else
		function = hNewBOP( EMIT_OP_EQVI, dvreg, svreg )
	end select

end function

'':::::
function emitIMP _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewBOP( EMIT_OP_IMPL, dvreg, svreg )

	case else
		function = hNewBOP( EMIT_OP_IMPI, dvreg, svreg )
	end select

end function

'':::::
function emitATN2 _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewBOP( EMIT_OP_ATN2, dvreg, svreg )

end function

'':::::
function emitPOW _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewBOP( EMIT_OP_POW, dvreg, svreg )

end function

'':::::
function emitADDROF _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewBOP( EMIT_OP_ADDROF, dvreg, svreg )

end function

'':::::
function emitDEREF _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewBOP( EMIT_OP_DEREF, dvreg, svreg )

end function

''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
'' REL
''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

'':::::
function emitGT _
	( _
		byval rvreg as IRVREG ptr, _
		byval label as FBSYMBOL ptr, _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewREL( EMIT_OP_CGTL, rvreg, label, dvreg, svreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewREL( EMIT_OP_CGTF, rvreg, label, dvreg, svreg )

	case else
		function = hNewREL( EMIT_OP_CGTI, rvreg, label, dvreg, svreg )
	end select

end function

'':::::
function emitLT _
	( _
		byval rvreg as IRVREG ptr, _
		byval label as FBSYMBOL ptr, _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewREL( EMIT_OP_CLTL, rvreg, label, dvreg, svreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewREL( EMIT_OP_CLTF, rvreg, label, dvreg, svreg )

	case else
		function = hNewREL( EMIT_OP_CLTI, rvreg, label, dvreg, svreg )
	end select

end function

'':::::
function emitEQ _
	( _
		byval rvreg as IRVREG ptr, _
		byval label as FBSYMBOL ptr, _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewREL( EMIT_OP_CEQL, rvreg, label, dvreg, svreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewREL( EMIT_OP_CEQF, rvreg, label, dvreg, svreg )

	case else
		function = hNewREL( EMIT_OP_CEQI, rvreg, label, dvreg, svreg )
	end select

end function

'':::::
function emitNE _
	( _
		byval rvreg as IRVREG ptr, _
		byval label as FBSYMBOL ptr, _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewREL( EMIT_OP_CNEL, rvreg, label, dvreg, svreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewREL( EMIT_OP_CNEF, rvreg, label, dvreg, svreg )

	case else
		function = hNewREL( EMIT_OP_CNEI, rvreg, label, dvreg, svreg )
	end select

end function

'':::::
function emitGE _
	( _
		byval rvreg as IRVREG ptr, _
		byval label as FBSYMBOL ptr, _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewREL( EMIT_OP_CGEL, rvreg, label, dvreg, svreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewREL( EMIT_OP_CGEF, rvreg, label, dvreg, svreg )

	case else
		function = hNewREL( EMIT_OP_CGEI, rvreg, label, dvreg, svreg )
	end select

end function

'':::::
function emitLE _
	( _
		byval rvreg as IRVREG ptr, _
		byval label as FBSYMBOL ptr, _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewREL( EMIT_OP_CLEL, rvreg, label, dvreg, svreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewREL( EMIT_OP_CLEF, rvreg, label, dvreg, svreg )

	case else
		function = hNewREL( EMIT_OP_CLEI, rvreg, label, dvreg, svreg )
	end select

end function

''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
'' UOP
''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

'':::::
function emitNEG _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewUOP( EMIT_OP_NEGL, dvreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewUOP( EMIT_OP_NEGF, dvreg )

	case else
		function = hNewUOP( EMIT_OP_NEGI, dvreg )
	end select

end function

'':::::
function emitNOT _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewUOP( EMIT_OP_NOTL, dvreg )

	case else
		function = hNewUOP( EMIT_OP_NOTI, dvreg )
	end select

end function

'':::::
function emitHADD _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewUOP( EMIT_OP_HADDF, dvreg )

	case else
		'function = hNewUOP( EMIT_OP_HADDI, dvreg )
	end select

end function

'':::::
function emitABS _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewUOP( EMIT_OP_ABSL, dvreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewUOP( EMIT_OP_ABSF, dvreg )

	case else
		function = hNewUOP( EMIT_OP_ABSI, dvreg )
	end select

end function

'':::::
function emitSGN _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewUOP( EMIT_OP_SGNL, dvreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewUOP( EMIT_OP_SGNF, dvreg )

	case else
		function = hNewUOP( EMIT_OP_SGNI, dvreg )
	end select

end function

'':::::
function emitFIX _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_FIX, dvreg )

end function

'':::::
function emitFRAC _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_FRAC, dvreg )

end function

'':::::
function emitSIN _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_SIN, dvreg )

end function

'':::::
function emitASIN _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_ASIN, dvreg )

end function

'':::::
function emitCOS _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_COS, dvreg )

end function

'':::::
function emitACOS _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_ACOS, dvreg )

end function

'':::::
function emitTAN _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_TAN, dvreg )

end function

'':::::
function emitATAN _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_ATAN, dvreg )

end function

'':::::
function emitSQRT _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_SQRT, dvreg )

end function

'':::::
function emitRSQRT _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_RSQRT, dvreg )

end function

'':::::
function emitRCP _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_RCP, dvreg )

end function

'':::::
function emitLOG _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_LOG, dvreg )

end function

'':::::
function emitEXP _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_EXP, dvreg )

end function

'':::::
function emitFLOOR _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_FLOOR, dvreg )

end function

'':::::
function emitXchgTOS _
	( _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_XCHGTOS, svreg )

end function

'':::::
function emitSWZREP _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewUOP( EMIT_OP_SWZREP, dvreg )

end function

''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
'' STK
''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

'':::::
function emitPUSH _
	( _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const svreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewSTK( EMIT_OP_PUSHL, svreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewSTK( EMIT_OP_PUSHF, svreg )

	case else
		function = hNewSTK( EMIT_OP_PUSHI, svreg )
	end select

end function

'':::::
function emitPOP _
	( _
		byval dvreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	select case as const dvreg->dtype
	'' longint?
	case FB_DATATYPE_LONGINT, FB_DATATYPE_ULONGINT
		function = hNewSTK( EMIT_OP_POPL, dvreg )

	'' float?
	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
		function = hNewSTK( EMIT_OP_POPF, dvreg )

	case else
		function = hNewSTK( EMIT_OP_POPI, dvreg )
	end select

end function

'':::::
function emitPUSHUDT _
	( _
		byval svreg as IRVREG ptr, _
		byval sdsize as integer _
	) as EMIT_NODE ptr static

	function = hNewSTK( EMIT_OP_PUSHUDT, svreg, sdsize )

end function

''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
'' MISC
''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

'':::::
function emitCOMMENT _
	( _
		byval text as zstring ptr _
	) as EMIT_NODE ptr static

	function = hNewLIT( "##" + *text, FALSE )

end function

'':::::
function emitASM _
	( _
		byval text as zstring ptr _
	) as EMIT_NODE ptr static
    dim as integer c

    function = hNewLIT( text, TRUE )

	'' reset reg usage
	for c = 0 to EMIT_REGCLASSES-1
		EMIT_REGTRASHALL( c )						'' can't check the reg usage
	next

end function

'':::::
function emitLIT _
	( _
		byval text as zstring ptr _
	) as EMIT_NODE ptr static

	function = hNewLIT( text, FALSE )

end function

'':::::
function emitALIGN _
	( _
		byval bytes as integer _
	) as EMIT_NODE ptr static

    static as IRVREG vr

	vr.typ = IR_VREGTYPE_IMM
	vr.value.int = bytes

	function = hNewUOP( EMIT_OP_ALIGN, @vr )

end function

'':::::
function emitSTACKALIGN _
	( _
		byval bytes as integer _
	) as EMIT_NODE ptr static
    static as IRVREG vr

	vr.typ = IR_VREGTYPE_IMM
	vr.value.int = bytes

	function = hNewUOP( EMIT_OP_STKALIGN, @vr )

end function

'':::::
function emitJMPTB _
	( _
		byval dtype as integer, _
		byval text as zstring ptr _
	) as EMIT_NODE ptr static

	function = hNewJMPTB( dtype, text )

end function

'':::::
function emitCALL _
	( _
		byval label as FBSYMBOL ptr, _
		byval bytestopop as integer _
	) as EMIT_NODE ptr static

	function = hNewBRANCH( EMIT_OP_CALL, NULL, label, bytestopop )

end function

'':::::
function emitCALLPTR _
	( _
		byval svreg as IRVREG ptr, _
		byval bytestopop as integer _
	) as EMIT_NODE ptr static

	function = hNewBRANCH( EMIT_OP_CALLPTR, svreg, NULL, bytestopop )

end function

'':::::
function emitBRANCH _
	( _
		byval op as integer, _
		byval label as FBSYMBOL ptr _
	) as EMIT_NODE ptr static

	function = hNewBRANCH( EMIT_OP_BRANCH, NULL, label, op )

end function

'':::::
function emitJUMP _
	( _
		byval label as FBSYMBOL ptr _
	) as EMIT_NODE ptr static

	function = hNewBRANCH( EMIT_OP_JUMP, NULL, label )

end function

'':::::
function emitJUMPPTR _
	( _
		byval svreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewBRANCH( EMIT_OP_JUMPPTR, svreg, NULL )

end function

'':::::
function emitRET _
	( _
		byval bytestopop as integer _
	) as EMIT_NODE ptr static
    static as IRVREG vr

	vr.typ = IR_VREGTYPE_IMM
	vr.value.int = bytestopop

	function = hNewUOP( EMIT_OP_RET, @vr )

end function

'':::::
function emitLABEL _
	( _
		byval label as FBSYMBOL ptr _
	) as EMIT_NODE ptr static

	function = hNewSYMOP( EMIT_OP_LABEL, label )

end function

'':::::
function emitPUBLIC _
	( _
		byval label as FBSYMBOL ptr _
	) as EMIT_NODE ptr static

	function = hNewSYMOP( EMIT_OP_PUBLIC, label )

end function


''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
'' MEM
''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

'':::::
function emitMEMMOVE _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr, _
		byval bytes as integer _
	) as EMIT_NODE ptr static

	function = hNewMEM( EMIT_OP_MEMMOVE, dvreg, svreg, bytes )

end function

'':::::
function emitMEMSWAP _
	( _
		byval dvreg as IRVREG ptr, _
		byval svreg as IRVREG ptr, _
		byval bytes as integer _
	) as EMIT_NODE ptr static

	function = hNewMEM( EMIT_OP_MEMSWAP, dvreg, svreg, bytes )

end function

'':::::
function emitMEMCLEAR _
	( _
		byval dvreg as IRVREG ptr, _
		byval bytes_vreg as IRVREG ptr _
	) as EMIT_NODE ptr static

	function = hNewMEM( EMIT_OP_MEMCLEAR, dvreg, bytes_vreg, 0 )

end function

'':::::
function emitSTKCLEAR _
	( _
		byval bytes as integer, _
		byval baseofs as integer _
	) as EMIT_NODE ptr static

	function = hNewMEM( EMIT_OP_STKCLEAR, NULL, NULL, bytes, baseofs )

end function

''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
'' DBG
''::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

'':::::
function emitDBGLineBegin _
	( _
		byval proc as FBSYMBOL ptr, _
		byval lnum as integer _
	) as EMIT_NODE ptr

	function = hNewDBG( EMIT_OP_LINEINI, proc, lnum, emit.pos )

end function

'':::::
function emitDBGLineEnd _
	( _
		byval proc as FBSYMBOL ptr, _
		byval lnum as integer _
	) as EMIT_NODE ptr

	function = hNewDBG( EMIT_OP_LINEEND, proc, lnum, emit.pos )

end function

'':::::
function emitDBGScopeBegin _
	( _
		byval sym as FBSYMBOL ptr _
	) as EMIT_NODE ptr

	function = hNewDBG( EMIT_OP_SCOPEINI, sym )

end function

'':::::
function emitDBGScopeEnd _
	( _
		byval sym as FBSYMBOL ptr _
	) as EMIT_NODE ptr

	function = hNewDBG( EMIT_OP_SCOPEEND, sym )

end function


