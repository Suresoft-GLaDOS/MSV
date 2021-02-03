' This is file gtk.bi
' (FreeBasic binding for GTK - The GIMP Toolkit)
'
' (C) 2011-2012 Thomas[ dot ]Freiherr[ at ]gmx[ dot ]net
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
' See #INCLUDE files for original license text.

' GTK installations:
' Defaults to compile against GTK 2.24. Prepend your code by
'#DEFINE __USE_GTK_OLD__ 'for compiling against versions < 2.16 (and >= 2.0),
'#DEFINE __USE_GTK3__ 'for compiling against versions >= 3.0

#IFDEF __USE_GTK3__
#INCLUDE ONCE "gtk-3.4.bi"
#ELSE
#INCLUDE ONCE "gtk-2.24.bi"
#ENDIF
