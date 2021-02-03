# include "fbcu.bi"




namespace fbc_tests.functions.var_args

const TEST_B as byte = -128
const TEST_UB as ubyte = 255
const TEST_S as short = -32768
const TEST_US as ushort = 65535
const TEST_I as integer = -2147483648
const TEST_UI as uinteger = 4294967295U
const TEST_L as longint = 9223372036854775808
const TEST_UL as ulongint = 18446744073709551615ULL
const TEST_F as single = 1.234567
const TEST_D as double = 1.23456789012345
const TEST_Z as string = "FoO BaR!"

'':::::	
sub test cdecl (fmtstr as string, ...)
	dim as any ptr arg
	dim as zstring ptr p
	dim as integer i, char
	
	arg = va_first()
	
	p = strptr( fmtstr )
	i = len( fmtstr )
	do while( i > 0 ) 
		char = *p
		p += 1
		i -= 1
		
		if( char = asc( "%" ) ) then
			char = *p
			p += 1
			i -= 1
			
			select case as const char
			case asc( "b" )
				CU_ASSERT( va_arg( arg, byte ) = TEST_B )
				arg = va_next( arg, integer )

			case asc( "c" )
				CU_ASSERT( va_arg( arg, ubyte ) = TEST_UB )
				arg = va_next( arg, uinteger )

			case asc( "s" )
				CU_ASSERT( va_arg( arg, short ) = TEST_S )
				arg = va_next( arg, integer )

			case asc( "r" )
				CU_ASSERT( va_arg( arg, ushort ) = TEST_US )
				arg = va_next( arg, uinteger )

			case asc( "i" )
				CU_ASSERT( va_arg( arg, integer ) = TEST_I )
				arg = va_next( arg, integer )

			case asc( "j" )
				CU_ASSERT( va_arg( arg, uinteger ) = TEST_UI )
				arg = va_next( arg, uinteger )

			case asc( "l" )
				CU_ASSERT( va_arg( arg, longint ) = TEST_L )
				arg = va_next( arg, longint )

			case asc( "m" )
				CU_ASSERT( va_arg( arg, ulongint ) = TEST_UL )
				arg = va_next( arg, ulongint )
			
			case asc( "f" )
				CU_ASSERT( va_arg( arg, double ) = TEST_F )
				arg = va_next( arg, double )

			case asc( "d" )
				CU_ASSERT( va_arg( arg, double ) = TEST_D )
				arg = va_next( arg, double )
			
			case asc( "z" )
				CU_ASSERT( *va_arg( arg, zstring ptr ) = TEST_Z )
				arg = va_next( arg, zstring ptr )
			end select
			
		end if
		
	loop

end sub

sub test_1 cdecl ()

	dim as byte b = TEST_B
	dim as ubyte ub = TEST_UB
	dim as short s = TEST_S
	dim as ushort us = TEST_US
	dim as integer i = TEST_I
	dim as uinteger ui = TEST_UI
	dim as longint l = TEST_L
	dim as ulongint ul = TEST_UL
	dim as single f = TEST_F
	dim as double d = TEST_D
	dim as string vs = TEST_Z
	dim as zstring * 32 z = TEST_Z

	test( "%b %c %s %r %i %j %l %m %f %d %z %z", _
		   b, ub, s, us, i, ui, l, ul, f, d, vs, z )

end sub

sub ctor () constructor

	fbcu.add_suite("fbc_tests.functions.var_args")
	fbcu.add_test("test_1", @test_1)

end sub

end namespace
