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


'' object and library info section reader and writer
''
'' chng: dec/2006 written [v1ctor]
''

#include once "inc\fb.bi"
#include once "inc\fbint.bi"
#include once "inc\list.bi"
#include once "inc\ast.bi"

#include once "inc\fb-bfd-bridge.bi"

declare function hProcessObject _
	( _
		byval objf as bfd ptr, _
		byval objName as zstring ptr, _
		byval addLib as FB_CALLBACK_ADDLIB, _
		byval addLibPath as FB_CALLBACK_ADDLIBPATH, _
		byval addOption as FB_CALLBACK_ADDOPTION _
	) as integer

'':::::
private sub hInitialize( )
	static as integer inited = FALSE

	if( inited = FALSE ) then
		inited = TRUE
		bfd_init( )
	end if

end sub

'':::::
function fbObjInfoReadObj _
	( _
		byval objName as zstring ptr, _
		byval addLib as FB_CALLBACK_ADDLIB, _
		byval addLibPath as FB_CALLBACK_ADDLIBPATH, _
		byval addOption as FB_CALLBACK_ADDOPTION _
	) as integer

	hInitialize( )

	'' open the object file for read
	dim as bfd ptr objf = bfd_openr( objName, NULL )

	if( objf = NULL ) then
#if defined( DEBUG_BFD )
		print "BFD couldn't open " & *objName
#endif
		return FALSE
	else
#if defined( DEBUG_BFD )
		print "BFD opened " & *objName
#endif
	end if

	'' dump it
	function = hProcessObject( objf, objName, addLib, addLibPath, addOption )

	'' close the object file
	bfd_close( objf )

end function

'':::::
function fbObjInfoReadLib _
	( _
		byval libName as zstring ptr, _
		byval addLib as FB_CALLBACK_ADDLIB, _
		byval addLibPath as FB_CALLBACK_ADDLIBPATH, _
		byval addOption as FB_CALLBACK_ADDOPTION, _
		byval libpathlist as TLIST ptr _
	) as integer

	hInitialize( )

	static as string libfile
	static as string filename

	'' only search for static libraries (import are named .dll.a and dynamic .so)
	filename =  "lib" + *libName + ".a"

	'' try finding it at the lib paths
	dim as FBS_LIB ptr path = listGetHead( libpathlist )
	do while( path <> NULL )

		libfile = *path->name
		libfile += FB_HOST_PATHDIV + filename
		if( hFileExists( libfile ) ) then
