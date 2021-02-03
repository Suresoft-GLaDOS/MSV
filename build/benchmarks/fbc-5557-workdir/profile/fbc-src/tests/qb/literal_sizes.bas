' TEST_MODE : COMPILE_AND_RUN_OK

#define ASSERT_SHORT(c) __ASSERT( __sizeof(c) = __sizeof(integer) )
#define ASSERT_LONG(c) __ASSERT( __sizeof(c) = __sizeof(long) )

dim i as integer, l as long

__ASSERT( __sizeof(integer) = 2 )
__ASSERT( __sizeof(long) = 4 )

ASSERT_SHORT( 32767 )

ASSERT_SHORT( &h8000 )
ASSERT_SHORT( &o100000 )
ASSERT_SHORT( &b1000000000000000 )

ASSERT_SHORT( &h7fff )
ASSERT_SHORT( &o077777 )
ASSERT_SHORT( &b0111111111111111 )

ASSERT_SHORT( &hffff )
ASSERT_SHORT( &o177777 )
ASSERT_SHORT( &b1111111111111111 )


ASSERT_LONG(  32767& )

ASSERT_LONG(  32768 )

ASSERT_LONG(  &h10000 )
ASSERT_LONG(  &o200000 )
ASSERT_LONG(  &o1000000 )
ASSERT_LONG(  &b10000000000000000 )


l = 100000
__ASSERT( l = 100000& )

i = 100
__ASSERT( 100000 \ i = 1000 )
