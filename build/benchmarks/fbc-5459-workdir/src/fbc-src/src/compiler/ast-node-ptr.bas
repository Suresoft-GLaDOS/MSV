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

'' AST deref pointer nodes
'' l = pointer expression; r = NULL
''
'' chng: sep/2004 written [v1ctor]


#include once "inc\fb.bi"
#include once "inc\fbint.bi"
#include once "inc\ir.bi"
#include once "inc\ast.bi"

'':::::
function astNewDEREF _
	( _
		byval l as ASTNODE ptr, _
		byval dtype as integer, _
		byval subtype as FBSYMBOL ptr, _
		byval ofs as integer _
	) as ASTNODE ptr

    dim as ASTNODE ptr n = any

	if( l <> NULL ) then
    	if( dtype = FB_DATATYPE_INVALID ) then
    		dtype = typeDeref( astGetFullType( l ) )
    		subtype = astGetSubType( l )
    	end if

		if( ofs = 0 ) then
			'' skip any casting if they won't do any conversion
			dim as ASTNODE ptr t = l
			if( l->class = AST_NODECLASS_CONV ) then
				if( l->cast.doconv = FALSE ) then
					t = l->l
				end if
			end if

			'' convert *@ to nothing
    		dim as integer delchild = any
			select case t->class
			case AST_NODECLASS_ADDROF
				delchild = TRUE

			case AST_NODECLASS_OFFSET
				delchild = (t->ofs.ofs = 0)

			case else
				delchild = FALSE
			end select

			''
			if( delchild ) then
				n = t->l
				astDelNode( t )
				if( t <> l ) then
					astDelNode( l )
				end if

				astSetType( n, dtype, subtype )
				return n
			end if
		end if

		if( astIsCONST( l ) ) then
			ofs += astGetValueAsInt( l )
			astDelNode( l )
			l = NULL
		end if
	end if

	'' alloc new node
	n = astNewNode( AST_NODECLASS_DEREF, dtype, subtype )

	if( n = NULL ) then
		return NULL
	end if

	n->l = l
	n->ptr.ofs = ofs

	function = n

end function

'':::::
function astLoadDEREF _
	( _
		byval n as ASTNODE ptr _
	) as IRVREG ptr

    dim as ASTNODE ptr l = any
    dim as IRVREG ptr v1 = any, vp = any, vr = any

	l = n->l
	'' no index? can happen with absolute addresses + ptr typecasting
	if( l = NULL ) then
		if( ast.doemit ) then
			vr = irAllocVRPTR( astGetDataType( n ), n->subtype, n->ptr.ofs, NULL )
			vr->vector = n->vector
		end if
		return vr
	end if

	v1 = astLoad( l )

	''
	if( ast.doemit ) then
		'' src is not a reg?
		if( (irIsREG( v1 ) = FALSE) or _
			(irGetVRDataClass( v1 ) <> FB_DATACLASS_INTEGER) or _
			(irGetVRDataSize( v1 ) <> FB_POINTERSIZE) ) then

			vp = irAllocVREG( typeAddrOf( astGetDataType( n ) ), n->subtype )
			irEmitADDR( AST_OP_DEREF, v1, vp )
		else
			vp = v1
		end if

		vr = irAllocVRPTR( astGetDataType( n ), n->subtype, n->ptr.ofs, vp )
		vr->vector = n->vector
	end if

	astDelNode( l )

	function = vr

end function

