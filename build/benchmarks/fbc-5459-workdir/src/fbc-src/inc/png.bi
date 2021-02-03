''
''
'' png -- header translated with help of SWIG FB wrapper
''
'' NOTICE: This file is part of the FreeBASIC Compiler package and can't
''         be included in other distributions without authorization.
''
''
#ifndef __png_bi__
#define __png_bi__

#inclib "png"

#define PNG_LIBPNG_VER_STRING "1.2.16"
#define PNG_HEADER_VERSION_STRING !" libpng version 1.2.16 - January 31, 2007 (header)\n"
#define PNG_LIBPNG_VER_SONUM 0
#define PNG_LIBPNG_VER_DLLNUM 13
#define PNG_LIBPNG_VER_MAJOR 1
#define PNG_LIBPNG_VER_MINOR 2
#define PNG_LIBPNG_VER_RELEASE 16
#define PNG_LIBPNG_VER_BUILD 0
#define PNG_LIBPNG_BUILD_ALPHA 1
#define PNG_LIBPNG_BUILD_BETA 2
#define PNG_LIBPNG_BUILD_RC 3
#define PNG_LIBPNG_BUILD_STABLE 4
#define PNG_LIBPNG_BUILD_RELEASE_STATUS_MASK 7
#define PNG_LIBPNG_BUILD_PATCH 8
#define PNG_LIBPNG_BUILD_PRIVATE 16
#define PNG_LIBPNG_BUILD_SPECIAL 32
#define PNG_LIBPNG_BUILD_BASE_TYPE 4
#define PNG_LIBPNG_VER 10216

#include once "zlib.bi"

'' begin include from pngconf.bi
#define PNG_ZBUF_SIZE 8192

#include once "crt/stdio.bi"
#include once "crt/sys/types.bi"
#include once "crt/setjmp.bi"
#include once "crt/string.bi"

#define PNG_USER_WIDTH_MAX 1000000L
#define PNG_USER_HEIGHT_MAX 1000000L

#include once "crt/time.bi"

type png_uint_32 as uinteger
type png_int_32 as integer
type png_uint_16 as ushort
type png_int_16 as short
type png_byte as ubyte
type png_size_t as size_t
type png_fixed_point as png_int_32
type png_voidp as any ptr
type png_bytep as png_byte ptr
type png_uint_32p as png_uint_32 ptr
type png_int_32p as png_int_32 ptr
type png_uint_16p as png_uint_16 ptr
type png_int_16p as png_int_16 ptr
type png_const_charp as zstring ptr
type png_charp as zstring ptr
type png_fixed_point_p as png_fixed_point ptr
type png_FILE_p as FILE ptr
type png_doublep as double ptr
type png_bytepp as png_byte ptr ptr
type png_uint_32pp as png_uint_32 ptr ptr
type png_int_32pp as png_int_32 ptr ptr
type png_uint_16pp as png_uint_16 ptr ptr
type png_int_16pp as png_int_16 ptr ptr
type png_const_charpp as zstring ptr ptr
type png_charpp as zstring ptr ptr
type png_fixed_point_pp as png_fixed_point ptr ptr
type png_doublepp as double ptr ptr
type png_charppp as zstring ptr ptr ptr
type png_zcharp as charf ptr
type png_zcharpp as charf ptr ptr
type png_zstreamp as z_stream ptr

#ifndef PNG_ABORT
# define PNG_ABORT() abort()
#endif

#define png_jmpbuf(png) @(png)->jmpbuf
'' end include

#define PNG_LIBPNG_BUILD_TYPE 4
extern import png_libpng_ver_str alias "png_libpng_ver" as zstring * 18
extern import png_pass_start(0 to 7-1) alias "png_pass_start" as integer
extern import png_pass_inc(0 to 7-1) alias "png_pass_inc" as integer
extern import png_pass_ystart(0 to 7-1) alias "png_pass_ystart" as integer
extern import png_pass_yinc(0 to 7-1) alias "png_pass_yinc" as integer
extern import png_pass_mask(0 to 7-1) alias "png_pass_mask" as integer
extern import png_pass_dsp_mask(0 to 7-1) alias "png_pass_dsp_mask" as integer

type png_color
	red as png_byte
	green as png_byte
	blue as png_byte
end type

type png_colorp as png_color ptr
type png_colorpp as png_color ptr ptr

type png_color_16
	index as png_byte
	red as png_uint_16
	green as png_uint_16
	blue as png_uint_16
	gray as png_uint_16
end type

type png_color_16p as png_color_16 ptr
type png_color_16pp as png_color_16 ptr ptr

type png_color_8
	red as png_byte
	green as png_byte
	blue as png_byte
	gray as png_byte
	alpha as png_byte
end type

type png_color_8p as png_color_8 ptr
type png_color_8pp as png_color_8 ptr ptr

type png_sPLT_entry
	red as png_uint_16
	green as png_uint_16
	blue as png_uint_16
	alpha as png_uint_16
	frequency as png_uint_16
end type

type png_sPLT_entryp as png_sPLT_entry ptr
type png_sPLT_entrypp as png_sPLT_entry ptr ptr

type png_sPLT
	name as png_charp
	depth as png_byte
	entries as png_sPLT_entryp
	nentries as png_int_32
end type

type png_sPLT_tp as png_sPLT_t ptr
type png_sPLT_tpp as png_sPLT_t ptr ptr

type png_text
	compression as integer
	key as png_charp
	text as png_charp
	text_length as png_size_t
end type

type png_textp as png_text ptr
type png_textpp as png_text ptr ptr

#define PNG_TEXT_COMPRESSION_NONE_WR -3
#define PNG_TEXT_COMPRESSION_zTXt_WR -2
#define PNG_TEXT_COMPRESSION_NONE -1
#define PNG_TEXT_COMPRESSION_zTXt 0
#define PNG_ITXT_COMPRESSION_NONE 1
#define PNG_ITXT_COMPRESSION_zTXt 2
#define PNG_TEXT_COMPRESSION_LAST 3

type png_time
	year as png_uint_16
	month as png_byte
	day as png_byte
	hour as png_byte
	minute as png_byte
	second as png_byte
end type

type png_timep as png_time ptr
type png_timepp as png_time ptr ptr

type png_unknown_chunk
	name(0 to 5-1) as png_byte
	data as png_byte ptr
	size as png_size_t
	location as png_byte
end type

type png_unknown_chunkp as png_unknown_chunk ptr
type png_unknown_chunkpp as png_unknown_chunk ptr ptr

