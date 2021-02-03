' This is file cairo-ps.bi
' (FreeBasic binding for cairo library version 1.10.2)
'
' (C) 2011 Thomas[ dot ]Freiherr[ at ]gmx[ dot ]net
' translated with help of h_2_bi.bas
' (http://www.freebasic-portal.de/downloads/ressourcencompiler/h2bi-bas-134.html)
'
' Licence:
' This library binding is free software; you can redistribute it
' and/or modify it under the terms of the GNU Lesser General Public
' License as published by the Free Software Foundation; either
' version 2 of the License, or (at your option) ANY later version.
'
' This binding is distributed in the hope that it will be useful,
' but WITHOUT ANY WARRANTY; without even the implied warranty of
' MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
' Lesser General Public License for more details, refer to:
' http://www.gnu.org/licenses/lgpl.html
'
'
' Original license text:
'
'/* cairo - a vector graphics library with display and print output
 '*
 '* Copyright © 2002 University of Southern California
 '*
 '* This library is free software; you can redistribute it and/or
 '* modify it either under the terms of the GNU Lesser General Public
 '* License version 2.1 as published by the Free Software Foundation
 '* (the "LGPL") or, at your option, under the terms of the Mozilla
 '* Public License Version 1.1 (the "MPL"). If you do not alter this
 '* notice, a recipient may use your version of this file under either
 '* the MPL or the LGPL.
 '*
 '* You should have received a copy of the LGPL along with this library
 '* in the file COPYING-LGPL-2.1; if not, write to the Free Software
 '* Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335, USA
 '* You should have received a copy of the MPL along with this library
 '* in the file COPYING-MPL-1.1
 '*
 '* The contents of this file are subject to the Mozilla Public License
 '* Version 1.1 (the "License"); you may not use this file except in
 '* compliance with the License. You may obtain a copy of the License at
 '* http://www.mozilla.org/MPL/
 '*
 '* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 '* OF ANY KIND, either express or implied. See the LGPL or the MPL for
 '* the specific language governing rights and limitations.
 '*
 '* The Original Code is the cairo graphics library.
 '*
 '* The Initial Developer of the Original Code is University of Southern
 '* California.
 '*
 '* Contributor(s):
 '*  Carl D. Worth <cworth@cworth.org>
 '*/

#IFNDEF CAIRO_PS_H
#DEFINE CAIRO_PS_H
#INCLUDE ONCE "cairo.bi"

EXTERN "C"

#IF CAIRO_HAS_PS_SURFACE

#INCLUDE ONCE "crt/stdio.bi"

ENUM _cairo_ps_level
  CAIRO_PS_LEVEL_2
  CAIRO_PS_LEVEL_3
END ENUM

TYPE AS _cairo_ps_level cairo_ps_level_t

DECLARE FUNCTION cairo_ps_surface_create CDECL(BYVAL AS CONST ZSTRING PTR, BYVAL AS DOUBLE, BYVAL AS DOUBLE) AS cairo_surface_t PTR
DECLARE FUNCTION cairo_ps_surface_create_for_stream CDECL(BYVAL AS cairo_write_func_t, BYVAL AS ANY PTR, BYVAL AS DOUBLE, BYVAL AS DOUBLE) AS cairo_surface_t PTR
DECLARE SUB cairo_ps_surface_restrict_to_level CDECL(BYVAL AS cairo_surface_t PTR, BYVAL AS cairo_ps_level_t)
DECLARE SUB cairo_ps_get_levels CDECL(BYVAL AS cairo_ps_level_t CONST PTR PTR, BYVAL AS INTEGER PTR)
DECLARE FUNCTION cairo_ps_level_to_string AS FUNCTION CDECL(BYVAL AS cairo_ps_level_t) AS CONST ZSTRING PTR
DECLARE SUB cairo_ps_surface_set_eps CDECL(BYVAL AS cairo_surface_t PTR, BYVAL AS cairo_bool_t)
DECLARE FUNCTION cairo_ps_surface_get_eps CDECL(BYVAL AS cairo_surface_t PTR) AS cairo_bool_t
DECLARE SUB cairo_ps_surface_set_size CDECL(BYVAL AS cairo_surface_t PTR, BYVAL AS DOUBLE, BYVAL AS DOUBLE)
DECLARE SUB cairo_ps_surface_dsc_comment CDECL(BYVAL AS cairo_surface_t PTR, BYVAL AS CONST ZSTRING PTR)
DECLARE SUB cairo_ps_surface_dsc_begin_setup CDECL(BYVAL AS cairo_surface_t PTR)
DECLARE SUB cairo_ps_surface_dsc_begin_page_setup CDECL(BYVAL AS cairo_surface_t PTR)

#ELSE
#ERROR Cairo was not compiled with support for the ps backend
#ENDIF

END EXTERN

#ENDIF
