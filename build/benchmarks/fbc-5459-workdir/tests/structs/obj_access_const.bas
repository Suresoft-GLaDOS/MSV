' TEST_MODE : COMPILE_ONLY_FAIL

Type Test
        Private:
                enum first
                        one = 1
                End enum

        Public:
                enum second
                        two = 2
                End enum

                X As Integer
End Type

	var r = test.first.one
	r = test.one
	