type png_info
	width as png_uint_32
	height as png_uint_32
	valid as png_uint_32
	rowbytes as png_uint_32
	palette as png_colorp
	num_palette as png_uint_16
	num_trans as png_uint_16
	bit_depth as png_byte
	color_type as png_byte
	compression_type as png_byte
	filter_type as png_byte
	interlace_type as png_byte
	channels as png_byte
	pixel_depth as png_byte
	spare_byte as png_byte
	signature(0 to 8-1) as png_byte
	gamma as single
	srgb_intent as png_byte
	num_text as integer
	max_text as integer
	text as png_textp
	mod_time as png_time
	sig_bit as png_color_8
	trans as png_bytep
	trans_values as png_color_16
	background as png_color_16
	x_offset as png_int_32
	y_offset as png_int_32
	offset_unit_type as png_byte
	x_pixels_per_unit as png_uint_32
	y_pixels_per_unit as png_uint_32
	phys_unit_type as png_byte
	hist as png_uint_16p
	x_white as single
	y_white as single
	x_red as single
	y_red as single
	x_green as single
	y_green as single
	x_blue as single
	y_blue as single
	pcal_purpose as png_charp
	pcal_X0 as png_int_32
	pcal_X1 as png_int_32
	pcal_units as png_charp
	pcal_params as png_charpp
	pcal_type as png_byte
	pcal_nparams as png_byte
	free_me as png_uint_32
	unknown_chunks as png_unknown_chunkp
	unknown_chunks_num as png_size_t
	iccp_name as png_charp
	iccp_profile as png_charp
	iccp_proflen as png_uint_32
	iccp_compression as png_byte
	splt_palettes as png_sPLT_tp
	splt_palettes_num as png_uint_32
	scal_unit as png_byte
	scal_pixel_width as double
	scal_pixel_height as double
	scal_s_width as png_charp
	scal_s_height as png_charp
	row_pointers as png_bytepp
	int_gamma as png_fixed_point
	int_x_white as png_fixed_point
	int_y_white as png_fixed_point
	int_x_red as png_fixed_point
	int_y_red as png_fixed_point
	int_x_green as png_fixed_point
	int_y_green as png_fixed_point
	int_x_blue as png_fixed_point
	int_y_blue as png_fixed_point
end type

type png_infop as png_info ptr
type png_infopp as png_info ptr ptr

#define PNG_COLOR_MASK_PALETTE 1
#define PNG_COLOR_MASK_COLOR 2
#define PNG_COLOR_MASK_ALPHA 4
#define PNG_COLOR_TYPE_GRAY 0
#define PNG_COLOR_TYPE_PALETTE (2 or 1)
#define PNG_COLOR_TYPE_RGB (2)
#define PNG_COLOR_TYPE_RGB_ALPHA (2 or 4)
#define PNG_COLOR_TYPE_GRAY_ALPHA (4)
#define PNG_COLOR_TYPE_RGBA (2 or 4)
#define PNG_COLOR_TYPE_GA (4)
#define PNG_COMPRESSION_TYPE_BASE 0
#define PNG_COMPRESSION_TYPE_DEFAULT 0
#define PNG_FILTER_TYPE_BASE 0
#define PNG_INTRAPIXEL_DIFFERENCING 64
#define PNG_FILTER_TYPE_DEFAULT 0
#define PNG_INTERLACE_NONE 0
#define PNG_INTERLACE_ADAM7 1
#define PNG_INTERLACE_LAST 2
#define PNG_OFFSET_PIXEL 0
#define PNG_OFFSET_MICROMETER 1
#define PNG_OFFSET_LAST 2
#define PNG_EQUATION_LINEAR 0
#define PNG_EQUATION_BASE_E 1
#define PNG_EQUATION_ARBITRARY 2
#define PNG_EQUATION_HYPERBOLIC 3
#define PNG_EQUATION_LAST 4
#define PNG_SCALE_UNKNOWN 0
#define PNG_SCALE_METER 1
#define PNG_SCALE_RADIAN 2
#define PNG_SCALE_LAST 3
#define PNG_RESOLUTION_UNKNOWN 0
#define PNG_RESOLUTION_METER 1
#define PNG_RESOLUTION_LAST 2
#define PNG_sRGB_INTENT_PERCEPTUAL 0
#define PNG_sRGB_INTENT_RELATIVE 1
#define PNG_sRGB_INTENT_SATURATION 2
#define PNG_sRGB_INTENT_ABSOLUTE 3
#define PNG_sRGB_INTENT_LAST 4
#define PNG_KEYWORD_MAX_LENGTH 79
#define PNG_MAX_PALETTE_LENGTH 256
#define PNG_INFO_gAMA &h0001
#define PNG_INFO_sBIT &h0002
#define PNG_INFO_cHRM &h0004
#define PNG_INFO_PLTE &h0008
#define PNG_INFO_tRNS &h0010
#define PNG_INFO_bKGD &h0020
#define PNG_INFO_hIST &h0040
#define PNG_INFO_pHYs &h0080
#define PNG_INFO_oFFs &h0100
#define PNG_INFO_tIME &h0200
#define PNG_INFO_pCAL &h0400
#define PNG_INFO_sRGB &h0800
#define PNG_INFO_iCCP &h1000
#define PNG_INFO_sPLT &h2000
#define PNG_INFO_sCAL &h4000
#define PNG_INFO_IDAT &h8000L

type png_row_info
	width as png_uint_32
	rowbytes as png_uint_32
	color_type as png_byte
	bit_depth as png_byte
	channels as png_byte
	pixel_depth as png_byte
end type

type png_row_infop as png_row_info ptr
type png_row_infopp as png_row_info ptr ptr

type png_struct as png_struct_def
type png_structp as png_struct ptr

type png_error_ptr as sub cdecl(byval as png_structp, byval as png_const_charp)
type png_rw_ptr as sub cdecl(byval as png_structp, byval as png_bytep, byval as png_size_t)
type png_flush_ptr as sub cdecl(byval as png_structp)
type png_read_status_ptr as sub cdecl(byval as png_structp, byval as png_uint_32, byval as integer)
type png_write_status_ptr as sub cdecl(byval as png_structp, byval as png_uint_32, byval as integer)
type png_progressive_info_ptr as sub cdecl(byval as png_structp, byval as png_infop)
type png_progressive_end_ptr as sub cdecl(byval as png_structp, byval as png_infop)
type png_progressive_row_ptr as sub cdecl(byval as png_structp, byval as png_bytep, byval as png_uint_32, byval as integer)
type png_user_transform_ptr as sub cdecl(byval as png_structp, byval as png_row_infop, byval as png_bytep)
type png_user_chunk_ptr as function cdecl(byval as png_structp, byval as png_unknown_chunkp) as integer
type png_unknown_chunk_ptr as sub cdecl(byval as png_structp)

