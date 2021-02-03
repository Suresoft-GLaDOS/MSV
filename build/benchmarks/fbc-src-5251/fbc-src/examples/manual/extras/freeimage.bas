'' examples/manual/extras/freeimage.bas
''
'' NOTICE: This file is part of the FreeBASIC Compiler package and can't
''         be included in other distributions without authorization.
''
'' See Also: http://www.freebasic.net/wiki/wikka.php?wakka=FreeImage
'' --------


#include "freeimage.bi"
#include "crt.bi"

'' Code example for loading all common image types using FreeImage.
'' The example loads an image passed as a command line argument.

'' The function FI_Load returns a null pointer (0) if there was an error during 
'' loading.  Otherwise it returns a 32-bit PUT compatible buffer 

Declare Function FI_Load(filename As String) As Any Ptr

'::::
Function FI_Load(filename As String) As Any Ptr
	Dim FIF As FREE_IMAGE_FORMAT
	Dim dib As FIBITMAP Ptr
	Dim dib32 As FIBITMAP Ptr
	Dim DIBWidth As UInteger
	Dim DIBHeight As UInteger
	Dim flags As UInteger
	Dim Sprite As Any Ptr
	Dim Bits As Any Ptr 

	'' Get File Format
	FIF = FreeImage_GetFileType(StrPtr(filename), 0)
	If FIF = FIF_UNKNOWN Then
		FIF = FreeImage_GetFIFFromFilename(StrPtr(filename))
	End If

	'' Exit If Unknown
	If FIF = FIF_UNKNOWN Then
		Return NULL
	End If

	'' Always load jpegs accurately
	If FIF = FIF_JPEG Then 
		flags = JPEG_ACCURATE
	End If

	'' Load Image
	dib = FreeImage_Load(FIF, StrPtr(filename), flags)

	If dib = 0 Then
		'' Problem During Load
		Return NULL
	End If

	'' Get Size
	DIBWidth = FreeImage_GetWidth(dib)
	DIBHeight = FreeImage_GetHeight(dib)

	'' Flip and force 32 bits
	FreeImage_FlipVertical Dib
	Dib32 = FreeImage_ConvertTo32Bits(Dib)

	'' make our sprite and get a ptr to the FI dib
	Sprite = ImageCreate(DIBWidth, DIBHeight)
	Bits = FreeImage_GetBits(Dib32)

	'' copy from dib to sprite
	MemCpy @CPtr(UByte Ptr, Sprite)[4], Bits, DIBWidth * DIBHeight * 4

	'' Unload dibs
	FreeImage_Unload dib
	FreeImage_Unload dib32

	Return Sprite

End Function

'' ========
'' MAIN
'' ========

Dim Image As UInteger Ptr

ScreenRes 640, 480, 32

Image = FI_Load(Command$(1))

If Image <> 0 Then
	Put (0, 0), Image
Else
	Print "Problem Loading File : " & Command$(1)
End If

Sleep