#if defined( DEBUG_BFD )
			print "BFD found library """ & libfile & """"
#endif
			exit do
		end if

		path = listGetnext( path )
	loop

	'' not found?
	if( path = NULL ) then
		return FALSE
	end if

	'' open the archive file for read
	dim as bfd ptr archf = bfd_openr( libfile, NULL )

	if( archf = NULL ) then
#if defined( DEBUG_BFD )
		print "BFD couldn't open " & libfile
#endif
		return FALSE
	else
#if defined( DEBUG_BFD )
		print "BFD opened " & libfile
#endif
	end if

	'' not a valid archive file?
	if( bfd_check_format( archf, bfd_archive ) = FALSE ) then
  		bfd_close( archf )
		return FALSE
	end if

	'' for each object in the archive..
	dim as bfd ptr objf = NULL

	'' get the first (and only the first) object file
	objf = bfd_openr_next_archived_file( archf, objf )
	if( objf <> NULL ) then

		if( *bfd_get_filename( objf ) = FB_INFOSEC_OBJNAME ) then
			function = hProcessObject( objf, libname, addLib, addLibPath, addOption )
		end if

		bfd_close( objf )

	else
		function = FALSE
	end if

	'' close the archive file
	bfd_close( archf )

end function

'':::::
private function hFillSection _
	( _
		byval liblist as TLIST ptr, _
		byval libpathlist as TLIST ptr, _
		byref size as integer _
	) as byte ptr

	static as integer buf_size = 0
	static as byte ptr buf_ini = NULL
	dim as integer header_emitted = FALSE
	dim as integer buf_len = 0
	dim as byte ptr buf_end = buf_ini + buf_size
	dim as byte ptr buf_ptr = buf_ini

	'' this must follow the emitWriteInfoSection() format

#macro hAllocSpace( bytes )
	if( buf_ptr + (bytes) >= buf_end ) then
		buf_size += iif( (bytes) > 64, bytes, 64 )
		buf_ptr -= cast( long, buf_ini )
		buf_ini = reallocate( buf_ini, buf_size )
		buf_end = buf_ini + buf_size
		buf_ptr += cast( long, buf_ini )
	end if
#endmacro

#macro hWriteByte( value )
	hAllocSpace( 1 )
	*buf_ptr = value
	buf_ptr += 1
#endmacro

#macro hWriteStr( value, lgt )
	hAllocSpace( (lgt) + 1 )
	*cast( zstring ptr, buf_ptr ) = value
	buf_ptr += (lgt) + 1
#endmacro

#macro hEmitInfoHeader( )
	if( header_emitted = FALSE ) then
		header_emitted = TRUE
		hWriteByte( FB_INFOSEC_VERSION )
	end if
#endmacro

	'' libraries
	dim as FBS_LIB ptr nlib = listGetHead( liblist )
	if( nlib <> NULL ) then
		hEmitInfoHeader( )

        hWriteByte( FB_INFOSEC_LIB )
		do
            '' never add a default one
            if( nlib->isdefault = FALSE ) then
            	dim as integer lgt = len( *nlib->name )
            	hWriteByte( lgt )
            	hWriteStr( *nlib->name, lgt )
#if defined( DEBUG_BFD )
            	print "BFD wrote library: " & *nlib->name
#endif
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
            	dim as integer lgt = len( *npath->name )
            	hWriteByte( lgt )
            	hWriteStr( *npath->name, lgt )
#if defined( DEBUG_BFD )
            	print "BFD wrote lib path: " & *nlib->name
#endif
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
            dim as integer lgt = len( "-lang" )
            hWriteByte( lgt )
            hWriteStr( "-lang", lgt )

            dim as string text = fbGetLangName( env.clopt.lang )
            lgt = len( text )
            hWriteStr( text, lgt )
    	end if

    	'' MT?
    	if( env.clopt.multithreaded ) then
            dim as integer lgt = len( "-mt" )
            hWriteByte( lgt )
            hWriteStr( "-mt", lgt )
		end if

		hWriteByte( 0 )
	end if

	if( header_emitted ) then
		hWriteByte( FB_INFOSEC_EOL )

		size = buf_ptr - buf_ini
		function = buf_ini
	else
		size = 0
		function = NULL
	end if

end function

'':::::
function fbObjInfoWriteObj _
	( _
		byval liblist as TLIST ptr, _
		byval libpathlist as TLIST ptr _
	) as integer

	'' note: both lists of FBS_LIB

	dim as bfd ptr objf = bfd_openw( FB_INFOSEC_OBJNAME, NULL )
	if( objf = NULL ) then
		return FALSE
	end if

  	if( bfd_set_format( objf, bfd_object ) = FALSE ) then
		bfd_close( objf )
		return FALSE
  	end if

  	'' !!!FIXME!!! x86 assumption
  	if( bfd_set_arch_mach( objf, bfd_arch_i386, 0 ) = FALSE ) then
		bfd_close( objf )
		return FALSE
  	end if

	'' create a new section
	dim as asection ptr sec = bfd_make_section( objf, "." + FB_INFOSEC_NAME )
	if( sec = NULL ) then
		bfd_close( objf )
		return FALSE
	end if

	bfd_set_section_flags( objf, _
						   sec, _
						   SEC_HAS_CONTENTS )
	'' fill it
    dim as integer size = 0
    dim as byte ptr buf = hFillSection( liblist, libpathlist, size )

    '' save the contents
    if( (buf <> NULL) and (size > 0) ) then
#if defined( DEBUG_BFD )
    	print "BFD saved data"
#endif
		bfd_set_section_size( objf, sec, size )
		bfd_set_section_contents( objf, sec, buf, 0, size )
		function = TRUE
	else
		function = FALSE
	end if

	'' close the output obj
	bfd_close( objf )

end function

'':::::
function hProcessLibList _
	( _
		byval buf_ini as byte ptr, _
		byval buf_end as byte ptr, _
		byval addLib as FB_CALLBACK_ADDLIB, _
		byval objName as zstring ptr _
	) as integer

	dim as byte ptr p = buf_ini

	'' for each entry..
	do while( p < buf_end )
		dim as integer lgt = *p
	  	p += 1

	  	if( lgt = 0 ) then
	  		exit do
		end if

#if defined( DEBUG_BFD )
        print "BFD added library """ & *cast( zstring ptr, p ) & """"
#endif
		addLib( cast( zstring ptr, p ), objName )

	  	p += lgt + 1
	loop

	'' return the list length
	function = p - buf_ini

end function

'':::::
function hProcessLibPathList _
	( _
		byval buf_ini as byte ptr, _
		byval buf_end as byte ptr, _
		byval addLibPath as FB_CALLBACK_ADDLIBPATH, _
		byval objName as zstring ptr _
	) as integer

	dim as byte ptr p = buf_ini

	'' for each entry..
	do while( p < buf_end )
		dim as integer lgt = *p
	  	p += 1

	  	if( lgt = 0 ) then
	  		exit do
		end if
        
#if defined( DEBUG_BFD )
        print "BFD added library path """ & *cast( zstring ptr, p ) & """"
#endif
		addLibPath( cast( zstring ptr, p ), objName )

	  	p += lgt + 1
	loop

	'' return the list length
	function = p - buf_ini

end function

'':::::
function hProcessCmdList _
	( _
		byval buf_ini as byte ptr, _
		byval buf_end as byte ptr, _
		byval addOption as FB_CALLBACK_ADDOPTION, _
		byval objName as zstring ptr _
	) as integer

	dim as byte ptr p = buf_ini
	dim as zstring ptr value = any
	dim as FB_COMPOPT opt = any

	'' for each entry..
	do while( p < buf_end )
		dim as integer lgt = *p
	  	p += 1

	  	if( lgt = 0 ) then
	  		exit do
		end if

		select case *cast( zstring ptr, p )
		case "-mt"
			opt = FB_COMPOPT_MULTITHREADED
			value = NULL

		case "-lang"
			opt = FB_COMPOPT_LANG
			value = p + len( "-lang" ) + 1
			lgt += len( *value ) + 1
		end select

		addOption( opt, value, objName )

	  	p += lgt + 1
	loop

	'' return the list length
	function = p - buf_ini

end function

'':::::
private function hProcessObject _
	( _
		byval objf as bfd ptr, _
		byval objName as zstring ptr, _
		byval addLib as FB_CALLBACK_ADDLIB, _
		byval addLibPath as FB_CALLBACK_ADDLIB, _
		byval addOption as FB_CALLBACK_ADDOPTION _
	) as integer

	'' not a valid object file?
	if( bfd_check_format_matches( objf, bfd_object, NULL ) = FALSE ) then
#if defined( DEBUG_BFD )
		print "BFD format mismatch"
#endif
		return FALSE
	end if

	'' search for our special section
	dim as asection ptr sec = bfd_get_section_by_name( objf, "." + FB_INFOSEC_NAME )
	if( sec = NULL ) then
#if defined( DEBUG_BFD )
		print "BFD couldn't find FB info"
#endif
		return FALSE
	end if

  	'' load the section to memory
  	dim as integer size = bfd_get_section_size( sec )
  	if( size = 0 ) then
#if defined( DEBUG_BFD )
		print "BFD couldn't load the section"
#endif
  		return FALSE
	end if

  	'' alloc mem
  	static as byte ptr buf_ini = NULL
  	static as integer buf_len = 0

  	if( (buf_ini = NULL) or (size > buf_len) ) then
  		buf_ini = reallocate( buf_ini, size )
  		buf_len = size
  	end if

  	bfd_get_section_contents( objf, sec, buf_ini, 0, size )

  	dim as byte ptr buf_end = buf_ini + size, p = buf_ini

  	'' check version
  	if( *p <> FB_INFOSEC_VERSION ) then
#if defined( DEBUG_BFD )
		print "BFD couldn't match versions"
#endif
  		return FALSE
  	end if

  	p += 1

  	'' for each entry in the section..
  	do while( p < buf_end )
  		dim as integer id = *p
  		if( id = FB_INFOSEC_EOL ) then
  			exit do
  		end if

  		p += 1

      	'' dump the entries
      	select case as const id
      	case FB_INFOSEC_LIB
#if defined( DEBUG_BFD )
      		print "BFD found a library in " & *objName
#endif
	  		p += hProcessLibList( p, buf_end, addLib, objName )

      	case FB_INFOSEC_PTH
#if defined( DEBUG_BFD )
      		print "BFD found a path in " & *objName
#endif
	  		p += hProcessLibPathList( p, buf_end, addLibPath, objName )

      	case FB_INFOSEC_CMD
#if defined( DEBUG_BFD )
      		print "BFD found a command in " & *objName
#endif
	  		p += hProcessCmdList( p, buf_end, addOption, objName )

	  	case else
	  		exit do
	  	end select

	loop

	function = TRUE

end function