#define PNG_TRANSFORM_IDENTITY &h0000
#define PNG_TRANSFORM_STRIP_16 &h0001
#define PNG_TRANSFORM_STRIP_ALPHA &h0002
#define PNG_TRANSFORM_PACKING &h0004
#define PNG_TRANSFORM_PACKSWAP &h0008
#define PNG_TRANSFORM_EXPAND &h0010
#define PNG_TRANSFORM_INVERT_MONO &h0020
#define PNG_TRANSFORM_SHIFT &h0040
#define PNG_TRANSFORM_BGR &h0080
#define PNG_TRANSFORM_SWAP_ALPHA &h0100
#define PNG_TRANSFORM_SWAP_ENDIAN &h0200
#define PNG_TRANSFORM_INVERT_ALPHA &h0400
#define PNG_TRANSFORM_STRIP_FILLER &h0800
#define PNG_FLAG_MNG_EMPTY_PLTE &h01
#define PNG_FLAG_MNG_FILTER_64 &h04
#define PNG_ALL_MNG_FEATURES &h05

type png_malloc_ptr as function cdecl(byval as png_structp, byval as png_size_t) as png_voidp
type png_free_ptr as sub cdecl(byval as png_structp, byval as png_voidp)

type png_struct_def
	jmpbuf as jmp_buf
	error_fn as png_error_ptr
	warning_fn as png_error_ptr
	error_ptr as png_voidp
	write_data_fn as png_rw_ptr
	read_data_fn as png_rw_ptr
	io_ptr as png_voidp
	read_user_transform_fn as png_user_transform_ptr
	write_user_transform_fn as png_user_transform_ptr
	user_transform_ptr as png_voidp
	user_transform_depth as png_byte
	user_transform_channels as png_byte
	mode as png_uint_32
	flags as png_uint_32
	transformations as png_uint_32
	zstream as z_stream
	zbuf as png_bytep
	zbuf_size as png_size_t
	zlib_level as integer
	zlib_method as integer
	zlib_window_bits as integer
	zlib_mem_level as integer
	zlib_strategy as integer
	width as png_uint_32
	height as png_uint_32
	num_rows as png_uint_32
	usr_width as png_uint_32
	rowbytes as png_uint_32
	irowbytes as png_uint_32
	iwidth as png_uint_32
	row_number as png_uint_32
	prev_row as png_bytep
	row_buf as png_bytep
	sub_row as png_bytep
	up_row as png_bytep
	avg_row as png_bytep
	paeth_row as png_bytep
	row_info as png_row_info
	idat_size as png_uint_32
	crc as png_uint_32
	palette as png_colorp
	num_palette as png_uint_16
	num_trans as png_uint_16
	chunk_name(0 to 5-1) as png_byte
	compression as png_byte
	filter as png_byte
	interlaced as png_byte
	pass as png_byte
	do_filter as png_byte
	color_type as png_byte
	bit_depth as png_byte
	usr_bit_depth as png_byte
	pixel_depth as png_byte
	channels as png_byte
	usr_channels as png_byte
	sig_bytes as png_byte
	filler as png_uint_16
	background_gamma_type as png_byte
	background_gamma as single
	background as png_color_16
	background_1 as png_color_16
	output_flush_fn as png_flush_ptr
	flush_dist as png_uint_32
	flush_rows as png_uint_32
	gamma_shift as integer
	gamma as single
	screen_gamma as single
	gamma_table as png_bytep
	gamma_from_1 as png_bytep
	gamma_to_1 as png_bytep
	gamma_16_table as png_uint_16pp
	gamma_16_from_1 as png_uint_16pp
	gamma_16_to_1 as png_uint_16pp
	sig_bit as png_color_8
	shift as png_color_8
	trans as png_bytep
	trans_values as png_color_16
	read_row_fn as png_read_status_ptr
	write_row_fn as png_write_status_ptr
	info_fn as png_progressive_info_ptr
	row_fn as png_progressive_row_ptr
	end_fn as png_progressive_end_ptr
	save_buffer_ptr as png_bytep
	save_buffer as png_bytep
	current_buffer_ptr as png_bytep
	current_buffer as png_bytep
	push_length as png_uint_32
	skip_length as png_uint_32
	save_buffer_size as png_size_t
	save_buffer_max as png_size_t
	buffer_size as png_size_t
	current_buffer_size as png_size_t
	process_mode as integer
	cur_palette as integer
	current_text_size as png_size_t
	current_text_left as png_size_t
	current_text as png_charp
	current_text_ptr as png_charp
	palette_lookup as png_bytep
	dither_index as png_bytep
	hist as png_uint_16p
	heuristic_method as png_byte
	num_prev_filters as png_byte
	prev_filters as png_bytep
	filter_weights as png_uint_16p
	inv_filter_weights as png_uint_16p
	filter_costs as png_uint_16p
	inv_filter_costs as png_uint_16p
	time_buffer as png_charp
	free_me as png_uint_32
	user_chunk_ptr as png_voidp
	read_user_chunk_fn as png_user_chunk_ptr
	num_chunk_list as integer
	chunk_list as png_bytep
	rgb_to_gray_status as png_byte
	rgb_to_gray_red_coeff as png_uint_16
	rgb_to_gray_green_coeff as png_uint_16
	rgb_to_gray_blue_coeff as png_uint_16
	mng_features_permitted as png_uint_32
	int_gamma as png_fixed_point
	filter_type as png_byte
	mmx_bitdepth_threshold as png_byte
	mmx_rowbytes_threshold as png_uint_32
	asm_flags as png_uint_32
	mem_ptr as png_voidp
	malloc_fn as png_malloc_ptr
	free_fn as png_free_ptr
	big_row_buf as png_bytep
	dither_sort as png_bytep
	index_to_palette as png_bytep
	palette_to_index as png_bytep
	compression_type as png_byte
	user_width_max as png_uint_32
	user_height_max as png_uint_32
end type

type version_1_2_16 as png_structp
type png_structpp as png_struct ptr ptr

#define PNG_FILLER_BEFORE 0
#define PNG_FILLER_AFTER 1

#define PNG_BACKGROUND_GAMMA_UNKNOWN 0
#define PNG_BACKGROUND_GAMMA_SCREEN 1
#define PNG_BACKGROUND_GAMMA_FILE 2
#define PNG_BACKGROUND_GAMMA_UNIQUE 3

#define PNG_CRC_DEFAULT 0
#define PNG_CRC_ERROR_QUIT 1
#define PNG_CRC_WARN_DISCARD 2
#define PNG_CRC_WARN_USE 3
#define PNG_CRC_QUIET_USE 4
#define PNG_CRC_NO_CHANGE 5

#define PNG_NO_FILTERS &h00
#define PNG_FILTER_NONE &h08
#define PNG_FILTER_SUB &h10
#define PNG_FILTER_UP &h20
#define PNG_FILTER_AVG &h40
#define PNG_FILTER_PAETH &h80
#define PNG_ALL_FILTERS (&h08 or &h10 or &h20 or &h40 or &h80)
#define PNG_FILTER_VALUE_NONE 0
#define PNG_FILTER_VALUE_SUB 1
#define PNG_FILTER_VALUE_UP 2
#define PNG_FILTER_VALUE_AVG 3
#define PNG_FILTER_VALUE_PAETH 4
#define PNG_FILTER_VALUE_LAST 5

#define PNG_FILTER_HEURISTIC_DEFAULT 0
#define PNG_FILTER_HEURISTIC_UNWEIGHTED 1
#define PNG_FILTER_HEURISTIC_WEIGHTED 2
#define PNG_FILTER_HEURISTIC_LAST 3

#define PNG_DESTROY_WILL_FREE_DATA 1
#define PNG_SET_WILL_FREE_DATA 1
#define PNG_USER_WILL_FREE_DATA 2
#define PNG_FREE_HIST &h0008
#define PNG_FREE_ICCP &h0010
#define PNG_FREE_SPLT &h0020
#define PNG_FREE_ROWS &h0040
#define PNG_FREE_PCAL &h0080
#define PNG_FREE_SCAL &h0100
#define PNG_FREE_UNKN &h0200
#define PNG_FREE_LIST &h0400
#define PNG_FREE_PLTE &h1000
#define PNG_FREE_TRNS &h2000
#define PNG_FREE_TEXT &h4000
#define PNG_FREE_ALL &h7fff
#define PNG_FREE_MUL &h4220

#define PNG_HANDLE_CHUNK_AS_DEFAULT 0
#define PNG_HANDLE_CHUNK_NEVER 1
#define PNG_HANDLE_CHUNK_IF_SAFE 2
#define PNG_HANDLE_CHUNK_ALWAYS 3
#define PNG_ASM_FLAG_MMX_SUPPORT_COMPILED &h01
#define PNG_ASM_FLAG_MMX_SUPPORT_IN_CPU &h02
#define PNG_ASM_FLAG_MMX_READ_COMBINE_ROW &h04
#define PNG_ASM_FLAG_MMX_READ_INTERLACE &h08
#define PNG_ASM_FLAG_MMX_READ_FILTER_SUB &h10
#define PNG_ASM_FLAG_MMX_READ_FILTER_UP &h20
#define PNG_ASM_FLAG_MMX_READ_FILTER_AVG &h40
#define PNG_ASM_FLAG_MMX_READ_FILTER_PAETH &h80
#define PNG_ASM_FLAGS_INITIALIZED &h80000000
#define PNG_MMX_READ_FLAGS (&h04 or &h08 or &h10 or &h20 or &h40 or &h80)
#define PNG_MMX_WRITE_FLAGS (0)
#define PNG_MMX_FLAGS (&h01 or &h02 or (&h04 or &h08 or &h10 or &h20 or &h40 or &h80) or (0))
#define PNG_SELECT_READ 1
#define PNG_SELECT_WRITE 2

extern "c"
declare function png_access_version_number () as png_uint_32
declare sub png_set_sig_bytes (byval png_ptr as png_structp, byval num_bytes as integer)
declare function png_sig_cmp (byval sig as png_bytep, byval start as png_size_t, byval num_to_check as png_size_t) as integer
declare function png_check_sig (byval sig as png_bytep, byval num as integer) as integer
declare function png_create_read_struct (byval user_png_ver as png_const_charp, byval error_ptr as png_voidp, byval error_fn as png_error_ptr, byval warn_fn as png_error_ptr) as png_structp
declare function png_create_write_struct (byval user_png_ver as png_const_charp, byval error_ptr as png_voidp, byval error_fn as png_error_ptr, byval warn_fn as png_error_ptr) as png_structp
declare function png_get_compression_buffer_size (byval png_ptr as png_structp) as png_uint_32
declare sub png_set_compression_buffer_size (byval png_ptr as png_structp, byval size as png_uint_32)
declare function png_reset_zstream (byval png_ptr as png_structp) as integer
declare function png_create_read_struct_2 (byval user_png_ver as png_const_charp, byval error_ptr as png_voidp, byval error_fn as png_error_ptr, byval warn_fn as png_error_ptr, byval mem_ptr as png_voidp, byval malloc_fn as png_malloc_ptr, byval free_fn as png_free_ptr) as png_structp
declare function png_create_write_struct_2 (byval user_png_ver as png_const_charp, byval error_ptr as png_voidp, byval error_fn as png_error_ptr, byval warn_fn as png_error_ptr, byval mem_ptr as png_voidp, byval malloc_fn as png_malloc_ptr, byval free_fn as png_free_ptr) as png_structp
declare sub png_write_chunk (byval png_ptr as png_structp, byval chunk_name as png_bytep, byval data as png_bytep, byval length as png_size_t)
declare sub png_write_chunk_start (byval png_ptr as png_structp, byval chunk_name as png_bytep, byval length as png_uint_32)
declare sub png_write_chunk_data (byval png_ptr as png_structp, byval data as png_bytep, byval length as png_size_t)
declare sub png_write_chunk_end (byval png_ptr as png_structp)
declare function png_create_info_struct (byval png_ptr as png_structp) as png_infop
declare sub png_info_init (byval info_ptr as png_infop)
declare sub png_info_init_3 (byval info_ptr as png_infopp, byval png_info_struct_size as png_size_t)
declare sub png_write_info_before_PLTE (byval png_ptr as png_structp, byval info_ptr as png_infop)
declare sub png_write_info (byval png_ptr as png_structp, byval info_ptr as png_infop)
declare sub png_read_info (byval png_ptr as png_structp, byval info_ptr as png_infop)
declare function png_convert_to_rfc1123 (byval png_ptr as png_structp, byval ptime as png_timep) as png_charp
declare sub png_convert_from_struct_tm (byval ptime as png_timep, byval ttime as tm ptr)
declare sub png_convert_from_time_t (byval ptime as png_timep, byval ttime as time_t)
declare sub png_set_expand (byval png_ptr as png_structp)
declare sub png_set_gray_1_2_4_to_8 (byval png_ptr as png_structp)
declare sub png_set_expand_gray_1_2_4_to_8 (byval png_ptr as png_structp)
declare sub png_set_palette_to_rgb (byval png_ptr as png_structp)
declare sub png_set_tRNS_to_alpha (byval png_ptr as png_structp)
declare sub png_set_bgr (byval png_ptr as png_structp)
declare sub png_set_gray_to_rgb (byval png_ptr as png_structp)
declare sub png_set_rgb_to_gray (byval png_ptr as png_structp, byval error_action as integer, byval red as double, byval green as double)
declare sub png_set_rgb_to_gray_fixed (byval png_ptr as png_structp, byval error_action as integer, byval red as png_fixed_point, byval green as png_fixed_point)
declare function png_get_rgb_to_gray_status (byval png_ptr as png_structp) as png_byte
declare sub png_build_grayscale_palette (byval bit_depth as integer, byval palette as png_colorp)
declare sub png_set_strip_alpha (byval png_ptr as png_structp)
declare sub png_set_swap_alpha (byval png_ptr as png_structp)
declare sub png_set_invert_alpha (byval png_ptr as png_structp)
declare sub png_set_filler (byval png_ptr as png_structp, byval filler as png_uint_32, byval flags as integer)
declare sub png_set_add_alpha (byval png_ptr as png_structp, byval filler as png_uint_32, byval flags as integer)
declare sub png_set_swap (byval png_ptr as png_structp)
declare sub png_set_packing (byval png_ptr as png_structp)
declare sub png_set_packswap (byval png_ptr as png_structp)
declare sub png_set_shift (byval png_ptr as png_structp, byval true_bits as png_color_8p)
declare function png_set_interlace_handling (byval png_ptr as png_structp) as integer
declare sub png_set_invert_mono (byval png_ptr as png_structp)
declare sub png_set_background (byval png_ptr as png_structp, byval background_color as png_color_16p, byval background_gamma_code as integer, byval need_expand as integer, byval background_gamma as double)
declare sub png_set_strip_16 (byval png_ptr as png_structp)
declare sub png_set_dither (byval png_ptr as png_structp, byval palette as png_colorp, byval num_palette as integer, byval maximum_colors as integer, byval histogram as png_uint_16p, byval full_dither as integer)
declare sub png_set_gamma (byval png_ptr as png_structp, byval screen_gamma as double, byval default_file_gamma as double)
declare sub png_permit_empty_plte (byval png_ptr as png_structp, byval empty_plte_permitted as integer)
declare sub png_set_flush (byval png_ptr as png_structp, byval nrows as integer)
declare sub png_write_flush (byval png_ptr as png_structp)
declare sub png_start_read_image (byval png_ptr as png_structp)
declare sub png_read_update_info (byval png_ptr as png_structp, byval info_ptr as png_infop)
declare sub png_read_rows (byval png_ptr as png_structp, byval row as png_bytepp, byval display_row as png_bytepp, byval num_rows as png_uint_32)
declare sub png_read_row (byval png_ptr as png_structp, byval row as png_bytep, byval display_row as png_bytep)
declare sub png_read_image (byval png_ptr as png_structp, byval image as png_bytepp)
declare sub png_write_row (byval png_ptr as png_structp, byval row as png_bytep)
declare sub png_write_rows (byval png_ptr as png_structp, byval row as png_bytepp, byval num_rows as png_uint_32)
declare sub png_write_image (byval png_ptr as png_structp, byval image as png_bytepp)
declare sub png_write_end (byval png_ptr as png_structp, byval info_ptr as png_infop)
declare sub png_read_end (byval png_ptr as png_structp, byval info_ptr as png_infop)
declare sub png_destroy_info_struct (byval png_ptr as png_structp, byval info_ptr_ptr as png_infopp)
declare sub png_destroy_read_struct (byval png_ptr_ptr as png_structpp, byval info_ptr_ptr as png_infopp, byval end_info_ptr_ptr as png_infopp)
declare sub png_read_destroy (byval png_ptr as png_structp, byval info_ptr as png_infop, byval end_info_ptr as png_infop)
declare sub png_destroy_write_struct (byval png_ptr_ptr as png_structpp, byval info_ptr_ptr as png_infopp)
declare sub png_write_destroy (byval png_ptr as png_structp)
declare sub png_set_crc_action (byval png_ptr as png_structp, byval crit_action as integer, byval ancil_action as integer)
declare sub png_set_filter (byval png_ptr as png_structp, byval method as integer, byval filters as integer)
declare sub png_set_filter_heuristics (byval png_ptr as png_structp, byval heuristic_method as integer, byval num_weights as integer, byval filter_weights as png_doublep, byval filter_costs as png_doublep)
declare sub png_set_compression_level (byval png_ptr as png_structp, byval level as integer)
declare sub png_set_compression_mem_level (byval png_ptr as png_structp, byval mem_level as integer)
declare sub png_set_compression_strategy (byval png_ptr as png_structp, byval strategy as integer)
declare sub png_set_compression_window_bits (byval png_ptr as png_structp, byval window_bits as integer)
declare sub png_set_compression_method (byval png_ptr as png_structp, byval method as integer)
declare sub png_init_io (byval png_ptr as png_structp, byval fp as png_FILE_p)
declare sub png_set_error_fn (byval png_ptr as png_structp, byval error_ptr as png_voidp, byval error_fn as png_error_ptr, byval warning_fn as png_error_ptr)
declare function png_get_error_ptr (byval png_ptr as png_structp) as png_voidp
declare sub png_set_write_fn (byval png_ptr as png_structp, byval io_ptr as png_voidp, byval write_data_fn as png_rw_ptr, byval output_flush_fn as png_flush_ptr)
declare sub png_set_read_fn (byval png_ptr as png_structp, byval io_ptr as png_voidp, byval read_data_fn as png_rw_ptr)
declare function png_get_io_ptr (byval png_ptr as png_structp) as png_voidp
declare sub png_set_read_status_fn (byval png_ptr as png_structp, byval read_row_fn as png_read_status_ptr)
declare sub png_set_write_status_fn (byval png_ptr as png_structp, byval write_row_fn as png_write_status_ptr)
declare sub png_set_mem_fn (byval png_ptr as png_structp, byval mem_ptr as png_voidp, byval malloc_fn as png_malloc_ptr, byval free_fn as png_free_ptr)
declare function png_get_mem_ptr (byval png_ptr as png_structp) as png_voidp
declare sub png_set_read_user_transform_fn (byval png_ptr as png_structp, byval read_user_transform_fn as png_user_transform_ptr)
declare sub png_set_write_user_transform_fn (byval png_ptr as png_structp, byval write_user_transform_fn as png_user_transform_ptr)
declare sub png_set_user_transform_info (byval png_ptr as png_structp, byval user_transform_ptr as png_voidp, byval user_transform_depth as integer, byval user_transform_channels as integer)
declare function png_get_user_transform_ptr (byval png_ptr as png_structp) as png_voidp
declare sub png_set_read_user_chunk_fn (byval png_ptr as png_structp, byval user_chunk_ptr as png_voidp, byval read_user_chunk_fn as png_user_chunk_ptr)
declare function png_get_user_chunk_ptr (byval png_ptr as png_structp) as png_voidp
declare sub png_set_progressive_read_fn (byval png_ptr as png_structp, byval progressive_ptr as png_voidp, byval info_fn as png_progressive_info_ptr, byval row_fn as png_progressive_row_ptr, byval end_fn as png_progressive_end_ptr)
declare function png_get_progressive_ptr (byval png_ptr as png_structp) as png_voidp
declare sub png_process_data (byval png_ptr as png_structp, byval info_ptr as png_infop, byval buffer as png_bytep, byval buffer_size as png_size_t)
declare sub png_progressive_combine_row (byval png_ptr as png_structp, byval old_row as png_bytep, byval new_row as png_bytep)
declare function png_malloc (byval png_ptr as png_structp, byval size as png_uint_32) as png_voidp
declare function png_malloc_warn (byval png_ptr as png_structp, byval size as png_uint_32) as png_voidp
declare sub png_free (byval png_ptr as png_structp, byval ptr as png_voidp)
declare sub png_free_data (byval png_ptr as png_structp, byval info_ptr as png_infop, byval free_me as png_uint_32, byval num as integer)
declare sub png_data_freer (byval png_ptr as png_structp, byval info_ptr as png_infop, byval freer as integer, byval mask as png_uint_32)
declare function png_malloc_default (byval png_ptr as png_structp, byval size as png_uint_32) as png_voidp
declare sub png_free_default (byval png_ptr as png_structp, byval ptr as png_voidp)
declare function png_memcpy_check (byval png_ptr as png_structp, byval s1 as png_voidp, byval s2 as png_voidp, byval size as png_uint_32) as png_voidp
declare function png_memset_check (byval png_ptr as png_structp, byval s1 as png_voidp, byval value as integer, byval size as png_uint_32) as png_voidp
declare sub png_error (byval png_ptr as png_structp, byval error_message as png_const_charp)
declare sub png_chunk_error (byval png_ptr as png_structp, byval error_message as png_const_charp)
declare sub png_warning (byval png_ptr as png_structp, byval warning_message as png_const_charp)
declare sub png_chunk_warning (byval png_ptr as png_structp, byval warning_message as png_const_charp)
declare function png_get_valid (byval png_ptr as png_structp, byval info_ptr as png_infop, byval flag as png_uint_32) as png_uint_32
declare function png_get_rowbytes (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_uint_32
declare function png_get_rows (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_bytepp
declare sub png_set_rows (byval png_ptr as png_structp, byval info_ptr as png_infop, byval row_pointers as png_bytepp)
declare function png_get_channels (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_byte
declare function png_get_image_width (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_uint_32
declare function png_get_image_height (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_uint_32
declare function png_get_bit_depth (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_byte
declare function png_get_color_type (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_byte
declare function png_get_filter_type (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_byte
declare function png_get_interlace_type (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_byte
declare function png_get_compression_type (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_byte
declare function png_get_pixels_per_meter (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_uint_32
declare function png_get_x_pixels_per_meter (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_uint_32
declare function png_get_y_pixels_per_meter (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_uint_32
declare function png_get_pixel_aspect_ratio (byval png_ptr as png_structp, byval info_ptr as png_infop) as single
declare function png_get_x_offset_pixels (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_int_32
declare function png_get_y_offset_pixels (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_int_32
declare function png_get_x_offset_microns (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_int_32
declare function png_get_y_offset_microns (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_int_32
declare function png_get_signature (byval png_ptr as png_structp, byval info_ptr as png_infop) as png_bytep
declare function png_get_bKGD (byval png_ptr as png_structp, byval info_ptr as png_infop, byval background as png_color_16p ptr) as png_uint_32
declare sub png_set_bKGD (byval png_ptr as png_structp, byval info_ptr as png_infop, byval background as png_color_16p)
declare function png_get_cHRM (byval png_ptr as png_structp, byval info_ptr as png_infop, byval white_x as double ptr, byval white_y as double ptr, byval red_x as double ptr, byval red_y as double ptr, byval green_x as double ptr, byval green_y as double ptr, byval blue_x as double ptr, byval blue_y as double ptr) as png_uint_32
declare function png_get_cHRM_fixed (byval png_ptr as png_structp, byval info_ptr as png_infop, byval int_white_x as png_fixed_point ptr, byval int_white_y as png_fixed_point ptr, byval int_red_x as png_fixed_point ptr, byval int_red_y as png_fixed_point ptr, byval int_green_x as png_fixed_point ptr, byval int_green_y as png_fixed_point ptr, byval int_blue_x as png_fixed_point ptr, byval int_blue_y as png_fixed_point ptr) as png_uint_32
declare sub png_set_cHRM (byval png_ptr as png_structp, byval info_ptr as png_infop, byval white_x as double, byval white_y as double, byval red_x as double, byval red_y as double, byval green_x as double, byval green_y as double, byval blue_x as double, byval blue_y as double)
declare sub png_set_cHRM_fixed (byval png_ptr as png_structp, byval info_ptr as png_infop, byval int_white_x as png_fixed_point, byval int_white_y as png_fixed_point, byval int_red_x as png_fixed_point, byval int_red_y as png_fixed_point, byval int_green_x as png_fixed_point, byval int_green_y as png_fixed_point, byval int_blue_x as png_fixed_point, byval int_blue_y as png_fixed_point)
declare function png_get_gAMA (byval png_ptr as png_structp, byval info_ptr as png_infop, byval file_gamma as double ptr) as png_uint_32
declare function png_get_gAMA_fixed (byval png_ptr as png_structp, byval info_ptr as png_infop, byval int_file_gamma as png_fixed_point ptr) as png_uint_32
declare sub png_set_gAMA (byval png_ptr as png_structp, byval info_ptr as png_infop, byval file_gamma as double)
declare sub png_set_gAMA_fixed (byval png_ptr as png_structp, byval info_ptr as png_infop, byval int_file_gamma as png_fixed_point)
declare function png_get_hIST (byval png_ptr as png_structp, byval info_ptr as png_infop, byval hist as png_uint_16p ptr) as png_uint_32
declare sub png_set_hIST (byval png_ptr as png_structp, byval info_ptr as png_infop, byval hist as png_uint_16p)
declare function png_get_IHDR (byval png_ptr as png_structp, byval info_ptr as png_infop, byval width as png_uint_32 ptr, byval height as png_uint_32 ptr, byval bit_depth as integer ptr, byval color_type as integer ptr, byval interlace_method as integer ptr, byval compression_method as integer ptr, byval filter_method as integer ptr) as png_uint_32
declare sub png_set_IHDR (byval png_ptr as png_structp, byval info_ptr as png_infop, byval width as png_uint_32, byval height as png_uint_32, byval bit_depth as integer, byval color_type as integer, byval interlace_method as integer, byval compression_method as integer, byval filter_method as integer)
declare function png_get_oFFs (byval png_ptr as png_structp, byval info_ptr as png_infop, byval offset_x as png_int_32 ptr, byval offset_y as png_int_32 ptr, byval unit_type as integer ptr) as png_uint_32
declare sub png_set_oFFs (byval png_ptr as png_structp, byval info_ptr as png_infop, byval offset_x as png_int_32, byval offset_y as png_int_32, byval unit_type as integer)
declare function png_get_pCAL (byval png_ptr as png_structp, byval info_ptr as png_infop, byval purpose as png_charp ptr, byval X0 as png_int_32 ptr, byval X1 as png_int_32 ptr, byval type as integer ptr, byval nparams as integer ptr, byval units as png_charp ptr, byval params as png_charpp ptr) as png_uint_32
declare sub png_set_pCAL (byval png_ptr as png_structp, byval info_ptr as png_infop, byval purpose as png_charp, byval X0 as png_int_32, byval X1 as png_int_32, byval type as integer, byval nparams as integer, byval units as png_charp, byval params as png_charpp)
declare function png_get_pHYs (byval png_ptr as png_structp, byval info_ptr as png_infop, byval res_x as png_uint_32 ptr, byval res_y as png_uint_32 ptr, byval unit_type as integer ptr) as png_uint_32
declare sub png_set_pHYs (byval png_ptr as png_structp, byval info_ptr as png_infop, byval res_x as png_uint_32, byval res_y as png_uint_32, byval unit_type as integer)
declare function png_get_PLTE (byval png_ptr as png_structp, byval info_ptr as png_infop, byval palette as png_colorp ptr, byval num_palette as integer ptr) as png_uint_32
declare sub png_set_PLTE (byval png_ptr as png_structp, byval info_ptr as png_infop, byval palette as png_colorp, byval num_palette as integer)
declare function png_get_sBIT (byval png_ptr as png_structp, byval info_ptr as png_infop, byval sig_bit as png_color_8p ptr) as png_uint_32
declare sub png_set_sBIT (byval png_ptr as png_structp, byval info_ptr as png_infop, byval sig_bit as png_color_8p)
declare function png_get_sRGB (byval png_ptr as png_structp, byval info_ptr as png_infop, byval intent as integer ptr) as png_uint_32
declare sub png_set_sRGB (byval png_ptr as png_structp, byval info_ptr as png_infop, byval intent as integer)
declare sub png_set_sRGB_gAMA_and_cHRM (byval png_ptr as png_structp, byval info_ptr as png_infop, byval intent as integer)
declare function png_get_iCCP (byval png_ptr as png_structp, byval info_ptr as png_infop, byval name as png_charpp, byval compression_type as integer ptr, byval profile as png_charpp, byval proflen as png_uint_32 ptr) as png_uint_32
declare sub png_set_iCCP (byval png_ptr as png_structp, byval info_ptr as png_infop, byval name as png_charp, byval compression_type as integer, byval profile as png_charp, byval proflen as png_uint_32)
declare function png_get_sPLT (byval png_ptr as png_structp, byval info_ptr as png_infop, byval entries as png_sPLT_tpp) as png_uint_32
declare sub png_set_sPLT (byval png_ptr as png_structp, byval info_ptr as png_infop, byval entries as png_sPLT_tp, byval nentries as integer)
declare function png_get_text (byval png_ptr as png_structp, byval info_ptr as png_infop, byval text_ptr as png_textp ptr, byval num_text as integer ptr) as png_uint_32
declare sub png_set_text (byval png_ptr as png_structp, byval info_ptr as png_infop, byval text_ptr as png_textp, byval num_text as integer)
declare function png_get_tIME (byval png_ptr as png_structp, byval info_ptr as png_infop, byval mod_time as png_timep ptr) as png_uint_32
declare sub png_set_tIME (byval png_ptr as png_structp, byval info_ptr as png_infop, byval mod_time as png_timep)
declare function png_get_tRNS (byval png_ptr as png_structp, byval info_ptr as png_infop, byval trans as png_bytep ptr, byval num_trans as integer ptr, byval trans_values as png_color_16p ptr) as png_uint_32
declare sub png_set_tRNS (byval png_ptr as png_structp, byval info_ptr as png_infop, byval trans as png_bytep, byval num_trans as integer, byval trans_values as png_color_16p)
declare function png_get_sCAL (byval png_ptr as png_structp, byval info_ptr as png_infop, byval unit as integer ptr, byval width as double ptr, byval height as double ptr) as png_uint_32
declare sub png_set_sCAL (byval png_ptr as png_structp, byval info_ptr as png_infop, byval unit as integer, byval width as double, byval height as double)
declare sub png_set_sCAL_s (byval png_ptr as png_structp, byval info_ptr as png_infop, byval unit as integer, byval swidth as png_charp, byval sheight as png_charp)
declare sub png_set_keep_unknown_chunks (byval png_ptr as png_structp, byval keep as integer, byval chunk_list as png_bytep, byval num_chunks as integer)
declare sub png_set_unknown_chunks (byval png_ptr as png_structp, byval info_ptr as png_infop, byval unknowns as png_unknown_chunkp, byval num_unknowns as integer)
declare sub png_set_unknown_chunk_location (byval png_ptr as png_structp, byval info_ptr as png_infop, byval chunk as integer, byval location as integer)
declare function png_get_unknown_chunks (byval png_ptr as png_structp, byval info_ptr as png_infop, byval entries as png_unknown_chunkpp) as png_uint_32
declare function png_handle_as_unknown (byval png_ptr as png_structp, byval chunk_name as png_bytep) as integer
declare sub png_set_invalid (byval png_ptr as png_structp, byval info_ptr as png_infop, byval mask as integer)
declare sub png_read_png (byval png_ptr as png_structp, byval info_ptr as png_infop, byval transforms as integer, byval params as png_voidp)
declare sub png_write_png (byval png_ptr as png_structp, byval info_ptr as png_infop, byval transforms as integer, byval params as png_voidp)
declare function png_sig_bytes () as png_bytep
declare function png_get_copyright (byval png_ptr as png_structp) as png_charp
declare function png_get_header_ver (byval png_ptr as png_structp) as png_charp
declare function png_get_header_version (byval png_ptr as png_structp) as png_charp
declare function png_get_libpng_ver (byval png_ptr as png_structp) as png_charp
declare function png_permit_mng_features (byval png_ptr as png_structp, byval mng_features_permitted as png_uint_32) as png_uint_32
declare function png_get_mmx_flagmask (byval flag_select as integer, byval compilerID as integer ptr) as png_uint_32
declare function png_get_asm_flagmask (byval flag_select as integer) as png_uint_32
declare function png_get_asm_flags (byval png_ptr as png_structp) as png_uint_32
declare function png_get_mmx_bitdepth_threshold (byval png_ptr as png_structp) as png_byte
declare function png_get_mmx_rowbytes_threshold (byval png_ptr as png_structp) as png_uint_32
declare sub png_set_asm_flags (byval png_ptr as png_structp, byval asm_flags as png_uint_32)
declare sub png_set_mmx_thresholds (byval png_ptr as png_structp, byval mmx_bitdepth_threshold as png_byte, byval mmx_rowbytes_threshold as png_uint_32)
declare function png_mmx_support () as integer
declare sub png_set_strip_error_numbers (byval png_ptr as png_structp, byval strip_mode as png_uint_32)
declare sub png_set_user_limits (byval png_ptr as png_structp, byval user_width_max as png_uint_32, byval user_height_max as png_uint_32)
declare function png_get_user_width_max (byval png_ptr as png_structp) as png_uint_32
declare function png_get_user_height_max (byval png_ptr as png_structp) as png_uint_32
declare function png_get_uint_32 (byval buf as png_bytep) as png_uint_32
declare function png_get_uint_16 (byval buf as png_bytep) as png_uint_16
declare function png_get_int_32 (byval buf as png_bytep) as png_int_32
declare function png_get_uint_31 (byval png_ptr as png_structp, byval buf as png_bytep) as png_uint_32
declare sub png_save_uint_32 (byval buf as png_bytep, byval i as png_uint_32)
declare sub png_save_int_32 (byval buf as png_bytep, byval i as png_int_32)
declare sub png_save_uint_16 (byval buf as png_bytep, byval i as uinteger)
end extern

#define PNG_COLOR_DIST(c1, c2) (abs(cint(c1.red) - cint(c2.red)) + abs(cint(c1.green) - cint(c2.green)) + abs(cint(c1.blue) - cint(c2.blue)))

#define PNG_ROWBYTES(pixel_bits, width_) iif( (pixel_bits) >= 8, _
    	 									  (width_) * (cast(png_uint_32,pixel_bits) shr 3), _
    	 									  (width_) * ((cast(png_uint_32,pixel_bits) + 7) shr 3) )

#define PNG_OUT_OF_RANGE(value, ideal, delta) ( (value) < (ideal)-(delta) or (value) > (ideal)+(delta) )

#define PNG_IHDR_ dim as png_byte png_IHDR(0 to 5-1) = { 73,  72,  68,  82, 0}
#define PNG_IDAT_ dim as png_byte png_IDAT(0 to 5-1) = { 73,  68,  65,  84, 0}
#define PNG_IEND_ dim as png_byte png_IEND(0 to 5-1) = { 73,  69,  78,  68, 0}
#define PNG_PLTE_ dim as png_byte png_PLTE(0 to 5-1) = { 80,  76,  84,  69, 0}
#define PNG_bKGD_ dim as png_byte png_bKGD(0 to 5-1) = { 98,  75,  71,  68, 0}
#define PNG_cHRM_ dim as png_byte png_cHRM(0 to 5-1) = { 99,  72,  82,  77, 0}
#define PNG_gAMA_ dim as png_byte png_gAMA(0 to 5-1) = {103,  65,  77,  65, 0}
#define PNG_hIST_ dim as png_byte png_hIST(0 to 5-1) = {104,  73,  83,  84, 0}
#define PNG_iCCP_ dim as png_byte png_iCCP(0 to 5-1) = {105,  67,  67,  80, 0}
#define PNG_iTXt_ dim as png_byte png_iTXt(0 to 5-1) = {105,  84,  88, 116, 0}
#define PNG_oFFs_ dim as png_byte png_oFFs(0 to 5-1) = {111,  70,  70, 115, 0}
#define PNG_pCAL_ dim as png_byte png_pCAL(0 to 5-1) = {112,  67,  65,  76, 0}
#define PNG_sCAL_ dim as png_byte png_sCAL(0 to 5-1) = {115,  67,  65,  76, 0}
#define PNG_pHYs_ dim as png_byte png_pHYs(0 to 5-1) = {112,  72,  89, 115, 0}
#define PNG_sBIT_ dim as png_byte png_sBIT(0 to 5-1) = {115,  66,  73,  84, 0}
#define PNG_sPLT_ dim as png_byte png_sPLT(0 to 5-1) = {115,  80,  76,  84, 0}
#define PNG_sRGB_ dim as png_byte png_sRGB(0 to 5-1) = {115,  82,  71,  66, 0}
#define PNG_tEXt_ dim as png_byte png_tEXt(0 to 5-1) = {116,  69,  88, 116, 0}
#define PNG_tIME_ dim as png_byte png_tIME(0 to 5-1) = {116,  73,  77,  69, 0}
#define PNG_tRNS_ dim as png_byte png_tRNS(0 to 5-1) = {116,  82,  78,  83, 0}
#define PNG_zTXt_ dim as png_byte png_zTXt(0 to 5-1) = {122,  84,  88, 116, 0}

#endif
