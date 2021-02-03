------------------------------------------------------------------------
r5581 | dkls | 2011-09-14 07:46:33 -0700 (Wed, 14 Sep 2011) | 1 line

Note: This repository is no longer used, fbc moved to Git
------------------------------------------------------------------------
r5580 | whotookcha0s-_- | 2011-06-27 18:44:21 -0700 (Mon, 27 Jun 2011) | 3 lines

Changed data arrays to var-len.


------------------------------------------------------------------------
r5579 | counting_pine | 2011-06-27 07:49:19 -0700 (Mon, 27 Jun 2011) | 3 lines

Close #0 now returns an error instead of closing all files.
Previously Close (no params) performed Close #0 internally, now it just calls a newly-created, special CloseAll function.
Unit test added.
------------------------------------------------------------------------
r5578 | counting_pine | 2011-06-25 01:39:18 -0700 (Sat, 25 Jun 2011) | 2 lines

Fix for #3327533: Internal compiler error trying to parse non-integer bounds in array declarations.
Plus memory leak avoidance in a similar previous fix (r5210 for #2533376)
------------------------------------------------------------------------
r5577 | counting_pine | 2011-06-14 08:12:43 -0700 (Tue, 14 Jun 2011) | 5 lines

Better (but still suboptimal) fix for #3153953
- Covers more (all?) cases of the bug
- Previous suspect hOptConstDistMUL() re-enabled
- New unit test added
- (Relevant code section disabled, not fixed.  TODO: add "proper" fix - need better understanding of algorithm)
------------------------------------------------------------------------
r5576 | dkls | 2011-05-21 10:12:30 -0700 (Sat, 21 May 2011) | 1 line

.deb's updated: Better lintian compliance, and separate amd64 packages for Debian & Ubuntu (Debian one relies on ia32-libs-dev, while Ubuntu doesn't have that and so getlibs is used instead)
------------------------------------------------------------------------
r5575 | dkls | 2011-05-16 06:01:02 -0700 (Mon, 16 May 2011) | 1 line

Fixed: bitfields passed to BYREF parameters didn't cause an error
------------------------------------------------------------------------
r5574 | dkls | 2011-05-16 05:40:55 -0700 (Mon, 16 May 2011) | 1 line

(ditto)
------------------------------------------------------------------------
r5573 | dkls | 2011-05-16 05:39:50 -0700 (Mon, 16 May 2011) | 1 line

Bumped to 0.23.0
------------------------------------------------------------------------
r5565 | dkls | 2011-05-07 09:36:57 -0700 (Sat, 07 May 2011) | 1 line

Fixed .deb installation path
------------------------------------------------------------------------
r5564 | dkls | 2011-05-06 14:17:02 -0700 (Fri, 06 May 2011) | 3 lines

Improved .deb creation: can now build both i386 and amd64 .deb's; the amd64
one uses getlibs to download and install needed 32bit x11 libraries.

------------------------------------------------------------------------
r5561 | dkls | 2011-05-06 05:41:55 -0700 (Fri, 06 May 2011) | 1 line

inc/GL/glext.bi: GL{int|sizei}ptr[ARB] typedefs should be integers, not pointers
------------------------------------------------------------------------
r5560 | dkls | 2011-05-06 04:35:18 -0700 (Fri, 06 May 2011) | 1 line

inc/crt/stdint.bi: uint64_t wasn't unsigned
------------------------------------------------------------------------
r5558 | dkls | 2011-05-06 04:11:38 -0700 (Fri, 06 May 2011) | 1 line

Remove symbIsChildOf(), so PROTECTED members are handled the same way as PRIVATE members for now
------------------------------------------------------------------------
r5557 | dkls | 2011-05-03 13:10:36 -0700 (Tue, 03 May 2011) | 1 line

The rtlib's QB STR$() for ULONGINTs didn't prepend a space except on Windows
------------------------------------------------------------------------
r5556 | counting_pine | 2011-05-03 06:20:39 -0700 (Tue, 03 May 2011) | 2 lines

Unit tests for guarding against accessing private/protected fields outside the type (based on const_member.bas / const_member2.bas).
No fix submitted as yet.  See www.freebasic.net/forum/viewtopic.php?p=156718#156718 for more info.
------------------------------------------------------------------------
r5555 | dkls | 2011-05-02 11:58:33 -0700 (Mon, 02 May 2011) | 1 line

Update with changes from 0.22 branch (new examples/manifests/wiki files/installer etc.)
------------------------------------------------------------------------
r5544 | dkls | 2011-04-29 13:55:20 -0700 (Fri, 29 Apr 2011) | 1 line

Updated the manifests
------------------------------------------------------------------------
r5541 | dkls | 2011-04-28 20:55:56 -0700 (Thu, 28 Apr 2011) | 1 line

Updated wiki examples and cache
------------------------------------------------------------------------
r5540 | dkls | 2011-04-28 20:42:19 -0700 (Thu, 28 Apr 2011) | 1 line

Updated the copyright dates
------------------------------------------------------------------------
r5531 | dkls | 2011-04-20 12:39:02 -0700 (Wed, 20 Apr 2011) | 1 line

Fixed #3251750: Local class-like UDT FOR/NEXT iterators were being initialized with their default constructor (if they had one), in addition to the initialization to the FOR start value. Now only the latter is done.
------------------------------------------------------------------------
r5530 | counting_pine | 2011-04-20 11:12:33 -0700 (Wed, 20 Apr 2011) | 1 line

Add support for user-defined operators EXP/LOG/SIN/ASIN/COS/ACOS/TAN/ATN
------------------------------------------------------------------------
r5529 | dkls | 2011-04-20 09:40:41 -0700 (Wed, 20 Apr 2011) | 1 line

Made va_first() return an ANY PTR
------------------------------------------------------------------------
r5523 | dkls | 2011-04-18 09:30:28 -0700 (Mon, 18 Apr 2011) | 5 lines

Remove LOCAL attribute from array descriptor structs if the corresponding
descriptor symbol isn't local either, to avoid confusion in the C emitter.
(the symbol inherits the array's attributes, but the struct didn't, it was
made local by default when a scope was created during typeini)

------------------------------------------------------------------------
r5522 | dkls | 2011-04-18 05:35:22 -0700 (Mon, 18 Apr 2011) | 1 line

C emitter wasn't emitting namespace'd procedure declarations (r5431 regression)
------------------------------------------------------------------------
r5521 | dkls | 2011-04-18 02:52:12 -0700 (Mon, 18 Apr 2011) | 1 line

Previous commit broke nested calls in the C emitter, fix it by keeping track of recursion level in the static argument list (originally, the list was kept on stack in astLoadCALL)
------------------------------------------------------------------------
r5520 | dkls | 2011-04-17 17:16:32 -0700 (Sun, 17 Apr 2011) | 4 lines

Fixed #2973014: Partly revert changes to call emitting from r5309 (gcc emitter
merge), so that the asm backend pushes each call argument before the next
argument is flushed, as before.

------------------------------------------------------------------------
r5519 | dkls | 2011-04-16 16:02:43 -0700 (Sat, 16 Apr 2011) | 6 lines

Allow -1 to be used as array upper bound, at the cost of -2147483648 being
disallowed. The value has to be reserved to internally represent ellipsis in
array declarations. It's not quite a fix, but -1 hopefully is more useful than
the other. Also, the compiler will now complain immediately if the disallowed
value is specified, instead of thinking ellipsis were specified as upper bound.

------------------------------------------------------------------------
r5518 | dkls | 2011-04-16 13:56:45 -0700 (Sat, 16 Apr 2011) | 4 lines

Fixed: Symbols were being deleted without having been removed from backpatching
lists of forward references, causing the next symbol at the same address to
get its datatype corrupted when backpatching happened.

------------------------------------------------------------------------
r5517 | dkls | 2011-04-15 14:39:03 -0700 (Fri, 15 Apr 2011) | 1 line

astIsBO()/astIsUOP() macros: check node class -before- accessing the possibly unset data, instead of doing it afterwards, to fix valgrind complaints.
------------------------------------------------------------------------
r5516 | dkls | 2011-04-15 13:08:19 -0700 (Fri, 15 Apr 2011) | 1 line

Oops, revert accidentally committed change in rtlib
------------------------------------------------------------------------
r5515 | dkls | 2011-04-15 08:37:19 -0700 (Fri, 15 Apr 2011) | 23 lines

Allow stdcall to be used on DOS and Unix-like systems:

  - Don't emit stdcall as cdecl anymore (was left-over from fbc linux
    port? -nostdcall has long been removed)

  - Determine FBCALL based on the (cross)target, just like the rtlib, instead
    of always using "stdcall"

  - Change all rtlib procedure declarations (rtl-*.bas) that were using
    stdcall to use the pseudo FBCALL, it's then changed to the real target
    dependant value when the rtl decls are loaded.

  - Similarly: change cProcCallingConv() to use the target specific fbcall
    when the default is requested

  - Remap FB_FUNCMODE_STDCALL to FB_FUNCMODE_STDCALL_MS for DOS/Unix, so the
    @N suffix isn't added, like in GCC. GAS doesn't support it anyways except
    on MinGW/Cygwin.

Any procedure declarations using stdcall will actually be stdcall now, instead
of the default cdecl, and EXTERN "windows" and EXTERN "windows-ms" will be
equal (unlike Windows) and result in stdcall instead of cdecl.

------------------------------------------------------------------------
r5514 | dkls | 2011-04-15 07:20:23 -0700 (Fri, 15 Apr 2011) | 1 line

Testsuite's makefiles changed to use printf instead of echo -e, because for some reason GNU make on Linux uses it's own echo, which doesn't know -e, even though /bin/echo can handle it...
------------------------------------------------------------------------
r5513 | dkls | 2011-04-14 15:21:34 -0700 (Thu, 14 Apr 2011) | 1 line

(internal) Use rtlProfileCall_monstartup() instead of duplicating code
------------------------------------------------------------------------
r5511 | dkls | 2011-04-14 13:12:49 -0700 (Thu, 14 Apr 2011) | 1 line

Fixed mismatching declarations of fb_DevFileGetSize()
------------------------------------------------------------------------
r5507 | dkls | 2011-04-14 08:37:04 -0700 (Thu, 14 Apr 2011) | 1 line

aspell.bi: On Linux 'aspell' must be linked, instead of 'aspell-15'
------------------------------------------------------------------------
r5481 | dkls | 2011-04-06 15:58:37 -0700 (Wed, 06 Apr 2011) | 1 line

Prevent out-of-bounds array accesses when parsing ON ... GOTO and report an error when too many labels were specified.
------------------------------------------------------------------------
r5478 | v1ctor | 2011-04-03 07:11:03 -0700 (Sun, 03 Apr 2011) | 1 line

changed: inheritance (more WIP)
------------------------------------------------------------------------
r5477 | v1ctor | 2011-04-02 18:34:15 -0700 (Sat, 02 Apr 2011) | 1 line

changed: inheritance (more WIP)
------------------------------------------------------------------------
r5475 | v1ctor | 2011-04-02 18:13:02 -0700 (Sat, 02 Apr 2011) | 1 line

changed: inheritance (more WIP)
------------------------------------------------------------------------
r5474 | v1ctor | 2011-04-02 14:04:42 -0700 (Sat, 02 Apr 2011) | 1 line

changed: inheritance (more WIP)
------------------------------------------------------------------------
r5470 | v1ctor | 2011-03-29 14:53:48 -0700 (Tue, 29 Mar 2011) | 1 line

some inheritance tests/examples
------------------------------------------------------------------------
r5460 | dkls | 2011-03-24 13:58:24 -0700 (Thu, 24 Mar 2011) | 1 line

Fixed va_first() to make it take into account 4 byte parameter alignment; the address it returned was wrong when the last fixed parameter of the vararg function was something with sizeof() <> 4.
------------------------------------------------------------------------
r5459 | counting_pine | 2011-02-25 06:00:31 -0800 (Fri, 25 Feb 2011) | 3 lines

Fix/change: more intuitive/QB-like behavior for MID(s,n,l)=t when l=0 (was treating 0 as omitted, like -1).  Now sets 0 chars (i.e. leaves s unchanged).
- Fix inner code in fb_StrAssignMid() (change len<1 to len<0) but also add in len==0 short-circuit check, making first fix superfluous
- Add test case to check behaviours for combinations of n>=1 and l>=0
------------------------------------------------------------------------
r5458 | dkls | 2011-02-01 17:21:42 -0800 (Tue, 01 Feb 2011) | 1 line

SCREENRES's number of pages did not default to 1 anymore
------------------------------------------------------------------------
r5457 | counting_pine | 2011-01-21 15:42:38 -0800 (Fri, 21 Jan 2011) | 1 line

Fix for #1444576: WINDOWTITLE() does not persist
------------------------------------------------------------------------
r5456 | counting_pine | 2011-01-14 11:42:02 -0800 (Fri, 14 Jan 2011) | 1 line

Internal: Move astDumpTree() declare from ast-optimize.bas to ast.bi
------------------------------------------------------------------------
r5455 | counting_pine | 2011-01-14 11:31:04 -0800 (Fri, 14 Jan 2011) | 3 lines

Temporary fix for #3153953: (i - (j+C1) * C2 + k) was being optimised to (i - j * C2+ k + C1*C2), instead of (... - C1*C2).
- problem seems to be in hOptConstDistMUL(), disabled routine as a temporary fix
- unit test added
------------------------------------------------------------------------
r5454 | counting_pine | 2010-12-27 23:37:15 -0800 (Mon, 27 Dec 2010) | 2 lines

Changed: __FB_UNIX__ and __FB_PCOS__ are no longer defined when false, allowing #IFDEF to work
(Next release: make it a blank define (instead of -1) when true, so it functions exactly like other OS defines; also remove the #if/#elseif/#endif stuff)
------------------------------------------------------------------------
r5453 | counting_pine | 2010-12-27 23:18:28 -0800 (Mon, 27 Dec 2010) | 2 lines

Change __FB_PCOS__ / __FB_UNIX__ defines to be static (predefined, remove hDef*_cb functions)
- Should be no change in behaviour atm; both should still be defined as -1 or 0
------------------------------------------------------------------------
r5452 | counting_pine | 2010-12-20 00:22:16 -0800 (Mon, 20 Dec 2010) | 3 lines

patch #3108334: GetKey in graphics modes shouldn't wait 20ms if there is a key already in the key buffer (thanks to TeeEmCee for patch)

fb_GfxGetkey() now matches behaviour of win32 version of fb_ConsoleGetkey() in rtlib/win32/libfb_io_inkey.c (code doesn't compare as well on other platforms)
------------------------------------------------------------------------
r5451 | counting_pine | 2010-12-19 01:05:21 -0800 (Sun, 19 Dec 2010) | 2 lines

Fix: Expressions containing constants that differed by "small" amounts (e.g. a*1e-20, a*1e-30) could be assumed to be equal in comparisons.
- Fixed by making astIsTreeEqual() do strict checks on floating-point consts - I don't think an Epsilon makes sense there.
------------------------------------------------------------------------
r5450 | counting_pine | 2010-12-11 19:58:05 -0800 (Sat, 11 Dec 2010) | 4 lines

QB-compatibility fix: SCREEN modes now default to the same number of pages as QB instead of just 1 (e.g. SCREEN 7 now defaults to 8 pages).
This means that lang qb code like "SCREEN 7,, 0, 1" will work as expected
Values pulled from QBASIC help file, maximum values chosen where number is adaptor-dependent.

------------------------------------------------------------------------
r5449 | counting_pine | 2010-11-29 01:53:06 -0800 (Mon, 29 Nov 2010) | 5 lines

Fix for #2099245: mixing signed and unsigned operands (e.g. c1 + uv1) could result in the wrong result type if the first operand was a const and the second wasn't, due to an optimization that swapped the operands, which could affect the result's type.

- optimization disabled for now, but probably not too hard to fix.  Should probably be moved to ast-optimize.bas though.  Note: could also be extended to more commutative ops, e.g. (c AND v), (c = v), and adapted for inequality comparisions, e.g.  (c>=v) -> (v<=c)

- Unit test added for a+b cases with mixed const/var, signed/unsigned
------------------------------------------------------------------------
r5448 | dkls | 2010-11-15 08:31:04 -0800 (Mon, 15 Nov 2010) | 1 line

Fixed wrong file name in the error message if the output .asm/.c couldn't be opened
------------------------------------------------------------------------
r5447 | counting_pine | 2010-11-14 16:35:05 -0800 (Sun, 14 Nov 2010) | 1 line

Internal: more astIsUOP/astIsBOP macro usage in ast-optimize.bas
------------------------------------------------------------------------
r5446 | dkls | 2010-11-10 12:35:28 -0800 (Wed, 10 Nov 2010) | 1 line

C emitter: Reverted r5430 (don't emit addrof's for fixed-length string access) to fix #3106470
------------------------------------------------------------------------
r5445 | dkls | 2010-11-06 00:27:55 -0700 (Sat, 06 Nov 2010) | 1 line

Changelog update
------------------------------------------------------------------------
r5444 | dkls | 2010-11-06 00:26:47 -0700 (Sat, 06 Nov 2010) | 1 line

Fixed (C emitter): comparisons returned 1 and 0 instead of -1 and 0
------------------------------------------------------------------------
r5443 | dkls | 2010-11-06 00:09:11 -0700 (Sat, 06 Nov 2010) | 1 line

C emitter: fix array access again
------------------------------------------------------------------------
r5442 | dkls | 2010-11-05 22:43:41 -0700 (Fri, 05 Nov 2010) | 1 line

Added __FB_BACKEND__
------------------------------------------------------------------------
r5441 | dkls | 2010-11-05 22:31:45 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): string literals in var initializers
------------------------------------------------------------------------
r5440 | dkls | 2010-11-05 22:30:02 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed uninitialized variable access in hEscapeUCN() (C emitter's wstring literal escaper)
------------------------------------------------------------------------
r5439 | dkls | 2010-11-05 19:34:03 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): offsets (function pointers, string literals, array access, ...)
------------------------------------------------------------------------
r5438 | dkls | 2010-11-05 16:49:32 -0700 (Fri, 05 Nov 2010) | 1 line

C emitter: added "wchar" typedef for easier debugging
------------------------------------------------------------------------
r5437 | dkls | 2010-11-05 16:04:29 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): RTL functions returning "wstrings" (actually wchar pointers)
------------------------------------------------------------------------
r5436 | dkls | 2010-11-05 15:54:28 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): use a local variable instead of a #define for CALL vregs, because they may have side effects (problem observed with rtl wstr functions, actual problem may be somewhere else)
------------------------------------------------------------------------
r5435 | dkls | 2010-11-05 15:51:24 -0700 (Fri, 05 Nov 2010) | 1 line

C emitter: simplify vreg emitting code
------------------------------------------------------------------------
r5434 | dkls | 2010-11-05 15:42:38 -0700 (Fri, 05 Nov 2010) | 4 lines

Fixed (C emitter): debugging (-g)
- use // for debug-comments instead of /**/, because /**/ don't nest (and /**/ may be used in string literals in the input .bas, like in the C emitter itself...)
- prevent backslashes in debug-commented comments from escaping EOL
- escape backslashes in filenames in debug-#line's
------------------------------------------------------------------------
r5433 | dkls | 2010-11-05 15:32:21 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): number literals (add type suffixes, handle edge cases like INT_MIN, avoid GCC warnings)
------------------------------------------------------------------------
r5432 | dkls | 2010-11-05 15:20:15 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): addrof labels (used in code added by -exx)
------------------------------------------------------------------------
r5431 | dkls | 2010-11-05 15:17:40 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): emit procedure declarations before variables, so @func works in var initializers
------------------------------------------------------------------------
r5430 | dkls | 2010-11-05 15:14:30 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): don't emit addrof's (&) for fixed-length string access (just like arrays)
------------------------------------------------------------------------
r5429 | dkls | 2010-11-05 15:10:40 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): arrays/fixed-length string field/variable declarations (fixed length strings are now emitted as char arrays)
------------------------------------------------------------------------
r5428 | dkls | 2010-11-05 15:03:32 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): use temp names for typedefs instead of forward references, so the forward referenced structs will be mangled correctly
------------------------------------------------------------------------
r5427 | dkls | 2010-11-05 14:55:28 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): implicit casts in function pointer assignments
------------------------------------------------------------------------
r5426 | dkls | 2010-11-05 14:52:15 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): SWAPping pointers
------------------------------------------------------------------------
r5425 | dkls | 2010-11-05 14:46:54 -0700 (Fri, 05 Nov 2010) | 1 line

Fixed (C emitter): circular UDT dependencies
------------------------------------------------------------------------
r5424 | dkls | 2010-11-05 14:07:40 -0700 (Fri, 05 Nov 2010) | 1 line

Fix GCC warning about different pointer types
------------------------------------------------------------------------
r5423 | dkls | 2010-11-05 14:06:30 -0700 (Fri, 05 Nov 2010) | 1 line

Don't compare pointers against (the address of) string literals (fixes GCC warning)
------------------------------------------------------------------------
r5422 | dkls | 2010-11-05 14:04:55 -0700 (Fri, 05 Nov 2010) | 1 line

Remove usage of vararg functions (just one affected) from the compiler, because that's not supported with -gen gcc
------------------------------------------------------------------------
r5421 | dkls | 2010-11-03 12:48:20 -0700 (Wed, 03 Nov 2010) | 1 line

Fixed #3076871: typeof() was not preserving the data type length when used on operation expressions
------------------------------------------------------------------------
r5420 | dkls | 2010-10-28 13:00:42 -0700 (Thu, 28 Oct 2010) | 1 line

commctrl.bi: Only declare NMLVFINDITEMA #ifndef UNICODE
------------------------------------------------------------------------
r5418 | dkls | 2010-10-25 08:32:10 -0700 (Mon, 25 Oct 2010) | 5 lines

Forward reference back patching fixes
- Using typedefs and forward references, types with more than 8 PTR's could be created, this will now result in an error
- Forward references solving was faulty, e.g. a forward reference couldn't be solved with a function pointer or another forward reference
- Test cases added

------------------------------------------------------------------------
r5417 | dkls | 2010-10-25 08:30:34 -0700 (Mon, 25 Oct 2010) | 5 lines

Fixed typedef parsing:
- Multiple typedefs in the same line were not being accepted if each had its own type: 'type a as integer, b as integer'
- Could not typedef to a forward reference with multi-declaration syntax: 'type as Forward Foo'
- CONST's on PTR's weren't always preserved on typedefs to forward references
- Test cases added
------------------------------------------------------------------------
r5416 | counting_pine | 2010-10-25 00:24:55 -0700 (Mon, 25 Oct 2010) | 1 line

Internal: Add a couple of macros to simplify some checks in ast-optimize.bas
------------------------------------------------------------------------
r5415 | counting_pine | 2010-10-24 23:12:19 -0700 (Sun, 24 Oct 2010) | 1 line

Fixed #3089424: type parameter error and missing types in commctrl.bi (thanks to vbmrupp)
------------------------------------------------------------------------
r5414 | dkls | 2010-10-18 09:49:06 -0700 (Mon, 18 Oct 2010) | 1 line

Fixed #3089481: -pp didn't distinguish \" escapes from "" sequences
------------------------------------------------------------------------
r5413 | dkls | 2010-10-12 04:24:49 -0700 (Tue, 12 Oct 2010) | 1 line

Added variadic macros to the PP
------------------------------------------------------------------------
r5412 | dkls | 2010-10-12 01:46:23 -0700 (Tue, 12 Oct 2010) | 1 line

Updated examples/makefile for DevIL/Newton tests added before
------------------------------------------------------------------------
r5411 | dkls | 2010-10-12 01:44:32 -0700 (Tue, 12 Oct 2010) | 1 line

Renamed libfb_file_tree.c to libfb_file_free.c
------------------------------------------------------------------------
r5410 | dkls | 2010-10-12 01:37:45 -0700 (Tue, 12 Oct 2010) | 1 line

Fixed: rtlib allocated room for N user file numbers but only allowed N-1 to be used
------------------------------------------------------------------------
r5409 | counting_pine | 2010-10-10 22:08:22 -0700 (Sun, 10 Oct 2010) | 2 lines

Fixed: SCREEN(x,y) and VIEW|WINDOW SCREEN weren't recognised in lang QB (it was necessary to use __SCREEN)
- Problem introduced in r5238 when TK_SCREENQB was added for lang QB usage (thereby enabling TK_SCREEN to be used as __screen) but TK_SCREENQB was only recognised for the SCREEN statement, meaning __screen needed to be used elsewhere.  Now both are allowed in those places.
------------------------------------------------------------------------
r5408 | dkls | 2010-09-26 13:01:01 -0700 (Sun, 26 Sep 2010) | 1 line

Added examples for DevIL & Newton libraries (useful to test the import libs)
------------------------------------------------------------------------
r5407 | dkls | 2010-09-26 12:57:37 -0700 (Sun, 26 Sep 2010) | 4 lines

tram2:
- dos: could be using the wrong ar, because rtlib/gfxlib's configure don't forward the AR variable (now passing it to make instead)
- win32/dos: now generates .zip's directly, instead of .tar.lzma's
- no longer waits for user input (better for automation)
------------------------------------------------------------------------
r5406 | counting_pine | 2010-09-20 01:12:49 -0700 (Mon, 20 Sep 2010) | 2 lines

- FRAC(n) was returning n for Integer-types when it should have been returning 0.
- Make FIX/INT leave integer values/types unchanged to prevent inefficiencies or possible precision losses.
------------------------------------------------------------------------
r5405 | counting_pine | 2010-09-20 00:18:30 -0700 (Mon, 20 Sep 2010) | 1 line

Fix const-folding bug: SGN(const_longint) was leaving the upper 32 bits unchanged
------------------------------------------------------------------------
r5404 | counting_pine | 2010-09-12 02:39:40 -0700 (Sun, 12 Sep 2010) | 1 line

- Disable "No newline at end of file" warning unless in -w pedantic
------------------------------------------------------------------------
r5403 | dkls | 2010-09-09 11:51:06 -0700 (Thu, 09 Sep 2010) | 1 line

Some vfw functions need to use cdecl instead of stdcall; added #inclib "vfw32"
------------------------------------------------------------------------
r5402 | dkls | 2010-08-26 21:04:04 -0700 (Thu, 26 Aug 2010) | 1 line

Added .deb creation script (src/contrib/deb), updated tram2 to use it
------------------------------------------------------------------------
r5401 | dkls | 2010-08-26 18:41:15 -0700 (Thu, 26 Aug 2010) | 3 lines

- Moved win32 installer creation code out of tram2, so it can be used separately
- Added back start-shell tool
- Installer now optionally creates a "FreeBASIC shell here" Explorer context menu entry
------------------------------------------------------------------------
r5400 | dkls | 2010-08-25 07:17:48 -0700 (Wed, 25 Aug 2010) | 1 line

Fixed: #3052712: -pp was not emitting the last line of input if it didn't have an EOL marker
------------------------------------------------------------------------
r5397 | dkls | 2010-08-11 09:31:59 -0700 (Wed, 11 Aug 2010) | 3 lines

tram2 linux:
- Fixed: Normal non-standalone release was being linked against whatever libs were in /usr/local, instead of the libs from the dev tree.
- Still need to compile binutils, a) to work on 64bit, b) to avoid linking against a shared libbfd and c) to avoid libbfd's zlib dependency
------------------------------------------------------------------------
r5395 | dkls | 2010-08-10 14:59:18 -0700 (Tue, 10 Aug 2010) | 1 line

Added some missing FB_UNLOCK()'s to the rtlib
------------------------------------------------------------------------
r5394 | dkls | 2010-08-10 14:29:11 -0700 (Tue, 10 Aug 2010) | 1 line

Updated to 0.21.1
------------------------------------------------------------------------
r5391 | dkls | 2010-08-10 14:04:43 -0700 (Tue, 10 Aug 2010) | 1 line

Restored accidentally deleted changelog entry
------------------------------------------------------------------------
r5390 | dkls | 2010-08-10 13:28:17 -0700 (Tue, 10 Aug 2010) | 1 line

tram2: Compiling binutils isn't necessary anymore
------------------------------------------------------------------------
r5389 | dkls | 2010-08-10 13:04:09 -0700 (Tue, 10 Aug 2010) | 1 line

tram2: Use binutils from previous release instead of new ones
------------------------------------------------------------------------
r5388 | dkls | 2010-08-10 12:57:06 -0700 (Tue, 10 Aug 2010) | 1 line

Removed the "v" and "b" from the version number in __FB_SIGNATURE__, following the release archive names
------------------------------------------------------------------------
r5387 | dkls | 2010-08-03 15:01:54 -0700 (Tue, 03 Aug 2010) | 1 line

Fixed - gfxlib's x11 window didn't detect the mouse being outside the window sometimes because it checked only the mouse's y position. 
------------------------------------------------------------------------
r5386 | counting_pine | 2010-07-28 07:33:11 -0700 (Wed, 28 Jul 2010) | 1 line

Bug #3034516 - remove trailing backslash from #macro line
------------------------------------------------------------------------
r5384 | dkls | 2010-07-24 06:11:55 -0700 (Sat, 24 Jul 2010) | 1 line

tram2: Nicer title for source archive
------------------------------------------------------------------------
r5382 | dkls | 2010-07-16 20:27:11 -0700 (Fri, 16 Jul 2010) | 1 line

tram2 dos: Copy libc.a instead of crt0.o from FB 0.20
------------------------------------------------------------------------
r5380 | dkls | 2010-07-16 10:29:31 -0700 (Fri, 16 Jul 2010) | 1 line

Manifest update
------------------------------------------------------------------------
r5378 | dkls | 2010-07-16 10:11:46 -0700 (Fri, 16 Jul 2010) | 1 line

tram2: Use dlltool & dos crt0.o from FB 0.20
------------------------------------------------------------------------
r5376 | dkls | 2010-07-07 12:49:04 -0700 (Wed, 07 Jul 2010) | 2 lines

Back to binutils 2.19.1 on linux, to have the same binutils version on all systems.
tram2: expect binutils to be in /usr/local instead of /usr (should have done that from the beginning)
------------------------------------------------------------------------
r5374 | dkls | 2010-07-06 10:31:27 -0700 (Tue, 06 Jul 2010) | 1 line

tram2: Fixed - was just appending to manifests, now properly overwrites them
------------------------------------------------------------------------
r5373 | dkls | 2010-07-06 09:15:57 -0700 (Tue, 06 Jul 2010) | 3 lines

tram2: Copy in import libraries from previous release for now, instead of using genimplibs, because with dlltool 2.19 some of the generated libs are broken.

Possible solution: adjust genimplibs to invoke dlltool with "--add-stdcall-underscore", but that doesn't work with all libs.
------------------------------------------------------------------------
r5372 | dkls | 2010-07-02 08:47:37 -0700 (Fri, 02 Jul 2010) | 1 line

Added EOL at EOF to examples where it was missing
------------------------------------------------------------------------
r5371 | dkls | 2010-07-02 03:10:46 -0700 (Fri, 02 Jul 2010) | 1 line

Added __FB_UNIX__/__FB_PCOS__ (target type specific defines), thanks to segin for the patch
------------------------------------------------------------------------
r5369 | dkls | 2010-06-25 06:39:48 -0700 (Fri, 25 Jun 2010) | 1 line

Merged r5368 from 0.21 branch
------------------------------------------------------------------------
r5366 | dkls | 2010-06-25 02:39:44 -0700 (Fri, 25 Jun 2010) | 1 line

tram2: Added source code archive generation
------------------------------------------------------------------------
r5365 | counting_pine | 2010-06-25 02:02:45 -0700 (Fri, 25 Jun 2010) | 7 lines

* Make z/wstring params byref by default - byval not (yet?) supported
* Make sure use_default is initialised to FALSE
* Add Declares to unit test to check implicit byrefs (will fail or clash with function headers if not)
* Add a String parameter test, for the heck of it

Todo: Run tests (build broken atm), revert if breakage, otherwise port to release branch

------------------------------------------------------------------------
r5363 | dkls | 2010-06-23 09:53:38 -0700 (Wed, 23 Jun 2010) | 1 line

Merged r5360-r5362 from 0.21 branch
------------------------------------------------------------------------
r5359 | dkls | 2010-06-23 09:37:01 -0700 (Wed, 23 Jun 2010) | 1 line

More version number updates
------------------------------------------------------------------------
r5356 | dkls | 2010-06-18 22:33:45 -0700 (Fri, 18 Jun 2010) | 1 line

Updated trunk version to 0.22
------------------------------------------------------------------------
r5354 | dkls | 2010-06-18 07:36:48 -0700 (Fri, 18 Jun 2010) | 1 line

tram2: Fixed packaging, which was broken.
------------------------------------------------------------------------
r5353 | dkls | 2010-06-18 06:34:52 -0700 (Fri, 18 Jun 2010) | 1 line

tram2: Added updated Windows installer, faster packaging.
------------------------------------------------------------------------
r5352 | dkls | 2010-06-14 19:35:00 -0700 (Mon, 14 Jun 2010) | 1 line

tram2: Fix linux/linux-standalone mixup in manifests; hold back the extra standalone libs (not working as expected).
------------------------------------------------------------------------
r5351 | dkls | 2010-06-14 18:29:16 -0700 (Mon, 14 Jun 2010) | 1 line

Updated manifests.
------------------------------------------------------------------------
r5350 | dkls | 2010-06-14 18:27:07 -0700 (Mon, 14 Jun 2010) | 1 line

tram2: Added more libs to linux-standalone.
------------------------------------------------------------------------
r5349 | dkls | 2010-06-14 17:34:22 -0700 (Mon, 14 Jun 2010) | 1 line

tram2: Added missing win32 libs; excluded windows.bi from dos/linux manifests; updated tram2.
------------------------------------------------------------------------
r5348 | dkls | 2010-06-13 15:23:19 -0700 (Sun, 13 Jun 2010) | 1 line

Removed the old manifests from trunk. In the future, each release branch and the trunk should have one set of manifests only.
------------------------------------------------------------------------
r5347 | dkls | 2010-06-13 14:03:22 -0700 (Sun, 13 Jun 2010) | 1 line

Added/moved fblogo.ico & fblogo.xpm into FreeBASIC/.
------------------------------------------------------------------------
r5346 | dkls | 2010-06-13 13:26:37 -0700 (Sun, 13 Jun 2010) | 1 line

tram2 win32: Added MinGW GCC 4.5.0 how-to. Always create .tar.lzma packages now, .zip's were twice as big.
------------------------------------------------------------------------
r5345 | dkls | 2010-06-13 13:14:27 -0700 (Sun, 13 Jun 2010) | 1 line

Replaced old manifests with auto-generated ones.
------------------------------------------------------------------------
r5344 | dkls | 2010-06-12 20:04:53 -0700 (Sat, 12 Jun 2010) | 1 line

genimplibs: Updated output path & added a makefile.
------------------------------------------------------------------------
r5343 | dkls | 2010-06-12 19:42:23 -0700 (Sat, 12 Jun 2010) | 1 line

tram2: Added linux support and proper packaging.
------------------------------------------------------------------------
r5342 | dkls | 2010-06-12 09:16:00 -0700 (Sat, 12 Jun 2010) | 1 line

Deleted ld scripts from bin/<target> (unused).
------------------------------------------------------------------------
r5341 | dkls | 2010-06-11 15:40:43 -0700 (Fri, 11 Jun 2010) | 1 line

Updated examples to current FB, thanks to MOD for the patch.
------------------------------------------------------------------------
r5340 | dkls | 2010-06-11 11:42:37 -0700 (Fri, 11 Jun 2010) | 1 line

Fixed - assignments to bitfields in globals were broken.
------------------------------------------------------------------------
r5339 | dkls | 2010-06-11 08:24:04 -0700 (Fri, 11 Jun 2010) | 1 line

Moved the genimplibs tool into src/contrib.
------------------------------------------------------------------------
r5338 | dkls | 2010-06-11 08:13:25 -0700 (Fri, 11 Jun 2010) | 1 line

Removed most of the WinAPI *.def's from lib/win32/def, these libs will be pulled in from MinGW instead.
------------------------------------------------------------------------
r5337 | dkls | 2010-06-11 08:10:32 -0700 (Fri, 11 Jun 2010) | 1 line

Added src/contrib/tram2: tool to automatically build & package releases.
------------------------------------------------------------------------
r5336 | dkls | 2010-06-11 08:08:12 -0700 (Fri, 11 Jun 2010) | 4 lines

Changed makefiles for DOS:
* call DOS tools indirectly (to prevent errors from the win32 make)
* pass @optionsfile to DOS tools (to work around long command-line problems)
The DOS fbc can now build itself.
------------------------------------------------------------------------
r5335 | dkls | 2010-06-07 15:18:28 -0700 (Mon, 07 Jun 2010) | 1 line

Added @<file> command-line option, to read command-line options from files.
------------------------------------------------------------------------
r5334 | dkls | 2010-06-01 16:44:56 -0700 (Tue, 01 Jun 2010) | 1 line

Added - preprocessor-only mode (-pp)
------------------------------------------------------------------------
r5333 | dkls | 2010-05-24 11:15:45 -0700 (Mon, 24 May 2010) | 1 line

Fix: do not access deleted AST node
------------------------------------------------------------------------
r5332 | dkls | 2010-05-10 13:15:08 -0700 (Mon, 10 May 2010) | 1 line

emit_x86 export table output: refactor dup code for procs & methods, and inline hStripUnderscore().
------------------------------------------------------------------------
r5331 | dkls | 2010-05-10 13:07:40 -0700 (Mon, 10 May 2010) | 1 line

delete unused header dag.bi
------------------------------------------------------------------------
r5330 | whotookcha0s-_- | 2010-05-10 02:23:09 -0700 (Mon, 10 May 2010) | 1 line

Added - search 32-bit library paths in the linker scripts for Linux and FreeBSD (darn typos)
------------------------------------------------------------------------
r5329 | whotookcha0s-_- | 2010-05-10 02:22:43 -0700 (Mon, 10 May 2010) | 1 line

Added - search 320bit library paths in the linker scripts for Linux and FreeBSD
------------------------------------------------------------------------
r5328 | whotookcha0s-_- | 2010-05-09 23:45:25 -0700 (Sun, 09 May 2010) | 3 lines

Added a simple demonstration of how to simulate getting a class function pointer


------------------------------------------------------------------------
r5327 | whotookcha0s-_- | 2010-05-09 18:44:09 -0700 (Sun, 09 May 2010) | 3 lines

Fixed - The final slash stripping code in fbcGetLibPathList() was only working properly on systems using '\' as a path separator.


------------------------------------------------------------------------
r5326 | whotookcha0s-_- | 2010-05-09 16:49:20 -0700 (Sun, 09 May 2010) | 3 lines

Fixed #2974397 - Dynamic arrays with any const qualifiers below the first level (aka [ptr ]*) would erroneously report duplicate definition (cha0s)


------------------------------------------------------------------------
r5325 | dkls | 2010-04-08 14:28:46 -0700 (Thu, 08 Apr 2010) | 1 line

unix rtlib was repeatedly leaking its /dev/tty file handle (bug #2590116)
------------------------------------------------------------------------
r5324 | dkls | 2010-03-29 06:01:29 -0700 (Mon, 29 Mar 2010) | 2 lines

'dim a as string * expr = ""' wasn't working because of expression/varinit ambiguity

------------------------------------------------------------------------
r5323 | dkls | 2010-03-10 15:39:39 -0800 (Wed, 10 Mar 2010) | 1 line

global destructors could be EXPORT'ed due to a typo
------------------------------------------------------------------------
r5322 | dkls | 2010-03-08 12:01:33 -0800 (Mon, 08 Mar 2010) | 1 line

added more division by constant zero checks, see bug #2961790
------------------------------------------------------------------------
r5321 | dkls | 2010-03-08 11:25:55 -0800 (Mon, 08 Mar 2010) | 1 line

fix uninitialized variable, bug #2876567
------------------------------------------------------------------------
r5320 | dkls | 2010-03-03 16:24:49 -0800 (Wed, 03 Mar 2010) | 1 line

update changelog too
------------------------------------------------------------------------
r5319 | dkls | 2010-03-03 15:29:30 -0800 (Wed, 03 Mar 2010) | 1 line

adjust fb_FileInputNextToken() to do hReadChar() for the last char, so it's not stored in the put-back-buffer and read again later; fixes bug #2914509 "INPUT command generates spurious data on 4096 byte boundary."
------------------------------------------------------------------------
r5318 | dkls | 2010-03-03 15:01:44 -0800 (Wed, 03 Mar 2010) | 1 line

parser: correctly keep track of current compound statement, fixes bug #2776255 "extern, type and overload bug?"
------------------------------------------------------------------------
r5317 | dkls | 2010-03-03 14:25:23 -0800 (Wed, 03 Mar 2010) | 1 line

set svn:eol-style=LF for rtlib's config.in
------------------------------------------------------------------------
r5316 | v1ctor | 2010-02-01 01:30:34 -0800 (Mon, 01 Feb 2010) | 1 line

fixed: the returned type was not been checked for UDTs when emitting function prototypes (gcc emitter)
------------------------------------------------------------------------
r5315 | counting_pine | 2010-01-30 19:17:57 -0800 (Sat, 30 Jan 2010) | 1 line

Fix for #2939523: SIZEOF() errors inside ASM blocks
------------------------------------------------------------------------
r5314 | v1ctor | 2010-01-22 18:11:52 -0800 (Fri, 22 Jan 2010) | 1 line

fixed (gcc emitter): UDTs with same name in different namespaces; functions returning UDTs
------------------------------------------------------------------------
r5313 | v1ctor | 2010-01-22 12:11:10 -0800 (Fri, 22 Jan 2010) | 1 line

fixed (gcc emitter): function pointers w/o prototypes; float to ubyte casting
------------------------------------------------------------------------
r5312 | v1ctor | 2010-01-22 12:08:56 -0800 (Fri, 22 Jan 2010) | 1 line

fixed (gcc emitter): function pointers w/o prototypes; float to ubyte casting
------------------------------------------------------------------------
r5311 | v1ctor | 2010-01-22 10:15:39 -0800 (Fri, 22 Jan 2010) | 1 line

fixed (gcc emitter): local UDTs
------------------------------------------------------------------------
r5310 | v1ctor | 2010-01-22 09:27:36 -0800 (Fri, 22 Jan 2010) | 1 line

fixed (gcc emitter): imported (dll) symbols
------------------------------------------------------------------------
r5309 | v1ctor | 2010-01-22 08:06:12 -0800 (Fri, 22 Jan 2010) | 1 line

merged with the "v0_21-gengcc" branch
------------------------------------------------------------------------
r5276 | v1ctor | 2010-01-13 10:40:41 -0800 (Wed, 13 Jan 2010) | 1 line

internal changes to allow fbc to use the TDM/MinGW's (gcc 4.4) runtime
------------------------------------------------------------------------
r5275 | counting_pine | 2010-01-03 20:52:50 -0800 (Sun, 03 Jan 2010) | 1 line

Change svn:eol-style to LF, as per dkl's suggestion in http://freebasic.net/forum/viewtopic.php?p=128913#128913
------------------------------------------------------------------------
r5274 | counting_pine | 2009-12-21 11:53:26 -0800 (Mon, 21 Dec 2009) | 2 lines

Submit SARGS patch to fix stabs for global redim arrays (http://www.freebasic.net/forum/viewtopic.php?p=117584#117584)
Note that I have done very little work with stabs and don't really understand the patch, nor am I particularly satisfied with the method.  It would be nice to have a cleaner solution.
------------------------------------------------------------------------
r5273 | counting_pine | 2009-11-01 23:44:54 -0800 (Sun, 01 Nov 2009) | 4 lines

Fix: len/sizeof/typeof(string * n) wasn't working

The previous type/expr disambiguation improvement was overenthusiastic, and rejecting STRING * n because the '*' made it look like an expression.  Fixed by not treating '*' as an operator.
Quick unit test addition to check typeof(string*n)
------------------------------------------------------------------------
r5272 | counting_pine | 2009-11-01 22:52:34 -0800 (Sun, 01 Nov 2009) | 1 line

Internal change in parser-quirk-math.bas: move Len/Sizeof and Atan2 code into separate functions.  Should be no difference to end user
------------------------------------------------------------------------
r5271 | counting_pine | 2009-10-31 08:52:05 -0700 (Sat, 31 Oct 2009) | 1 line

My changes broke the sign detection in conversions from non-registers.  Hopefully it's all working properly now.  Unit test updated, should hopefully catch that problem in future.
------------------------------------------------------------------------
r5270 | counting_pine | 2009-10-31 06:57:11 -0700 (Sat, 31 Oct 2009) | 9 lines

Second fix for #2082801 - errors when converting ulongint expressions to floating-point (note: previous fix was actually for float to ulongint - careless old me...)

This was happening because for expressions (which were kept in registers), the sign bit check (used to work out whether to add 2^64 to the number, because fild treats values as signed) was apparently being done on the wrong register.  Hopefully fixed that.  Also renamed the variable for hoped increase in clarity.

Also, hexed the HPUSH'ed numbers for clarity/consistency.

Unit test added, fix for possible bug in f2ll unit test.

Note: both #2082801 fixes have been done on both x86 and SSE emitters.  The fixes were the same.
------------------------------------------------------------------------
r5269 | counting_pine | 2009-10-31 04:44:07 -0700 (Sat, 31 Oct 2009) | 1 line

Fix unit test (no changes to compiler/rtlib)
------------------------------------------------------------------------
r5268 | counting_pine | 2009-10-25 23:39:55 -0700 (Sun, 25 Oct 2009) | 2 lines

Part fix for #2082801 - errors when converting ulongint numbers to floating-point
- changes the conversion method used for ulongints - now it just uses the same method as for longints.  (This means that numbers >= 2^63 will be negative, but still an improvement on before)
------------------------------------------------------------------------
r5267 | counting_pine | 2009-09-17 13:32:14 -0700 (Thu, 17 Sep 2009) | 1 line

Fixed incorrect pointer sizes being emitted in -fpu SSE code - thanks to brybry for patch
------------------------------------------------------------------------
r5266 | counting_pine | 2009-09-07 07:06:54 -0700 (Mon, 07 Sep 2009) | 1 line

Fix: Close button was not working on Linux in SVN - thanks to Sisophon2001 for patch
------------------------------------------------------------------------
r5264 | counting_pine | 2009-08-23 21:43:43 -0700 (Sun, 23 Aug 2009) | 3 lines

Synchronise fbdoc cache with wiki
Update examples/manual/... files
Append linebreaks to a couple of fbdoc modules
------------------------------------------------------------------------
r5263 | i_am_drv | 2009-08-13 15:45:23 -0700 (Thu, 13 Aug 2009) | 5 lines

A forgotten Darwin-related change:
 - The constant/read-only section is named ".const"
 - Sections are emitted without the ".section" directive


------------------------------------------------------------------------
r5262 | counting_pine | 2009-08-09 23:53:55 -0700 (Sun, 09 Aug 2009) | 2 lines

- Allow Err to be called with empty parentheses - i.e. Err() - like a normal function
- Remove some debugging prints left in parser-decl-var.bas from r5259
------------------------------------------------------------------------
r5261 | counting_pine | 2009-08-09 23:33:36 -0700 (Sun, 09 Aug 2009) | 9 lines

Fix for #2102417 - Scope/Dtor problem if no new-line after "LOOP"-statement:

- in lex.bas/lexSkipToken, increment parser.stmt.cnt on FB_TK_EOF (as it already does for TK_EOL/TK_STMTSTEP)

-  in ast-node-scope.bas, rewrite hisInside() macro to use andalso instead of iif(...), for clarity, plus extra parentheses for safety/pedantry.  Functionality is unchanged (although changing '<' to '<=' could be another possible avenue for fixing this bug)

- couple of log-tests added, for exit do / continue do (note the lack of EOL at the end of them)

Thanks to dkls for report/insights on this
------------------------------------------------------------------------
r5260 | i_am_drv | 2009-07-20 10:32:38 -0700 (Mon, 20 Jul 2009) | 6 lines

Fix inline asm in commented-out VESA PM palette set code.

This has been sitting in my local tree for a while; however, code is still not
tested.


------------------------------------------------------------------------
r5259 | counting_pine | 2009-07-12 16:50:45 -0700 (Sun, 12 Jul 2009) | 1 line

#2818918: Prevent parsing '...' in arrays in UDTs - there was no internal support for them
------------------------------------------------------------------------
r5258 | counting_pine | 2009-07-12 16:34:21 -0700 (Sun, 12 Jul 2009) | 2 lines

- Make "Byval As String" warning only show when compiling with -w param|pedantic
- add warning for when the file doesn't end with a newline (GCC has this, also we currently have a bug (#2102417) that can occur on such files)
------------------------------------------------------------------------
r5257 | counting_pine | 2009-07-06 06:10:48 -0700 (Mon, 06 Jul 2009) | 4 lines

#2806342 - symb can lose wstring literals
- apply fix by dkls (couple of additional minor changes made)
- write unit test

------------------------------------------------------------------------
r5256 | i_am_drv | 2009-07-04 09:35:22 -0700 (Sat, 04 Jul 2009) | 3 lines

Fix optimizations that did not check node class before checking op
This fixes bug #2816558

------------------------------------------------------------------------
r5255 | counting_pine | 2009-06-18 01:19:34 -0700 (Thu, 18 Jun 2009) | 3 lines

- Add _FB_FPMODE__/__FB_FPU__ defines
- Change official x87 -fpu mode name from "fpu" to "x87" ("fpu" still works as a synonym)

------------------------------------------------------------------------
r5254 | counting_pine | 2009-06-15 02:11:42 -0700 (Mon, 15 Jun 2009) | 4 lines

Fix some unit test warnings (hidden by -w 3)
- Remove BYVAL AS STRING from many unit tests
- Add a return value for a dummy operator
- fix a case where the test sub wasn't cdecl (mercifully parameterless, so hopefully didn't cause any problems)
------------------------------------------------------------------------
r5253 | counting_pine | 2009-06-15 00:04:22 -0700 (Mon, 15 Jun 2009) | 4 lines

Small lexer fixes:
- Disallow "unsigned float" literal suffixes, e.g. 1uf, &h1u#
- Prevent upper->lower case transformations in '&B'/'&O'/'&H' (e.g. in stringized macro parameters); add unit test
- add '&77' octal test to input_large_int unit test
------------------------------------------------------------------------
r5252 | counting_pine | 2009-06-11 01:28:09 -0700 (Thu, 11 Jun 2009) | 5 lines

- Added: Support for "&..." octal numbers in VAL/VALINT/... (like "&O..." octal numbers, but without the "O")
  No compile-time support for &... in number literals though, at least for the moment.

- Some work on test case, including adding new "&..." cases.

------------------------------------------------------------------------
r5251 | counting_pine | 2009-06-09 01:22:10 -0700 (Tue, 09 Jun 2009) | 1 line

Fix: SINGLE constants were being converted to string with double precision, e.g. str(sqr(2!))
------------------------------------------------------------------------
r5250 | counting_pine | 2009-06-07 00:11:30 -0700 (Sun, 07 Jun 2009) | 1 line

Port changes from PRINT USING branch - now trunk supports "&" for numbers, SPC/TAB, and normal expressions before the USING.
------------------------------------------------------------------------
r5249 | i_am_drv | 2009-06-04 11:00:29 -0700 (Thu, 04 Jun 2009) | 5 lines

Logic optimization:
not (const xor x)    to    (not const) xor x
not (x xor const)    to    x xor (not const)


------------------------------------------------------------------------
r5248 | i_am_drv | 2009-06-04 10:37:28 -0700 (Thu, 04 Jun 2009) | 6 lines

Another logic optimization:
(not x) xor y    to    not (x xor y)
x xor (not y)    to    not (x xor y)
Allows elimination of NOT if nested (ex. not ((not x) xor y))


------------------------------------------------------------------------
r5246 | counting_pine | 2009-05-29 06:30:49 -0700 (Fri, 29 May 2009) | 1 line

Apply patch #2702183 ("Patch for modified Linux gfxlib") - thanks Sisophon2001
------------------------------------------------------------------------
r5245 | counting_pine | 2009-05-29 06:19:30 -0700 (Fri, 29 May 2009) | 2 lines

Committed the fix for #2433409 by Sisophon2001

------------------------------------------------------------------------
r5244 | counting_pine | 2009-05-29 05:55:40 -0700 (Fri, 29 May 2009) | 1 line

Add warning message for "BYVAL AS STRING" (unintuitively has same behaviour as new "BYREF AS ZSTRING")
------------------------------------------------------------------------
r5243 | counting_pine | 2009-05-26 01:07:30 -0700 (Tue, 26 May 2009) | 5 lines

Internal changes:
- use "byval as zstring ptr" instead of "byval as string" in edbg_stab.bas, to avoid soon-to-come warning
- replace some *lexGetText() checks with neater hMatchText() tests
- redo r5241 patch ("print #f, modify(f)") using already-existing astIsCONST() check

------------------------------------------------------------------------
r5241 | counting_pine | 2009-05-18 02:58:50 -0700 (Mon, 18 May 2009) | 3 lines

Fix: PRINT|WRITE #f could print to the wrong file number if it printed an expression that modified f
- duplicate f whenever it's a non-constant, not just when it's a function call

------------------------------------------------------------------------
r5238 | counting_pine | 2009-05-09 00:42:51 -0700 (Sat, 09 May 2009) | 4 lines

lang qb: added __SCREEN keyword for accessing FB's normal SCREEN syntax
- added a KWD_QB_ONLY keyword flag
- created a new SCREEN keyword (FB_TK_SCREENQB) with QB_ONLY flag
- set the main SCREEN keyword (FB_TK_SCREEN) as NO_QB, so it can (only) be accessed with __ prefix in lang qb
------------------------------------------------------------------------
r5237 | counting_pine | 2009-05-04 00:11:54 -0700 (Mon, 04 May 2009) | 1 line

Add support for BYREF AS ZSTRING/BYREF AS WSTRING parameters
------------------------------------------------------------------------
r5236 | counting_pine | 2009-05-03 00:25:46 -0700 (Sun, 03 May 2009) | 3 lines

Fix for #2038682: faulty optimisation - 16-bit ops on constants could result in constant values that weren't truncated to fit the result type
- Add test for uops/bops
- Minor, unrelated hlp.bi fix (changed a parameter name in r5203)
------------------------------------------------------------------------
r5235 | counting_pine | 2009-04-16 23:44:03 -0700 (Thu, 16 Apr 2009) | 2 lines

Fix: FB was "ghosting" parameter names in error messages, in cases where the parameter was unnamed, but a previous function header had a parameter with a name in the same position (counting_pine)

------------------------------------------------------------------------
r5234 | counting_pine | 2009-04-05 00:39:57 -0700 (Sun, 05 Apr 2009) | 11 lines

patch for rtlib/libfb_file_winputstr.c, by dkls:

- changed fb_FileWstrInput in libfb_file_winputstr.c to always set a null
term in the resulting buffer when successfully reading from a file.

- cleaned up the unneeded null term assignments there, I hope I did not
overlook something there.

- added call to fb_wstr_Del to free the buffer on error

- added missing FB_UNLOCK() at the end
------------------------------------------------------------------------
r5233 | counting_pine | 2009-03-25 01:35:14 -0700 (Wed, 25 Mar 2009) | 2 lines

Fix for INSTR: make it use malloc for lookup tables instead of alloca, to avoid exceeding stack space when searching for large patterns.
(bm_gc and suffixes lookup tables need an int for every char in the pattern string.)
------------------------------------------------------------------------
r5232 | counting_pine | 2009-03-16 03:12:19 -0700 (Mon, 16 Mar 2009) | 1 line

Quick fix for #2688314 - fbc crash: -maxerr & error in shared array initialization
------------------------------------------------------------------------
r5231 | counting_pine | 2009-03-15 03:33:29 -0700 (Sun, 15 Mar 2009) | 3 lines

FBC tests:
- Add a unit test for vectorization (thanks Bryan!)
- Disable a broken log-test
------------------------------------------------------------------------
r5230 | counting_pine | 2009-03-09 02:28:31 -0700 (Mon, 09 Mar 2009) | 3 lines

Fix:
- make sure backslashes in .stabs/.file paths are escaped
- for appropriate platforms, change the slashes in those paths to backslashes, since they will be escaped now
------------------------------------------------------------------------
r5229 | counting_pine | 2009-03-07 04:29:31 -0800 (Sat, 07 Mar 2009) | 2 lines

Stopgap fix for #2617069: use a simple naive algorithm for instrrev; add a regression test for the reported case.

------------------------------------------------------------------------
r5228 | counting_pine | 2009-03-06 12:02:41 -0800 (Fri, 06 Mar 2009) | 3 lines

Minor fix: SLEEP() was listed as an Integer function in rtl-system.bas, but is a void() in the rtlib


------------------------------------------------------------------------
r5227 | counting_pine | 2009-03-06 03:04:47 -0800 (Fri, 06 Mar 2009) | 2 lines

Internal: change ErrReportParam[Warn]() functions so they take an FBSYMBOL Ptr instead of an Any Ptr.
(Used a fwd-ref to sidestep some circular dependencies in the headers, presumably the reason why an Any Ptr was used in the first place)
------------------------------------------------------------------------
r5226 | counting_pine | 2009-03-06 00:43:17 -0800 (Fri, 06 Mar 2009) | 4 lines

- Bug fix for #2659332 - rtlFilePut/Get should use errReportParamWarn correctly (thanks dkls)

- Make the "Passing Pointer" warning slightly more helpful by giving the parameter number; moving it out of parser-quirk-file.bas and into rtl-file.bas, in the same place as the other warning

------------------------------------------------------------------------
r5225 | counting_pine | 2009-03-01 23:36:22 -0800 (Sun, 01 Mar 2009) | 1 line

r5209 fixed bug #2483518 in the tracker.  Update changelog to show that.
------------------------------------------------------------------------
r5224 | counting_pine | 2009-03-01 23:08:11 -0800 (Sun, 01 Mar 2009) | 1 line

New version of Bryan's vectorization patch
------------------------------------------------------------------------
r5219 | v1ctor | 2009-03-01 11:13:14 -0800 (Sun, 01 Mar 2009) | 1 line

added: builtin functions for FRAC(), FIX() and SGN()
------------------------------------------------------------------------
r5215 | counting_pine | 2009-02-14 20:37:01 -0800 (Sat, 14 Feb 2009) | 1 line

Update changelog.txt to reflect reversion in last commit, for now (not quite reverted, since r5213 had edits for other changelog items too)
------------------------------------------------------------------------
r5214 | counting_pine | 2009-02-14 20:28:28 -0800 (Sat, 14 Feb 2009) | 1 line

Revert last change due to apparent bug (see #2601554)
------------------------------------------------------------------------
r5213 | counting_pine | 2009-02-14 01:50:18 -0800 (Sat, 14 Feb 2009) | 1 line

Apply brybry's vectorising patch
------------------------------------------------------------------------
r5212 | counting_pine | 2009-02-12 23:43:08 -0800 (Thu, 12 Feb 2009) | 1 line

Additional fix for #2270346, #2492086 ("dim as integer x, x(0 to ...)")
------------------------------------------------------------------------
r5211 | counting_pine | 2009-02-08 23:49:26 -0800 (Sun, 08 Feb 2009) | 1 line

Fix for #2270346, #2492086: a duplicated definition in a Dim (e.g. "Dim as integer x, x") could crash fbc.
------------------------------------------------------------------------
r5210 | counting_pine | 2009-02-01 20:13:43 -0800 (Sun, 01 Feb 2009) | 3 lines

Fix for #2533376: Crash on non-integral datatype in count specifier of new[]
- Make sure the count is cast to an Integer beforehand.  (Possibly not the best way, but does the trick...)
- additional, minor formatting fixes/changes in a couple of places
------------------------------------------------------------------------
r5209 | counting_pine | 2009-01-24 20:15:56 -0800 (Sat, 24 Jan 2009) | 2 lines

Disregard letter suffixes on numbers in asm blocks, so things like "je 2f" can work (add a new lexer flag)

------------------------------------------------------------------------
r5208 | counting_pine | 2009-01-24 18:26:32 -0800 (Sat, 24 Jan 2009) | 2 lines

Prevent STRPTR/SADD from returning a modifiable reference, e.g. [let] strptr(s) = 0
(achieve this by adding 0 to it in the AST, which will be optimized away later)
------------------------------------------------------------------------
r5207 | counting_pine | 2009-01-20 21:01:01 -0800 (Tue, 20 Jan 2009) | 2 lines

Release the overhauled PRINT USING into the wild...

------------------------------------------------------------------------
r5204 | counting_pine | 2009-01-16 05:21:12 -0800 (Fri, 16 Jan 2009) | 1 line

Bug fix: Newlines weren't being appended after error messages when compiling unicode sources (lexPeekCurrentLine would return an empty string, which wasn't handled properly)
------------------------------------------------------------------------
r5203 | counting_pine | 2009-01-15 18:58:26 -0800 (Thu, 15 Jan 2009) | 3 lines

Bug fix: "CASE IS rel_op expr" was allowing any token for rel_op - hFBrelop2IRrelop was returning correct values for relational operators, but just returning the value of the given token otherwise

Move my DRAW changelog item from "fixed" to "changed"
------------------------------------------------------------------------
r5197 | counting_pine | 2009-01-05 08:28:22 -0800 (Mon, 05 Jan 2009) | 1 line

Apply Bryan's fp optimizations patch - thanks Bryan!
------------------------------------------------------------------------
r5196 | counting_pine | 2009-01-01 01:36:57 -0800 (Thu, 01 Jan 2009) | 1 line

Ignore multi-line comment markers while parsing single-line comments for metacommands (in lang fb, at least) - thanks, VonGodric
------------------------------------------------------------------------
r5195 | counting_pine | 2008-12-19 09:36:31 -0800 (Fri, 19 Dec 2008) | 3 lines

Make COLOR() store the colors in the format for the graphics screen's bit depth, rather than for the bit depth of the last used context (usually these will be the same anyway).  COLOR is the only way to set the fg/bg color for PRINT, but with other drawing statements a color can always explicitly be given.

(In future, we could think about always storing the default color in 32-bit and converting it as needed for 16-bit.  We do this anyway for explicit colors...)
------------------------------------------------------------------------
r5194 | counting_pine | 2008-12-17 19:33:41 -0800 (Wed, 17 Dec 2008) | 1 line

Update changelog for naked functions, add example and unit test
------------------------------------------------------------------------
r5193 | counting_pine | 2008-12-16 07:42:28 -0800 (Tue, 16 Dec 2008) | 1 line

Fix for #2225273 - patch by sisophon2001
------------------------------------------------------------------------
r5192 | counting_pine | 2008-12-03 13:49:39 -0800 (Wed, 03 Dec 2008) | 1 line

Added: more precise "FOR/NEXT variable name mismatch" error
------------------------------------------------------------------------
r5191 | counting_pine | 2008-11-30 19:19:13 -0800 (Sun, 30 Nov 2008) | 3 lines

Improved/more QB-like parsing in DRAW:
- allow things like "M+ -10, 10"
- only treat as relational when there is a sign before the x-coordinate
------------------------------------------------------------------------
r5190 | jeffmarshall | 2008-11-30 08:47:21 -0800 (Sun, 30 Nov 2008) | 1 line

fbc-0.21: update manifests
------------------------------------------------------------------------
r5188 | jeffmarshall | 2008-11-30 08:06:44 -0800 (Sun, 30 Nov 2008) | 1 line

examples/manual: update svn properties to ignore .exe files
------------------------------------------------------------------------
r5187 | jeffmarshall | 2008-11-30 07:45:00 -0800 (Sun, 30 Nov 2008) | 1 line

updated: examples/manual from wiki and added svn:eol-style=native property on all .bas files.
------------------------------------------------------------------------
r5186 | yetifoot | 2008-11-25 20:23:48 -0800 (Tue, 25 Nov 2008) | 1 line

naked functions
------------------------------------------------------------------------
r5185 | i_am_drv | 2008-11-23 14:12:27 -0800 (Sun, 23 Nov 2008) | 1 line

fixed: BLOAD: allow BMP palette to overlap image data
------------------------------------------------------------------------
r5184 | counting_pine | 2008-11-14 04:06:39 -0800 (Fri, 14 Nov 2008) | 2 lines

Another hOptSelfCompare fix: (a OP a) should be left untouched when OP isn't a comparison operator; add test

------------------------------------------------------------------------
r5183 | yetifoot | 2008-11-11 02:11:19 -0800 (Tue, 11 Nov 2008) | 1 line

small bug in function that checks if idents used in #define are valid
------------------------------------------------------------------------
r5182 | counting_pine | 2008-11-10 10:32:19 -0800 (Mon, 10 Nov 2008) | 2 lines

[ #2210424 ] superfluous warning message for UDT and PUT:
Remove the FB_UDTOPT_HASCTORFIELD check from symbGetUDTHasPtrField, so it only checks for FB_UDTOPT_HASPTRFIELD.  But still enable warning for var-len STRINGs in UDTs by also doing symbGetUDTHasPtrField on them.  (Technically, they do contain ptrs, and that's the reason why they shouldn't be PUT.)
------------------------------------------------------------------------
r5181 | yetifoot | 2008-11-08 09:49:52 -0800 (Sat, 08 Nov 2008) | 1 line

... subscript when initializing array, ie dim array(0 to ...) = {1, 2, 3, 4} is now allowed
------------------------------------------------------------------------
r5180 | yetifoot | 2008-11-07 17:25:56 -0800 (Fri, 07 Nov 2008) | 1 line

Mark ANDALSO/ORELSE as done
------------------------------------------------------------------------
r5179 | yetifoot | 2008-11-06 22:19:21 -0800 (Thu, 06 Nov 2008) | 1 line

test for NEW with regular data type initializers
------------------------------------------------------------------------
r5178 | yetifoot | 2008-11-06 21:27:53 -0800 (Thu, 06 Nov 2008) | 1 line

2212554: NEW was failing to initialize correctly with regular data types, caused problem with floats
------------------------------------------------------------------------
r5177 | yetifoot | 2008-11-05 16:41:34 -0800 (Wed, 05 Nov 2008) | 1 line

silly fix
------------------------------------------------------------------------
r5176 | yetifoot | 2008-11-05 16:35:07 -0800 (Wed, 05 Nov 2008) | 1 line

Make include files case insensitive again on win/dos
------------------------------------------------------------------------
r5175 | yetifoot | 2008-11-05 16:33:54 -0800 (Wed, 05 Nov 2008) | 1 line

Make include files case insensitive again on win/dos
------------------------------------------------------------------------
r5174 | counting_pine | 2008-11-05 10:45:09 -0800 (Wed, 05 Nov 2008) | 2 lines

Faulty optimisation: "mod 4294967295" was being seen as "mod -1" and being reduced to zero.

------------------------------------------------------------------------
r5173 | yetifoot | 2008-11-05 05:56:57 -0800 (Wed, 05 Nov 2008) | 1 line

2010551: included files names were being emitted in .stabs in upper case, causing problems in gdb under linux
------------------------------------------------------------------------
r5172 | yetifoot | 2008-11-02 19:18:35 -0800 (Sun, 02 Nov 2008) | 1 line


------------------------------------------------------------------------
r5171 | yetifoot | 2008-11-02 16:20:08 -0800 (Sun, 02 Nov 2008) | 1 line

2210314: IIF within new[] was failing because trees with IIF were not cloned correctly
------------------------------------------------------------------------
r5170 | yetifoot | 2008-10-30 00:43:04 -0700 (Thu, 30 Oct 2008) | 1 line

added a useless space to boost my ohloh score
------------------------------------------------------------------------
r5169 | counting_pine | 2008-10-26 03:23:31 -0700 (Sun, 26 Oct 2008) | 1 line

Fix to fb_hFloat2Str() change made in r5161: win32 apparently has a buggy snprintf, which neither nul-terminates nor returns an error when len==maxlen
------------------------------------------------------------------------
r5168 | counting_pine | 2008-10-25 03:21:30 -0700 (Sat, 25 Oct 2008) | 1 line

Internal: Rename "allocate" type element in REGCLASS
------------------------------------------------------------------------
r5167 | yetifoot | 2008-10-24 16:23:00 -0700 (Fri, 24 Oct 2008) | 1 line

aligned image buffers
------------------------------------------------------------------------
r5166 | yetifoot | 2008-10-21 09:55:07 -0700 (Tue, 21 Oct 2008) | 1 line

fix aligned allocation to be always sure theres space to store the original pointer
------------------------------------------------------------------------
r5165 | i_am_drv | 2008-10-20 15:17:20 -0700 (Mon, 20 Oct 2008) | 1 line

check if stdint.h exists before including it
------------------------------------------------------------------------
r5164 | i_am_drv | 2008-10-20 15:15:28 -0700 (Mon, 20 Oct 2008) | 1 line

Sparc (32-bit) configure support for rtlib
------------------------------------------------------------------------
r5163 | yetifoot | 2008-10-19 17:29:40 -0700 (Sun, 19 Oct 2008) | 1 line

paragraph align screen buffers
------------------------------------------------------------------------
r5162 | yetifoot | 2008-10-19 17:10:24 -0700 (Sun, 19 Oct 2008) | 1 line

paragraph align image buffers
------------------------------------------------------------------------
r5161 | counting_pine | 2008-10-16 16:36:13 -0700 (Thu, 16 Oct 2008) | 1 line

Fix for #2169551: remove win32-specific float->string conversion code, and just rely on snprintf
------------------------------------------------------------------------
r5159 | counting_pine | 2008-10-15 19:16:40 -0700 (Wed, 15 Oct 2008) | 1 line

GMP example should deallocate bignum when it's done
------------------------------------------------------------------------
r5158 | whotookcha0s-_- | 2008-10-14 20:55:12 -0700 (Tue, 14 Oct 2008) | 1 line

- ANY PTR resolution was broken; if a function taking an ANY PTR had an overload taking another type (e.g. BYTE), the overload resolution would incorrectly report that the function call was ambiguous, if an ANY PTR was passed to the function. Now it chooses the ANY PTR candidate.
------------------------------------------------------------------------
r5157 | yetifoot | 2008-10-14 20:37:50 -0700 (Tue, 14 Oct 2008) | 2 lines

local array declaration

------------------------------------------------------------------------
r5156 | counting_pine | 2008-10-14 18:39:21 -0700 (Tue, 14 Oct 2008) | 2 lines

Fix for #2150255: SWAP with strings should have been copying the entire descriptor, not just the first two elements (many thanks to Mike Kennedy for discovering the bug and persevering to find the cause)

------------------------------------------------------------------------
r5155 | i_am_drv | 2008-10-11 21:51:12 -0700 (Sat, 11 Oct 2008) | 1 line

Fix lua_Alloc type (spotted by MVXA)
------------------------------------------------------------------------
r5154 | i_am_drv | 2008-10-09 15:37:02 -0700 (Thu, 09 Oct 2008) | 1 line

change AI_* #define to const to avoid name conflicts (ai_canonname/AI_CANONNAME), spotted by stylin
------------------------------------------------------------------------
r5153 | counting_pine | 2008-10-08 19:39:45 -0700 (Wed, 08 Oct 2008) | 2 lines

Make Randomize use a better default seed (that changes more than once a second) by taking the Timer value and XORing together the DWORDs it's stored in.

------------------------------------------------------------------------
r5152 | counting_pine | 2008-10-07 08:07:30 -0700 (Tue, 07 Oct 2008) | 2 lines

- Add -forcelang to override #LANG/$LANG statements; allow #LANG/$LANG to override the original -lang

------------------------------------------------------------------------
r5151 | counting_pine | 2008-10-06 18:49:57 -0700 (Mon, 06 Oct 2008) | 2 lines

Better disambiguation in LEN/SIZEOF/TYPEOF when they see a token followed by an operator

------------------------------------------------------------------------
r5150 | i_am_drv | 2008-10-05 23:31:22 -0700 (Sun, 05 Oct 2008) | 1 line

don't use RegisterWindowMessage when the message is only needed for private messages within one window class (use WM_USER range instead)
------------------------------------------------------------------------
r5149 | i_am_drv | 2008-10-05 23:17:45 -0700 (Sun, 05 Oct 2008) | 3 lines

fixed: Win32 DirectX gfx driver crashing when DirectX not working/available
(make sure all variables checked in directx_exit are set to NULL in case it is called twice, once from directx_thread and once from fb_hWin32Exit)

------------------------------------------------------------------------
r5148 | jeffmarshall | 2008-10-05 07:15:54 -0700 (Sun, 05 Oct 2008) | 1 line

fixed #2109564: POW and ATAN2 were not checking operand sizes in the SSE emitter - thanks Bryan S. for fix
------------------------------------------------------------------------
r5147 | counting_pine | 2008-10-03 18:35:39 -0700 (Fri, 03 Oct 2008) | 3 lines

Add warning for SHL/SHR when a const shift value is too big (or negative).
(This doesn't prevent values larger than 255 causing asm errors though...)

------------------------------------------------------------------------
r5146 | counting_pine | 2008-09-30 15:33:37 -0700 (Tue, 30 Sep 2008) | 2 lines

Better side-effects checking in hOptNullOp

------------------------------------------------------------------------
r5145 | counting_pine | 2008-09-30 08:47:15 -0700 (Tue, 30 Sep 2008) | 2 lines

Fix hOptSelfCompare optimisation: (a >= a), (a <= a) should be true

------------------------------------------------------------------------
r5144 | counting_pine | 2008-09-29 18:18:29 -0700 (Mon, 29 Sep 2008) | 2 lines

lang qb: Allow DEF___ statements inside procs/compound statements/anywhere else

------------------------------------------------------------------------
r5143 | counting_pine | 2008-09-29 14:53:34 -0700 (Mon, 29 Sep 2008) | 2 lines

Add warning for IF directly after multi-line ELSE (can be confused with ELSEIF)

------------------------------------------------------------------------
r5142 | counting_pine | 2008-09-28 05:31:42 -0700 (Sun, 28 Sep 2008) | 2 lines

WINDOW: float cooridinates were being swapped with an int value

------------------------------------------------------------------------
r5139 | counting_pine | 2008-09-21 16:43:31 -0700 (Sun, 21 Sep 2008) | 2 lines

Fix png_read example so it works with new and old image buffers (GIF and JPEG still need doing)

------------------------------------------------------------------------
r5138 | counting_pine | 2008-09-21 16:21:23 -0700 (Sun, 21 Sep 2008) | 2 lines

Don't allow expressions in the AST that have side-effects to be optimised away; add test

------------------------------------------------------------------------
r5137 | counting_pine | 2008-09-21 15:52:30 -0700 (Sun, 21 Sep 2008) | 2 lines

Add UOP checks to hOptLogic, to prevent false positives

------------------------------------------------------------------------
r5136 | i_am_drv | 2008-09-18 16:15:40 -0700 (Thu, 18 Sep 2008) | 8 lines

more minor optimizations, to allow factoring out NOT:
const and (not x)    to    not ((not const) or  x)
const or  (not x)    to    not ((not const) and x)
const xor (not x)    to    (not const) xor x
(not x) and const    to    not (x or  (not const))
(not x) or  const    to    not (x and (not const))
(not x) xor const    to    x xor (not const)

------------------------------------------------------------------------
r5135 | i_am_drv | 2008-09-16 16:04:53 -0700 (Tue, 16 Sep 2008) | 6 lines

new optimizations:
not not x to x
(not x) and (not y) to not (x or  y)
(not x) or  (not y) to not (x and y)
(not x) xor (not y) to x xor y

------------------------------------------------------------------------
r5131 | counting_pine | 2008-09-12 07:48:37 -0700 (Fri, 12 Sep 2008) | 2 lines

Allow string literals to contain the (non-printable) FB internal escape character

------------------------------------------------------------------------
r5130 | counting_pine | 2008-09-12 02:23:54 -0700 (Fri, 12 Sep 2008) | 3 lines

Disallow EOL chars in string literal escape sequences; small elaboration 
in lex.bi comment 

------------------------------------------------------------------------
r5129 | i_am_drv | 2008-09-09 11:01:40 -0700 (Tue, 09 Sep 2008) | 1 line

Make NetBSD use unix-style findbin
------------------------------------------------------------------------
r5128 | i_am_drv | 2008-09-09 10:57:46 -0700 (Tue, 09 Sep 2008) | 1 line

Finish compiler support for NetBSD
------------------------------------------------------------------------
r5127 | i_am_drv | 2008-09-08 08:37:28 -0700 (Mon, 08 Sep 2008) | 1 line

allow NULL dst (automatic allocation) in fb_StrConcat (tracker #2099345)
------------------------------------------------------------------------
r5126 | counting_pine | 2008-09-08 03:03:47 -0700 (Mon, 08 Sep 2008) | 2 lines

Make FBC print error codes in verbose mode when external programs fail

------------------------------------------------------------------------
r5125 | jeffmarshall | 2008-09-07 19:28:26 -0700 (Sun, 07 Sep 2008) | 1 line

updated log
------------------------------------------------------------------------
r5124 | jeffmarshall | 2008-09-07 19:21:14 -0700 (Sun, 07 Sep 2008) | 1 line

fbc 0.21: update current manifests (and remove some old ones - 0.18.2 though 0.18.5)
------------------------------------------------------------------------
r5120 | jeffmarshall | 2008-09-07 18:09:04 -0700 (Sun, 07 Sep 2008) | 1 line

fix warning (string literal missing closing quote)
------------------------------------------------------------------------
r5119 | jeffmarshall | 2008-09-07 18:04:49 -0700 (Sun, 07 Sep 2008) | 1 line

updated wiki examples
------------------------------------------------------------------------
r5118 | whotookcha0s-_- | 2008-09-07 16:48:48 -0700 (Sun, 07 Sep 2008) | 2 lines

fixed: #2085637 - define was not catching illegal symbol names
added: regression log test using sslaxx's code
------------------------------------------------------------------------
r5117 | whotookcha0s-_- | 2008-09-07 15:10:39 -0700 (Sun, 07 Sep 2008) | 3 lines

fixed: some null ptr checks were missing with object arrays that had no constructor, but a destructor (casing a crash)
added: a small note about an unused variable i encountered
added: regression test for the latest fix
------------------------------------------------------------------------
r5116 | jeffmarshall | 2008-09-07 09:29:10 -0700 (Sun, 07 Sep 2008) | 1 line

gfxlib internal: remove SCREEN_LOCKED flag since the check can be done using the "lock_count" counter instead
------------------------------------------------------------------------
r5115 | jeffmarshall | 2008-09-07 08:09:12 -0700 (Sun, 07 Sep 2008) | 1 line

update FreeImage header to v3.11.0 and .def file for Win32
------------------------------------------------------------------------
r5114 | jeffmarshall | 2008-09-07 08:05:38 -0700 (Sun, 07 Sep 2008) | 1 line

Updates for FreeImage 3.10.0 (add missing #defines and fix .def file)
------------------------------------------------------------------------
r5113 | jeffmarshall | 2008-09-07 08:00:56 -0700 (Sun, 07 Sep 2008) | 1 line

fbc internal: change macro ast-node-conv.bas:hCheckPtr to a function for easier debugging
------------------------------------------------------------------------
r5112 | jeffmarshall | 2008-09-07 04:45:20 -0700 (Sun, 07 Sep 2008) | 1 line

update compiler configuration info in INSTALL and update log messages in changelog.txt
------------------------------------------------------------------------
r5111 | counting_pine | 2008-09-06 01:43:20 -0700 (Sat, 06 Sep 2008) | 2 lines

Fix for #2057952: Make Lang QB identify f-p literal types properly, like in QBASIC

------------------------------------------------------------------------
r5110 | i_am_drv | 2008-09-04 16:02:55 -0700 (Thu, 04 Sep 2008) | 1 line

added: Multi-monitor support for Win32 OpenGL gfx driver
------------------------------------------------------------------------
r5109 | i_am_drv | 2008-09-04 15:25:42 -0700 (Thu, 04 Sep 2008) | 1 line

added: Multi-monitor support for Win32 GDI gfx driver
------------------------------------------------------------------------
r5108 | counting_pine | 2008-09-04 03:52:58 -0700 (Thu, 04 Sep 2008) | 2 lines

SCREEN in lang qb shouldn't try to set pages if parameters are omitted

------------------------------------------------------------------------
r5107 | counting_pine | 2008-09-04 03:14:13 -0700 (Thu, 04 Sep 2008) | 3 lines

Fix similar off-by-one console page validating error in SCREEN , a, v
correct/update changelog (last one was actually PCOPY)

------------------------------------------------------------------------
r5106 | counting_pine | 2008-09-04 02:53:43 -0700 (Thu, 04 Sep 2008) | 2 lines

Fix off-by-one validating error in PCOPY when checking console page number parameters

------------------------------------------------------------------------
r5105 | counting_pine | 2008-09-01 20:39:53 -0700 (Mon, 01 Sep 2008) | 2 lines

FBC fix: revert debug emitter to always emitting forward slashes in path strings

------------------------------------------------------------------------
r5104 | whotookcha0s-_- | 2008-08-31 01:19:38 -0700 (Sun, 31 Aug 2008) | 1 line

the last hrevertslash commit was breaking on my build, replaced with a smaller function (i'm not sure why it broke the old way...)
------------------------------------------------------------------------
r5103 | i_am_drv | 2008-08-30 11:15:09 -0700 (Sat, 30 Aug 2008) | 1 line

Make hRevertSlash change all back and forward slashes to host path separator
------------------------------------------------------------------------
r5102 | whotookcha0s-_- | 2008-08-28 09:13:18 -0700 (Thu, 28 Aug 2008) | 1 line

added: notes on the lexcheck enum at lex.bi
------------------------------------------------------------------------
r5101 | i_am_drv | 2008-08-27 08:34:28 -0700 (Wed, 27 Aug 2008) | 1 line

Frontend cleanup - move most target parameter setup into fbc_*.bas
------------------------------------------------------------------------
r5099 | counting_pine | 2008-08-25 16:47:22 -0700 (Mon, 25 Aug 2008) | 2 lines

More thorough checking for when numeric literals exceed max ulongint value, but highest bit isn't set: e.g. 27670116110564327424 = 3 * 2^63

------------------------------------------------------------------------
r5098 | i_am_drv | 2008-08-25 13:26:33 -0700 (Mon, 25 Aug 2008) | 1 line

initial pass at netbsd support for compiler (unfinished)
------------------------------------------------------------------------
r5097 | i_am_drv | 2008-08-25 10:14:18 -0700 (Mon, 25 Aug 2008) | 1 line

always use XMoveResizeWindow in fullscreen mode in x11 gfx driver
------------------------------------------------------------------------
r5096 | i_am_drv | 2008-08-25 09:35:22 -0700 (Mon, 25 Aug 2008) | 1 line

initial pass at NetBSD rtlib and gfxlib2 support
------------------------------------------------------------------------
r5095 | i_am_drv | 2008-08-24 10:39:48 -0700 (Sun, 24 Aug 2008) | 1 line

svn:ignore * in openbsd obj dir
------------------------------------------------------------------------
r5094 | i_am_drv | 2008-08-24 10:39:03 -0700 (Sun, 24 Aug 2008) | 1 line

fix build when FB_BFD defined but BFD_VER not
------------------------------------------------------------------------
r5093 | i_am_drv | 2008-08-24 10:10:36 -0700 (Sun, 24 Aug 2008) | 1 line

display objinfo configuration in -version
------------------------------------------------------------------------
r5092 | i_am_drv | 2008-08-23 21:14:02 -0700 (Sat, 23 Aug 2008) | 1 line

quickly scrolling the mouse wheel was causing double click events in the x11 gfx driver (spotted by Sisophon2001)
------------------------------------------------------------------------
r5091 | i_am_drv | 2008-08-23 15:31:07 -0700 (Sat, 23 Aug 2008) | 1 line

add missing fbc_darwin.bas
------------------------------------------------------------------------
r5090 | i_am_drv | 2008-08-23 11:37:50 -0700 (Sat, 23 Aug 2008) | 1 line

remove broken update check (doesn't work for recursive copies)
------------------------------------------------------------------------
r5089 | i_am_drv | 2008-08-23 11:19:14 -0700 (Sat, 23 Aug 2008) | 1 line

OpenBSD X11 gfx driver
------------------------------------------------------------------------
r5088 | i_am_drv | 2008-08-23 11:18:38 -0700 (Sat, 23 Aug 2008) | 1 line

add portable update check to install.sh
------------------------------------------------------------------------
r5087 | i_am_drv | 2008-08-22 15:40:51 -0700 (Fri, 22 Aug 2008) | 1 line

darwin support for compiler (unfinished and untested)
------------------------------------------------------------------------
r5086 | i_am_drv | 2008-08-22 12:19:29 -0700 (Fri, 22 Aug 2008) | 1 line

Only require libintl for objinfo builds
------------------------------------------------------------------------
r5085 | i_am_drv | 2008-08-22 12:18:30 -0700 (Fri, 22 Aug 2008) | 1 line

Don't specify linker script on FreeBSD
------------------------------------------------------------------------
r5084 | i_am_drv | 2008-08-22 09:03:23 -0700 (Fri, 22 Aug 2008) | 1 line

Make gcc lib list per-target
------------------------------------------------------------------------
r5083 | i_am_drv | 2008-08-22 08:24:57 -0700 (Fri, 22 Aug 2008) | 5 lines

Portability fixes for install.sh
Moved switches to beginning of command lines (GNU extension)
Removed -u from cp lines (GNU extension) - maybe write our own update check instead?
install.sh now works on OpenBSD

------------------------------------------------------------------------
r5082 | i_am_drv | 2008-08-22 08:09:10 -0700 (Fri, 22 Aug 2008) | 1 line

Remove -lintl from openbsd compiler makefile
------------------------------------------------------------------------
r5081 | i_am_drv | 2008-08-22 07:54:11 -0700 (Fri, 22 Aug 2008) | 1 line

Initialize unix console cursor location to 1,1 if it is unknown
------------------------------------------------------------------------
r5080 | i_am_drv | 2008-08-21 14:38:29 -0700 (Thu, 21 Aug 2008) | 1 line

OpenBSD default libs
------------------------------------------------------------------------
r5079 | i_am_drv | 2008-08-21 14:32:55 -0700 (Thu, 21 Aug 2008) | 1 line

Temporary hack to get fbc to link (swprintf missing on OpenBSD)
------------------------------------------------------------------------
r5078 | i_am_drv | 2008-08-21 13:31:47 -0700 (Thu, 21 Aug 2008) | 1 line

Add openbsd obj dir
------------------------------------------------------------------------
r5077 | i_am_drv | 2008-08-21 13:29:56 -0700 (Thu, 21 Aug 2008) | 1 line

Fix startup files for OpenBSD
------------------------------------------------------------------------
r5076 | i_am_drv | 2008-08-21 13:12:01 -0700 (Thu, 21 Aug 2008) | 1 line

Initial pass at OpenBSD support for compiler
------------------------------------------------------------------------
r5075 | i_am_drv | 2008-08-21 12:19:36 -0700 (Thu, 21 Aug 2008) | 1 line

First pass at OpenBSD support for gfxlib2
------------------------------------------------------------------------
r5074 | i_am_drv | 2008-08-21 12:10:15 -0700 (Thu, 21 Aug 2008) | 1 line

First pass at OpenBSD support for rtlib
------------------------------------------------------------------------
r5068 | whotookcha0s-_- | 2008-08-15 18:14:28 -0700 (Fri, 15 Aug 2008) | 1 line

fixed: comparing [u]shorts with pointers was causing the compiler to crash
------------------------------------------------------------------------
r5067 | counting_pine | 2008-08-15 17:02:00 -0700 (Fri, 15 Aug 2008) | 2 lines

Make ImageConvertRow abort on invalid source bit depths

------------------------------------------------------------------------
r5063 | i_am_drv | 2008-08-14 10:06:22 -0700 (Thu, 14 Aug 2008) | 1 line

updated
------------------------------------------------------------------------
r5062 | i_am_drv | 2008-08-14 10:00:11 -0700 (Thu, 14 Aug 2008) | 1 line

pass alpha channel unmodified for all 32bpp BMP files (fixes Draw String custom fonts)
------------------------------------------------------------------------
r5061 | counting_pine | 2008-08-12 18:18:13 -0700 (Tue, 12 Aug 2008) | 2 lines

Silence "Implicit variable allocation" warning in Explicit mode (it will have thrown a "Variable not declared" error already.)

------------------------------------------------------------------------
r5060 | counting_pine | 2008-08-11 16:39:56 -0700 (Mon, 11 Aug 2008) | 2 lines

Add support for "n" in FORMAT, for unambiguous minute selection

------------------------------------------------------------------------
r5059 | i_am_drv | 2008-08-11 10:49:37 -0700 (Mon, 11 Aug 2008) | 1 line

fix WIDTH crash (properly this time; text_* is total on-screen characters, while font_* is size of font in pixels)
------------------------------------------------------------------------
r5058 | i_am_drv | 2008-08-11 10:45:05 -0700 (Mon, 11 Aug 2008) | 1 line

updated
------------------------------------------------------------------------
r5057 | i_am_drv | 2008-08-11 10:43:07 -0700 (Mon, 11 Aug 2008) | 3 lines

Fix WIDTH crash when not specifying optional parameter (default -1 value was getting used instead of current value)
Clean up indentation

------------------------------------------------------------------------
r5056 | i_am_drv | 2008-08-10 20:56:54 -0700 (Sun, 10 Aug 2008) | 4 lines

Generalize lex-utf.bas wchar size tests
Fix host/target confusion
Use little/big-endian test value from configure instead of hardcoding x86

------------------------------------------------------------------------
r5055 | i_am_drv | 2008-08-10 20:41:31 -0700 (Sun, 10 Aug 2008) | 1 line

x86-64 type sizes
------------------------------------------------------------------------
r5054 | i_am_drv | 2008-08-10 20:39:27 -0700 (Sun, 10 Aug 2008) | 1 line

x86-64 build system support for compiler (doesn't actually do anything useful yet)
------------------------------------------------------------------------
r5053 | jeffmarshall | 2008-08-10 18:08:27 -0700 (Sun, 10 Aug 2008) | 1 line

update compiler version to 0.21.0 on trunk
------------------------------------------------------------------------
r5050 | jeffmarshall | 2008-08-09 06:48:31 -0700 (Sat, 09 Aug 2008) | 2 lines

merged r5049 from fbc-0_20-branch to trunk:
- added 0.20.0 manifests
------------------------------------------------------------------------
r5044 | jeffmarshall | 2008-08-08 06:28:24 -0700 (Fri, 08 Aug 2008) | 1 line

fbc-0.18.6 - updated log messages for duplicate win32 GFX bugs
------------------------------------------------------------------------
r5042 | jeffmarshall | 2008-08-08 04:57:33 -0700 (Fri, 08 Aug 2008) | 1 line

fixed #2038666 - lang qb: variables with the same name as a keyword must have a suffix
------------------------------------------------------------------------
r5041 | jeffmarshall | 2008-08-08 04:52:46 -0700 (Fri, 08 Aug 2008) | 1 line

updated log message
------------------------------------------------------------------------
r5040 | jeffmarshall | 2008-08-08 04:45:18 -0700 (Fri, 08 Aug 2008) | 1 line

fixed: getting default data type for (for fake id's in error recovery handling on duplicate definitions) was causing an out of bounds array access in symbGetDefType because temp symbol names begin with '.'
------------------------------------------------------------------------
r5039 | jeffmarshall | 2008-08-08 04:39:57 -0700 (Fri, 08 Aug 2008) | 1 line

fixed #1872418 - Win32 GFX updater thread was calling rtlib string conversion unsafely in the non-threaded version of the rtlib
------------------------------------------------------------------------
r5035 | jeffmarshall | 2008-08-06 02:19:58 -0700 (Wed, 06 Aug 2008) | 2 lines

merged r5028 from fbc-0_18-branch to trunk:
- added 0.18.6 manifests
------------------------------------------------------------------------
r5034 | jeffmarshall | 2008-08-06 02:14:21 -0700 (Wed, 06 Aug 2008) | 1 line

update emails
------------------------------------------------------------------------
r5033 | i_am_drv | 2008-08-02 08:09:09 -0700 (Sat, 02 Aug 2008) | 1 line

updated (missed change from cia outage)
------------------------------------------------------------------------
r5032 | counting_pine | 2008-07-31 11:11:44 -0700 (Thu, 31 Jul 2008) | 2 lines

Make sure casting/conversion functions, e.g. CSNG, CLNG, CVL, always return exactly the right type.

------------------------------------------------------------------------
r5030 | jeffmarshall | 2008-07-29 07:53:42 -0700 (Tue, 29 Jul 2008) | 1 line

updated: added 0.18.6 section
------------------------------------------------------------------------
r5026 | counting_pine | 2008-07-28 16:39:43 -0700 (Mon, 28 Jul 2008) | 2 lines

Allow colons following element names in TYPE blocks to act as statement separators if no bitfield is found, e.g. "TYPE FOO: AS INTEGER BAR: END TYPE"

------------------------------------------------------------------------
r5025 | jeffmarshall | 2008-07-28 13:09:43 -0700 (Mon, 28 Jul 2008) | 1 line

reverted r4715 and fixed r4390 - (only the win32 *GDI* driver was calling exit routines twice)
------------------------------------------------------------------------
r5024 | jeffmarshall | 2008-07-27 11:10:45 -0700 (Sun, 27 Jul 2008) | 1 line

updated current (v0.20.0) manifests
------------------------------------------------------------------------
r5023 | jeffmarshall | 2008-07-27 11:10:05 -0700 (Sun, 27 Jul 2008) | 1 line

updated migrating.txt from CompilerDialects
------------------------------------------------------------------------
r5020 | jeffmarshall | 2008-07-27 07:22:04 -0700 (Sun, 27 Jul 2008) | 1 line

updated examples/manual from wiki
------------------------------------------------------------------------
r5012 | jeffmarshall | 2008-07-26 19:13:35 -0700 (Sat, 26 Jul 2008) | 1 line

update version numbers to 0.20.0 in configure files
------------------------------------------------------------------------
r5011 | jeffmarshall | 2008-07-26 19:13:08 -0700 (Sat, 26 Jul 2008) | 1 line

update version numbers to 0.20.0 in configure.ac files
------------------------------------------------------------------------
r5010 | i_am_drv | 2008-07-26 08:55:04 -0700 (Sat, 26 Jul 2008) | 1 line

Revert accidental commits to other files from previous commit
------------------------------------------------------------------------
r5009 | i_am_drv | 2008-07-26 08:40:21 -0700 (Sat, 26 Jul 2008) | 1 line

minor spelling fix
------------------------------------------------------------------------
r5008 | jeffmarshall | 2008-07-25 21:42:54 -0700 (Fri, 25 Jul 2008) | 1 line

small fix: premature unlock in win32 gfx driver
------------------------------------------------------------------------
r5007 | jeffmarshall | 2008-07-25 13:26:53 -0700 (Fri, 25 Jul 2008) | 1 line

win32 installer: add options for caca.bi, jpgalleg.bi, quicklz.bi
------------------------------------------------------------------------
r5006 | jeffmarshall | 2008-07-25 13:17:57 -0700 (Fri, 25 Jul 2008) | 1 line

removed: image_compat.bi
------------------------------------------------------------------------
r5005 | jeffmarshall | 2008-07-25 13:09:44 -0700 (Fri, 25 Jul 2008) | 1 line

removed: inc/mini.bi, example, and wrapper
------------------------------------------------------------------------
r5004 | jeffmarshall | 2008-07-25 12:40:18 -0700 (Fri, 25 Jul 2008) | 1 line

win32 installer: libwinspool.dll.a now installed by default
------------------------------------------------------------------------
r5003 | counting_pine | 2008-07-25 11:21:18 -0700 (Fri, 25 Jul 2008) | 2 lines

Minor example explicit/pedantic fixes

------------------------------------------------------------------------
r5002 | jeffmarshall | 2008-07-25 07:03:36 -0700 (Fri, 25 Jul 2008) | 1 line

updated readme.txt credits: sync with fb.net and wiki
------------------------------------------------------------------------
r5001 | i_am_drv | 2008-07-24 15:52:36 -0700 (Thu, 24 Jul 2008) | 3 lines

Fix Darwin .ctors/.dtors sections (untested)
Patch by fefeman (bug #1947345)

------------------------------------------------------------------------
r5000 | jeffmarshall | 2008-07-23 06:25:55 -0700 (Wed, 23 Jul 2008) | 1 line

updated fbc.1 man page - added -C and -R options
------------------------------------------------------------------------
r4999 | jeffmarshall | 2008-07-22 04:07:11 -0700 (Tue, 22 Jul 2008) | 1 line

updated manifests (v0.20.0)
------------------------------------------------------------------------
r4998 | jeffmarshall | 2008-07-22 03:58:42 -0700 (Tue, 22 Jul 2008) | 4 lines

updated win32 installer:
- update version to 0.20.0b
- copy hash.bas and hash.bi from fbdocs/fbdoc

------------------------------------------------------------------------
r4995 | jeffmarshall | 2008-07-21 12:55:48 -0700 (Mon, 21 Jul 2008) | 1 line

samples.bas: remove -lang comment checks - wiki examples use #lang/$lang
------------------------------------------------------------------------
r4994 | jeffmarshall | 2008-07-21 12:53:43 -0700 (Mon, 21 Jul 2008) | 1 line

updated examples from wiki (using #lang/$lang)
------------------------------------------------------------------------
r4993 | i_am_drv | 2008-07-21 10:44:11 -0700 (Mon, 21 Jul 2008) | 1 line

win32 gfxlib: reset lock count on exit if user left screen locked
------------------------------------------------------------------------
r4992 | jeffmarshall | 2008-07-21 09:00:00 -0700 (Mon, 21 Jul 2008) | 1 line

updated wiki example
------------------------------------------------------------------------
r4991 | jeffmarshall | 2008-07-21 08:24:57 -0700 (Mon, 21 Jul 2008) | 1 line

fixed #2016487: inc/win/vfw.bi - Changed "Delete" to "Delete__" and ported the remaining IC* AVI* and MCI* defines
------------------------------------------------------------------------
r4990 | jeffmarshall | 2008-07-21 06:53:30 -0700 (Mon, 21 Jul 2008) | 1 line

minor fix: removed SCREEN_AUTOLOCKED in libfb_gfx_win32.c as it is no longer defined (needs testing).
------------------------------------------------------------------------
r4989 | counting_pine | 2008-07-20 15:21:19 -0700 (Sun, 20 Jul 2008) | 1 line

Some of the qb tests were trying to use LEN with types
------------------------------------------------------------------------
r4988 | i_am_drv | 2008-07-20 14:57:24 -0700 (Sun, 20 Jul 2008) | 1 line

update changelog
------------------------------------------------------------------------
r4987 | i_am_drv | 2008-07-20 14:23:07 -0700 (Sun, 20 Jul 2008) | 1 line

On Linux, only use our own linker script on standalone builds
------------------------------------------------------------------------
r4986 | i_am_drv | 2008-07-20 14:09:13 -0700 (Sun, 20 Jul 2008) | 1 line

Add reference count to SCREENLOCK/SCREENUNLOCK
------------------------------------------------------------------------
r4985 | counting_pine | 2008-07-20 11:58:06 -0700 (Sun, 20 Jul 2008) | 2 lines

Add DIR overloads with byref uinteger params for out_attrib (non-optional, to prevent overload ambiguities when omitted)

------------------------------------------------------------------------
r4984 | counting_pine | 2008-07-17 17:59:47 -0700 (Thu, 17 Jul 2008) | 2 lines

Remove uinteger ptr support from DIR: was causing abmiguity when parameter was omitted

------------------------------------------------------------------------
r4983 | jeffmarshall | 2008-07-16 14:04:55 -0700 (Wed, 16 Jul 2008) | 1 line

small change: make "lang" case insensitive
------------------------------------------------------------------------
r4980 | jeffmarshall | 2008-07-15 20:14:46 -0700 (Tue, 15 Jul 2008) | 1 line

updated wiki examples
------------------------------------------------------------------------
r4978 | jeffmarshall | 2008-07-15 19:12:12 -0700 (Tue, 15 Jul 2008) | 1 line

added: #lang directive and $lang metacommand to set compiler dialect from source code
------------------------------------------------------------------------
r4977 | jeffmarshall | 2008-07-15 05:43:42 -0700 (Tue, 15 Jul 2008) | 1 line

updated: (internal) enum FB_RTERROR not the same as in rtlib/fb_error.h
------------------------------------------------------------------------
r4976 | counting_pine | 2008-07-13 19:07:32 -0700 (Sun, 13 Jul 2008) | 2 lines

Add helpful error message for when GOSUB is used without OPTION GOSUB

------------------------------------------------------------------------
r4975 | jeffmarshall | 2008-07-13 16:19:41 -0700 (Sun, 13 Jul 2008) | 1 line

improved error messages for GET#/PUT# - warn if passing a pointer as source/destination, error if elements is given on string or array
------------------------------------------------------------------------
r4974 | jeffmarshall | 2008-07-13 11:35:02 -0700 (Sun, 13 Jul 2008) | 1 line

added #1515157 (feature request) - GET# returns number of bytes read in 5th byref parameter (jeffm)
------------------------------------------------------------------------
r4973 | jeffmarshall | 2008-07-13 11:33:28 -0700 (Sun, 13 Jul 2008) | 1 line

added #1515157 (feature request) - GET# returns number of bytes read in 5th byref parameter (jeffm)
------------------------------------------------------------------------
r4972 | jeffmarshall | 2008-07-12 19:23:12 -0700 (Sat, 12 Jul 2008) | 1 line

updated manifests (added examples from wiki)
------------------------------------------------------------------------
r4970 | jeffmarshall | 2008-07-12 19:11:11 -0700 (Sat, 12 Jul 2008) | 1 line

wiki examples updated/added
------------------------------------------------------------------------
r4969 | counting_pine | 2008-07-11 17:26:21 -0700 (Fri, 11 Jul 2008) | 2 lines

Allow uinteger ptrs for out_attrib in DIR, by adding a couple of extra overloads

------------------------------------------------------------------------
r4968 | whotookcha0s-_- | 2008-07-06 01:33:38 -0700 (Sun, 06 Jul 2008) | 1 line

small fix to unix thread stack size stuff
------------------------------------------------------------------------
r4967 | i_am_drv | 2008-07-05 13:36:31 -0700 (Sat, 05 Jul 2008) | 3 lines

BLOAD fixes and improvements:
- load alpha from BMP files with BITMAPV4HEADER/BITMAPV5HEADER
- fix alpha on other formats when converting to 32bpp
------------------------------------------------------------------------
r4966 | i_am_drv | 2008-07-05 13:05:57 -0700 (Sat, 05 Jul 2008) | 1 line

remove accidental printf debugging output from last commit
------------------------------------------------------------------------
r4965 | i_am_drv | 2008-07-05 13:03:21 -0700 (Sat, 05 Jul 2008) | 2 lines

BLOAD fixes and improvements:
- load BMP files with BITMAPV4HEADER/BITMAPV5HEADER
------------------------------------------------------------------------
r4964 | i_am_drv | 2008-07-05 12:50:21 -0700 (Sat, 05 Jul 2008) | 2 lines

BLOAD fixes and improvements:
- load BMP files with BITMAPINFOHEADER2 (OS/2 V2 BMP)
------------------------------------------------------------------------
r4963 | i_am_drv | 2008-07-05 12:26:43 -0700 (Sat, 05 Jul 2008) | 2 lines

BLOAD fixes and improvements:
- load flipped BMP files (biHeight < 0)
------------------------------------------------------------------------
r4962 | i_am_drv | 2008-07-05 12:11:47 -0700 (Sat, 05 Jul 2008) | 2 lines

BLOAD fixes and improvements:
- load BMP files with BITMAPCOREHEADER (OS/2 BMP)
------------------------------------------------------------------------
r4961 | i_am_drv | 2008-07-05 10:42:38 -0700 (Sat, 05 Jul 2008) | 3 lines

BLOAD fixes and improvements:
- load 15, 16, 32 bpp BMP files
- handle arbitrary BI_BITFIELDS-style BMP files
------------------------------------------------------------------------
r4960 | jeffmarshall | 2008-07-02 21:09:43 -0700 (Wed, 02 Jul 2008) | 1 line

fixed #1979795 - GETMOUSE in Win32 GFX driver was not clearing the status of the XBUTTON's correctly.
------------------------------------------------------------------------
r4959 | i_am_drv | 2008-07-02 10:40:18 -0700 (Wed, 02 Jul 2008) | 1 line

Fix gai_strerror (was inline function in C header)
------------------------------------------------------------------------
r4958 | jeffmarshall | 2008-07-01 10:16:48 -0700 (Tue, 01 Jul 2008) | 1 line

fixed #1914763 - LINE INPUT to a WSTRING variable on an ascii text file was failing if last line of file had no end of line character
------------------------------------------------------------------------
r4957 | jeffmarshall | 2008-06-29 12:39:13 -0700 (Sun, 29 Jun 2008) | 1 line

fixed #2004439 - LSET/RSET was not deleting temporary string descriptors for the destination
------------------------------------------------------------------------
r4956 | jeffmarshall | 2008-06-29 12:37:09 -0700 (Sun, 29 Jun 2008) | 1 line

tidy warnings
------------------------------------------------------------------------
r4955 | jeffmarshall | 2008-06-29 09:26:52 -0700 (Sun, 29 Jun 2008) | 1 line

updated wiki examples
------------------------------------------------------------------------
r4952 | i_am_drv | 2008-06-27 06:26:41 -0700 (Fri, 27 Jun 2008) | 1 line

astDumpTree improvements from cfg branch
------------------------------------------------------------------------
r4949 | counting_pine | 2008-06-25 20:57:17 -0700 (Wed, 25 Jun 2008) | 1 line

Fix image_compat.bi
------------------------------------------------------------------------
r4947 | counting_pine | 2008-06-25 20:50:23 -0700 (Wed, 25 Jun 2008) | 1 line

QuickLZ: pedantic fixes, add version info
------------------------------------------------------------------------
r4943 | counting_pine | 2008-06-25 16:20:38 -0700 (Wed, 25 Jun 2008) | 1 line

CACA examples: cosmetic changes and fixes (while not -> until, case insensitivity translation issues)
------------------------------------------------------------------------
r4942 | counting_pine | 2008-06-25 15:22:03 -0700 (Wed, 25 Jun 2008) | 1 line

fix caca.bi declares - sub mislabeled as function, one too many int params on triangle-drawing subs
------------------------------------------------------------------------
r4933 | i_am_drv | 2008-06-17 06:27:21 -0700 (Tue, 17 Jun 2008) | 1 line

Do center window on X11 driver for fullscreen or no border modes
------------------------------------------------------------------------
r4932 | i_am_drv | 2008-06-17 06:18:46 -0700 (Tue, 17 Jun 2008) | 1 line

better fix for static c_dfDIKeyboard, c_rgodfDIKeyboard
------------------------------------------------------------------------
r4889 | jeffmarshall | 2008-06-14 18:05:18 -0700 (Sat, 14 Jun 2008) | 2 lines

Move FreeBASIC/src/contrib/fbdoc to fbdocs/fbdoc
- Moved remotely
------------------------------------------------------------------------
r4884 | i_am_drv | 2008-06-13 13:21:44 -0700 (Fri, 13 Jun 2008) | 1 line

Find 32-bit libc on 64-bit linux
------------------------------------------------------------------------
r4883 | i_am_drv | 2008-06-13 13:01:08 -0700 (Fri, 13 Jun 2008) | 1 line

64-bit linux frontend fixes
------------------------------------------------------------------------
r4882 | i_am_drv | 2008-06-13 06:02:35 -0700 (Fri, 13 Jun 2008) | 1 line

Fix win32 gfxlib2 build with gcc 4.3
------------------------------------------------------------------------
r4881 | i_am_drv | 2008-06-13 05:53:01 -0700 (Fri, 13 Jun 2008) | 1 line

Fix DOS build (include stdint.h and use it in fb_unicode.h)
------------------------------------------------------------------------
r4880 | i_am_drv | 2008-06-12 12:42:41 -0700 (Thu, 12 Jun 2008) | 1 line

Remove unused vars (-Wall fixes)
------------------------------------------------------------------------
r4879 | i_am_drv | 2008-06-12 12:40:37 -0700 (Thu, 12 Jun 2008) | 1 line

64-bit gfxlib2 fixes
------------------------------------------------------------------------
r4878 | i_am_drv | 2008-06-12 12:36:41 -0700 (Thu, 12 Jun 2008) | 1 line

Add x86_64 configure support for gfxlib2
------------------------------------------------------------------------
r4877 | i_am_drv | 2008-06-12 12:34:18 -0700 (Thu, 12 Jun 2008) | 1 line

Fix swprintf warnings with newer glibc headers
------------------------------------------------------------------------
r4876 | i_am_drv | 2008-06-12 12:11:39 -0700 (Thu, 12 Jun 2008) | 1 line

rtlib 64-bit fixes
------------------------------------------------------------------------
r4875 | jeffmarshall | 2008-06-08 05:12:54 -0700 (Sun, 08 Jun 2008) | 1 line

andalso/orelse was not being allowed with pointer data types, regression test added.
------------------------------------------------------------------------
r4874 | jeffmarshall | 2008-06-08 05:10:20 -0700 (Sun, 08 Jun 2008) | 1 line

lang qb: LEN() should only allow expressions (not TYPE's or TYPEDEF's)
------------------------------------------------------------------------
r4873 | yetifoot | 2008-06-06 06:43:41 -0700 (Fri, 06 Jun 2008) | 1 line

c emitter: better pointer arithmatic and other improvements
------------------------------------------------------------------------
r4872 | yetifoot | 2008-06-05 16:35:43 -0700 (Thu, 05 Jun 2008) | 1 line

c emitter: clean up some BOP code, expand quirks, better / behaviour, other changes
------------------------------------------------------------------------
r4871 | yetifoot | 2008-06-04 19:27:20 -0700 (Wed, 04 Jun 2008) | 1 line

c emitter: cast proc args in call
------------------------------------------------------------------------
r4870 | yetifoot | 2008-06-04 18:18:41 -0700 (Wed, 04 Jun 2008) | 1 line

c emitter: fix floating point divide
------------------------------------------------------------------------
r4869 | yetifoot | 2008-06-04 17:36:59 -0700 (Wed, 04 Jun 2008) | 1 line

no warning for unclosed literal strings when within comments
------------------------------------------------------------------------
r4868 | yetifoot | 2008-06-04 17:10:11 -0700 (Wed, 04 Jun 2008) | 1 line

catch some pointer arithmatic, and make it 1 based
------------------------------------------------------------------------
r4867 | yetifoot | 2008-06-04 02:16:38 -0700 (Wed, 04 Jun 2008) | 1 line

more improvements to c emitter, arrays
------------------------------------------------------------------------
r4866 | yetifoot | 2008-06-03 18:56:28 -0700 (Tue, 03 Jun 2008) | 1 line

better arg stack and literal strings
------------------------------------------------------------------------
r4865 | yetifoot | 2008-06-03 02:59:39 -0700 (Tue, 03 Jun 2008) | 1 line

tidy
------------------------------------------------------------------------
r4864 | yetifoot | 2008-06-03 02:53:21 -0700 (Tue, 03 Jun 2008) | 1 line

tidy
------------------------------------------------------------------------
r4863 | yetifoot | 2008-06-03 02:30:52 -0700 (Tue, 03 Jun 2008) | 1 line

fix a couple of issues in -gen gcc with procs
------------------------------------------------------------------------
r4862 | jeffmarshall | 2008-06-02 09:58:40 -0700 (Mon, 02 Jun 2008) | 3 lines

SSE/SSE2:
- clarify a few code comments

------------------------------------------------------------------------
r4861 | jeffmarshall | 2008-06-02 09:57:59 -0700 (Mon, 02 Jun 2008) | 3 lines

SSE/SSE2: (Bryan S.)
- Exit application if SSE/SSE2 check fails at start-up
- OPTION("") not required on function header if is specified on function declaration
------------------------------------------------------------------------
r4858 | jeffmarshall | 2008-05-31 16:53:16 -0700 (Sat, 31 May 2008) | 1 line

internal: var_.align was being used uninitialized
------------------------------------------------------------------------
r4857 | jeffmarshall | 2008-05-31 13:38:20 -0700 (Sat, 31 May 2008) | 1 line

updated current 0.20 manifests
------------------------------------------------------------------------
r4856 | jeffmarshall | 2008-05-31 13:33:23 -0700 (Sat, 31 May 2008) | 1 line

updated/added wiki examples
------------------------------------------------------------------------
r4855 | jeffmarshall | 2008-05-31 13:31:25 -0700 (Sat, 31 May 2008) | 1 line

added keywords AndAlso, OrElse, __FB_SSE__
------------------------------------------------------------------------
r4854 | jeffmarshall | 2008-05-31 07:46:08 -0700 (Sat, 31 May 2008) | 1 line

updated (SSE/SSE2 log messages)
------------------------------------------------------------------------
r4853 | jeffmarshall | 2008-05-31 07:44:10 -0700 (Sat, 31 May 2008) | 1 line

small fix: closing quotes on string literals
------------------------------------------------------------------------
r4852 | jeffmarshall | 2008-05-31 07:39:33 -0700 (Sat, 31 May 2008) | 2 lines

Add "FPU" option to test-suite

------------------------------------------------------------------------
r4851 | jeffmarshall | 2008-05-31 07:35:40 -0700 (Sat, 31 May 2008) | 6 lines

added (thanks Bryan Stoeberl):
- SSE/SSE2 instructions in x86 emitter for floating point math.
- "-fpu FPU|SSE" command line switch.
- OPTION("FPU|SSE") to specify proc return method.
- CPU check for SSE bits

------------------------------------------------------------------------
r4848 | counting_pine | 2008-05-30 20:01:09 -0700 (Fri, 30 May 2008) | 1 line

make CIRCLE's aspect ratio independent of the WINDOW/VIEW ratio
------------------------------------------------------------------------
r4847 | counting_pine | 2008-05-30 18:07:56 -0700 (Fri, 30 May 2008) | 1 line

make SCREENRES set a default aspect ratio of 1.0 for CIRCLE
------------------------------------------------------------------------
r4846 | i_am_drv | 2008-05-29 16:42:34 -0700 (Thu, 29 May 2008) | 1 line

ignore cache and texinfo files
------------------------------------------------------------------------
r4845 | i_am_drv | 2008-05-29 11:59:30 -0700 (Thu, 29 May 2008) | 1 line

add declaration of fb_FileInputNextTokenWstr
------------------------------------------------------------------------
r4844 | jeffmarshall | 2008-05-27 16:23:30 -0700 (Tue, 27 May 2008) | 3 lines

fixed:
- normal warning if CPTR is casting to non-pointer or non-integer
- pedantic warning if CPTR is casting to anything but pointer
------------------------------------------------------------------------
r4843 | jeffmarshall | 2008-05-27 16:21:27 -0700 (Tue, 27 May 2008) | 1 line

small fix: only show note on how to get target specific options if "-target" option is available.
------------------------------------------------------------------------
r4842 | yetifoot | 2008-05-26 10:54:14 -0700 (Mon, 26 May 2008) | 1 line

add note on how to get target specific options shown in help screen when cross compiling
------------------------------------------------------------------------
r4840 | counting_pine | 2008-05-23 17:53:26 -0700 (Fri, 23 May 2008) | 5 lines

Add '-R', '-C' command-line switches:
- -R preserves asm, -C preserves obj
- -r now means "output asm only"
- if -c is present, -r works the same as -R

------------------------------------------------------------------------
r4838 | yetifoot | 2008-05-22 23:43:23 -0700 (Thu, 22 May 2008) | 1 line

disallow overloading andalso/orelse
------------------------------------------------------------------------
r4837 | yetifoot | 2008-05-22 22:47:56 -0700 (Thu, 22 May 2008) | 1 line

test for andalso/orelse
------------------------------------------------------------------------
r4833 | yetifoot | 2008-05-22 19:57:57 -0700 (Thu, 22 May 2008) | 1 line

make andalso/orelse behave better with floats
------------------------------------------------------------------------
r4832 | counting_pine | 2008-05-21 10:10:16 -0700 (Wed, 21 May 2008) | 1 line

Make RSET work like LSET (allow '=', const checking)
------------------------------------------------------------------------
r4831 | counting_pine | 2008-05-20 17:29:42 -0700 (Tue, 20 May 2008) | 1 line

update changelog for r4830
------------------------------------------------------------------------
r4830 | counting_pine | 2008-05-20 11:55:49 -0700 (Tue, 20 May 2008) | 1 line

#1961732 - LSET wasn't accepting dereferenced vars for the destination
------------------------------------------------------------------------
r4829 | yetifoot | 2008-05-19 13:28:36 -0700 (Mon, 19 May 2008) | 1 line

ANDALSO/ORELSE
------------------------------------------------------------------------
r4828 | whotookcha0s-_- | 2008-05-17 19:54:37 -0700 (Sat, 17 May 2008) | 1 line

changed: the binary operator overload resolution logic... Now an operator is only eligible if at least one side of the expression matches its parameter list exactly, without implicit coercion/casting.
------------------------------------------------------------------------
r4827 | counting_pine | 2008-05-17 19:30:31 -0700 (Sat, 17 May 2008) | 1 line

CV__: Disallow constant expressions for CVS/CVD, and for cases where *zs is not reported to be long enough
------------------------------------------------------------------------
r4826 | jeffmarshall | 2008-05-15 16:11:53 -0700 (Thu, 15 May 2008) | 3 lines

added:
- "-z opt" command line option for miscellaneous options
- "-z gosub-setjmp" to select setjmp/longjmp implementation even when backend is ASM
------------------------------------------------------------------------
r4825 | jeffmarshall | 2008-05-15 15:33:44 -0700 (Thu, 15 May 2008) | 1 line

add "ARCH" option to test-suite
------------------------------------------------------------------------
r4824 | jeffmarshall | 2008-05-15 15:28:31 -0700 (Thu, 15 May 2008) | 1 line

small fix: remove unused variable
------------------------------------------------------------------------
r4823 | counting_pine | 2008-05-15 09:39:56 -0700 (Thu, 15 May 2008) | 1 line

Small decimal escape fix; re-allow leading zeroes; add \x.., \X.. support
------------------------------------------------------------------------
r4822 | counting_pine | 2008-05-09 12:04:17 -0700 (Fri, 09 May 2008) | 1 line

Pedantic fix - add byvals
------------------------------------------------------------------------
r4821 | jeffmarshall | 2008-05-08 07:08:49 -0700 (Thu, 08 May 2008) | 1 line

updated: 0.20.0 current manifests
------------------------------------------------------------------------
r4820 | jeffmarshall | 2008-05-08 06:34:27 -0700 (Thu, 08 May 2008) | 1 line

added image from wiki
------------------------------------------------------------------------
r4819 | jeffmarshall | 2008-05-08 06:33:28 -0700 (Thu, 08 May 2008) | 1 line

added/updated examples/manual from wiki
------------------------------------------------------------------------
r4818 | jeffmarshall | 2008-05-08 04:31:22 -0700 (Thu, 08 May 2008) | 7 lines

merged v0_20-gosub (r4789:4815) branch to trunk:
- lang qb can now use all the OPTIONs fblite can, e.g. OPTION STATIC|BYVAL|... (counting_pine)
- C emitter: GOSUB and ON GOSUB using setjmp/longjmp (jeffm)
- fblite, qb: OPTION GOSUB, OPTION NOGOSUB to enable/disable GOSUB support. State can be checked with the intrinsic define __FB_OPTION_GOSUB__ (jeffm)
- #1899251 (feature request) - RETURN without GOSUB runtime error check for both ASM and setjmp/longjump implementations of GOSUB (jeffm)


------------------------------------------------------------------------
r4817 | jeffmarshall | 2008-05-08 04:29:25 -0700 (Thu, 08 May 2008) | 7 lines

merged v0_20-gosub (r4789:4815) branch to trunk:
- lang qb can now use all the OPTIONs fblite can, e.g. OPTION STATIC|BYVAL|... (counting_pine)
- C emitter: GOSUB and ON GOSUB using setjmp/longjmp (jeffm)
- fblite, qb: OPTION GOSUB, OPTION NOGOSUB to enable/disable GOSUB support. State can be checked with the intrinsic define __FB_OPTION_GOSUB__ (jeffm)
- #1899251 (feature request) - RETURN without GOSUB runtime error check for both ASM and setjmp/longjump implementations of GOSUB (jeffm)


------------------------------------------------------------------------
r4816 | jeffmarshall | 2008-05-08 04:25:13 -0700 (Thu, 08 May 2008) | 1 line

added: rtlib support for GOSUB using setjmp/longjmp (selected only in the C emitter)
------------------------------------------------------------------------
r4814 | counting_pine | 2008-05-07 16:56:15 -0700 (Wed, 07 May 2008) | 3 lines

- Fix: Double-quotes in string literals were being removed too early
- Add warning message when strings aren't closed properly

------------------------------------------------------------------------
r4813 | whotookcha0s-_- | 2008-05-07 15:57:20 -0700 (Wed, 07 May 2008) | 1 line

patch to the C emitter submitted by sir_mud, several improvements including the ability to call functions. (WIP)
------------------------------------------------------------------------
r4810 | jeffmarshall | 2008-05-06 13:10:33 -0700 (Tue, 06 May 2008) | 1 line

small fix: "--32 " option in debug mode was missing trailing space.
------------------------------------------------------------------------
r4809 | i_am_drv | 2008-05-06 09:50:29 -0700 (Tue, 06 May 2008) | 1 line

add more longint shift tests
------------------------------------------------------------------------
r4808 | i_am_drv | 2008-05-06 09:44:28 -0700 (Tue, 06 May 2008) | 1 line

non-imm hSHIFTL back to using stack
------------------------------------------------------------------------
r4807 | yetifoot | 2008-05-05 18:21:37 -0700 (Mon, 05 May 2008) | 1 line

[1942805] patch from redcrab to stop errors when compiling on a 64bit system, due to defaults
------------------------------------------------------------------------
r4805 | counting_pine | 2008-05-05 11:38:53 -0700 (Mon, 05 May 2008) | 2 lines

- Fix for #1957281: Give Screen 12 a 256-color palette
- Alternative fix for #1892194: Limit Palette Get Using by bit depth, not just palette length
------------------------------------------------------------------------
r4795 | i_am_drv | 2008-04-29 06:44:07 -0700 (Tue, 29 Apr 2008) | 1 line

spelling
------------------------------------------------------------------------
r4792 | jeffmarshall | 2008-04-28 18:55:45 -0700 (Mon, 28 Apr 2008) | 1 line

fixed: uninitialized return value
------------------------------------------------------------------------
r4787 | jeffmarshall | 2008-04-28 12:26:18 -0700 (Mon, 28 Apr 2008) | 1 line

added new wiki examples to current (0.20) manifest
------------------------------------------------------------------------
r4786 | jeffmarshall | 2008-04-28 11:57:02 -0700 (Mon, 28 Apr 2008) | 1 line

updated examples from wiki
------------------------------------------------------------------------
r4785 | i_am_drv | 2008-04-25 01:26:36 -0700 (Fri, 25 Apr 2008) | 1 line

only check for X when necessary
------------------------------------------------------------------------
r4784 | jeffmarshall | 2008-04-24 14:27:05 -0700 (Thu, 24 Apr 2008) | 1 line

fixed: typo in font names (thanks AGS)
------------------------------------------------------------------------
r4783 | jeffmarshall | 2008-04-24 13:45:55 -0700 (Thu, 24 Apr 2008) | 1 line

updated examples from wiki
------------------------------------------------------------------------
r4782 | jeffmarshall | 2008-04-24 09:33:52 -0700 (Thu, 24 Apr 2008) | 1 line

updated: log messages (jeffm)
------------------------------------------------------------------------
r4781 | jeffmarshall | 2008-04-24 09:27:14 -0700 (Thu, 24 Apr 2008) | 1 line

fixed: linux serial driver wasn't handling device names correctly.
------------------------------------------------------------------------
r4780 | jeffmarshall | 2008-04-19 15:12:15 -0700 (Sat, 19 Apr 2008) | 1 line

merge r4777 from 0.18 branch (copy 0.18.5 manifests from current)
------------------------------------------------------------------------
r4776 | counting_pine | 2008-04-18 13:33:47 -0700 (Fri, 18 Apr 2008) | 1 line

Further WRITE fixes
------------------------------------------------------------------------
r4775 | counting_pine | 2008-04-18 13:00:23 -0700 (Fri, 18 Apr 2008) | 1 line

Fix WRITE empty string optimizations
------------------------------------------------------------------------
r4774 | i_am_drv | 2008-04-18 09:14:52 -0700 (Fri, 18 Apr 2008) | 1 line

update LUID structure (alignment issues)
------------------------------------------------------------------------
r4773 | i_am_drv | 2008-04-18 07:07:04 -0700 (Fri, 18 Apr 2008) | 1 line

fixed hSHIFTL changes
------------------------------------------------------------------------
r4772 | i_am_drv | 2008-04-18 06:49:01 -0700 (Fri, 18 Apr 2008) | 1 line

revert hSHIFTL changes (broke bit___ test)
------------------------------------------------------------------------
r4771 | i_am_drv | 2008-04-18 06:27:26 -0700 (Fri, 18 Apr 2008) | 1 line

rewrite of hSHIFTL (avoid using stack or specific regs when unnecessary)
------------------------------------------------------------------------
r4770 | i_am_drv | 2008-04-18 02:13:25 -0700 (Fri, 18 Apr 2008) | 1 line

make fb_CpuDetect return family in top 4 bits instead of 8 to make room for more feature flags
------------------------------------------------------------------------
r4769 | i_am_drv | 2008-04-18 01:58:57 -0700 (Fri, 18 Apr 2008) | 1 line

indentation cleanup
------------------------------------------------------------------------
r4768 | jeffmarshall | 2008-04-17 17:57:35 -0700 (Thu, 17 Apr 2008) | 4 lines

merge r4765:4767 from 0.18 branch to trunk
- permissions in install.sh and install-standalone.sh must be applied recursively (thanks cubaxd)
- missing avicap32 and gdiplus in installer 
- removed libglut.dll.a and libglaux.dll.a from win32 manifest (not used)
------------------------------------------------------------------------
r4764 | jeffmarshall | 2008-04-17 10:30:33 -0700 (Thu, 17 Apr 2008) | 1 line

log message for install-standalone.sh
------------------------------------------------------------------------
r4762 | jeffmarshall | 2008-04-17 06:57:20 -0700 (Thu, 17 Apr 2008) | 1 line

install.sh should have been install-standalone.sh in one instance
------------------------------------------------------------------------
r4760 | jeffmarshall | 2008-04-16 19:50:48 -0700 (Wed, 16 Apr 2008) | 4 lines

merge r4754:4759 from 0.18 branch to trunk
- updated install.sh
- added install-standalone.sh
- updated instructions in readme.txt
------------------------------------------------------------------------
r4753 | i_am_drv | 2008-04-16 11:19:44 -0700 (Wed, 16 Apr 2008) | 1 line

update man page version on trunk
------------------------------------------------------------------------
r4752 | counting_pine | 2008-04-16 10:48:12 -0700 (Wed, 16 Apr 2008) | 1 line

Fix ICE on ReEscaping strings in unicode sources
------------------------------------------------------------------------
r4751 | jeffmarshall | 2008-04-16 10:31:41 -0700 (Wed, 16 Apr 2008) | 2 lines

ported r4750 to trunk
- fbc-0.18: additional notes on installing (wip for linux).
------------------------------------------------------------------------
r4748 | counting_pine | 2008-04-14 17:27:51 -0700 (Mon, 14 Apr 2008) | 1 line

Remove __fb_ctx.lang dependency in FB_PRINTNUM_EX
------------------------------------------------------------------------
r4747 | jeffmarshall | 2008-04-13 05:37:05 -0700 (Sun, 13 Apr 2008) | 1 line

added: imageinfo.png from wiki
------------------------------------------------------------------------
r4746 | counting_pine | 2008-04-12 04:32:38 -0700 (Sat, 12 Apr 2008) | 1 line

Stop Get/Put truncating padded UDTs
------------------------------------------------------------------------
r4745 | counting_pine | 2008-04-12 04:23:57 -0700 (Sat, 12 Apr 2008) | 1 line

rtl comment fixes
------------------------------------------------------------------------
r4744 | jeffmarshall | 2008-04-11 17:32:49 -0700 (Fri, 11 Apr 2008) | 1 line

updated: current 0.20.0 manifests
------------------------------------------------------------------------
r4743 | jeffmarshall | 2008-04-11 17:23:47 -0700 (Fri, 11 Apr 2008) | 1 line

sync manual/examples with wiki
------------------------------------------------------------------------
r4742 | jeffmarshall | 2008-04-11 17:22:29 -0700 (Fri, 11 Apr 2008) | 1 line

added test (from 0.18 branch)
------------------------------------------------------------------------
r4741 | jeffmarshall | 2008-04-11 07:12:30 -0700 (Fri, 11 Apr 2008) | 1 line

updated
------------------------------------------------------------------------
r4738 | jeffmarshall | 2008-04-09 22:46:52 -0700 (Wed, 09 Apr 2008) | 1 line

lang qb: prototypes for overloaded procdefs were not being added correctly
------------------------------------------------------------------------
r4737 | i_am_drv | 2008-04-09 15:58:27 -0700 (Wed, 09 Apr 2008) | 1 line

allow building gfxlib without X on linux (fbdev only)
------------------------------------------------------------------------
r4736 | i_am_drv | 2008-04-09 15:37:48 -0700 (Wed, 09 Apr 2008) | 1 line

add WITH_X define to gfxlib also (not used yet)
------------------------------------------------------------------------
r4735 | jeffmarshall | 2008-04-09 15:29:05 -0700 (Wed, 09 Apr 2008) | 1 line

fixed: gfxlib win32 driver was calling exit routines twice
------------------------------------------------------------------------
r4734 | counting_pine | 2008-04-09 09:57:03 -0700 (Wed, 09 Apr 2008) | 1 line

Swap two arguments in IMAGEINFO; add to keywords.lst
------------------------------------------------------------------------
r4733 | jeffmarshall | 2008-04-09 08:53:29 -0700 (Wed, 09 Apr 2008) | 1 line

fbdoc: added texinfo directory
------------------------------------------------------------------------
r4732 | jeffmarshall | 2008-04-09 08:38:24 -0700 (Wed, 09 Apr 2008) | 1 line

fbdoc: added half working texinfo emitter (W.I.P.) - based on existing plain text emitter.
------------------------------------------------------------------------
r4731 | jeffmarshall | 2008-04-09 08:33:52 -0700 (Wed, 09 Apr 2008) | 1 line

added wiki example
------------------------------------------------------------------------
r4730 | counting_pine | 2008-04-09 07:44:21 -0700 (Wed, 09 Apr 2008) | 1 line

Remove IMAGEINFO from lang qb namespace
------------------------------------------------------------------------
r4729 | jeffmarshall | 2008-04-08 14:28:22 -0700 (Tue, 08 Apr 2008) | 1 line

fixed #1861363 - gfxlib win32 gdi driver was using the alpha_remover_blitter on depths < 32 bits
------------------------------------------------------------------------
r4728 | yetifoot | 2008-04-08 13:06:28 -0700 (Tue, 08 Apr 2008) | 1 line

some fixed
------------------------------------------------------------------------
r4727 | jeffmarshall | 2008-04-08 12:58:08 -0700 (Tue, 08 Apr 2008) | 1 line

small fix: -lang qb/fblite was allowing dups of unsuffixed variables having different types
------------------------------------------------------------------------
r4726 | yetifoot | 2008-04-08 12:21:28 -0700 (Tue, 08 Apr 2008) | 1 line

[1934048] added cdecls to example file
------------------------------------------------------------------------
r4725 | counting_pine | 2008-04-08 10:07:30 -0700 (Tue, 08 Apr 2008) | 1 line

Add IMAGEINFO function
------------------------------------------------------------------------
r4724 | counting_pine | 2008-04-08 09:17:48 -0700 (Tue, 08 Apr 2008) | 1 line

More lang qb STR fixes
------------------------------------------------------------------------
r4723 | i_am_drv | 2008-04-08 05:53:58 -0700 (Tue, 08 Apr 2008) | 1 line

optimize self comparison and assignment
------------------------------------------------------------------------
r4722 | jeffmarshall | 2008-04-07 22:41:14 -0700 (Mon, 07 Apr 2008) | 1 line

removed runtime check for dialect in BLOAD/IMAGECREATE/GET so they will respect the dialect where used even if main module is compiled under a different dialect.  qb/fblite uses old header and fb uses new-header.
------------------------------------------------------------------------
r4721 | counting_pine | 2008-04-07 17:39:56 -0700 (Mon, 07 Apr 2008) | 1 line

Make lang qb use separate routines for STR
------------------------------------------------------------------------
r4720 | counting_pine | 2008-04-07 10:51:36 -0700 (Mon, 07 Apr 2008) | 1 line

WIP: Add separate rtlib functions for STR in lang qb
------------------------------------------------------------------------
r4719 | i_am_drv | 2008-04-07 05:51:23 -0700 (Mon, 07 Apr 2008) | 1 line

make rtlib compile on darwin (functionality not tested yet)
------------------------------------------------------------------------
r4718 | yetifoot | 2008-04-06 18:24:25 -0700 (Sun, 06 Apr 2008) | 1 line

small fix to handle \a1 etc
------------------------------------------------------------------------
r4717 | yetifoot | 2008-04-06 17:52:55 -0700 (Sun, 06 Apr 2008) | 1 line

[1933706] \a now causes \7 in the GAS output
------------------------------------------------------------------------
r4716 | counting_pine | 2008-04-05 19:23:18 -0700 (Sat, 05 Apr 2008) | 1 line

Fix ASC inconsistencies: function returns uinteger, but consts were returning integer.
------------------------------------------------------------------------
r4715 | i_am_drv | 2008-04-05 18:48:21 -0700 (Sat, 05 Apr 2008) | 1 line

centralize user32 proc loading
------------------------------------------------------------------------
r4714 | i_am_drv | 2008-04-05 17:11:48 -0700 (Sat, 05 Apr 2008) | 1 line

use consistent string when loading user32.dll
------------------------------------------------------------------------
r4713 | i_am_drv | 2008-04-04 19:00:31 -0700 (Fri, 04 Apr 2008) | 1 line

split pipe device in preparation for bidirectional pipes
------------------------------------------------------------------------
r4712 | i_am_drv | 2008-04-04 18:50:15 -0700 (Fri, 04 Apr 2008) | 1 line

allow rtlib to be build without X (--without-x configure option or autodetected if X not installed)
------------------------------------------------------------------------
r4711 | i_am_drv | 2008-04-04 02:47:11 -0700 (Fri, 04 Apr 2008) | 1 line

fix line endings
------------------------------------------------------------------------
r4710 | i_am_drv | 2008-04-02 22:14:54 -0700 (Wed, 02 Apr 2008) | 1 line

fix filename case
------------------------------------------------------------------------
r4709 | jeffmarshall | 2008-04-01 13:24:31 -0700 (Tue, 01 Apr 2008) | 1 line

deprecated => fblite
------------------------------------------------------------------------
r4708 | yetifoot | 2008-04-01 09:02:17 -0700 (Tue, 01 Apr 2008) | 1 line

[1914051] stop \x and \0 escapes
------------------------------------------------------------------------
r4707 | counting_pine | 2008-03-31 20:39:03 -0700 (Mon, 31 Mar 2008) | 1 line

VAL saturation patches, plus test
------------------------------------------------------------------------
r4706 | counting_pine | 2008-03-30 21:08:03 -0700 (Sun, 30 Mar 2008) | 1 line

VAL: Workaround for 'd' exponent issue using temp buffer
------------------------------------------------------------------------
r4704 | jeffmarshall | 2008-03-30 14:04:46 -0700 (Sun, 30 Mar 2008) | 1 line

reverted: libsupc++.a is back on the gcc lib list.
------------------------------------------------------------------------
r4700 | jeffmarshall | 2008-03-30 08:24:16 -0700 (Sun, 30 Mar 2008) | 1 line

install.sh should use "=" for string comparison instead of "=="
------------------------------------------------------------------------
r4699 | counting_pine | 2008-03-30 06:16:45 -0700 (Sun, 30 Mar 2008) | 1 line

Stop PCOPY checking against max console pages when calling ScreenCopy
------------------------------------------------------------------------
r4698 | i_am_drv | 2008-03-30 02:32:38 -0700 (Sun, 30 Mar 2008) | 1 line

update changelog
------------------------------------------------------------------------
r4697 | i_am_drv | 2008-03-30 02:29:40 -0700 (Sun, 30 Mar 2008) | 1 line

x11 gfx driver: let the window manager pick the window position
------------------------------------------------------------------------
r4696 | whotookcha0s-_- | 2008-03-29 17:34:31 -0700 (Sat, 29 Mar 2008) | 1 line

set the configure scripts to always use unix-style line endings, to make it possible to compile from the same source tree on mingw32/linux
------------------------------------------------------------------------
r4695 | whotookcha0s-_- | 2008-03-29 17:25:09 -0700 (Sat, 29 Mar 2008) | 1 line

dynamic libs have to link in fb_program_icon on linux, otherwise dlopen (dylibload) can fail
------------------------------------------------------------------------
r4692 | jeffmarshall | 2008-03-28 12:30:50 -0700 (Fri, 28 Mar 2008) | 1 line

copy manifest/current to manifest/fbc-0.18.4
------------------------------------------------------------------------
r4691 | jeffmarshall | 2008-03-28 12:21:23 -0700 (Fri, 28 Mar 2008) | 4 lines

merged r4690 to trunk:
- added: --enable-standalone configuration option for backwards compatibility with 0.18.3 and earlier on linux.   
- changed: installation on an absolute prefix now uses "$prefix/lib/freebasic/<target>" and "$prefix/bin/freebasic/<target>" directories.
- internal: entries in pathTb must must have trailing path separator (removed RSLASH from dos/win32/xbox target modules)
------------------------------------------------------------------------
r4687 | jeffmarshall | 2008-03-27 04:32:54 -0700 (Thu, 27 Mar 2008) | 1 line

merge r4686 to trunk
------------------------------------------------------------------------
r4685 | jeffmarshall | 2008-03-25 21:17:32 -0700 (Tue, 25 Mar 2008) | 1 line

fbc to version 0.20.0
------------------------------------------------------------------------
r4684 | jeffmarshall | 2008-03-25 19:49:14 -0700 (Tue, 25 Mar 2008) | 1 line

merge 0.18 branch r4667:4683 to trunk
------------------------------------------------------------------------
r4681 | counting_pine | 2008-03-25 17:32:30 -0700 (Tue, 25 Mar 2008) | 1 line

More VAL* invalid character issues
------------------------------------------------------------------------
r4680 | whotookcha0s-_- | 2008-03-25 03:15:46 -0700 (Tue, 25 Mar 2008) | 1 line

included files weren't normalizing slashes before being cached
------------------------------------------------------------------------
r4673 | whotookcha0s-_- | 2008-03-24 12:45:40 -0700 (Mon, 24 Mar 2008) | 1 line

updated our freeimage interface
------------------------------------------------------------------------
r4672 | whotookcha0s-_- | 2008-03-24 12:41:51 -0700 (Mon, 24 Mar 2008) | 1 line

deleteme.txt added back in for distribution creation
------------------------------------------------------------------------
r4671 | whotookcha0s-_- | 2008-03-24 12:32:44 -0700 (Mon, 24 Mar 2008) | 3 lines

BFD was using '\' instead of host separator
added some echo command to BFD module (if DEBUG_BFD is defined)
fixed a lookup of typedef info, in the case of non-type typedefs
------------------------------------------------------------------------
r4664 | jeffmarshall | 2008-03-23 16:35:09 -0700 (Sun, 23 Mar 2008) | 1 line

add instructions to create the target platform directory if it does not exist
------------------------------------------------------------------------
r4663 | whotookcha0s-_- | 2008-03-22 23:18:19 -0700 (Sat, 22 Mar 2008) | 3 lines

gcc libraries can be substituted by a user's in prefix/lib/freebasic
added the LINK_STATIC option to Makefile.in
tram calls tar with h to traverse symlinks
------------------------------------------------------------------------
r4662 | counting_pine | 2008-03-22 08:22:56 -0700 (Sat, 22 Mar 2008) | 1 line

Fix bad sentence
------------------------------------------------------------------------
r4661 | whotookcha0s-_- | 2008-03-22 07:29:10 -0700 (Sat, 22 Mar 2008) | 1 line

place examples, docs in prefix/share
------------------------------------------------------------------------
r4660 | whotookcha0s-_- | 2008-03-22 06:36:08 -0700 (Sat, 22 Mar 2008) | 3 lines

updated manifest files
updated install.sh, -i now ignores any explicit arg, u doesn't

------------------------------------------------------------------------
r4659 | jeffmarshall | 2008-03-22 06:01:19 -0700 (Sat, 22 Mar 2008) | 4 lines

updated:
- copyright notices and version numbers
- INSTALL notes (BFD library license compatibility)

------------------------------------------------------------------------
r4658 | whotookcha0s-_- | 2008-03-22 00:58:51 -0700 (Sat, 22 Mar 2008) | 1 line

reverted last change (accidentally committed)
------------------------------------------------------------------------
r4657 | jeffmarshall | 2008-03-21 15:41:40 -0700 (Fri, 21 Mar 2008) | 1 line

changed: ignore const correctness when passing to varargs
------------------------------------------------------------------------
r4656 | jeffmarshall | 2008-03-21 15:37:33 -0700 (Fri, 21 Mar 2008) | 1 line

minor clean-up: removed fb_VAL64()
------------------------------------------------------------------------
r4655 | jeffmarshall | 2008-03-21 15:34:23 -0700 (Fri, 21 Mar 2008) | 1 line

minor clean-up: was using TARGET_ when probably should be using __FB_*__.
------------------------------------------------------------------------
r4654 | jeffmarshall | 2008-03-20 10:55:45 -0700 (Thu, 20 Mar 2008) | 1 line

internal: rewrite of solve_path() should be complete now
------------------------------------------------------------------------
r4653 | jeffmarshall | 2008-03-20 08:30:44 -0700 (Thu, 20 Mar 2008) | 1 line

WIP
------------------------------------------------------------------------
r4652 | jeffmarshall | 2008-03-20 08:23:42 -0700 (Thu, 20 Mar 2008) | 1 line

WIP
------------------------------------------------------------------------
r4651 | jeffmarshall | 2008-03-20 08:01:58 -0700 (Thu, 20 Mar 2008) | 1 line

fixed bug #19196630: fbc wasn't handling UNC paths on windows
------------------------------------------------------------------------
r4650 | counting_pine | 2008-03-20 07:23:44 -0700 (Thu, 20 Mar 2008) | 1 line

Replace strtoll() with (long long)strtoull() to prevent saturation; add some more test cases
------------------------------------------------------------------------
r4649 | counting_pine | 2008-03-19 17:33:31 -0700 (Wed, 19 Mar 2008) | 2 lines

- Stop string Inputs being delimited by spaces
- Convert 'd' to 'e' when Inputting floating-point numbers, for compatibility with non-win32 platforms (patch by jeffmarshall)
------------------------------------------------------------------------
r4648 | jeffmarshall | 2008-03-19 13:52:10 -0700 (Wed, 19 Mar 2008) | 1 line

updated: instructions for building libcunit.a on dos
------------------------------------------------------------------------
r4647 | jeffmarshall | 2008-03-19 09:56:48 -0700 (Wed, 19 Mar 2008) | 1 line

-Wall fixes
------------------------------------------------------------------------
r4646 | jeffmarshall | 2008-03-19 09:53:17 -0700 (Wed, 19 Mar 2008) | 1 line

missing alias for wcscspn
------------------------------------------------------------------------
r4645 | jeffmarshall | 2008-03-18 11:29:10 -0700 (Tue, 18 Mar 2008) | 1 line

updated source manifest (with new objinfo files)
------------------------------------------------------------------------
r4644 | jeffmarshall | 2008-03-18 11:20:40 -0700 (Tue, 18 Mar 2008) | 1 line

updated: copyright notice
------------------------------------------------------------------------
r4643 | jeffmarshall | 2008-03-18 10:32:29 -0700 (Tue, 18 Mar 2008) | 1 line

updated: copyright notice
------------------------------------------------------------------------
r4642 | jeffmarshall | 2008-03-18 09:49:43 -0700 (Tue, 18 Mar 2008) | 1 line

update: copyright notice
------------------------------------------------------------------------
r4641 | i_am_drv | 2008-03-17 21:21:33 -0700 (Mon, 17 Mar 2008) | 2 lines

add install rule for compiler

------------------------------------------------------------------------
r4640 | i_am_drv | 2008-03-17 21:17:45 -0700 (Mon, 17 Mar 2008) | 2 lines

make sure fbc is executable by all

------------------------------------------------------------------------
r4639 | whotookcha0s-_- | 2008-03-17 19:52:28 -0700 (Mon, 17 Mar 2008) | 1 line

updated install script to only copy headers if they're newer (-u)
------------------------------------------------------------------------
r4638 | jeffmarshall | 2008-03-16 17:56:04 -0700 (Sun, 16 Mar 2008) | 1 line

cache gcc -print-file-name queries
------------------------------------------------------------------------
r4637 | jeffmarshall | 2008-03-16 14:25:44 -0700 (Sun, 16 Mar 2008) | 1 line

-Wall fixes, clean-up.
------------------------------------------------------------------------
r4636 | jeffmarshall | 2008-03-16 08:18:13 -0700 (Sun, 16 Mar 2008) | 1 line

add guards and copyright
------------------------------------------------------------------------
r4635 | jeffmarshall | 2008-03-16 08:11:09 -0700 (Sun, 16 Mar 2008) | 1 line

should use compiler copyright - pasted wrong one last commit (jeffm)
------------------------------------------------------------------------
r4634 | jeffmarshall | 2008-03-16 07:44:54 -0700 (Sun, 16 Mar 2008) | 2 lines

docs for BFD makefile options

------------------------------------------------------------------------
r4633 | jeffmarshall | 2008-03-16 07:26:41 -0700 (Sun, 16 Mar 2008) | 1 line

only build c-objinfo.c if objinfo is enabled.  Pass __BFD_VER__ instead of BFD_VER define to compiler.  Renamed some bfd wrapper types.
------------------------------------------------------------------------
r4632 | whotookcha0s-_- | 2008-03-16 01:39:33 -0700 (Sun, 16 Mar 2008) | 1 line

forgot 'return's
------------------------------------------------------------------------
r4631 | whotookcha0s-_- | 2008-03-16 01:35:55 -0700 (Sun, 16 Mar 2008) | 2 lines

added C interface to BFD to keep FB from breaking if it updates in the future.
added the BFD_VER and FB_BFD variables to the makefile. use FB_BFD to use the freebasic interface to bfd. Use BFD_VER=217, to use the fb interface for bfd 2.17, etc
------------------------------------------------------------------------
r4630 | i_am_drv | 2008-03-15 13:30:36 -0700 (Sat, 15 Mar 2008) | 2 lines

detect configured prefix in install.sh

------------------------------------------------------------------------
r4629 | i_am_drv | 2008-03-15 12:33:52 -0700 (Sat, 15 Mar 2008) | 2 lines

compress man file when installing

------------------------------------------------------------------------
r4628 | i_am_drv | 2008-03-15 12:31:22 -0700 (Sat, 15 Mar 2008) | 2 lines

install man page in $INSTALLDIR too (don't touch anything outside $INSTALLDIR)

------------------------------------------------------------------------
r4627 | jeffmarshall | 2008-03-15 05:35:43 -0700 (Sat, 15 Mar 2008) | 1 line

remove trailing space on some lines
------------------------------------------------------------------------
r4626 | jeffmarshall | 2008-03-14 17:55:22 -0700 (Fri, 14 Mar 2008) | 1 line

updated with inc/bfd files, and remove files in win32.lst that are generated by w32_inst.
------------------------------------------------------------------------
r4625 | jeffmarshall | 2008-03-14 17:48:12 -0700 (Fri, 14 Mar 2008) | 4 lines

update win32 installer:
- allow verification of package contents with win32.lst manifest
- updated docs

------------------------------------------------------------------------
r4624 | jeffmarshall | 2008-03-14 15:29:40 -0700 (Fri, 14 Mar 2008) | 1 line

clean-up
------------------------------------------------------------------------
r4623 | i_am_drv | 2008-03-14 08:59:55 -0700 (Fri, 14 Mar 2008) | 1 line

split bfd.bi by version
------------------------------------------------------------------------
r4622 | jeffmarshall | 2008-03-14 08:34:18 -0700 (Fri, 14 Mar 2008) | 1 line

updated from wiki
------------------------------------------------------------------------
r4621 | jeffmarshall | 2008-03-14 07:32:11 -0700 (Fri, 14 Mar 2008) | 1 line

small fix - make clean or touch src/compiler/fb-objinfo.bas
------------------------------------------------------------------------
r4620 | jeffmarshall | 2008-03-14 06:01:05 -0700 (Fri, 14 Mar 2008) | 1 line

update current manifests
------------------------------------------------------------------------
r4619 | jeffmarshall | 2008-03-14 05:48:51 -0700 (Fri, 14 Mar 2008) | 1 line

wiki examples updated
------------------------------------------------------------------------
r4618 | jeffmarshall | 2008-03-14 04:59:51 -0700 (Fri, 14 Mar 2008) | 1 line

don't show configured prefix if the host compiler doesn't use it
------------------------------------------------------------------------
r4617 | jeffmarshall | 2008-03-14 04:57:12 -0700 (Fri, 14 Mar 2008) | 1 line

small fix
------------------------------------------------------------------------
r4616 | i_am_drv | 2008-03-13 20:17:04 -0700 (Thu, 13 Mar 2008) | 1 line

remove svn:executable
------------------------------------------------------------------------
r4615 | i_am_drv | 2008-03-13 19:43:28 -0700 (Thu, 13 Mar 2008) | 1 line

bump copyright year
------------------------------------------------------------------------
r4614 | i_am_drv | 2008-03-13 19:41:02 -0700 (Thu, 13 Mar 2008) | 1 line

add prefix to -version output
------------------------------------------------------------------------
r4613 | i_am_drv | 2008-03-13 19:30:38 -0700 (Thu, 13 Mar 2008) | 1 line

ignore autom4te.cache dirs
------------------------------------------------------------------------
r4612 | i_am_drv | 2008-03-13 19:26:54 -0700 (Thu, 13 Mar 2008) | 1 line

fix --enable-crosscomp-all (missing win32)
------------------------------------------------------------------------
r4611 | i_am_drv | 2008-03-13 19:24:16 -0700 (Thu, 13 Mar 2008) | 3 lines

add FB_HOST_PATHDIV constant


------------------------------------------------------------------------
r4610 | counting_pine | 2008-03-13 18:51:28 -0700 (Thu, 13 Mar 2008) | 2 lines

Fixed small copy-paste error: no const checking for G or B in Palette Get

------------------------------------------------------------------------
r4609 | i_am_drv | 2008-03-13 17:38:05 -0700 (Thu, 13 Mar 2008) | 1 line

updates for bfd 2.18
------------------------------------------------------------------------
r4608 | counting_pine | 2008-03-13 17:28:58 -0700 (Thu, 13 Mar 2008) | 2 lines

Fixed: Str() was trying to fold const-qualified vars at compile time

------------------------------------------------------------------------
r4607 | jeffmarshall | 2008-03-13 07:35:25 -0700 (Thu, 13 Mar 2008) | 3 lines

- don't invoke the LET operator if the assignment is an initializer (ctor) and the types match.
- dim/redim_param.bas couldn't be run if compiled with -exx -g due

------------------------------------------------------------------------
r4606 | whotookcha0s-_- | 2008-03-13 02:11:26 -0700 (Thu, 13 Mar 2008) | 2 lines

fix let initialization (clone before using astNewArg)
small fix on -exx pointer node creation (thanks coderJeff)
------------------------------------------------------------------------
r4605 | jeffmarshall | 2008-03-12 06:44:02 -0700 (Wed, 12 Mar 2008) | 1 line

added EXTRAERR option
------------------------------------------------------------------------
r4604 | jeffmarshall | 2008-03-12 06:32:08 -0700 (Wed, 12 Mar 2008) | 1 line

extern+const qualifier regression test (for #1909925)
------------------------------------------------------------------------
r4603 | counting_pine | 2008-03-12 05:01:22 -0700 (Wed, 12 Mar 2008) | 2 lines

Fix faulty optimization with signed integer division by powers of two greater than 2 ^ 1

------------------------------------------------------------------------
r4602 | i_am_drv | 2008-03-11 17:43:36 -0700 (Tue, 11 Mar 2008) | 1 line

add win32 process.bi CRT header
------------------------------------------------------------------------
r4601 | jeffmarshall | 2008-03-11 16:00:07 -0700 (Tue, 11 Mar 2008) | 1 line

fixed bug #1909925: const qualifier with EXTERN should not expect an initializer
------------------------------------------------------------------------
r4600 | jeffmarshall | 2008-03-11 15:52:47 -0700 (Tue, 11 Mar 2008) | 1 line

guards on crt timer functions
------------------------------------------------------------------------
r4599 | i_am_drv | 2008-03-11 13:38:15 -0700 (Tue, 11 Mar 2008) | 1 line

make sure bin path has a trailing slash
------------------------------------------------------------------------
r4598 | i_am_drv | 2008-03-11 11:09:15 -0700 (Tue, 11 Mar 2008) | 1 line

make sure bin directory exists
------------------------------------------------------------------------
r4597 | whotookcha0s-_- | 2008-03-11 03:31:54 -0700 (Tue, 11 Mar 2008) | 1 line

changed: Linux/BSD file structure is now a lot more unix-friendly
------------------------------------------------------------------------
r4596 | jeffmarshall | 2008-03-05 20:14:50 -0800 (Wed, 05 Mar 2008) | 1 line

fblite: arrays need to be cleared before intializers used as assignment.  minor code clean-up.
------------------------------------------------------------------------
r4595 | jeffmarshall | 2008-03-05 11:52:48 -0800 (Wed, 05 Mar 2008) | 1 line

fblite: minor fix for static, common, shared variables.
------------------------------------------------------------------------
r4594 | counting_pine | 2008-03-04 21:35:12 -0800 (Tue, 04 Mar 2008) | 3 lines

- Remove unneeded SCOPE keyword from lang qb
- Allow statements to appear directly after ELSE/ELSEIF..THEN in multi-line IFs.

------------------------------------------------------------------------
r4593 | jeffmarshall | 2008-03-04 16:39:44 -0800 (Tue, 04 Mar 2008) | 1 line

internal: isolate dialects in hVarDeclEx()
------------------------------------------------------------------------
r4592 | jeffmarshall | 2008-03-04 15:07:26 -0800 (Tue, 04 Mar 2008) | 2 lines

internal: reverted

------------------------------------------------------------------------
r4591 | jeffmarshall | 2008-03-04 14:26:44 -0800 (Tue, 04 Mar 2008) | 1 line

internal: isolate dialects in hVarDeclEx()
------------------------------------------------------------------------
r4590 | counting_pine | 2008-03-04 09:54:00 -0800 (Tue, 04 Mar 2008) | 2 lines

Add the "fblite" dialect (WIP)

------------------------------------------------------------------------
r4589 | counting_pine | 2008-03-03 22:31:27 -0800 (Mon, 03 Mar 2008) | 5 lines

- Add changelog entry for r4565
- Much more robust number INPUTing, plus extensive input test (for whole numbers)
- Make VALLNG (and other longint parsing functions) break on invalid hex/oct/bin digits
- Make VAL use VALLNG instead of VALINT for hex/bin/oct numbers

------------------------------------------------------------------------
r4588 | jeffmarshall | 2008-03-03 19:15:54 -0800 (Mon, 03 Mar 2008) | 1 line

fixed: bug #1905669
------------------------------------------------------------------------
r4587 | whotookcha0s-_- | 2008-03-03 16:20:29 -0800 (Mon, 03 Mar 2008) | 1 line

make sure to unlock if there's an error requiring an exit
------------------------------------------------------------------------
r4586 | whotookcha0s-_- | 2008-03-03 11:58:18 -0800 (Mon, 03 Mar 2008) | 1 line

fixed: bug #1906004 - fb_PageSet() was testing page requests against console limits, even in gfx mode
------------------------------------------------------------------------
r4585 | jeffmarshall | 2008-03-03 08:11:46 -0800 (Mon, 03 Mar 2008) | 1 line

lang qb: OPEN not handling COM/LPT device names correctly
------------------------------------------------------------------------
r4584 | jeffmarshall | 2008-03-03 06:14:22 -0800 (Mon, 03 Mar 2008) | 1 line

wip: bug #1905669
------------------------------------------------------------------------
r4583 | v1ctor | 2008-03-02 15:33:05 -0800 (Sun, 02 Mar 2008) | 1 line

fixed: bug #1905725
------------------------------------------------------------------------
r4582 | v1ctor | 2008-03-02 15:30:19 -0800 (Sun, 02 Mar 2008) | 1 line

fixed: bug #1905725
------------------------------------------------------------------------
r4581 | jeffmarshall | 2008-03-02 08:35:48 -0800 (Sun, 02 Mar 2008) | 1 line

wiki example moved to new page
------------------------------------------------------------------------
r4580 | jeffmarshall | 2008-03-02 08:35:03 -0800 (Sun, 02 Mar 2008) | 1 line

minor clean-up
------------------------------------------------------------------------
r4579 | jeffmarshall | 2008-03-02 06:42:10 -0800 (Sun, 02 Mar 2008) | 1 line

more tests for array initializers in TYPE's
------------------------------------------------------------------------
r4578 | jeffmarshall | 2008-03-01 18:10:08 -0800 (Sat, 01 Mar 2008) | 1 line

fixed #1890693: array and UDT initializers in TYPE's
------------------------------------------------------------------------
r4577 | jeffmarshall | 2008-03-01 14:39:04 -0800 (Sat, 01 Mar 2008) | 1 line

internal: astDumpTree() shows basic operator names
------------------------------------------------------------------------
r4576 | jeffmarshall | 2008-03-01 10:50:22 -0800 (Sat, 01 Mar 2008) | 1 line

internal: changed astDumpTree() and removed 'locate' statements
------------------------------------------------------------------------
r4575 | jeffmarshall | 2008-03-01 09:35:29 -0800 (Sat, 01 Mar 2008) | 1 line

remove deleteme.txt from manifests and add wiki examples
------------------------------------------------------------------------
r4574 | whotookcha0s-_- | 2008-03-01 09:29:28 -0800 (Sat, 01 Mar 2008) | 1 line

fixed: #1905284 - bitfield accessing wasn't always working when the optimizer would remove the field node
------------------------------------------------------------------------
r4573 | whotookcha0s-_- | 2008-03-01 06:47:00 -0800 (Sat, 01 Mar 2008) | 1 line

internal: added astUpdateBitfieldAssigment
------------------------------------------------------------------------
r4572 | whotookcha0s-_- | 2008-03-01 05:57:52 -0800 (Sat, 01 Mar 2008) | 3 lines

changed: bug #1881320 - global operators defined within type body have to be disallowed until the 

feature is properly implemented
------------------------------------------------------------------------
r4571 | whotookcha0s-_- | 2008-03-01 05:30:50 -0800 (Sat, 01 Mar 2008) | 3 lines

fixeD: bug #1865673 - symbols defined inside namespaces that had already been imported were throwing a 

duplicate definition
------------------------------------------------------------------------
r4570 | i_am_drv | 2008-02-29 18:43:27 -0800 (Fri, 29 Feb 2008) | 1 line

remove other target headers from manifests
------------------------------------------------------------------------
r4569 | jeffmarshall | 2008-02-29 18:42:17 -0800 (Fri, 29 Feb 2008) | 1 line

wiki examples added
------------------------------------------------------------------------
r4568 | i_am_drv | 2008-02-29 15:19:47 -0800 (Fri, 29 Feb 2008) | 4 lines

- get rid of deleteme.txt files (not needed with svn)
- add svn:ignore properties for files that should never be versioned


------------------------------------------------------------------------
r4567 | i_am_drv | 2008-02-29 14:54:33 -0800 (Fri, 29 Feb 2008) | 4 lines

- split crt/sys/time.bi by target
- move Linux gettimeofday from crt/time.bi to crt/sys/time.bi


------------------------------------------------------------------------
r4566 | i_am_drv | 2008-02-29 13:25:38 -0800 (Fri, 29 Feb 2008) | 1 line

fix timerisset
------------------------------------------------------------------------
r4565 | counting_pine | 2008-02-29 13:17:51 -0800 (Fri, 29 Feb 2008) | 1 line

fixed: ICE on inputting non-identifiers, e.g. "INPUT 1"
------------------------------------------------------------------------
r4564 | jeffmarshall | 2008-02-29 12:54:29 -0800 (Fri, 29 Feb 2008) | 1 line

updated manifests
------------------------------------------------------------------------
r4563 | jeffmarshall | 2008-02-29 10:17:19 -0800 (Fri, 29 Feb 2008) | 1 line

fixed #1825178: win32 gdi/directx drivers again use normal thread priority as default (like in fbc-0.17 releases).  Passing fb.GFX_HIGH_PRIORITY to the flags in SCREEN/SCREENRES will give the gfx updater thread above normal priority. (like in 0.18.x releases)
------------------------------------------------------------------------
r4562 | jeffmarshall | 2008-02-29 05:37:19 -0800 (Fri, 29 Feb 2008) | 1 line

fixed #1900586: GET #/PUT # not returning errors for mismatched record lengths with RANDOM mode files or negative record numbers.
------------------------------------------------------------------------
r4561 | whotookcha0s-_- | 2008-02-28 06:13:29 -0800 (Thu, 28 Feb 2008) | 1 line

fixed: bug #1900852 - bitfields used in variable initializers weren't handled properly
------------------------------------------------------------------------
r4560 | whotookcha0s-_- | 2008-02-27 05:18:57 -0800 (Wed, 27 Feb 2008) | 3 lines

fixed: bug #1901924 - INPUTting large numbers to a floating-point were incorrectly allowing integer 

overflow before being casted to the floating-point type (God help the QB quirks this breaks!)
------------------------------------------------------------------------
r4559 | counting_pine | 2008-02-25 17:49:12 -0800 (Mon, 25 Feb 2008) | 2 lines

internal: make MKI/CVI dependent on internal type remappings

------------------------------------------------------------------------
r4558 | whotookcha0s-_- | 2008-02-25 03:52:17 -0800 (Mon, 25 Feb 2008) | 1 line

added manual example (hope i did it right!)
------------------------------------------------------------------------
r4557 | whotookcha0s-_- | 2008-02-25 03:44:05 -0800 (Mon, 25 Feb 2008) | 1 line

explicit c/dtor calls needed to generate a default c/dtor if none existed, to call.
------------------------------------------------------------------------
r4556 | whotookcha0s-_- | 2008-02-25 02:24:50 -0800 (Mon, 25 Feb 2008) | 6 lines

added: placement new: see wiki for details on syntax and usage 
added: explicit ctor/dtor calling (WIP)
fixed: #1901102 - operator NEW wasn't working correctly when used directly in the body of a type (not 

in an explicit constructor) ex: type foo: as integer ptr a = new integer: end type

------------------------------------------------------------------------
r4555 | whotookcha0s-_- | 2008-02-24 05:34:34 -0800 (Sun, 24 Feb 2008) | 6 lines

added: SWAP now supports one or more bitfield variables 
fixed: assignment of bitfields wasn't ensuring that the assignment wouldn't trash the parent 

variable, if a number too big was assigned

regression test added
------------------------------------------------------------------------
r4554 | jeffmarshall | 2008-02-24 04:42:22 -0800 (Sun, 24 Feb 2008) | 1 line

added new images from wiki
------------------------------------------------------------------------
r4553 | jeffmarshall | 2008-02-22 20:25:37 -0800 (Fri, 22 Feb 2008) | 1 line

fixed #1899836
------------------------------------------------------------------------
r4552 | whotookcha0s-_- | 2008-02-22 13:57:11 -0800 (Fri, 22 Feb 2008) | 1 line

fixed: ctors etc should have allowed a byval arg of the parent type, if it's a pointer
------------------------------------------------------------------------
r4551 | i_am_drv | 2008-02-21 12:12:41 -0800 (Thu, 21 Feb 2008) | 1 line

add crt/dos/unistd.bi
------------------------------------------------------------------------
r4550 | i_am_drv | 2008-02-20 07:35:45 -0800 (Wed, 20 Feb 2008) | 1 line

remove extra #endif
------------------------------------------------------------------------
r4549 | yetifoot | 2008-02-20 04:23:04 -0800 (Wed, 20 Feb 2008) | 1 line

add gettimeofday for linux
------------------------------------------------------------------------
r4548 | yetifoot | 2008-02-20 04:16:46 -0800 (Wed, 20 Feb 2008) | 2 lines

example of CGI for web servers

------------------------------------------------------------------------
r4547 | whotookcha0s-_- | 2008-02-19 16:11:46 -0800 (Tue, 19 Feb 2008) | 3 lines

added crt/linux/stdlib.bi
mkstemp should have been in crt/linux/stdlib.bi

------------------------------------------------------------------------
r4546 | whotookcha0s-_- | 2008-02-19 16:05:46 -0800 (Tue, 19 Feb 2008) | 1 line

added: CRT function: declare function mkstemp( byval template_ as zstring ptr ) as integer
------------------------------------------------------------------------
r4545 | whotookcha0s-_- | 2008-02-19 04:36:38 -0800 (Tue, 19 Feb 2008) | 1 line

fixed: bug #1892194 - gfxlib2 had screen 12 with a 256 color palette, instead of 16 
------------------------------------------------------------------------
r4544 | whotookcha0s-_- | 2008-02-19 04:28:06 -0800 (Tue, 19 Feb 2008) | 1 line

regression test for the fwd ref fix
------------------------------------------------------------------------
r4543 | whotookcha0s-_- | 2008-02-19 04:27:22 -0800 (Tue, 19 Feb 2008) | 1 line

fixed: bug #1891408 - forward references weren't taking the symbol-to-be-patched's const info into account when backpatching
------------------------------------------------------------------------
r4542 | whotookcha0s-_- | 2008-02-19 03:29:13 -0800 (Tue, 19 Feb 2008) | 1 line

added: regression test for previous bugfix
------------------------------------------------------------------------
r4541 | whotookcha0s-_- | 2008-02-19 03:28:22 -0800 (Tue, 19 Feb 2008) | 1 line

fixed: BUG #1890675 - dereferencing bogus casted expressions would cause a crash
------------------------------------------------------------------------
r4540 | whotookcha0s-_- | 2008-02-19 02:33:07 -0800 (Tue, 19 Feb 2008) | 1 line

internal: removed the QB dialect check for FOR scoping
------------------------------------------------------------------------
r4539 | whotookcha0s-_- | 2008-02-19 02:32:25 -0800 (Tue, 19 Feb 2008) | 1 line

fixed: QB forward calling wasn't handling arrays properly
------------------------------------------------------------------------
r4538 | i_am_drv | 2008-02-16 00:10:19 -0800 (Sat, 16 Feb 2008) | 1 line

update manifests for unistd.bi
------------------------------------------------------------------------
r4537 | i_am_drv | 2008-02-16 00:05:54 -0800 (Sat, 16 Feb 2008) | 1 line

add dos crt/unistd.bi
------------------------------------------------------------------------
r4536 | counting_pine | 2008-02-14 20:10:26 -0800 (Thu, 14 Feb 2008) | 1 line

lang qb: Allow literals outside the 16-bit range to be stored as 32-bit integers
------------------------------------------------------------------------
r4535 | jeffmarshall | 2008-02-14 11:46:48 -0800 (Thu, 14 Feb 2008) | 1 line

updated current manifest with new wiki examples
------------------------------------------------------------------------
r4534 | jeffmarshall | 2008-02-14 11:40:22 -0800 (Thu, 14 Feb 2008) | 1 line

updated/added examples from wiki
------------------------------------------------------------------------
r4533 | jeffmarshall | 2008-02-14 10:30:12 -0800 (Thu, 14 Feb 2008) | 1 line

fixed #1891840 - internal name mangling for function ptrs causing collision
------------------------------------------------------------------------
r4532 | jeffmarshall | 2008-02-08 19:25:05 -0800 (Fri, 08 Feb 2008) | 1 line

updated copyright notice
------------------------------------------------------------------------
r4531 | jeffmarshall | 2008-02-08 18:55:13 -0800 (Fri, 08 Feb 2008) | 1 line

added DLL imports and copyright
------------------------------------------------------------------------
r4530 | i_am_drv | 2008-02-07 13:25:15 -0800 (Thu, 07 Feb 2008) | 1 line

update pcre headers and import libs to version 7.6 (patch by stylin)
------------------------------------------------------------------------
r4529 | jeffmarshall | 2008-02-05 16:25:22 -0800 (Tue, 05 Feb 2008) | 1 line

small fix
------------------------------------------------------------------------
r4528 | jeffmarshall | 2008-02-03 18:23:24 -0800 (Sun, 03 Feb 2008) | 1 line

#1885211 - non-const to const param checking in overloaded proc resolution
------------------------------------------------------------------------
r4527 | jeffmarshall | 2008-02-03 07:00:11 -0800 (Sun, 03 Feb 2008) | 1 line

updated with new examples from manual
------------------------------------------------------------------------
r4526 | jeffmarshall | 2008-02-03 06:13:11 -0800 (Sun, 03 Feb 2008) | 1 line

wiki examples updated / added
------------------------------------------------------------------------
r4525 | jeffmarshall | 2008-02-01 23:11:47 -0800 (Fri, 01 Feb 2008) | 5 lines

fixed:
- #1877439 - const qualifier overloading with byref (jeffm)
- #1872532 - fix null pointer access when parsing parameters and allow OVERLOAD in all member procs declarations for consistency (jeffm)
- #1876286 - constructor should not take byval arg of parent type as only parameter ( jeffm )

------------------------------------------------------------------------
r4524 | jeffmarshall | 2008-02-01 23:03:50 -0800 (Fri, 01 Feb 2008) | 1 line

minor spelling fix
------------------------------------------------------------------------
r4523 | jeffmarshall | 2008-02-01 06:01:40 -0800 (Fri, 01 Feb 2008) | 1 line

lang qb: allow datetime.bi and file.bi
------------------------------------------------------------------------
r4522 | jeffmarshall | 2008-01-24 09:26:42 -0800 (Thu, 24 Jan 2008) | 1 line

fixed: bug #1877440
------------------------------------------------------------------------
r4521 | counting_pine | 2008-01-22 17:38:26 -0800 (Tue, 22 Jan 2008) | 1 line

lang qb: parse "IF...THEN : ...", "IF...THEN REM ..." as single-line IF
------------------------------------------------------------------------
r4520 | v1ctor | 2008-01-19 03:56:47 -0800 (Sat, 19 Jan 2008) | 1 line

fixed: bug #1872313
------------------------------------------------------------------------
r4519 | jeffmarshall | 2008-01-17 13:05:42 -0800 (Thu, 17 Jan 2008) | 2 lines

updated (log entry for binutils changes)

------------------------------------------------------------------------
r4518 | jeffmarshall | 2008-01-17 10:38:53 -0800 (Thu, 17 Jan 2008) | 1 line

updated and dropped binutils from Linux package
------------------------------------------------------------------------
r4517 | jeffmarshall | 2008-01-17 10:01:37 -0800 (Thu, 17 Jan 2008) | 1 line

some unicode tests missing the wstr() conversions
------------------------------------------------------------------------
r4516 | whotookcha0s-_- | 2008-01-16 01:34:08 -0800 (Wed, 16 Jan 2008) | 2 lines

fixed: variables with const qualifier shouldn't have been accepting ANY as an initializer
couple tests for this and last fix
------------------------------------------------------------------------
r4515 | whotookcha0s-_- | 2008-01-16 00:31:55 -0800 (Wed, 16 Jan 2008) | 1 line

fixed: VAR was disregarding the extended type information (like const qualifiers), found by stylin
------------------------------------------------------------------------
r4514 | jeffmarshall | 2008-01-15 21:07:57 -0800 (Tue, 15 Jan 2008) | 1 line

alternate find-bin logic should be for linux host (not target)
------------------------------------------------------------------------
r4513 | jeffmarshall | 2008-01-15 20:45:34 -0800 (Tue, 15 Jan 2008) | 1 line

all calls to binary utilities use fbFindBinFile() to resolve path
------------------------------------------------------------------------
r4512 | i_am_drv | 2008-01-15 14:07:30 -0800 (Tue, 15 Jan 2008) | 1 line

-Wall fixes
------------------------------------------------------------------------
r4511 | i_am_drv | 2008-01-15 14:04:59 -0800 (Tue, 15 Jan 2008) | 1 line

-Wall fixes
------------------------------------------------------------------------
r4510 | i_am_drv | 2008-01-15 14:03:20 -0800 (Tue, 15 Jan 2008) | 1 line

-Wall fixes
------------------------------------------------------------------------
r4509 | jeffmarshall | 2008-01-14 15:59:13 -0800 (Mon, 14 Jan 2008) | 1 line

INSTRREV will use (-1) as default start value to allow 'i=instrrev(s1,s2,i-1)' logic.  Added regression tests for unicode versions of instr/instrrev.
------------------------------------------------------------------------
r4508 | jeffmarshall | 2008-01-14 15:23:33 -0800 (Mon, 14 Jan 2008) | 2 lines

added fbFindBinFile() helper for locating binaries

------------------------------------------------------------------------
r4507 | counting_pine | 2008-01-14 10:36:16 -0800 (Mon, 14 Jan 2008) | 1 line

lang qb: make CINT convert to 16-bit integer; append a space when PRINTing numbers
------------------------------------------------------------------------
r4506 | jeffmarshall | 2008-01-13 19:41:17 -0800 (Sun, 13 Jan 2008) | 1 line

update
------------------------------------------------------------------------
r4505 | jeffmarshall | 2008-01-13 19:23:23 -0800 (Sun, 13 Jan 2008) | 1 line

updated
------------------------------------------------------------------------
r4504 | jeffmarshall | 2008-01-13 12:48:48 -0800 (Sun, 13 Jan 2008) | 1 line

added '--enable-crosscomp-all' convenience option to rtlib configure script
------------------------------------------------------------------------
r4503 | jeffmarshall | 2008-01-13 12:46:59 -0800 (Sun, 13 Jan 2008) | 1 line

fixed: lang qb: SLEEP with no arguments not handled correctly in gfx modes
------------------------------------------------------------------------
r4502 | jeffmarshall | 2008-01-13 12:45:46 -0800 (Sun, 13 Jan 2008) | 1 line

fixed: lang qb: LINE -(x,y) was not being allowed due to parser changes
------------------------------------------------------------------------
r4501 | i_am_drv | 2008-01-12 22:36:16 -0800 (Sat, 12 Jan 2008) | 1 line

fix dos exepath in child processes
------------------------------------------------------------------------
r4500 | v1ctor | 2008-01-12 11:06:04 -0800 (Sat, 12 Jan 2008) | 1 line

added: 'ENUM foo EXPLICIT' to allow the enum elements not to be imported to the current namespace (syntax suggested by Pritchard)
------------------------------------------------------------------------
r4499 | v1ctor | 2008-01-12 11:05:29 -0800 (Sat, 12 Jan 2008) | 1 line

added: 'ENUM foo EXPLICIT' to allow the enum elements not to be imported to the current namespace (syntax suggested by Pritchard)
------------------------------------------------------------------------
r4498 | jeffmarshall | 2008-01-11 19:22:04 -0800 (Fri, 11 Jan 2008) | 1 line

added INSTRREV - feature request #1244607
------------------------------------------------------------------------
r4497 | i_am_drv | 2008-01-10 13:22:36 -0800 (Thu, 10 Jan 2008) | 1 line

declare fb_keysym_to_scancode
------------------------------------------------------------------------
r4496 | i_am_drv | 2008-01-07 11:56:27 -0800 (Mon, 07 Jan 2008) | 1 line

stub gfxlib joystick driver for freebsd
------------------------------------------------------------------------
r4495 | i_am_drv | 2008-01-07 11:35:37 -0800 (Mon, 07 Jan 2008) | 1 line

stub console mouse and multikey for freebsd
------------------------------------------------------------------------
r4494 | i_am_drv | 2008-01-07 11:04:47 -0800 (Mon, 07 Jan 2008) | 1 line

fix freebsd getexename
------------------------------------------------------------------------
r4493 | i_am_drv | 2008-01-07 11:01:07 -0800 (Mon, 07 Jan 2008) | 1 line

freebsd support for gfxlib
------------------------------------------------------------------------
r4492 | i_am_drv | 2008-01-07 11:00:03 -0800 (Mon, 07 Jan 2008) | 1 line

move fb_keysym_to_scancode table to separate file
------------------------------------------------------------------------
r4491 | i_am_drv | 2008-01-07 08:14:02 -0800 (Mon, 07 Jan 2008) | 1 line

add freebsd obj dir
------------------------------------------------------------------------
r4490 | i_am_drv | 2008-01-06 20:50:23 -0800 (Sun, 06 Jan 2008) | 1 line

add freebsd obj dirs
------------------------------------------------------------------------
r4489 | counting_pine | 2008-01-06 19:32:40 -0800 (Sun, 06 Jan 2008) | 1 line

Add old-style header support to load_bmp (fix for bug #1865526); remove unneeded variable
------------------------------------------------------------------------
r4488 | v1ctor | 2008-01-06 14:38:11 -0800 (Sun, 06 Jan 2008) | 1 line

added: missing functions
------------------------------------------------------------------------
r4487 | v1ctor | 2008-01-06 14:37:33 -0800 (Sun, 06 Jan 2008) | 1 line

fixed: name conflict with the 'delete' keyword
------------------------------------------------------------------------
r4486 | v1ctor | 2008-01-06 14:36:00 -0800 (Sun, 06 Jan 2008) | 1 line

-lang qb: DATA should be allowed inside compound-statements
------------------------------------------------------------------------
r4485 | v1ctor | 2008-01-06 14:14:12 -0800 (Sun, 06 Jan 2008) | 1 line

small cleanup
------------------------------------------------------------------------
r4484 | whotookcha0s-_- | 2008-01-06 09:47:03 -0800 (Sun, 06 Jan 2008) | 1 line

small correction...
------------------------------------------------------------------------
r4483 | i_am_drv | 2008-01-05 22:01:41 -0800 (Sat, 05 Jan 2008) | 1 line

split X11 driver from linux
------------------------------------------------------------------------
r4482 | i_am_drv | 2008-01-05 20:32:08 -0800 (Sat, 05 Jan 2008) | 1 line

added: FRE function for freebsd
------------------------------------------------------------------------
r4481 | counting_pine | 2008-01-05 18:49:56 -0800 (Sat, 05 Jan 2008) | 1 line

(Inaugural patch) Change lang qb's MK/CVI to use MK/CVSHORT instead
------------------------------------------------------------------------
r4480 | jeffmarshall | 2008-01-05 09:21:55 -0800 (Sat, 05 Jan 2008) | 1 line

updated
------------------------------------------------------------------------
r4479 | jeffmarshall | 2008-01-05 08:49:07 -0800 (Sat, 05 Jan 2008) | 1 line

PHTREAD_MUTEX_RECURSIVE causing problems on linux
------------------------------------------------------------------------
r4478 | jeffmarshall | 2008-01-05 06:27:21 -0800 (Sat, 05 Jan 2008) | 2 lines

- updated version number to 0.18.4b
- added install options for lzo, ogg and vorbis includes
------------------------------------------------------------------------
r4477 | jeffmarshall | 2008-01-05 06:22:47 -0800 (Sat, 05 Jan 2008) | 1 line

changed generator for HTML image tags to use wiki image links
------------------------------------------------------------------------
r4476 | jeffmarshall | 2008-01-05 06:21:28 -0800 (Sat, 05 Jan 2008) | 1 line

backup of all wiki images
------------------------------------------------------------------------
r4475 | jeffmarshall | 2008-01-04 14:17:39 -0800 (Fri, 04 Jan 2008) | 1 line

fbdoc images renamed
------------------------------------------------------------------------
r4474 | jeffmarshall | 2008-01-04 14:15:11 -0800 (Fri, 04 Jan 2008) | 1 line

fbdoc images renamed
------------------------------------------------------------------------
r4473 | jeffmarshall | 2008-01-04 11:17:20 -0800 (Fri, 04 Jan 2008) | 2 lines

DOS serial driver: added support for IR option - ( allows irq3 to irq7, on COM1 through COM4 )

------------------------------------------------------------------------
r4472 | jeffmarshall | 2008-01-04 11:15:27 -0800 (Fri, 04 Jan 2008) | 1 line

drop SDL, fmod, threading from manifest
------------------------------------------------------------------------
r4471 | jeffmarshall | 2008-01-04 11:14:51 -0800 (Fri, 04 Jan 2008) | 1 line

file renamed with 8.3 naming
------------------------------------------------------------------------
r4470 | jeffmarshall | 2008-01-04 11:13:15 -0800 (Fri, 04 Jan 2008) | 2 lines

(missing log entry)
fixed: OPEN COM was not allowing default parameters
------------------------------------------------------------------------
r4469 | v1ctor | 2008-01-01 12:49:28 -0800 (Tue, 01 Jan 2008) | 1 line

changed: keywords, data types, functions or macros not present in QB can now used in -lang qb by prefixing them with "__" (ie: "dim foo as __byte __ptr")
------------------------------------------------------------------------
r4468 | i_am_drv | 2007-12-29 20:41:37 -0800 (Sat, 29 Dec 2007) | 1 line

only use extended ANSI escapes on linux for now
------------------------------------------------------------------------
r4467 | i_am_drv | 2007-12-29 18:33:12 -0800 (Sat, 29 Dec 2007) | 1 line

fix freebsd exepath
------------------------------------------------------------------------
r4466 | i_am_drv | 2007-12-29 18:18:25 -0800 (Sat, 29 Dec 2007) | 1 line

fix slashes
------------------------------------------------------------------------
r4465 | i_am_drv | 2007-12-29 17:21:11 -0800 (Sat, 29 Dec 2007) | 1 line

make sure __USE_UNIX98 is declared when including <pthread.h> so PTHREAD_MUTEX_RECURSIVE is defined
------------------------------------------------------------------------
r4464 | jeffmarshall | 2007-12-29 16:08:10 -0800 (Sat, 29 Dec 2007) | 1 line

fixed: default COM parameters not being allowed
------------------------------------------------------------------------
r4463 | jeffmarshall | 2007-12-28 16:12:27 -0800 (Fri, 28 Dec 2007) | 1 line

update to version 0.18.4
------------------------------------------------------------------------
r4462 | i_am_drv | 2007-12-28 12:26:34 -0800 (Fri, 28 Dec 2007) | 1 line

check for PTHREAD_MUTEX_RECURSIVE in configure instead of with #ifdef (enum is used on freebsd)
------------------------------------------------------------------------
r4457 | jeffmarshall | 2007-12-28 05:52:00 -0800 (Fri, 28 Dec 2007) | 1 line

updated to version 0.18.3
------------------------------------------------------------------------
r4452 | jeffmarshall | 2007-12-28 05:42:20 -0800 (Fri, 28 Dec 2007) | 1 line

update date to Dec 28, 2007
------------------------------------------------------------------------
r4449 | jeffmarshall | 2007-12-28 05:34:57 -0800 (Fri, 28 Dec 2007) | 1 line

update to version number only to 0.18.3
------------------------------------------------------------------------
r4446 | jeffmarshall | 2007-12-27 17:55:12 -0800 (Thu, 27 Dec 2007) | 1 line

update to version 0.18.3
------------------------------------------------------------------------
r4438 | jeffmarshall | 2007-12-27 16:44:22 -0800 (Thu, 27 Dec 2007) | 1 line

added fbc-0.18.3 manifests
------------------------------------------------------------------------
r4437 | jeffmarshall | 2007-12-27 16:43:22 -0800 (Thu, 27 Dec 2007) | 1 line

updated (built using autoconf 2.59)
------------------------------------------------------------------------
r4436 | jeffmarshall | 2007-12-27 16:42:47 -0800 (Thu, 27 Dec 2007) | 1 line

version number change only to 0.18.3
------------------------------------------------------------------------
r4435 | jeffmarshall | 2007-12-27 07:24:24 -0800 (Thu, 27 Dec 2007) | 1 line

remove meaningless NEXT variables
------------------------------------------------------------------------
r4434 | jeffmarshall | 2007-12-27 06:56:27 -0800 (Thu, 27 Dec 2007) | 1 line

updated (for 0.18.3 release)
------------------------------------------------------------------------
r4433 | jeffmarshall | 2007-12-27 06:51:31 -0800 (Thu, 27 Dec 2007) | 1 line

update to version 0.18.3
------------------------------------------------------------------------
r4432 | jeffmarshall | 2007-12-27 05:07:24 -0800 (Thu, 27 Dec 2007) | 1 line

added: first version of DOS serial driver
------------------------------------------------------------------------
r4431 | jeffmarshall | 2007-12-27 05:06:54 -0800 (Thu, 27 Dec 2007) | 1 line

Add const qualifier type safety item
------------------------------------------------------------------------
r4430 | jeffmarshall | 2007-12-27 05:06:01 -0800 (Thu, 27 Dec 2007) | 1 line

small fix: albyte should be zstring for filename parameter
------------------------------------------------------------------------
r4429 | jeffmarshall | 2007-12-27 05:05:16 -0800 (Thu, 27 Dec 2007) | 1 line

fixed THREADCREATE parameters
------------------------------------------------------------------------
r4428 | jeffmarshall | 2007-12-27 05:04:46 -0800 (Thu, 27 Dec 2007) | 1 line

fixed FOR..NEXT variable mismatches
------------------------------------------------------------------------
r4427 | jeffmarshall | 2007-12-27 05:03:54 -0800 (Thu, 27 Dec 2007) | 1 line

small fix: missing null pointer check if there was a previous error closing scopes.
------------------------------------------------------------------------
r4426 | jeffmarshall | 2007-12-27 05:02:48 -0800 (Thu, 27 Dec 2007) | 1 line

update ODE (0.9) header and import library (Win32), removed examples/libraries/ode
------------------------------------------------------------------------
r4425 | whotookcha0s-_- | 2007-12-24 00:27:23 -0800 (Mon, 24 Dec 2007) | 2 lines

updated examples using cond*, also fixed some warnings from threadcreate

------------------------------------------------------------------------
r4424 | jeffmarshall | 2007-12-23 19:08:26 -0800 (Sun, 23 Dec 2007) | 2 lines

- #1856569 - BSAVE to BMP was calculating scan line padding incorectly for 8, 15, and 16 bit depths
- #1825178 - fbgfx/win32 changes thread priority to normal if the gfx window is minimized or inactive, partial fix
------------------------------------------------------------------------
r4423 | i_am_drv | 2007-12-23 15:28:32 -0800 (Sun, 23 Dec 2007) | 1 line

fix strict aliasing warnings
------------------------------------------------------------------------
r4422 | i_am_drv | 2007-12-23 15:03:02 -0800 (Sun, 23 Dec 2007) | 1 line

== should have been =
------------------------------------------------------------------------
r4421 | i_am_drv | 2007-12-23 15:01:34 -0800 (Sun, 23 Dec 2007) | 1 line

make sure alloca is defined
------------------------------------------------------------------------
r4420 | whotookcha0s-_- | 2007-12-22 18:15:27 -0800 (Sat, 22 Dec 2007) | 1 line

fixed: self ops defined as global needed to throw an error [bug #1830582]
------------------------------------------------------------------------
r4419 | jeffmarshall | 2007-12-21 12:01:21 -0800 (Fri, 21 Dec 2007) | 1 line

wiki examples updated
------------------------------------------------------------------------
r4418 | jeffmarshall | 2007-12-21 11:59:43 -0800 (Fri, 21 Dec 2007) | 2 lines

- thread-safe fb_CondInit()
- check for working SignalObjectAndWait() API function
------------------------------------------------------------------------
r4417 | jeffmarshall | 2007-12-21 11:58:22 -0800 (Fri, 21 Dec 2007) | 1 line

mutex parameter needed on CONDWAIT stub
------------------------------------------------------------------------
r4416 | jeffmarshall | 2007-12-21 11:57:17 -0800 (Fri, 21 Dec 2007) | 1 line

code clean-up (tab indents) and changed test: INSTR(s,ANY "") returns 0.
------------------------------------------------------------------------
r4415 | jeffmarshall | 2007-12-20 11:57:29 -0800 (Thu, 20 Dec 2007) | 1 line

missing cdecl in prototype.  Thanks D.J.Peters
------------------------------------------------------------------------
r4414 | jeffmarshall | 2007-12-20 11:51:21 -0800 (Thu, 20 Dec 2007) | 3 lines

- added missing INSTR(wstring,ANY wstring) function to rtlib
- added rtlib implementation of INSTRREV (W.I.P.) feature request #1244607
- thanks to counting_pine for the adaptation of the Boyer-Moore algorithm to a reverse string search function.
------------------------------------------------------------------------
r4413 | i_am_drv | 2007-12-19 18:23:55 -0800 (Wed, 19 Dec 2007) | 1 line

add a few more nullops
------------------------------------------------------------------------
r4412 | i_am_drv | 2007-12-17 20:16:31 -0800 (Mon, 17 Dec 2007) | 1 line

fixed CONDWAIT semantics (now takes mutex parameter)
------------------------------------------------------------------------
r4411 | jeffmarshall | 2007-12-16 16:33:26 -0800 (Sun, 16 Dec 2007) | 1 line

"TYPE ID AS SUB() AS DATATYPE" was being seen as a function. Syntax check added.
------------------------------------------------------------------------
r4410 | jeffmarshall | 2007-12-16 07:24:40 -0800 (Sun, 16 Dec 2007) | 1 line

#1813104 - scancodes for SC_LWIN(&h5b), SC_RWIN(&h5c), SC_MENU(&h5d), are now same in both gfx/console modes and on all platforms
------------------------------------------------------------------------
r4409 | jeffmarshall | 2007-12-16 07:22:50 -0800 (Sun, 16 Dec 2007) | 1 line

updated
------------------------------------------------------------------------
r4408 | jeffmarshall | 2007-12-15 03:59:23 -0800 (Sat, 15 Dec 2007) | 1 line

"-w next" warning not enabled by default with "-w pedantic"
------------------------------------------------------------------------
r4407 | jeffmarshall | 2007-12-14 04:18:25 -0800 (Fri, 14 Dec 2007) | 1 line

wiki examples updated
------------------------------------------------------------------------
r4406 | whotookcha0s-_- | 2007-12-12 23:35:09 -0800 (Wed, 12 Dec 2007) | 1 line

fixed: stabs line number information (?)
------------------------------------------------------------------------
r4405 | whotookcha0s-_- | 2007-12-12 22:39:47 -0800 (Wed, 12 Dec 2007) | 1 line

fixed: the stabs information now contains the datatype of the 'data' and 'ptr' fields in dynamic array descriptors
------------------------------------------------------------------------
r4404 | jeffmarshall | 2007-12-12 14:19:01 -0800 (Wed, 12 Dec 2007) | 1 line

typos
------------------------------------------------------------------------
r4403 | jeffmarshall | 2007-12-10 05:36:45 -0800 (Mon, 10 Dec 2007) | 1 line

check for matching FOR/NEXT vars in all -lang modes.
------------------------------------------------------------------------
r4402 | jeffmarshall | 2007-12-10 05:36:09 -0800 (Mon, 10 Dec 2007) | 1 line

check for matching FOR/NEXT vars in all -lang modes.
------------------------------------------------------------------------
r4401 | jeffmarshall | 2007-12-09 06:57:24 -0800 (Sun, 09 Dec 2007) | 1 line

updated (many missing log entries)
------------------------------------------------------------------------
r4400 | jeffmarshall | 2007-12-09 06:56:23 -0800 (Sun, 09 Dec 2007) | 1 line

pedantic warning when variable name follows NEXT
------------------------------------------------------------------------
r4399 | jeffmarshall | 2007-12-09 04:21:47 -0800 (Sun, 09 Dec 2007) | 1 line

updated
------------------------------------------------------------------------
r4398 | jeffmarshall | 2007-12-09 04:05:44 -0800 (Sun, 09 Dec 2007) | 1 line

wiki examples updated
------------------------------------------------------------------------
r4397 | jeffmarshall | 2007-12-08 18:34:38 -0800 (Sat, 08 Dec 2007) | 1 line

updated
------------------------------------------------------------------------
r4396 | jeffmarshall | 2007-12-08 18:25:11 -0800 (Sat, 08 Dec 2007) | 1 line

wiki samples updated
------------------------------------------------------------------------
r4395 | jeffmarshall | 2007-12-08 02:27:06 -0800 (Sat, 08 Dec 2007) | 1 line

minor addition: CONST in code comments
------------------------------------------------------------------------
r4394 | jeffmarshall | 2007-12-08 02:25:36 -0800 (Sat, 08 Dec 2007) | 1 line

lang qb: Show error if NEXT var does not match FOR var
------------------------------------------------------------------------
r4393 | i_am_drv | 2007-12-06 23:37:34 -0800 (Thu, 06 Dec 2007) | 1 line

revert accidental unrelated change
------------------------------------------------------------------------
r4392 | i_am_drv | 2007-12-06 23:04:46 -0800 (Thu, 06 Dec 2007) | 1 line

make PUT ALPHA with alpha paramter = 0 do nothing
------------------------------------------------------------------------
r4391 | whotookcha0s-_- | 2007-12-04 10:34:56 -0800 (Tue, 04 Dec 2007) | 1 line

fixed: a bug introduced by the new interface for building function pointers in rtl.bas
------------------------------------------------------------------------
r4390 | whotookcha0s-_- | 2007-12-03 17:42:29 -0800 (Mon, 03 Dec 2007) | 2 lines

more robust solution for solving deadlock on app close if screenlock is called with no screenunlock
fixed: the weird desktop resolution screwing bug that was introduced by the old fix
------------------------------------------------------------------------
r4389 | yetifoot | 2007-11-28 00:18:45 -0800 (Wed, 28 Nov 2007) | 1 line

headers for ogg and vorbis
------------------------------------------------------------------------
r4388 | yetifoot | 2007-11-27 10:27:10 -0800 (Tue, 27 Nov 2007) | 1 line

borderless opengl windows on linux
------------------------------------------------------------------------
r4387 | whotookcha0s-_- | 2007-11-25 23:14:31 -0800 (Sun, 25 Nov 2007) | 1 line

fixed: parameter type mismatches weren't being caught with bydesc parameters
------------------------------------------------------------------------
r4386 | i_am_drv | 2007-11-25 20:47:06 -0800 (Sun, 25 Nov 2007) | 1 line

update OpenAL and alut .def files for Win32
------------------------------------------------------------------------
r4385 | i_am_drv | 2007-11-25 20:44:58 -0800 (Sun, 25 Nov 2007) | 1 line

update OpenAL (0.0.8) and alut (1.1.0) headers
------------------------------------------------------------------------
r4384 | i_am_drv | 2007-11-25 19:51:01 -0800 (Sun, 25 Nov 2007) | 1 line

allow GFX_NO_FRAME in Win32 OpenGL gfxlib driver
------------------------------------------------------------------------
r4383 | whotookcha0s-_- | 2007-11-25 02:37:53 -0800 (Sun, 25 Nov 2007) | 1 line

cleanup
------------------------------------------------------------------------
r4382 | whotookcha0s-_- | 2007-11-25 02:36:57 -0800 (Sun, 25 Nov 2007) | 1 line

added a new error message to clarify incorrect const method invocation
------------------------------------------------------------------------
r4381 | whotookcha0s-_- | 2007-11-25 00:58:56 -0800 (Sun, 25 Nov 2007) | 3 lines

fixed: WITH with const UDTs was failing
fixed: calling const procs was imporperly modifying the protoype's type, causing calls before the proc's definition to throw a type mismatch error
fixed: small problem with error recovery when mis-calling const procs
------------------------------------------------------------------------
r4380 | yetifoot | 2007-11-24 06:49:54 -0800 (Sat, 24 Nov 2007) | 1 line

more close window fixes
------------------------------------------------------------------------
r4379 | yetifoot | 2007-11-24 06:44:43 -0800 (Sat, 24 Nov 2007) | 1 line

fix window close code
------------------------------------------------------------------------
r4378 | whotookcha0s-_- | 2007-11-23 11:18:02 -0800 (Fri, 23 Nov 2007) | 3 lines

fixed: const UDT member accesses weren't flagged as illegal
added: const attributes for procs (affects the instance pointer)

------------------------------------------------------------------------
r4377 | whotookcha0s-_- | 2007-11-18 12:02:42 -0800 (Sun, 18 Nov 2007) | 1 line

more reliable method for using native binaries on linux
------------------------------------------------------------------------
r4376 | whotookcha0s-_- | 2007-11-17 16:49:56 -0800 (Sat, 17 Nov 2007) | 1 line

small typo
------------------------------------------------------------------------
r4375 | i_am_drv | 2007-11-16 16:41:31 -0800 (Fri, 16 Nov 2007) | 2 lines

remove unused variable (-Wall cleanup)

------------------------------------------------------------------------
r4374 | i_am_drv | 2007-11-16 16:22:40 -0800 (Fri, 16 Nov 2007) | 2 lines

add declaration of fb_hArrayCtorObj

------------------------------------------------------------------------
r4373 | whotookcha0s-_- | 2007-11-16 06:41:34 -0800 (Fri, 16 Nov 2007) | 1 line

temporary hack to allow fbc to compile using "native" binutils on linux
------------------------------------------------------------------------
r4372 | whotookcha0s-_- | 2007-11-16 06:32:01 -0800 (Fri, 16 Nov 2007) | 1 line

temporary hack to allow fbc to compile using "native" binutils on linux
------------------------------------------------------------------------
r4371 | i_am_drv | 2007-11-14 23:43:57 -0800 (Wed, 14 Nov 2007) | 1 line

DOS console SETMOUSE was not scaling the coordinates
------------------------------------------------------------------------
r4370 | i_am_drv | 2007-11-12 23:33:01 -0800 (Mon, 12 Nov 2007) | 3 lines

replace UBOOL with boolean and define it correctly for non-win32 platforms
NOTE: this has only been tested on win32 and linux; other platforms might need a different definition for boolean

------------------------------------------------------------------------
r4369 | i_am_drv | 2007-11-12 22:48:17 -0800 (Mon, 12 Nov 2007) | 3 lines

- fix type mismatch and implicit conversion
- fix indentation
- read jpeg filename from command line
------------------------------------------------------------------------
r4368 | jeffmarshall | 2007-11-11 17:22:31 -0800 (Sun, 11 Nov 2007) | 1 line

small fix
------------------------------------------------------------------------
r4367 | jeffmarshall | 2007-11-11 16:39:18 -0800 (Sun, 11 Nov 2007) | 1 line

lang qb: INPUT$() function wasn't being allowed.
------------------------------------------------------------------------
r4366 | jeffmarshall | 2007-11-11 16:38:14 -0800 (Sun, 11 Nov 2007) | 1 line

minor updates
------------------------------------------------------------------------
r4365 | jeffmarshall | 2007-11-08 17:34:45 -0800 (Thu, 08 Nov 2007) | 1 line

spelling mistake in cmd help
------------------------------------------------------------------------
r4364 | jeffmarshall | 2007-11-08 17:31:08 -0800 (Thu, 08 Nov 2007) | 1 line

fixed order
------------------------------------------------------------------------
r4363 | jeffmarshall | 2007-11-08 17:28:35 -0800 (Thu, 08 Nov 2007) | 1 line

updated
------------------------------------------------------------------------
r4362 | jeffmarshall | 2007-11-08 17:26:27 -0800 (Thu, 08 Nov 2007) | 1 line

wiki examples updated
------------------------------------------------------------------------
r4361 | whotookcha0s-_- | 2007-11-07 15:37:35 -0800 (Wed, 07 Nov 2007) | 1 line

added constant CSIDL_FLAG_CREATE
------------------------------------------------------------------------
r4360 | whotookcha0s-_- | 2007-11-04 22:54:24 -0800 (Sun, 04 Nov 2007) | 1 line

fixed: error recovery problem
------------------------------------------------------------------------
r4359 | jeffmarshall | 2007-11-04 16:05:17 -0800 (Sun, 04 Nov 2007) | 1 line

Cheap CHM index builder
------------------------------------------------------------------------
r4358 | jeffmarshall | 2007-11-04 16:04:03 -0800 (Sun, 04 Nov 2007) | 1 line

images referenced by manual
------------------------------------------------------------------------
r4357 | yetifoot | 2007-10-31 15:58:48 -0700 (Wed, 31 Oct 2007) | 1 line

add the VK_A-VK_Z and VK_0-VK_9 scancodes
------------------------------------------------------------------------
r4356 | jeffmarshall | 2007-10-30 17:31:27 -0700 (Tue, 30 Oct 2007) | 2 lines

Added RESET(streamno) to reset stdio after redirection or pipe.

------------------------------------------------------------------------
r4355 | jeffmarshall | 2007-10-30 17:24:11 -0700 (Tue, 30 Oct 2007) | 1 line

added prototypes for QB compatible STICK/STRIG/INKEY/SLEEP
------------------------------------------------------------------------
r4354 | jeffmarshall | 2007-10-30 08:52:35 -0700 (Tue, 30 Oct 2007) | 1 line

updated
------------------------------------------------------------------------
r4353 | jeffmarshall | 2007-10-30 08:39:44 -0700 (Tue, 30 Oct 2007) | 1 line

wiki examples updated/added
------------------------------------------------------------------------
r4352 | jeffmarshall | 2007-10-30 08:39:20 -0700 (Tue, 30 Oct 2007) | 1 line

typos in comments
------------------------------------------------------------------------
r4351 | jeffmarshall | 2007-10-30 07:00:38 -0700 (Tue, 30 Oct 2007) | 1 line

This shouldn't even be here
------------------------------------------------------------------------
r4350 | jeffmarshall | 2007-10-30 06:08:02 -0700 (Tue, 30 Oct 2007) | 8 lines

lang qb: 
STICK(n) and STRIG(n) added

Feature Request #1718012:
INKEY$ returns CHR$(0) as the extended character
SLEEP(n) expects seconds


------------------------------------------------------------------------
r4349 | jeffmarshall | 2007-10-30 06:04:43 -0700 (Tue, 30 Oct 2007) | 1 line


------------------------------------------------------------------------
r4348 | jeffmarshall | 2007-10-30 06:02:38 -0700 (Tue, 30 Oct 2007) | 8 lines

lang qb: 
STICK(n) and STRIG(n) added

Feature Request #1718012:
INKEY$ returns CHR$(0) as the extended character
SLEEP(n) expects seconds


------------------------------------------------------------------------
r4347 | jeffmarshall | 2007-10-30 06:01:48 -0700 (Tue, 30 Oct 2007) | 3 lines

lang qb: 
STICK(n) and STRIG(n) added

------------------------------------------------------------------------
r4346 | jeffmarshall | 2007-10-30 05:57:24 -0700 (Tue, 30 Oct 2007) | 8 lines

lang qb: 
STICK(n) and STRIG(n) added

Feature Request #1718012:
INKEY$ returns CHR$(0) as the extended character
SLEEP(n) expects seconds


------------------------------------------------------------------------
r4345 | whotookcha0s-_- | 2007-10-29 05:55:56 -0700 (Mon, 29 Oct 2007) | 2 lines

reverted: the changes made to the parser/rtl modules regarding the rtl(gfx) functions getmouse, getjoystick, screenevent
added: the 'check_const' field to type FB_RTL_PARAMDEF, to specify whether the parameter  should do normal const checking, rtl functions default to no constant checking
------------------------------------------------------------------------
r4344 | whotookcha0s-_- | 2007-10-28 18:00:15 -0700 (Sun, 28 Oct 2007) | 1 line

getmouse, getjoystick, and screenevent weren't being allowed to be called as functions
------------------------------------------------------------------------
r4343 | v1ctor | 2007-10-28 15:46:48 -0700 (Sun, 28 Oct 2007) | 1 line

fixed: the isascii() prototype was missing
------------------------------------------------------------------------
r4342 | whotookcha0s-_- | 2007-10-28 13:33:04 -0700 (Sun, 28 Oct 2007) | 4 lines

udts were inheriting const status from source udts' fields used to assign the destination's fields
typedefs were giving dup def cause of const flags
excluded methods' first param (byref as the_type) from const checks (that's correct?)
small fixes, cleanups
------------------------------------------------------------------------
r4341 | v1ctor | 2007-10-26 13:31:56 -0700 (Fri, 26 Oct 2007) | 1 line

fixed: the isascii() prototype was missing
------------------------------------------------------------------------
r4340 | v1ctor | 2007-10-26 13:20:55 -0700 (Fri, 26 Oct 2007) | 1 line

fixed: the isascii() prototype was missing
------------------------------------------------------------------------
r4339 | whotookcha0s-_- | 2007-10-26 07:26:16 -0700 (Fri, 26 Oct 2007) | 1 line

some fixes
------------------------------------------------------------------------
r4338 | whotookcha0s-_- | 2007-10-25 03:48:17 -0700 (Thu, 25 Oct 2007) | 2 lines

cleaned the const overload solution some more
fixed test
------------------------------------------------------------------------
r4337 | whotookcha0s-_- | 2007-10-25 01:30:12 -0700 (Thu, 25 Oct 2007) | 1 line

new test
------------------------------------------------------------------------
r4336 | whotookcha0s-_- | 2007-10-25 01:25:29 -0700 (Thu, 25 Oct 2007) | 2 lines

fixed let( x, y, z ) = const_struct
test updated
------------------------------------------------------------------------
r4335 | whotookcha0s-_- | 2007-10-25 01:14:53 -0700 (Thu, 25 Oct 2007) | 1 line

changed: allow "3.6e-08f" notation (type specification is accepted after 'e')
------------------------------------------------------------------------
r4334 | whotookcha0s-_- | 2007-10-25 00:06:57 -0700 (Thu, 25 Oct 2007) | 2 lines

fixed mangler
test added
------------------------------------------------------------------------
r4333 | whotookcha0s-_- | 2007-10-24 21:35:23 -0700 (Wed, 24 Oct 2007) | 1 line

couple new tests
------------------------------------------------------------------------
r4332 | whotookcha0s-_- | 2007-10-24 21:31:46 -0700 (Wed, 24 Oct 2007) | 3 lines

new const assignment check, symbCheckConstAssign
improved overload resolution when dealing with consts
various fixes
------------------------------------------------------------------------
r4331 | v1ctor | 2007-10-24 11:43:30 -0700 (Wed, 24 Oct 2007) | 1 line

fixed: 'byval string' params should be passed as 'zstring ptr' internally
------------------------------------------------------------------------
r4330 | whotookcha0s-_- | 2007-10-24 08:46:05 -0700 (Wed, 24 Oct 2007) | 1 line

parameter allocation wasn't preserving const info
------------------------------------------------------------------------
r4329 | whotookcha0s-_- | 2007-10-24 08:02:48 -0700 (Wed, 24 Oct 2007) | 1 line

allowed passing const arg to byval non-const param
------------------------------------------------------------------------
r4328 | whotookcha0s-_- | 2007-10-24 07:47:27 -0700 (Wed, 24 Oct 2007) | 1 line

this too
------------------------------------------------------------------------
r4327 | whotookcha0s-_- | 2007-10-24 07:09:49 -0700 (Wed, 24 Oct 2007) | 2 lines

fixed various improper dtype usage
cleanup
------------------------------------------------------------------------
r4326 | whotookcha0s-_- | 2007-10-24 03:26:57 -0700 (Wed, 24 Oct 2007) | 4 lines

added tests
updated typeof test
changed log-tests makefile to allow debug info
changed cunit test makefile to compile with a higher warning threshold (those warnings were annoying, yes?)
------------------------------------------------------------------------
r4325 | whotookcha0s-_- | 2007-10-24 03:25:19 -0700 (Wed, 24 Oct 2007) | 1 line

now properly handling const pointer assignments
------------------------------------------------------------------------
r4324 | whotookcha0s-_- | 2007-10-24 03:22:25 -0700 (Wed, 24 Oct 2007) | 1 line

updated TYPEOF() to handle const info
------------------------------------------------------------------------
r4323 | whotookcha0s-_- | 2007-10-24 03:09:36 -0700 (Wed, 24 Oct 2007) | 1 line

QB forward call had a small error
------------------------------------------------------------------------
r4322 | whotookcha0s-_- | 2007-10-23 21:57:51 -0700 (Tue, 23 Oct 2007) | 1 line

another small const/type error
------------------------------------------------------------------------
r4321 | whotookcha0s-_- | 2007-10-23 21:44:30 -0700 (Tue, 23 Oct 2007) | 1 line

udt string cast in rtl functions print and write was broken
------------------------------------------------------------------------
r4320 | whotookcha0s-_- | 2007-10-23 02:36:59 -0700 (Tue, 23 Oct 2007) | 2 lines

fixed rtl gfx functions target const checking
added some tests
------------------------------------------------------------------------
r4319 | whotookcha0s-_- | 2007-10-23 01:37:39 -0700 (Tue, 23 Oct 2007) | 1 line

top-level consts must now be initialized when dim'd
------------------------------------------------------------------------
r4318 | whotookcha0s-_- | 2007-10-23 01:24:48 -0700 (Tue, 23 Oct 2007) | 1 line

fixed overload resolution with consts
------------------------------------------------------------------------
r4317 | whotookcha0s-_- | 2007-10-23 01:24:31 -0700 (Tue, 23 Oct 2007) | 1 line

fixed mangling with consts
------------------------------------------------------------------------
r4316 | i_am_drv | 2007-10-23 00:55:22 -0700 (Tue, 23 Oct 2007) | 2 lines

solaris compatibility changes for rtlib (incomplete)

------------------------------------------------------------------------
r4315 | whotookcha0s-_- | 2007-10-23 00:39:58 -0700 (Tue, 23 Oct 2007) | 1 line

procs weren't taking full dtype information when comparing to the prototype
------------------------------------------------------------------------
r4314 | whotookcha0s-_- | 2007-10-22 19:25:00 -0700 (Mon, 22 Oct 2007) | 1 line

update/fix
------------------------------------------------------------------------
r4313 | whotookcha0s-_- | 2007-10-22 19:16:39 -0700 (Mon, 22 Oct 2007) | 4 lines

internal: astGetDataType and symbGetType now return the equivalent of typeGetDtAndPtrOnly, to get the unmasked data, use symbGetFullType and astGetFullType
UDT members now inherit const attribute from the parent UDT (dim as const foo bar: bar.member = value '' error)
forward refs now handle the const mask properly
more..?
------------------------------------------------------------------------
r4312 | whotookcha0s-_- | 2007-10-22 09:15:49 -0700 (Mon, 22 Oct 2007) | 2 lines

added a new regex example (author: Mindless)
removed old example
------------------------------------------------------------------------
r4311 | whotookcha0s-_- | 2007-10-22 04:16:31 -0700 (Mon, 22 Oct 2007) | 1 line

operator '&' was broken if any const strings were being concatenated
------------------------------------------------------------------------
r4310 | i_am_drv | 2007-10-22 01:18:01 -0700 (Mon, 22 Oct 2007) | 2 lines

fix term.h dependency on curses.h on some unices

------------------------------------------------------------------------
r4309 | i_am_drv | 2007-10-22 01:09:09 -0700 (Mon, 22 Oct 2007) | 3 lines

- add configure checks for term.h and termcap.h
- add configure support for Solaris

------------------------------------------------------------------------
r4308 | i_am_drv | 2007-10-21 17:37:51 -0700 (Sun, 21 Oct 2007) | 1 line

lock data accessed in ISR
------------------------------------------------------------------------
r4307 | whotookcha0s-_- | 2007-10-21 13:59:23 -0700 (Sun, 21 Oct 2007) | 1 line

corrected error message when using a const var as a FOR counter
------------------------------------------------------------------------
r4306 | whotookcha0s-_- | 2007-10-21 13:55:53 -0700 (Sun, 21 Oct 2007) | 1 line

added: proper const checks for SCREENEVENT
------------------------------------------------------------------------
r4305 | whotookcha0s-_- | 2007-10-21 13:17:53 -0700 (Sun, 21 Oct 2007) | 3 lines

added more dtype filtering all over...
'= any' optimizations all over...

------------------------------------------------------------------------
r4304 | whotookcha0s-_- | 2007-10-21 10:44:00 -0700 (Sun, 21 Oct 2007) | 3 lines

rtl-file.bas const handling fixed
couple tests fixed
swap/const error handling fixed
------------------------------------------------------------------------
r4303 | whotookcha0s-_- | 2007-10-21 05:51:00 -0700 (Sun, 21 Oct 2007) | 1 line

added some rtl function arg checks for consts, WIP
------------------------------------------------------------------------
r4302 | whotookcha0s-_- | 2007-10-21 02:46:48 -0700 (Sun, 21 Oct 2007) | 1 line

disable const-to-nonconst arg check for rtl procs (needs work still)
------------------------------------------------------------------------
r4301 | whotookcha0s-_- | 2007-10-21 00:45:45 -0700 (Sun, 21 Oct 2007) | 1 line

left in a gdb trap
------------------------------------------------------------------------
r4300 | whotookcha0s-_- | 2007-10-21 00:42:29 -0700 (Sun, 21 Oct 2007) | 1 line

some fixes on args using const
------------------------------------------------------------------------
r4298 | v1ctor | 2007-10-20 11:31:31 -0700 (Sat, 20 Oct 2007) | 1 line

added: CONST qualifier (W.I.P.)
------------------------------------------------------------------------
r4297 | whotookcha0s-_- | 2007-10-20 04:49:32 -0700 (Sat, 20 Oct 2007) | 1 line

was using some old image buffer assumptions
------------------------------------------------------------------------
r4296 | i_am_drv | 2007-10-20 01:43:29 -0700 (Sat, 20 Oct 2007) | 1 line

fixed: SCREENLOCK/SCREENUNLOCK is now properly handled by the DOS gfxlib
------------------------------------------------------------------------
r4295 | yetifoot | 2007-10-19 22:42:01 -0700 (Fri, 19 Oct 2007) | 1 line

1816871  sizeof() is now usable in asm blocks
------------------------------------------------------------------------
r4294 | yetifoot | 2007-10-19 04:54:35 -0700 (Fri, 19 Oct 2007) | 1 line

mouse_init for linux tty was failing
------------------------------------------------------------------------
r4293 | yetifoot | 2007-10-18 03:46:43 -0700 (Thu, 18 Oct 2007) | 1 line

fix for 1815503 do nothing if radius <= 0
------------------------------------------------------------------------
r4292 | v1ctor | 2007-10-14 12:19:18 -0700 (Sun, 14 Oct 2007) | 1 line

updated
------------------------------------------------------------------------
r4291 | v1ctor | 2007-10-14 10:59:54 -0700 (Sun, 14 Oct 2007) | 8 lines

changed:
- lang qb: INTEGER (%) and LONG (&) are now, respectively, 16- and 32-bit wide
- lang qb: default numeric literals are now assumed to be INTEGER or SINGLE
- lang qb: symbols with the same name as keywords are now allowed: dim left as integer: const dim$ = "" etc

fixed:
- GOTO, GOSUB and RETURN were accepting invalid labels

------------------------------------------------------------------------
r4290 | i_am_drv | 2007-10-12 13:42:46 -0700 (Fri, 12 Oct 2007) | 1 line

remove unused file
------------------------------------------------------------------------
r4289 | i_am_drv | 2007-10-12 13:30:55 -0700 (Fri, 12 Oct 2007) | 1 line

allow building with mac os x assembler
------------------------------------------------------------------------
r4286 | jeffmarshall | 2007-10-12 12:18:05 -0700 (Fri, 12 Oct 2007) | 1 line

update to version 0.18.2
------------------------------------------------------------------------
r4276 | jeffmarshall | 2007-10-12 08:45:35 -0700 (Fri, 12 Oct 2007) | 1 line

update 0.18.2 manifests
------------------------------------------------------------------------
r4271 | jeffmarshall | 2007-10-12 00:42:19 -0700 (Fri, 12 Oct 2007) | 1 line

updated
------------------------------------------------------------------------
r4270 | jeffmarshall | 2007-10-12 00:41:39 -0700 (Fri, 12 Oct 2007) | 1 line

updated to 0.18.3
------------------------------------------------------------------------
r4267 | jeffmarshall | 2007-10-12 00:40:41 -0700 (Fri, 12 Oct 2007) | 1 line

add patch level to version/signature
------------------------------------------------------------------------
r4266 | jeffmarshall | 2007-10-12 00:09:00 -0700 (Fri, 12 Oct 2007) | 1 line

copy fbc.1.gz to /usr/share/man/man1/
------------------------------------------------------------------------
r4256 | i_am_drv | 2007-10-11 12:35:04 -0700 (Thu, 11 Oct 2007) | 2 lines

sizeof(wstring) is not always 2

------------------------------------------------------------------------
r4255 | yetifoot | 2007-10-11 12:22:00 -0700 (Thu, 11 Oct 2007) | 1 line

alignment on windows for size 8 elements is different to dos/linux
------------------------------------------------------------------------
r4254 | whotookcha0s-_- | 2007-10-11 06:42:33 -0700 (Thu, 11 Oct 2007) | 1 line

fixed: *cast(zstring ptr, @some_var) = "hello" wasn't working...
------------------------------------------------------------------------
r4253 | whotookcha0s-_- | 2007-10-11 06:23:16 -0700 (Thu, 11 Oct 2007) | 1 line

defined jmpbuf as jmp_buf
------------------------------------------------------------------------
r4247 | jeffmarshall | 2007-10-11 02:47:05 -0700 (Thu, 11 Oct 2007) | 1 line

updated
------------------------------------------------------------------------
r4246 | jeffmarshall | 2007-10-11 01:40:12 -0700 (Thu, 11 Oct 2007) | 1 line

updated
------------------------------------------------------------------------
r4245 | jeffmarshall | 2007-10-11 01:29:06 -0700 (Thu, 11 Oct 2007) | 1 line

tests won't work on linux
------------------------------------------------------------------------
r4244 | jeffmarshall | 2007-10-11 01:27:23 -0700 (Thu, 11 Oct 2007) | 1 line

tests won't work on linux
------------------------------------------------------------------------
r4243 | whotookcha0s-_- | 2007-10-10 16:25:07 -0700 (Wed, 10 Oct 2007) | 1 line

fixed: bug #1684966 - temporaries' dtor flushing needs to be delayed when updating typeini nodes
------------------------------------------------------------------------
r4242 | jeffmarshall | 2007-10-10 14:30:40 -0700 (Wed, 10 Oct 2007) | 1 line

added -prefix option and updated to version 0.18.2
------------------------------------------------------------------------
r4241 | jeffmarshall | 2007-10-10 10:40:43 -0700 (Wed, 10 Oct 2007) | 1 line

added manifests for 0.18.2 release
------------------------------------------------------------------------
r4240 | jeffmarshall | 2007-10-10 06:51:35 -0700 (Wed, 10 Oct 2007) | 1 line

fixed LIBRARY name
------------------------------------------------------------------------
r4239 | jeffmarshall | 2007-10-09 21:20:10 -0700 (Tue, 09 Oct 2007) | 1 line

updated from CompilerDialects page in FBWiki
------------------------------------------------------------------------
r4238 | jeffmarshall | 2007-10-09 21:03:50 -0700 (Tue, 09 Oct 2007) | 1 line

updated
------------------------------------------------------------------------
r4237 | jeffmarshall | 2007-10-09 21:01:23 -0700 (Tue, 09 Oct 2007) | 1 line

fix spelling mistakes
------------------------------------------------------------------------
r4236 | v1ctor | 2007-10-09 09:01:23 -0700 (Tue, 09 Oct 2007) | 1 line

fixed: bug #1809646 (x86 flags assumption)
------------------------------------------------------------------------
r4235 | yetifoot | 2007-10-09 07:52:54 -0700 (Tue, 09 Oct 2007) | 1 line

Fix for bug #1807165 in A ^ 2 optimization if A is integer, should promote A and make sure A * A returns a float, to be consistent with A ^ 3
------------------------------------------------------------------------
r4234 | whotookcha0s-_- | 2007-10-09 04:51:50 -0700 (Tue, 09 Oct 2007) | 1 line

what? you didn't see anything ...
------------------------------------------------------------------------
r4233 | whotookcha0s-_- | 2007-10-09 04:40:19 -0700 (Tue, 09 Oct 2007) | 2 lines

updated logic test
readded drv's optimization branches
------------------------------------------------------------------------
r4232 | yetifoot | 2007-10-09 04:34:04 -0700 (Tue, 09 Oct 2007) | 1 line

quick patch for 1809646
------------------------------------------------------------------------
r4231 | yetifoot | 2007-10-09 04:25:39 -0700 (Tue, 09 Oct 2007) | 1 line


------------------------------------------------------------------------
r4230 | whotookcha0s-_- | 2007-10-09 04:03:34 -0700 (Tue, 09 Oct 2007) | 1 line

added: some regression tests to make sure the logic optimizer works after changes
------------------------------------------------------------------------
r4229 | whotookcha0s-_- | 2007-10-09 03:48:39 -0700 (Tue, 09 Oct 2007) | 1 line

optimizer module was handling some constant logic folding improperly
------------------------------------------------------------------------
r4228 | whotookcha0s-_- | 2007-10-09 03:17:10 -0700 (Tue, 09 Oct 2007) | 2 lines

fixed: DIM wasn't being allowed after COMMON
Made full use of some helper macros
------------------------------------------------------------------------
r4227 | whotookcha0s-_- | 2007-10-09 02:51:10 -0700 (Tue, 09 Oct 2007) | 2 lines

fixed: bug #1772809 - ERASE wasn't reconstructing fixed-length object arrays after destroying them
changed: ERASE is now compatible with QB, that is when ERASE is called on a variable-length array, the bounds information is reset in addition to the data being destroyed
------------------------------------------------------------------------
r4226 | whotookcha0s-_- | 2007-10-09 00:27:35 -0700 (Tue, 09 Oct 2007) | 1 line

fixed: bug #1745685 - rtlib functions whose addresses were taken weren't getting their callbacks called, leading to potential linker errors
------------------------------------------------------------------------
r4225 | whotookcha0s-_- | 2007-10-09 00:06:54 -0700 (Tue, 09 Oct 2007) | 1 line

fixed: bug #1720013 - UDTs with operator LET were being allowed to take a byval arg with the same type as the UDT, causing infinite recursion at execution
------------------------------------------------------------------------
r4224 | whotookcha0s-_- | 2007-10-08 22:34:53 -0700 (Mon, 08 Oct 2007) | 1 line

small fixes on a couple tests
------------------------------------------------------------------------
r4223 | whotookcha0s-_- | 2007-10-08 22:33:58 -0700 (Mon, 08 Oct 2007) | 1 line

fixed: bug #1682972b - UDTs with operator let weren't initializing the destination variable first in a variable initialization, so if operator let didn't do anything, the variable remained uninitialized
------------------------------------------------------------------------
r4222 | whotookcha0s-_- | 2007-10-08 22:31:12 -0700 (Mon, 08 Oct 2007) | 1 line

fixed: bug #1682972a - functions returning UDT's through a hidden 1st arg pointer weren't first initializing the UDT
------------------------------------------------------------------------
r4221 | jeffmarshall | 2007-10-08 11:11:08 -0700 (Mon, 08 Oct 2007) | 1 line

updated
------------------------------------------------------------------------
r4220 | v1ctor | 2007-10-06 08:38:58 -0700 (Sat, 06 Oct 2007) | 1 line

changed: RND is now 100% compatible with QB in -lang qb mode (patch written by counting_pine)
------------------------------------------------------------------------
r4219 | v1ctor | 2007-10-06 08:15:08 -0700 (Sat, 06 Oct 2007) | 1 line

fixed: bug #1738490 - COMMON can't be used with UDT's that have a ctor/dtor (what includes var-len string fields)
------------------------------------------------------------------------
r4218 | v1ctor | 2007-10-05 20:58:13 -0700 (Fri, 05 Oct 2007) | 1 line

fixed: bug #1696229 - string constant members
------------------------------------------------------------------------
r4217 | v1ctor | 2007-10-05 20:46:14 -0700 (Fri, 05 Oct 2007) | 1 line

fixed: bug #1744984
------------------------------------------------------------------------
r4216 | v1ctor | 2007-10-05 20:18:24 -0700 (Fri, 05 Oct 2007) | 1 line

fixed: bug #1805177
------------------------------------------------------------------------
r4215 | v1ctor | 2007-10-05 20:09:22 -0700 (Fri, 05 Oct 2007) | 1 line

fixed: bug #1805176
------------------------------------------------------------------------
r4214 | v1ctor | 2007-10-05 15:55:41 -0700 (Fri, 05 Oct 2007) | 1 line

small fix
------------------------------------------------------------------------
r4213 | v1ctor | 2007-10-05 15:54:03 -0700 (Fri, 05 Oct 2007) | 1 line

updated
------------------------------------------------------------------------
r4212 | v1ctor | 2007-10-05 15:52:07 -0700 (Fri, 05 Oct 2007) | 1 line

fixed: bug #1780306 (temporary variables + GOSUB in -lang qb)
------------------------------------------------------------------------
r4211 | v1ctor | 2007-10-05 15:47:21 -0700 (Fri, 05 Oct 2007) | 1 line

fixed: no dup definition in -lang qb when vars with the same name were declared inside compound stmts
------------------------------------------------------------------------
r4210 | i_am_drv | 2007-10-03 10:27:52 -0700 (Wed, 03 Oct 2007) | 1 line

RSLASH is \, not /
------------------------------------------------------------------------
r4209 | v1ctor | 2007-10-02 20:19:03 -0700 (Tue, 02 Oct 2007) | 1 line

added: support for multiple assignments 'LET( foo, bar, ... ) = udt_expression'
------------------------------------------------------------------------
r4208 | jeffmarshall | 2007-10-01 16:43:48 -0700 (Mon, 01 Oct 2007) | 1 line

new intrinsic defines
------------------------------------------------------------------------
r4207 | jeffmarshall | 2007-10-01 06:56:44 -0700 (Mon, 01 Oct 2007) | 1 line

information added added for compiler configure options 
------------------------------------------------------------------------
r4206 | jeffmarshall | 2007-10-01 06:56:05 -0700 (Mon, 01 Oct 2007) | 1 line


------------------------------------------------------------------------
r4205 | jeffmarshall | 2007-10-01 06:54:17 -0700 (Mon, 01 Oct 2007) | 1 line

wiki examples added (new cross comp intrinsic defines)
------------------------------------------------------------------------
r4204 | jeffmarshall | 2007-09-30 14:35:56 -0700 (Sun, 30 Sep 2007) | 1 line

wiki examples updated
------------------------------------------------------------------------
r4203 | jeffmarshall | 2007-09-30 11:00:26 -0700 (Sun, 30 Sep 2007) | 1 line

Add library option for GNU-ASpell
------------------------------------------------------------------------
r4202 | jeffmarshall | 2007-09-30 10:55:55 -0700 (Sun, 30 Sep 2007) | 1 line

int should have been integer
------------------------------------------------------------------------
r4201 | jeffmarshall | 2007-09-30 10:54:44 -0700 (Sun, 30 Sep 2007) | 1 line

int should have been integer
------------------------------------------------------------------------
r4200 | v1ctor | 2007-09-29 17:32:52 -0700 (Sat, 29 Sep 2007) | 1 line

fixed: ERASE was also calling the default dtors when a object pointer was passed
------------------------------------------------------------------------
r4199 | jeffmarshall | 2007-09-29 16:29:56 -0700 (Sat, 29 Sep 2007) | 1 line

updated
------------------------------------------------------------------------
r4198 | jeffmarshall | 2007-09-29 16:20:43 -0700 (Sat, 29 Sep 2007) | 1 line

wrong name for def file
------------------------------------------------------------------------
r4197 | jeffmarshall | 2007-09-29 16:08:08 -0700 (Sat, 29 Sep 2007) | 1 line

updated
------------------------------------------------------------------------
r4196 | jeffmarshall | 2007-09-29 16:07:40 -0700 (Sat, 29 Sep 2007) | 1 line

GNU ASpell-0.50 header and import library ( translated from win version at http://aspell.net
------------------------------------------------------------------------
r4195 | jeffmarshall | 2007-09-29 16:01:52 -0700 (Sat, 29 Sep 2007) | 1 line

fbrt0.o was being linked with -nodeflibs specified
------------------------------------------------------------------------
r4194 | jeffmarshall | 2007-09-29 15:56:07 -0700 (Sat, 29 Sep 2007) | 1 line

added global PrintLog hook to capture printing in non-console applications ( when fbdoc is used as a library )
------------------------------------------------------------------------
r4193 | whotookcha0s-_- | 2007-09-28 17:37:17 -0700 (Fri, 28 Sep 2007) | 1 line

fixed: REDIM was instantiating objects, even when the array type was a pointer
------------------------------------------------------------------------
r4192 | v1ctor | 2007-09-27 10:22:25 -0700 (Thu, 27 Sep 2007) | 1 line

added: support for PCOPY in text/console modes (DOS and Win32 only)
------------------------------------------------------------------------
r4191 | v1ctor | 2007-09-25 20:22:21 -0700 (Tue, 25 Sep 2007) | 1 line


------------------------------------------------------------------------
r4190 | v1ctor | 2007-09-25 20:18:32 -0700 (Tue, 25 Sep 2007) | 1 line

added: support for PCOPY in text/console modes (DOS and Win32 only)
------------------------------------------------------------------------
r4189 | v1ctor | 2007-09-25 20:18:17 -0700 (Tue, 25 Sep 2007) | 1 line

added: support for PCOPY in text/console modes (DOS and Win32 only)
------------------------------------------------------------------------
r4188 | v1ctor | 2007-09-25 20:16:40 -0700 (Tue, 25 Sep 2007) | 1 line

added: support for PCOPY in text/console modes (DOS and Win32 only)
------------------------------------------------------------------------
r4187 | i_am_drv | 2007-09-24 01:23:47 -0700 (Mon, 24 Sep 2007) | 1 line

revert incorrect change
------------------------------------------------------------------------
r4186 | i_am_drv | 2007-09-24 00:43:41 -0700 (Mon, 24 Sep 2007) | 1 line

remove use of MIN
------------------------------------------------------------------------
r4185 | i_am_drv | 2007-09-24 00:38:38 -0700 (Mon, 24 Sep 2007) | 1 line

rename unix/libfb_h{init,exit}.c to avoid duplicates
------------------------------------------------------------------------
r4184 | i_am_drv | 2007-09-24 00:25:35 -0700 (Mon, 24 Sep 2007) | 1 line

add X include path detection
------------------------------------------------------------------------
r4183 | i_am_drv | 2007-09-23 23:16:18 -0700 (Sun, 23 Sep 2007) | 1 line

add some parts of freebsd rtlib implementation
------------------------------------------------------------------------
r4182 | i_am_drv | 2007-09-23 22:26:50 -0700 (Sun, 23 Sep 2007) | 2 lines

- remove fb_linux.h from generic unix files
- formatting fixes
------------------------------------------------------------------------
r4181 | i_am_drv | 2007-09-23 21:18:27 -0700 (Sun, 23 Sep 2007) | 1 line

move console-related stuff from linux to unix in rtlib
------------------------------------------------------------------------
r4180 | i_am_drv | 2007-09-23 19:41:32 -0700 (Sun, 23 Sep 2007) | 1 line

exit if execvp() fails in fb_ExecEx()
------------------------------------------------------------------------
r4179 | i_am_drv | 2007-09-22 20:25:39 -0700 (Sat, 22 Sep 2007) | 2 lines

- change dos frontend to only use response files on win32 (for using the djgpp tools directly, not cross-compiled ones as would be found on linux etc.)
- add missing paren from last commit
------------------------------------------------------------------------
r4178 | i_am_drv | 2007-09-22 20:22:25 -0700 (Sat, 22 Sep 2007) | 2 lines

- replace some incorrect TARGET_* with __FB_*__
- various other formatting clean-up
------------------------------------------------------------------------
r4177 | v1ctor | 2007-09-22 13:20:33 -0700 (Sat, 22 Sep 2007) | 1 line

updated
------------------------------------------------------------------------
r4176 | v1ctor | 2007-09-22 13:18:31 -0700 (Sat, 22 Sep 2007) | 1 line

added: cursor for INPUT/LINE INPUT in GFX modes
------------------------------------------------------------------------
r4175 | v1ctor | 2007-09-22 10:16:54 -0700 (Sat, 22 Sep 2007) | 1 line

C emitter: updated (W.I.P.)
------------------------------------------------------------------------
r4174 | whotookcha0s-_- | 2007-09-21 06:32:18 -0700 (Fri, 21 Sep 2007) | 1 line

fixed: #1790722 - INPUT wasn't prompting with a '?' if no prompt string was given
------------------------------------------------------------------------
r4173 | whotookcha0s-_- | 2007-09-21 05:36:06 -0700 (Fri, 21 Sep 2007) | 1 line

fixed: #1799517 - VAL() was evaluating out-of-bound characters impromperly in non-base 10 numbers.
------------------------------------------------------------------------
r4172 | whotookcha0s-_- | 2007-09-20 03:53:01 -0700 (Thu, 20 Sep 2007) | 1 line

internal: replaced a couple of gotos with nested block exits
------------------------------------------------------------------------
r4171 | v1ctor | 2007-09-13 21:01:16 -0700 (Thu, 13 Sep 2007) | 1 line

C emitter: temporary vars are now #define's
------------------------------------------------------------------------
r4170 | whotookcha0s-_- | 2007-09-10 21:01:47 -0700 (Mon, 10 Sep 2007) | 1 line

fixed - [re]dim wasn't working properly with EXTERN since the last change
------------------------------------------------------------------------
r4169 | v1ctor | 2007-09-09 16:03:48 -0700 (Sun, 09 Sep 2007) | 1 line

first steps to create the "C" (GCC) generator (W.I.P., the code generated is unusable atm)
------------------------------------------------------------------------
r4168 | v1ctor | 2007-09-08 19:29:56 -0700 (Sat, 08 Sep 2007) | 1 line

removed: unused
------------------------------------------------------------------------
r4167 | v1ctor | 2007-09-08 12:54:35 -0700 (Sat, 08 Sep 2007) | 1 line

updated
------------------------------------------------------------------------
r4165 | v1ctor | 2007-09-08 10:54:24 -0700 (Sat, 08 Sep 2007) | 1 line

changed: fb_ctors/fb_dtors definitions removed (unused)
------------------------------------------------------------------------
r4164 | jeffmarshall | 2007-09-08 09:19:26 -0700 (Sat, 08 Sep 2007) | 1 line

small fix
------------------------------------------------------------------------
r4163 | i_am_drv | 2007-09-07 22:18:03 -0700 (Fri, 07 Sep 2007) | 1 line

set svn:eol-style property
------------------------------------------------------------------------
r4162 | i_am_drv | 2007-09-07 22:00:45 -0700 (Fri, 07 Sep 2007) | 1 line

update config.guess and config.sub
------------------------------------------------------------------------
r4161 | jeffmarshall | 2007-09-07 20:47:16 -0700 (Fri, 07 Sep 2007) | 1 line

updated
------------------------------------------------------------------------
r4160 | jeffmarshall | 2007-09-07 20:34:20 -0700 (Fri, 07 Sep 2007) | 1 line

allow C code blocks to be processed.  (formats as PRE)
------------------------------------------------------------------------
r4159 | jeffmarshall | 2007-09-07 20:31:59 -0700 (Fri, 07 Sep 2007) | 2 lines

wiki examples updated
added: support for building multi-module samples
------------------------------------------------------------------------
r4158 | i_am_drv | 2007-09-07 19:51:02 -0700 (Fri, 07 Sep 2007) | 1 line

add --enable-crosscomp-(target) configure options
------------------------------------------------------------------------
r4157 | i_am_drv | 2007-09-07 19:09:36 -0700 (Fri, 07 Sep 2007) | 2 lines

- combine default target #ifdefs
- remove xbox host #ifdef since the compiler can't actually run there
------------------------------------------------------------------------
r4156 | i_am_drv | 2007-09-07 18:57:13 -0700 (Fri, 07 Sep 2007) | 1 line

alphabetize -target help messages again
------------------------------------------------------------------------
r4155 | i_am_drv | 2007-09-07 18:52:51 -0700 (Fri, 07 Sep 2007) | 1 line

allow all -target values on most platforms
------------------------------------------------------------------------
r4154 | i_am_drv | 2007-09-07 18:46:31 -0700 (Fri, 07 Sep 2007) | 1 line

fix host/target confusion in Makefile
------------------------------------------------------------------------
r4153 | i_am_drv | 2007-09-07 18:32:25 -0700 (Fri, 07 Sep 2007) | 1 line

fix some configure and Makefile confusion between host, build, and target
------------------------------------------------------------------------
r4152 | v1ctor | 2007-09-07 18:18:22 -0700 (Fri, 07 Sep 2007) | 1 line

new tests
------------------------------------------------------------------------
r4151 | v1ctor | 2007-09-07 18:09:51 -0700 (Fri, 07 Sep 2007) | 8 lines

internal changes: 
- rewrote the parser-expr-variable module, making it more easy to write a C emitter later

fixed:
- private constants/enums could be accessed outside methods

readded:
- array indexing and pointer optimizations are now back
------------------------------------------------------------------------
r4150 | i_am_drv | 2007-09-07 18:00:05 -0700 (Fri, 07 Sep 2007) | 1 line

reverse some preprocessor statements so win/dos is the special case to make porting to unix-like systems simpler
------------------------------------------------------------------------
r4149 | i_am_drv | 2007-09-07 13:13:28 -0700 (Fri, 07 Sep 2007) | 1 line

added definitions of GetDefaultPrinterA/W
------------------------------------------------------------------------
r4148 | jeffmarshall | 2007-09-05 05:36:10 -0700 (Wed, 05 Sep 2007) | 1 line

updated
------------------------------------------------------------------------
r4147 | jeffmarshall | 2007-09-05 05:35:21 -0700 (Wed, 05 Sep 2007) | 1 line

updated
------------------------------------------------------------------------
r4146 | whotookcha0s-_- | 2007-09-05 04:21:06 -0700 (Wed, 05 Sep 2007) | 3 lines

fixed: #1785162 - math overflows weren't being checked if the floating point #s were the same 

type
------------------------------------------------------------------------
r4145 | whotookcha0s-_- | 2007-09-05 03:16:36 -0700 (Wed, 05 Sep 2007) | 2 lines

fixed: this was allowed; redim foo( 1 ) as integer: dim as integer bar: dim as integer foo(bar)

------------------------------------------------------------------------
r4144 | whotookcha0s-_- | 2007-09-05 02:11:07 -0700 (Wed, 05 Sep 2007) | 1 line

#1777001 - fixed - the check for dimming duplicate arrays to proc params wasn't allowing redims
------------------------------------------------------------------------
r4143 | v1ctor | 2007-09-03 21:03:55 -0700 (Mon, 03 Sep 2007) | 1 line

fixed: BYTE replaced by UBYTE
------------------------------------------------------------------------
r4142 | jeffmarshall | 2007-09-03 19:19:44 -0700 (Mon, 03 Sep 2007) | 1 line

"dynamic" => "var-len" terminology.
------------------------------------------------------------------------
r4141 | jeffmarshall | 2007-09-03 13:14:48 -0700 (Mon, 03 Sep 2007) | 1 line

wiki examples added
------------------------------------------------------------------------
r4140 | v1ctor | 2007-09-02 06:43:29 -0700 (Sun, 02 Sep 2007) | 1 line

small fix
------------------------------------------------------------------------
r4139 | v1ctor | 2007-09-02 06:36:10 -0700 (Sun, 02 Sep 2007) | 1 line

new tests
------------------------------------------------------------------------
r4138 | jeffmarshall | 2007-09-02 02:25:57 -0700 (Sun, 02 Sep 2007) | 1 line

example added
------------------------------------------------------------------------
r4137 | i_am_drv | 2007-08-31 08:35:22 -0700 (Fri, 31 Aug 2007) | 1 line

add missing extern "c" block (bug #1785469)
------------------------------------------------------------------------
r4136 | v1ctor | 2007-08-26 21:15:40 -0700 (Sun, 26 Aug 2007) | 1 line

updated: using classes
------------------------------------------------------------------------
r4135 | v1ctor | 2007-08-26 20:59:34 -0700 (Sun, 26 Aug 2007) | 1 line

moved: to /libraries
------------------------------------------------------------------------
r4134 | v1ctor | 2007-08-26 20:57:58 -0700 (Sun, 26 Aug 2007) | 1 line

moved: to /libraries
------------------------------------------------------------------------
r4133 | v1ctor | 2007-08-26 20:52:30 -0700 (Sun, 26 Aug 2007) | 1 line

moved: to /libraries
------------------------------------------------------------------------
r4132 | v1ctor | 2007-08-26 20:48:01 -0700 (Sun, 26 Aug 2007) | 1 line

moved: to /libraries
------------------------------------------------------------------------
r4131 | v1ctor | 2007-08-26 14:07:53 -0700 (Sun, 26 Aug 2007) | 1 line

updated: using Extern "windows-ms" now
------------------------------------------------------------------------
r4130 | jeffmarshall | 2007-08-24 06:15:58 -0700 (Fri, 24 Aug 2007) | 1 line

wiki examples added/updated
------------------------------------------------------------------------
r4129 | jeffmarshall | 2007-08-23 16:32:21 -0700 (Thu, 23 Aug 2007) | 1 line

wiki examples added/updated
------------------------------------------------------------------------
r4126 | jeffmarshall | 2007-08-21 21:29:02 -0700 (Tue, 21 Aug 2007) | 1 line

pascal calling convention was emitting bad asm
------------------------------------------------------------------------
r4125 | jeffmarshall | 2007-08-20 12:57:00 -0700 (Mon, 20 Aug 2007) | 1 line

examples updated from wiki
------------------------------------------------------------------------
r4124 | whotookcha0s-_- | 2007-08-20 03:29:02 -0700 (Mon, 20 Aug 2007) | 1 line


------------------------------------------------------------------------
r4123 | whotookcha0s-_- | 2007-08-20 02:16:11 -0700 (Mon, 20 Aug 2007) | 1 line

fixed: #1777001 - array parameters were not being checked for duplicates when DIMming local variable-length arrays
------------------------------------------------------------------------
r4122 | v1ctor | 2007-08-18 14:35:13 -0700 (Sat, 18 Aug 2007) | 1 line

updated
------------------------------------------------------------------------
r4121 | v1ctor | 2007-08-18 14:34:46 -0700 (Sat, 18 Aug 2007) | 4 lines

added: the "windows-ms" mangling support to EXTERN .. END EXTERN, so MS-ish DLL's can be created and used without editing the .def files

fixed: casting overloaded operators were not being checked when initializing variables with incompatible types

------------------------------------------------------------------------
r4120 | jeffmarshall | 2007-08-14 18:45:20 -0700 (Tue, 14 Aug 2007) | 1 line

example file renamed
------------------------------------------------------------------------
r4119 | jeffmarshall | 2007-08-14 18:40:45 -0700 (Tue, 14 Aug 2007) | 1 line

manual/examples moved or removed
------------------------------------------------------------------------
r4118 | jeffmarshall | 2007-08-14 17:40:07 -0700 (Tue, 14 Aug 2007) | 1 line

wiki examples : proguide
------------------------------------------------------------------------
r4117 | jeffmarshall | 2007-08-13 15:53:05 -0700 (Mon, 13 Aug 2007) | 1 line

fixed: bin/freebsd was being selected by the installer script
------------------------------------------------------------------------
r4116 | jeffmarshall | 2007-08-13 13:45:41 -0700 (Mon, 13 Aug 2007) | 1 line

updated to 0.18.2 version number
------------------------------------------------------------------------
r4115 | jeffmarshall | 2007-08-13 13:43:36 -0700 (Mon, 13 Aug 2007) | 1 line

updated header copyright notice.
------------------------------------------------------------------------
r4114 | jeffmarshall | 2007-08-13 02:57:27 -0700 (Mon, 13 Aug 2007) | 1 line

FileLen() works with '/' on all platforms but not '\' on linux
------------------------------------------------------------------------
r4112 | jeffmarshall | 2007-08-13 02:51:23 -0700 (Mon, 13 Aug 2007) | 1 line

libintl.a not needed on linux
------------------------------------------------------------------------
r4107 | jeffmarshall | 2007-08-12 06:25:10 -0700 (Sun, 12 Aug 2007) | 1 line

updated
------------------------------------------------------------------------
r4097 | jeffmarshall | 2007-08-12 04:47:14 -0700 (Sun, 12 Aug 2007) | 1 line

updated for 0.18.1 release
------------------------------------------------------------------------
r4096 | i_am_drv | 2007-08-10 23:28:50 -0700 (Fri, 10 Aug 2007) | 2 lines

fix zstring constructor in libvariant
patch by voodooattack (tracker #1750468)
------------------------------------------------------------------------
r4095 | i_am_drv | 2007-08-10 23:20:01 -0700 (Fri, 10 Aug 2007) | 6 lines

fixes for color function in gfx mode:
- Return just the foreground color for full-color modes, since both colors
can't fit in one 32-bit integer
- Upsample 16-bit colors to 32-bit, with opacity, for compatibility with
other gfx functions.
patch by counting_pine (tracker #1726580)
------------------------------------------------------------------------
r4094 | i_am_drv | 2007-08-10 23:14:31 -0700 (Fri, 10 Aug 2007) | 2 lines

extend 16-bit colors returned from screen function in gfx mode to full 32-bit colors
patch by counting_pine (tracker #1726579)
------------------------------------------------------------------------
r4092 | i_am_drv | 2007-08-10 22:59:14 -0700 (Fri, 10 Aug 2007) | 1 line

remove unused variables
------------------------------------------------------------------------
r4091 | jeffmarshall | 2007-08-06 06:32:12 -0700 (Mon, 06 Aug 2007) | 2 lines

updated for FreeBASIC 0.18.1

------------------------------------------------------------------------
r4090 | jeffmarshall | 2007-08-06 06:13:32 -0700 (Mon, 06 Aug 2007) | 1 line

updated: ODE 0.8 headers, import library, and examples, thanks D.J.Peters
------------------------------------------------------------------------
r4089 | jeffmarshall | 2007-08-06 06:08:33 -0700 (Mon, 06 Aug 2007) | 1 line

updated: removed "Table Of Contents" as top level node in CHM
------------------------------------------------------------------------
r4088 | jeffmarshall | 2007-08-06 06:07:36 -0700 (Mon, 06 Aug 2007) | 1 line

updated
------------------------------------------------------------------------
r4087 | v1ctor | 2007-08-04 18:06:24 -0700 (Sat, 04 Aug 2007) | 1 line

small fix
------------------------------------------------------------------------
r4086 | jeffmarshall | 2007-08-04 10:39:47 -0700 (Sat, 04 Aug 2007) | 2 lines

added: support for ExtLib* pages
added: freebasic styles to style.css for code formatting
------------------------------------------------------------------------
r4085 | jeffmarshall | 2007-08-03 13:44:44 -0700 (Fri, 03 Aug 2007) | 1 line

wiki examples: minor update
------------------------------------------------------------------------
r4084 | i_am_drv | 2007-08-03 13:15:27 -0700 (Fri, 03 Aug 2007) | 1 line

add horizontal scroll and xbutton support on win32 to gfxlib (patch by 1000101 (Eric Cowles))
------------------------------------------------------------------------
r4083 | jeffmarshall | 2007-08-03 09:49:09 -0700 (Fri, 03 Aug 2007) | 1 line

wiki examples: parts of proguide
------------------------------------------------------------------------
r4082 | i_am_drv | 2007-08-01 23:05:56 -0700 (Wed, 01 Aug 2007) | 1 line

fix palette rgb value shifting for vesa
------------------------------------------------------------------------
r4081 | jeffmarshall | 2007-08-01 20:08:16 -0700 (Wed, 01 Aug 2007) | 1 line

wiki examples: check extras meta misc module
------------------------------------------------------------------------
r4080 | jeffmarshall | 2007-08-01 20:01:53 -0700 (Wed, 01 Aug 2007) | 1 line

updated: first steps to support multi-module examples
------------------------------------------------------------------------
r4079 | jeffmarshall | 2007-08-01 19:54:51 -0700 (Wed, 01 Aug 2007) | 1 line

wiki examples: check extras meta misc module
------------------------------------------------------------------------
r4078 | i_am_drv | 2007-08-01 10:43:01 -0700 (Wed, 01 Aug 2007) | 1 line

add vesa vbe palette setter
------------------------------------------------------------------------
r4077 | i_am_drv | 2007-08-01 09:42:40 -0700 (Wed, 01 Aug 2007) | 1 line

add fb_hFarMemSet declarations
------------------------------------------------------------------------
r4076 | jeffmarshall | 2007-07-30 17:16:41 -0700 (Mon, 30 Jul 2007) | 1 line

wiki examples: console data fileio input memory
------------------------------------------------------------------------
r4075 | jeffmarshall | 2007-07-30 07:46:20 -0700 (Mon, 30 Jul 2007) | 1 line

FBWiki samples builder program for examples/manual
------------------------------------------------------------------------
r4074 | jeffmarshall | 2007-07-30 07:42:31 -0700 (Mon, 30 Jul 2007) | 1 line

wiki examples: debug error gfx operator switches
------------------------------------------------------------------------
r4073 | whotookcha0s-_- | 2007-07-29 21:08:25 -0700 (Sun, 29 Jul 2007) | 1 line

changed - TYPEs with default constructor/destructors or methods had to be allowed on the module level. This affects you if you define a TYPE inside a function with any functions inside the TYPE, or any child objects that have constructors or destructors. This includes elements 'as string', since they need constructors and destructors to initialize and clean up memory
------------------------------------------------------------------------
r4072 | v1ctor | 2007-07-29 21:07:10 -0700 (Sun, 29 Jul 2007) | 1 line

fixed: bug #1761961
------------------------------------------------------------------------
r4071 | jeffmarshall | 2007-07-29 13:08:32 -0700 (Sun, 29 Jul 2007) | 1 line

.dtype in the rtl-system function tables was being incrementally modified for FB_DATATYPE_FUNCTION -- causing a problem with calling THREADCREATE in multiple modules specified on the command line.
------------------------------------------------------------------------
r4070 | jeffmarshall | 2007-07-27 07:48:36 -0700 (Fri, 27 Jul 2007) | 1 line

wiki examples: control variable procs udt prepro
------------------------------------------------------------------------
r4069 | whotookcha0s-_- | 2007-07-26 23:08:03 -0700 (Thu, 26 Jul 2007) | 1 line


------------------------------------------------------------------------
r4068 | i_am_drv | 2007-07-26 09:54:28 -0700 (Thu, 26 Jul 2007) | 2 lines

need libintl on freebsd

------------------------------------------------------------------------
r4067 | i_am_drv | 2007-07-26 09:34:33 -0700 (Thu, 26 Jul 2007) | 2 lines

add __FB_FREEBSD__ define

------------------------------------------------------------------------
r4066 | i_am_drv | 2007-07-26 09:12:18 -0700 (Thu, 26 Jul 2007) | 2 lines

add freebsd target and host support in compiler configure script

------------------------------------------------------------------------
r4065 | i_am_drv | 2007-07-26 08:54:05 -0700 (Thu, 26 Jul 2007) | 2 lines

add freebsd linker script

------------------------------------------------------------------------
r4064 | jeffmarshall | 2007-07-26 05:09:25 -0700 (Thu, 26 Jul 2007) | 1 line

gfxlib depends on this header - make sure the unix header gets included
------------------------------------------------------------------------
r4063 | i_am_drv | 2007-07-25 22:30:18 -0700 (Wed, 25 Jul 2007) | 2 lines

add -dynamic-linker

------------------------------------------------------------------------
r4062 | i_am_drv | 2007-07-25 22:14:56 -0700 (Wed, 25 Jul 2007) | 2 lines

add default libs for freebsd

------------------------------------------------------------------------
r4061 | i_am_drv | 2007-07-25 22:03:58 -0700 (Wed, 25 Jul 2007) | 2 lines

add freebsd target frontend

------------------------------------------------------------------------
r4060 | i_am_drv | 2007-07-25 21:49:58 -0700 (Wed, 25 Jul 2007) | 2 lines

replace #ifdefs determining bin extensions in the compiler with FB_HOST_EXEEXT const

------------------------------------------------------------------------
r4059 | jeffmarshall | 2007-07-25 18:01:54 -0700 (Wed, 25 Jul 2007) | 1 line

fixed: missing closing paren
------------------------------------------------------------------------
r4058 | jeffmarshall | 2007-07-25 17:46:17 -0700 (Wed, 25 Jul 2007) | 1 line

fixed: empty FBRT_TARGET_BASE was still being used on dos/win32
------------------------------------------------------------------------
r4057 | jeffmarshall | 2007-07-25 17:43:15 -0700 (Wed, 25 Jul 2007) | 1 line

fixed: would not compile
------------------------------------------------------------------------
r4056 | i_am_drv | 2007-07-25 16:58:40 -0700 (Wed, 25 Jul 2007) | 2 lines

add pathTB entries for freebsd

------------------------------------------------------------------------
r4055 | i_am_drv | 2007-07-25 16:55:19 -0700 (Wed, 25 Jul 2007) | 2 lines

escape $* in install.sh

------------------------------------------------------------------------
r4054 | i_am_drv | 2007-07-25 16:44:56 -0700 (Wed, 25 Jul 2007) | 3 lines

- add -prefix option to compiler to support unixes where exepath can't be determined
- change install.sh to make /usr/bin/fbc a shell script which specifies -prefix instead of a link

------------------------------------------------------------------------
r4053 | i_am_drv | 2007-07-25 14:57:55 -0700 (Wed, 25 Jul 2007) | 2 lines

allow crosscompile from linux to FreeBSD

------------------------------------------------------------------------
r4052 | i_am_drv | 2007-07-25 12:45:22 -0700 (Wed, 25 Jul 2007) | 2 lines

only use .rdata on windows-like systems

------------------------------------------------------------------------
r4051 | i_am_drv | 2007-07-25 11:44:44 -0700 (Wed, 25 Jul 2007) | 2 lines

implement threadcreate stack size parameter for unix

------------------------------------------------------------------------
r4050 | i_am_drv | 2007-07-25 11:27:51 -0700 (Wed, 25 Jul 2007) | 2 lines

split generic unix stuff from fb_linux.h into fb_unix.h

------------------------------------------------------------------------
r4049 | jeffmarshall | 2007-07-24 05:10:53 -0700 (Tue, 24 Jul 2007) | 1 line

wiki examples: strings
------------------------------------------------------------------------
r4048 | jeffmarshall | 2007-07-24 04:47:41 -0700 (Tue, 24 Jul 2007) | 1 line

fixed: was accessing null pointer
------------------------------------------------------------------------
r4047 | jeffmarshall | 2007-07-24 03:24:49 -0700 (Tue, 24 Jul 2007) | 1 line

wiki examples: datatype dates math threads defines casting
------------------------------------------------------------------------
r4046 | jeffmarshall | 2007-07-24 03:21:51 -0700 (Tue, 24 Jul 2007) | 1 line

this also needed to support --disable-objinfo
------------------------------------------------------------------------
r4045 | whotookcha0s-_- | 2007-07-23 22:23:15 -0700 (Mon, 23 Jul 2007) | 1 line


------------------------------------------------------------------------
r4044 | whotookcha0s-_- | 2007-07-23 21:56:32 -0700 (Mon, 23 Jul 2007) | 1 line

added - inc/image_compat.bi, to abstract some of the issues between new/old image formats
------------------------------------------------------------------------
r4043 | whotookcha0s-_- | 2007-07-23 21:53:33 -0700 (Mon, 23 Jul 2007) | 1 line

fixed - #1713886 - symbols in FOR had the temp bit stripped out to make scope jumping work, but it can be added in later, otherwise the branch checker will get confused
------------------------------------------------------------------------
r4042 | whotookcha0s-_- | 2007-07-23 02:28:24 -0700 (Mon, 23 Jul 2007) | 1 line

added - A third parameter to ThreadCreate, you can specify the size of the thread's stack, in bytes
------------------------------------------------------------------------
r4041 | jeffmarshall | 2007-07-22 18:07:48 -0700 (Sun, 22 Jul 2007) | 1 line

wiki examples: array bits system
------------------------------------------------------------------------
r4040 | jeffmarshall | 2007-07-22 10:32:33 -0700 (Sun, 22 Jul 2007) | 1 line

new CWiki methods to assist with programmatic editing of wakka script.
------------------------------------------------------------------------
r4039 | jeffmarshall | 2007-07-21 17:01:21 -0700 (Sat, 21 Jul 2007) | 1 line

updated
------------------------------------------------------------------------
r4038 | jeffmarshall | 2007-07-21 16:51:33 -0700 (Sat, 21 Jul 2007) | 2 lines

Updated to version numbers to 0.18.1
Added --disable-objinfo option to compiler configure script
------------------------------------------------------------------------
r4037 | jeffmarshall | 2007-07-21 16:47:13 -0700 (Sat, 21 Jul 2007) | 1 line

File needed to build on linux - not exactly sure what's supposed to be in here yet though
------------------------------------------------------------------------
r4036 | i_am_drv | 2007-07-19 10:01:33 -0700 (Thu, 19 Jul 2007) | 2 lines

fix install rule for versions of cp that don't accept options after filenames

------------------------------------------------------------------------
r4035 | i_am_drv | 2007-07-19 09:46:40 -0700 (Thu, 19 Jul 2007) | 2 lines

add TARGET_BASE_ defines

------------------------------------------------------------------------
r4034 | i_am_drv | 2007-07-19 09:11:57 -0700 (Thu, 19 Jul 2007) | 2 lines

clean up generic unix rtlib code so it builds on freebsd

------------------------------------------------------------------------
r4033 | whotookcha0s-_- | 2007-07-19 03:59:10 -0700 (Thu, 19 Jul 2007) | 1 line

fixed - ThreadCreate was accepting literally "any" pointer at its first parameter. Added a mechanism for building function pointer symbols (to check against, say the first arg passed to ThreadCreate) at the rtl symbol initializer. (cha0s)
------------------------------------------------------------------------
r4032 | whotookcha0s-_- | 2007-07-19 00:51:00 -0700 (Thu, 19 Jul 2007) | 1 line

fixed - Mangling params taking FB arrays wasn't unique from params as the array's datatype. Recompile any libraries/linked objects taking arrays as parameterse.
------------------------------------------------------------------------
r4031 | i_am_drv | 2007-07-16 14:38:42 -0700 (Mon, 16 Jul 2007) | 2 lines

include base target headers in Makefile dependencies

------------------------------------------------------------------------
r4030 | i_am_drv | 2007-07-16 14:32:10 -0700 (Mon, 16 Jul 2007) | 2 lines

move pthreads rtlib thread implementation to generic unix from linux

------------------------------------------------------------------------
r4029 | i_am_drv | 2007-07-16 14:21:02 -0700 (Mon, 16 Jul 2007) | 2 lines

split generic unix code from linux-specific code in rtlib

------------------------------------------------------------------------
r4028 | whotookcha0s-_- | 2007-07-15 01:24:44 -0700 (Sun, 15 Jul 2007) | 1 line

fixed - error recovery with nested empty UDTs
------------------------------------------------------------------------
r4027 | whotookcha0s-_- | 2007-07-14 22:11:00 -0700 (Sat, 14 Jul 2007) | 3 lines

fixed a crash on the UDT initializer when dealing with non-variables

internal: on symbol initialization, the symbol's parent is explicitly set to NULL, then assigned only if needed. This simplifies the lookup later, as orphan symbols can be ignored.
------------------------------------------------------------------------
r4026 | whotookcha0s-_- | 2007-07-14 21:02:02 -0700 (Sat, 14 Jul 2007) | 1 line

fixed - methods in types' parent had to be set properly
------------------------------------------------------------------------
r4025 | whotookcha0s-_- | 2007-07-14 06:52:38 -0700 (Sat, 14 Jul 2007) | 1 line

tidy
------------------------------------------------------------------------
r4024 | whotookcha0s-_- | 2007-07-14 06:39:33 -0700 (Sat, 14 Jul 2007) | 1 line

the UDT initializer should now be able to handle infinite levels of nested [anonymous] UDTs and unions; inner UDTs/unions' parentheses are fully optional, but may be needed in situations of ambiguity (top-level initializer parentheses are obligatory)
------------------------------------------------------------------------
r4023 | i_am_drv | 2007-07-12 15:30:39 -0700 (Thu, 12 Jul 2007) | 1 line

add [in]/[out] to all \params
------------------------------------------------------------------------
r4022 | i_am_drv | 2007-07-12 15:25:50 -0700 (Thu, 12 Jul 2007) | 1 line

add doxygen comments to GFXDRIVER struct and doxygenize the FBGFX struct comments (no code changes)
------------------------------------------------------------------------
r4021 | whotookcha0s-_- | 2007-07-08 07:40:10 -0700 (Sun, 08 Jul 2007) | 1 line

fixed - the BYREF/BYVAL changes were accidentally set to take place in -lang qb, instead of -lang fb. d'oh!
------------------------------------------------------------------------
r4020 | whotookcha0s-_- | 2007-07-08 07:39:19 -0700 (Sun, 08 Jul 2007) | 1 line

fixed - skipped union elements weren't being tallied
------------------------------------------------------------------------
r4019 | whotookcha0s-_- | 2007-07-08 07:38:31 -0700 (Sun, 08 Jul 2007) | 1 line

made byval explicit
------------------------------------------------------------------------
r4018 | whotookcha0s-_- | 2007-07-06 23:36:25 -0700 (Fri, 06 Jul 2007) | 1 line

changed - the default calling convention of variables has changed in -lang FB. UDT's (including strings) will default to BYREF, all else will default to BYVAL
------------------------------------------------------------------------
r4017 | whotookcha0s-_- | 2007-07-04 02:08:45 -0700 (Wed, 04 Jul 2007) | 1 line

fixed - #1734192 - UDT initialization with unions should have been checking the first union element against the given expresssion. 
------------------------------------------------------------------------
r4016 | whotookcha0s-_- | 2007-07-03 22:08:22 -0700 (Tue, 03 Jul 2007) | 1 line

fixed - screencontrol was rejecting only one arg.
------------------------------------------------------------------------
r4015 | i_am_drv | 2007-07-03 11:03:40 -0700 (Tue, 03 Jul 2007) | 1 line

make hOptNullOp work on longints too
------------------------------------------------------------------------
r4014 | i_am_drv | 2007-07-03 09:30:24 -0700 (Tue, 03 Jul 2007) | 1 line

optimize 'a AND 0', 'a OR -1'
------------------------------------------------------------------------
r4013 | i_am_drv | 2007-07-03 08:04:04 -0700 (Tue, 03 Jul 2007) | 1 line

add build instructions for xbox rtlib
------------------------------------------------------------------------
r4012 | whotookcha0s-_- | 2007-07-03 01:32:47 -0700 (Tue, 03 Jul 2007) | 1 line

added - manifest files for the latest .18 intermediate release
------------------------------------------------------------------------
r4011 | whotookcha0s-_- | 2007-07-03 01:30:58 -0700 (Tue, 03 Jul 2007) | 3 lines

fixed - #1735413 - UDT initialization would fail if the initialization expression was surrounded in parenthesis, due to the udt = (elm, elm) syntax. it will now fallback to check if the first expression encountered matches the udt type, if so, it will attempt a ctor call/assign.

regression test added
------------------------------------------------------------------------
r4010 | i_am_drv | 2007-07-01 20:07:11 -0700 (Sun, 01 Jul 2007) | 1 line

half-working xbox gfxlib2 driver implementation
------------------------------------------------------------------------
r4009 | i_am_drv | 2007-07-01 20:04:27 -0700 (Sun, 01 Jul 2007) | 1 line

xbox rtlib cleanup and improvements
------------------------------------------------------------------------
r4008 | whotookcha0s-_- | 2007-06-29 16:56:25 -0700 (Fri, 29 Jun 2007) | 1 line

updated manifest files
------------------------------------------------------------------------
r4007 | whotookcha0s-_- | 2007-06-28 04:09:11 -0700 (Thu, 28 Jun 2007) | 1 line

added: a second STEP in PUT, see the wiki for details
------------------------------------------------------------------------
r4006 | i_am_drv | 2007-06-22 21:12:05 -0700 (Fri, 22 Jun 2007) | 1 line

use vsnprintf instead of vsprintf in snprintf implementation
------------------------------------------------------------------------
r4005 | whotookcha0s-_- | 2007-06-20 04:31:20 -0700 (Wed, 20 Jun 2007) | 1 line

fixed - gfxlib wasn't waiting long enough when preventing a potential deadlock, causing closing to fail (and crappy graphical consequences)
------------------------------------------------------------------------
r4004 | whotookcha0s-_- | 2007-06-20 03:31:26 -0700 (Wed, 20 Jun 2007) | 1 line

changed - gfxlib creates its windows with THREAD_PRIORITY_ABOVE_NORMAL again, without it was jerky
------------------------------------------------------------------------
r4003 | jeffmarshall | 2007-06-19 06:49:31 -0700 (Tue, 19 Jun 2007) | 1 line

check for &quot in action params and other minor changes
------------------------------------------------------------------------
r4002 | whotookcha0s-_- | 2007-06-17 15:33:47 -0700 (Sun, 17 Jun 2007) | 1 line

fixed - bogus field accesses should have been creating a fake symbol
------------------------------------------------------------------------
r4001 | i_am_drv | 2007-06-15 20:27:12 -0700 (Fri, 15 Jun 2007) | 2 lines

added: minimal mouse clipping for X11

------------------------------------------------------------------------
r4000 | i_am_drv | 2007-06-15 13:48:01 -0700 (Fri, 15 Jun 2007) | 1 line

add svn:executable property to configure scripts
------------------------------------------------------------------------
r3999 | whotookcha0s-_- | 2007-06-14 19:35:23 -0700 (Thu, 14 Jun 2007) | 2 lines

added - a vararg procedure can now be defined in a TYPE, it has to be unique
regression test added.
------------------------------------------------------------------------
r3998 | whotookcha0s-_- | 2007-06-14 16:11:23 -0700 (Thu, 14 Jun 2007) | 3 lines

fixed - the overload resolution wasn't normalizing the total sum of its matching args
regression test added
fixed the screencontrol_s prototype (cosmetic)
------------------------------------------------------------------------
r3997 | i_am_drv | 2007-06-12 08:57:35 -0700 (Tue, 12 Jun 2007) | 1 line

fixed: fullscreen with DirectX gfx driver was not working correctly on some systems  with a single monitor
------------------------------------------------------------------------
r3996 | i_am_drv | 2007-06-11 21:13:07 -0700 (Mon, 11 Jun 2007) | 1 line

fixed: fullscreen with DirectX gfx driver was not working correctly on some systems  with a single monitor
------------------------------------------------------------------------
r3995 | i_am_drv | 2007-06-11 21:12:19 -0700 (Mon, 11 Jun 2007) | 1 line

fixed: fullscreen with DirectX gfx driver was not working correctly on some systems  with a single monitor
------------------------------------------------------------------------
r3994 | i_am_drv | 2007-06-11 13:52:06 -0700 (Mon, 11 Jun 2007) | 1 line

change libsupcxx.a to libsupcx.a on DOS (avoid problems with LFN vs. 8.3 names)
------------------------------------------------------------------------
r3993 | i_am_drv | 2007-06-11 13:34:25 -0700 (Mon, 11 Jun 2007) | 1 line

always terminate exepath string
------------------------------------------------------------------------
r3992 | whotookcha0s-_- | 2007-06-10 13:17:32 -0700 (Sun, 10 Jun 2007) | 1 line

fixed: "as ANY" as being allowed on TYPE members
------------------------------------------------------------------------
r3991 | i_am_drv | 2007-06-09 18:06:45 -0700 (Sat, 09 Jun 2007) | 1 line

move web to a separate module in trunk (out of FreeBASIC/src)
------------------------------------------------------------------------
r3990 | i_am_drv | 2007-06-08 14:00:41 -0700 (Fri, 08 Jun 2007) | 1 line

handle both \ and / in argv[0] in DOS EXEPATH
------------------------------------------------------------------------
r3989 | i_am_drv | 2007-06-07 14:41:11 -0700 (Thu, 07 Jun 2007) | 1 line

split up LZW decoder and encoder (slightly smaller executable if encoder is not used, which is normally the case)
------------------------------------------------------------------------
r3988 | i_am_drv | 2007-06-06 15:59:50 -0700 (Wed, 06 Jun 2007) | 1 line

make EXEPATH always return paths with backslashes on DOS
------------------------------------------------------------------------
r3987 | whotookcha0s-_- | 2007-06-06 09:29:51 -0700 (Wed, 06 Jun 2007) | 1 line

fixed: the 'elements' arg wasn't being checked for validity, causing internal width multiplication to fail on all but numeric types
------------------------------------------------------------------------
r3986 | i_am_drv | 2007-06-05 23:32:10 -0700 (Tue, 05 Jun 2007) | 1 line

fix DOS port after SETMOUSE changes
------------------------------------------------------------------------
r3985 | i_am_drv | 2007-06-05 20:08:48 -0700 (Tue, 05 Jun 2007) | 1 line

add some NT constants to the xbox header (testing SVN and CIA hook)
------------------------------------------------------------------------
r3984 | i_am_drv | 2007-06-04 19:14:07 -0700 (Mon, 04 Jun 2007) | 2 lines

add translation of djgpp sys/exceptn.h

------------------------------------------------------------------------
r3983 | whotookcha0s-_- | 2007-06-01 19:03:02 -0700 (Fri, 01 Jun 2007) | 2 lines

fixed: the inline ASM parser was doing case-sensitive checks for keywords. should be insensitive.

------------------------------------------------------------------------
r3982 | i_am_drv | 2007-05-31 09:54:59 -0700 (Thu, 31 May 2007) | 2 lines

add mouse clip parameter to SETMOUSE

------------------------------------------------------------------------
r3981 | jeffmarshall | 2007-05-30 03:55:38 -0700 (Wed, 30 May 2007) | 2 lines

bug #1680718 - local scope variables were being allowed with static and shared initializers (jeffm)

------------------------------------------------------------------------
r3980 | jeffmarshall | 2007-05-30 03:42:57 -0700 (Wed, 30 May 2007) | 2 lines

libcurl 7.15.4

------------------------------------------------------------------------
r3979 | whotookcha0s-_- | 2007-05-29 05:56:16 -0700 (Tue, 29 May 2007) | 3 lines

MK* had to be reverted, it will not evaluate constant numeric expressions to constant literal  expressions anymore (it will behave like it used to), because of the NULL-terminated nature of literal  strings, sorry (CV* still evaluates to constants(arguably much more important ;))
Also reverted the (redundant) overflow detection, and renamed the 'implict conversion' warnings where applicable.

------------------------------------------------------------------------
r3978 | whotookcha0s-_- | 2007-05-28 19:59:34 -0700 (Mon, 28 May 2007) | 3 lines

fixed: bug #1726019 (Bad conversion of FP constant to uinteger)
added: literal conversion overflow warnings

------------------------------------------------------------------------
r3977 | whotookcha0s-_- | 2007-05-28 15:56:06 -0700 (Mon, 28 May 2007) | 2 lines

fixed: bug #1711210 (Freeze on End without ScreenUnlock)

------------------------------------------------------------------------
r3976 | i_am_drv | 2007-05-27 14:10:53 -0700 (Sun, 27 May 2007) | 2 lines

fix RGB/BGR detection for linear VESA driver when VBE 3.0 fields aren't available

------------------------------------------------------------------------
r3975 | jeffmarshall | 2007-05-26 13:07:35 -0700 (Sat, 26 May 2007) | 2 lines

allow sources to be compiled with 0.16 + 0.17-july-2006-testing

------------------------------------------------------------------------
r3974 | jeffmarshall | 2007-05-26 13:06:36 -0700 (Sat, 26 May 2007) | 2 lines

unmangled enum members were not being emitted to stabs

------------------------------------------------------------------------
r3973 | i_am_drv | 2007-05-24 17:08:13 -0700 (Thu, 24 May 2007) | 2 lines

quicklz example

------------------------------------------------------------------------
r3972 | i_am_drv | 2007-05-24 17:05:10 -0700 (Thu, 24 May 2007) | 2 lines

quicklz header by sir_mud

------------------------------------------------------------------------
r3971 | jeffmarshall | 2007-05-24 14:05:43 -0700 (Thu, 24 May 2007) | 2 lines

bug #1696394 - default colors for GFX primitives were being set using the wrong target bpp

------------------------------------------------------------------------
r3970 | whotookcha0s-_- | 2007-05-22 11:07:52 -0700 (Tue, 22 May 2007) | 2 lines

fixed: TYPEOF would crash when handling an array with an index expression

------------------------------------------------------------------------
r3969 | whotookcha0s-_- | 2007-05-21 21:57:17 -0700 (Mon, 21 May 2007) | 2 lines

PUT would crash with a null source pointer

------------------------------------------------------------------------
r3968 | jeffmarshall | 2007-05-20 09:20:52 -0700 (Sun, 20 May 2007) | 2 lines

updated

------------------------------------------------------------------------
r3967 | jeffmarshall | 2007-05-20 08:48:24 -0700 (Sun, 20 May 2007) | 4 lines

Fixed bug #1716570
- bit positions of bitfields shouldn't advance in a UNION
- regression test added

------------------------------------------------------------------------
r3966 | jeffmarshall | 2007-05-20 08:41:48 -0700 (Sun, 20 May 2007) | 2 lines

font.tga paletted format not readable by lesson24.bas example - reverted to 0.15 version

------------------------------------------------------------------------
r3965 | jeffmarshall | 2007-05-17 19:12:45 -0700 (Thu, 17 May 2007) | 2 lines

typo on german 'readme'

------------------------------------------------------------------------
r3964 | i_am_drv | 2007-05-17 11:02:52 -0700 (Thu, 17 May 2007) | 2 lines

simple LZO example

------------------------------------------------------------------------
r3963 | i_am_drv | 2007-05-17 10:53:55 -0700 (Thu, 17 May 2007) | 2 lines

translation of LZO 2.02 headers

------------------------------------------------------------------------
r3962 | jeffmarshall | 2007-05-16 12:34:09 -0700 (Wed, 16 May 2007) | 2 lines

added gprof[.exe] for win32 and linux

------------------------------------------------------------------------
r3961 | whotookcha0s-_- | 2007-05-15 19:30:23 -0700 (Tue, 15 May 2007) | 2 lines

time_'s arg should be optional, defaulting to NULL

------------------------------------------------------------------------
r3960 | whotookcha0s-_- | 2007-05-14 17:32:18 -0700 (Mon, 14 May 2007) | 2 lines

fixed: overloaded property resolution was failing in some cases

------------------------------------------------------------------------
r3959 | whotookcha0s-_- | 2007-05-14 14:31:48 -0700 (Mon, 14 May 2007) | 2 lines

updated font.tga

------------------------------------------------------------------------
r3958 | jeffmarshall | 2007-05-14 12:01:45 -0700 (Mon, 14 May 2007) | 2 lines

added new files for 0.17 release.  changed final install page to have two options: readme or shell.

------------------------------------------------------------------------
r3957 | jeffmarshall | 2007-05-12 04:36:31 -0700 (Sat, 12 May 2007) | 2 lines

updated current manifests (0.18)

------------------------------------------------------------------------
r3956 | jeffmarshall | 2007-05-12 04:30:58 -0700 (Sat, 12 May 2007) | 2 lines

added the manifests for 0.17 final release

------------------------------------------------------------------------
r3955 | jeffmarshall | 2007-05-12 04:29:36 -0700 (Sat, 12 May 2007) | 2 lines

remove uninst.exe - it's created by the installer

------------------------------------------------------------------------
r3953 | jeffmarshall | 2007-05-12 04:26:15 -0700 (Sat, 12 May 2007) | 2 lines

changed version from 0.16 to 0.17

------------------------------------------------------------------------
r3952 | whotookcha0s-_- | 2007-05-11 17:42:37 -0700 (Fri, 11 May 2007) | 2 lines

fixed: a sub/function left open would cause fbc to seg

------------------------------------------------------------------------
r3951 | i_am_drv | 2007-05-08 12:49:44 -0700 (Tue, 08 May 2007) | 2 lines

declare external function fb_hPutTrans

------------------------------------------------------------------------
r3950 | i_am_drv | 2007-05-08 12:32:14 -0700 (Tue, 08 May 2007) | 2 lines

xbox port doesn't depend on SDL

------------------------------------------------------------------------
r3949 | whotookcha0s-_- | 2007-05-08 02:47:49 -0700 (Tue, 08 May 2007) | 3 lines

Cleaned manifest files
Made TRAM supah awesome

------------------------------------------------------------------------
r3948 | whotookcha0s-_- | 2007-05-07 21:14:33 -0700 (Mon, 07 May 2007) | 2 lines

TRAM updated, see src/contrib/tram/readme.txt for details

------------------------------------------------------------------------
r3947 | i_am_drv | 2007-05-07 21:12:43 -0700 (Mon, 07 May 2007) | 2 lines

win32 ddraw gfxlib now goes fullscreen on the monitor the mouse pointer is on when not switching from windowed mode

------------------------------------------------------------------------
r3946 | i_am_drv | 2007-05-07 21:04:45 -0700 (Mon, 07 May 2007) | 2 lines

fix win32 ddraw fullscreen when not switching from windowed mode

------------------------------------------------------------------------
r3945 | jeffmarshall | 2007-05-07 13:19:20 -0700 (Mon, 07 May 2007) | 2 lines

updated and sorted fbc-current (0.17) source lists

------------------------------------------------------------------------
r3944 | jeffmarshall | 2007-05-07 13:17:17 -0700 (Mon, 07 May 2007) | 2 lines

added fbc-0.16 source lists

------------------------------------------------------------------------
r3943 | jeffmarshall | 2007-05-07 11:43:56 -0700 (Mon, 07 May 2007) | 2 lines

remove some dos related files

------------------------------------------------------------------------
r3942 | whotookcha0s-_- | 2007-05-07 11:23:00 -0700 (Mon, 07 May 2007) | 2 lines

added: source lists

------------------------------------------------------------------------
r3941 | jeffmarshall | 2007-05-06 21:04:50 -0700 (Sun, 06 May 2007) | 2 lines

added package manifests for fbc current

------------------------------------------------------------------------
r3940 | jeffmarshall | 2007-05-06 21:04:21 -0700 (Sun, 06 May 2007) | 2 lines

added package manifests for fbc-0.16

------------------------------------------------------------------------
r3939 | jeffmarshall | 2007-05-06 18:32:40 -0700 (Sun, 06 May 2007) | 2 lines

possible bug in liburl 7.16.2 requires a fresh connection when GETting raw wiki pages and IDs.

------------------------------------------------------------------------
r3938 | jeffmarshall | 2007-05-05 05:24:24 -0700 (Sat, 05 May 2007) | 2 lines

updated for libcurl 7.16.2 - missed a few defines

------------------------------------------------------------------------
r3937 | jeffmarshall | 2007-05-05 04:37:17 -0700 (Sat, 05 May 2007) | 2 lines

updated for libcurl 7.16.2

------------------------------------------------------------------------
r3936 | whotookcha0s-_- | 2007-05-05 01:34:21 -0700 (Sat, 05 May 2007) | 2 lines

fixed potential path resolution error on Linux

------------------------------------------------------------------------
r3935 | whotookcha0s-_- | 2007-05-05 01:12:36 -0700 (Sat, 05 May 2007) | 4 lines

fixed: the #include once and #pragma once PP directives weren't fully solving the paths of their include  files

regression tests added/fixed

------------------------------------------------------------------------
r3934 | jeffmarshall | 2007-05-04 19:11:34 -0700 (Fri, 04 May 2007) | 2 lines

-lang changes

------------------------------------------------------------------------
r3933 | whotookcha0s-_- | 2007-05-04 11:17:01 -0700 (Fri, 04 May 2007) | 2 lines

fixed: pointer counters in FOR were stripping all indirection levels, instead of just one.

------------------------------------------------------------------------
r3932 | whotookcha0s-_- | 2007-05-04 06:39:53 -0700 (Fri, 04 May 2007) | 2 lines

fixed case-sensitvity (thanks garvan)

------------------------------------------------------------------------
r3931 | whotookcha0s-_- | 2007-05-03 16:57:26 -0700 (Thu, 03 May 2007) | 3 lines

removed some backwards compatibility stuff
added some files to exclude from the DOS version ...

------------------------------------------------------------------------
r3930 | whotookcha0s-_- | 2007-05-02 13:47:16 -0700 (Wed, 02 May 2007) | 2 lines

updated to .18...

------------------------------------------------------------------------
r3929 | i_am_drv | 2007-05-01 21:31:48 -0700 (Tue, 01 May 2007) | 2 lines

- inc/dos/dir.bi is redundant - just make it include the inc/crt version

------------------------------------------------------------------------
r3928 | i_am_drv | 2007-05-01 21:24:20 -0700 (Tue, 01 May 2007) | 4 lines

- change names so they match the original C names (underscores at beginning)
- add a few things which depended on features that weren't available when originally written (bitfields, #define with arguments)
- fix missing return type on a couple of functions

------------------------------------------------------------------------
r3926 | whotookcha0s-_- | 2007-05-01 19:17:05 -0700 (Tue, 01 May 2007) | 2 lines

updated TRAM

------------------------------------------------------------------------
r3925 | whotookcha0s-_- | 2007-05-01 12:05:35 -0700 (Tue, 01 May 2007) | 2 lines

fixed buffer sizes

------------------------------------------------------------------------
r3924 | whotookcha0s-_- | 2007-04-29 20:50:17 -0700 (Sun, 29 Apr 2007) | 2 lines

internal: all pointer-related operations are now abstracted using macros

------------------------------------------------------------------------
r3923 | i_am_drv | 2007-04-21 22:27:24 -0700 (Sat, 21 Apr 2007) | 2 lines

avoid potential read past end of buffer when converting 24bpp bitmaps

------------------------------------------------------------------------
r3922 | i_am_drv | 2007-04-18 06:46:31 -0700 (Wed, 18 Apr 2007) | 2 lines

added more constants from w32api header

------------------------------------------------------------------------
r3921 | whotookcha0s-_- | 2007-04-16 16:55:11 -0700 (Mon, 16 Apr 2007) | 2 lines

changed: internal restructuring/cleaning

------------------------------------------------------------------------
r3919 | jeffmarshall | 2007-04-11 12:42:54 -0700 (Wed, 11 Apr 2007) | 2 lines

Export symbols (procs) declared in TYPE's.

------------------------------------------------------------------------
r3918 | jeffmarshall | 2007-04-10 12:29:10 -0700 (Tue, 10 Apr 2007) | 2 lines

Fixed missing/incorrect glyphs for control characters ( submitted by Michael Webster )

------------------------------------------------------------------------
r3917 | whotookcha0s-_- | 2007-04-10 12:28:16 -0700 (Tue, 10 Apr 2007) | 2 lines

backwards compatibility hack

------------------------------------------------------------------------
r3916 | whotookcha0s-_- | 2007-04-09 11:16:01 -0700 (Mon, 09 Apr 2007) | 3 lines

fixed: CVS, CVD, CVLONGIN
regression test updated...

------------------------------------------------------------------------
r3915 | i_am_drv | 2007-04-09 04:46:24 -0700 (Mon, 09 Apr 2007) | 2 lines

use C-style comments when using gcc -x assembler-with-cpp

------------------------------------------------------------------------
r3914 | i_am_drv | 2007-04-08 20:52:38 -0700 (Sun, 08 Apr 2007) | 2 lines

fix duplicate definition..

------------------------------------------------------------------------
r3913 | i_am_drv | 2007-04-08 20:51:07 -0700 (Sun, 08 Apr 2007) | 2 lines

always link crt0.o for xbox

------------------------------------------------------------------------
r3912 | i_am_drv | 2007-04-08 20:39:44 -0700 (Sun, 08 Apr 2007) | 2 lines

update/fix xbox frontend

------------------------------------------------------------------------
r3911 | i_am_drv | 2007-04-08 20:16:28 -0700 (Sun, 08 Apr 2007) | 2 lines

add patch for openxdk configure script to allow building with mingw/msys

------------------------------------------------------------------------
r3910 | whotookcha0s-_- | 2007-04-05 20:49:40 -0700 (Thu, 05 Apr 2007) | 2 lines

fixed: bug #1690717, patch submitted by counting_pine (thanks)

------------------------------------------------------------------------
r3909 | jeffmarshall | 2007-04-04 13:57:48 -0700 (Wed, 04 Apr 2007) | 2 lines

added support for including images in the HTML/CHM conversion

------------------------------------------------------------------------
r3908 | whotookcha0s-_- | 2007-04-04 03:25:10 -0700 (Wed, 04 Apr 2007) | 2 lines

/me stupid...

------------------------------------------------------------------------
r3907 | whotookcha0s-_- | 2007-04-04 03:23:43 -0700 (Wed, 04 Apr 2007) | 2 lines

fixed: error reporting on periods in method names

------------------------------------------------------------------------
r3906 | whotookcha0s-_- | 2007-04-04 03:09:10 -0700 (Wed, 04 Apr 2007) | 2 lines

fixed: len wasn't allowed as a member method name

------------------------------------------------------------------------
r3905 | jeffmarshall | 2007-04-01 07:04:10 -0700 (Sun, 01 Apr 2007) | 2 lines

WORKAROUND for #1678350

------------------------------------------------------------------------
r3904 | whotookcha0s-_- | 2007-04-01 05:20:22 -0700 (Sun, 01 Apr 2007) | 2 lines

I meant to commit this file last time >.<

------------------------------------------------------------------------
r3903 | i_am_drv | 2007-04-01 05:00:49 -0700 (Sun, 01 Apr 2007) | 2 lines

change thread parameter to any ptr

------------------------------------------------------------------------
r3902 | i_am_drv | 2007-04-01 04:56:40 -0700 (Sun, 01 Apr 2007) | 2 lines

fix line endings

------------------------------------------------------------------------
r3901 | whotookcha0s-_- | 2007-04-01 04:09:34 -0700 (Sun, 01 Apr 2007) | 3 lines

fixed: hCalcTypesDiff couldn't assume the top 32-bits would be cleared
if the type was a 32-bit integer.

------------------------------------------------------------------------
r3900 | jeffmarshall | 2007-03-31 19:37:30 -0700 (Sat, 31 Mar 2007) | 2 lines

remove extra AS

------------------------------------------------------------------------
r3899 | i_am_drv | 2007-03-28 04:34:22 -0700 (Wed, 28 Mar 2007) | 2 lines

move divide outside loop

------------------------------------------------------------------------
r3898 | whotookcha0s-_- | 2007-03-27 18:24:56 -0700 (Tue, 27 Mar 2007) | 2 lines

fixed: unsigned char's type code was "h", not "b".

------------------------------------------------------------------------
r3897 | whotookcha0s-_- | 2007-03-27 17:56:47 -0700 (Tue, 27 Mar 2007) | 2 lines

updated copyright notice to 2007

------------------------------------------------------------------------
r3896 | i_am_drv | 2007-03-27 02:06:56 -0700 (Tue, 27 Mar 2007) | 2 lines

add more tree view constants

------------------------------------------------------------------------
r3895 | whotookcha0s-_- | 2007-03-26 18:26:16 -0700 (Mon, 26 Mar 2007) | 2 lines

regression test in case cha0s tries fixing/breaking things again

------------------------------------------------------------------------
r3894 | whotookcha0s-_- | 2007-03-26 18:12:18 -0700 (Mon, 26 Mar 2007) | 2 lines

reverted: allowing UDT casts to resolve overloaded procs got messy...

------------------------------------------------------------------------
r3893 | whotookcha0s-_- | 2007-03-25 19:26:41 -0700 (Sun, 25 Mar 2007) | 2 lines

cia test

------------------------------------------------------------------------
r3892 | whotookcha0s-_- | 2007-03-25 17:45:10 -0700 (Sun, 25 Mar 2007) | 2 lines

fixed: palette would access invalid memory if the index given was >= maximum palette indices.

------------------------------------------------------------------------
r3891 | whotookcha0s-_- | 2007-03-25 01:31:13 -0700 (Sun, 25 Mar 2007) | 4 lines

fixed: ulongints were being improperly seen as pointers since the bottom 32-bits were 0 and they were  only being checked as the native integer type(thanks counting_pine)
fixed: ulongints would qualify as pointers if they were completely 0, a check had to be added for  native pointer width equivalence.
regression test added

------------------------------------------------------------------------
r3890 | whotookcha0s-_- | 2007-03-24 05:08:44 -0700 (Sat, 24 Mar 2007) | 3 lines

changed: allowed UDT's to use implicit casts in resolving overloaded proc args (#1678165)
regression test added

------------------------------------------------------------------------
r3889 | whotookcha0s-_- | 2007-03-24 02:13:14 -0700 (Sat, 24 Mar 2007) | 2 lines

fixed: hCheckByDescParam() wasn't checking if the symbol was a var class before accessing the union

------------------------------------------------------------------------
r3888 | jeffmarshall | 2007-03-23 20:39:17 -0700 (Fri, 23 Mar 2007) | 2 lines

fixed: bug # 1687189

------------------------------------------------------------------------
r3887 | jeffmarshall | 2007-03-23 20:15:17 -0700 (Fri, 23 Mar 2007) | 2 lines

fixed: putters were being set to non-static local data

------------------------------------------------------------------------
r3886 | i_am_drv | 2007-03-23 00:13:53 -0700 (Fri, 23 Mar 2007) | 2 lines

updated to libpng-1.2.16

------------------------------------------------------------------------
r3885 | i_am_drv | 2007-03-22 23:33:26 -0700 (Thu, 22 Mar 2007) | 2 lines

updated

------------------------------------------------------------------------
r3884 | whotookcha0s-_- | 2007-03-22 13:13:13 -0700 (Thu, 22 Mar 2007) | 2 lines

fixed: TYPEOF() was screwing up when the base type was void.

------------------------------------------------------------------------
r3883 | whotookcha0s-_- | 2007-03-20 03:34:40 -0700 (Tue, 20 Mar 2007) | 2 lines

cia test

------------------------------------------------------------------------
r3882 | whotookcha0s-_- | 2007-03-20 03:32:13 -0700 (Tue, 20 Mar 2007) | 2 lines

testing

------------------------------------------------------------------------
r3881 | v1ctor | 2007-03-18 18:18:12 -0700 (Sun, 18 Mar 2007) | 2 lines

changed: the 'VARIANT' struct was renamed to 'VARIANT_' (an 'VARIANT' alias was also added)

------------------------------------------------------------------------
r3880 | v1ctor | 2007-03-18 18:13:30 -0700 (Sun, 18 Mar 2007) | 2 lines

changed: CVariant struct renamed to VARIANT

------------------------------------------------------------------------
r3879 | v1ctor | 2007-03-18 17:32:06 -0700 (Sun, 18 Mar 2007) | 2 lines

small fix

------------------------------------------------------------------------
r3878 | lillo | 2007-03-18 13:48:01 -0700 (Sun, 18 Mar 2007) | 2 lines

reenabled MMX (forgot about it), added optimized special ALPHA mask MMX blitter

------------------------------------------------------------------------
r3877 | v1ctor | 2007-03-18 12:15:49 -0700 (Sun, 18 Mar 2007) | 2 lines

updated

------------------------------------------------------------------------
r3876 | lillo | 2007-03-18 07:53:52 -0700 (Sun, 18 Mar 2007) | 2 lines

avoid linking TRANS putter if not using any custom font in DRAW STRING

------------------------------------------------------------------------
r3875 | lillo | 2007-03-18 07:39:32 -0700 (Sun, 18 Mar 2007) | 2 lines

splitted PUT into several source files, one for each drawing method, helping reducing EXE size when using PUT

------------------------------------------------------------------------
r3874 | v1ctor | 2007-03-17 14:32:38 -0700 (Sat, 17 Mar 2007) | 2 lines

updated

------------------------------------------------------------------------
r3873 | v1ctor | 2007-03-17 14:10:03 -0700 (Sat, 17 Mar 2007) | 2 lines

fixed: bug #1681640

------------------------------------------------------------------------
r3872 | whotookcha0s-_- | 2007-03-16 15:57:13 -0700 (Fri, 16 Mar 2007) | 2 lines

fixed: STATIC/SHARED array initalizers would fail if the array creation failed, causing a segmentation violation

------------------------------------------------------------------------
r3871 | whotookcha0s-_- | 2007-03-15 20:12:01 -0700 (Thu, 15 Mar 2007) | 2 lines

fixed: mid statement was taking off one too many chars

------------------------------------------------------------------------
r3870 | i_am_drv | 2007-03-15 12:22:20 -0700 (Thu, 15 Mar 2007) | 2 lines

updated man page

------------------------------------------------------------------------
r3869 | whotookcha0s-_- | 2007-03-14 20:06:03 -0700 (Wed, 14 Mar 2007) | 4 lines

fixed: VAR, when creating static variables, wasn't checking the initalizer's constant requirement

added: STATIC VAR foo = bar

------------------------------------------------------------------------
r3868 | lillo | 2007-03-14 15:32:25 -0700 (Wed, 14 Mar 2007) | 6 lines

Made all PUT blenders affect the destination alpha on 32bpp targets.
Specifically:
- ADD,<value>: dstA = ((srcA * value) >> 8) + dstA
- ALPHA,<value>: dstA = (((srcA - dstA) * value) >> 8) + dstA
- ALPHA: dstA = (((srcA - dstA) * srcA) >> 8) + dstA

------------------------------------------------------------------------
r3867 | whotookcha0s-_- | 2007-03-13 21:35:06 -0700 (Tue, 13 Mar 2007) | 2 lines

changed: Addition, subtraction, multiplication, and both types of division can now be done at the preprocessor

------------------------------------------------------------------------
r3866 | whotookcha0s-_- | 2007-03-12 03:33:35 -0700 (Mon, 12 Mar 2007) | 2 lines

forgot this file...

------------------------------------------------------------------------
r3865 | whotookcha0s-_- | 2007-03-12 03:22:10 -0700 (Mon, 12 Mar 2007) | 4 lines

changed: TYPE() now supports intrinsic types

type<typeof(foo)>() now works as well

------------------------------------------------------------------------
r3864 | whotookcha0s-_- | 2007-03-11 04:43:58 -0700 (Sun, 11 Mar 2007) | 2 lines

fixed: VAR was trying to add a ctor call to the AST even when the initialization was not a ctor but a regular assign

------------------------------------------------------------------------
r3863 | jeffmarshall | 2007-03-09 07:32:26 -0800 (Fri, 09 Mar 2007) | 2 lines

added CULNG

------------------------------------------------------------------------
r3862 | i_am_drv | 2007-03-08 16:45:47 -0800 (Thu, 08 Mar 2007) | 2 lines

remove FlashWindowEx dependency (for compatibility with Win9x)

------------------------------------------------------------------------
r3861 | jeffmarshall | 2007-03-08 11:22:28 -0800 (Thu, 08 Mar 2007) | 2 lines

added open addressed hash table support to improve page lookup and keyword lookup performance

------------------------------------------------------------------------
r3860 | whotookcha0s-_- | 2007-03-07 04:13:47 -0800 (Wed, 07 Mar 2007) | 2 lines

added: stack size check against static arrays (requested by yetifoot)

------------------------------------------------------------------------
r3859 | whotookcha0s-_- | 2007-03-07 02:03:32 -0800 (Wed, 07 Mar 2007) | 2 lines

added: stylin submitted a patch to warn when deleting ANY pointers

------------------------------------------------------------------------
r3858 | whotookcha0s-_- | 2007-03-07 01:03:54 -0800 (Wed, 07 Mar 2007) | 2 lines

fixed: AST optimizations can't/shouldn't be done when taking the address of an array with non-constant indexing expressions

------------------------------------------------------------------------
r3857 | lillo | 2007-03-06 07:30:59 -0800 (Tue, 06 Mar 2007) | 4 lines

All alpha blitters now correctly set destination alpha value to the
source alpha value. Only exception is add blender, where dest_alpha =
src_alpha + dest_alpha

------------------------------------------------------------------------
r3856 | jeffmarshall | 2007-03-05 06:40:56 -0800 (Mon, 05 Mar 2007) | 2 lines

moved c++ constructor definition outside the struct.

------------------------------------------------------------------------
r3855 | jeffmarshall | 2007-03-04 11:30:59 -0800 (Sun, 04 Mar 2007) | 10 lines

changed:
- [[www.links]] not seen as urls
- WikiToken now uses id property to (de)allocate extended token information
added:
- CWiki->Build() method, the opposite of Parse()
- new keyword(s)
- TODO.txt and changelog.txt files
updated:
- readme.txt

------------------------------------------------------------------------
r3854 | lillo | 2007-03-04 07:54:42 -0800 (Sun, 04 Mar 2007) | 2 lines

removed debug output

------------------------------------------------------------------------
r3853 | lillo | 2007-03-03 05:07:42 -0800 (Sat, 03 Mar 2007) | 2 lines

Fixed PUT when using PSET/PRESET/AND/OR/XOR drawing modes on target buffers with a depth different than the current screen one

------------------------------------------------------------------------
r3852 | whotookcha0s-_- | 2007-03-03 00:57:18 -0800 (Sat, 03 Mar 2007) | 4 lines

fixed - literals weren't allowed in TYPEOF(), nor enums

changed - TYPEOF() will now accept intrinsic types like INTEGER, and also UDT's, without needing to pass a variable of that type.

------------------------------------------------------------------------
r3851 | i_am_drv | 2007-03-02 09:04:55 -0800 (Fri, 02 Mar 2007) | 2 lines

fixed possible null pointer dereference in ddenum_callback

------------------------------------------------------------------------
r3850 | whotookcha0s-_- | 2007-02-28 05:07:54 -0800 (Wed, 28 Feb 2007) | 3 lines

changed: the UDT in FOR syntax changed AGAIN =) Check examples/classes for the iterator examples.
fixed: UDT in FOR wasn't reporting the correct error for incorrectly overloaded FOR..STEP...NEXT ops.

------------------------------------------------------------------------
r3849 | lillo | 2007-02-27 02:11:10 -0800 (Tue, 27 Feb 2007) | 2 lines

fixed bug #1662406

------------------------------------------------------------------------
r3848 | whotookcha0s-_- | 2007-02-25 22:48:37 -0800 (Sun, 25 Feb 2007) | 5 lines

added: the TYPEOF() compiler intrinsic, which substitues the type of the variable given in it's place, "dim as typeof(foo) bar"... see
  http://www.freebasic.net/wiki/wikka.php?wakka=KeyPgTypeof for details

fixed: the BIT, BITSET, BITRESET macros now use TYPEOF() to determine the variable type when accessing.

------------------------------------------------------------------------
r3847 | whotookcha0s-_- | 2007-02-25 04:45:02 -0800 (Sun, 25 Feb 2007) | 2 lines

fixed: fb_DateTimeParse wasn't checking for a NULL data pointer

------------------------------------------------------------------------
r3846 | whotookcha0s-_- | 2007-02-25 03:14:42 -0800 (Sun, 25 Feb 2007) | 4 lines

fixed: with wasn't allowing a property for its variable

internal: added an interface to force cVarOrDeref() to parse as an expression

------------------------------------------------------------------------
r3845 | whotookcha0s-_- | 2007-02-25 00:57:12 -0800 (Sun, 25 Feb 2007) | 2 lines

fixed - delete wouldn't accept a property returning a pointer

------------------------------------------------------------------------
r3844 | i_am_drv | 2007-02-24 19:09:06 -0800 (Sat, 24 Feb 2007) | 2 lines

fix DirectDrawEnumerateEx callback return value (keep enumerating if monitor not found)

------------------------------------------------------------------------
r3843 | v1ctor | 2007-02-24 04:56:37 -0800 (Sat, 24 Feb 2007) | 2 lines

small speedup

------------------------------------------------------------------------
r3842 | v1ctor | 2007-02-24 04:55:05 -0800 (Sat, 24 Feb 2007) | 2 lines

new test

------------------------------------------------------------------------
r3841 | v1ctor | 2007-02-24 04:54:23 -0800 (Sat, 24 Feb 2007) | 2 lines

fixed: var-len string fields couldn't be initialized

------------------------------------------------------------------------
r3840 | v1ctor | 2007-02-22 17:54:18 -0800 (Thu, 22 Feb 2007) | 2 lines

updated

------------------------------------------------------------------------
r3839 | i_am_drv | 2007-02-22 15:55:45 -0800 (Thu, 22 Feb 2007) | 2 lines

- added freebsd as compiler target (unfinished)

------------------------------------------------------------------------
r3838 | v1ctor | 2007-02-22 15:35:57 -0800 (Thu, 22 Feb 2007) | 2 lines

added: CALL to forward functions in -lang qb mode

------------------------------------------------------------------------
r3837 | v1ctor | 2007-02-22 14:26:31 -0800 (Thu, 22 Feb 2007) | 2 lines

fixed: bug #1660036

------------------------------------------------------------------------
r3836 | v1ctor | 2007-02-22 12:45:17 -0800 (Thu, 22 Feb 2007) | 4 lines

fixed:
- the quotes of literal strings were not being preserved when used inside #IF expressions;
- symbols were case-sensitive when compared inside #IF expressions.

------------------------------------------------------------------------
r3835 | v1ctor | 2007-02-22 08:57:31 -0800 (Thu, 22 Feb 2007) | 2 lines

updated (patch sent by counting_pine)

------------------------------------------------------------------------
r3834 | v1ctor | 2007-02-22 08:53:47 -0800 (Thu, 22 Feb 2007) | 2 lines

fixed: trailing zeros were not being preserved in DATA when -lang qb was used

------------------------------------------------------------------------
r3833 | v1ctor | 2007-02-22 05:51:29 -0800 (Thu, 22 Feb 2007) | 2 lines

fixed: var-len string fields couldn't be initialized

------------------------------------------------------------------------
r3831 | v1ctor | 2007-02-21 18:57:06 -0800 (Wed, 21 Feb 2007) | 4 lines

fixed: arguments couldn't be passed by descriptor to constructors when initializing arrays of objects

changed: STEP re-added as operator (when UDT's are used with FOR)

------------------------------------------------------------------------
r3830 | jeffmarshall | 2007-02-21 05:25:39 -0800 (Wed, 21 Feb 2007) | 2 lines

fixed: UL was being closed and reopened after every LI, and monospace text not preserved in a nested DIV section.

------------------------------------------------------------------------
r3829 | jeffmarshall | 2007-02-20 13:05:37 -0800 (Tue, 20 Feb 2007) | 3 lines

updated:
- indentation support

------------------------------------------------------------------------
r3828 | jeffmarshall | 2007-02-20 13:04:14 -0800 (Tue, 20 Feb 2007) | 6 lines

updated:
- only "keyword" links followed on CatPg pages for TOC
- indentation support
- section/subsect support
- minor code fixes

------------------------------------------------------------------------
r3827 | jeffmarshall | 2007-02-20 13:02:08 -0800 (Tue, 20 Feb 2007) | 2 lines

fixed: incorrectly named file

------------------------------------------------------------------------
r3826 | whotookcha0s-_- | 2007-02-18 05:42:47 -0800 (Sun, 18 Feb 2007) | 2 lines

removed the STEP operator

------------------------------------------------------------------------
r3825 | whotookcha0s-_- | 2007-02-18 05:03:34 -0800 (Sun, 18 Feb 2007) | 2 lines

changed - UDT in FOR now takes different args for its operators, see examples/classes/iterator.bas for the new syntax

------------------------------------------------------------------------
r3824 | i_am_drv | 2007-02-16 06:56:24 -0800 (Fri, 16 Feb 2007) | 2 lines

win32 ddraw gfx driver: when user fullscreens window (with maximize button or alt-enter) it will be fullscreened on the monitor that the window is currently on

------------------------------------------------------------------------
r3823 | jeffmarshall | 2007-02-14 12:17:18 -0800 (Wed, 14 Feb 2007) | 2 lines

fixed: bug #1640441

------------------------------------------------------------------------
r3822 | whotookcha0s-_- | 2007-02-14 10:13:56 -0800 (Wed, 14 Feb 2007) | 2 lines

fixed - The NEW operator was accepting 'ANY' as a symbol type.

------------------------------------------------------------------------
r3821 | i_am_drv | 2007-02-13 01:20:42 -0800 (Tue, 13 Feb 2007) | 2 lines

added ws2tcpip.bi, translated from mingw/w32api ws2tcpip.h with SWIG + manual editing

------------------------------------------------------------------------
r3820 | i_am_drv | 2007-02-11 02:24:40 -0800 (Sun, 11 Feb 2007) | 2 lines

updated

------------------------------------------------------------------------
r3819 | i_am_drv | 2007-02-11 02:22:42 -0800 (Sun, 11 Feb 2007) | 2 lines

fixed: DIR on DOS now returns only files with the attributes specified; also now uses thread context for static data storage

------------------------------------------------------------------------
r3818 | whotookcha0s-_- | 2007-02-11 01:56:49 -0800 (Sun, 11 Feb 2007) | 2 lines

bfd.bi was using 'ulong' which wasn't implemented in .17 testing.

------------------------------------------------------------------------
r3817 | whotookcha0s-_- | 2007-02-11 01:53:05 -0800 (Sun, 11 Feb 2007) | 3 lines

chicken-egg problem with string_function( *string_pointer ) which kept the
compiler from being able to be compiled with .17 testing.

------------------------------------------------------------------------
r3816 | i_am_drv | 2007-02-04 04:01:06 -0800 (Sun, 04 Feb 2007) | 3 lines

- fix blitter selection code for VESA drivers
- fix linear VESA driver nearptr calculation

------------------------------------------------------------------------
r3815 | i_am_drv | 2007-02-04 03:16:27 -0800 (Sun, 04 Feb 2007) | 2 lines

added more constants and complete VESA 3.0 mode info block

------------------------------------------------------------------------
r3814 | jeffmarshall | 2007-02-02 11:18:31 -0800 (Fri, 02 Feb 2007) | 2 lines

changed : TOC and CatPg* link processing

------------------------------------------------------------------------
r3813 | whotookcha0s-_- | 2007-01-30 21:33:48 -0800 (Tue, 30 Jan 2007) | 2 lines

GTK changed some of the ABI, updated to reflect that

------------------------------------------------------------------------
r3812 | v1ctor | 2007-01-30 15:11:06 -0800 (Tue, 30 Jan 2007) | 2 lines

added: (MILESTONE) implicit GOTO's to numeric labels are now allowed after ELSE

------------------------------------------------------------------------
r3811 | v1ctor | 2007-01-30 15:02:51 -0800 (Tue, 30 Jan 2007) | 2 lines

added: (MILESTONE) floating-point literals used as numeric labels in -lang qb mode

------------------------------------------------------------------------
r3810 | v1ctor | 2007-01-30 09:21:15 -0800 (Tue, 30 Jan 2007) | 2 lines

new test

------------------------------------------------------------------------
r3809 | v1ctor | 2007-01-30 09:19:20 -0800 (Tue, 30 Jan 2007) | 2 lines

fixed: initialized fields and default params in non-trivial TYPE/UNION's should allow references to self without explicit scope resolution

------------------------------------------------------------------------
r3808 | v1ctor | 2007-01-30 09:10:18 -0800 (Tue, 30 Jan 2007) | 2 lines

reverted: "OPEN ... LEN '=' Expr" was not being parsed correctly

------------------------------------------------------------------------
r3807 | i_am_drv | 2007-01-30 05:09:02 -0800 (Tue, 30 Jan 2007) | 2 lines

updated

------------------------------------------------------------------------
r3806 | i_am_drv | 2007-01-30 05:07:35 -0800 (Tue, 30 Jan 2007) | 2 lines

fixed: kill always returned 0

------------------------------------------------------------------------
r3805 | v1ctor | 2007-01-29 01:50:50 -0800 (Mon, 29 Jan 2007) | 2 lines

new test

------------------------------------------------------------------------
r3804 | v1ctor | 2007-01-29 01:50:25 -0800 (Mon, 29 Jan 2007) | 2 lines

fixed: types of arrays passed by descriptor were not being changed to pointers if those arrays were passed again by descriptor to other functions

------------------------------------------------------------------------
r3803 | whotookcha0s-_- | 2007-01-28 18:52:06 -0800 (Sun, 28 Jan 2007) | 2 lines

enum overlapped with a function

------------------------------------------------------------------------
r3802 | jeffmarshall | 2007-01-28 13:43:23 -0800 (Sun, 28 Jan 2007) | 5 lines

Updated:
- {{color}} action
- static members for Lang and Templates class
- new keywords

------------------------------------------------------------------------
r3801 | jeffmarshall | 2007-01-28 10:47:54 -0800 (Sun, 28 Jan 2007) | 2 lines

remove FIELD #

------------------------------------------------------------------------
r3800 | whotookcha0s-_- | 2007-01-26 01:40:21 -0800 (Fri, 26 Jan 2007) | 3 lines

fixed - OPEN's quirky little tokens clashed with implicit member functions.
regression test added.

------------------------------------------------------------------------
r3799 | jeffmarshall | 2007-01-25 16:42:36 -0800 (Thu, 25 Jan 2007) | 2 lines

field # statement wasn't working with array elements

------------------------------------------------------------------------
r3798 | lillo | 2007-01-25 08:33:47 -0800 (Thu, 25 Jan 2007) | 2 lines

Added forced events polling via ScreenControl POLL_EVENTS flag (thanks 1000101\!)

------------------------------------------------------------------------
r3797 | whotookcha0s-_- | 2007-01-25 03:33:33 -0800 (Thu, 25 Jan 2007) | 3 lines

fixed - private members should not be initializable
updated a test to be able to compile with that change

------------------------------------------------------------------------
r3796 | whotookcha0s-_- | 2007-01-25 03:22:11 -0800 (Thu, 25 Jan 2007) | 2 lines

fixed - variable initializers weren't allowing overloaded casts

------------------------------------------------------------------------
r3795 | whotookcha0s-_- | 2007-01-25 02:52:58 -0800 (Thu, 25 Jan 2007) | 2 lines

fixed the borked test

------------------------------------------------------------------------
r3794 | whotookcha0s-_- | 2007-01-25 02:43:49 -0800 (Thu, 25 Jan 2007) | 2 lines

fixed - MID stmt wasn't taking a deref'd var as a destination

------------------------------------------------------------------------
r3793 | jeffmarshall | 2007-01-24 18:08:29 -0800 (Wed, 24 Jan 2007) | 2 lines

FIELD # statement

------------------------------------------------------------------------
r3792 | jeffmarshall | 2007-01-24 17:58:11 -0800 (Wed, 24 Jan 2007) | 3 lines

fixed: GET/PUT would skip records if a full reclen was read/written
added: support for FIELD # statement

------------------------------------------------------------------------
r3791 | i_am_drv | 2007-01-24 16:43:26 -0800 (Wed, 24 Jan 2007) | 2 lines

NeHe tutorial fixes (by Pritchard)

------------------------------------------------------------------------
r3790 | v1ctor | 2007-01-24 15:24:49 -0800 (Wed, 24 Jan 2007) | 2 lines

small fix

------------------------------------------------------------------------
r3789 | whotookcha0s-_- | 2007-01-24 09:27:46 -0800 (Wed, 24 Jan 2007) | 3 lines

changed - MK* and CV* will now evaluate constant expressions at compile-time
test added

------------------------------------------------------------------------
r3788 | whotookcha0s-_- | 2007-01-24 05:30:08 -0800 (Wed, 24 Jan 2007) | 2 lines

VAR broke the library, fixed

------------------------------------------------------------------------
r3787 | v1ctor | 2007-01-23 11:18:08 -0800 (Tue, 23 Jan 2007) | 2 lines

updated

------------------------------------------------------------------------
r3786 | v1ctor | 2007-01-23 09:30:48 -0800 (Tue, 23 Jan 2007) | 2 lines

updated

------------------------------------------------------------------------
r3785 | v1ctor | 2007-01-23 09:00:00 -0800 (Tue, 23 Jan 2007) | 2 lines

updated

------------------------------------------------------------------------
r3784 | v1ctor | 2007-01-22 11:53:02 -0800 (Mon, 22 Jan 2007) | 2 lines

fixed: bug #1641854

------------------------------------------------------------------------
r3783 | v1ctor | 2007-01-22 08:33:47 -0800 (Mon, 22 Jan 2007) | 2 lines

updated

------------------------------------------------------------------------
r3782 | v1ctor | 2007-01-22 08:32:22 -0800 (Mon, 22 Jan 2007) | 2 lines

new tests

------------------------------------------------------------------------
r3781 | v1ctor | 2007-01-22 08:31:51 -0800 (Mon, 22 Jan 2007) | 4 lines

changed:
- ENUM's will now open a new scope and do a implicit USING to import all symbols, unless it was declared inside an EXTERN .. END EXTERN block;
- (internal) speeded up searching for symbols in imported namespaces, from O(n) to O(1);

------------------------------------------------------------------------
r3779 | whotookcha0s-_- | 2007-01-21 22:05:36 -0800 (Sun, 21 Jan 2007) | 4 lines

fixed - LEN wasn't checking for a WITH symbol, test added
updated changelog.txt to reflect the new DyLibSymbol overload
small cleanup to variable type parsing

------------------------------------------------------------------------
r3778 | i_am_drv | 2007-01-21 13:40:23 -0800 (Sun, 21 Jan 2007) | 2 lines

make sure 'make install' copies into directory instead of overwriting it

------------------------------------------------------------------------
r3777 | jeffmarshall | 2007-01-20 12:54:05 -0800 (Sat, 20 Jan 2007) | 2 lines

Fixed: bug #1640225

------------------------------------------------------------------------
r3776 | jeffmarshall | 2007-01-20 11:35:08 -0800 (Sat, 20 Jan 2007) | 2 lines

new tests

------------------------------------------------------------------------
r3775 | jeffmarshall | 2007-01-20 11:32:49 -0800 (Sat, 20 Jan 2007) | 2 lines

added #pragma once

------------------------------------------------------------------------
r3774 | v1ctor | 2007-01-20 09:48:47 -0800 (Sat, 20 Jan 2007) | 2 lines

new test

------------------------------------------------------------------------
r3773 | v1ctor | 2007-01-20 09:48:34 -0800 (Sat, 20 Jan 2007) | 2 lines

reverted: __FB_BUILD_TIME__ removed or there would be no way to check for differences between fbc and fbc_new when comparing those files

------------------------------------------------------------------------
r3772 | v1ctor | 2007-01-20 09:46:10 -0800 (Sat, 20 Jan 2007) | 2 lines

added: check for duplicated symbols imported from different namespaces

------------------------------------------------------------------------
r3771 | v1ctor | 2007-01-20 09:08:36 -0800 (Sat, 20 Jan 2007) | 2 lines

new tests

------------------------------------------------------------------------
r3770 | v1ctor | 2007-01-20 09:07:47 -0800 (Sat, 20 Jan 2007) | 2 lines

added: check for duplicated symbols imported from different namespaces

------------------------------------------------------------------------
r3769 | whotookcha0s-_- | 2007-01-19 05:22:45 -0800 (Fri, 19 Jan 2007) | 2 lines

- DyLibSymbol is now overloaded; it can take a short as the second arg, to specify the proc's ordinal. Patch by voodooattack

------------------------------------------------------------------------
r3768 | whotookcha0s-_- | 2007-01-18 02:12:25 -0800 (Thu, 18 Jan 2007) | 2 lines

added - the __FB_BUILD_DATE__, __FB_BUILD_TIME__, and __FB_BUILD_SIGNATURE__ intrinsic defines, see wiki  for details.

------------------------------------------------------------------------
r3767 | i_am_drv | 2007-01-17 18:36:24 -0800 (Wed, 17 Jan 2007) | 2 lines

free all lists and hashes used in the frontend; patch by yetifoot

------------------------------------------------------------------------
r3766 | jeffmarshall | 2007-01-17 10:17:46 -0800 (Wed, 17 Jan 2007) | 2 lines

Fixed: Bug #1636747

------------------------------------------------------------------------
r3765 | jeffmarshall | 2007-01-17 10:11:22 -0800 (Wed, 17 Jan 2007) | 2 lines

removed absolute path for find

------------------------------------------------------------------------
r3764 | v1ctor | 2007-01-16 09:14:48 -0800 (Tue, 16 Jan 2007) | 2 lines

new example

------------------------------------------------------------------------
r3763 | v1ctor | 2007-01-16 07:55:26 -0800 (Tue, 16 Jan 2007) | 2 lines

new tests

------------------------------------------------------------------------
r3761 | v1ctor | 2007-01-16 07:55:00 -0800 (Tue, 16 Jan 2007) | 4 lines

fixed: private/protected static member functions were not being checked when accessed

added: support for static member functions in @ and PROCPTR()

------------------------------------------------------------------------
r3760 | v1ctor | 2007-01-16 05:41:30 -0800 (Tue, 16 Jan 2007) | 2 lines

fixed: bug #1629248

------------------------------------------------------------------------
r3759 | v1ctor | 2007-01-16 04:01:19 -0800 (Tue, 16 Jan 2007) | 2 lines

fixed: literal strings were not being allowed in inline asm blocks

------------------------------------------------------------------------
r3758 | v1ctor | 2007-01-16 03:51:29 -0800 (Tue, 16 Jan 2007) | 2 lines

fixed: bug #1634989

------------------------------------------------------------------------
r3757 | v1ctor | 2007-01-16 03:40:12 -0800 (Tue, 16 Jan 2007) | 2 lines

fixed: bug #1635922

------------------------------------------------------------------------
r3756 | whotookcha0s-_- | 2007-01-15 11:51:26 -0800 (Mon, 15 Jan 2007) | 2 lines

small fix

------------------------------------------------------------------------
r3755 | whotookcha0s-_- | 2007-01-15 11:40:02 -0800 (Mon, 15 Jan 2007) | 2 lines

Added build date to version output

------------------------------------------------------------------------
r3754 | whotookcha0s-_- | 2007-01-15 11:38:49 -0800 (Mon, 15 Jan 2007) | 2 lines

added - some operator tests

------------------------------------------------------------------------
r3753 | whotookcha0s-_- | 2007-01-15 11:22:44 -0800 (Mon, 15 Jan 2007) | 3 lines

fixed - operators weren't minding whether or not they were self, if they were comparison ops
fixed - ICE if a self op was overloaded without a parent

------------------------------------------------------------------------
r3752 | jeffmarshall | 2007-01-15 10:11:20 -0800 (Mon, 15 Jan 2007) | 4 lines

Changed:
- gettext => _conio_gettext
- alias for gettext

------------------------------------------------------------------------
r3751 | lillo | 2007-01-15 06:46:10 -0800 (Mon, 15 Jan 2007) | 2 lines

fixed bug in restoring old resolution on exit under X11

------------------------------------------------------------------------
r3750 | lillo | 2007-01-15 06:32:58 -0800 (Mon, 15 Jan 2007) | 3 lines

- fixed build under Linux
- new optimized style bits reversal routine in LINE

------------------------------------------------------------------------
r3749 | v1ctor | 2007-01-15 05:18:09 -0800 (Mon, 15 Jan 2007) | 2 lines

added: (internal) peephole optimizations to the abstract emit module

------------------------------------------------------------------------
r3748 | whotookcha0s-_- | 2007-01-15 04:35:21 -0800 (Mon, 15 Jan 2007) | 2 lines

compiler directives added to ensure backwards compatibility

------------------------------------------------------------------------
r3747 | jeffmarshall | 2007-01-14 09:13:42 -0800 (Sun, 14 Jan 2007) | 2 lines

fixed: end sub was being used instead of end destructor

------------------------------------------------------------------------
r3746 | jeffmarshall | 2007-01-14 04:37:31 -0800 (Sun, 14 Jan 2007) | 2 lines

conditionally compile SET_GL_* states depending on HAVE_GL_GL_H

------------------------------------------------------------------------
r3745 | whotookcha0s-_- | 2007-01-13 18:47:31 -0800 (Sat, 13 Jan 2007) | 2 lines

some functions needed aliases

------------------------------------------------------------------------
r3744 | jeffmarshall | 2007-01-13 17:27:24 -0800 (Sat, 13 Jan 2007) | 2 lines

makefile updated to put all code (except fbdoc.bas) in a library to be used with other tools

------------------------------------------------------------------------
r3743 | v1ctor | 2007-01-13 16:17:16 -0800 (Sat, 13 Jan 2007) | 2 lines

new tests

------------------------------------------------------------------------
r3742 | v1ctor | 2007-01-13 16:17:01 -0800 (Sat, 13 Jan 2007) | 2 lines

changed: (internal) moved var auto off decl-symb-init and simplified its initialization

------------------------------------------------------------------------
r3741 | v1ctor | 2007-01-13 11:16:43 -0800 (Sat, 13 Jan 2007) | 2 lines

updated

------------------------------------------------------------------------
r3740 | v1ctor | 2007-01-13 11:15:31 -0800 (Sat, 13 Jan 2007) | 2 lines

new tests

------------------------------------------------------------------------
r3739 | v1ctor | 2007-01-13 11:15:17 -0800 (Sat, 13 Jan 2007) | 4 lines

fixed: EXIT FOR wasn't calling the destructors of the temporaries created

added: multiple EXIT and CONTINUE levels support

------------------------------------------------------------------------
r3738 | v1ctor | 2007-01-13 06:04:28 -0800 (Sat, 13 Jan 2007) | 2 lines

new test

------------------------------------------------------------------------
r3737 | v1ctor | 2007-01-13 06:04:17 -0800 (Sat, 13 Jan 2007) | 2 lines

fixed: property get/set dup definitions

------------------------------------------------------------------------
r3736 | lillo | 2007-01-13 04:59:21 -0800 (Sat, 13 Jan 2007) | 2 lines

guess who commited this?

------------------------------------------------------------------------
r3735 | whotookcha0s-_- | 2007-01-12 23:11:44 -0800 (Fri, 12 Jan 2007) | 2 lines

fixed an error with VAR's tree symbol searching

------------------------------------------------------------------------
r3734 | lillo | 2007-01-12 07:59:52 -0800 (Fri, 12 Jan 2007) | 2 lines

added new GL pixel format flags accepted by ScreenControl

------------------------------------------------------------------------
r3733 | lillo | 2007-01-12 07:49:24 -0800 (Fri, 12 Jan 2007) | 2 lines

Enabled OpenGL pixel format customization via ScreenControl

------------------------------------------------------------------------
r3732 | v1ctor | 2007-01-12 05:55:33 -0800 (Fri, 12 Jan 2007) | 2 lines

new tests

------------------------------------------------------------------------
r3731 | v1ctor | 2007-01-12 05:55:08 -0800 (Fri, 12 Jan 2007) | 4 lines

fixed:
- premature optimizations in type-casting;
- DATA was not allowing offsets;

------------------------------------------------------------------------
r3729 | jeffmarshall | 2007-01-11 04:05:07 -0800 (Thu, 11 Jan 2007) | 2 lines

AUTO => VAR

------------------------------------------------------------------------
r3728 | lillo | 2007-01-10 09:40:03 -0800 (Wed, 10 Jan 2007) | 4 lines

- fixed bug with MMX single alpha pixel plotter (error popped up with
antialiased circles demo on forums)
- small rework on Linux fullscreen toggling code

------------------------------------------------------------------------
r3727 | v1ctor | 2007-01-10 08:15:55 -0800 (Wed, 10 Jan 2007) | 2 lines

new example

------------------------------------------------------------------------
r3726 | v1ctor | 2007-01-10 08:12:49 -0800 (Wed, 10 Jan 2007) | 2 lines

changed: due the quirks with FOR, and also to allow safer/easier implementations of iterators, now the UDT's must implement the FOR, STEP and NEXT operators

------------------------------------------------------------------------
r3725 | i_am_drv | 2007-01-10 07:22:08 -0800 (Wed, 10 Jan 2007) | 2 lines

removed extra lparens

------------------------------------------------------------------------
r3724 | whotookcha0s-_- | 2007-01-10 06:05:55 -0800 (Wed, 10 Jan 2007) | 2 lines

'var' conflicts

------------------------------------------------------------------------
r3723 | v1ctor | 2007-01-10 04:23:45 -0800 (Wed, 10 Jan 2007) | 2 lines

changed: STRPTR, VARPTR, LEN and SIZEOF can now be redeclared as methods; LIB was supported in QB too

------------------------------------------------------------------------
r3722 | v1ctor | 2007-01-10 03:28:55 -0800 (Wed, 10 Jan 2007) | 2 lines

fixed: the pre-defined parameters were not being counted

------------------------------------------------------------------------
r3721 | whotookcha0s-_- | 2007-01-10 03:00:28 -0800 (Wed, 10 Jan 2007) | 2 lines

fixed - properties should always call hOvlProcArgList()

------------------------------------------------------------------------
r3720 | whotookcha0s-_- | 2007-01-09 18:52:14 -0800 (Tue, 09 Jan 2007) | 2 lines

er...

------------------------------------------------------------------------
r3719 | whotookcha0s-_- | 2007-01-09 18:50:46 -0800 (Tue, 09 Jan 2007) | 2 lines

table beautification

------------------------------------------------------------------------
r3718 | i_am_drv | 2007-01-09 01:51:50 -0800 (Tue, 09 Jan 2007) | 2 lines

revert previous commit (will not work on FreeBSD, the only place it would have been necessary)

------------------------------------------------------------------------
r3717 | i_am_drv | 2007-01-09 01:18:18 -0800 (Tue, 09 Jan 2007) | 2 lines

add start_dir member to __fb_ctx (needed for upcoming exepath fix for linux fbc running under FreeBSD Linux binary emulation)

------------------------------------------------------------------------
r3716 | whotookcha0s-_- | 2007-01-08 18:43:37 -0800 (Mon, 08 Jan 2007) | 4 lines

changed - AUTO was changed to VAR
fixed - 'VAR foo = foo' was disallowed
Some small commenting on the FOR parser...

------------------------------------------------------------------------
r3715 | whotookcha0s-_- | 2007-01-08 09:41:36 -0800 (Mon, 08 Jan 2007) | 2 lines

Tidy + comments

------------------------------------------------------------------------
r3714 | whotookcha0s-_- | 2007-01-08 00:29:09 -0800 (Mon, 08 Jan 2007) | 2 lines

fixed FB_INIOPT_AUTO bitflag value

------------------------------------------------------------------------
r3713 | whotookcha0s-_- | 2007-01-07 19:56:59 -0800 (Sun, 07 Jan 2007) | 2 lines

added - AUTO SHARED

------------------------------------------------------------------------
r3712 | whotookcha0s-_- | 2007-01-07 06:10:37 -0800 (Sun, 07 Jan 2007) | 3 lines

Fixed ICE when step positivity check failed.
Allowed UDTs to be used as STEP variables.

------------------------------------------------------------------------
r3711 | whotookcha0s-_- | 2007-01-06 21:55:22 -0800 (Sat, 06 Jan 2007) | 2 lines

AUTO should have been parsing the expression earlier. Modularized the backpatching process.

------------------------------------------------------------------------
r3710 | whotookcha0s-_- | 2007-01-06 14:41:52 -0800 (Sat, 06 Jan 2007) | 2 lines

added - the AUTO keyword, which allows type inferrence from initializers (think DIM without AS) ex: auto foo = "hello", bar = 2.4

------------------------------------------------------------------------
r3709 | whotookcha0s-_- | 2007-01-06 14:30:46 -0800 (Sat, 06 Jan 2007) | 2 lines

allowed internal disabling of keywords, for -lang modes

------------------------------------------------------------------------
r3708 | lillo | 2007-01-06 05:22:31 -0800 (Sat, 06 Jan 2007) | 2 lines

more correct behaviour when reporting window got/lost focus and mouse enter/exit events under win32

------------------------------------------------------------------------
r3707 | lillo | 2007-01-05 14:40:40 -0800 (Fri, 05 Jan 2007) | 2 lines

fixed bug #1624633

------------------------------------------------------------------------
r3706 | lillo | 2007-01-05 07:57:55 -0800 (Fri, 05 Jan 2007) | 2 lines

tentative fix for bug #1619683

------------------------------------------------------------------------
r3705 | v1ctor | 2007-01-04 10:24:45 -0800 (Thu, 04 Jan 2007) | 2 lines

small fix

------------------------------------------------------------------------
r3704 | v1ctor | 2007-01-04 08:16:45 -0800 (Thu, 04 Jan 2007) | 2 lines

small fix

------------------------------------------------------------------------
r3703 | v1ctor | 2007-01-04 08:10:10 -0800 (Thu, 04 Jan 2007) | 2 lines

fixed: FOR was not converting the expressions to the counter's type when an UDT was passed

------------------------------------------------------------------------
r3702 | v1ctor | 2007-01-04 02:27:02 -0800 (Thu, 04 Jan 2007) | 2 lines

fixed: FOR was not converting the expressions to the counter's type when an UDT was passed

------------------------------------------------------------------------
r3701 | whotookcha0s-_- | 2007-01-04 02:20:34 -0800 (Thu, 04 Jan 2007) | 2 lines

listAllocTB() would fail, if 'nodes' was 1. A future access of 'list->fhead' would then crash the compiler.

------------------------------------------------------------------------
r3700 | i_am_drv | 2007-01-03 22:10:31 -0800 (Wed, 03 Jan 2007) | 2 lines

add extended family support to x86 cpu detection

------------------------------------------------------------------------
r3699 | v1ctor | 2007-01-03 17:55:09 -0800 (Wed, 03 Jan 2007) | 2 lines

fixed: FOR was not converting the expressions to the counter's type when an UDT was passed

------------------------------------------------------------------------
r3698 | jeffmarshall | 2007-01-03 11:59:20 -0800 (Wed, 03 Jan 2007) | 2 lines

added: new page section titles

------------------------------------------------------------------------
r3697 | jeffmarshall | 2007-01-03 08:22:56 -0800 (Wed, 03 Jan 2007) | 4 lines

changed:
- __BFD_216__ removed,  binutils-2.17 required on all ports.
- Only SEC_HAS_CONTENTS passed to bfd_make_section()

------------------------------------------------------------------------
r3696 | jeffmarshall | 2007-01-03 07:03:34 -0800 (Wed, 03 Jan 2007) | 2 lines

fixed: double => single

------------------------------------------------------------------------
r3695 | jeffmarshall | 2007-01-03 05:58:53 -0800 (Wed, 03 Jan 2007) | 2 lines

small change on COLOR statment parser

------------------------------------------------------------------------
r3694 | v1ctor | 2007-01-03 05:33:08 -0800 (Wed, 03 Jan 2007) | 2 lines

fixed: bug #1625151 (patch sent/fixed by counting_pine)

------------------------------------------------------------------------
r3693 | whotookcha0s-_- | 2007-01-02 16:28:05 -0800 (Tue, 02 Jan 2007) | 2 lines

dup def when doing Berkely sockets n whatnot

------------------------------------------------------------------------
r3692 | i_am_drv | 2007-01-01 20:51:17 -0800 (Mon, 01 Jan 2007) | 2 lines

split banked VESA gfx driver from routines shared by both banked and linear drivers

------------------------------------------------------------------------
r3691 | v1ctor | 2007-01-01 15:59:13 -0800 (Mon, 01 Jan 2007) | 7 lines

fixed:
- (Linux) LD still needing the SEARCH() script directives to find
libraries;
- (Linux) BFD doesn't use intl;
- (Linux) a bug in BFD 2.17 forced writing objects directly to be
disabled;

------------------------------------------------------------------------
r3690 | v1ctor | 2007-01-01 05:16:49 -0800 (Mon, 01 Jan 2007) | 2 lines

added: operator overloading support for NEW, NEW[], DELETE and DELETE[]

------------------------------------------------------------------------
r3689 | v1ctor | 2006-12-31 14:12:39 -0800 (Sun, 31 Dec 2006) | 2 lines

fixed: destructors of temporary instances are now called right-after the instances are used

------------------------------------------------------------------------
r3688 | v1ctor | 2006-12-31 08:50:54 -0800 (Sun, 31 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3687 | v1ctor | 2006-12-31 08:20:09 -0800 (Sun, 31 Dec 2006) | 2 lines

fixed: destructors of temporary instances are now called right-after the instances are used

------------------------------------------------------------------------
r3686 | v1ctor | 2006-12-31 08:19:43 -0800 (Sun, 31 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3685 | v1ctor | 2006-12-31 08:14:23 -0800 (Sun, 31 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3684 | jeffmarshall | 2006-12-31 08:11:51 -0800 (Sun, 31 Dec 2006) | 4 lines

fixed:
- dos file offsets are 32 bit
- direction member missing

------------------------------------------------------------------------
r3683 | i_am_drv | 2006-12-31 08:00:24 -0800 (Sun, 31 Dec 2006) | 2 lines

remove spurious *

------------------------------------------------------------------------
r3682 | v1ctor | 2006-12-30 08:58:31 -0800 (Sat, 30 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3679 | v1ctor | 2006-12-30 08:56:57 -0800 (Sat, 30 Dec 2006) | 2 lines

fixed: RESTORE w/o label was causing an ICE if no DATA's were parsed

------------------------------------------------------------------------
r3678 | v1ctor | 2006-12-30 08:35:31 -0800 (Sat, 30 Dec 2006) | 2 lines

fixed: the property's get accessor couldn't be used outside expressions

------------------------------------------------------------------------
r3677 | whotookcha0s-_- | 2006-12-29 11:52:25 -0800 (Fri, 29 Dec 2006) | 2 lines

tinyptc needs configuration info from fb's rtlib. also added variable NASM, to customize the location of NASM.

------------------------------------------------------------------------
r3676 | v1ctor | 2006-12-29 04:08:47 -0800 (Fri, 29 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3675 | v1ctor | 2006-12-29 04:07:22 -0800 (Fri, 29 Dec 2006) | 2 lines

added: auto-scan for objects and libraries when linking, plus checks for mixed use of -lang and -mt

------------------------------------------------------------------------
r3674 | v1ctor | 2006-12-29 04:05:44 -0800 (Fri, 29 Dec 2006) | 2 lines

added: auto-scan for objects and libraries when linking, plus checks for mixed use of -lang qb and -mt

------------------------------------------------------------------------
r3673 | v1ctor | 2006-12-28 18:46:06 -0800 (Thu, 28 Dec 2006) | 2 lines

added: backward-compatibility with version 2.16.1 if __BFD_216__ is defined

------------------------------------------------------------------------
r3672 | i_am_drv | 2006-12-27 19:04:37 -0800 (Wed, 27 Dec 2006) | 2 lines

fixed build when GL/gl.h is missing

------------------------------------------------------------------------
r3671 | jeffmarshall | 2006-12-27 11:05:58 -0800 (Wed, 27 Dec 2006) | 4 lines

updated:
- added template macros for inserting build date, time, and url
- using templates for DocToc on all outputs

------------------------------------------------------------------------
r3670 | jeffmarshall | 2006-12-27 11:00:14 -0800 (Wed, 27 Dec 2006) | 2 lines

updated: added template macros for inserting build date, time, and url

------------------------------------------------------------------------
r3669 | jeffmarshall | 2006-12-27 10:59:25 -0800 (Wed, 27 Dec 2006) | 2 lines

update: new keywords

------------------------------------------------------------------------
r3668 | jeffmarshall | 2006-12-27 10:57:59 -0800 (Wed, 27 Dec 2006) | 2 lines

added: special templates for DocToc to automatically insert build date/time/url

------------------------------------------------------------------------
r3667 | jeffmarshall | 2006-12-27 10:25:21 -0800 (Wed, 27 Dec 2006) | 2 lines

fixed: quirk statement COLOR was not allowing parameters in ()'s

------------------------------------------------------------------------
r3666 | lillo | 2006-12-26 12:24:56 -0800 (Tue, 26 Dec 2006) | 2 lines

forgot to commit these yesterday...

------------------------------------------------------------------------
r3665 | v1ctor | 2006-12-25 16:29:31 -0800 (Mon, 25 Dec 2006) | 2 lines

added: backward-compatibility with version 2.16.1 if __BFD_216__ is defined

------------------------------------------------------------------------
r3664 | v1ctor | 2006-12-25 09:50:39 -0800 (Mon, 25 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3663 | v1ctor | 2006-12-24 18:59:26 -0800 (Sun, 24 Dec 2006) | 2 lines

added

------------------------------------------------------------------------
r3662 | lillo | 2006-12-24 07:44:10 -0800 (Sun, 24 Dec 2006) | 3 lines

- Added GET_GL_EXTENSIONS support to ScreenControl to retrieve a string holding supported GL extensions (ex: dim exts as string: ScreenControl GET_GL_EXTENSIONS, exts: print exts)
- Added the new ScreenGLProc function to retrive the address of a GL procedure, to be used for extensions support (ex: SwapInterval = ScreenGLProc("wglSwapIntervalEXT"))

------------------------------------------------------------------------
r3661 | lillo | 2006-12-24 03:52:07 -0800 (Sun, 24 Dec 2006) | 3 lines

- FB apps are now using default thread priority also when using the DirectDraw gfx driver
- Hopefully fixed all DirectDraw surface restoring problems when toggling fullscreen

------------------------------------------------------------------------
r3660 | whotookcha0s-_- | 2006-12-23 23:29:04 -0800 (Sat, 23 Dec 2006) | 2 lines

fixed - PRN: was being shorted out

------------------------------------------------------------------------
r3658 | v1ctor | 2006-12-23 18:27:16 -0800 (Sat, 23 Dec 2006) | 2 lines

fixed: NEW[] when setting the buffer allocated to 0

------------------------------------------------------------------------
r3657 | v1ctor | 2006-12-23 18:17:13 -0800 (Sat, 23 Dec 2006) | 2 lines

fixed: OPEN when used as a function in -lang qb mode

------------------------------------------------------------------------
r3656 | whotookcha0s-_- | 2006-12-23 11:33:40 -0800 (Sat, 23 Dec 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r3655 | whotookcha0s-_- | 2006-12-23 11:31:21 -0800 (Sat, 23 Dec 2006) | 2 lines

changed - OPEN now is more compatible with QB, the file/device is parsed at runtime

------------------------------------------------------------------------
r3654 | lillo | 2006-12-23 09:23:23 -0800 (Sat, 23 Dec 2006) | 2 lines

Fixed GetMouse behaviour under Win32 to report -1 if mouse not on window; when SetMouse is used to hide cursor, cursor is hidden only in the window client area and not on the border

------------------------------------------------------------------------
r3653 | v1ctor | 2006-12-23 08:04:26 -0800 (Sat, 23 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3652 | v1ctor | 2006-12-23 07:56:26 -0800 (Sat, 23 Dec 2006) | 4 lines

changed: ACOS(), ASIN(), LOG(), EXP(), SGN() and FIX() are now inlined

added: FRAC()

------------------------------------------------------------------------
r3651 | lillo | 2006-12-23 07:43:15 -0800 (Sat, 23 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3650 | lillo | 2006-12-23 07:29:44 -0800 (Sat, 23 Dec 2006) | 2 lines

fixed bug when restoring directdraw surfaces

------------------------------------------------------------------------
r3649 | lillo | 2006-12-23 06:18:51 -0800 (Sat, 23 Dec 2006) | 2 lines

Fixed bug #1617686

------------------------------------------------------------------------
r3648 | whotookcha0s-_- | 2006-12-23 05:36:23 -0800 (Sat, 23 Dec 2006) | 2 lines

'sockaddr' conflicts resolved...

------------------------------------------------------------------------
r3647 | lillo | 2006-12-22 17:25:58 -0800 (Fri, 22 Dec 2006) | 2 lines

Fixed -Wall warnings

------------------------------------------------------------------------
r3646 | v1ctor | 2006-12-22 15:11:09 -0800 (Fri, 22 Dec 2006) | 2 lines

changed: better error message for invalid options

------------------------------------------------------------------------
r3645 | v1ctor | 2006-12-22 15:02:27 -0800 (Fri, 22 Dec 2006) | 2 lines

fixed: ICE when processing invalid cmd-line arguments

------------------------------------------------------------------------
r3644 | lillo | 2006-12-22 10:54:32 -0800 (Fri, 22 Dec 2006) | 2 lines

always move container window

------------------------------------------------------------------------
r3643 | lillo | 2006-12-22 10:28:54 -0800 (Fri, 22 Dec 2006) | 5 lines

- Moving windows via ScreenControl under X11 now allows to move window
  past desktop edges
- Greatly reduced flickering with GFX_SHAPED_WINDOW under X11 (not
  completely removed though)

------------------------------------------------------------------------
r3642 | whotookcha0s-_- | 2006-12-22 10:00:46 -0800 (Fri, 22 Dec 2006) | 2 lines

reverted - SLEEP should remain as milliseconds

------------------------------------------------------------------------
r3641 | v1ctor | 2006-12-22 09:45:17 -0800 (Fri, 22 Dec 2006) | 2 lines

fixed: MS stdcall mixed with GCC exported symbols

------------------------------------------------------------------------
r3640 | lillo | 2006-12-22 09:39:41 -0800 (Fri, 22 Dec 2006) | 2 lines

BLOAD fixes

------------------------------------------------------------------------
r3639 | lillo | 2006-12-22 09:30:40 -0800 (Fri, 22 Dec 2006) | 3 lines

- Added alpha mask PUTter: using ALPHA drawing mode when PUTing an 8bpp
sprite on top of a 32bpp one sets the target alpha channel

------------------------------------------------------------------------
r3638 | lillo | 2006-12-22 09:16:58 -0800 (Fri, 22 Dec 2006) | 4 lines

- Refactored PUT code
- Fixed various issues related to drawing to an image buffer with a
  color depth different than the current one

------------------------------------------------------------------------
r3637 | whotookcha0s-_- | 2006-12-22 08:51:51 -0800 (Fri, 22 Dec 2006) | 2 lines

fixed - str( constant ) wasn't respecting the QB changes

------------------------------------------------------------------------
r3636 | whotookcha0s-_- | 2006-12-22 08:10:08 -0800 (Fri, 22 Dec 2006) | 2 lines

changed - SLEEP now sleeps in seconds, if -lang qb is used

------------------------------------------------------------------------
r3635 | whotookcha0s-_- | 2006-12-22 07:33:29 -0800 (Fri, 22 Dec 2006) | 3 lines

changed - STR will now pad positive numbers with a single space, as in QB, but ONLY if -lang qb is used.
Regression tests added.

------------------------------------------------------------------------
r3634 | whotookcha0s-_- | 2006-12-22 06:35:57 -0800 (Fri, 22 Dec 2006) | 2 lines

changed - LOCATE will now accept a full 5 args to be compatible with QB, however the final 2 args have no effect.

------------------------------------------------------------------------
r3633 | v1ctor | 2006-12-22 06:30:13 -0800 (Fri, 22 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3632 | v1ctor | 2006-12-22 06:22:13 -0800 (Fri, 22 Dec 2006) | 2 lines

new tests

------------------------------------------------------------------------
r3631 | v1ctor | 2006-12-22 06:21:25 -0800 (Fri, 22 Dec 2006) | 4 lines

added: static member functions

changed: when overloading op=, the '=' is now obligatory

------------------------------------------------------------------------
r3630 | v1ctor | 2006-12-21 10:39:44 -0800 (Thu, 21 Dec 2006) | 2 lines

changed: #PTR and #HND defines renamed to #PTR_ and #HND_

------------------------------------------------------------------------
r3628 | v1ctor | 2006-12-20 02:48:51 -0800 (Wed, 20 Dec 2006) | 2 lines

changed: the DATA statement is now compatible with QB in -lang qb mode

------------------------------------------------------------------------
r3627 | v1ctor | 2006-12-19 18:37:11 -0800 (Tue, 19 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3626 | v1ctor | 2006-12-19 18:28:59 -0800 (Tue, 19 Dec 2006) | 2 lines

changed: the DATA statement is now compatible with QB in -lang qb mode

------------------------------------------------------------------------
r3625 | i_am_drv | 2006-12-19 18:09:59 -0800 (Tue, 19 Dec 2006) | 2 lines

always set a text mode when WIDTH is called (DOS)

------------------------------------------------------------------------
r3624 | i_am_drv | 2006-12-19 15:01:11 -0800 (Tue, 19 Dec 2006) | 2 lines

fixed: blink was not being disabled in WIDTH on DOS

------------------------------------------------------------------------
r3623 | jeffmarshall | 2006-12-19 08:43:57 -0800 (Tue, 19 Dec 2006) | 2 lines

EXEC in dos and linux now parses arguments like in windows and with no hardcoded limits on the allowable number of arguments

------------------------------------------------------------------------
r3622 | v1ctor | 2006-12-18 16:18:00 -0800 (Mon, 18 Dec 2006) | 2 lines

new tests

------------------------------------------------------------------------
r3621 | v1ctor | 2006-12-18 16:17:26 -0800 (Mon, 18 Dec 2006) | 8 lines

fixed:
- NEW and NEW[] operators were not setting the contents of the buffers allocated to 0's;
- the padding of initialized arrays of fixed-len strings was wrong;


added:
- NEW(ANY) and NEW[] {ANY} to explicitly turn off setting the contents to 0's;

------------------------------------------------------------------------
r3620 | jeffmarshall | 2006-12-17 20:32:02 -0800 (Sun, 17 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3619 | jeffmarshall | 2006-12-17 16:13:36 -0800 (Sun, 17 Dec 2006) | 3 lines

fixed:
- constructors were not setting default values

------------------------------------------------------------------------
r3618 | v1ctor | 2006-12-17 14:48:14 -0800 (Sun, 17 Dec 2006) | 2 lines

reverted: the change broke RESUME

------------------------------------------------------------------------
r3617 | jeffmarshall | 2006-12-17 08:47:05 -0800 (Sun, 17 Dec 2006) | 3 lines

- updated copyright notice
- replaced all non-static pseudo classes

------------------------------------------------------------------------
r3616 | v1ctor | 2006-12-17 08:41:37 -0800 (Sun, 17 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3615 | v1ctor | 2006-12-17 08:40:33 -0800 (Sun, 17 Dec 2006) | 2 lines

changed: no more limits in the number of arguments and files that can be passed in the command-line

------------------------------------------------------------------------
r3614 | v1ctor | 2006-12-16 13:10:26 -0800 (Sat, 16 Dec 2006) | 3 lines

fixed: now that constants are emitted to .rodata in Linux, the unsafe
byval string param change won't work, breaking -g

------------------------------------------------------------------------
r3613 | v1ctor | 2006-12-15 07:13:17 -0800 (Fri, 15 Dec 2006) | 2 lines

changed: (internal) removed most x86 assumptions from the AST

------------------------------------------------------------------------
r3612 | v1ctor | 2006-12-14 19:55:00 -0800 (Thu, 14 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3611 | v1ctor | 2006-12-14 19:21:56 -0800 (Thu, 14 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3610 | v1ctor | 2006-12-14 19:21:10 -0800 (Thu, 14 Dec 2006) | 2 lines

new tests

------------------------------------------------------------------------
r3609 | v1ctor | 2006-12-14 19:20:42 -0800 (Thu, 14 Dec 2006) | 5 lines

changed:
- (internal) the IR and EMIT modules are now selected at compile-time;
- (internal) DATA statements are now convert to static initialized arrays;
- DATA can't be declared inside procs anymore (as in QB);

------------------------------------------------------------------------
r3608 | i_am_drv | 2006-12-14 00:17:49 -0800 (Thu, 14 Dec 2006) | 2 lines

added: fbc man page (by Ebben Feagan)

------------------------------------------------------------------------
r3607 | lillo | 2006-12-13 03:49:36 -0800 (Wed, 13 Dec 2006) | 2 lines

Added GET/SET_ALPHA_PRIMITIVES constants to ScreenControl

------------------------------------------------------------------------
r3606 | whotookcha0s-_- | 2006-12-13 02:59:12 -0800 (Wed, 13 Dec 2006) | 2 lines

added an updated/fixed sieve test (submitted by Michael Webster)

------------------------------------------------------------------------
r3605 | whotookcha0s-_- | 2006-12-13 02:37:34 -0800 (Wed, 13 Dec 2006) | 2 lines

imagecreate() was crashing if !__fb_gfx

------------------------------------------------------------------------
r3604 | whotookcha0s-_- | 2006-12-13 01:53:34 -0800 (Wed, 13 Dec 2006) | 2 lines

qb alignment test

------------------------------------------------------------------------
r3602 | whotookcha0s-_- | 2006-12-13 01:50:00 -0800 (Wed, 13 Dec 2006) | 3 lines

changed - -lang qb now skips UDT alignment by default (cha0s)
fixed some tests that used gfxlib

------------------------------------------------------------------------
r3601 | v1ctor | 2006-12-12 15:25:46 -0800 (Tue, 12 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3600 | v1ctor | 2006-12-12 15:25:16 -0800 (Tue, 12 Dec 2006) | 2 lines

fixed: D3DXCreateFont() shouldn't include the A or W suffix

------------------------------------------------------------------------
r3599 | lillo | 2006-12-12 13:55:11 -0800 (Tue, 12 Dec 2006) | 2 lines

use .rodata section under linux for const data. Other platforms still use .data as before, need testing before changing them too

------------------------------------------------------------------------
r3598 | v1ctor | 2006-12-12 13:03:53 -0800 (Tue, 12 Dec 2006) | 2 lines

updated: cdecl alias "..." to extern "c" ... end extern

------------------------------------------------------------------------
r3597 | v1ctor | 2006-12-11 19:23:05 -0800 (Mon, 11 Dec 2006) | 2 lines

fixed: IMPORT used with EXTERN's

------------------------------------------------------------------------
r3596 | v1ctor | 2006-12-11 19:21:10 -0800 (Mon, 11 Dec 2006) | 2 lines

added: libcairo headers and example

------------------------------------------------------------------------
r3595 | lillo | 2006-12-11 13:45:51 -0800 (Mon, 11 Dec 2006) | 2 lines

Give error if label of an ON ERROR GOTO statement crosses function scopes

------------------------------------------------------------------------
r3594 | v1ctor | 2006-12-11 09:28:08 -0800 (Mon, 11 Dec 2006) | 2 lines

added: @static_array(const_expr) can now be used to initialize static symbols

------------------------------------------------------------------------
r3593 | lillo | 2006-12-10 13:42:18 -0800 (Sun, 10 Dec 2006) | 2 lines

ops, forgot this

------------------------------------------------------------------------
r3592 | lillo | 2006-12-10 13:41:56 -0800 (Sun, 10 Dec 2006) | 2 lines

Second effect of having lang in rtlib context: RND uses a different algorithm depending on the current lang (Marsenne Twister in -lang fb, CRT rand() in -lang deprecated, yetifoot's QB implementation in -lang QB. RANDOMIZE now accepts an optional second parameter to force RND to use a specific algorithm

------------------------------------------------------------------------
r3591 | lillo | 2006-12-10 12:38:22 -0800 (Sun, 10 Dec 2006) | 2 lines

First effect of having lang in rtlib context: gfxlib knows when to use old-style PUT headers without dedicated hidden SCREEN flag

------------------------------------------------------------------------
r3590 | lillo | 2006-12-10 12:29:33 -0800 (Sun, 10 Dec 2006) | 2 lines

Rtlib context now stores the language with which a program was compiled

------------------------------------------------------------------------
r3589 | lillo | 2006-12-10 10:44:43 -0800 (Sun, 10 Dec 2006) | 2 lines

fixed bug #1610260

------------------------------------------------------------------------
r3588 | v1ctor | 2006-12-09 19:44:35 -0800 (Sat, 09 Dec 2006) | 2 lines

added: operator overloading support for @, * and ->

------------------------------------------------------------------------
r3587 | v1ctor | 2006-12-09 10:42:34 -0800 (Sat, 09 Dec 2006) | 2 lines

new tests

------------------------------------------------------------------------
r3586 | v1ctor | 2006-12-09 10:41:43 -0800 (Sat, 09 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3585 | v1ctor | 2006-12-09 10:40:06 -0800 (Sat, 09 Dec 2006) | 7 lines

changed:
- (internal) pointer deref is now handled by a single node class;
- (internal) IR's PushArg() was simplified;

fixed:
- bug #1611274

------------------------------------------------------------------------
r3584 | whotookcha0s-_- | 2006-12-09 09:25:06 -0800 (Sat, 09 Dec 2006) | 2 lines

fixed bug #1611274

------------------------------------------------------------------------
r3583 | jeffmarshall | 2006-12-08 21:18:33 -0800 (Fri, 08 Dec 2006) | 2 lines

size_t is already defined in stddef.bi

------------------------------------------------------------------------
r3582 | jeffmarshall | 2006-12-08 21:16:51 -0800 (Fri, 08 Dec 2006) | 2 lines

reverted: DrV's daily build libs are not using _conio_gettext

------------------------------------------------------------------------
r3581 | jeffmarshall | 2006-12-08 21:14:50 -0800 (Fri, 08 Dec 2006) | 3 lines

Added support for TXT file output
Fixed some major formatting errors

------------------------------------------------------------------------
r3579 | v1ctor | 2006-12-08 07:47:40 -0800 (Fri, 08 Dec 2006) | 2 lines

fixed: bug #1611290

------------------------------------------------------------------------
r3578 | v1ctor | 2006-12-08 07:26:19 -0800 (Fri, 08 Dec 2006) | 2 lines

fixed: #0 was being allowed as the file slot

------------------------------------------------------------------------
r3577 | i_am_drv | 2006-12-06 02:12:46 -0800 (Wed, 06 Dec 2006) | 2 lines

remove <linux/kd.h> - unused

------------------------------------------------------------------------
r3576 | i_am_drv | 2006-12-06 00:14:39 -0800 (Wed, 06 Dec 2006) | 2 lines

first steps to building on FreeBSD

------------------------------------------------------------------------
r3575 | i_am_drv | 2006-12-05 09:52:24 -0800 (Tue, 05 Dec 2006) | 2 lines

only include OpenGL driver if GL/gl.h is present

------------------------------------------------------------------------
r3574 | v1ctor | 2006-12-05 06:39:46 -0800 (Tue, 05 Dec 2006) | 2 lines

fixed: bug #1608095

------------------------------------------------------------------------
r3573 | lillo | 2006-12-05 02:03:58 -0800 (Tue, 05 Dec 2006) | 2 lines

small fix

------------------------------------------------------------------------
r3572 | lillo | 2006-12-04 08:41:56 -0800 (Mon, 04 Dec 2006) | 2 lines

small fixes to x11 driver

------------------------------------------------------------------------
r3571 | lillo | 2006-12-04 07:20:37 -0800 (Mon, 04 Dec 2006) | 2 lines

no more special color value to signal default color in COLOR and gfx primitives, resulting in no more 'hole' in supported colors range

------------------------------------------------------------------------
r3570 | whotookcha0s-_- | 2006-12-03 22:14:47 -0800 (Sun, 03 Dec 2006) | 2 lines

some sc_* constants were incorrect

------------------------------------------------------------------------
r3569 | lillo | 2006-12-02 18:46:04 -0800 (Sat, 02 Dec 2006) | 7 lines

Major rework of default color passing system: what was "coord_type" is
now a generic flags integer, and if a primitive uses (fore or back)
default colors specific bits are set there to inform gfxlib. No more
special color value to signal default color, meaning there won't be a
"hole" in the accepted colors range.
Still to update the COLOR hook to reflect the change...

------------------------------------------------------------------------
r3568 | lillo | 2006-12-02 16:57:49 -0800 (Sat, 02 Dec 2006) | 6 lines

- #define -> const as integer
- renamed PUT_HEADER type to IMAGE, preserved PUT_HEADER as typedef for
  compatibility
- added pixels() property to IMAGE type to return the address of raw
  pixels data

------------------------------------------------------------------------
r3567 | v1ctor | 2006-12-02 15:18:51 -0800 (Sat, 02 Dec 2006) | 2 lines

updated

------------------------------------------------------------------------
r3566 | v1ctor | 2006-12-02 15:10:30 -0800 (Sat, 02 Dec 2006) | 2 lines

fixed: "rem as" fields

------------------------------------------------------------------------
r3565 | whotookcha0s-_- | 2006-12-02 14:11:31 -0800 (Sat, 02 Dec 2006) | 2 lines

tests updated

------------------------------------------------------------------------
r3564 | v1ctor | 2006-12-02 08:48:34 -0800 (Sat, 02 Dec 2006) | 2 lines

fixed: ICE when profiler calls were added to constructors

------------------------------------------------------------------------
r3563 | jeffmarshall | 2006-12-02 06:57:52 -0800 (Sat, 02 Dec 2006) | 2 lines

changed: *old* profiler code removed.

------------------------------------------------------------------------
r3562 | jeffmarshall | 2006-12-01 22:32:43 -0800 (Fri, 01 Dec 2006) | 2 lines

added: profiling support for gprof

------------------------------------------------------------------------
r3561 | whotookcha0s-_- | 2006-12-01 22:32:32 -0800 (Fri, 01 Dec 2006) | 2 lines

added hMatchText( byval as zstring ptr ) to match raw text in case a token check would fail (in quirk gfx functions when methods of the same name exist), to fix 'palette get',  regression tests added

------------------------------------------------------------------------
r3560 | jeffmarshall | 2006-12-01 22:25:16 -0800 (Fri, 01 Dec 2006) | 2 lines

added: profiling support for gprof

------------------------------------------------------------------------
r3559 | jeffmarshall | 2006-12-01 22:21:38 -0800 (Fri, 01 Dec 2006) | 2 lines

added: profiling support for gprof

------------------------------------------------------------------------
r3558 | jeffmarshall | 2006-12-01 21:53:34 -0800 (Fri, 01 Dec 2006) | 2 lines

added: profiling support for gprof

------------------------------------------------------------------------
r3557 | jeffmarshall | 2006-12-01 21:44:08 -0800 (Fri, 01 Dec 2006) | 2 lines

added: profiling support for gprof

------------------------------------------------------------------------
r3556 | whotookcha0s-_- | 2006-12-01 21:13:36 -0800 (Fri, 01 Dec 2006) | 2 lines

put mode would fail on pset and preset, if used within a method in a type that had methods named pset or preset, respectively. (thanks stylin ;)

------------------------------------------------------------------------
r3555 | v1ctor | 2006-12-01 19:27:35 -0800 (Fri, 01 Dec 2006) | 2 lines

added: JNI header and example

------------------------------------------------------------------------
r3554 | jeffmarshall | 2006-12-01 19:20:44 -0800 (Fri, 01 Dec 2006) | 2 lines

added: new rtlib startup code (based on libfb_ctor.c)

------------------------------------------------------------------------
r3553 | v1ctor | 2006-12-01 19:18:34 -0800 (Fri, 01 Dec 2006) | 2 lines

added: the "-Wa <options>" and "-Wl <options>" command-line options, to pass options to GAS and LD

------------------------------------------------------------------------
r3552 | jeffmarshall | 2006-12-01 09:02:51 -0800 (Fri, 01 Dec 2006) | 2 lines

changed: libgcc_eh.a is a now a default lib on linux

------------------------------------------------------------------------
r3551 | v1ctor | 2006-12-01 08:17:40 -0800 (Fri, 01 Dec 2006) | 2 lines

updated: the FB namespace was missing

------------------------------------------------------------------------
r3550 | v1ctor | 2006-12-01 07:52:55 -0800 (Fri, 01 Dec 2006) | 2 lines

changed: using real classes now

------------------------------------------------------------------------
r3549 | v1ctor | 2006-12-01 07:49:39 -0800 (Fri, 01 Dec 2006) | 6 lines

changed: the NEW and DELETE vector operators syntax is now "= new foo[expr]" and "delete[] foo"

fixed: DELETE wasn't checking for null pointers

added: builtin types and UDT's without constructors can now be initialized using NEW too

------------------------------------------------------------------------
r3548 | lillo | 2006-12-01 02:35:50 -0800 (Fri, 01 Dec 2006) | 2 lines

removed other x11 program windows size/pos restoring functions, they never really solved the issue they were intended for

------------------------------------------------------------------------
r3547 | lillo | 2006-12-01 02:33:12 -0800 (Fri, 01 Dec 2006) | 2 lines

reset context drawing hookson custom target buffer depth

------------------------------------------------------------------------
r3546 | lillo | 2006-12-01 01:30:05 -0800 (Fri, 01 Dec 2006) | 7 lines

- Fixed CIRCLE clipping bug
- Added SET_DRIVER_NAME as possible setting to ScreenControl, to force
  next call to Screen(Res) to use a specific driver
- Made ImageCreate to accept an additional optional fourth parameter to
  specify the image depth, if it differs from the current depth
- All constants/types in fbgfx.bi are now within the "FB" namespace

------------------------------------------------------------------------
r3545 | jeffmarshall | 2006-11-30 08:58:13 -0800 (Thu, 30 Nov 2006) | 2 lines

changed: ULONG is now a builtin type

------------------------------------------------------------------------
r3544 | v1ctor | 2006-11-30 07:11:36 -0800 (Thu, 30 Nov 2006) | 2 lines

added: check for non-static members

------------------------------------------------------------------------
r3543 | v1ctor | 2006-11-30 05:48:49 -0800 (Thu, 30 Nov 2006) | 2 lines

added: access mode check for operators

------------------------------------------------------------------------
r3542 | v1ctor | 2006-11-30 05:32:29 -0800 (Thu, 30 Nov 2006) | 2 lines

new test

------------------------------------------------------------------------
r3541 | v1ctor | 2006-11-30 05:28:43 -0800 (Thu, 30 Nov 2006) | 2 lines

fixed: methods' access modes should only be checked when overloading is resolved

------------------------------------------------------------------------
r3540 | v1ctor | 2006-11-29 10:49:45 -0800 (Wed, 29 Nov 2006) | 2 lines

fixed: enums declared inside TYPE's couldn't be used as data types

------------------------------------------------------------------------
r3539 | v1ctor | 2006-11-29 10:34:01 -0800 (Wed, 29 Nov 2006) | 2 lines

reverted: the compiler test suite stopped compiling because the last change

------------------------------------------------------------------------
r3538 | v1ctor | 2006-11-29 10:33:02 -0800 (Wed, 29 Nov 2006) | 2 lines

fixed: constants weren't allowed with pointers; different types of constants weren't being allowed; nodes were leaking

------------------------------------------------------------------------
r3537 | jeffmarshall | 2006-11-29 10:11:20 -0800 (Wed, 29 Nov 2006) | 2 lines

small change: gettext => _conio_gettext

------------------------------------------------------------------------
r3536 | whotookcha0s-_- | 2006-11-29 09:52:44 -0800 (Wed, 29 Nov 2006) | 2 lines

fixed: taking an enum or const of a dimmed UDT variable would fail.

------------------------------------------------------------------------
r3535 | whotookcha0s-_- | 2006-11-29 09:51:14 -0800 (Wed, 29 Nov 2006) | 2 lines

fixed: #define var var would fail if 'var' was passed as a parameter to another macro (var2) that used 'var' within 'var2'.

------------------------------------------------------------------------
r3534 | v1ctor | 2006-11-29 08:29:50 -0800 (Wed, 29 Nov 2006) | 2 lines

fixed: ()'s should be allowed around plain DELETE

------------------------------------------------------------------------
r3533 | v1ctor | 2006-11-29 08:25:00 -0800 (Wed, 29 Nov 2006) | 2 lines

changed: using real classes now

------------------------------------------------------------------------
r3532 | v1ctor | 2006-11-29 07:35:15 -0800 (Wed, 29 Nov 2006) | 2 lines

changed: the original name for libsupc++.a will be used now (it's only libsupcxx.a in DOS)

------------------------------------------------------------------------
r3531 | v1ctor | 2006-11-29 05:53:49 -0800 (Wed, 29 Nov 2006) | 2 lines

added: ctor/dtor access check

------------------------------------------------------------------------
r3530 | v1ctor | 2006-11-29 05:34:54 -0800 (Wed, 29 Nov 2006) | 2 lines

added: ctor/dtor access check

------------------------------------------------------------------------
r3529 | i_am_drv | 2006-11-28 21:01:29 -0800 (Tue, 28 Nov 2006) | 2 lines

fixed: size_t is unsigned int on linux

------------------------------------------------------------------------
r3528 | i_am_drv | 2006-11-28 20:41:51 -0800 (Tue, 28 Nov 2006) | 2 lines

fixed: libsupc++ filename varies by platform

------------------------------------------------------------------------
r3527 | v1ctor | 2006-11-28 19:35:08 -0800 (Tue, 28 Nov 2006) | 2 lines

added: NEW and DELETE operators

------------------------------------------------------------------------
r3526 | whotookcha0s-_- | 2006-11-28 14:02:37 -0800 (Tue, 28 Nov 2006) | 2 lines

added - the PRIVATE and PUBLIC attributes for members/methods of TYPE's

------------------------------------------------------------------------
r3525 | v1ctor | 2006-11-28 11:42:14 -0800 (Tue, 28 Nov 2006) | 2 lines

added: the LONG and ULONG builtin types

------------------------------------------------------------------------
r3524 | v1ctor | 2006-11-28 11:18:19 -0800 (Tue, 28 Nov 2006) | 2 lines

added: the LONG and ULONG builtin types

------------------------------------------------------------------------
r3523 | whotookcha0s-_- | 2006-11-28 10:46:01 -0800 (Tue, 28 Nov 2006) | 2 lines

fixed examples to support new-style gfx buffers also

------------------------------------------------------------------------
r3522 | v1ctor | 2006-11-28 10:08:38 -0800 (Tue, 28 Nov 2006) | 2 lines

removed: outdated

------------------------------------------------------------------------
r3521 | v1ctor | 2006-11-28 10:03:22 -0800 (Tue, 28 Nov 2006) | 2 lines

fixed: warning report in pedantic mode

------------------------------------------------------------------------
r3520 | v1ctor | 2006-11-28 10:02:26 -0800 (Tue, 28 Nov 2006) | 2 lines

added: the LONG and ULONG builtin types

------------------------------------------------------------------------
r3519 | v1ctor | 2006-11-28 09:49:21 -0800 (Tue, 28 Nov 2006) | 2 lines

added: the LONG and ULONG builtin types

------------------------------------------------------------------------
r3518 | i_am_drv | 2006-11-28 09:12:44 -0800 (Tue, 28 Nov 2006) | 2 lines

avoid conflict with errno #define on win32

------------------------------------------------------------------------
r3517 | v1ctor | 2006-11-27 08:35:59 -0800 (Mon, 27 Nov 2006) | 2 lines

small clean up

------------------------------------------------------------------------
r3516 | whotookcha0s-_- | 2006-11-27 04:45:15 -0800 (Mon, 27 Nov 2006) | 2 lines

added - FOR now supports UDT's as counter variables, if operator let, +=, and global <=, >= are overloaded with operators taking numeric rhs args. (regression tests also)

------------------------------------------------------------------------
r3515 | lillo | 2006-11-27 03:44:42 -0800 (Mon, 27 Nov 2006) | 2 lines

added an additional optional final parameter to BLOAD and BSAVE to hold the palette of the image, if any.

------------------------------------------------------------------------
r3514 | whotookcha0s-_- | 2006-11-27 01:18:52 -0800 (Mon, 27 Nov 2006) | 2 lines

cha0s is a dummy

------------------------------------------------------------------------
r3513 | whotookcha0s-_- | 2006-11-27 01:12:33 -0800 (Mon, 27 Nov 2006) | 2 lines

better overload error response for failed UDT to bool expr cast

------------------------------------------------------------------------
r3512 | whotookcha0s-_- | 2006-11-27 00:39:19 -0800 (Mon, 27 Nov 2006) | 2 lines

UDT boolean result test

------------------------------------------------------------------------
r3511 | whotookcha0s-_- | 2006-11-27 00:37:13 -0800 (Mon, 27 Nov 2006) | 2 lines

added - UDT's can now return a boolean result, if operator cast() was overloaded

------------------------------------------------------------------------
r3510 | lillo | 2006-11-26 10:15:49 -0800 (Sun, 26 Nov 2006) | 2 lines

Fixed inkey problem under X11

------------------------------------------------------------------------
r3509 | lillo | 2006-11-26 04:37:25 -0800 (Sun, 26 Nov 2006) | 2 lines

small improvements to the marsenne twister implementation

------------------------------------------------------------------------
r3508 | lillo | 2006-11-25 05:58:14 -0800 (Sat, 25 Nov 2006) | 2 lines

default bg color should have alpha set to 255

------------------------------------------------------------------------
r3507 | v1ctor | 2006-11-25 04:11:23 -0800 (Sat, 25 Nov 2006) | 2 lines

fixed: bug #1602493

------------------------------------------------------------------------
r3506 | whotookcha0s-_- | 2006-11-24 05:17:39 -0800 (Fri, 24 Nov 2006) | 3 lines

fixed - new variables in for blocks were being initialized on creation, not necessary;
 the start condition will do that

------------------------------------------------------------------------
r3505 | whotookcha0s-_- | 2006-11-23 23:28:20 -0800 (Thu, 23 Nov 2006) | 2 lines

fixed - bsave had the header size calculations backwards

------------------------------------------------------------------------
r3504 | lillo | 2006-11-23 09:54:24 -0800 (Thu, 23 Nov 2006) | 2 lines

fixed DirectDraw surface restoring when lost (on display mode change for example)

------------------------------------------------------------------------
r3503 | whotookcha0s-_- | 2006-11-23 07:33:24 -0800 (Thu, 23 Nov 2006) | 3 lines

fixed - no need to mind QB scope hacks in "for [var] as
 [type]", since that is -lang specific

------------------------------------------------------------------------
r3502 | lillo | 2006-11-23 06:51:27 -0800 (Thu, 23 Nov 2006) | 2 lines

fixed QB gfx compatibility

------------------------------------------------------------------------
r3501 | lillo | 2006-11-23 04:34:35 -0800 (Thu, 23 Nov 2006) | 2 lines

added 4 different RND implementation, defaulting to CRT one for now

------------------------------------------------------------------------
r3500 | lillo | 2006-11-22 10:31:30 -0800 (Wed, 22 Nov 2006) | 2 lines

Added vga16 framebuffer support for Linux fbdev driver

------------------------------------------------------------------------
r3499 | jeffmarshall | 2006-11-22 10:24:53 -0800 (Wed, 22 Nov 2006) | 2 lines

test for FOR...NEXT

------------------------------------------------------------------------
r3498 | lillo | 2006-11-22 06:33:51 -0800 (Wed, 22 Nov 2006) | 2 lines

cleanup

------------------------------------------------------------------------
r3497 | lillo | 2006-11-22 03:37:19 -0800 (Wed, 22 Nov 2006) | 2 lines

small fix, cleanup

------------------------------------------------------------------------
r3496 | lillo | 2006-11-22 02:17:56 -0800 (Wed, 22 Nov 2006) | 2 lines

fixed bug #1600115

------------------------------------------------------------------------
r3495 | whotookcha0s-_- | 2006-11-21 21:28:24 -0800 (Tue, 21 Nov 2006) | 2 lines

fixed - for i as ... wasn't handling scopes properly in lang-qb (cha0s)

------------------------------------------------------------------------
r3494 | jeffmarshall | 2006-11-21 15:45:30 -0800 (Tue, 21 Nov 2006) | 2 lines

fixed typos

------------------------------------------------------------------------
r3493 | lillo | 2006-11-21 14:02:05 -0800 (Tue, 21 Nov 2006) | 2 lines

small fix

------------------------------------------------------------------------
r3492 | whotookcha0s-_- | 2006-11-21 12:29:24 -0800 (Tue, 21 Nov 2006) | 2 lines

added - FOR [var] AS [type] = 0 to ... variable dimensioning withing FOR statements (cha0s)

------------------------------------------------------------------------
r3491 | jeffmarshall | 2006-11-21 09:13:52 -0800 (Tue, 21 Nov 2006) | 5 lines

changed:
- ignore offsite images in converted output
- fix preprocessor statments in syntax highlighter
- added missing keywords

------------------------------------------------------------------------
r3490 | jeffmarshall | 2006-11-21 08:48:15 -0800 (Tue, 21 Nov 2006) | 2 lines

c++ lib example

------------------------------------------------------------------------
r3489 | whotookcha0s-_- | 2006-11-21 06:56:24 -0800 (Tue, 21 Nov 2006) | 2 lines

fixed: arrays with lbounds <> 0 passed as args or dynamic would fail in quirk gfx functions

------------------------------------------------------------------------
r3488 | lillo | 2006-11-21 05:46:59 -0800 (Tue, 21 Nov 2006) | 2 lines

return refresh rate in ScreenInfo under X11 even if no gfx mode has been set

------------------------------------------------------------------------
r3487 | lillo | 2006-11-21 05:38:33 -0800 (Tue, 21 Nov 2006) | 2 lines

-Wall fixes

------------------------------------------------------------------------
r3486 | lillo | 2006-11-21 05:14:34 -0800 (Tue, 21 Nov 2006) | 2 lines

imagecreate should use old-style header only when -lang qb or deprecated is specified

------------------------------------------------------------------------
r3485 | lillo | 2006-11-21 04:52:57 -0800 (Tue, 21 Nov 2006) | 2 lines

use old-style get/put header only if -lang qb or deprecated is specified

------------------------------------------------------------------------
r3484 | lillo | 2006-11-21 04:52:14 -0800 (Tue, 21 Nov 2006) | 2 lines

pass QB compatibility flag to gfxlib if -lang qb or deprecated is specified

------------------------------------------------------------------------
r3483 | i_am_drv | 2006-11-21 04:48:27 -0800 (Tue, 21 Nov 2006) | 2 lines

updated

------------------------------------------------------------------------
r3482 | lillo | 2006-11-21 03:46:42 -0800 (Tue, 21 Nov 2006) | 2 lines

speeded up alpha primitives drawing by adding an mmx single pixel plotter

------------------------------------------------------------------------
r3481 | whotookcha0s-_- | 2006-11-21 01:04:16 -0800 (Tue, 21 Nov 2006) | 2 lines

fb_GfxGet was using the array descriptor's 'ptr' field to assert the validity of the data, instead of the 'data' field

------------------------------------------------------------------------
r3480 | whotookcha0s-_- | 2006-11-21 00:38:11 -0800 (Tue, 21 Nov 2006) | 2 lines

fixed - static arrays with lower bounds <> 0 would fail when passed as index-less arguments to quirk gfx statements

------------------------------------------------------------------------
r3479 | lillo | 2006-11-20 09:57:51 -0800 (Mon, 20 Nov 2006) | 2 lines

Fixed refresh rate computation on Linux

------------------------------------------------------------------------
r3478 | whotookcha0s-_- | 2006-11-20 03:21:45 -0800 (Mon, 20 Nov 2006) | 2 lines

temp fix for null ptr checks in -exx when taking the address of pointers or UDT fields

------------------------------------------------------------------------
r3477 | i_am_drv | 2006-11-20 03:17:24 -0800 (Mon, 20 Nov 2006) | 2 lines

remove unused variables

------------------------------------------------------------------------
r3476 | whotookcha0s-_- | 2006-11-20 02:58:29 -0800 (Mon, 20 Nov 2006) | 2 lines

PRINT USING was not adding the stars and dollar sign to the integer padding amount.

------------------------------------------------------------------------
r3475 | whotookcha0s-_- | 2006-11-20 01:28:49 -0800 (Mon, 20 Nov 2006) | 2 lines

fixed period-in-symbol problem caused by ambiguity in quirk DRAW stmt

------------------------------------------------------------------------
r3474 | whotookcha0s-_- | 2006-11-20 00:18:25 -0800 (Mon, 20 Nov 2006) | 2 lines

fixed bug where quirk gfx statements would assume properties were always procs, not functions

------------------------------------------------------------------------
r3473 | whotookcha0s-_- | 2006-11-20 00:14:01 -0800 (Mon, 20 Nov 2006) | 2 lines

fixed bug with -exx and nested ctors

------------------------------------------------------------------------
r3472 | i_am_drv | 2006-11-19 22:45:05 -0800 (Sun, 19 Nov 2006) | 2 lines

added /lib32 and /usr/lib32 to the library search dir list needed for some 64-bit Linux distributions like Debian

------------------------------------------------------------------------
r3471 | i_am_drv | 2006-11-19 22:42:17 -0800 (Sun, 19 Nov 2006) | 2 lines

added __EH_FRAME_BEGIN__ and __EH_FRAME_END__

------------------------------------------------------------------------
r3470 | lillo | 2006-11-19 16:30:22 -0800 (Sun, 19 Nov 2006) | 2 lines

Fixed mmx conflicts with fpu. Cleanup.

------------------------------------------------------------------------
r3469 | lillo | 2006-11-18 10:09:28 -0800 (Sat, 18 Nov 2006) | 2 lines

added double click support to the linux fbdev driver as well

------------------------------------------------------------------------
r3468 | lillo | 2006-11-18 09:55:29 -0800 (Sat, 18 Nov 2006) | 2 lines

implemented double click support for linux

------------------------------------------------------------------------
r3467 | lillo | 2006-11-18 08:12:52 -0800 (Sat, 18 Nov 2006) | 4 lines

- Added EVENT_MOUSE_DOUBLE_CLICK event support to SCREENEVENT (win32 only for now)
- Fixed fullscreen GDI palette handling
- Cleanup

------------------------------------------------------------------------
r3466 | i_am_drv | 2006-11-17 05:35:36 -0800 (Fri, 17 Nov 2006) | 2 lines

fix DOS build

------------------------------------------------------------------------
r3465 | i_am_drv | 2006-11-17 05:01:10 -0800 (Fri, 17 Nov 2006) | 2 lines

fixed compilation on DOS

------------------------------------------------------------------------
r3464 | i_am_drv | 2006-11-16 22:55:32 -0800 (Thu, 16 Nov 2006) | 2 lines

fix size assumptions

------------------------------------------------------------------------
r3463 | i_am_drv | 2006-11-16 22:12:40 -0800 (Thu, 16 Nov 2006) | 3 lines

- fixed pointer size assumptions
- added x86_64 target cpu to rtlib

------------------------------------------------------------------------
r3462 | lillo | 2006-11-16 14:56:06 -0800 (Thu, 16 Nov 2006) | 2 lines

rtlib has to know gfxlib needs a dedicated TLS slot

------------------------------------------------------------------------
r3461 | lillo | 2006-11-16 14:55:14 -0800 (Thu, 16 Nov 2006) | 4 lines

- First big step towards making gfxlib thread-safe: now using TLS to
store many contextual internal states
- Cleanup

------------------------------------------------------------------------
r3460 | v1ctor | 2006-11-16 07:26:22 -0800 (Thu, 16 Nov 2006) | 2 lines

fixed: bug #1597700

------------------------------------------------------------------------
r3459 | v1ctor | 2006-11-16 06:44:26 -0800 (Thu, 16 Nov 2006) | 2 lines

fixed: bug #1597763

------------------------------------------------------------------------
r3458 | v1ctor | 2006-11-15 19:20:18 -0800 (Wed, 15 Nov 2006) | 2 lines

fixed: parameter number when reporting errors in methods

------------------------------------------------------------------------
r3457 | jeffmarshall | 2006-11-15 18:32:33 -0800 (Wed, 15 Nov 2006) | 2 lines

fixed: DRAW STRING was not freeing temporaries on error/zero length string.

------------------------------------------------------------------------
r3456 | jeffmarshall | 2006-11-15 16:03:12 -0800 (Wed, 15 Nov 2006) | 2 lines

fixed: Quirk $INCLUDE was not seeing the single quoted file names

------------------------------------------------------------------------
r3455 | jeffmarshall | 2006-11-15 15:46:53 -0800 (Wed, 15 Nov 2006) | 2 lines

changed: DRAW STRING will not throw an error when passed a zero length string

------------------------------------------------------------------------
r3454 | v1ctor | 2006-11-15 06:35:23 -0800 (Wed, 15 Nov 2006) | 2 lines

added: PostgreSQL headers (fixed by dr0p)

------------------------------------------------------------------------
r3453 | v1ctor | 2006-11-15 04:51:38 -0800 (Wed, 15 Nov 2006) | 4 lines

fixed:
- bug #1596674
- the namespaces order of the reported method names was wrong, when an error occured

------------------------------------------------------------------------
r3452 | v1ctor | 2006-11-14 15:27:52 -0800 (Tue, 14 Nov 2006) | 2 lines

new test

------------------------------------------------------------------------
r3451 | v1ctor | 2006-11-14 15:24:34 -0800 (Tue, 14 Nov 2006) | 2 lines

fixed: bug #1596610

------------------------------------------------------------------------
r3450 | i_am_drv | 2006-11-14 02:56:58 -0800 (Tue, 14 Nov 2006) | 2 lines

consolidate DOS memory locking macros

------------------------------------------------------------------------
r3449 | v1ctor | 2006-11-13 19:38:24 -0800 (Mon, 13 Nov 2006) | 2 lines

fixed: initialization of var-len string arrays would fail if complex expressions were passed

------------------------------------------------------------------------
r3448 | i_am_drv | 2006-11-13 08:08:34 -0800 (Mon, 13 Nov 2006) | 2 lines

fixed: 486 detection

------------------------------------------------------------------------
r3447 | i_am_drv | 2006-11-13 06:48:13 -0800 (Mon, 13 Nov 2006) | 2 lines

changed: ScreenEvent on DOS now takes numlock into account

------------------------------------------------------------------------
r3446 | i_am_drv | 2006-11-13 04:36:14 -0800 (Mon, 13 Nov 2006) | 2 lines

changed: ScreenEvent on DOS now takes caps lock into account and produces key repeat events when necessary

------------------------------------------------------------------------
r3445 | i_am_drv | 2006-11-13 03:24:09 -0800 (Mon, 13 Nov 2006) | 2 lines

fixed: keyboard buffer no longer gets full on DOS when calling only multikey/screenevent and not inkey

------------------------------------------------------------------------
r3444 | v1ctor | 2006-11-12 11:58:46 -0800 (Sun, 12 Nov 2006) | 2 lines

added: indexed properties

------------------------------------------------------------------------
r3443 | v1ctor | 2006-11-11 20:51:18 -0800 (Sat, 11 Nov 2006) | 2 lines

fixed: bug #1594927

------------------------------------------------------------------------
r3442 | v1ctor | 2006-11-11 20:44:17 -0800 (Sat, 11 Nov 2006) | 2 lines

fixed: bug #1594920

------------------------------------------------------------------------
r3441 | i_am_drv | 2006-11-11 16:55:14 -0800 (Sat, 11 Nov 2006) | 2 lines

define some constants to allow compilation with old Mesa headers

------------------------------------------------------------------------
r3440 | jeffmarshall | 2006-11-11 10:54:48 -0800 (Sat, 11 Nov 2006) | 2 lines

fixed: FORMAT not releasing locks

------------------------------------------------------------------------
r3439 | jeffmarshall | 2006-11-10 10:12:15 -0800 (Fri, 10 Nov 2006) | 3 lines

*opengl.c files moved from /src/gfxlib2/[target/] to /src/gfxlib2/opengl/[target/]
gfxlib configure checks for GL/gl.h and defines HAVE_GL_GL_H if present.

------------------------------------------------------------------------
r3438 | i_am_drv | 2006-11-10 03:47:14 -0800 (Fri, 10 Nov 2006) | 2 lines

add multikey ISR hook (DOS-only) for ScreenEvent support

------------------------------------------------------------------------
r3437 | v1ctor | 2006-11-09 12:12:34 -0800 (Thu, 09 Nov 2006) | 2 lines

fixed: missing ?etClassLongPtr defines

------------------------------------------------------------------------
r3436 | v1ctor | 2006-11-09 12:05:21 -0800 (Thu, 09 Nov 2006) | 4 lines

fixed: REDIM when used with arrays declared in different namespaces

changed: all variables declared inside namespaces are now SHARED by default

------------------------------------------------------------------------
r3435 | v1ctor | 2006-11-09 08:07:47 -0800 (Thu, 09 Nov 2006) | 2 lines

fixed: bug #1593503

------------------------------------------------------------------------
r3434 | i_am_drv | 2006-11-08 20:06:22 -0800 (Wed, 08 Nov 2006) | 2 lines

fix for bug 1577407

------------------------------------------------------------------------
r3433 | v1ctor | 2006-11-08 18:26:47 -0800 (Wed, 08 Nov 2006) | 2 lines

fixed: overloaded methods with different signatures could be declared outside the original class

------------------------------------------------------------------------
r3432 | v1ctor | 2006-11-08 16:05:07 -0800 (Wed, 08 Nov 2006) | 2 lines

changed: __FUNCTION__ now returns the namespaces and checks for properties, ctors/dtors and operators

------------------------------------------------------------------------
r3431 | lillo | 2006-11-06 15:56:01 -0800 (Mon, 06 Nov 2006) | 2 lines

on keypress event, event.ascii should be 0 on extended keys (ex: arrow keys) since they are not ascii

------------------------------------------------------------------------
r3430 | lillo | 2006-11-06 15:15:00 -0800 (Mon, 06 Nov 2006) | 4 lines

- Dragging around a window using the GDI driver does not screw up window contents anymore
- Added GFX_ALWAYS_ON_TOP support for win32 drivers
- Some code cleanup

------------------------------------------------------------------------
r3429 | v1ctor | 2006-11-06 11:30:53 -0800 (Mon, 06 Nov 2006) | 2 lines

fixed: bug #1590764

------------------------------------------------------------------------
r3428 | lillo | 2006-11-06 09:35:15 -0800 (Mon, 06 Nov 2006) | 2 lines

fixed EVENT_KEY_REPEAT under win32, also now mouse events are not reported if app has not got focus

------------------------------------------------------------------------
r3427 | lillo | 2006-11-06 09:23:12 -0800 (Mon, 06 Nov 2006) | 2 lines

forgot fb_hDynLoadAlso prototype for internal use

------------------------------------------------------------------------
r3426 | jeffmarshall | 2006-11-06 08:32:37 -0800 (Mon, 06 Nov 2006) | 2 lines

added fb_hDynLoadAlso() for win32

------------------------------------------------------------------------
r3425 | lillo | 2006-11-05 17:53:18 -0800 (Sun, 05 Nov 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r3424 | lillo | 2006-11-05 17:50:39 -0800 (Sun, 05 Nov 2006) | 2 lines

don't report keypress if trying to switch to the current console

------------------------------------------------------------------------
r3423 | lillo | 2006-11-05 17:45:27 -0800 (Sun, 05 Nov 2006) | 2 lines

Added EVENT_KEY_REPEAT support to Linux fbdev driver

------------------------------------------------------------------------
r3422 | lillo | 2006-11-05 17:24:36 -0800 (Sun, 05 Nov 2006) | 2 lines

cleanup

------------------------------------------------------------------------
r3421 | lillo | 2006-11-05 17:20:21 -0800 (Sun, 05 Nov 2006) | 2 lines

don't report mouse motion events if window has not got focus

------------------------------------------------------------------------
r3420 | lillo | 2006-11-05 16:21:49 -0800 (Sun, 05 Nov 2006) | 2 lines

swapped GFX_ALPHA_PRIMITIVES and GFX_ALWAYS_ON_TOP flag values. Makes more sense to have all window related flags side by side

------------------------------------------------------------------------
r3419 | lillo | 2006-11-05 16:12:38 -0800 (Sun, 05 Nov 2006) | 2 lines

forgot these

------------------------------------------------------------------------
r3418 | lillo | 2006-11-05 16:04:40 -0800 (Sun, 05 Nov 2006) | 2 lines

Added GFX_ALWAYS_ON_TOP screen flag, cleaned up X11 drivers

------------------------------------------------------------------------
r3417 | lillo | 2006-11-05 14:06:26 -0800 (Sun, 05 Nov 2006) | 2 lines

Added the EVENT_KEY_REPEAT event

------------------------------------------------------------------------
r3416 | lillo | 2006-11-05 13:38:02 -0800 (Sun, 05 Nov 2006) | 2 lines

return ascii code (if available) also on key release event under X11

------------------------------------------------------------------------
r3415 | lillo | 2006-11-05 13:23:56 -0800 (Sun, 05 Nov 2006) | 2 lines

Save windows position/size before changing fullscreen mode, so we can restore them later. This is to avoid the mess X11 does with windows when switching modes

------------------------------------------------------------------------
r3414 | lillo | 2006-11-05 08:27:16 -0800 (Sun, 05 Nov 2006) | 2 lines

example now makes use of fullscreen antialiasing

------------------------------------------------------------------------
r3413 | lillo | 2006-11-05 08:23:52 -0800 (Sun, 05 Nov 2006) | 2 lines

updated fbgfx.bi with GFX_MULTISAMPLE definition

------------------------------------------------------------------------
r3412 | lillo | 2006-11-05 08:22:27 -0800 (Sun, 05 Nov 2006) | 2 lines

fb_hDynLoadAlso implementation for Linux

------------------------------------------------------------------------
r3411 | lillo | 2006-11-05 08:21:34 -0800 (Sun, 05 Nov 2006) | 2 lines

enabled GFX_MULTISAMPLE under Linux too

------------------------------------------------------------------------
r3410 | lillo | 2006-11-05 08:00:55 -0800 (Sun, 05 Nov 2006) | 3 lines

- GFX_MULTISAMPLE screen flag to enable fullscreen antialiasing in OpenGL mode
- Beginnings of the GL backend

------------------------------------------------------------------------
r3409 | lillo | 2006-11-05 06:04:42 -0800 (Sun, 05 Nov 2006) | 2 lines

fixed 16->24 bpp screen updater blitters

------------------------------------------------------------------------
r3408 | v1ctor | 2006-11-04 20:43:59 -0800 (Sat, 04 Nov 2006) | 2 lines

added: properties (better error messages if GET or SET methods weren't defined)

------------------------------------------------------------------------
r3407 | jeffmarshall | 2006-11-04 17:24:56 -0800 (Sat, 04 Nov 2006) | 2 lines

updated: new keywords

------------------------------------------------------------------------
r3406 | jeffmarshall | 2006-11-04 17:23:30 -0800 (Sat, 04 Nov 2006) | 2 lines

added positional fields to wiki parser and tokens

------------------------------------------------------------------------
r3405 | v1ctor | 2006-11-04 15:04:20 -0800 (Sat, 04 Nov 2006) | 2 lines

fixed: bug #1590383 (patch written by cha0s)

------------------------------------------------------------------------
r3404 | v1ctor | 2006-11-04 14:51:18 -0800 (Sat, 04 Nov 2006) | 2 lines

added: properties

------------------------------------------------------------------------
r3403 | v1ctor | 2006-11-04 14:44:40 -0800 (Sat, 04 Nov 2006) | 2 lines

added: properties

------------------------------------------------------------------------
r3402 | lillo | 2006-11-02 07:14:55 -0800 (Thu, 02 Nov 2006) | 2 lines

screencopy crashed if specified pages did not exist

------------------------------------------------------------------------
r3401 | lillo | 2006-11-02 03:08:15 -0800 (Thu, 02 Nov 2006) | 2 lines

don't send focus events on window expose

------------------------------------------------------------------------
r3400 | lillo | 2006-11-01 08:06:44 -0800 (Wed, 01 Nov 2006) | 2 lines

Fixed more assumption of alpha=0 in screen update blitters

------------------------------------------------------------------------
r3399 | lillo | 2006-11-01 07:59:10 -0800 (Wed, 01 Nov 2006) | 2 lines

fix for 32->24 bpp screen update blitters

------------------------------------------------------------------------
r3398 | v1ctor | 2006-10-31 13:48:28 -0800 (Tue, 31 Oct 2006) | 2 lines

fixed: bug #1585768

------------------------------------------------------------------------
r3397 | lillo | 2006-10-31 08:55:36 -0800 (Tue, 31 Oct 2006) | 2 lines

Fixed pitch computation when PUTting an old-style gfx buffer

------------------------------------------------------------------------
r3396 | lillo | 2006-10-31 05:52:51 -0800 (Tue, 31 Oct 2006) | 2 lines

ensure image header reserved words are zero on creation

------------------------------------------------------------------------
r3395 | lillo | 2006-10-29 14:09:09 -0800 (Sun, 29 Oct 2006) | 2 lines

Fixed a bug in the C add blitter

------------------------------------------------------------------------
r3394 | lillo | 2006-10-29 02:48:42 -0800 (Sun, 29 Oct 2006) | 2 lines

fixed bug #1586405; C blitters now handle any framebuffer width

------------------------------------------------------------------------
r3393 | i_am_drv | 2006-10-29 01:45:04 -0800 (Sun, 29 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3392 | i_am_drv | 2006-10-29 01:43:06 -0800 (Sun, 29 Oct 2006) | 2 lines

changed: SetDate/SetTime now return an error when they fail (request 1583292)

------------------------------------------------------------------------
r3391 | i_am_drv | 2006-10-28 19:23:14 -0700 (Sat, 28 Oct 2006) | 2 lines

arrays in types + with regression test (by cha0s)

------------------------------------------------------------------------
r3390 | i_am_drv | 2006-10-28 19:10:57 -0700 (Sat, 28 Oct 2006) | 2 lines

added: regex replace example (by MindlessXD)

------------------------------------------------------------------------
r3389 | i_am_drv | 2006-10-28 17:37:09 -0700 (Sat, 28 Oct 2006) | 2 lines

fixed: bug 1585768 - patch by cha0s

------------------------------------------------------------------------
r3388 | i_am_drv | 2006-10-28 17:22:51 -0700 (Sat, 28 Oct 2006) | 2 lines

added protected-mode bank switcher

------------------------------------------------------------------------
r3387 | jeffmarshall | 2006-10-28 08:57:09 -0700 (Sat, 28 Oct 2006) | 2 lines

fixed: missing fb.h

------------------------------------------------------------------------
r3386 | jeffmarshall | 2006-10-28 08:56:00 -0700 (Sat, 28 Oct 2006) | 2 lines

fixed: dos needs FB_DYLIB defined to keep headers happy

------------------------------------------------------------------------
r3385 | jeffmarshall | 2006-10-28 07:52:48 -0700 (Sat, 28 Oct 2006) | 2 lines

added (currently disabled) .hidden/.protected attribute support for better cross-plat shared lib compatibility later.

------------------------------------------------------------------------
r3384 | lillo | 2006-10-28 06:41:52 -0700 (Sat, 28 Oct 2006) | 2 lines

Added generic dynamic runtime library loading routines; win32 opengl gfx driver uses this now to load opengl32.dll functions at startup

------------------------------------------------------------------------
r3383 | lillo | 2006-10-27 06:39:06 -0700 (Fri, 27 Oct 2006) | 2 lines

fixed bug #1584069

------------------------------------------------------------------------
r3382 | lillo | 2006-10-27 05:41:33 -0700 (Fri, 27 Oct 2006) | 2 lines

fixed bug #1583876, removed fb_hResize() call need

------------------------------------------------------------------------
r3381 | lillo | 2006-10-27 02:09:18 -0700 (Fri, 27 Oct 2006) | 2 lines

added mmx pixel blitters (should speed up all raster operations that fill regions like cls, line ,bf and circle ,f

------------------------------------------------------------------------
r3380 | lillo | 2006-10-27 01:37:05 -0700 (Fri, 27 Oct 2006) | 2 lines

tentative fix for shaped window under win32

------------------------------------------------------------------------
r3379 | i_am_drv | 2006-10-25 23:59:52 -0700 (Wed, 25 Oct 2006) | 2 lines

fixed: __PATH__ contained backslashes on Linux in some cases (patch by cha0s)

------------------------------------------------------------------------
r3378 | v1ctor | 2006-10-25 21:19:01 -0700 (Wed, 25 Oct 2006) | 2 lines

added: web site source-code

------------------------------------------------------------------------
r3377 | v1ctor | 2006-10-25 21:18:13 -0700 (Wed, 25 Oct 2006) | 2 lines

updated: copyright notice

------------------------------------------------------------------------
r3376 | v1ctor | 2006-10-25 15:52:49 -0700 (Wed, 25 Oct 2006) | 2 lines

added: web site source-code

------------------------------------------------------------------------
r3375 | i_am_drv | 2006-10-25 14:49:47 -0700 (Wed, 25 Oct 2006) | 2 lines

use protected-mode bank setter when available in banked vbe driver

------------------------------------------------------------------------
r3374 | v1ctor | 2006-10-25 10:24:19 -0700 (Wed, 25 Oct 2006) | 2 lines

fixed: the object destructor was not being called

------------------------------------------------------------------------
r3373 | i_am_drv | 2006-10-25 00:26:49 -0700 (Wed, 25 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3372 | i_am_drv | 2006-10-25 00:20:29 -0700 (Wed, 25 Oct 2006) | 2 lines

fixed: DOS gfxlib always failed when no mouse was detected

------------------------------------------------------------------------
r3371 | v1ctor | 2006-10-24 19:24:25 -0700 (Tue, 24 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3370 | v1ctor | 2006-10-24 19:23:46 -0700 (Tue, 24 Oct 2006) | 2 lines

updated: copyright notice

------------------------------------------------------------------------
r3369 | v1ctor | 2006-10-24 19:18:06 -0700 (Tue, 24 Oct 2006) | 2 lines

updated: copyright notice

------------------------------------------------------------------------
r3368 | v1ctor | 2006-10-24 19:12:20 -0700 (Tue, 24 Oct 2006) | 2 lines

new test

------------------------------------------------------------------------
r3367 | v1ctor | 2006-10-24 19:11:12 -0700 (Tue, 24 Oct 2006) | 2 lines

fixed: bug #1584112

------------------------------------------------------------------------
r3366 | v1ctor | 2006-10-24 19:07:19 -0700 (Tue, 24 Oct 2006) | 2 lines

fixed: bug #1582588

------------------------------------------------------------------------
r3365 | v1ctor | 2006-10-24 19:03:28 -0700 (Tue, 24 Oct 2006) | 2 lines

fixed: bug #1582590

------------------------------------------------------------------------
r3364 | jeffmarshall | 2006-10-23 17:44:42 -0700 (Mon, 23 Oct 2006) | 2 lines

small internal change

------------------------------------------------------------------------
r3363 | jeffmarshall | 2006-10-23 17:43:50 -0700 (Mon, 23 Oct 2006) | 2 lines

no message

------------------------------------------------------------------------
r3362 | lillo | 2006-10-23 16:56:35 -0700 (Mon, 23 Oct 2006) | 2 lines

forgot to add win32 set_window_pos() hook implementation

------------------------------------------------------------------------
r3361 | lillo | 2006-10-23 15:51:57 -0700 (Mon, 23 Oct 2006) | 2 lines

updated fbgfx.bi with new constants

------------------------------------------------------------------------
r3360 | lillo | 2006-10-23 15:51:15 -0700 (Mon, 23 Oct 2006) | 2 lines

Added GET_WINDOW_HANDLE support to ScreenControl

------------------------------------------------------------------------
r3359 | lillo | 2006-10-23 15:28:26 -0700 (Mon, 23 Oct 2006) | 2 lines

window shape transparency should be indipendent of alpha

------------------------------------------------------------------------
r3358 | lillo | 2006-10-23 15:20:42 -0700 (Mon, 23 Oct 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r3357 | jeffmarshall | 2006-10-23 13:58:18 -0700 (Mon, 23 Oct 2006) | 3 lines

Fixed: Windows COM driver was ignoring the TX and RX buffer size settings.
Updated: FILEATTR returns file handles for COM and LPT devices

------------------------------------------------------------------------
r3356 | lillo | 2006-10-23 13:54:48 -0700 (Mon, 23 Oct 2006) | 2 lines

fix for X11 set_window_pos() hook: getting window coordinates worked only with borderless windows

------------------------------------------------------------------------
r3355 | lillo | 2006-10-23 05:05:21 -0700 (Mon, 23 Oct 2006) | 2 lines

forgot to add this

------------------------------------------------------------------------
r3354 | lillo | 2006-10-23 05:04:46 -0700 (Mon, 23 Oct 2006) | 2 lines

use XQueryPointer to have more responsiveness in GETMOUSE

------------------------------------------------------------------------
r3353 | lillo | 2006-10-23 03:43:08 -0700 (Mon, 23 Oct 2006) | 5 lines

- SCREENCONTROL implementation. See fbgfx.bi for a list of accepted parameters
- Window positioning support via SET/GET_WINDOW_POS to SCREENCONTROL:
	SCREENCONTROL GET_WINDOW_POS, x, y
	SCREENCONTROL SET_WINDOW_POS, x + 100, y + 100

------------------------------------------------------------------------
r3352 | i_am_drv | 2006-10-23 02:43:39 -0700 (Mon, 23 Oct 2006) | 2 lines

added #inclib "user32" for win32 (not a default lib)

------------------------------------------------------------------------
r3351 | jeffmarshall | 2006-10-22 20:43:21 -0700 (Sun, 22 Oct 2006) | 2 lines

Added: Priority attribute for global (module) constructors and destructors.

------------------------------------------------------------------------
r3350 | jeffmarshall | 2006-10-22 20:40:53 -0700 (Sun, 22 Oct 2006) | 2 lines

Changed: Global Init routines moved to .ctors.* section

------------------------------------------------------------------------
r3349 | v1ctor | 2006-10-22 20:17:11 -0700 (Sun, 22 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3348 | lillo | 2006-10-22 02:20:53 -0700 (Sun, 22 Oct 2006) | 2 lines

Fixed stupid bug in GET

------------------------------------------------------------------------
r3347 | v1ctor | 2006-10-21 19:05:23 -0700 (Sat, 21 Oct 2006) | 2 lines

fixed: an EXTERN block around a TYPE could disable mangling of methods

------------------------------------------------------------------------
r3346 | v1ctor | 2006-10-21 11:55:08 -0700 (Sat, 21 Oct 2006) | 2 lines

new example (fbgl written by Angelo)

------------------------------------------------------------------------
r3345 | v1ctor | 2006-10-21 11:51:08 -0700 (Sat, 21 Oct 2006) | 2 lines

new example (fbgl written by Angelo)

------------------------------------------------------------------------
r3344 | jeffmarshall | 2006-10-20 12:56:02 -0700 (Fri, 20 Oct 2006) | 2 lines

Fixed: LPRINT was being seen as PRINT.  LPRINT is now allowed but only with -lang qb or deprecated.

------------------------------------------------------------------------
r3343 | jeffmarshall | 2006-10-19 20:51:28 -0700 (Thu, 19 Oct 2006) | 2 lines

libfb_ctor.o must also go in the group or linking will sometimes fail depending on order of inclib's

------------------------------------------------------------------------
r3342 | v1ctor | 2006-10-19 08:46:29 -0700 (Thu, 19 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3341 | v1ctor | 2006-10-18 22:16:01 -0700 (Wed, 18 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3340 | v1ctor | 2006-10-18 21:59:51 -0700 (Wed, 18 Oct 2006) | 2 lines

added: the GdiPlus headers

------------------------------------------------------------------------
r3339 | v1ctor | 2006-10-18 21:33:47 -0700 (Wed, 18 Oct 2006) | 2 lines

added: the GdiPlus headers

------------------------------------------------------------------------
r3338 | v1ctor | 2006-10-18 21:32:17 -0700 (Wed, 18 Oct 2006) | 2 lines

added: type aliases of TYPE's can now be used to invoke methods and constructors too

------------------------------------------------------------------------
r3337 | jeffmarshall | 2006-10-18 20:57:34 -0700 (Wed, 18 Oct 2006) | 2 lines

Swapped order of .ctors and .ctors.* for consistency with linux/mingw

------------------------------------------------------------------------
r3336 | v1ctor | 2006-10-18 19:43:27 -0700 (Wed, 18 Oct 2006) | 2 lines

changed: EXTERN .. END EXTERN blocks can now be declared inside namespaces to exclude the parent chain when mangling the symbols

------------------------------------------------------------------------
r3335 | v1ctor | 2006-10-18 18:35:27 -0700 (Wed, 18 Oct 2006) | 2 lines

added: ENUM .. END ENUM and CONST can now be declared inside TYPE's

------------------------------------------------------------------------
r3334 | v1ctor | 2006-10-18 11:29:13 -0700 (Wed, 18 Oct 2006) | 3 lines

changed: config.h will be always pre-included to set
_FILE_OFFSET_BITS=64 and define off_t correctly in *nix

------------------------------------------------------------------------
r3333 | v1ctor | 2006-10-18 11:04:16 -0700 (Wed, 18 Oct 2006) | 2 lines

new test

------------------------------------------------------------------------
r3332 | v1ctor | 2006-10-18 09:59:11 -0700 (Wed, 18 Oct 2006) | 2 lines

new test

------------------------------------------------------------------------
r3331 | v1ctor | 2006-10-17 10:35:25 -0700 (Tue, 17 Oct 2006) | 2 lines

changed: pipe() renamed to pipe_()

------------------------------------------------------------------------
r3330 | jeffmarshall | 2006-10-16 22:11:41 -0700 (Mon, 16 Oct 2006) | 2 lines

fixed: bug #1577280

------------------------------------------------------------------------
r3329 | v1ctor | 2006-10-16 17:26:38 -0700 (Mon, 16 Oct 2006) | 2 lines

fixed: bug #1578526

------------------------------------------------------------------------
r3328 | v1ctor | 2006-10-16 11:03:06 -0700 (Mon, 16 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3327 | v1ctor | 2006-10-16 10:56:56 -0700 (Mon, 16 Oct 2006) | 2 lines

changed: headers replaced by the machine-translated ones

------------------------------------------------------------------------
r3326 | v1ctor | 2006-10-16 10:54:49 -0700 (Mon, 16 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3325 | v1ctor | 2006-10-16 09:22:37 -0700 (Mon, 16 Oct 2006) | 2 lines

fixed: djgpp bug, the global constructors should be called in LIFO order

------------------------------------------------------------------------
r3324 | v1ctor | 2006-10-16 09:21:31 -0700 (Mon, 16 Oct 2006) | 2 lines

files moved to /djgpp

------------------------------------------------------------------------
r3323 | v1ctor | 2006-10-16 00:29:01 -0700 (Mon, 16 Oct 2006) | 2 lines

new tests

------------------------------------------------------------------------
r3322 | v1ctor | 2006-10-16 00:26:54 -0700 (Mon, 16 Oct 2006) | 2 lines

fixed: temporary symbols created inside var-initializer expressions were not being reallocated/cloned inside the (global) constructors

------------------------------------------------------------------------
r3321 | jeffmarshall | 2006-10-15 08:37:48 -0700 (Sun, 15 Oct 2006) | 2 lines

fixed: build on linux, added:

------------------------------------------------------------------------
r3320 | v1ctor | 2006-10-14 22:16:47 -0700 (Sat, 14 Oct 2006) | 2 lines

small -Wall "fix"

------------------------------------------------------------------------
r3319 | i_am_drv | 2006-10-14 08:18:41 -0700 (Sat, 14 Oct 2006) | 2 lines

add ScreenEvent implementation for DOS

------------------------------------------------------------------------
r3318 | i_am_drv | 2006-10-14 08:17:52 -0700 (Sat, 14 Oct 2006) | 2 lines

fix memory locking code

------------------------------------------------------------------------
r3317 | lillo | 2006-10-14 04:40:08 -0700 (Sat, 14 Oct 2006) | 2 lines

EVENT is now a packed structure

------------------------------------------------------------------------
r3315 | lillo | 2006-10-14 03:34:59 -0700 (Sat, 14 Oct 2006) | 2 lines

ScreenEvent parameter defaults to NULL if omitted

------------------------------------------------------------------------
r3314 | lillo | 2006-10-14 03:17:20 -0700 (Sat, 14 Oct 2006) | 2 lines

added SCREENEVENT support for Win32

------------------------------------------------------------------------
r3313 | lillo | 2006-10-14 03:10:56 -0700 (Sat, 14 Oct 2006) | 2 lines

added internal support function to convert a Win32 virtual keycode to an FB scancode

------------------------------------------------------------------------
r3312 | lillo | 2006-10-14 02:29:48 -0700 (Sat, 14 Oct 2006) | 2 lines

merged libfb_io_keys.c into libfb_io_input.c (they both declared and used the same big static data tables)

------------------------------------------------------------------------
r3311 | lillo | 2006-10-13 14:15:09 -0700 (Fri, 13 Oct 2006) | 6 lines

- Added new SCREENEVENT gfx function to retrieve system events. Linux
fbdev and X11 drivers updated to support events, missing DOS and Win32
drivers.
- Added an example on SCREENEVENT (examples/gfx/fbgfx_events.bas)
- Updated fbgfx.bi with new EVENT structure and supported event type IDs

------------------------------------------------------------------------
r3310 | v1ctor | 2006-10-13 07:30:25 -0700 (Fri, 13 Oct 2006) | 2 lines

fixed: bug introduced with the last fix

------------------------------------------------------------------------
r3309 | i_am_drv | 2006-10-13 02:45:21 -0700 (Fri, 13 Oct 2006) | 2 lines

fixed off-by-one error

------------------------------------------------------------------------
r3308 | v1ctor | 2006-10-12 14:06:26 -0700 (Thu, 12 Oct 2006) | 2 lines

internal change: the register allocator now takes the sizes into account so SI/DI won't be allocated as byte registers, simplifying the x86 emitter a bit

------------------------------------------------------------------------
r3307 | v1ctor | 2006-10-12 13:57:37 -0700 (Thu, 12 Oct 2006) | 2 lines

fixed: the proc node was not being set if the offset expression was NULL, causing an ICE

------------------------------------------------------------------------
r3306 | v1ctor | 2006-10-12 13:40:27 -0700 (Thu, 12 Oct 2006) | 2 lines

small -Wall "fix"

------------------------------------------------------------------------
r3305 | lillo | 2006-10-12 09:04:05 -0700 (Thu, 12 Oct 2006) | 2 lines

updated fbgfx.bi with new useful constants

------------------------------------------------------------------------
r3304 | lillo | 2006-10-12 09:03:27 -0700 (Thu, 12 Oct 2006) | 2 lines

forgot to update these

------------------------------------------------------------------------
r3303 | lillo | 2006-10-12 08:30:28 -0700 (Thu, 12 Oct 2006) | 2 lines

optimized softcursor drawing

------------------------------------------------------------------------
r3302 | lillo | 2006-10-12 07:15:15 -0700 (Thu, 12 Oct 2006) | 2 lines

slightly modified software cursor layout to make it nicer

------------------------------------------------------------------------
r3301 | lillo | 2006-10-11 13:06:29 -0700 (Wed, 11 Oct 2006) | 2 lines

Console was still cleared when exiting gfx mode using certain terminals under Linux

------------------------------------------------------------------------
r3300 | lillo | 2006-10-11 12:09:15 -0700 (Wed, 11 Oct 2006) | 2 lines

Call InvalidateRect only if under Win9x

------------------------------------------------------------------------
r3299 | i_am_drv | 2006-10-11 10:25:12 -0700 (Wed, 11 Oct 2006) | 2 lines

fix fb_off_t in Cygwin

------------------------------------------------------------------------
r3298 | lillo | 2006-10-11 06:30:05 -0700 (Wed, 11 Oct 2006) | 2 lines

fixes

------------------------------------------------------------------------
r3297 | lillo | 2006-10-11 06:03:01 -0700 (Wed, 11 Oct 2006) | 2 lines

removed 8192 pixels sprite width limit (thanks to new header)

------------------------------------------------------------------------
r3296 | lillo | 2006-10-11 05:17:00 -0700 (Wed, 11 Oct 2006) | 6 lines

We now have a new default extended sprite header for GET/PUT buffers.
All primitives are compatible with both old-style 4 bytes header and
this new, 32 bytes long one.
Thanks to the new header, we can now store custom sprite pitch, allowing
to align pixel data on the paragraph boundary (16 bytes)

------------------------------------------------------------------------
r3295 | i_am_drv | 2006-10-11 04:04:16 -0700 (Wed, 11 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3294 | i_am_drv | 2006-10-11 03:53:32 -0700 (Wed, 11 Oct 2006) | 2 lines

make FileLen return a longint

------------------------------------------------------------------------
r3292 | i_am_drv | 2006-10-11 03:23:33 -0700 (Wed, 11 Oct 2006) | 2 lines

all file offsets can be 64 bits now (large file support)

------------------------------------------------------------------------
r3291 | lillo | 2006-10-11 02:12:00 -0700 (Wed, 11 Oct 2006) | 3 lines

- New sleep() based screensync implementation for X11
- Enabled MMX Alpha blitter

------------------------------------------------------------------------
r3290 | lillo | 2006-10-11 01:33:09 -0700 (Wed, 11 Oct 2006) | 7 lines

Fixed bug 1573255.
Why in the first place the Linux INPUT code was different from the other
platforms? I guess specialized code was added long ago before the Linux
console was considered reliable enough... Anyway, now all platforms use
the same codebase, but I've left the old specialized code in case it may
be needed again in the future.

------------------------------------------------------------------------
r3289 | v1ctor | 2006-10-10 22:36:37 -0700 (Tue, 10 Oct 2006) | 2 lines

new test

------------------------------------------------------------------------
r3288 | v1ctor | 2006-10-10 22:36:02 -0700 (Tue, 10 Oct 2006) | 2 lines

fixed: implicit clone()'s would be created for structs with string fields that defined explicit clone()'s using unresolved forward types to self as the rhs parameter

------------------------------------------------------------------------
r3287 | v1ctor | 2006-10-10 20:09:46 -0700 (Tue, 10 Oct 2006) | 2 lines

new tests

------------------------------------------------------------------------
r3286 | v1ctor | 2006-10-10 20:09:35 -0700 (Tue, 10 Oct 2006) | 2 lines

fixed: initializers of static or shared instances without a default constructor were not being emitted

------------------------------------------------------------------------
r3285 | v1ctor | 2006-10-10 19:11:35 -0700 (Tue, 10 Oct 2006) | 2 lines

fixed: clone() wasn't being checked if another "let" operator was already defined

------------------------------------------------------------------------
r3284 | v1ctor | 2006-10-10 18:40:22 -0700 (Tue, 10 Oct 2006) | 3 lines

fixed: libfb_ctor.o must also be included in the LD's lib group or
dlopen() will fail

------------------------------------------------------------------------
r3283 | lillo | 2006-10-10 11:02:09 -0700 (Tue, 10 Oct 2006) | 2 lines

removed try at directdraw shaped window support - it just won't make it

------------------------------------------------------------------------
r3282 | lillo | 2006-10-10 08:39:22 -0700 (Tue, 10 Oct 2006) | 2 lines

added alpha channel support for all gfx primitives

------------------------------------------------------------------------
r3281 | lillo | 2006-10-10 08:33:25 -0700 (Tue, 10 Oct 2006) | 2 lines

added alpha channel support for all gfx primitives

------------------------------------------------------------------------
r3280 | v1ctor | 2006-10-10 08:32:55 -0700 (Tue, 10 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3279 | v1ctor | 2006-10-10 08:23:58 -0700 (Tue, 10 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3278 | v1ctor | 2006-10-10 08:22:43 -0700 (Tue, 10 Oct 2006) | 3 lines

internal changes:
- rtInit() and rtEnd() will be called from a private static ctor/dtor in libfb_ctor.o (the latest obj file passed before crtend);

------------------------------------------------------------------------
r3277 | v1ctor | 2006-10-10 07:52:17 -0700 (Tue, 10 Oct 2006) | 2 lines

fixed: FD_* macros

------------------------------------------------------------------------
r3276 | v1ctor | 2006-10-09 22:54:15 -0700 (Mon, 09 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3275 | v1ctor | 2006-10-09 22:13:40 -0700 (Mon, 09 Oct 2006) | 9 lines

reverted:
- the GCC's .ctors/.dtors sections must be used in Linux or global
ctors/dtors defined in FB inside shared libraries won't work;

internal changes:
- all atexit()'s removed from the rtlib;
- rtInit() and rtEnd() will be called from a private static ctor/dtor in
libfb_ctor.o (the latest obj file passed before crtend);

------------------------------------------------------------------------
r3273 | v1ctor | 2006-10-09 10:48:03 -0700 (Mon, 09 Oct 2006) | 2 lines

new example

------------------------------------------------------------------------
r3272 | v1ctor | 2006-10-08 22:15:10 -0700 (Sun, 08 Oct 2006) | 2 lines

fixed: bug #1567823

------------------------------------------------------------------------
r3271 | v1ctor | 2006-10-08 20:23:59 -0700 (Sun, 08 Oct 2006) | 2 lines

fixed: bug #1573122

------------------------------------------------------------------------
r3270 | v1ctor | 2006-10-08 19:51:27 -0700 (Sun, 08 Oct 2006) | 9 lines

reverted: the .fb_ctors/.fb_dtors sections are now again used to invoke
the global constructors/destructors because fb_rtExit() was being called
before the global dtors

(warning: to a chicken-egg problem not occur, compile the compiler first
using the current rtlib, then build the rtlib using these sources and
finally use the newest compiler to compile itself, linking to the latest
rtlib built)

------------------------------------------------------------------------
r3269 | v1ctor | 2006-10-08 19:22:59 -0700 (Sun, 08 Oct 2006) | 4 lines

reverted: the .fb_ctors/.fb_dtors sections are now again used to invoke the global constructors/destructors because fb_rtExit() was being called before the global dtors

(warning: to a chicken-egg problem not occur, compile the compiler first using the current rtlib, then build the rtlib using these sources and finally use the newest compiler to compile itself, linking to the latest rtlib built)

------------------------------------------------------------------------
r3268 | v1ctor | 2006-10-08 18:52:12 -0700 (Sun, 08 Oct 2006) | 2 lines

removed: no access to internal rtlib structures/functions should be allowed

------------------------------------------------------------------------
r3267 | v1ctor | 2006-10-08 18:50:25 -0700 (Sun, 08 Oct 2006) | 2 lines

reverted: some C libraries expect the address of errno, it can't be a function

------------------------------------------------------------------------
r3265 | jeffmarshall | 2006-10-08 07:12:24 -0700 (Sun, 08 Oct 2006) | 2 lines

changed include path for sys/socket.bi

------------------------------------------------------------------------
r3264 | v1ctor | 2006-10-07 21:50:46 -0700 (Sat, 07 Oct 2006) | 2 lines

fixed: the HKEY_* defines were missing

------------------------------------------------------------------------
r3263 | v1ctor | 2006-10-07 18:12:08 -0700 (Sat, 07 Oct 2006) | 2 lines

fixed: duplicated definition

------------------------------------------------------------------------
r3262 | v1ctor | 2006-10-07 17:25:00 -0700 (Sat, 07 Oct 2006) | 2 lines

fixed: ()'s were being used in the initialization, not {}'s

------------------------------------------------------------------------
r3261 | v1ctor | 2006-10-07 11:16:01 -0700 (Sat, 07 Oct 2006) | 2 lines

updated

------------------------------------------------------------------------
r3260 | v1ctor | 2006-10-07 10:50:05 -0700 (Sat, 07 Oct 2006) | 2 lines

added: socket/unistd/netdb/arpa/netinet headers (untested)

------------------------------------------------------------------------
r3259 | v1ctor | 2006-10-07 08:40:07 -0700 (Sat, 07 Oct 2006) | 2 lines

new test

------------------------------------------------------------------------
r3258 | v1ctor | 2006-10-07 08:39:03 -0700 (Sat, 07 Oct 2006) | 2 lines

fixed: foo()->bar = rhs assignments were not being accepted

------------------------------------------------------------------------
r3257 | v1ctor | 2006-10-07 06:40:35 -0700 (Sat, 07 Oct 2006) | 2 lines

fixed: bug #1570460

------------------------------------------------------------------------
r3256 | v1ctor | 2006-10-07 06:20:22 -0700 (Sat, 07 Oct 2006) | 2 lines

fixed: IIF() was not recovering from errors

------------------------------------------------------------------------
r3255 | v1ctor | 2006-10-07 06:17:48 -0700 (Sat, 07 Oct 2006) | 2 lines

fixed: values <= 0 shouldn't be accepted for -maxerr

------------------------------------------------------------------------
r3254 | lillo | 2006-10-05 14:49:54 -0700 (Thu, 05 Oct 2006) | 2 lines

enabled high-intensity background colors in linux console (xterm only)

------------------------------------------------------------------------
r3253 | v1ctor | 2006-10-05 10:38:35 -0700 (Thu, 05 Oct 2006) | 2 lines

fixed: argv[i] can also be NULL

------------------------------------------------------------------------
r3252 | i_am_drv | 2006-10-04 18:44:44 -0700 (Wed, 04 Oct 2006) | 2 lines

don't crash in ctors (before main runs)

------------------------------------------------------------------------
r3251 | i_am_drv | 2006-10-04 17:03:27 -0700 (Wed, 04 Oct 2006) | 2 lines

fixed typos

------------------------------------------------------------------------
r3250 | i_am_drv | 2006-10-04 16:52:36 -0700 (Wed, 04 Oct 2006) | 2 lines

translated from Linux stdint.h

------------------------------------------------------------------------
r3249 | i_am_drv | 2006-10-02 22:01:50 -0700 (Mon, 02 Oct 2006) | 2 lines

added sparc64 and powerpc64 cpu targets

------------------------------------------------------------------------
r3248 | i_am_drv | 2006-10-02 21:09:54 -0700 (Mon, 02 Oct 2006) | 2 lines

fb_gfx_mmx.h is x86-specific

------------------------------------------------------------------------
r3247 | v1ctor | 2006-10-02 21:04:38 -0700 (Mon, 02 Oct 2006) | 2 lines

changed: let 0ms to be passed to fb_Delay( )

------------------------------------------------------------------------
r3246 | i_am_drv | 2006-10-02 21:03:55 -0700 (Mon, 02 Oct 2006) | 2 lines

fixed

------------------------------------------------------------------------
r3245 | i_am_drv | 2006-10-02 21:01:39 -0700 (Mon, 02 Oct 2006) | 2 lines

inline x86 asm for bit rotation moved to a macro (replaced with C on non-x86 platforms)

------------------------------------------------------------------------
r3244 | i_am_drv | 2006-10-02 20:09:10 -0700 (Mon, 02 Oct 2006) | 2 lines

added powerpc64 cpu target

------------------------------------------------------------------------
r3243 | i_am_drv | 2006-10-02 19:40:58 -0700 (Mon, 02 Oct 2006) | 2 lines

added sparc64 cpu target

------------------------------------------------------------------------
r3242 | i_am_drv | 2006-10-02 17:07:20 -0700 (Mon, 02 Oct 2006) | 2 lines

more changes for large file support

------------------------------------------------------------------------
r3241 | v1ctor | 2006-10-01 12:20:57 -0700 (Sun, 01 Oct 2006) | 2 lines

fixed: the export section wasn't including functions declared inside namespaces

------------------------------------------------------------------------
r3240 | v1ctor | 2006-09-30 18:42:32 -0700 (Sat, 30 Sep 2006) | 2 lines

new tests (sent by stylin)

------------------------------------------------------------------------
r3239 | v1ctor | 2006-09-30 12:51:23 -0700 (Sat, 30 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3238 | v1ctor | 2006-09-30 11:54:10 -0700 (Sat, 30 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3237 | v1ctor | 2006-09-30 11:53:31 -0700 (Sat, 30 Sep 2006) | 2 lines

fixed: field arrays from dereferenced UDT's were causing an ICE when passed by descriptor to functions

------------------------------------------------------------------------
r3236 | v1ctor | 2006-09-30 11:17:36 -0700 (Sat, 30 Sep 2006) | 2 lines

fixed: error recovery was causing an ICE

------------------------------------------------------------------------
r3235 | v1ctor | 2006-09-30 11:07:55 -0700 (Sat, 30 Sep 2006) | 2 lines

fixed: temporary instances must be destroyed right after their use

------------------------------------------------------------------------
r3234 | v1ctor | 2006-09-30 11:04:26 -0700 (Sat, 30 Sep 2006) | 2 lines

changed: new to new_

------------------------------------------------------------------------
r3233 | v1ctor | 2006-09-30 11:03:41 -0700 (Sat, 30 Sep 2006) | 2 lines

new tests

------------------------------------------------------------------------
r3232 | v1ctor | 2006-09-30 11:01:33 -0700 (Sat, 30 Sep 2006) | 2 lines

added: real10 library written by srvaldez

------------------------------------------------------------------------
r3231 | v1ctor | 2006-09-30 10:58:27 -0700 (Sat, 30 Sep 2006) | 2 lines

changed: new to new_

------------------------------------------------------------------------
r3230 | v1ctor | 2006-09-30 10:54:38 -0700 (Sat, 30 Sep 2006) | 2 lines

changed: Delete() field renamed to Del()

------------------------------------------------------------------------
r3229 | jeffmarshall | 2006-09-30 08:31:46 -0700 (Sat, 30 Sep 2006) | 2 lines

Updated: split web/sql loader, bug fixes for syntax highlighter, bug fixes for http forms

------------------------------------------------------------------------
r3228 | jeffmarshall | 2006-09-30 08:29:09 -0700 (Sat, 30 Sep 2006) | 2 lines

Updated: camel cased keywords (with help from DrV)

------------------------------------------------------------------------
r3227 | jeffmarshall | 2006-09-30 08:18:28 -0700 (Sat, 30 Sep 2006) | 2 lines

Changed: patch for camel cased keywords

------------------------------------------------------------------------
r3226 | jeffmarshall | 2006-09-30 08:17:55 -0700 (Sat, 30 Sep 2006) | 2 lines

Changed: camel cased keywords

------------------------------------------------------------------------
r3225 | v1ctor | 2006-09-30 00:04:05 -0700 (Sat, 30 Sep 2006) | 2 lines

fixed: &str.len was being passed to FB_STRSIZE(), instead of &str

------------------------------------------------------------------------
r3224 | i_am_drv | 2006-09-28 09:40:47 -0700 (Thu, 28 Sep 2006) | 2 lines

byte ptr -> zstring ptr

------------------------------------------------------------------------
r3223 | i_am_drv | 2006-09-27 23:56:24 -0700 (Wed, 27 Sep 2006) | 2 lines

only try to use MMX on platforms that support it

------------------------------------------------------------------------
r3222 | i_am_drv | 2006-09-27 23:28:35 -0700 (Wed, 27 Sep 2006) | 2 lines

move architecture-specific code to its own directory

------------------------------------------------------------------------
r3221 | i_am_drv | 2006-09-27 23:19:32 -0700 (Wed, 27 Sep 2006) | 2 lines

move architecture-specific code to its own directory

------------------------------------------------------------------------
r3220 | i_am_drv | 2006-09-27 23:00:10 -0700 (Wed, 27 Sep 2006) | 2 lines

move MMX code to architecture-specific directory

------------------------------------------------------------------------
r3219 | i_am_drv | 2006-09-27 22:43:56 -0700 (Wed, 27 Sep 2006) | 2 lines

move MMX code to architecture-specific directory

------------------------------------------------------------------------
r3218 | i_am_drv | 2006-09-27 22:43:19 -0700 (Wed, 27 Sep 2006) | 2 lines

lock data accessed in timer ISR

------------------------------------------------------------------------
r3217 | v1ctor | 2006-09-26 09:23:48 -0700 (Tue, 26 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3216 | v1ctor | 2006-09-26 09:22:49 -0700 (Tue, 26 Sep 2006) | 2 lines

fixed: by default, methods couldn't be overloaded

------------------------------------------------------------------------
r3215 | v1ctor | 2006-09-25 17:36:29 -0700 (Mon, 25 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3214 | v1ctor | 2006-09-25 17:35:14 -0700 (Mon, 25 Sep 2006) | 2 lines

fixed: USING was not checking if the top parent was a TYPE or CLASS when importing and removing symbols

------------------------------------------------------------------------
r3213 | v1ctor | 2006-09-24 10:47:18 -0700 (Sun, 24 Sep 2006) | 2 lines

fixed: duplicated definitions

------------------------------------------------------------------------
r3212 | v1ctor | 2006-09-23 20:55:01 -0700 (Sat, 23 Sep 2006) | 2 lines

fixed: ICE when invalid types were used as iterators in FOR

------------------------------------------------------------------------
r3211 | v1ctor | 2006-09-23 19:15:01 -0700 (Sat, 23 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3210 | v1ctor | 2006-09-23 14:31:30 -0700 (Sat, 23 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3209 | v1ctor | 2006-09-23 14:31:08 -0700 (Sat, 23 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3208 | v1ctor | 2006-09-23 14:30:21 -0700 (Sat, 23 Sep 2006) | 4 lines

fixed:
- auto-coercion of UDT's was not checking for invalid types;
- fields of UDT's returned from functions couldn't be accessed directly;

------------------------------------------------------------------------
r3207 | v1ctor | 2006-09-23 09:48:15 -0700 (Sat, 23 Sep 2006) | 2 lines

fixed: functions declared outside the original namespace were not being checked

------------------------------------------------------------------------
r3206 | v1ctor | 2006-09-22 23:50:42 -0700 (Fri, 22 Sep 2006) | 2 lines

fixed: double free when the same global operators were declared in different namespaces

------------------------------------------------------------------------
r3205 | v1ctor | 2006-09-22 14:33:35 -0700 (Fri, 22 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3204 | v1ctor | 2006-09-22 14:25:43 -0700 (Fri, 22 Sep 2006) | 2 lines

added: NT DDK example, written by voodooattack

------------------------------------------------------------------------
r3203 | v1ctor | 2006-09-22 14:25:36 -0700 (Fri, 22 Sep 2006) | 2 lines

added: the NT DDK headers, sent by voodooattack

------------------------------------------------------------------------
r3202 | v1ctor | 2006-09-22 13:51:28 -0700 (Fri, 22 Sep 2006) | 2 lines

changed: if no default constructor is defined and other ctors are explicitly declared, no default instantiation will be allowed

------------------------------------------------------------------------
r3201 | v1ctor | 2006-09-22 10:49:34 -0700 (Fri, 22 Sep 2006) | 2 lines

fixed: TYPE() can  now be used to invoke the default constructors

------------------------------------------------------------------------
r3200 | v1ctor | 2006-09-22 10:11:25 -0700 (Fri, 22 Sep 2006) | 2 lines

reverted: scope rules for implicit variables will still the same in as 0.16 for -lang deprecated

------------------------------------------------------------------------
r3199 | v1ctor | 2006-09-22 09:16:50 -0700 (Fri, 22 Sep 2006) | 2 lines

changed: offsetof() can now be used inside inline asm blocks - patch by cha0s

------------------------------------------------------------------------
r3198 | v1ctor | 2006-09-22 07:53:45 -0700 (Fri, 22 Sep 2006) | 2 lines

fixed: the alignment of negative numbers was off by one if the sign char wasn't passed explicitly

------------------------------------------------------------------------
r3197 | v1ctor | 2006-09-22 00:00:41 -0700 (Fri, 22 Sep 2006) | 2 lines

fixed: auto-coercion was selecting a ctor() if any was declared

------------------------------------------------------------------------
r3196 | i_am_drv | 2006-09-21 23:15:33 -0700 (Thu, 21 Sep 2006) | 2 lines

change errno on win32 to a private function

------------------------------------------------------------------------
r3195 | v1ctor | 2006-09-21 21:03:06 -0700 (Thu, 21 Sep 2006) | 2 lines

fixed: couldn't be compiled by the latest testing release because the '\"'s

------------------------------------------------------------------------
r3194 | v1ctor | 2006-09-21 19:55:48 -0700 (Thu, 21 Sep 2006) | 2 lines

fixed: redundant STR_LOCK()'s removed

------------------------------------------------------------------------
r3193 | v1ctor | 2006-09-21 14:58:55 -0700 (Thu, 21 Sep 2006) | 2 lines

added: support for methods in TYPE's

------------------------------------------------------------------------
r3192 | jeffmarshall | 2006-09-21 10:04:25 -0700 (Thu, 21 Sep 2006) | 2 lines

added: new keywords

------------------------------------------------------------------------
r3191 | jeffmarshall | 2006-09-21 10:03:42 -0700 (Thu, 21 Sep 2006) | 2 lines

small change: for latest fbc compatibility

------------------------------------------------------------------------
r3189 | v1ctor | 2006-09-20 19:28:15 -0700 (Wed, 20 Sep 2006) | 2 lines

added: the & and &= operators can now be overloaded too

------------------------------------------------------------------------
r3188 | v1ctor | 2006-09-20 19:28:12 -0700 (Wed, 20 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3187 | v1ctor | 2006-09-20 13:42:51 -0700 (Wed, 20 Sep 2006) | 2 lines

added: WRITE and PRINT USING will try to convert the UDT's using the casting operators

------------------------------------------------------------------------
r3186 | v1ctor | 2006-09-20 13:41:19 -0700 (Wed, 20 Sep 2006) | 2 lines

added: the "op=" operators can declared just as "op"

------------------------------------------------------------------------
r3185 | v1ctor | 2006-09-20 13:36:24 -0700 (Wed, 20 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3184 | v1ctor | 2006-09-20 13:18:36 -0700 (Wed, 20 Sep 2006) | 2 lines

new tests

------------------------------------------------------------------------
r3183 | v1ctor | 2006-09-20 10:57:31 -0700 (Wed, 20 Sep 2006) | 2 lines

small fixes

------------------------------------------------------------------------
r3182 | v1ctor | 2006-09-20 10:55:25 -0700 (Wed, 20 Sep 2006) | 2 lines

new tests

------------------------------------------------------------------------
r3181 | v1ctor | 2006-09-20 10:26:42 -0700 (Wed, 20 Sep 2006) | 4 lines

fixed: bitfield initializations in constructors;

added: fields in UNION's are now initialized only once;

------------------------------------------------------------------------
r3180 | i_am_drv | 2006-09-19 20:01:42 -0700 (Tue, 19 Sep 2006) | 2 lines

file size and position data types changed to fb_off_t typedef

------------------------------------------------------------------------
r3179 | v1ctor | 2006-09-19 19:52:05 -0700 (Tue, 19 Sep 2006) | 2 lines

added: missing defines

------------------------------------------------------------------------
r3178 | v1ctor | 2006-09-19 19:28:20 -0700 (Tue, 19 Sep 2006) | 6 lines

fixed:
- len() when used with global arrays of strings, passed without indexes, was resulting in undefined symbols;
- invalid pointer initializations were not being checked;
- the string concatenation optimization was not checking if the destine string was used more than once in the rhs expression;
- illegal subscripts in static arrays were not being checked;

------------------------------------------------------------------------
r3177 | v1ctor | 2006-09-19 17:47:40 -0700 (Tue, 19 Sep 2006) | 2 lines

fixed: bug #1555661

------------------------------------------------------------------------
r3176 | i_am_drv | 2006-09-19 17:24:26 -0700 (Tue, 19 Sep 2006) | 2 lines

rename sqlite_test to sqlite3_test

------------------------------------------------------------------------
r3175 | i_am_drv | 2006-09-19 17:20:53 -0700 (Tue, 19 Sep 2006) | 2 lines

add sqlite2 test

------------------------------------------------------------------------
r3174 | i_am_drv | 2006-09-19 17:18:22 -0700 (Tue, 19 Sep 2006) | 2 lines

remove Option Explicit

------------------------------------------------------------------------
r3173 | i_am_drv | 2006-09-19 17:13:39 -0700 (Tue, 19 Sep 2006) | 2 lines

add LIBRARY directive

------------------------------------------------------------------------
r3172 | i_am_drv | 2006-09-19 16:47:23 -0700 (Tue, 19 Sep 2006) | 2 lines

sqlite_callback: byte ptr ptr -> zstring ptr ptr

------------------------------------------------------------------------
r3171 | i_am_drv | 2006-09-19 14:12:57 -0700 (Tue, 19 Sep 2006) | 2 lines

mouse wheel should be 0 if not present

------------------------------------------------------------------------
r3170 | v1ctor | 2006-09-19 13:06:57 -0700 (Tue, 19 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3169 | v1ctor | 2006-09-19 13:05:49 -0700 (Tue, 19 Sep 2006) | 4 lines

fixed: padding of UDT's was incorrect if only byte and/or short fields were present

added: auto-coercion of UDT's that define casting operators when doing binary operations

------------------------------------------------------------------------
r3168 | v1ctor | 2006-09-19 13:05:45 -0700 (Tue, 19 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3167 | v1ctor | 2006-09-18 22:14:15 -0700 (Mon, 18 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3166 | v1ctor | 2006-09-18 22:13:11 -0700 (Mon, 18 Sep 2006) | 2 lines

fixed: auto-coercion was allowed in cast()

------------------------------------------------------------------------
r3165 | v1ctor | 2006-09-18 15:56:49 -0700 (Mon, 18 Sep 2006) | 2 lines

fixed: auto-coercion for constructors and cast() operators would give false matches

------------------------------------------------------------------------
r3164 | v1ctor | 2006-09-18 13:55:12 -0700 (Mon, 18 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3163 | v1ctor | 2006-09-18 13:53:36 -0700 (Mon, 18 Sep 2006) | 4 lines

changed: byval structs returned in registers can now be passed byref to other functions

fixed: constructor auto-coercion

------------------------------------------------------------------------
r3162 | v1ctor | 2006-09-18 13:45:09 -0700 (Mon, 18 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3161 | v1ctor | 2006-09-18 12:15:29 -0700 (Mon, 18 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3160 | v1ctor | 2006-09-18 12:11:37 -0700 (Mon, 18 Sep 2006) | 2 lines

new tests

------------------------------------------------------------------------
r3159 | v1ctor | 2006-09-18 10:28:10 -0700 (Mon, 18 Sep 2006) | 2 lines

added: str(), wstr(), abs(), fix(), int() and sgn() overloading

------------------------------------------------------------------------
r3158 | v1ctor | 2006-09-17 23:40:47 -0700 (Sun, 17 Sep 2006) | 2 lines

fixed: auto-coercion overloading for arguments of overloaded functions

------------------------------------------------------------------------
r3157 | v1ctor | 2006-09-17 14:35:53 -0700 (Sun, 17 Sep 2006) | 2 lines

fixed: debugging info was including methods in the UDT description

------------------------------------------------------------------------
r3156 | v1ctor | 2006-09-17 14:18:12 -0700 (Sun, 17 Sep 2006) | 2 lines

fixed: local method variables couldn't be dups of data members

------------------------------------------------------------------------
r3155 | v1ctor | 2006-09-17 14:13:10 -0700 (Sun, 17 Sep 2006) | 2 lines

fixed: error recovery

------------------------------------------------------------------------
r3154 | v1ctor | 2006-09-17 13:56:09 -0700 (Sun, 17 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3153 | v1ctor | 2006-09-17 13:55:39 -0700 (Sun, 17 Sep 2006) | 2 lines

fixed: auto-coercion overloading for unary operators

------------------------------------------------------------------------
r3152 | v1ctor | 2006-09-17 11:48:23 -0700 (Sun, 17 Sep 2006) | 2 lines

fixed: field->initree wasn't being set to NULL

------------------------------------------------------------------------
r3151 | v1ctor | 2006-09-17 00:25:05 -0700 (Sun, 17 Sep 2006) | 4 lines

fixed: dtors of temporary variables were not being called inside scope blocks

added: implicit casting op overload check for unary ops and params

------------------------------------------------------------------------
r3150 | v1ctor | 2006-09-16 23:37:27 -0700 (Sat, 16 Sep 2006) | 2 lines

added: missing functions

------------------------------------------------------------------------
r3149 | v1ctor | 2006-09-16 21:23:08 -0700 (Sat, 16 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3148 | v1ctor | 2006-09-16 20:42:00 -0700 (Sat, 16 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3147 | v1ctor | 2006-09-16 17:32:22 -0700 (Sat, 16 Sep 2006) | 2 lines

added: constructor and destructor

------------------------------------------------------------------------
r3146 | v1ctor | 2006-09-16 17:18:41 -0700 (Sat, 16 Sep 2006) | 2 lines

added: constructor and destructor

------------------------------------------------------------------------
r3145 | jeffmarshall | 2006-09-16 17:14:29 -0700 (Sat, 16 Sep 2006) | 2 lines

fixed : linux printer driver causing SIGPIPE on close

------------------------------------------------------------------------
r3144 | v1ctor | 2006-09-16 16:43:09 -0700 (Sat, 16 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3143 | v1ctor | 2006-09-16 16:41:11 -0700 (Sat, 16 Sep 2006) | 2 lines

added: new tests

------------------------------------------------------------------------
r3142 | v1ctor | 2006-09-16 16:40:32 -0700 (Sat, 16 Sep 2006) | 2 lines

added: constructor and destructor support for structs

------------------------------------------------------------------------
r3140 | v1ctor | 2006-09-16 16:33:27 -0700 (Sat, 16 Sep 2006) | 2 lines

added: constructor and destructor support for structs

------------------------------------------------------------------------
r3139 | v1ctor | 2006-09-16 16:26:02 -0700 (Sat, 16 Sep 2006) | 2 lines

added: constructor and destructor

------------------------------------------------------------------------
r3138 | jeffmarshall | 2006-09-15 10:49:25 -0700 (Fri, 15 Sep 2006) | 2 lines

fixed : popen with "rt" and "wt" not supported on all libc implementations.  reverted to "r" and "w" only for text mode.

------------------------------------------------------------------------
r3137 | jeffmarshall | 2006-09-13 09:36:34 -0700 (Wed, 13 Sep 2006) | 2 lines

small change : syntax error on linux

------------------------------------------------------------------------
r3136 | v1ctor | 2006-09-11 19:08:18 -0700 (Mon, 11 Sep 2006) | 2 lines

new example

------------------------------------------------------------------------
r3135 | i_am_drv | 2006-09-09 14:13:24 -0700 (Sat, 09 Sep 2006) | 2 lines

change Makefile.mingw to avoid using cmd.exe/command.com mkdir; make STDCALL fix MSVC-specific

------------------------------------------------------------------------
r3134 | i_am_drv | 2006-09-09 13:26:10 -0700 (Sat, 09 Sep 2006) | 2 lines

fixed portio driver build with MS DDK (fix by voodooattack)

------------------------------------------------------------------------
r3133 | jeffmarshall | 2006-09-09 01:57:51 -0700 (Sat, 09 Sep 2006) | 2 lines

moved deprecated test

------------------------------------------------------------------------
r3132 | jeffmarshall | 2006-09-09 01:49:01 -0700 (Sat, 09 Sep 2006) | 2 lines

new test

------------------------------------------------------------------------
r3131 | jeffmarshall | 2006-09-09 01:43:01 -0700 (Sat, 09 Sep 2006) | 2 lines

moved : deprecated tests

------------------------------------------------------------------------
r3130 | jeffmarshall | 2006-09-09 01:41:56 -0700 (Sat, 09 Sep 2006) | 2 lines

updated : allow lang specific tests

------------------------------------------------------------------------
r3129 | jeffmarshall | 2006-09-07 23:37:27 -0700 (Thu, 07 Sep 2006) | 2 lines

updated : new keywords

------------------------------------------------------------------------
r3128 | jeffmarshall | 2006-09-07 23:33:37 -0700 (Thu, 07 Sep 2006) | 2 lines

updated : small change for fbhelp formatting

------------------------------------------------------------------------
r3127 | v1ctor | 2006-09-07 20:27:32 -0700 (Thu, 07 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3126 | v1ctor | 2006-09-07 19:41:39 -0700 (Thu, 07 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3125 | v1ctor | 2006-09-07 19:01:39 -0700 (Thu, 07 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3124 | v1ctor | 2006-09-07 18:08:54 -0700 (Thu, 07 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3123 | jeffmarshall | 2006-09-07 12:58:15 -0700 (Thu, 07 Sep 2006) | 2 lines

added : missing fake/fbcu.bi

------------------------------------------------------------------------
r3122 | jeffmarshall | 2006-09-07 12:55:36 -0700 (Thu, 07 Sep 2006) | 2 lines

added : missing bmk files

------------------------------------------------------------------------
r3121 | jeffmarshall | 2006-09-07 12:51:06 -0700 (Thu, 07 Sep 2006) | 2 lines

added : missing pretest files

------------------------------------------------------------------------
r3120 | jeffmarshall | 2006-09-07 12:42:25 -0700 (Thu, 07 Sep 2006) | 2 lines

added : missing pretest files

------------------------------------------------------------------------
r3119 | v1ctor | 2006-09-07 12:10:01 -0700 (Thu, 07 Sep 2006) | 2 lines

changed: /pretest dir missing

------------------------------------------------------------------------
r3118 | v1ctor | 2006-09-04 09:06:54 -0700 (Mon, 04 Sep 2006) | 2 lines

updated

------------------------------------------------------------------------
r3117 | jeffmarshall | 2006-09-02 05:26:20 -0700 (Sat, 02 Sep 2006) | 2 lines

small change : typo

------------------------------------------------------------------------
r3116 | jeffmarshall | 2006-09-02 04:23:19 -0700 (Sat, 02 Sep 2006) | 2 lines

changed : new makefiles for testing to support cunit and other methods

------------------------------------------------------------------------
r3115 | v1ctor | 2006-08-31 14:52:10 -0700 (Thu, 31 Aug 2006) | 2 lines

updated

------------------------------------------------------------------------
r3114 | v1ctor | 2006-08-31 14:49:53 -0700 (Thu, 31 Aug 2006) | 2 lines

changed: thread*, cond* and mutex* ids are now "any ptr", not integers, to allow easy and safe migration to 64-bit platforms in future

------------------------------------------------------------------------
r3113 | lillo | 2006-08-29 14:13:25 -0700 (Tue, 29 Aug 2006) | 2 lines

Pixels with a zero alpha value in PUT ALPHA mode are now not drawn at all. As a drawback, fully bright pixels are not drawn fully bright anymore (RGBA(255,255,255,255) will become RGBA(254,254,254,254)) to maintain speed.

------------------------------------------------------------------------
r3112 | lillo | 2006-08-29 03:14:52 -0700 (Tue, 29 Aug 2006) | 2 lines

update

------------------------------------------------------------------------
r3111 | lillo | 2006-08-29 03:12:08 -0700 (Tue, 29 Aug 2006) | 2 lines

- 'DRAW "X" & @cmd' did reset coordinates before and after cmd execution to the starting position before the whole DRAW statement was run.

------------------------------------------------------------------------
r3110 | lillo | 2006-08-29 02:54:35 -0700 (Tue, 29 Aug 2006) | 2 lines

Fixed fullscreen bug in X11 gfx driver

------------------------------------------------------------------------
r3109 | i_am_drv | 2006-08-28 00:05:51 -0700 (Mon, 28 Aug 2006) | 2 lines

updated

------------------------------------------------------------------------
r3108 | v1ctor | 2006-08-27 11:37:56 -0700 (Sun, 27 Aug 2006) | 2 lines

fixed: the EQV operator had the same mangled name as the = binary op

------------------------------------------------------------------------
r3107 | v1ctor | 2006-08-27 09:47:31 -0700 (Sun, 27 Aug 2006) | 2 lines

updated

------------------------------------------------------------------------
r3106 | v1ctor | 2006-08-27 09:44:42 -0700 (Sun, 27 Aug 2006) | 2 lines

fixed: pointers were being allowed as array indexes

------------------------------------------------------------------------
r3105 | v1ctor | 2006-08-26 07:34:21 -0700 (Sat, 26 Aug 2006) | 2 lines

reverted: _wcstoi64 and _wcstoui64 are only present in msvcrt 7.x or above

------------------------------------------------------------------------
r3104 | v1ctor | 2006-08-25 09:31:35 -0700 (Fri, 25 Aug 2006) | 2 lines

fixed: errors in English, patches written by cha0s

------------------------------------------------------------------------
r3103 | v1ctor | 2006-08-25 07:33:19 -0700 (Fri, 25 Aug 2006) | 2 lines

cosmetic internal changes

------------------------------------------------------------------------
r3102 | v1ctor | 2006-08-25 07:31:28 -0700 (Fri, 25 Aug 2006) | 2 lines

new test

------------------------------------------------------------------------
r3101 | v1ctor | 2006-08-25 07:31:27 -0700 (Fri, 25 Aug 2006) | 2 lines

fixed: bug #1546423

------------------------------------------------------------------------
r3100 | jeffmarshall | 2006-08-24 19:31:10 -0700 (Thu, 24 Aug 2006) | 2 lines

small change

------------------------------------------------------------------------
r3099 | v1ctor | 2006-08-24 19:30:10 -0700 (Thu, 24 Aug 2006) | 2 lines

changed: tests moved to /deprecated

------------------------------------------------------------------------
r3098 | v1ctor | 2006-08-24 19:28:25 -0700 (Thu, 24 Aug 2006) | 2 lines

changed: tests re-implemented using CUnit, by stylin

------------------------------------------------------------------------
r3097 | jeffmarshall | 2006-08-24 14:34:37 -0700 (Thu, 24 Aug 2006) | 2 lines

changed: GetJoystick supports axis #7 and #8

------------------------------------------------------------------------
r3096 | v1ctor | 2006-08-24 14:20:57 -0700 (Thu, 24 Aug 2006) | 2 lines

-Wall fixes

------------------------------------------------------------------------
r3095 | v1ctor | 2006-08-24 14:20:14 -0700 (Thu, 24 Aug 2006) | 2 lines

added: wcstoll and wcstoull configure checks

------------------------------------------------------------------------
r3094 | v1ctor | 2006-08-24 14:07:41 -0700 (Thu, 24 Aug 2006) | 2 lines

-Wall fixes

------------------------------------------------------------------------
r3093 | v1ctor | 2006-08-24 12:40:49 -0700 (Thu, 24 Aug 2006) | 2 lines

-Wall fixes

------------------------------------------------------------------------
r3092 | v1ctor | 2006-08-24 12:28:04 -0700 (Thu, 24 Aug 2006) | 2 lines

-Wall fixes

------------------------------------------------------------------------
r3091 | v1ctor | 2006-08-24 11:33:00 -0700 (Thu, 24 Aug 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r3090 | v1ctor | 2006-08-24 10:58:56 -0700 (Thu, 24 Aug 2006) | 3 lines

fixed: '.' without '#' around should be printed as an ordinary
character

------------------------------------------------------------------------
r3089 | jeffmarshall | 2006-08-24 10:51:50 -0700 (Thu, 24 Aug 2006) | 2 lines

updated: allow compilation with '-w pedantic'

------------------------------------------------------------------------
r3088 | v1ctor | 2006-08-24 10:22:34 -0700 (Thu, 24 Aug 2006) | 2 lines

fixed: info was being passed to get_attrib() before calling stat()

------------------------------------------------------------------------
r3087 | v1ctor | 2006-08-24 06:49:30 -0700 (Thu, 24 Aug 2006) | 2 lines

reverted: the mutex is obligatory, and pthreads should take care of the dead-locks (the locked mutex is atomically unlocked when pthread_cond_wait() is called)

------------------------------------------------------------------------
r3086 | v1ctor | 2006-08-23 20:20:18 -0700 (Wed, 23 Aug 2006) | 2 lines

fixed: a deadlock could happen if the mutex was held in wait() and another thread was restarted before the call to pthread's wait(); if the restarted thread call signal(), it will be locked because the mutex held by wait(), and phtread's signal() won't ever be called; the thread holding the mutex will be restarted and the call to pthread's wait() will never return

------------------------------------------------------------------------
r3085 | v1ctor | 2006-08-23 19:42:30 -0700 (Wed, 23 Aug 2006) | 2 lines

changed: *TSTR are now mapped to [w|z]string ptr

------------------------------------------------------------------------
r3084 | v1ctor | 2006-08-22 20:33:57 -0700 (Tue, 22 Aug 2006) | 2 lines

fixed: "ushort" was being re-defined

------------------------------------------------------------------------
r3083 | jeffmarshall | 2006-08-22 15:02:10 -0700 (Tue, 22 Aug 2006) | 2 lines

updated: put # on random mode files returns error if data size does not match open record size

------------------------------------------------------------------------
r3082 | jeffmarshall | 2006-08-22 15:00:56 -0700 (Tue, 22 Aug 2006) | 2 lines

updated: get # on random mode files returns error if data size does not match open record size

------------------------------------------------------------------------
r3081 | v1ctor | 2006-08-22 12:36:59 -0700 (Tue, 22 Aug 2006) | 2 lines

header replaced by a machine translated version

------------------------------------------------------------------------
r3080 | v1ctor | 2006-08-22 10:41:57 -0700 (Tue, 22 Aug 2006) | 2 lines

changed: handle->putback_size will be checked by fb_FileEofEx()

------------------------------------------------------------------------
r3079 | v1ctor | 2006-08-22 09:47:48 -0700 (Tue, 22 Aug 2006) | 2 lines

updated

------------------------------------------------------------------------
r3078 | v1ctor | 2006-08-22 09:11:26 -0700 (Tue, 22 Aug 2006) | 2 lines

fixed: bug if absolute paths were passed in cmd-line (patch written by cha0s)

------------------------------------------------------------------------
r3077 | v1ctor | 2006-08-21 20:02:50 -0700 (Mon, 21 Aug 2006) | 2 lines

small fixes

------------------------------------------------------------------------
r3076 | v1ctor | 2006-08-21 19:57:36 -0700 (Mon, 21 Aug 2006) | 2 lines

updated

------------------------------------------------------------------------
r3075 | v1ctor | 2006-08-21 19:57:10 -0700 (Mon, 21 Aug 2006) | 2 lines

fixed: (*udtptr).field wasn't allowed

------------------------------------------------------------------------
r3074 | v1ctor | 2006-08-21 19:56:10 -0700 (Mon, 21 Aug 2006) | 2 lines

new test

------------------------------------------------------------------------
r3073 | v1ctor | 2006-08-21 18:35:39 -0700 (Mon, 21 Aug 2006) | 2 lines

fixed: prototypes of external functions inside namespaces

------------------------------------------------------------------------
r3072 | v1ctor | 2006-08-21 18:34:56 -0700 (Mon, 21 Aug 2006) | 2 lines

updated

------------------------------------------------------------------------
r3071 | v1ctor | 2006-08-21 18:32:53 -0700 (Mon, 21 Aug 2006) | 2 lines

fixed: bug #1538884 (Intrinsic string-modifying subs allow constant strings)

------------------------------------------------------------------------
r3070 | v1ctor | 2006-08-21 18:22:41 -0700 (Mon, 21 Aug 2006) | 2 lines

added: DLL hot-linking (voodooattack's suggestion), but disabled by now, LD won't resolve _foo@x to _foo

------------------------------------------------------------------------
r3069 | v1ctor | 2006-08-21 18:20:18 -0700 (Mon, 21 Aug 2006) | 2 lines

changed: even with an explicit alias, symbols will be mangled if declared inside a namespace (v1c)

------------------------------------------------------------------------
r3068 | v1ctor | 2006-08-21 17:34:21 -0700 (Mon, 21 Aug 2006) | 2 lines

changed: tests re-implemented using CUnit, by stylin

------------------------------------------------------------------------
r3067 | jeffmarshall | 2006-08-21 17:27:58 -0700 (Mon, 21 Aug 2006) | 2 lines

fixed: reference to removed fb_rterr.h - is now fb_error.h

------------------------------------------------------------------------
r3066 | v1ctor | 2006-08-21 16:01:31 -0700 (Mon, 21 Aug 2006) | 2 lines

changed: tests re-implemented using CUnit, by stylin

------------------------------------------------------------------------
r3065 | v1ctor | 2006-08-21 16:01:15 -0700 (Mon, 21 Aug 2006) | 2 lines

small fix

------------------------------------------------------------------------
r3064 | v1ctor | 2006-08-21 09:28:45 -0700 (Mon, 21 Aug 2006) | 2 lines

added: DLL hot-linking (voodooattack's suggestion), disabled by now, see the FIXME's

------------------------------------------------------------------------
r3063 | v1ctor | 2006-08-21 09:27:27 -0700 (Mon, 21 Aug 2006) | 2 lines

updated

------------------------------------------------------------------------
r3062 | v1ctor | 2006-08-21 08:32:39 -0700 (Mon, 21 Aug 2006) | 2 lines

added: the __PATH__ intrinsic define, that returns the __FILE__'s path (patch sent my cha0s)

------------------------------------------------------------------------
r3061 | v1ctor | 2006-08-20 21:43:45 -0700 (Sun, 20 Aug 2006) | 7 lines

reverted:
- functions can't be by default overloaded in -lang fb mode or the g++ name mangling would always be used;

fixed:
- check for implicit cast operator overload in PRINT;
- "as any" parameters and var-args functions could be declared as OVERLOAD if they were the first to be defined;

------------------------------------------------------------------------
r3060 | v1ctor | 2006-08-20 20:12:23 -0700 (Sun, 20 Aug 2006) | 8 lines

added:
- warning message when the parameters list length is over 256 bytes;
- check for "byref as any" parameters in overloaded functions;
- in -lang fb mode, all functions can be overloaded by default;

fixed:
- symbol suffixes check inside the pre-processor conditional blocks;

------------------------------------------------------------------------
r3059 | v1ctor | 2006-08-20 20:05:44 -0700 (Sun, 20 Aug 2006) | 2 lines

internal changes: DATA's to array initializers

------------------------------------------------------------------------
r3058 | v1ctor | 2006-08-19 19:15:50 -0700 (Sat, 19 Aug 2006) | 2 lines

added: warning message in pedantic mode if the parameter length is above 16 bytes

------------------------------------------------------------------------
r3057 | v1ctor | 2006-08-19 19:01:53 -0700 (Sat, 19 Aug 2006) | 2 lines

new test

------------------------------------------------------------------------
r3056 | v1ctor | 2006-08-19 18:45:29 -0700 (Sat, 19 Aug 2006) | 2 lines

updated

------------------------------------------------------------------------
r3055 | v1ctor | 2006-08-19 18:45:06 -0700 (Sat, 19 Aug 2006) | 2 lines

fixed: lvalue op= rhs had side-effects if functions were present in lvalue

------------------------------------------------------------------------
r3054 | jeffmarshall | 2006-08-19 17:30:26 -0700 (Sat, 19 Aug 2006) | 2 lines

updated:

------------------------------------------------------------------------
r3053 | jeffmarshall | 2006-08-19 17:24:49 -0700 (Sat, 19 Aug 2006) | 2 lines

updated: compatibility with -lang fb

------------------------------------------------------------------------
r3052 | v1ctor | 2006-08-19 14:55:51 -0700 (Sat, 19 Aug 2006) | 2 lines

internal changes: DATA's to array initializers

------------------------------------------------------------------------
r3051 | v1ctor | 2006-08-19 14:47:11 -0700 (Sat, 19 Aug 2006) | 2 lines

removed: dependency on the gfxlib (see the FIXME's)

------------------------------------------------------------------------
r3050 | v1ctor | 2006-08-19 14:42:23 -0700 (Sat, 19 Aug 2006) | 2 lines

changed: the rtlibx functions were moved back to rtlib

------------------------------------------------------------------------
r3049 | jeffmarshall | 2006-08-19 08:43:48 -0700 (Sat, 19 Aug 2006) | 2 lines

added: new test for intrinsic defines

------------------------------------------------------------------------
r3048 | jeffmarshall | 2006-08-19 08:40:47 -0700 (Sat, 19 Aug 2006) | 2 lines

fixed: __FB_VER_* was returning internal variable name, not value

------------------------------------------------------------------------
r3047 | v1ctor | 2006-08-19 07:32:48 -0700 (Sat, 19 Aug 2006) | 2 lines

fixed: ref to self was being allowed in multi-list mode

------------------------------------------------------------------------
r3046 | v1ctor | 2006-08-18 11:57:41 -0700 (Fri, 18 Aug 2006) | 2 lines

changed: the rtlibx functions were moved back to rtlib

------------------------------------------------------------------------
r3045 | v1ctor | 2006-08-18 11:50:33 -0700 (Fri, 18 Aug 2006) | 2 lines

updated: -lang changes

------------------------------------------------------------------------
r3044 | v1ctor | 2006-08-18 10:48:31 -0700 (Fri, 18 Aug 2006) | 2 lines

example moved

------------------------------------------------------------------------
r3043 | v1ctor | 2006-08-18 10:41:45 -0700 (Fri, 18 Aug 2006) | 2 lines

updated: -lang changes

------------------------------------------------------------------------
r3042 | v1ctor | 2006-08-18 10:37:04 -0700 (Fri, 18 Aug 2006) | 2 lines

added: the -lang command-line option, allowing new features to be added while keeping (and improving) the backward compatibility with QB

------------------------------------------------------------------------
r3041 | jeffmarshall | 2006-08-17 18:42:11 -0700 (Thu, 17 Aug 2006) | 2 lines

fixed: sigsegv on #MACRO with no macro body

------------------------------------------------------------------------
r3040 | jeffmarshall | 2006-08-17 18:36:09 -0700 (Thu, 17 Aug 2006) | 2 lines

added new test

------------------------------------------------------------------------
r3039 | jeffmarshall | 2006-08-15 18:10:08 -0700 (Tue, 15 Aug 2006) | 2 lines

fixed: INPUTand LINE INPUT not resetting tab location

------------------------------------------------------------------------
r3038 | jeffmarshall | 2006-08-15 13:23:50 -0700 (Tue, 15 Aug 2006) | 2 lines

updated

------------------------------------------------------------------------
r3037 | jeffmarshall | 2006-08-15 13:23:21 -0700 (Tue, 15 Aug 2006) | 2 lines

main GFX window on win32 captures mouse input to allow out of window mouse button releases

------------------------------------------------------------------------
r3036 | v1ctor | 2006-08-14 11:46:00 -0700 (Mon, 14 Aug 2006) | 2 lines

fixed: file size calculation in text-mode, EOF() should always work in text-mode now

------------------------------------------------------------------------
r3035 | v1ctor | 2006-08-14 11:44:29 -0700 (Mon, 14 Aug 2006) | 2 lines

small change

------------------------------------------------------------------------
r3034 | v1ctor | 2006-08-14 11:44:16 -0700 (Mon, 14 Aug 2006) | 2 lines

new test

------------------------------------------------------------------------
r3033 | jeffmarshall | 2006-08-14 10:32:50 -0700 (Mon, 14 Aug 2006) | 2 lines

updated: GetJoystick() returns POV in U,V parameters if available, otherwise returns axis 5 and 6.

------------------------------------------------------------------------
r3032 | v1ctor | 2006-08-13 09:48:19 -0700 (Sun, 13 Aug 2006) | 2 lines

added: libvariant (operator overloading for the VARIANT Win API type)

------------------------------------------------------------------------
r3031 | v1ctor | 2006-08-12 19:38:58 -0700 (Sat, 12 Aug 2006) | 2 lines

added: the missing llVal field in the VARIANT UDT

------------------------------------------------------------------------
r3030 | i_am_drv | 2006-08-10 15:35:54 -0700 (Thu, 10 Aug 2006) | 2 lines

"GBD" -> "GDB"

------------------------------------------------------------------------
r3029 | i_am_drv | 2006-08-09 20:48:21 -0700 (Wed, 09 Aug 2006) | 2 lines

fbdev: discard invalid mouse coordinates (default values are -1)

------------------------------------------------------------------------
r3028 | i_am_drv | 2006-08-08 20:54:32 -0700 (Tue, 08 Aug 2006) | 2 lines

fixed: soft cursor was not always being erased on linux

------------------------------------------------------------------------
r3027 | v1ctor | 2006-08-08 20:51:59 -0700 (Tue, 08 Aug 2006) | 2 lines

fixed: fb_linux.display was not being set to NULL

------------------------------------------------------------------------
r3026 | i_am_drv | 2006-08-07 20:22:58 -0700 (Mon, 07 Aug 2006) | 2 lines

save and restore FPU state in DOS ISR handler

------------------------------------------------------------------------
r3025 | i_am_drv | 2006-08-07 19:50:00 -0700 (Mon, 07 Aug 2006) | 2 lines

fix the fix for GFX_NO_FRAME

------------------------------------------------------------------------
r3024 | jeffmarshall | 2006-08-07 17:45:15 -0700 (Mon, 07 Aug 2006) | 2 lines

added: rtlibx IsDate() function

------------------------------------------------------------------------
r3023 | i_am_drv | 2006-08-07 16:43:23 -0700 (Mon, 07 Aug 2006) | 2 lines

fix window size with GFX_NO_FRAME

------------------------------------------------------------------------
r3022 | v1ctor | 2006-08-07 12:44:18 -0700 (Mon, 07 Aug 2006) | 2 lines

added: new test

------------------------------------------------------------------------
r3021 | v1ctor | 2006-08-07 12:44:05 -0700 (Mon, 07 Aug 2006) | 2 lines

fixed: STRPTR couldn't be used with pointer derefs

------------------------------------------------------------------------
r3020 | v1ctor | 2006-08-07 12:22:55 -0700 (Mon, 07 Aug 2006) | 2 lines

fixed: quirk string functions were refusing invalid types without reporting the errors

------------------------------------------------------------------------
r3019 | v1ctor | 2006-08-07 12:02:12 -0700 (Mon, 07 Aug 2006) | 2 lines

added: new test

------------------------------------------------------------------------
r3018 | v1ctor | 2006-08-07 12:02:03 -0700 (Mon, 07 Aug 2006) | 2 lines

fixed: SUB pointers returned by functions couldn't be directly dereferenced

------------------------------------------------------------------------
r3017 | v1ctor | 2006-08-07 11:41:42 -0700 (Mon, 07 Aug 2006) | 2 lines

added: new test

------------------------------------------------------------------------
r3016 | v1ctor | 2006-08-07 11:36:51 -0700 (Mon, 07 Aug 2006) | 2 lines

fixed: the function type was being checked before the overloading resolution

------------------------------------------------------------------------
r3015 | jeffmarshall | 2006-08-07 11:08:34 -0700 (Mon, 07 Aug 2006) | 2 lines

fixed: COM string comparison was case sensitive

------------------------------------------------------------------------
r3014 | jeffmarshall | 2006-08-06 20:20:13 -0700 (Sun, 06 Aug 2006) | 2 lines

fixed: serial device - allow "COM:" for default serial device on linux

------------------------------------------------------------------------
r3013 | jeffmarshall | 2006-08-06 17:06:21 -0700 (Sun, 06 Aug 2006) | 2 lines

updated: serial device - allow "COM:" for default serial device.

------------------------------------------------------------------------
r3012 | jeffmarshall | 2006-08-06 17:05:41 -0700 (Sun, 06 Aug 2006) | 2 lines

updated: win32 serial driver - allow "COM:" for default serial device.

------------------------------------------------------------------------
r3011 | jeffmarshall | 2006-08-06 14:28:02 -0700 (Sun, 06 Aug 2006) | 2 lines

added: linux serial driver - support for COMn => /dev/ttySn translation

------------------------------------------------------------------------
r3010 | v1ctor | 2006-08-06 09:53:15 -0700 (Sun, 06 Aug 2006) | 2 lines

updated

------------------------------------------------------------------------
r3009 | v1ctor | 2006-08-06 09:52:45 -0700 (Sun, 06 Aug 2006) | 2 lines

added: serial device support in Linux, written by Jose Manuel Postigo

------------------------------------------------------------------------
r3008 | jeffmarshall | 2006-08-05 19:56:54 -0700 (Sat, 05 Aug 2006) | 2 lines

added: linux lpt driver checks default printer and named printers with lpstat and returns an error if they do not exist.

------------------------------------------------------------------------
r3007 | v1ctor | 2006-08-05 10:09:24 -0700 (Sat, 05 Aug 2006) | 2 lines

added: sqlite2 header and import library

------------------------------------------------------------------------
r3006 | v1ctor | 2006-08-03 22:27:06 -0700 (Thu, 03 Aug 2006) | 2 lines

fixed: the jmp_buf type, for all platforms

------------------------------------------------------------------------
r3005 | v1ctor | 2006-08-03 22:25:45 -0700 (Thu, 03 Aug 2006) | 2 lines

added: the __FILE_NQ__ and __FUNCTION_NQ__ intrinsic defines, same as __FILE__ and __FUNCTION__, but without double-quotes

------------------------------------------------------------------------
r3004 | v1ctor | 2006-08-03 12:46:20 -0700 (Thu, 03 Aug 2006) | 2 lines

changed: LOCATE is now an ordinary function, no runtime-error check

------------------------------------------------------------------------
r3003 | v1ctor | 2006-08-03 12:44:23 -0700 (Thu, 03 Aug 2006) | 2 lines

changed: LOCATE is now an ordinary function, no runtime-error check

------------------------------------------------------------------------
r3002 | v1ctor | 2006-08-01 21:34:48 -0700 (Tue, 01 Aug 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r3001 | v1ctor | 2006-08-01 21:30:53 -0700 (Tue, 01 Aug 2006) | 2 lines

updated

------------------------------------------------------------------------
r3000 | v1ctor | 2006-08-01 21:30:11 -0700 (Tue, 01 Aug 2006) | 2 lines

added: support for typedefs in fixed-len string declarations

------------------------------------------------------------------------
r2999 | i_am_drv | 2006-08-01 16:23:43 -0700 (Tue, 01 Aug 2006) | 2 lines

cleanup

------------------------------------------------------------------------
r2998 | i_am_drv | 2006-08-01 16:21:54 -0700 (Tue, 01 Aug 2006) | 2 lines

minor fixes and cleanup

------------------------------------------------------------------------
r2997 | i_am_drv | 2006-08-01 16:14:01 -0700 (Tue, 01 Aug 2006) | 2 lines

fixed: mouse isr might be executed during timer isr

------------------------------------------------------------------------
r2996 | i_am_drv | 2006-08-01 15:52:52 -0700 (Tue, 01 Aug 2006) | 2 lines

remove extraneous code

------------------------------------------------------------------------
r2995 | lillo | 2006-08-01 10:22:39 -0700 (Tue, 01 Aug 2006) | 2 lines

BLOAD/BSAVE now convert path separator accordingly to current platform

------------------------------------------------------------------------
r2994 | i_am_drv | 2006-07-31 20:38:01 -0700 (Mon, 31 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2993 | i_am_drv | 2006-07-31 20:23:46 -0700 (Mon, 31 Jul 2006) | 2 lines

mark entire screen as dirty on startup

------------------------------------------------------------------------
r2992 | i_am_drv | 2006-07-31 20:19:33 -0700 (Mon, 31 Jul 2006) | 2 lines

lock all memory touched by the timer ISR

------------------------------------------------------------------------
r2991 | i_am_drv | 2006-07-31 19:13:32 -0700 (Mon, 31 Jul 2006) | 2 lines

in soviet russia, mouse interrupts you

------------------------------------------------------------------------
r2990 | jeffmarshall | 2006-07-30 19:21:11 -0700 (Sun, 30 Jul 2006) | 2 lines

fixed: DTR was not being set high in open com

------------------------------------------------------------------------
r2989 | v1ctor | 2006-07-30 16:55:58 -0700 (Sun, 30 Jul 2006) | 2 lines

fixed: Linux support

------------------------------------------------------------------------
r2988 | v1ctor | 2006-07-30 16:35:42 -0700 (Sun, 30 Jul 2006) | 4 lines

fixed:
- the last '/' wasn't being added to the path name;
- the read-only check was resulting in false-positives;

------------------------------------------------------------------------
r2987 | i_am_drv | 2006-07-30 12:40:50 -0700 (Sun, 30 Jul 2006) | 2 lines

added linear VESA DOS gfxlib driver

------------------------------------------------------------------------
r2986 | i_am_drv | 2006-07-30 11:05:55 -0700 (Sun, 30 Jul 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2985 | i_am_drv | 2006-07-29 23:58:27 -0700 (Sat, 29 Jul 2006) | 2 lines

DOS gfxlib: only update range of hardware palette entries that has changed

------------------------------------------------------------------------
r2984 | i_am_drv | 2006-07-29 23:45:52 -0700 (Sat, 29 Jul 2006) | 2 lines

use dirty line info in VESA gfx driver

------------------------------------------------------------------------
r2983 | i_am_drv | 2006-07-29 23:10:01 -0700 (Sat, 29 Jul 2006) | 2 lines

added ScreenList for GDI win32 driver

------------------------------------------------------------------------
r2982 | jeffmarshall | 2006-07-29 19:41:02 -0700 (Sat, 29 Jul 2006) | 2 lines

fixed: wiki -> fbhelp converter was breaking long links

------------------------------------------------------------------------
r2981 | jeffmarshall | 2006-07-29 19:40:27 -0700 (Sat, 29 Jul 2006) | 2 lines

added: -getpage option to refresh specified wiki pages to local cache

------------------------------------------------------------------------
r2980 | v1ctor | 2006-07-29 18:12:28 -0700 (Sat, 29 Jul 2006) | 2 lines

fixed: error recovery of function headers was resulting in an ICE

------------------------------------------------------------------------
r2979 | jeffmarshall | 2006-07-29 08:24:12 -0700 (Sat, 29 Jul 2006) | 2 lines

fixed: TOC level setting for catpgfunctindex

------------------------------------------------------------------------
r2978 | v1ctor | 2006-07-28 19:05:45 -0700 (Fri, 28 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2977 | v1ctor | 2006-07-28 19:03:20 -0700 (Fri, 28 Jul 2006) | 2 lines

fixed: empty literal strings couldn't be used with stringize'd macro params

------------------------------------------------------------------------
r2976 | v1ctor | 2006-07-28 17:36:26 -0700 (Fri, 28 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2975 | v1ctor | 2006-07-28 17:35:54 -0700 (Fri, 28 Jul 2006) | 2 lines

fixed: empty literal strings couldn't be used with stringize'd macro params

------------------------------------------------------------------------
r2974 | v1ctor | 2006-07-28 17:19:19 -0700 (Fri, 28 Jul 2006) | 2 lines

added: CUnit (unit testing framework) headers, static library and examples

------------------------------------------------------------------------
r2973 | v1ctor | 2006-07-28 15:07:38 -0700 (Fri, 28 Jul 2006) | 2 lines

added: CUnit (unit testing framework) headers, static library and examples

------------------------------------------------------------------------
r2972 | jeffmarshall | 2006-07-28 11:31:15 -0700 (Fri, 28 Jul 2006) | 2 lines

fixed: serial ports not opening on win95

------------------------------------------------------------------------
r2971 | jeffmarshall | 2006-07-28 08:40:26 -0700 (Fri, 28 Jul 2006) | 2 lines

fixed: was not generating TOC sections for CatPgFunctIndex in CHM

------------------------------------------------------------------------
r2970 | jeffmarshall | 2006-07-28 08:38:09 -0700 (Fri, 28 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2969 | jeffmarshall | 2006-07-28 01:35:18 -0700 (Fri, 28 Jul 2006) | 2 lines

added

------------------------------------------------------------------------
r2968 | jeffmarshall | 2006-07-28 01:34:51 -0700 (Fri, 28 Jul 2006) | 2 lines

updated - support for fbhelp console viewer file format

------------------------------------------------------------------------
r2967 | v1ctor | 2006-07-27 18:20:26 -0700 (Thu, 27 Jul 2006) | 2 lines

changed: dups removed

------------------------------------------------------------------------
r2966 | v1ctor | 2006-07-27 18:18:12 -0700 (Thu, 27 Jul 2006) | 2 lines

changed: stub removed, prototypes added

------------------------------------------------------------------------
r2965 | v1ctor | 2006-07-27 10:56:53 -0700 (Thu, 27 Jul 2006) | 2 lines

added: tar+gzip support

------------------------------------------------------------------------
r2964 | v1ctor | 2006-07-26 22:00:00 -0700 (Wed, 26 Jul 2006) | 2 lines

fixed: anon-udt would deref a null-pointer if the udt was a void typedef, causing an ICE

------------------------------------------------------------------------
r2963 | v1ctor | 2006-07-26 21:32:13 -0700 (Wed, 26 Jul 2006) | 2 lines

added

------------------------------------------------------------------------
r2962 | v1ctor | 2006-07-26 21:28:14 -0700 (Wed, 26 Jul 2006) | 2 lines

added: the -excl cmd-line option, to exclude directories

------------------------------------------------------------------------
r2961 | v1ctor | 2006-07-26 18:13:19 -0700 (Wed, 26 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2960 | v1ctor | 2006-07-26 18:08:39 -0700 (Wed, 26 Jul 2006) | 2 lines

TRAM - Testing Release Archive Maker

------------------------------------------------------------------------
r2959 | v1ctor | 2006-07-26 15:28:18 -0700 (Wed, 26 Jul 2006) | 2 lines

changed: include files will now be checked in this order: 1st) in the current source-file path, 2nd) in the absolute/relative path, 3rd) in the compiler include paths

------------------------------------------------------------------------
r2958 | v1ctor | 2006-07-26 13:46:25 -0700 (Wed, 26 Jul 2006) | 2 lines

added: support for BINARY mode

------------------------------------------------------------------------
r2957 | v1ctor | 2006-07-26 12:53:17 -0700 (Wed, 26 Jul 2006) | 2 lines

added: FileDateTime (returns a date + time serial, not a string)

------------------------------------------------------------------------
r2956 | v1ctor | 2006-07-26 10:58:13 -0700 (Wed, 26 Jul 2006) | 2 lines

changed: FBSTRING * params to const char *

------------------------------------------------------------------------
r2955 | v1ctor | 2006-07-25 22:50:08 -0700 (Tue, 25 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2954 | v1ctor | 2006-07-25 22:44:47 -0700 (Tue, 25 Jul 2006) | 2 lines

fixed: mangling of symbols declared inside nested namespaces

------------------------------------------------------------------------
r2953 | v1ctor | 2006-07-25 22:42:57 -0700 (Tue, 25 Jul 2006) | 2 lines

new test

------------------------------------------------------------------------
r2952 | i_am_drv | 2006-07-25 19:25:20 -0700 (Tue, 25 Jul 2006) | 2 lines

fix soft cursor under DOS - old mouse cursor was not being erased due to dirty lines not being preserved for current mouse pos

------------------------------------------------------------------------
r2951 | i_am_drv | 2006-07-25 18:58:59 -0700 (Tue, 25 Jul 2006) | 2 lines

move __attribute((packed)) to end of struct definition (gcc 4.1 warning fix)

------------------------------------------------------------------------
r2950 | v1ctor | 2006-07-24 14:54:26 -0700 (Mon, 24 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2949 | v1ctor | 2006-07-24 14:53:09 -0700 (Mon, 24 Jul 2006) | 2 lines

reverted: size can't be used with files opened in INPUT mode, fseek() is unreliable

------------------------------------------------------------------------
r2948 | v1ctor | 2006-07-24 09:52:37 -0700 (Mon, 24 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2947 | v1ctor | 2006-07-24 09:51:55 -0700 (Mon, 24 Jul 2006) | 2 lines

fixed: string indexing was been allowed with functions

------------------------------------------------------------------------
r2946 | jeffmarshall | 2006-07-23 08:34:13 -0700 (Sun, 23 Jul 2006) | 2 lines

fixed: DRIVER_SHAPED_WINDOW was not ignoring GFX_NULL = -1

------------------------------------------------------------------------
r2945 | i_am_drv | 2006-07-22 21:04:49 -0700 (Sat, 22 Jul 2006) | 2 lines

fixed FileExists rtlibx function (forgot to dereference FBSTRING)

------------------------------------------------------------------------
r2944 | i_am_drv | 2006-07-22 21:01:36 -0700 (Sat, 22 Jul 2006) | 2 lines

added FileExists rtlibx function

------------------------------------------------------------------------
r2943 | v1ctor | 2006-07-22 18:09:44 -0700 (Sat, 22 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2942 | v1ctor | 2006-07-22 18:08:43 -0700 (Sat, 22 Jul 2006) | 2 lines

added: namespace nesting, as in .net: namespace foo.bar.baz : end namespace

------------------------------------------------------------------------
r2941 | v1ctor | 2006-07-22 18:04:35 -0700 (Sat, 22 Jul 2006) | 2 lines

new example

------------------------------------------------------------------------
r2940 | v1ctor | 2006-07-22 18:04:22 -0700 (Sat, 22 Jul 2006) | 2 lines

new test

------------------------------------------------------------------------
r2939 | jeffmarshall | 2006-07-22 17:46:29 -0700 (Sat, 22 Jul 2006) | 2 lines

added: new keywords to syntax highlighter

------------------------------------------------------------------------
r2938 | jeffmarshall | 2006-07-22 16:48:41 -0700 (Sat, 22 Jul 2006) | 2 lines

fixed: DRAW STRING g was returning run-time error on off-screen blits.

------------------------------------------------------------------------
r2937 | i_am_drv | 2006-07-22 12:31:24 -0700 (Sat, 22 Jul 2006) | 2 lines

updated for new memory locking routines

------------------------------------------------------------------------
r2936 | v1ctor | 2006-07-22 11:58:03 -0700 (Sat, 22 Jul 2006) | 2 lines

stub file

------------------------------------------------------------------------
r2935 | v1ctor | 2006-07-22 11:20:34 -0700 (Sat, 22 Jul 2006) | 2 lines

fixed: the preprocessor symbol-table was been accessed after been deallocated

------------------------------------------------------------------------
r2934 | lillo | 2006-07-21 13:40:02 -0700 (Fri, 21 Jul 2006) | 4 lines

- Added the ADD drawing mode for additive blending. Affects PUT and DRAW STRING
- Changed CUSTOM mode to allow an user-def parameter to be passed to the blender function, which now must have the form:
  function blender(byval src as uinteger, byval dest as uinteger, byval param as any ptr) as uinteger

------------------------------------------------------------------------
r2933 | v1ctor | 2006-07-21 12:20:31 -0700 (Fri, 21 Jul 2006) | 2 lines

added: ctype.bi

------------------------------------------------------------------------
r2932 | i_am_drv | 2006-07-20 20:22:40 -0700 (Thu, 20 Jul 2006) | 2 lines

fix dos memory locking (virtual address != linear address, oops...); fix dos gfxlib scrollwheel support

------------------------------------------------------------------------
r2931 | i_am_drv | 2006-07-20 20:20:31 -0700 (Thu, 20 Jul 2006) | 2 lines

fix dos memory locking (virtual address != linear address, oops...)

------------------------------------------------------------------------
r2930 | i_am_drv | 2006-07-20 20:18:45 -0700 (Thu, 20 Jul 2006) | 2 lines

fix dos mouse scrollwheel support

------------------------------------------------------------------------
r2929 | v1ctor | 2006-07-19 21:59:44 -0700 (Wed, 19 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2928 | v1ctor | 2006-07-19 21:58:31 -0700 (Wed, 19 Jul 2006) | 2 lines

new test

------------------------------------------------------------------------
r2927 | v1ctor | 2006-07-19 21:55:52 -0700 (Wed, 19 Jul 2006) | 2 lines

fixed: the prefix was not been added to C++ mangled symbols declared inside namespaces

------------------------------------------------------------------------
r2926 | v1ctor | 2006-07-19 19:43:32 -0700 (Wed, 19 Jul 2006) | 2 lines

new example, by yetifoot

------------------------------------------------------------------------
r2925 | v1ctor | 2006-07-19 14:56:12 -0700 (Wed, 19 Jul 2006) | 2 lines

new test

------------------------------------------------------------------------
r2924 | v1ctor | 2006-07-19 14:55:24 -0700 (Wed, 19 Jul 2006) | 2 lines

small fix

------------------------------------------------------------------------
r2923 | v1ctor | 2006-07-19 14:31:17 -0700 (Wed, 19 Jul 2006) | 2 lines

fixed: symbols declared inside namespaces were been prefixed twice

------------------------------------------------------------------------
r2922 | jeffmarshall | 2006-07-19 07:14:29 -0700 (Wed, 19 Jul 2006) | 2 lines

fixed: 'lp' was showing output on stderr

------------------------------------------------------------------------
r2921 | v1ctor | 2006-07-18 23:35:53 -0700 (Tue, 18 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2920 | v1ctor | 2006-07-18 23:35:15 -0700 (Tue, 18 Jul 2006) | 2 lines

fixed: SWAP with bitfields was been allowed

------------------------------------------------------------------------
r2919 | v1ctor | 2006-07-18 21:31:50 -0700 (Tue, 18 Jul 2006) | 2 lines

reverted: "opaque" is a GDI define, including windows.bi before zlib.bi would cause an error

------------------------------------------------------------------------
r2918 | v1ctor | 2006-07-18 20:43:19 -0700 (Tue, 18 Jul 2006) | 2 lines

fixed: the line count was been updated when expanding a multi-line macros

------------------------------------------------------------------------
r2917 | i_am_drv | 2006-07-18 20:17:54 -0700 (Tue, 18 Jul 2006) | 2 lines

dos gfxlib now uses shared soft cursor code; additional dos gfxlib cleanup and error checking

------------------------------------------------------------------------
r2916 | lillo | 2006-07-18 13:28:32 -0700 (Tue, 18 Jul 2006) | 3 lines

- Added borderless and custom shaped windows support to Win32 gfx drivers
- Win32 GDI driver now supports fullscreen

------------------------------------------------------------------------
r2915 | i_am_drv | 2006-07-15 22:50:09 -0700 (Sat, 15 Jul 2006) | 2 lines

remove underscore from 'opaque' member of z_stream to match C header

------------------------------------------------------------------------
r2914 | i_am_drv | 2006-07-15 22:28:15 -0700 (Sat, 15 Jul 2006) | 2 lines

added zlib init macros

------------------------------------------------------------------------
r2913 | v1ctor | 2006-07-15 15:32:19 -0700 (Sat, 15 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2912 | v1ctor | 2006-07-15 15:27:38 -0700 (Sat, 15 Jul 2006) | 6 lines

fixed:
- forward types were incorrectly mangled (with function ptrs too);

added:
- forward types can now be used in params of function prototypes passed byref;

------------------------------------------------------------------------
r2911 | v1ctor | 2006-07-15 15:24:17 -0700 (Sat, 15 Jul 2006) | 2 lines

new tests

------------------------------------------------------------------------
r2910 | v1ctor | 2006-07-15 07:33:31 -0700 (Sat, 15 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2909 | v1ctor | 2006-07-15 07:33:03 -0700 (Sat, 15 Jul 2006) | 2 lines

fixed: debugging info for local static variables that were never referenced was been emitted

------------------------------------------------------------------------
r2908 | v1ctor | 2006-07-15 06:47:08 -0700 (Sat, 15 Jul 2006) | 2 lines

fixed: newlines were been printed

------------------------------------------------------------------------
r2907 | v1ctor | 2006-07-15 06:46:41 -0700 (Sat, 15 Jul 2006) | 2 lines

fixed: CALL didn't allow function pointers

------------------------------------------------------------------------
r2906 | lillo | 2006-07-13 06:56:04 -0700 (Thu, 13 Jul 2006) | 2 lines

fixed CIRCLE precision problems

------------------------------------------------------------------------
r2905 | lillo | 2006-07-12 04:24:25 -0700 (Wed, 12 Jul 2006) | 2 lines

clear whole framebuffer memory on console restore

------------------------------------------------------------------------
r2904 | lillo | 2006-07-12 03:17:59 -0700 (Wed, 12 Jul 2006) | 2 lines

fixed typo

------------------------------------------------------------------------
r2903 | v1ctor | 2006-07-11 19:35:22 -0700 (Tue, 11 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2902 | v1ctor | 2006-07-11 19:33:57 -0700 (Tue, 11 Jul 2006) | 2 lines

fixed: when the format string has no exponent, the fractions shouldn't be scaled up

------------------------------------------------------------------------
r2901 | v1ctor | 2006-07-11 18:32:50 -0700 (Tue, 11 Jul 2006) | 2 lines

fixed: null expressions could be passed to PRINT USING

------------------------------------------------------------------------
r2900 | v1ctor | 2006-07-11 12:20:18 -0700 (Tue, 11 Jul 2006) | 2 lines

new tests

------------------------------------------------------------------------
r2899 | v1ctor | 2006-07-07 19:20:33 -0700 (Fri, 07 Jul 2006) | 2 lines

fixed: no expression was been returned when recovering from errors in L|UBOUND()

------------------------------------------------------------------------
r2898 | jeffmarshall | 2006-07-06 18:44:47 -0700 (Thu, 06 Jul 2006) | 2 lines

win32 GDI driver needed to call InvalidateRect() for Win98

------------------------------------------------------------------------
r2897 | v1ctor | 2006-07-06 11:32:12 -0700 (Thu, 06 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2896 | v1ctor | 2006-07-06 11:29:55 -0700 (Thu, 06 Jul 2006) | 2 lines

added: the __FB_ERR__ define, set to 1, 3, 7, if one of the -e, -ex or -exx cmd-line options was passed, respectively

------------------------------------------------------------------------
r2895 | v1ctor | 2006-07-06 11:15:41 -0700 (Thu, 06 Jul 2006) | 2 lines

fixed: concatenation of a literal wstring with a var-len string was resulting in an ICE

------------------------------------------------------------------------
r2894 | jeffmarshall | 2006-07-06 07:01:33 -0700 (Thu, 06 Jul 2006) | 2 lines

fb_ConsoleGetMouse() on win32 always sets ENABLE_MOUSE_INPUT if needed

------------------------------------------------------------------------
r2893 | v1ctor | 2006-07-05 11:27:13 -0700 (Wed, 05 Jul 2006) | 2 lines

added: helper macros

------------------------------------------------------------------------
r2892 | v1ctor | 2006-07-05 11:22:09 -0700 (Wed, 05 Jul 2006) | 2 lines

new header

------------------------------------------------------------------------
r2891 | v1ctor | 2006-07-04 18:29:58 -0700 (Tue, 04 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2890 | v1ctor | 2006-07-04 18:29:28 -0700 (Tue, 04 Jul 2006) | 2 lines

fixed: #define's parameters weren't checked for dup definitions

------------------------------------------------------------------------
r2889 | v1ctor | 2006-07-04 18:26:31 -0700 (Tue, 04 Jul 2006) | 2 lines

fixed: duplicated definition in #define's parameters list

------------------------------------------------------------------------
r2888 | v1ctor | 2006-07-04 18:24:02 -0700 (Tue, 04 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2887 | v1ctor | 2006-07-03 20:49:33 -0700 (Mon, 03 Jul 2006) | 2 lines

fixed: recovering from a void function ptr call inside an expression was resulting in an ICE

------------------------------------------------------------------------
r2886 | lillo | 2006-07-03 15:41:51 -0700 (Mon, 03 Jul 2006) | 2 lines

Added frameless and custom shaped windows support (Linux X11 only for now)

------------------------------------------------------------------------
r2885 | lillo | 2006-07-03 05:29:13 -0700 (Mon, 03 Jul 2006) | 2 lines

removed warnings

------------------------------------------------------------------------
r2884 | v1ctor | 2006-07-02 21:34:02 -0700 (Sun, 02 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2883 | v1ctor | 2006-07-02 21:33:40 -0700 (Sun, 02 Jul 2006) | 2 lines

fixed: field arrays of function pointers were not correctly parsed

------------------------------------------------------------------------
r2882 | v1ctor | 2006-07-02 21:32:24 -0700 (Sun, 02 Jul 2006) | 2 lines

new test

------------------------------------------------------------------------
r2881 | v1ctor | 2006-07-02 20:21:13 -0700 (Sun, 02 Jul 2006) | 2 lines

fixed: hexadecimal literals beginning with 0X were not translated to &h by SWIG

------------------------------------------------------------------------
r2880 | lillo | 2006-07-02 11:40:16 -0700 (Sun, 02 Jul 2006) | 2 lines

reverted change. Applied a more robust fix in gfxlib itself

------------------------------------------------------------------------
r2879 | lillo | 2006-07-02 11:38:53 -0700 (Sun, 02 Jul 2006) | 2 lines

Fixed screen clearing on gfx program exit. Additional screen flags work in progress: linux X11 driver now supports windows without borders, shaped windows next...

------------------------------------------------------------------------
r2878 | jeffmarshall | 2006-07-02 10:39:00 -0700 (Sun, 02 Jul 2006) | 2 lines

fbdoc - FreeBASIC User's Manual Convert/Generator - Added

------------------------------------------------------------------------
r2877 | v1ctor | 2006-07-02 09:17:36 -0700 (Sun, 02 Jul 2006) | 2 lines

updated

------------------------------------------------------------------------
r2876 | jeffmarshall | 2006-07-01 20:02:06 -0700 (Sat, 01 Jul 2006) | 2 lines

more organized division between lpt device and driver

------------------------------------------------------------------------
r2875 | v1ctor | 2006-06-30 18:58:43 -0700 (Fri, 30 Jun 2006) | 2 lines

added: a check for strtof(), it's missing in DJGPP (note: run configure again)

------------------------------------------------------------------------
r2874 | v1ctor | 2006-06-30 18:44:42 -0700 (Fri, 30 Jun 2006) | 2 lines

fixed: libm was missing in DOS, linking to rint() would fail

------------------------------------------------------------------------
r2873 | v1ctor | 2006-06-30 14:30:15 -0700 (Fri, 30 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2872 | v1ctor | 2006-06-30 14:30:09 -0700 (Fri, 30 Jun 2006) | 2 lines

fixed: names of non-shared COMMON variables were not been preserved

------------------------------------------------------------------------
r2871 | v1ctor | 2006-06-30 14:22:39 -0700 (Fri, 30 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2870 | v1ctor | 2006-06-30 14:14:59 -0700 (Fri, 30 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2869 | v1ctor | 2006-06-30 13:32:29 -0700 (Fri, 30 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2868 | v1ctor | 2006-06-30 13:29:13 -0700 (Fri, 30 Jun 2006) | 2 lines

fixed: the #macro's body should be trimmed

------------------------------------------------------------------------
r2867 | v1ctor | 2006-06-30 13:22:14 -0700 (Fri, 30 Jun 2006) | 2 lines

new tests

------------------------------------------------------------------------
r2866 | v1ctor | 2006-06-30 00:21:36 -0700 (Fri, 30 Jun 2006) | 2 lines

fixed: the string concat operator (&) had a different precedence order than in VB

------------------------------------------------------------------------
r2865 | v1ctor | 2006-06-29 22:41:25 -0700 (Thu, 29 Jun 2006) | 2 lines

added: support for nested #macro .. #endmacro blocks

------------------------------------------------------------------------
r2864 | v1ctor | 2006-06-29 19:42:26 -0700 (Thu, 29 Jun 2006) | 2 lines

fixed: control characters were been printed in error messages

------------------------------------------------------------------------
r2863 | v1ctor | 2006-06-29 14:05:17 -0700 (Thu, 29 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2862 | v1ctor | 2006-06-29 13:57:17 -0700 (Thu, 29 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2861 | v1ctor | 2006-06-29 13:55:44 -0700 (Thu, 29 Jun 2006) | 7 lines

added:
- #macro .. #endmacro;

changed:
- the pre-processor keywords aren't reserved words anymore;
- because the above change, $inclib and $libpath were removed, use #inclib and #libpath instead;

------------------------------------------------------------------------
r2860 | v1ctor | 2006-06-29 13:52:55 -0700 (Thu, 29 Jun 2006) | 2 lines

internal changes: void initializers were used to speed up the recursive functions

------------------------------------------------------------------------
r2859 | v1ctor | 2006-06-28 23:27:58 -0700 (Wed, 28 Jun 2006) | 2 lines

changed: '$... to #...

------------------------------------------------------------------------
r2858 | v1ctor | 2006-06-28 23:27:12 -0700 (Wed, 28 Jun 2006) | 2 lines

small fixes

------------------------------------------------------------------------
r2857 | i_am_drv | 2006-06-28 18:50:06 -0700 (Wed, 28 Jun 2006) | 2 lines

fixed: DOS ISR memory locking functions now always succeeds (UGLY HACK)

------------------------------------------------------------------------
r2856 | i_am_drv | 2006-06-28 18:25:15 -0700 (Wed, 28 Jun 2006) | 2 lines

fixed: isr_timer example (assumed old NOT behavior; incorrect prototype for fb_isr_set)

------------------------------------------------------------------------
r2855 | jeffmarshall | 2006-06-28 09:02:50 -0700 (Wed, 28 Jun 2006) | 2 lines

added test

------------------------------------------------------------------------
r2854 | jeffmarshall | 2006-06-28 09:00:39 -0700 (Wed, 28 Jun 2006) | 2 lines

added test

------------------------------------------------------------------------
r2853 | jeffmarshall | 2006-06-28 09:00:06 -0700 (Wed, 28 Jun 2006) | 2 lines

added to wrong folder, moved to scopes

------------------------------------------------------------------------
r2852 | jeffmarshall | 2006-06-28 07:55:01 -0700 (Wed, 28 Jun 2006) | 2 lines

added scope and shadow test

------------------------------------------------------------------------
r2851 | jeffmarshall | 2006-06-27 23:59:14 -0700 (Tue, 27 Jun 2006) | 2 lines

RAW spooler support for linux.

------------------------------------------------------------------------
r2850 | v1ctor | 2006-06-27 12:05:49 -0700 (Tue, 27 Jun 2006) | 2 lines

fixed: EOF wasn't been checked

------------------------------------------------------------------------
r2849 | v1ctor | 2006-06-26 18:00:20 -0700 (Mon, 26 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2848 | v1ctor | 2006-06-26 17:59:54 -0700 (Mon, 26 Jun 2006) | 2 lines

fixed: the "= any" initializer was not working with UDT's - they were always cleared

------------------------------------------------------------------------
r2847 | v1ctor | 2006-06-26 17:58:29 -0700 (Mon, 26 Jun 2006) | 2 lines

fixed: static/global variables not accessed outside the asm block wouldn't not be emitted

------------------------------------------------------------------------
r2846 | jeffmarshall | 2006-06-26 17:29:00 -0700 (Mon, 26 Jun 2006) | 2 lines

CreateFile() needed \\.\ to access serial ports over COM9

------------------------------------------------------------------------
r2845 | jeffmarshall | 2006-06-26 00:10:41 -0700 (Mon, 26 Jun 2006) | 2 lines

clean-up

------------------------------------------------------------------------
r2844 | jeffmarshall | 2006-06-26 00:08:52 -0700 (Mon, 26 Jun 2006) | 2 lines

lpt: defaults to lpt1:

------------------------------------------------------------------------
r2843 | jeffmarshall | 2006-06-26 00:07:25 -0700 (Mon, 26 Jun 2006) | 2 lines

split libfb_dev_lpt.c

------------------------------------------------------------------------
r2842 | v1ctor | 2006-06-25 21:25:19 -0700 (Sun, 25 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2841 | v1ctor | 2006-06-25 21:22:02 -0700 (Sun, 25 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2840 | v1ctor | 2006-06-25 21:20:53 -0700 (Sun, 25 Jun 2006) | 2 lines

fixed: user32 is not linked by default

------------------------------------------------------------------------
r2839 | v1ctor | 2006-06-25 21:19:41 -0700 (Sun, 25 Jun 2006) | 2 lines

fixed: comparing a NULL string pointer to an empty string was not returning equal

------------------------------------------------------------------------
r2838 | v1ctor | 2006-06-25 21:18:07 -0700 (Sun, 25 Jun 2006) | 2 lines

fixed: literal wide-string arguments passed to byref ascii var-len string parameters were not creating a temporary string descriptor

------------------------------------------------------------------------
r2837 | v1ctor | 2006-06-25 21:18:07 -0700 (Sun, 25 Jun 2006) | 2 lines

fixed: literal wide-string arguments passed to byref var-len string parameters were not creating a temporary string descriptor

------------------------------------------------------------------------
r2836 | lillo | 2006-06-25 09:40:49 -0700 (Sun, 25 Jun 2006) | 2 lines

Fixed DIR() under Linux; SCREEN 0 now sets 80x25 text mode and clears the screen; exiting a gfx app does not clear the console anymore under Linux

------------------------------------------------------------------------
r2835 | v1ctor | 2006-06-24 18:46:23 -0700 (Sat, 24 Jun 2006) | 2 lines

added: the __FB_DEBUG__ define

------------------------------------------------------------------------
r2834 | v1ctor | 2006-06-24 10:39:51 -0700 (Sat, 24 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2833 | v1ctor | 2006-06-24 10:38:22 -0700 (Sat, 24 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2832 | v1ctor | 2006-06-24 10:38:16 -0700 (Sat, 24 Jun 2006) | 2 lines

fixed: arrays with more than 2 dimensions couldn't be initialized

------------------------------------------------------------------------
r2831 | v1ctor | 2006-06-24 10:37:27 -0700 (Sat, 24 Jun 2006) | 2 lines

small fix

------------------------------------------------------------------------
r2830 | i_am_drv | 2006-06-24 08:42:27 -0700 (Sat, 24 Jun 2006) | 2 lines

added check for win32 to fix gl + glext conflicts (matches C version now)

------------------------------------------------------------------------
r2829 | i_am_drv | 2006-06-23 17:13:14 -0700 (Fri, 23 Jun 2006) | 2 lines

added: FileCopy, FileAttr, and FileLen VB-compatability functions [DrV]

------------------------------------------------------------------------
r2828 | i_am_drv | 2006-06-23 17:09:16 -0700 (Fri, 23 Jun 2006) | 2 lines

added: FileLen function

------------------------------------------------------------------------
r2827 | i_am_drv | 2006-06-23 16:48:03 -0700 (Fri, 23 Jun 2006) | 2 lines

added: FileAttr function

------------------------------------------------------------------------
r2826 | v1ctor | 2006-06-23 11:59:21 -0700 (Fri, 23 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2825 | v1ctor | 2006-06-23 11:59:02 -0700 (Fri, 23 Jun 2006) | 2 lines

added: TYPE's, UNION's, ENUM's and typedefs can now be declared inside functions and scope blocks

------------------------------------------------------------------------
r2824 | v1ctor | 2006-06-23 11:56:16 -0700 (Fri, 23 Jun 2006) | 2 lines

new tests

------------------------------------------------------------------------
r2823 | v1ctor | 2006-06-23 11:44:41 -0700 (Fri, 23 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2822 | v1ctor | 2006-06-22 18:27:23 -0700 (Thu, 22 Jun 2006) | 2 lines

changed: the headers were split for better organization

------------------------------------------------------------------------
r2821 | jeffmarshall | 2006-06-22 17:41:51 -0700 (Thu, 22 Jun 2006) | 5 lines

printer drivers - added header file for common lpt decls
printer drivers - More robust and flexible lpt protocol parser.
printer drivers - win32 - Added "LPT:" support for default printer
printer drivers - win32 - Fixed problems with wide character strings

------------------------------------------------------------------------
r2820 | jeffmarshall | 2006-06-22 16:38:55 -0700 (Thu, 22 Jun 2006) | 5 lines

printer drivers - added header file for common lpt decls
printer drivers - More robust and flexible lpt protocol parser.
printer drivers - win32 - Added "LPT:" support for default printer
printer drivers - win32 - Fixed problems with wide character strings

------------------------------------------------------------------------
r2819 | v1ctor | 2006-06-22 10:01:43 -0700 (Thu, 22 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2818 | v1ctor | 2006-06-22 06:51:02 -0700 (Thu, 22 Jun 2006) | 2 lines

fixed: the gslcblas library is needed in Linux

------------------------------------------------------------------------
r2817 | v1ctor | 2006-06-21 18:29:17 -0700 (Wed, 21 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2816 | v1ctor | 2006-06-21 18:27:14 -0700 (Wed, 21 Jun 2006) | 6 lines

fixed:
- redefined namespaces were not updating the import (USING) list;

changed:
- #undef can't remove symbols defined inside namespaces anymore, due the fix listed above;

------------------------------------------------------------------------
r2815 | v1ctor | 2006-06-21 18:24:56 -0700 (Wed, 21 Jun 2006) | 2 lines

fixed: field deref (->) was been allowed with UDT variables

------------------------------------------------------------------------
r2814 | v1ctor | 2006-06-21 18:14:00 -0700 (Wed, 21 Jun 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2813 | i_am_drv | 2006-06-21 04:46:05 -0700 (Wed, 21 Jun 2006) | 2 lines

added inc/file.bi to win32 installer script

------------------------------------------------------------------------
r2812 | v1ctor | 2006-06-20 22:55:12 -0700 (Tue, 20 Jun 2006) | 4 lines

fixed:
- PRINT for wstrings was not using the wide-char versions of PrintPad and PrintVoid;
- when printing wstrings to a redirected output, the length passed was in chars, not in bytes;

------------------------------------------------------------------------
r2811 | v1ctor | 2006-06-20 22:33:34 -0700 (Tue, 20 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2810 | v1ctor | 2006-06-20 22:30:43 -0700 (Tue, 20 Jun 2006) | 4 lines

fixed:
- PRINT for wstrings was not using the wide-char versions of PrintPad and PrintVoid;
- when printing wstrings to a redirected output, the length passed was in chars, not in bytes;

------------------------------------------------------------------------
r2809 | v1ctor | 2006-06-20 10:29:40 -0700 (Tue, 20 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2808 | v1ctor | 2006-06-20 10:29:29 -0700 (Tue, 20 Jun 2006) | 2 lines

fixed: namespace symbols were been allowed in expressions

------------------------------------------------------------------------
r2807 | v1ctor | 2006-06-20 09:58:59 -0700 (Tue, 20 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2806 | v1ctor | 2006-06-20 09:57:58 -0700 (Tue, 20 Jun 2006) | 2 lines

changed: enumid.elementid was removed, because the ambiguity with other symbols like UDT variables that can have the same name and because with namespaces the elements would have to be added twice to work as both "elementid" and "enumid.elementid"

------------------------------------------------------------------------
r2805 | v1ctor | 2006-06-20 06:29:27 -0700 (Tue, 20 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2804 | v1ctor | 2006-06-20 06:27:50 -0700 (Tue, 20 Jun 2006) | 2 lines

fixed: parameter-less C++ functions were missing the void (v) type

------------------------------------------------------------------------
r2803 | v1ctor | 2006-06-19 21:33:13 -0700 (Mon, 19 Jun 2006) | 2 lines

updated: version signature

------------------------------------------------------------------------
r2801 | v1ctor | 2006-06-18 20:25:57 -0700 (Sun, 18 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2800 | v1ctor | 2006-06-18 20:12:49 -0700 (Sun, 18 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2799 | lillo | 2006-06-18 15:25:38 -0700 (Sun, 18 Jun 2006) | 2 lines

some fixes...

------------------------------------------------------------------------
r2798 | v1ctor | 2006-06-16 20:42:32 -0700 (Fri, 16 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2797 | v1ctor | 2006-06-16 20:41:03 -0700 (Fri, 16 Jun 2006) | 2 lines

changed: implicit variables allocated inside explicit SCOPE .. END SCOPE blocks won't be moved to the parent function scope

------------------------------------------------------------------------
r2796 | v1ctor | 2006-06-16 18:59:45 -0700 (Fri, 16 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2795 | v1ctor | 2006-06-16 18:56:38 -0700 (Fri, 16 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2794 | v1ctor | 2006-06-16 18:56:05 -0700 (Fri, 16 Jun 2006) | 2 lines

fixed: the case of aliases of extern dynamic arrays was not been preserved

------------------------------------------------------------------------
r2793 | v1ctor | 2006-06-16 18:39:39 -0700 (Fri, 16 Jun 2006) | 2 lines

changed: implicit variables will be always allocated at the function scope, not inside of the current one

------------------------------------------------------------------------
r2792 | v1ctor | 2006-06-16 18:28:03 -0700 (Fri, 16 Jun 2006) | 2 lines

changed: implicit variables will be always allocated at the function scope, not inside of the current scope

------------------------------------------------------------------------
r2791 | v1ctor | 2006-06-16 18:23:32 -0700 (Fri, 16 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2790 | v1ctor | 2006-06-16 18:20:28 -0700 (Fri, 16 Jun 2006) | 2 lines

small change

------------------------------------------------------------------------
r2789 | v1ctor | 2006-06-12 09:09:30 -0700 (Mon, 12 Jun 2006) | 2 lines

fixed: conversion of bitfields to signed integer

------------------------------------------------------------------------
r2788 | v1ctor | 2006-06-12 09:03:19 -0700 (Mon, 12 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2787 | lillo | 2006-06-12 03:08:02 -0700 (Mon, 12 Jun 2006) | 2 lines

Mouse buttons status is not reset when mouse goes out of the window

------------------------------------------------------------------------
r2786 | lillo | 2006-06-12 02:47:38 -0700 (Mon, 12 Jun 2006) | 2 lines

Fixed CIRCLE bug with arcs missing a row of pixels to be updated

------------------------------------------------------------------------
r2785 | lillo | 2006-06-12 02:08:36 -0700 (Mon, 12 Jun 2006) | 5 lines

- fixed COLOR bug, was not accepting negative values, thus all colors
>= &h80000000 in hicolor mode were not set as default color.
- fixed bug in PUT where in ALPHA,n mode mask color was not skipped if
pixel was not exactly &h00FF00FF. Now skips any &h**FF00FF.

------------------------------------------------------------------------
r2784 | i_am_drv | 2006-06-11 22:13:53 -0700 (Sun, 11 Jun 2006) | 2 lines

added FileCopy rtlibx function

------------------------------------------------------------------------
r2783 | v1ctor | 2006-06-11 15:42:25 -0700 (Sun, 11 Jun 2006) | 2 lines

small change

------------------------------------------------------------------------
r2782 | v1ctor | 2006-06-11 15:39:41 -0700 (Sun, 11 Jun 2006) | 2 lines

fixed: IDX nodes were not checking for IMM vregs and not loading the index to a register

------------------------------------------------------------------------
r2781 | v1ctor | 2006-06-11 15:34:12 -0700 (Sun, 11 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2780 | v1ctor | 2006-06-11 14:44:37 -0700 (Sun, 11 Jun 2006) | 2 lines

fixed: null chars in fixed-len strings were been printed

------------------------------------------------------------------------
r2779 | v1ctor | 2006-06-11 11:25:22 -0700 (Sun, 11 Jun 2006) | 2 lines

fixed: extern dynamic arrays were been always emitted

------------------------------------------------------------------------
r2778 | v1ctor | 2006-06-11 11:07:20 -0700 (Sun, 11 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2777 | v1ctor | 2006-06-11 09:35:48 -0700 (Sun, 11 Jun 2006) | 2 lines

fixed: macro names were not been checked for namespaces or periods

------------------------------------------------------------------------
r2776 | v1ctor | 2006-06-11 09:34:00 -0700 (Sun, 11 Jun 2006) | 2 lines

fixed: inline asm was skipping the same token twice when parsing literal numeric constants

------------------------------------------------------------------------
r2775 | v1ctor | 2006-06-10 22:05:13 -0700 (Sat, 10 Jun 2006) | 2 lines

small change

------------------------------------------------------------------------
r2774 | v1ctor | 2006-06-10 18:05:56 -0700 (Sat, 10 Jun 2006) | 2 lines

file moved

------------------------------------------------------------------------
r2773 | v1ctor | 2006-06-10 18:04:44 -0700 (Sat, 10 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2772 | v1ctor | 2006-06-10 14:45:43 -0700 (Sat, 10 Jun 2006) | 2 lines

fixed: unpadded UDT args were been passed to byval UDT params, it should be always padded

------------------------------------------------------------------------
r2771 | v1ctor | 2006-06-10 09:13:15 -0700 (Sat, 10 Jun 2006) | 2 lines

added: "= any" initializer for arrays parameters passed by descriptor

------------------------------------------------------------------------
r2770 | v1ctor | 2006-06-09 22:55:35 -0700 (Fri, 09 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2769 | v1ctor | 2006-06-09 22:53:27 -0700 (Fri, 09 Jun 2006) | 2 lines

small change

------------------------------------------------------------------------
r2768 | v1ctor | 2006-06-09 11:07:50 -0700 (Fri, 09 Jun 2006) | 2 lines

fixed: enum.id couldn't be used if declared inside namespaces

------------------------------------------------------------------------
r2767 | v1ctor | 2006-06-09 11:07:13 -0700 (Fri, 09 Jun 2006) | 2 lines

fixed: namespaces were been resolved when parsing the macro arguments

------------------------------------------------------------------------
r2766 | v1ctor | 2006-06-09 11:05:47 -0700 (Fri, 09 Jun 2006) | 2 lines

fixed: USING was not been removed from parent namespace's import list when going out scope

------------------------------------------------------------------------
r2765 | v1ctor | 2006-06-09 11:00:58 -0700 (Fri, 09 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2764 | v1ctor | 2006-06-09 07:33:34 -0700 (Fri, 09 Jun 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2763 | v1ctor | 2006-06-08 18:20:28 -0700 (Thu, 08 Jun 2006) | 2 lines

added: var-len string field check

------------------------------------------------------------------------
r2762 | v1ctor | 2006-06-08 07:09:53 -0700 (Thu, 08 Jun 2006) | 2 lines

small changes

------------------------------------------------------------------------
r2761 | v1ctor | 2006-06-08 06:55:05 -0700 (Thu, 08 Jun 2006) | 2 lines

small change

------------------------------------------------------------------------
r2760 | v1ctor | 2006-06-07 22:07:08 -0700 (Wed, 07 Jun 2006) | 2 lines

added: ermn and erfn, that return respectively. the names of the module and function where a runtime error occurred

------------------------------------------------------------------------
r2759 | v1ctor | 2006-06-07 21:58:41 -0700 (Wed, 07 Jun 2006) | 2 lines

added: ermn and erfn, that return respectively. the names of the module and function where a runtime error occurred

------------------------------------------------------------------------
r2758 | v1ctor | 2006-06-07 18:05:01 -0700 (Wed, 07 Jun 2006) | 2 lines

small change

------------------------------------------------------------------------
r2757 | v1ctor | 2006-06-07 18:02:25 -0700 (Wed, 07 Jun 2006) | 2 lines

new file

------------------------------------------------------------------------
r2756 | v1ctor | 2006-06-07 17:37:00 -0700 (Wed, 07 Jun 2006) | 2 lines

added: better check when branching to scope blocks with local variables inside

------------------------------------------------------------------------
r2755 | v1ctor | 2006-06-07 17:32:06 -0700 (Wed, 07 Jun 2006) | 2 lines

small change

------------------------------------------------------------------------
r2754 | v1ctor | 2006-06-07 17:26:21 -0700 (Wed, 07 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2753 | v1ctor | 2006-06-07 17:24:03 -0700 (Wed, 07 Jun 2006) | 2 lines

small change

------------------------------------------------------------------------
r2752 | v1ctor | 2006-06-07 17:23:40 -0700 (Wed, 07 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2751 | v1ctor | 2006-06-06 21:10:12 -0700 (Tue, 06 Jun 2006) | 2 lines

small changes

------------------------------------------------------------------------
r2750 | v1ctor | 2006-06-06 21:09:55 -0700 (Tue, 06 Jun 2006) | 2 lines

updated

------------------------------------------------------------------------
r2749 | v1ctor | 2006-06-06 21:03:35 -0700 (Tue, 06 Jun 2006) | 3 lines

added:
- the "= any" variable initializer, that tells the compiler to not clear the local symbol;

------------------------------------------------------------------------
r2748 | v1ctor | 2006-06-06 21:01:59 -0700 (Tue, 06 Jun 2006) | 13 lines

major changes:
- every compound statement (IF, SELECT, FOR, DO, WHILE) will open a new scope block now;
- local variables are only cleared (with 0's) when allocated, that's it, stack won't be cleared at once anymore;

added:
- the "= any" variable initializer, that's tell the compiler to now clear the local symbol;
- the erfn and ermn to retrieve respectively the function and module name when a runtime error occur;

fixed:
- stack was been wrongly poped in some cases of DIV and MOD of integers;
- resolution of overloaded function pointer arguments were not working;
- byte and short var-args were not been converted to integers;

------------------------------------------------------------------------
r2747 | v1ctor | 2006-06-06 21:01:09 -0700 (Tue, 06 Jun 2006) | 2 lines

- the erfn and ermn to retrieve respectively the function and module name when a runtime error occur;

------------------------------------------------------------------------
r2746 | v1ctor | 2006-06-06 20:41:48 -0700 (Tue, 06 Jun 2006) | 2 lines

small change

------------------------------------------------------------------------
r2745 | v1ctor | 2006-06-05 18:28:57 -0700 (Mon, 05 Jun 2006) | 2 lines

small change

------------------------------------------------------------------------
r2744 | v1ctor | 2006-06-05 18:17:10 -0700 (Mon, 05 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2743 | v1ctor | 2006-06-04 14:04:42 -0700 (Sun, 04 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2742 | v1ctor | 2006-06-03 21:45:31 -0700 (Sat, 03 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2741 | v1ctor | 2006-06-03 20:23:13 -0700 (Sat, 03 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2740 | v1ctor | 2006-06-03 14:12:26 -0700 (Sat, 03 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2739 | v1ctor | 2006-06-03 10:53:50 -0700 (Sat, 03 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2738 | v1ctor | 2006-06-02 21:25:57 -0700 (Fri, 02 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2736 | v1ctor | 2006-06-02 21:10:04 -0700 (Fri, 02 Jun 2006) | 2 lines

fixed: BOP association optimization of different types when the right operator was not converted (x86 assumption) could generate bad code

------------------------------------------------------------------------
r2735 | v1ctor | 2006-06-02 21:00:28 -0700 (Fri, 02 Jun 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2734 | v1ctor | 2006-06-02 12:16:55 -0700 (Fri, 02 Jun 2006) | 2 lines

new test

------------------------------------------------------------------------
r2733 | v1ctor | 2006-06-02 12:13:00 -0700 (Fri, 02 Jun 2006) | 2 lines

fixed: the type of the first overladed proc was been used, instead of the called one

------------------------------------------------------------------------
r2732 | v1ctor | 2006-06-01 13:31:30 -0700 (Thu, 01 Jun 2006) | 2 lines

fixed: non-decimal literal numbers were not been converted to GAS format

------------------------------------------------------------------------
r2731 | v1ctor | 2006-05-31 19:27:28 -0700 (Wed, 31 May 2006) | 2 lines

added: compound statement skipping when recovering from errors

------------------------------------------------------------------------
r2730 | v1ctor | 2006-05-31 19:22:35 -0700 (Wed, 31 May 2006) | 2 lines

small fix

------------------------------------------------------------------------
r2729 | v1ctor | 2006-05-31 19:22:20 -0700 (Wed, 31 May 2006) | 2 lines

small change

------------------------------------------------------------------------
r2728 | v1ctor | 2006-05-31 18:40:42 -0700 (Wed, 31 May 2006) | 2 lines

fixed: bitfields were not been set to the final type

------------------------------------------------------------------------
r2727 | v1ctor | 2006-05-31 18:40:09 -0700 (Wed, 31 May 2006) | 2 lines

new test

------------------------------------------------------------------------
r2726 | v1ctor | 2006-05-30 20:46:27 -0700 (Tue, 30 May 2006) | 2 lines

small change

------------------------------------------------------------------------
r2725 | v1ctor | 2006-05-30 20:45:40 -0700 (Tue, 30 May 2006) | 2 lines

added: global namespace access with ".foo" or "..foo" (if inside a WITH block)

------------------------------------------------------------------------
r2724 | v1ctor | 2006-05-30 20:44:17 -0700 (Tue, 30 May 2006) | 2 lines

new test

------------------------------------------------------------------------
r2723 | v1ctor | 2006-05-30 20:28:02 -0700 (Tue, 30 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2722 | v1ctor | 2006-05-29 18:11:52 -0700 (Mon, 29 May 2006) | 2 lines

added: non-fatal error handling - pt6

------------------------------------------------------------------------
r2721 | v1ctor | 2006-05-29 17:12:31 -0700 (Mon, 29 May 2006) | 2 lines

fixed: check for array indexes should be done before parsing the UDT fields

------------------------------------------------------------------------
r2720 | v1ctor | 2006-05-29 14:43:17 -0700 (Mon, 29 May 2006) | 2 lines

small fix

------------------------------------------------------------------------
r2719 | v1ctor | 2006-05-28 18:39:52 -0700 (Sun, 28 May 2006) | 2 lines

added: non-fatal error handling - pt5

------------------------------------------------------------------------
r2718 | v1ctor | 2006-05-28 18:39:27 -0700 (Sun, 28 May 2006) | 2 lines

cosmetic changes

------------------------------------------------------------------------
r2717 | v1ctor | 2006-05-28 18:39:11 -0700 (Sun, 28 May 2006) | 2 lines

small fix

------------------------------------------------------------------------
r2716 | v1ctor | 2006-05-28 18:32:22 -0700 (Sun, 28 May 2006) | 2 lines

small change

------------------------------------------------------------------------
r2715 | v1ctor | 2006-05-28 16:57:47 -0700 (Sun, 28 May 2006) | 2 lines

fixed: typo

------------------------------------------------------------------------
r2714 | v1ctor | 2006-05-28 12:28:53 -0700 (Sun, 28 May 2006) | 2 lines

small fix

------------------------------------------------------------------------
r2713 | v1ctor | 2006-05-28 12:18:38 -0700 (Sun, 28 May 2006) | 2 lines

fixed: typedefs couldn't be used with anonymous types

------------------------------------------------------------------------
r2712 | v1ctor | 2006-05-28 12:16:18 -0700 (Sun, 28 May 2006) | 2 lines

fixed: the level of indirection for UDT params and return type was not been take into account when creating the internal mangled names for function pointers

------------------------------------------------------------------------
r2711 | v1ctor | 2006-05-28 12:01:31 -0700 (Sun, 28 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2710 | v1ctor | 2006-05-28 12:00:36 -0700 (Sun, 28 May 2006) | 2 lines

fixed: the level of indirection of UDT params and return type was not been take into account when creating the internal mangled names for function pointers

------------------------------------------------------------------------
r2709 | v1ctor | 2006-05-28 11:42:16 -0700 (Sun, 28 May 2006) | 2 lines

new example added

------------------------------------------------------------------------
r2708 | v1ctor | 2006-05-27 18:00:01 -0700 (Sat, 27 May 2006) | 2 lines

internal change: all error module functions are now prefixed with "err"

------------------------------------------------------------------------
r2707 | v1ctor | 2006-05-27 17:57:41 -0700 (Sat, 27 May 2006) | 2 lines

fixed: anonymous namespaces were not allocating any symbol

------------------------------------------------------------------------
r2706 | v1ctor | 2006-05-27 17:57:06 -0700 (Sat, 27 May 2006) | 2 lines

fixed: debugging symbols shouldn't contain underscores or suffixes, unless mangled

------------------------------------------------------------------------
r2705 | v1ctor | 2006-05-27 12:15:37 -0700 (Sat, 27 May 2006) | 2 lines

fixed: profiler couldn't handle too long function names

------------------------------------------------------------------------
r2704 | v1ctor | 2006-05-27 12:14:27 -0700 (Sat, 27 May 2006) | 2 lines

fixed: line count wasn't been updated when skipping multi-line comments

------------------------------------------------------------------------
r2703 | v1ctor | 2006-05-27 09:20:11 -0700 (Sat, 27 May 2006) | 2 lines

fixed: ')'s were not been skipped when recovering from errors

------------------------------------------------------------------------
r2702 | v1ctor | 2006-05-27 09:19:11 -0700 (Sat, 27 May 2006) | 2 lines

cosmetic changes

------------------------------------------------------------------------
r2701 | v1ctor | 2006-05-27 09:18:50 -0700 (Sat, 27 May 2006) | 2 lines

internal change: hash tb added to process cmd-line options

------------------------------------------------------------------------
r2700 | v1ctor | 2006-05-26 21:23:05 -0700 (Fri, 26 May 2006) | 2 lines

small fix

------------------------------------------------------------------------
r2699 | v1ctor | 2006-05-26 21:15:59 -0700 (Fri, 26 May 2006) | 2 lines

small fix

------------------------------------------------------------------------
r2698 | v1ctor | 2006-05-26 15:39:55 -0700 (Fri, 26 May 2006) | 2 lines

added: multi-line comments

------------------------------------------------------------------------
r2697 | v1ctor | 2006-05-26 14:14:40 -0700 (Fri, 26 May 2006) | 2 lines

cosmetic changes

------------------------------------------------------------------------
r2696 | v1ctor | 2006-05-26 13:04:04 -0700 (Fri, 26 May 2006) | 3 lines

changed: chars over 127 won't be converted to UTF-8, to allow UTF-8
string to be printed too

------------------------------------------------------------------------
r2695 | v1ctor | 2006-05-26 11:38:16 -0700 (Fri, 26 May 2006) | 2 lines

small fixes

------------------------------------------------------------------------
r2694 | v1ctor | 2006-05-25 21:48:18 -0700 (Thu, 25 May 2006) | 2 lines

small change

------------------------------------------------------------------------
r2693 | v1ctor | 2006-05-25 21:43:27 -0700 (Thu, 25 May 2006) | 4 lines

changed: no more limits in the number of function parameters;

added: non-fatal error handling - pt5;

------------------------------------------------------------------------
r2692 | v1ctor | 2006-05-25 17:39:27 -0700 (Thu, 25 May 2006) | 2 lines

small change

------------------------------------------------------------------------
r2691 | v1ctor | 2006-05-25 08:54:50 -0700 (Thu, 25 May 2006) | 2 lines

small change

------------------------------------------------------------------------
r2690 | v1ctor | 2006-05-24 21:52:17 -0700 (Wed, 24 May 2006) | 2 lines

fixed: nested anonymous types, parent's offset wasn't been added

------------------------------------------------------------------------
r2689 | v1ctor | 2006-05-24 21:48:55 -0700 (Wed, 24 May 2006) | 2 lines

new test

------------------------------------------------------------------------
r2688 | v1ctor | 2006-05-24 15:34:59 -0700 (Wed, 24 May 2006) | 2 lines

added: non-fatal error handling - pt4

------------------------------------------------------------------------
r2687 | v1ctor | 2006-05-24 10:33:42 -0700 (Wed, 24 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2686 | v1ctor | 2006-05-24 10:33:08 -0700 (Wed, 24 May 2006) | 2 lines

fixed: dereference of address-of optimization

------------------------------------------------------------------------
r2685 | v1ctor | 2006-05-23 22:47:48 -0700 (Tue, 23 May 2006) | 2 lines

added: better function ptr error report (with demangle)

------------------------------------------------------------------------
r2684 | v1ctor | 2006-05-23 21:47:09 -0700 (Tue, 23 May 2006) | 2 lines

added: better function ptr error report (with demangle)

------------------------------------------------------------------------
r2683 | v1ctor | 2006-05-23 14:38:58 -0700 (Tue, 23 May 2006) | 2 lines

fixed: arg/param level of indirection check

------------------------------------------------------------------------
r2682 | v1ctor | 2006-05-23 11:11:29 -0700 (Tue, 23 May 2006) | 2 lines

added: the -maxerr option

------------------------------------------------------------------------
r2681 | v1ctor | 2006-05-23 11:10:37 -0700 (Tue, 23 May 2006) | 2 lines

added: non-fatal error handling - pt3

------------------------------------------------------------------------
r2680 | v1ctor | 2006-05-23 11:09:42 -0700 (Tue, 23 May 2006) | 2 lines

small fix

------------------------------------------------------------------------
r2679 | v1ctor | 2006-05-23 11:09:12 -0700 (Tue, 23 May 2006) | 2 lines

cosmetic changes

------------------------------------------------------------------------
r2678 | v1ctor | 2006-05-22 15:00:07 -0700 (Mon, 22 May 2006) | 2 lines

small fix

------------------------------------------------------------------------
r2677 | v1ctor | 2006-05-22 14:50:10 -0700 (Mon, 22 May 2006) | 2 lines

small change

------------------------------------------------------------------------
r2676 | v1ctor | 2006-05-22 14:49:00 -0700 (Mon, 22 May 2006) | 2 lines

added: non-fatal error handling - pt2

------------------------------------------------------------------------
r2675 | v1ctor | 2006-05-22 13:46:15 -0700 (Mon, 22 May 2006) | 2 lines

small fix

------------------------------------------------------------------------
r2674 | v1ctor | 2006-05-21 20:14:15 -0700 (Sun, 21 May 2006) | 2 lines

small update

------------------------------------------------------------------------
r2673 | v1ctor | 2006-05-21 19:51:02 -0700 (Sun, 21 May 2006) | 2 lines

changed: #define's will be always added to the global hash tb, lexer can't resolve namespaces

------------------------------------------------------------------------
r2672 | v1ctor | 2006-05-21 15:22:27 -0700 (Sun, 21 May 2006) | 2 lines

added: non-fatal error handling

------------------------------------------------------------------------
r2671 | v1ctor | 2006-05-19 19:01:41 -0700 (Fri, 19 May 2006) | 2 lines

new formatting updates

------------------------------------------------------------------------
r2670 | v1ctor | 2006-05-18 21:21:46 -0700 (Thu, 18 May 2006) | 2 lines

new tests

------------------------------------------------------------------------
r2669 | v1ctor | 2006-05-18 19:17:04 -0700 (Thu, 18 May 2006) | 2 lines

new directory

------------------------------------------------------------------------
r2668 | v1ctor | 2006-05-18 19:16:31 -0700 (Thu, 18 May 2006) | 2 lines

new test

------------------------------------------------------------------------
r2667 | v1ctor | 2006-05-18 19:16:26 -0700 (Thu, 18 May 2006) | 2 lines

added: dup definitions of global namespace symbols inside other namespaces are now better handled

------------------------------------------------------------------------
r2666 | v1ctor | 2006-05-17 10:35:01 -0700 (Wed, 17 May 2006) | 4 lines

added: namespace checks when declaring symbols outside the original namespaces;

changed: parser-decl-symb moved to parser-decl-var;

------------------------------------------------------------------------
r2665 | v1ctor | 2006-05-17 09:04:51 -0700 (Wed, 17 May 2006) | 2 lines

added: support for non-decimal numbers

------------------------------------------------------------------------
r2664 | v1ctor | 2006-05-15 18:14:42 -0700 (Mon, 15 May 2006) | 2 lines

added: checks for periods in symbols declared inside namespaces

------------------------------------------------------------------------
r2663 | v1ctor | 2006-05-15 18:13:44 -0700 (Mon, 15 May 2006) | 2 lines

fixed: no check for literal strings was been done

------------------------------------------------------------------------
r2662 | v1ctor | 2006-05-15 12:46:29 -0700 (Mon, 15 May 2006) | 2 lines

added: links to libraries

------------------------------------------------------------------------
r2661 | v1ctor | 2006-05-15 12:37:31 -0700 (Mon, 15 May 2006) | 2 lines

added: gif_lib

------------------------------------------------------------------------
r2660 | v1ctor | 2006-05-15 11:51:25 -0700 (Mon, 15 May 2006) | 2 lines

js lib added

------------------------------------------------------------------------
r2659 | v1ctor | 2006-05-15 11:48:26 -0700 (Mon, 15 May 2006) | 2 lines

examples added

------------------------------------------------------------------------
r2658 | v1ctor | 2006-05-15 11:46:37 -0700 (Mon, 15 May 2006) | 2 lines

headers added

------------------------------------------------------------------------
r2657 | v1ctor | 2006-05-15 10:43:44 -0700 (Mon, 15 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2656 | v1ctor | 2006-05-14 21:58:36 -0700 (Sun, 14 May 2006) | 2 lines

fixed: UDT's aliases shouldn't be stored when mangling function params

------------------------------------------------------------------------
r2655 | v1ctor | 2006-05-14 15:31:01 -0700 (Sun, 14 May 2006) | 2 lines

changed: using Tt now to define UDT's, as in g++

------------------------------------------------------------------------
r2654 | v1ctor | 2006-05-14 15:27:30 -0700 (Sun, 14 May 2006) | 2 lines

warning fix

------------------------------------------------------------------------
r2653 | v1ctor | 2006-05-14 15:26:35 -0700 (Sun, 14 May 2006) | 2 lines

fixed: SUB function ptr double deref

------------------------------------------------------------------------
r2652 | v1ctor | 2006-05-14 15:24:32 -0700 (Sun, 14 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2651 | v1ctor | 2006-05-14 13:48:42 -0700 (Sun, 14 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2650 | v1ctor | 2006-05-13 20:47:15 -0700 (Sat, 13 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2649 | v1ctor | 2006-05-13 20:44:51 -0700 (Sat, 13 May 2006) | 2 lines

changed: extern "c" added, aliases removed

------------------------------------------------------------------------
r2648 | v1ctor | 2006-05-13 20:27:51 -0700 (Sat, 13 May 2006) | 2 lines

added: the ALIAS option to TYPE, UNION, ENUM and NAMESPACE blocks, to be used with C++ when names clash with FB reserved words

------------------------------------------------------------------------
r2647 | v1ctor | 2006-05-13 20:20:50 -0700 (Sat, 13 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2646 | v1ctor | 2006-05-13 15:00:59 -0700 (Sat, 13 May 2006) | 2 lines

fixed: mangling of variables declared inside namespaces

------------------------------------------------------------------------
r2645 | v1ctor | 2006-05-13 15:00:59 -0700 (Sat, 13 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2644 | v1ctor | 2006-05-13 13:48:53 -0700 (Sat, 13 May 2006) | 2 lines

changed: extern "c" added, aliases removed

------------------------------------------------------------------------
r2643 | v1ctor | 2006-05-13 12:56:18 -0700 (Sat, 13 May 2006) | 4 lines

changed:
- extern can be now nested;
- compound stmts are only checked when not parsing include files;

------------------------------------------------------------------------
r2642 | v1ctor | 2006-05-13 12:08:53 -0700 (Sat, 13 May 2006) | 2 lines

changed: header exchanged by a machine-translated one

------------------------------------------------------------------------
r2641 | v1ctor | 2006-05-13 12:01:42 -0700 (Sat, 13 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2640 | v1ctor | 2006-05-13 11:55:41 -0700 (Sat, 13 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2639 | v1ctor | 2006-05-13 11:48:56 -0700 (Sat, 13 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2638 | v1ctor | 2006-05-13 11:27:32 -0700 (Sat, 13 May 2006) | 10 lines

added:
- name spaces;
- extern blocks;
- internal: C++ mangling (GCC 3.x compat);
- internal: memory pool module;

fixed:
- bitfields were been allocated on main()'s sym tb;
- non-decimal 64-bit literal constants were been sign-extended if < 2^32;

------------------------------------------------------------------------
r2637 | v1ctor | 2006-05-13 11:17:56 -0700 (Sat, 13 May 2006) | 2 lines

added: jpeg and png libs

------------------------------------------------------------------------
r2636 | v1ctor | 2006-05-13 11:17:30 -0700 (Sat, 13 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2635 | v1ctor | 2006-05-13 11:12:11 -0700 (Sat, 13 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2634 | v1ctor | 2006-05-13 10:15:58 -0700 (Sat, 13 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2633 | v1ctor | 2006-05-13 08:01:30 -0700 (Sat, 13 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2632 | v1ctor | 2006-05-13 07:57:57 -0700 (Sat, 13 May 2006) | 2 lines

dups removed

------------------------------------------------------------------------
r2631 | v1ctor | 2006-05-13 07:57:18 -0700 (Sat, 13 May 2006) | 2 lines

latest version

------------------------------------------------------------------------
r2630 | v1ctor | 2006-05-13 07:56:04 -0700 (Sat, 13 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2629 | v1ctor | 2006-05-13 07:52:42 -0700 (Sat, 13 May 2006) | 4 lines

fixed:
- INPUT was not skipping white spaces;
- WRITE was outputing fp numbers with quotes;

------------------------------------------------------------------------
r2628 | v1ctor | 2006-05-06 11:48:37 -0700 (Sat, 06 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2627 | v1ctor | 2006-05-04 10:51:07 -0700 (Thu, 04 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2626 | v1ctor | 2006-05-03 07:51:54 -0700 (Wed, 03 May 2006) | 2 lines

fixed: $ and * used as ordinary non-formatting chars

------------------------------------------------------------------------
r2624 | v1ctor | 2006-05-02 18:55:39 -0700 (Tue, 02 May 2006) | 2 lines

internal changes: linked-lists header removed from client code

------------------------------------------------------------------------
r2622 | v1ctor | 2006-05-02 17:54:12 -0700 (Tue, 02 May 2006) | 2 lines

fixed: recursive parsing of compound statements

------------------------------------------------------------------------
r2621 | v1ctor | 2006-05-02 12:29:03 -0700 (Tue, 02 May 2006) | 7 lines

added:
- CONST AS SymbolType;
- ENUM's in CONST;

changed:
- for CONST's with explicit type, no inference will be done, r-side value will be converted;

------------------------------------------------------------------------
r2620 | v1ctor | 2006-05-02 12:28:19 -0700 (Tue, 02 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2619 | v1ctor | 2006-05-02 12:21:09 -0700 (Tue, 02 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2618 | v1ctor | 2006-05-02 12:19:34 -0700 (Tue, 02 May 2006) | 7 lines

added:
- CONST AS SymbolType;
- ENUM's in CONST;

changed:
- for CONST's with explicit type, no inference will be done, r-side value will be converted;

------------------------------------------------------------------------
r2617 | v1ctor | 2006-05-01 21:56:30 -0700 (Mon, 01 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2615 | v1ctor | 2006-05-01 21:53:45 -0700 (Mon, 01 May 2006) | 2 lines

fixed: recursive parsing of compound statements

------------------------------------------------------------------------
r2614 | v1ctor | 2006-05-01 21:46:36 -0700 (Mon, 01 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2613 | v1ctor | 2006-05-01 21:09:57 -0700 (Mon, 01 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2612 | v1ctor | 2006-05-01 07:00:11 -0700 (Mon, 01 May 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2611 | v1ctor | 2006-04-30 19:39:17 -0700 (Sun, 30 Apr 2006) | 2 lines

fixed: bug #1479093

------------------------------------------------------------------------
r2610 | v1ctor | 2006-04-30 19:33:00 -0700 (Sun, 30 Apr 2006) | 2 lines

fixed: bug #1479085

------------------------------------------------------------------------
r2609 | v1ctor | 2006-04-29 17:12:32 -0700 (Sat, 29 Apr 2006) | 2 lines

changed: header exchanged by a machine-translated one

------------------------------------------------------------------------
r2608 | v1ctor | 2006-04-29 11:38:11 -0700 (Sat, 29 Apr 2006) | 2 lines

updated: alleg 4.1 changes

------------------------------------------------------------------------
r2607 | v1ctor | 2006-04-29 10:09:22 -0700 (Sat, 29 Apr 2006) | 2 lines

fixed: gcc 4.x warnings

------------------------------------------------------------------------
r2606 | v1ctor | 2006-04-29 09:43:08 -0700 (Sat, 29 Apr 2006) | 2 lines

internal changes: the include files table is now a hash table

------------------------------------------------------------------------
r2605 | v1ctor | 2006-04-29 08:36:12 -0700 (Sat, 29 Apr 2006) | 2 lines

changed: header exchanged by a machine-translated one

------------------------------------------------------------------------
r2604 | v1ctor | 2006-04-28 23:15:33 -0700 (Fri, 28 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2603 | v1ctor | 2006-04-28 23:11:23 -0700 (Fri, 28 Apr 2006) | 2 lines

changed: header exchanged by a machine-translated one

------------------------------------------------------------------------
r2602 | v1ctor | 2006-04-28 23:10:12 -0700 (Fri, 28 Apr 2006) | 2 lines

changed: platform-dependent symbols moved to different directories

------------------------------------------------------------------------
r2601 | v1ctor | 2006-04-28 17:04:25 -0700 (Fri, 28 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2600 | v1ctor | 2006-04-28 17:02:49 -0700 (Fri, 28 Apr 2006) | 6 lines

fixed:
- auto-coercion of arguments to pointer parameters;

changed:
- more restrict pointer resolution in overloaded procs;

------------------------------------------------------------------------
r2599 | v1ctor | 2006-04-27 23:31:14 -0700 (Thu, 27 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2598 | v1ctor | 2006-04-27 21:14:02 -0700 (Thu, 27 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2597 | v1ctor | 2006-04-27 19:16:15 -0700 (Thu, 27 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2596 | v1ctor | 2006-04-27 18:15:33 -0700 (Thu, 27 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2595 | v1ctor | 2006-04-27 18:14:40 -0700 (Thu, 27 Apr 2006) | 6 lines

changed:
- msvcrt-only prototypes moved to #ifdef's

added:
- DJGPP-only declarations (incomplete)

------------------------------------------------------------------------
r2594 | v1ctor | 2006-04-25 09:58:13 -0700 (Tue, 25 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2593 | v1ctor | 2006-04-25 07:44:23 -0700 (Tue, 25 Apr 2006) | 2 lines

fixed: pointer deref checking (with -exx) missing in string indexing

------------------------------------------------------------------------
r2592 | v1ctor | 2006-04-25 07:29:09 -0700 (Tue, 25 Apr 2006) | 2 lines

fixed: subtraction of void pointers

------------------------------------------------------------------------
r2591 | v1ctor | 2006-04-24 21:40:30 -0700 (Mon, 24 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2590 | v1ctor | 2006-04-24 18:14:35 -0700 (Mon, 24 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2589 | v1ctor | 2006-04-23 14:28:57 -0700 (Sun, 23 Apr 2006) | 2 lines

added: missing defines

------------------------------------------------------------------------
r2588 | v1ctor | 2006-04-23 13:15:33 -0700 (Sun, 23 Apr 2006) | 2 lines

fixed: back-patching forward declarations used as function results would change the functions parameters lenght, causing stack errors

------------------------------------------------------------------------
r2587 | v1ctor | 2006-04-23 13:10:24 -0700 (Sun, 23 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2586 | i_am_drv | 2006-04-23 07:15:34 -0700 (Sun, 23 Apr 2006) | 2 lines

added void * to thread struct for dos

------------------------------------------------------------------------
r2585 | v1ctor | 2006-04-22 21:05:15 -0700 (Sat, 22 Apr 2006) | 2 lines

added: missing defines

------------------------------------------------------------------------
r2584 | v1ctor | 2006-04-22 11:38:34 -0700 (Sat, 22 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2583 | v1ctor | 2006-04-22 11:22:53 -0700 (Sat, 22 Apr 2006) | 2 lines

fixed: wrong type casting

------------------------------------------------------------------------
r2582 | v1ctor | 2006-04-21 13:11:51 -0700 (Fri, 21 Apr 2006) | 2 lines

removed: not updated and not needed anymore, the official docs are at the Wiki

------------------------------------------------------------------------
r2581 | v1ctor | 2006-04-21 10:59:06 -0700 (Fri, 21 Apr 2006) | 2 lines

fixed: bug #1472883

------------------------------------------------------------------------
r2580 | v1ctor | 2006-04-21 10:49:04 -0700 (Fri, 21 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2579 | v1ctor | 2006-04-21 10:44:33 -0700 (Fri, 21 Apr 2006) | 2 lines

fixed: the max number of params wasn't been checked

------------------------------------------------------------------------
r2578 | v1ctor | 2006-04-21 10:43:50 -0700 (Fri, 21 Apr 2006) | 4 lines

fixed: descriptors of local dynamic arrays with unknown dimensions weren't been reseted

changed: max array dimensions changed to 8 to speed up allocation of arrays with unknown dimensions

------------------------------------------------------------------------
r2577 | v1ctor | 2006-04-21 10:40:22 -0700 (Fri, 21 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2576 | v1ctor | 2006-04-21 10:35:07 -0700 (Fri, 21 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2575 | v1ctor | 2006-04-20 18:27:36 -0700 (Thu, 20 Apr 2006) | 2 lines

bug not present in 0.15

------------------------------------------------------------------------
r2574 | v1ctor | 2006-04-20 18:07:58 -0700 (Thu, 20 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2573 | v1ctor | 2006-04-20 18:07:06 -0700 (Thu, 20 Apr 2006) | 4 lines

fixed:
- bitfields couldn't be used in PRINT, WRITE or [W}STR;
- enum's couldn't be used in [W}STR;

------------------------------------------------------------------------
r2572 | v1ctor | 2006-04-20 11:23:05 -0700 (Thu, 20 Apr 2006) | 2 lines

changed: WIN_INCLUDEPROXY

------------------------------------------------------------------------
r2571 | v1ctor | 2006-04-19 21:07:20 -0700 (Wed, 19 Apr 2006) | 2 lines

fixed: round errors

------------------------------------------------------------------------
r2570 | v1ctor | 2006-04-18 23:54:48 -0700 (Tue, 18 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2569 | v1ctor | 2006-04-18 23:31:08 -0700 (Tue, 18 Apr 2006) | 2 lines

added: unsigned versions of INPUT

------------------------------------------------------------------------
r2568 | v1ctor | 2006-04-18 23:30:43 -0700 (Tue, 18 Apr 2006) | 2 lines

changed: more QB-ish INPUT

------------------------------------------------------------------------
r2567 | v1ctor | 2006-04-18 23:29:53 -0700 (Tue, 18 Apr 2006) | 2 lines

changed: missing config symbols moved to fb_config.h

------------------------------------------------------------------------
r2566 | v1ctor | 2006-04-17 17:57:11 -0700 (Mon, 17 Apr 2006) | 2 lines

added: the __FB_OUT_###__ defines

------------------------------------------------------------------------
r2565 | v1ctor | 2006-04-17 17:56:42 -0700 (Mon, 17 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2564 | v1ctor | 2006-04-17 17:21:33 -0700 (Mon, 17 Apr 2006) | 2 lines

changed: _ESCAPE__ to $

------------------------------------------------------------------------
r2563 | v1ctor | 2006-04-17 17:21:27 -0700 (Mon, 17 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2562 | v1ctor | 2006-04-17 17:20:21 -0700 (Mon, 17 Apr 2006) | 2 lines

fixed: SCREEN() was returning an signed int

------------------------------------------------------------------------
r2561 | v1ctor | 2006-04-17 17:19:45 -0700 (Mon, 17 Apr 2006) | 2 lines

changed: escape seq handling moved off the lexer module

------------------------------------------------------------------------
r2560 | v1ctor | 2006-04-17 17:18:25 -0700 (Mon, 17 Apr 2006) | 2 lines

added: EXIT SELECT

------------------------------------------------------------------------
r2559 | v1ctor | 2006-04-17 14:16:52 -0700 (Mon, 17 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2558 | v1ctor | 2006-04-17 13:39:15 -0700 (Mon, 17 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2557 | v1ctor | 2006-04-17 01:14:42 -0700 (Mon, 17 Apr 2006) | 2 lines

fixed: SCREEN() was returning a signed int

------------------------------------------------------------------------
r2556 | v1ctor | 2006-04-15 21:14:45 -0700 (Sat, 15 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2555 | v1ctor | 2006-04-15 21:13:10 -0700 (Sat, 15 Apr 2006) | 4 lines

added:
- the $ prefix to literal strings, to turn off escaping when Option Escape is set;
- literal strings can be concatenated just by grouping them, w/o + or &;

------------------------------------------------------------------------
r2554 | v1ctor | 2006-04-15 18:30:34 -0700 (Sat, 15 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2553 | v1ctor | 2006-04-15 15:12:35 -0700 (Sat, 15 Apr 2006) | 2 lines

updated: copyright date

------------------------------------------------------------------------
r2552 | v1ctor | 2006-04-15 15:04:24 -0700 (Sat, 15 Apr 2006) | 2 lines

updated: copyright and license

------------------------------------------------------------------------
r2551 | v1ctor | 2006-04-14 22:33:13 -0700 (Fri, 14 Apr 2006) | 2 lines

fixed: typedef to itself was doing a case sensitive test

------------------------------------------------------------------------
r2550 | v1ctor | 2006-04-14 22:31:51 -0700 (Fri, 14 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2549 | v1ctor | 2006-04-14 21:57:13 -0700 (Fri, 14 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2548 | v1ctor | 2006-04-14 21:55:19 -0700 (Fri, 14 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2547 | v1ctor | 2006-04-14 21:53:01 -0700 (Fri, 14 Apr 2006) | 2 lines

fixed: "end if" missing in ASSERT

------------------------------------------------------------------------
r2546 | v1ctor | 2006-04-14 21:35:44 -0700 (Fri, 14 Apr 2006) | 2 lines

added: missing defines

------------------------------------------------------------------------
r2545 | v1ctor | 2006-04-13 18:32:25 -0700 (Thu, 13 Apr 2006) | 2 lines

changed: accept W|ZSTRING in typedefs without PTR's

------------------------------------------------------------------------
r2544 | v1ctor | 2006-04-13 18:31:30 -0700 (Thu, 13 Apr 2006) | 2 lines

changed: byte to zstring, any to wchar

------------------------------------------------------------------------
r2543 | v1ctor | 2006-04-13 18:29:44 -0700 (Thu, 13 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2542 | v1ctor | 2006-04-13 13:42:11 -0700 (Thu, 13 Apr 2006) | 2 lines

added: #inclib "advapi32"

------------------------------------------------------------------------
r2541 | v1ctor | 2006-04-12 11:44:19 -0700 (Wed, 12 Apr 2006) | 2 lines

fixed: temporary variables allocated inside scope blocks were been allocated in the proc scope

------------------------------------------------------------------------
r2540 | v1ctor | 2006-04-12 11:39:08 -0700 (Wed, 12 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2539 | v1ctor | 2006-04-12 09:06:00 -0700 (Wed, 12 Apr 2006) | 2 lines

fixed: bug #1469342

------------------------------------------------------------------------
r2538 | v1ctor | 2006-04-11 23:39:32 -0700 (Tue, 11 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2537 | v1ctor | 2006-04-11 23:38:26 -0700 (Tue, 11 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2536 | v1ctor | 2006-04-11 21:48:32 -0700 (Tue, 11 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2535 | v1ctor | 2006-04-11 19:29:07 -0700 (Tue, 11 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2534 | v1ctor | 2006-04-11 19:03:14 -0700 (Tue, 11 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2533 | v1ctor | 2006-04-11 18:56:47 -0700 (Tue, 11 Apr 2006) | 2 lines

#ifdef update

------------------------------------------------------------------------
r2532 | v1ctor | 2006-04-11 18:49:28 -0700 (Tue, 11 Apr 2006) | 2 lines

#ifdef update

------------------------------------------------------------------------
r2531 | v1ctor | 2006-04-11 18:11:04 -0700 (Tue, 11 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2530 | v1ctor | 2006-04-11 15:33:26 -0700 (Tue, 11 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2529 | v1ctor | 2006-04-10 23:46:38 -0700 (Mon, 10 Apr 2006) | 2 lines

added: new libraries

------------------------------------------------------------------------
r2528 | v1ctor | 2006-04-10 23:28:44 -0700 (Mon, 10 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2527 | v1ctor | 2006-04-10 23:09:43 -0700 (Mon, 10 Apr 2006) | 2 lines

added: __FB_OPTION_ESCAPE__

------------------------------------------------------------------------
r2526 | v1ctor | 2006-04-10 22:56:25 -0700 (Mon, 10 Apr 2006) | 2 lines

added: __FB_OPTION_###__ defines

------------------------------------------------------------------------
r2525 | v1ctor | 2006-04-10 22:56:25 -0700 (Mon, 10 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2524 | v1ctor | 2006-04-10 21:12:48 -0700 (Mon, 10 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2523 | v1ctor | 2006-04-10 20:42:23 -0700 (Mon, 10 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2522 | v1ctor | 2006-04-10 20:33:18 -0700 (Mon, 10 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2521 | v1ctor | 2006-04-10 14:41:45 -0700 (Mon, 10 Apr 2006) | 2 lines

added: dDOUBLE

------------------------------------------------------------------------
r2520 | v1ctor | 2006-04-10 14:32:22 -0700 (Mon, 10 Apr 2006) | 2 lines

added: request #1431128

------------------------------------------------------------------------
r2519 | v1ctor | 2006-04-08 20:56:25 -0700 (Sat, 08 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2518 | v1ctor | 2006-04-08 20:55:50 -0700 (Sat, 08 Apr 2006) | 2 lines

changed: the INPUT mode is (again) allowed

------------------------------------------------------------------------
r2517 | v1ctor | 2006-04-08 18:47:11 -0700 (Sat, 08 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2516 | v1ctor | 2006-04-08 18:44:04 -0700 (Sat, 08 Apr 2006) | 2 lines

changed: the INPUT mode is allowed now

------------------------------------------------------------------------
r2515 | v1ctor | 2006-04-08 18:33:31 -0700 (Sat, 08 Apr 2006) | 2 lines

fixed: typo in cygwin version

------------------------------------------------------------------------
r2514 | v1ctor | 2006-04-05 18:22:42 -0700 (Wed, 05 Apr 2006) | 2 lines

small speed up

------------------------------------------------------------------------
r2513 | v1ctor | 2006-04-05 11:21:02 -0700 (Wed, 05 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2512 | v1ctor | 2006-04-04 20:03:54 -0700 (Tue, 04 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2511 | v1ctor | 2006-04-04 20:02:44 -0700 (Tue, 04 Apr 2006) | 2 lines

fixed: crash when the function body wasn't complete parsed due errors

------------------------------------------------------------------------
r2510 | v1ctor | 2006-04-04 18:30:00 -0700 (Tue, 04 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2509 | v1ctor | 2006-04-04 17:25:42 -0700 (Tue, 04 Apr 2006) | 2 lines

minor change: fixed-len strings to pointers to reduce the fbc size

------------------------------------------------------------------------
r2508 | v1ctor | 2006-04-04 17:24:30 -0700 (Tue, 04 Apr 2006) | 2 lines

fixed: crash when the function body wasn't complete parsed due errors

------------------------------------------------------------------------
r2507 | v1ctor | 2006-04-04 17:23:26 -0700 (Tue, 04 Apr 2006) | 2 lines

fixed: backward branches would erase all dyn vars, not the ones defined after the target label only

------------------------------------------------------------------------
r2506 | v1ctor | 2006-04-04 17:22:00 -0700 (Tue, 04 Apr 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2505 | v1ctor | 2006-04-04 17:21:41 -0700 (Tue, 04 Apr 2006) | 2 lines

#ifdef clashes

------------------------------------------------------------------------
r2504 | v1ctor | 2006-04-04 17:20:52 -0700 (Tue, 04 Apr 2006) | 2 lines

IMPORT used with GUID's

------------------------------------------------------------------------
r2503 | v1ctor | 2006-03-28 20:38:14 -0800 (Tue, 28 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2502 | v1ctor | 2006-03-28 20:36:01 -0800 (Tue, 28 Mar 2006) | 3 lines

changed: Dir() attrib's by defaylt shouldn't include directories, hidden or system files
added: optional out_attrib param to Dir()

------------------------------------------------------------------------
r2501 | v1ctor | 2006-03-28 20:34:52 -0800 (Tue, 28 Mar 2006) | 2 lines

added: optional out_attrib param to Dir()

------------------------------------------------------------------------
r2500 | v1ctor | 2006-03-28 17:15:33 -0800 (Tue, 28 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2499 | v1ctor | 2006-03-28 16:58:27 -0800 (Tue, 28 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2498 | v1ctor | 2006-03-26 08:06:35 -0800 (Sun, 26 Mar 2006) | 2 lines

added: the library mingwex to LD's list, autoheader will flag snprintf() in new mingw versions

------------------------------------------------------------------------
r2497 | v1ctor | 2006-03-24 10:14:28 -0800 (Fri, 24 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2496 | v1ctor | 2006-03-24 10:13:12 -0800 (Fri, 24 Mar 2006) | 2 lines

fixed: enum's weren't been allowed with CAST()

------------------------------------------------------------------------
r2495 | v1ctor | 2006-03-24 09:52:06 -0800 (Fri, 24 Mar 2006) | 2 lines

fixed: no libraries were been passed to LD when linking shared libraries

------------------------------------------------------------------------
r2494 | v1ctor | 2006-03-23 17:03:12 -0800 (Thu, 23 Mar 2006) | 2 lines

changed: descriptors of static extern arrays won't be emitted unless accessed

------------------------------------------------------------------------
r2493 | v1ctor | 2006-03-23 16:41:20 -0800 (Thu, 23 Mar 2006) | 2 lines

changed: pointer args passed byref to pointer params were not been checked

------------------------------------------------------------------------
r2492 | v1ctor | 2006-03-22 19:31:04 -0800 (Wed, 22 Mar 2006) | 2 lines

fixed: assignments of complex l-hand side UDT's by type-ini were crashing due too early optimizations

------------------------------------------------------------------------
r2491 | v1ctor | 2006-03-22 19:29:13 -0800 (Wed, 22 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2490 | v1ctor | 2006-03-22 13:37:45 -0800 (Wed, 22 Mar 2006) | 2 lines

EXIT SUB | FUNCTION updates

------------------------------------------------------------------------
r2489 | v1ctor | 2006-03-22 13:35:14 -0800 (Wed, 22 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2488 | v1ctor | 2006-03-22 13:32:45 -0800 (Wed, 22 Mar 2006) | 2 lines

added: EXIT SUB | FUNCTION check

------------------------------------------------------------------------
r2487 | v1ctor | 2006-03-22 13:15:27 -0800 (Wed, 22 Mar 2006) | 2 lines

added: macro recursion check, top level only

------------------------------------------------------------------------
r2486 | v1ctor | 2006-03-22 12:02:48 -0800 (Wed, 22 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2485 | v1ctor | 2006-03-22 12:01:40 -0800 (Wed, 22 Mar 2006) | 2 lines

removed: nokeyword POINT

------------------------------------------------------------------------
r2484 | v1ctor | 2006-03-22 12:01:25 -0800 (Wed, 22 Mar 2006) | 2 lines

changed: UDT's can now be named with keywords

------------------------------------------------------------------------
r2483 | v1ctor | 2006-03-22 11:58:43 -0800 (Wed, 22 Mar 2006) | 2 lines

cptr's changed to cast's

------------------------------------------------------------------------
r2482 | v1ctor | 2006-03-22 10:41:21 -0800 (Wed, 22 Mar 2006) | 2 lines

no calls to assert() unless in DEBUG mode

------------------------------------------------------------------------
r2481 | v1ctor | 2006-03-22 09:37:29 -0800 (Wed, 22 Mar 2006) | 2 lines

added: EXIT, RETURN and GOTO now destroy objects or dyn arrays defined inside scope blocks

------------------------------------------------------------------------
r2480 | v1ctor | 2006-03-19 11:25:52 -0800 (Sun, 19 Mar 2006) | 2 lines

fixed: lib's name in anything but Win32

------------------------------------------------------------------------
r2479 | v1ctor | 2006-03-18 13:11:44 -0800 (Sat, 18 Mar 2006) | 2 lines

removed: errno from allegro_init(), every C lib has its own implementation as a macro, function or variable, there's no way to support that

------------------------------------------------------------------------
r2478 | lillo | 2006-03-17 07:06:37 -0800 (Fri, 17 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2476 | lillo | 2006-03-17 07:05:28 -0800 (Fri, 17 Mar 2006) | 2 lines

Compiler-side support for drawing modes to DRAW STRING

------------------------------------------------------------------------
r2475 | lillo | 2006-03-17 07:04:30 -0800 (Fri, 17 Mar 2006) | 2 lines

Fixed ALPHA PUT crash when alpha is zero; added full drawing mode support to DRAW STRING, so it has the same capabilities of PUT

------------------------------------------------------------------------
r2474 | lillo | 2006-03-17 07:02:20 -0800 (Fri, 17 Mar 2006) | 2 lines

Made WIDTH under Linux to clear console on call

------------------------------------------------------------------------
r2473 | v1ctor | 2006-03-16 17:57:41 -0800 (Thu, 16 Mar 2006) | 2 lines

changed: ast_procnode moved to ast_node

------------------------------------------------------------------------
r2472 | v1ctor | 2006-03-16 17:46:41 -0800 (Thu, 16 Mar 2006) | 2 lines

changed: ast_procnode moved to ast_node

------------------------------------------------------------------------
r2471 | v1ctor | 2006-03-16 16:54:10 -0800 (Thu, 16 Mar 2006) | 2 lines

cosmetic changes: ___Arg renamed to ___Param and vice-versa

------------------------------------------------------------------------
r2470 | v1ctor | 2006-03-14 21:26:25 -0800 (Tue, 14 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2469 | v1ctor | 2006-03-14 21:20:25 -0800 (Tue, 14 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2468 | v1ctor | 2006-03-14 20:53:40 -0800 (Tue, 14 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2467 | v1ctor | 2006-03-14 20:41:07 -0800 (Tue, 14 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2466 | v1ctor | 2006-03-14 20:35:12 -0800 (Tue, 14 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2465 | v1ctor | 2006-03-14 17:37:47 -0800 (Tue, 14 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2464 | v1ctor | 2006-03-14 17:07:39 -0800 (Tue, 14 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2463 | v1ctor | 2006-03-14 16:33:29 -0800 (Tue, 14 Mar 2006) | 2 lines

changed: pngconf.bi merged into png.bi

------------------------------------------------------------------------
r2462 | v1ctor | 2006-03-14 08:38:00 -0800 (Tue, 14 Mar 2006) | 2 lines

added: libpng

------------------------------------------------------------------------
r2461 | v1ctor | 2006-03-14 08:30:37 -0800 (Tue, 14 Mar 2006) | 2 lines

changed: atof() to strtod()

------------------------------------------------------------------------
r2460 | v1ctor | 2006-03-14 07:59:55 -0800 (Tue, 14 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2459 | v1ctor | 2006-03-14 07:53:23 -0800 (Tue, 14 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2457 | v1ctor | 2006-03-14 07:53:07 -0800 (Tue, 14 Mar 2006) | 2 lines

changed: ImageConvertRow's proto

------------------------------------------------------------------------
r2456 | v1ctor | 2006-03-14 07:52:45 -0800 (Tue, 14 Mar 2006) | 2 lines

change: RGB and BGR split

------------------------------------------------------------------------
r2455 | v1ctor | 2006-03-13 22:57:01 -0800 (Mon, 13 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2454 | v1ctor | 2006-03-13 22:55:15 -0800 (Mon, 13 Mar 2006) | 2 lines

added: ImageConvertRow

------------------------------------------------------------------------
r2453 | v1ctor | 2006-03-13 22:54:14 -0800 (Mon, 13 Mar 2006) | 2 lines

changed: AST arg node handling split from ast-node-funct

------------------------------------------------------------------------
r2452 | v1ctor | 2006-03-13 21:14:50 -0800 (Mon, 13 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2451 | v1ctor | 2006-03-13 21:03:01 -0800 (Mon, 13 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2450 | v1ctor | 2006-03-13 16:26:32 -0800 (Mon, 13 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2449 | v1ctor | 2006-03-13 16:26:05 -0800 (Mon, 13 Mar 2006) | 2 lines

changed: machine translated

------------------------------------------------------------------------
r2448 | v1ctor | 2006-03-13 16:24:45 -0800 (Mon, 13 Mar 2006) | 2 lines

fixed: calling function pointers wouldn't work in arrays of function pointer pointer arrays

------------------------------------------------------------------------
r2447 | v1ctor | 2006-03-12 14:20:12 -0800 (Sun, 12 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2446 | v1ctor | 2006-03-12 14:19:26 -0800 (Sun, 12 Mar 2006) | 2 lines

added: dup definition check for functions that return UDT's when allocating variables implicitly, if names contain periods (.)'s

------------------------------------------------------------------------
r2445 | v1ctor | 2006-03-12 07:24:26 -0800 (Sun, 12 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2444 | v1ctor | 2006-03-11 19:33:56 -0800 (Sat, 11 Mar 2006) | 2 lines

added: new prototypes (the def file must be updated too and the import library rebuilt)

------------------------------------------------------------------------
r2443 | v1ctor | 2006-03-11 19:33:13 -0800 (Sat, 11 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2442 | v1ctor | 2006-03-11 17:34:35 -0800 (Sat, 11 Mar 2006) | 2 lines

changed: duplicated definitions of type aliases are now allowed if the target types are the same

------------------------------------------------------------------------
r2441 | v1ctor | 2006-03-11 17:32:08 -0800 (Sat, 11 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2440 | v1ctor | 2006-03-11 16:50:30 -0800 (Sat, 11 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2439 | v1ctor | 2006-03-11 16:21:25 -0800 (Sat, 11 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2438 | v1ctor | 2006-03-11 16:21:05 -0800 (Sat, 11 Mar 2006) | 2 lines

fixed: the number of arguments passed to macros wasn't been checked

------------------------------------------------------------------------
r2437 | v1ctor | 2006-03-11 11:38:56 -0800 (Sat, 11 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2436 | v1ctor | 2006-03-11 11:37:04 -0800 (Sat, 11 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2435 | v1ctor | 2006-03-11 11:33:59 -0800 (Sat, 11 Mar 2006) | 2 lines

changed: optional params can now be expressions, including support for UDT's

------------------------------------------------------------------------
r2434 | v1ctor | 2006-03-10 18:12:38 -0800 (Fri, 10 Mar 2006) | 2 lines

changed: var initializers and anonymous types now create a new AST node and can be mixed

------------------------------------------------------------------------
r2433 | v1ctor | 2006-03-10 17:08:06 -0800 (Fri, 10 Mar 2006) | 2 lines

fixed: DIALOG's callback field

------------------------------------------------------------------------
r2432 | v1ctor | 2006-03-09 11:04:44 -0800 (Thu, 09 Mar 2006) | 2 lines

fixed: string indexing couldn't be used with derefered pointers

------------------------------------------------------------------------
r2431 | v1ctor | 2006-03-08 21:26:41 -0800 (Wed, 08 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2430 | v1ctor | 2006-03-08 21:19:06 -0800 (Wed, 08 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2429 | v1ctor | 2006-03-07 14:21:51 -0800 (Tue, 07 Mar 2006) | 2 lines

fixed: string indexing couldn't be used with derefered pointers

------------------------------------------------------------------------
r2428 | v1ctor | 2006-03-06 17:37:06 -0800 (Mon, 06 Mar 2006) | 2 lines

fixed: destroy_frame was not checking for scope vars, causing procs without args and local vars outside scope block to not restore the stack pointer

------------------------------------------------------------------------
r2427 | v1ctor | 2006-03-06 16:16:47 -0800 (Mon, 06 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2426 | v1ctor | 2006-03-06 16:15:46 -0800 (Mon, 06 Mar 2006) | 2 lines

fixed: text-mode's (SCREEN 0) width wasn't been restored, causing problems with PRINT

------------------------------------------------------------------------
r2425 | v1ctor | 2006-03-06 09:57:00 -0800 (Mon, 06 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2424 | v1ctor | 2006-03-06 09:52:33 -0800 (Mon, 06 Mar 2006) | 2 lines

fixed: typo, L instead of :

------------------------------------------------------------------------
r2423 | v1ctor | 2006-03-04 20:31:51 -0800 (Sat, 04 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2422 | v1ctor | 2006-03-04 20:31:13 -0800 (Sat, 04 Mar 2006) | 2 lines

added: missing avicap protos

------------------------------------------------------------------------
r2421 | v1ctor | 2006-03-04 12:03:42 -0800 (Sat, 04 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2420 | v1ctor | 2006-03-04 11:58:11 -0800 (Sat, 04 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2419 | v1ctor | 2006-03-03 21:46:34 -0800 (Fri, 03 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2418 | v1ctor | 2006-03-03 21:35:54 -0800 (Fri, 03 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2417 | v1ctor | 2006-03-03 21:03:24 -0800 (Fri, 03 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2416 | v1ctor | 2006-03-03 20:59:13 -0800 (Fri, 03 Mar 2006) | 2 lines

fixed: char & wchar to UTF-8 conversion

------------------------------------------------------------------------
r2415 | v1ctor | 2006-03-03 16:14:59 -0800 (Fri, 03 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2414 | v1ctor | 2006-03-03 14:23:23 -0800 (Fri, 03 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2413 | v1ctor | 2006-03-03 14:07:57 -0800 (Fri, 03 Mar 2006) | 2 lines

changed: ncurses and pdcurses headers were split

------------------------------------------------------------------------
r2412 | v1ctor | 2006-03-03 14:07:26 -0800 (Fri, 03 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2411 | v1ctor | 2006-03-01 08:16:16 -0800 (Wed, 01 Mar 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2410 | v1ctor | 2006-03-01 08:05:47 -0800 (Wed, 01 Mar 2006) | 2 lines

fixed: EOF wasn't been checked (ReadData() will return OK on EOF too)

------------------------------------------------------------------------
r2409 | v1ctor | 2006-02-28 17:19:53 -0800 (Tue, 28 Feb 2006) | 2 lines

IR_OP enum moved to AST

------------------------------------------------------------------------
r2408 | v1ctor | 2006-02-28 17:08:06 -0800 (Tue, 28 Feb 2006) | 2 lines

changed: GOSUB isn't allowed anymore inside procs (RETURN was ambiguous and dangerous to be used in FUNCTIONs and it would be too clumsy to do using the gcc backend, a global stack and longjmp() would make procs with GOSUB non-recursive)

------------------------------------------------------------------------
r2407 | v1ctor | 2006-02-28 13:07:39 -0800 (Tue, 28 Feb 2006) | 2 lines

fixed: too small fractions were been scaled and printed even when they didn't fit

------------------------------------------------------------------------
r2406 | v1ctor | 2006-02-27 21:29:28 -0800 (Mon, 27 Feb 2006) | 2 lines

changed: Single's are handled in a different function, too big exponents are now scaled in both directions, cleaned the code a bit

------------------------------------------------------------------------
r2405 | v1ctor | 2006-02-27 21:20:44 -0800 (Mon, 27 Feb 2006) | 2 lines

changed: Single's are handled in a different PRINT USING function

------------------------------------------------------------------------
r2404 | v1ctor | 2006-02-27 21:20:00 -0800 (Mon, 27 Feb 2006) | 2 lines

changed: Single's are handled in a different function, too big exponents are now scaled in both directions, cleaned the code a bit

------------------------------------------------------------------------
r2403 | v1ctor | 2006-02-27 08:51:09 -0800 (Mon, 27 Feb 2006) | 2 lines

typo

------------------------------------------------------------------------
r2402 | v1ctor | 2006-02-27 08:34:43 -0800 (Mon, 27 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2401 | v1ctor | 2006-02-27 07:45:34 -0800 (Mon, 27 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2400 | v1ctor | 2006-02-27 07:43:11 -0800 (Mon, 27 Feb 2006) | 4 lines

fixed:
- infinite looping when rounding;
- too big numbers (>2^63) where giving wrong results;

------------------------------------------------------------------------
r2399 | v1ctor | 2006-02-27 07:12:57 -0800 (Mon, 27 Feb 2006) | 4 lines

fixed:
- infinite looping when rounding;
- too big numbers (>2^63) where giving wrong results;

------------------------------------------------------------------------
r2398 | v1ctor | 2006-02-25 12:49:36 -0800 (Sat, 25 Feb 2006) | 2 lines

fixed: 0.0 with no decimal places was not been printed

------------------------------------------------------------------------
r2397 | v1ctor | 2006-02-24 20:20:35 -0800 (Fri, 24 Feb 2006) | 2 lines

fixed: crt objects must come before the app ones

------------------------------------------------------------------------
r2396 | v1ctor | 2006-02-24 20:06:35 -0800 (Fri, 24 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2395 | v1ctor | 2006-02-24 20:05:47 -0800 (Fri, 24 Feb 2006) | 2 lines

fixed: crt objects must come before the app objects

------------------------------------------------------------------------
r2394 | v1ctor | 2006-02-24 16:54:23 -0800 (Fri, 24 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2393 | v1ctor | 2006-02-24 12:40:37 -0800 (Fri, 24 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2391 | v1ctor | 2006-02-23 16:58:23 -0800 (Thu, 23 Feb 2006) | 2 lines

added: stack rewinding in scope blocks

------------------------------------------------------------------------
r2390 | v1ctor | 2006-02-23 16:41:44 -0800 (Thu, 23 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2389 | v1ctor | 2006-02-23 10:16:10 -0800 (Thu, 23 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2388 | v1ctor | 2006-02-23 09:44:23 -0800 (Thu, 23 Feb 2006) | 2 lines

non-optional ()'s update

------------------------------------------------------------------------
r2387 | v1ctor | 2006-02-23 09:43:51 -0800 (Thu, 23 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2386 | v1ctor | 2006-02-23 09:39:13 -0800 (Thu, 23 Feb 2006) | 2 lines

changed: ()'s aren't anymore optional with arg-less macros

------------------------------------------------------------------------
r2385 | v1ctor | 2006-02-23 09:36:49 -0800 (Thu, 23 Feb 2006) | 2 lines

small change

------------------------------------------------------------------------
r2384 | v1ctor | 2006-02-22 16:49:11 -0800 (Wed, 22 Feb 2006) | 2 lines

fixed: CHAR clashes with the Win API definition

------------------------------------------------------------------------
r2383 | lillo | 2006-02-22 15:34:32 -0800 (Wed, 22 Feb 2006) | 2 lines

Added font version info in font header

------------------------------------------------------------------------
r2382 | lillo | 2006-02-22 15:22:47 -0800 (Wed, 22 Feb 2006) | 2 lines

Small youth fixes

------------------------------------------------------------------------
r2381 | lillo | 2006-02-22 11:39:09 -0800 (Wed, 22 Feb 2006) | 4 lines

- Added DRAW STRING support
- Gfx GET and PUT can throw runtime errors, but check was disabled even
with -e enabled

------------------------------------------------------------------------
r2380 | v1ctor | 2006-02-22 09:34:51 -0800 (Wed, 22 Feb 2006) | 2 lines

updated to the latest DJGPP version

------------------------------------------------------------------------
r2379 | v1ctor | 2006-02-22 09:08:13 -0800 (Wed, 22 Feb 2006) | 2 lines

changed: extra fields used only with procs (not protos) are now allocated only when used

------------------------------------------------------------------------
r2378 | v1ctor | 2006-02-22 09:05:14 -0800 (Wed, 22 Feb 2006) | 2 lines

fixed: FUNCTION was not been parsed

------------------------------------------------------------------------
r2377 | v1ctor | 2006-02-22 08:50:21 -0800 (Wed, 22 Feb 2006) | 2 lines

fixed: INPUT's buffer must be as large as the largest type

------------------------------------------------------------------------
r2376 | v1ctor | 2006-02-21 18:48:27 -0800 (Tue, 21 Feb 2006) | 2 lines

fixed: main()'s node wasn't been set when compiling mod-level sources, causing IsModLevel() to fail

------------------------------------------------------------------------
r2375 | lillo | 2006-02-21 13:00:20 -0800 (Tue, 21 Feb 2006) | 2 lines

Fixed PAINT clipping bug(s)

------------------------------------------------------------------------
r2374 | lillo | 2006-02-21 12:30:30 -0800 (Tue, 21 Feb 2006) | 2 lines

Fixed win32 windowtitle lock issue

------------------------------------------------------------------------
r2373 | v1ctor | 2006-02-21 09:33:57 -0800 (Tue, 21 Feb 2006) | 2 lines

fixed: MakeTmpStr()'s buffer had a char less than needed

------------------------------------------------------------------------
r2372 | v1ctor | 2006-02-21 08:32:55 -0800 (Tue, 21 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2371 | lillo | 2006-02-20 13:06:03 -0800 (Mon, 20 Feb 2006) | 2 lines

Fixed WINDOWTITLE bug

------------------------------------------------------------------------
r2370 | v1ctor | 2006-02-20 12:37:18 -0800 (Mon, 20 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2369 | v1ctor | 2006-02-20 12:35:33 -0800 (Mon, 20 Feb 2006) | 2 lines

changed: inline asm won't cause the local vars to be immediately allocated on stack anymore

------------------------------------------------------------------------
r2368 | v1ctor | 2006-02-20 10:09:05 -0800 (Mon, 20 Feb 2006) | 6 lines

changed:
- non-shared variables allocated at module-level are now local, store on stack;
- STATIC can be used at module-level now;
- arrays with unknown size can be declared inside procs;
- REDIM can't be used with scalars anymore;

------------------------------------------------------------------------
r2367 | v1ctor | 2006-02-20 08:48:28 -0800 (Mon, 20 Feb 2006) | 6 lines

changed:
- non-shared variables allocated at module-level are now local, store on stack;
- STATIC can be used at module-level now;
- arrays with unknown size can be declared inside procs;
- REDIM can't be used with scalars anymore;

------------------------------------------------------------------------
r2366 | v1ctor | 2006-02-20 07:21:55 -0800 (Mon, 20 Feb 2006) | 2 lines

fixed: too small fractions are handled like in QB now

------------------------------------------------------------------------
r2365 | v1ctor | 2006-02-19 17:21:12 -0800 (Sun, 19 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2364 | v1ctor | 2006-02-19 14:16:05 -0800 (Sun, 19 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2363 | v1ctor | 2006-02-19 14:11:43 -0800 (Sun, 19 Feb 2006) | 2 lines

fixed: overflow could happen if assigning an asc string to a too small fixed-len wide string

------------------------------------------------------------------------
r2362 | v1ctor | 2006-02-18 15:38:50 -0800 (Sat, 18 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2361 | v1ctor | 2006-02-18 11:29:38 -0800 (Sat, 18 Feb 2006) | 2 lines

changed: fb .ctor and .dtor removed

------------------------------------------------------------------------
r2360 | v1ctor | 2006-02-18 11:24:00 -0800 (Sat, 18 Feb 2006) | 2 lines

removed, not more needed

------------------------------------------------------------------------
r2359 | v1ctor | 2006-02-18 03:38:38 -0800 (Sat, 18 Feb 2006) | 2 lines

changed: libgcc's dups removed

------------------------------------------------------------------------
r2358 | v1ctor | 2006-02-18 03:37:26 -0800 (Sat, 18 Feb 2006) | 2 lines

added: libgcc to the DOS version

------------------------------------------------------------------------
r2357 | v1ctor | 2006-02-18 03:37:03 -0800 (Sat, 18 Feb 2006) | 2 lines

cosmetic changes

------------------------------------------------------------------------
r2356 | lillo | 2006-02-18 03:22:50 -0800 (Sat, 18 Feb 2006) | 2 lines

Fixed INKEY behaviour under X11

------------------------------------------------------------------------
r2355 | v1ctor | 2006-02-17 18:45:56 -0800 (Fri, 17 Feb 2006) | 2 lines

changed: local symbols are now only allocated on stack when flushing the proc

------------------------------------------------------------------------
r2354 | v1ctor | 2006-02-17 18:32:33 -0800 (Fri, 17 Feb 2006) | 2 lines

fixed: dizima when passing Single params to the Double "value" arg

------------------------------------------------------------------------
r2353 | v1ctor | 2006-02-17 10:09:30 -0800 (Fri, 17 Feb 2006) | 2 lines

fixed: snprintf() missing in Windows

------------------------------------------------------------------------
r2352 | lillo | 2006-02-17 05:22:18 -0800 (Fri, 17 Feb 2006) | 2 lines

fixed vsync issue yet again

------------------------------------------------------------------------
r2351 | lillo | 2006-02-17 01:29:44 -0800 (Fri, 17 Feb 2006) | 12 lines

- Added Linux framebuffer console gfx driver
- Introduced generic software mouse cursor helper routines (may be helpful
  to DOS port too)
- Introduced generic runtime library loading functions to linux port, since
  a lot of its functionalities are loaded at runtime from shared libraries.
  This reduces code duplication
- Modified the fb_Die() function to store the error message into a buffer
  to be later displayed by fb_RtExit(). This is to ensure the console has
  been safely shut down (at fb_hExit() time) before the message is displayed,
  otherwise at least under Linux the message may not appear. The message is
  now also written to stderr.

------------------------------------------------------------------------
r2350 | v1ctor | 2006-02-16 20:52:56 -0800 (Thu, 16 Feb 2006) | 2 lines

changed: "naming" stuff removed, just "target" should be enough

------------------------------------------------------------------------
r2349 | v1ctor | 2006-02-16 20:38:04 -0800 (Thu, 16 Feb 2006) | 2 lines

changed: main() prolog and epilog moved off emit to fb-main

------------------------------------------------------------------------
r2348 | v1ctor | 2006-02-16 17:02:15 -0800 (Thu, 16 Feb 2006) | 2 lines

changed: pre-initialization in static constructors called before fb_Init()

------------------------------------------------------------------------
r2347 | v1ctor | 2006-02-16 09:26:08 -0800 (Thu, 16 Feb 2006) | 2 lines

fixed: null-terminator wasn't been added

------------------------------------------------------------------------
r2346 | v1ctor | 2006-02-16 08:49:17 -0800 (Thu, 16 Feb 2006) | 2 lines

fixed: CONS can't be opened for input (as in QB)

------------------------------------------------------------------------
r2345 | v1ctor | 2006-02-15 20:51:44 -0800 (Wed, 15 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2344 | v1ctor | 2006-02-15 20:49:05 -0800 (Wed, 15 Feb 2006) | 2 lines

fixed: wrong bitfields' offset calculation when used inside unions

------------------------------------------------------------------------
r2343 | v1ctor | 2006-02-15 11:20:35 -0800 (Wed, 15 Feb 2006) | 4 lines

changed:
- type names;
- moved data type functions from ir to symb-data

------------------------------------------------------------------------
r2342 | v1ctor | 2006-02-15 10:33:21 -0800 (Wed, 15 Feb 2006) | 2 lines

fixed: arguments taking function pointers weren't declared as CDECL

------------------------------------------------------------------------
r2341 | v1ctor | 2006-02-15 10:03:18 -0800 (Wed, 15 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2340 | v1ctor | 2006-02-15 10:01:14 -0800 (Wed, 15 Feb 2006) | 2 lines

fixed: wstring's mid statement will now pass the destine size if it's know at compile-time

------------------------------------------------------------------------
r2339 | v1ctor | 2006-02-15 10:00:17 -0800 (Wed, 15 Feb 2006) | 2 lines

fixed: wstring's mid statement must pass the destine size if it's know at compile-time (v1c)

------------------------------------------------------------------------
r2338 | lillo | 2006-02-15 09:48:21 -0800 (Wed, 15 Feb 2006) | 2 lines

Fixed VIEW border color bug

------------------------------------------------------------------------
r2337 | v1ctor | 2006-02-15 09:45:55 -0800 (Wed, 15 Feb 2006) | 2 lines

fixed: when indexing wstring's the index was not been multiplied by sizeof( wchar )

------------------------------------------------------------------------
r2336 | v1ctor | 2006-02-14 18:02:21 -0800 (Tue, 14 Feb 2006) | 2 lines

fixed: #1431502, the string argument was not been freed

------------------------------------------------------------------------
r2335 | lillo | 2006-02-14 14:07:22 -0800 (Tue, 14 Feb 2006) | 2 lines

changelog.txt

------------------------------------------------------------------------
r2334 | v1ctor | 2006-02-13 11:02:29 -0800 (Mon, 13 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2333 | lillo | 2006-02-13 08:23:23 -0800 (Mon, 13 Feb 2006) | 2 lines

fixed styled box drawing, speeded up styled line by using inline assembly

------------------------------------------------------------------------
r2332 | lillo | 2006-02-12 07:28:54 -0800 (Sun, 12 Feb 2006) | 2 lines

fixed GFX_NO_SWITCH issue with DirectX driver

------------------------------------------------------------------------
r2331 | v1ctor | 2006-02-12 06:32:39 -0800 (Sun, 12 Feb 2006) | 2 lines

fixed: single-line ELSE after RETURN

------------------------------------------------------------------------
r2330 | v1ctor | 2006-02-12 06:26:46 -0800 (Sun, 12 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2329 | v1ctor | 2006-02-12 06:25:28 -0800 (Sun, 12 Feb 2006) | 2 lines

fixed: calling void function pointers in expressions was been allowed

------------------------------------------------------------------------
r2328 | lillo | 2006-02-12 06:02:35 -0800 (Sun, 12 Feb 2006) | 2 lines

fixed DRAW shortcomings

------------------------------------------------------------------------
r2327 | v1ctor | 2006-02-10 17:29:47 -0800 (Fri, 10 Feb 2006) | 2 lines

fixed: hRevertSlash wasn't copying the src string to result if allocnew was TRUE

------------------------------------------------------------------------
r2326 | v1ctor | 2006-02-10 17:18:09 -0800 (Fri, 10 Feb 2006) | 2 lines

fixed: missing default stack size in Win32

------------------------------------------------------------------------
r2325 | v1ctor | 2006-02-10 17:02:59 -0800 (Fri, 10 Feb 2006) | 2 lines

changed: the size of arg-less #defines and macro expansions text can be now up to 2GB

------------------------------------------------------------------------
r2324 | v1ctor | 2006-02-10 17:02:38 -0800 (Fri, 10 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2323 | v1ctor | 2006-02-10 16:16:47 -0800 (Fri, 10 Feb 2006) | 4 lines

added:
- warnings when assigning UDT's with dyn string fields;
- warnings when passing UDT's with ptr or syn string fields to PUT and GET;

------------------------------------------------------------------------
r2322 | v1ctor | 2006-02-10 09:56:51 -0800 (Fri, 10 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2321 | v1ctor | 2006-02-10 09:19:13 -0800 (Fri, 10 Feb 2006) | 2 lines

fixed: precision when converting doubles to strings was one digit too low

------------------------------------------------------------------------
r2320 | v1ctor | 2006-02-09 10:24:26 -0800 (Thu, 09 Feb 2006) | 2 lines

added: missing macros

------------------------------------------------------------------------
r2319 | v1ctor | 2006-02-09 09:28:28 -0800 (Thu, 09 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2318 | v1ctor | 2006-02-09 09:18:25 -0800 (Thu, 09 Feb 2006) | 2 lines

fixed: missing #endif's and typedefs

------------------------------------------------------------------------
r2317 | v1ctor | 2006-02-08 17:09:25 -0800 (Wed, 08 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2316 | v1ctor | 2006-02-08 17:08:30 -0800 (Wed, 08 Feb 2006) | 2 lines

fixed: LINE INPUT memory leak

------------------------------------------------------------------------
r2315 | v1ctor | 2006-02-07 17:50:40 -0800 (Tue, 07 Feb 2006) | 2 lines

fixed: bug #1424182

------------------------------------------------------------------------
r2314 | v1ctor | 2006-02-06 20:40:30 -0800 (Mon, 06 Feb 2006) | 2 lines

fixed: shell32 library missing

------------------------------------------------------------------------
r2313 | v1ctor | 2006-02-06 20:39:59 -0800 (Mon, 06 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2312 | v1ctor | 2006-02-06 20:24:16 -0800 (Mon, 06 Feb 2006) | 2 lines

shell32.a includes IID's too

------------------------------------------------------------------------
r2311 | v1ctor | 2006-02-06 12:08:37 -0800 (Mon, 06 Feb 2006) | 2 lines

changed: gcc's ctors and dtors will been used now

------------------------------------------------------------------------
r2310 | v1ctor | 2006-02-06 12:07:18 -0800 (Mon, 06 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2309 | v1ctor | 2006-02-05 13:57:51 -0800 (Sun, 05 Feb 2006) | 2 lines

fixed: illegal param error reporting

------------------------------------------------------------------------
r2308 | v1ctor | 2006-02-04 10:52:49 -0800 (Sat, 04 Feb 2006) | 2 lines

fixed: negative exponents

------------------------------------------------------------------------
r2307 | v1ctor | 2006-02-04 07:39:12 -0800 (Sat, 04 Feb 2006) | 2 lines

fixed: an octal escape seq would not be converted back if locz. at the end of a literal string

------------------------------------------------------------------------
r2306 | v1ctor | 2006-02-03 12:14:37 -0800 (Fri, 03 Feb 2006) | 2 lines

fixed: the "skipchar" flags were not been reseted (all functions are static)

------------------------------------------------------------------------
r2305 | v1ctor | 2006-02-02 16:21:14 -0800 (Thu, 02 Feb 2006) | 2 lines

changed: termination now is done at fb_Exit() (called by atexit())

------------------------------------------------------------------------
r2304 | v1ctor | 2006-02-02 06:52:25 -0800 (Thu, 02 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2303 | v1ctor | 2006-02-01 10:08:47 -0800 (Wed, 01 Feb 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2302 | v1ctor | 2006-01-30 17:10:59 -0800 (Mon, 30 Jan 2006) | 2 lines

fixed: forced sign for negative values

------------------------------------------------------------------------
r2301 | v1ctor | 2006-01-30 16:48:36 -0800 (Mon, 30 Jan 2006) | 2 lines

fixed: too big negative exponents shouldn't scale the result

------------------------------------------------------------------------
r2300 | v1ctor | 2006-01-30 16:34:37 -0800 (Mon, 30 Jan 2006) | 2 lines

fixed: %num was been added when no exponent was passed explicitly

------------------------------------------------------------------------
r2299 | lillo | 2006-01-29 13:38:38 -0800 (Sun, 29 Jan 2006) | 2 lines

gcc 4.x fix

------------------------------------------------------------------------
r2298 | lillo | 2006-01-29 13:31:10 -0800 (Sun, 29 Jan 2006) | 2 lines

gcc 4.x fix

------------------------------------------------------------------------
r2297 | v1ctor | 2006-01-29 11:45:44 -0800 (Sun, 29 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2296 | v1ctor | 2006-01-29 11:42:42 -0800 (Sun, 29 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2295 | v1ctor | 2006-01-29 10:07:22 -0800 (Sun, 29 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2294 | v1ctor | 2006-01-29 10:05:28 -0800 (Sun, 29 Jan 2006) | 2 lines

fixed: profiling functions shouldn't be called inside dtor's

------------------------------------------------------------------------
r2293 | v1ctor | 2006-01-29 08:07:24 -0800 (Sun, 29 Jan 2006) | 2 lines

fixed: bug #1417548

------------------------------------------------------------------------
r2292 | v1ctor | 2006-01-28 09:29:54 -0800 (Sat, 28 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2291 | v1ctor | 2006-01-27 12:45:53 -0800 (Fri, 27 Jan 2006) | 2 lines

fixed: passing quad constants to GAS in decimal notation will fail in some cases (bug in GAS?)

------------------------------------------------------------------------
r2290 | v1ctor | 2006-01-27 12:44:15 -0800 (Fri, 27 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2289 | v1ctor | 2006-01-26 18:15:59 -0800 (Thu, 26 Jan 2006) | 3 lines

added: support for non-standard named ports
fixed: CreateFile()'s result wasn't be checked for INVALID

------------------------------------------------------------------------
r2288 | v1ctor | 2006-01-25 16:13:11 -0800 (Wed, 25 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2287 | v1ctor | 2006-01-25 16:06:50 -0800 (Wed, 25 Jan 2006) | 2 lines

fixed: regs would be spilled after a branch if one was present inside the condition

------------------------------------------------------------------------
r2286 | lillo | 2006-01-25 16:04:42 -0800 (Wed, 25 Jan 2006) | 2 lines

vertical sync should now work properly under Win32/DirectX

------------------------------------------------------------------------
r2285 | i_am_drv | 2006-01-22 23:33:29 -0800 (Sun, 22 Jan 2006) | 2 lines

added check for snprintf (implemented in DJGPP 2.04)

------------------------------------------------------------------------
r2284 | v1ctor | 2006-01-22 21:34:30 -0800 (Sun, 22 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2283 | v1ctor | 2006-01-22 20:49:20 -0800 (Sun, 22 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2282 | i_am_drv | 2006-01-21 17:13:43 -0800 (Sat, 21 Jan 2006) | 2 lines

added check for snprintf (implemented in DJGPP 2.04)

------------------------------------------------------------------------
r2281 | v1ctor | 2006-01-21 13:00:28 -0800 (Sat, 21 Jan 2006) | 2 lines

fixed: -target wasn't been passed

------------------------------------------------------------------------
r2280 | v1ctor | 2006-01-21 11:57:58 -0800 (Sat, 21 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2279 | v1ctor | 2006-01-21 11:49:27 -0800 (Sat, 21 Jan 2006) | 2 lines

added: missing macros

------------------------------------------------------------------------
r2278 | v1ctor | 2006-01-20 16:28:21 -0800 (Fri, 20 Jan 2006) | 2 lines

added: missing macros

------------------------------------------------------------------------
r2277 | v1ctor | 2006-01-19 18:40:38 -0800 (Thu, 19 Jan 2006) | 2 lines

fixed: h instead of view_h was been used to allocate the span list

------------------------------------------------------------------------
r2276 | v1ctor | 2006-01-18 23:20:46 -0800 (Wed, 18 Jan 2006) | 2 lines

changed: the GNU autoconf will now be used to rebuild this library

------------------------------------------------------------------------
r2275 | v1ctor | 2006-01-17 22:16:11 -0800 (Tue, 17 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2274 | v1ctor | 2006-01-17 22:16:05 -0800 (Tue, 17 Jan 2006) | 2 lines

added: the -include cmd-line option

------------------------------------------------------------------------
r2273 | v1ctor | 2006-01-17 15:52:15 -0800 (Tue, 17 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2272 | v1ctor | 2006-01-17 15:43:00 -0800 (Tue, 17 Jan 2006) | 2 lines

fixed: Input(n) crash when a NULL string was returned by Inkey()

------------------------------------------------------------------------
r2271 | v1ctor | 2006-01-17 15:25:44 -0800 (Tue, 17 Jan 2006) | 2 lines

changed: the GNU autoconf will now be used to rebuild this library

------------------------------------------------------------------------
r2270 | v1ctor | 2006-01-17 13:06:40 -0800 (Tue, 17 Jan 2006) | 2 lines

changed: the GNU autoconf will now be used to rebuild this library

------------------------------------------------------------------------
r2269 | v1ctor | 2006-01-17 13:05:59 -0800 (Tue, 17 Jan 2006) | 2 lines

changed: the GNU autoconf will now be used to rebuild this program

------------------------------------------------------------------------
r2268 | v1ctor | 2006-01-17 12:46:58 -0800 (Tue, 17 Jan 2006) | 2 lines

changed: the GNU autoconf will now be used to rebuild this library

------------------------------------------------------------------------
r2267 | v1ctor | 2006-01-17 11:41:39 -0800 (Tue, 17 Jan 2006) | 2 lines

changed: the GNU autoconf will now be used to rebuild this library

------------------------------------------------------------------------
r2266 | v1ctor | 2006-01-17 11:41:38 -0800 (Tue, 17 Jan 2006) | 2 lines

changed: the GNU autoconf will now be used to rebuild this library

------------------------------------------------------------------------
r2265 | v1ctor | 2006-01-16 21:39:08 -0800 (Mon, 16 Jan 2006) | 2 lines

changed: the GNU autoconf will now be used to rebuild this library

------------------------------------------------------------------------
r2264 | v1ctor | 2006-01-16 21:39:07 -0800 (Mon, 16 Jan 2006) | 2 lines

changed: the GNU autoconf will now be used to rebuild this library

------------------------------------------------------------------------
r2263 | v1ctor | 2006-01-16 21:25:28 -0800 (Mon, 16 Jan 2006) | 2 lines

changed: config files moved to /config

------------------------------------------------------------------------
r2262 | v1ctor | 2006-01-16 20:49:13 -0800 (Mon, 16 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2261 | v1ctor | 2006-01-16 13:08:51 -0800 (Mon, 16 Jan 2006) | 2 lines

changed: the GNU autoconf will now be used to rebuild this library

------------------------------------------------------------------------
r2260 | v1ctor | 2006-01-16 13:02:43 -0800 (Mon, 16 Jan 2006) | 2 lines

changed: the GNU autoconf will now be used to rebuild this library

------------------------------------------------------------------------
r2259 | v1ctor | 2006-01-13 20:34:12 -0800 (Fri, 13 Jan 2006) | 2 lines

added: fb_PrinterWriteWstr()

------------------------------------------------------------------------
r2258 | v1ctor | 2006-01-12 15:51:49 -0800 (Thu, 12 Jan 2006) | 2 lines

fixed: macro declaration

------------------------------------------------------------------------
r2257 | v1ctor | 2006-01-11 08:06:59 -0800 (Wed, 11 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2256 | v1ctor | 2006-01-11 08:04:05 -0800 (Wed, 11 Jan 2006) | 2 lines

fixed: field dereference couldn't be used with CAST( ptr, ... )

------------------------------------------------------------------------
r2255 | v1ctor | 2006-01-11 08:03:18 -0800 (Wed, 11 Jan 2006) | 2 lines

minor change

------------------------------------------------------------------------
r2254 | v1ctor | 2006-01-11 08:03:02 -0800 (Wed, 11 Jan 2006) | 2 lines

changed: no rt error checking in get###

------------------------------------------------------------------------
r2253 | v1ctor | 2006-01-11 07:41:28 -0800 (Wed, 11 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2252 | v1ctor | 2006-01-10 21:04:41 -0800 (Tue, 10 Jan 2006) | 2 lines

changed: NOT behavior

------------------------------------------------------------------------
r2251 | v1ctor | 2006-01-10 21:04:19 -0800 (Tue, 10 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2250 | v1ctor | 2006-01-10 20:23:47 -0800 (Tue, 10 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2249 | v1ctor | 2006-01-10 20:23:24 -0800 (Tue, 10 Jan 2006) | 2 lines

fixed: power-of-2 MOD to AND optimizations can't be done in signed types without checks at runtime

------------------------------------------------------------------------
r2248 | v1ctor | 2006-01-08 09:07:35 -0800 (Sun, 08 Jan 2006) | 2 lines

added: missing macros

------------------------------------------------------------------------
r2247 | v1ctor | 2006-01-08 08:44:40 -0800 (Sun, 08 Jan 2006) | 2 lines

fixed: ctor and dtor couldn't be private

------------------------------------------------------------------------
r2246 | v1ctor | 2006-01-06 17:21:43 -0800 (Fri, 06 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2245 | v1ctor | 2006-01-06 16:41:41 -0800 (Fri, 06 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2244 | v1ctor | 2006-01-06 16:40:49 -0800 (Fri, 06 Jan 2006) | 4 lines

fixed: assignment of different UDT types were been allowed

added: type<symbol> to allow anonymous UDT's to be passed to overloaded functions

------------------------------------------------------------------------
r2243 | v1ctor | 2006-01-06 15:59:41 -0800 (Fri, 06 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2242 | v1ctor | 2006-01-06 15:59:05 -0800 (Fri, 06 Jan 2006) | 2 lines

fixed: anonymous UDT (TYPE(...)) couldn't be used when setting function results

------------------------------------------------------------------------
r2241 | v1ctor | 2006-01-06 15:48:23 -0800 (Fri, 06 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2240 | v1ctor | 2006-01-06 15:48:03 -0800 (Fri, 06 Jan 2006) | 2 lines

fixed: __FILE__ when Option Escape was used

------------------------------------------------------------------------
r2239 | v1ctor | 2006-01-06 11:13:50 -0800 (Fri, 06 Jan 2006) | 2 lines

changed: NOT behavior

------------------------------------------------------------------------
r2238 | v1ctor | 2006-01-06 11:12:30 -0800 (Fri, 06 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2236 | v1ctor | 2006-01-06 11:10:39 -0800 (Fri, 06 Jan 2006) | 2 lines

fixed: TRUE should be -1 due NOT

------------------------------------------------------------------------
r2235 | v1ctor | 2006-01-06 11:00:59 -0800 (Fri, 06 Jan 2006) | 2 lines

changed: NOT behavior

------------------------------------------------------------------------
r2234 | v1ctor | 2006-01-06 10:43:23 -0800 (Fri, 06 Jan 2006) | 2 lines

changed: NOT behavior

------------------------------------------------------------------------
r2233 | v1ctor | 2006-01-04 12:34:43 -0800 (Wed, 04 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2232 | v1ctor | 2006-01-04 12:33:59 -0800 (Wed, 04 Jan 2006) | 2 lines

fixed: resource files were not been passed inside quotes to GoRC

------------------------------------------------------------------------
r2231 | v1ctor | 2006-01-03 11:45:45 -0800 (Tue, 03 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2230 | v1ctor | 2006-01-03 11:42:59 -0800 (Tue, 03 Jan 2006) | 2 lines

added: missing macros

------------------------------------------------------------------------
r2229 | v1ctor | 2006-01-02 20:07:50 -0800 (Mon, 02 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2228 | v1ctor | 2006-01-02 19:46:39 -0800 (Mon, 02 Jan 2006) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2227 | v1ctor | 2005-12-31 20:01:46 -0800 (Sat, 31 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2226 | v1ctor | 2005-12-31 19:59:42 -0800 (Sat, 31 Dec 2005) | 2 lines

fixed: cap* prototypes missing

------------------------------------------------------------------------
r2225 | v1ctor | 2005-12-30 11:54:57 -0800 (Fri, 30 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2224 | v1ctor | 2005-12-30 11:31:29 -0800 (Fri, 30 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2223 | v1ctor | 2005-12-30 11:10:49 -0800 (Fri, 30 Dec 2005) | 2 lines

added: the optional "args" argument to RUN

------------------------------------------------------------------------
r2222 | v1ctor | 2005-12-30 11:10:26 -0800 (Fri, 30 Dec 2005) | 2 lines

changed: Run() and Chain() calling ExecEx() now

------------------------------------------------------------------------
r2221 | v1ctor | 2005-12-29 19:58:35 -0800 (Thu, 29 Dec 2005) | 2 lines

fixed: passing empty string arguments will always give 0 as result

------------------------------------------------------------------------
r2220 | v1ctor | 2005-12-29 19:38:59 -0800 (Thu, 29 Dec 2005) | 2 lines

fixed: typo in __FB_MIN_VERSION__ definition

------------------------------------------------------------------------
r2219 | v1ctor | 2005-12-28 08:52:31 -0800 (Wed, 28 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2218 | v1ctor | 2005-12-28 08:49:57 -0800 (Wed, 28 Dec 2005) | 2 lines

fixed: better console redirection check

------------------------------------------------------------------------
r2217 | v1ctor | 2005-12-27 10:06:50 -0800 (Tue, 27 Dec 2005) | 2 lines

fixed: CUSTOM blender function was broken

------------------------------------------------------------------------
r2216 | v1ctor | 2005-12-26 13:40:17 -0800 (Mon, 26 Dec 2005) | 2 lines

minor change

------------------------------------------------------------------------
r2215 | v1ctor | 2005-12-26 13:37:38 -0800 (Mon, 26 Dec 2005) | 2 lines

fixed: w|zstrings were been allowed in IIF()

------------------------------------------------------------------------
r2214 | v1ctor | 2005-12-26 13:37:25 -0800 (Mon, 26 Dec 2005) | 2 lines

fixed: octal conversions

------------------------------------------------------------------------
r2213 | v1ctor | 2005-12-26 13:31:39 -0800 (Mon, 26 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2212 | v1ctor | 2005-12-25 15:38:07 -0800 (Sun, 25 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2211 | v1ctor | 2005-12-25 15:28:17 -0800 (Sun, 25 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2210 | v1ctor | 2005-12-25 15:27:49 -0800 (Sun, 25 Dec 2005) | 2 lines

fixed: w|zstrings were been allowed in IIF()

------------------------------------------------------------------------
r2209 | v1ctor | 2005-12-25 07:25:18 -0800 (Sun, 25 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2208 | v1ctor | 2005-12-24 18:55:34 -0800 (Sat, 24 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2207 | v1ctor | 2005-12-24 18:54:24 -0800 (Sat, 24 Dec 2005) | 2 lines

fixed: memxxx macros

------------------------------------------------------------------------
r2206 | v1ctor | 2005-12-24 17:25:59 -0800 (Sat, 24 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2205 | v1ctor | 2005-12-24 17:25:53 -0800 (Sat, 24 Dec 2005) | 2 lines

fixed: sign digit was always been printed

------------------------------------------------------------------------
r2204 | v1ctor | 2005-12-24 17:01:03 -0800 (Sat, 24 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2203 | v1ctor | 2005-12-24 16:50:09 -0800 (Sat, 24 Dec 2005) | 2 lines

fixed: NOT handling in conditional pre-processor statements

------------------------------------------------------------------------
r2202 | v1ctor | 2005-12-24 16:38:59 -0800 (Sat, 24 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2201 | v1ctor | 2005-12-23 17:30:48 -0800 (Fri, 23 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2200 | v1ctor | 2005-12-23 17:27:39 -0800 (Fri, 23 Dec 2005) | 5 lines

fixed:
- shifting 64-bit operands by constants above 32 were generating wrong code
- doing integer-only operations like \ mixing 64-bit and floating-point operands would generate bad code
- assigning floating-point constants to unsigned 64-bit's would not convert the literal

------------------------------------------------------------------------
r2199 | v1ctor | 2005-12-23 17:25:52 -0800 (Fri, 23 Dec 2005) | 2 lines

fixed: suffixes were not been preserved in #defines

------------------------------------------------------------------------
r2198 | v1ctor | 2005-12-23 17:23:10 -0800 (Fri, 23 Dec 2005) | 2 lines

fixed: shifting 64-bit operands by constants above 32 were generating wrong code

------------------------------------------------------------------------
r2197 | v1ctor | 2005-12-23 17:20:27 -0800 (Fri, 23 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2196 | v1ctor | 2005-12-23 17:16:31 -0800 (Fri, 23 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2195 | v1ctor | 2005-12-22 10:22:30 -0800 (Thu, 22 Dec 2005) | 2 lines

changed: using commctrl macros now

------------------------------------------------------------------------
r2194 | v1ctor | 2005-12-22 10:18:58 -0800 (Thu, 22 Dec 2005) | 2 lines

added: missing macros

------------------------------------------------------------------------
r2193 | v1ctor | 2005-12-21 21:01:26 -0800 (Wed, 21 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2192 | v1ctor | 2005-12-21 19:38:58 -0800 (Wed, 21 Dec 2005) | 2 lines

changed: pointer arguments in overloaded functions won't accept intergers but 0 as parameter;

------------------------------------------------------------------------
r2191 | v1ctor | 2005-12-21 08:34:19 -0800 (Wed, 21 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2190 | v1ctor | 2005-12-21 08:31:49 -0800 (Wed, 21 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2189 | v1ctor | 2005-12-21 08:05:24 -0800 (Wed, 21 Dec 2005) | 2 lines

fixed: I/O redirection check in GFX mode

------------------------------------------------------------------------
r2188 | v1ctor | 2005-12-20 12:10:08 -0800 (Tue, 20 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2187 | v1ctor | 2005-12-20 12:09:15 -0800 (Tue, 20 Dec 2005) | 2 lines

fixed: the default type in PEEK and POKE should be unsigned byte, not signed

------------------------------------------------------------------------
r2186 | v1ctor | 2005-12-20 11:59:44 -0800 (Tue, 20 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2185 | v1ctor | 2005-12-20 11:53:52 -0800 (Tue, 20 Dec 2005) | 2 lines

added: extended versions with "digits" args

------------------------------------------------------------------------
r2184 | v1ctor | 2005-12-20 11:53:35 -0800 (Tue, 20 Dec 2005) | 2 lines

fixed: woct() result declared as "string" instead of "wchar"

------------------------------------------------------------------------
r2183 | lillo | 2005-12-20 02:52:47 -0800 (Tue, 20 Dec 2005) | 6 lines

termcap -> ncurses
The termcap functionalities used by FB are implemented by ncurses since
on modern distros termcap has been replaced by it. Most of these distros
will already feature a symlink from libtermcap to libncurses, but we
directly link with ncurses to avoid any problems.

------------------------------------------------------------------------
r2182 | v1ctor | 2005-12-17 18:37:15 -0800 (Sat, 17 Dec 2005) | 2 lines

removed: stdcall's

------------------------------------------------------------------------
r2181 | v1ctor | 2005-12-17 11:10:49 -0800 (Sat, 17 Dec 2005) | 2 lines

fixed: GetLastError() prototype missing

------------------------------------------------------------------------
r2180 | v1ctor | 2005-12-17 09:48:35 -0800 (Sat, 17 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2179 | v1ctor | 2005-12-17 09:09:43 -0800 (Sat, 17 Dec 2005) | 2 lines

fixed: var deallocation inside Scope blocks would trash mem if arrays were present

------------------------------------------------------------------------
r2178 | v1ctor | 2005-12-17 08:08:15 -0800 (Sat, 17 Dec 2005) | 2 lines

added: extended versions with "digits" args for [w]hex, [w]oct, [w]bin

------------------------------------------------------------------------
r2177 | v1ctor | 2005-12-17 08:06:57 -0800 (Sat, 17 Dec 2005) | 2 lines

added: extended versions with "digits" args

------------------------------------------------------------------------
r2176 | v1ctor | 2005-12-17 05:56:43 -0800 (Sat, 17 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2175 | v1ctor | 2005-12-15 18:42:31 -0800 (Thu, 15 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2174 | v1ctor | 2005-12-15 15:52:46 -0800 (Thu, 15 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2173 | v1ctor | 2005-12-15 15:51:49 -0800 (Thu, 15 Dec 2005) | 2 lines

added: anonymous UDT's using TYPE(...)

------------------------------------------------------------------------
r2172 | v1ctor | 2005-12-15 15:35:19 -0800 (Thu, 15 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2171 | v1ctor | 2005-12-15 15:33:47 -0800 (Thu, 15 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2170 | v1ctor | 2005-12-15 15:27:28 -0800 (Thu, 15 Dec 2005) | 2 lines

added: anonymous UDT's using TYPE(...)

------------------------------------------------------------------------
r2169 | v1ctor | 2005-12-15 13:41:22 -0800 (Thu, 15 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2168 | v1ctor | 2005-12-15 05:51:15 -0800 (Thu, 15 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2167 | v1ctor | 2005-12-15 05:24:19 -0800 (Thu, 15 Dec 2005) | 2 lines

fixed: missing #inclib "advapi32"

------------------------------------------------------------------------
r2166 | v1ctor | 2005-12-14 19:18:11 -0800 (Wed, 14 Dec 2005) | 2 lines

removed the bad merges

------------------------------------------------------------------------
r2165 | i_am_drv | 2005-12-14 11:53:38 -0800 (Wed, 14 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2164 | i_am_drv | 2005-12-14 11:50:50 -0800 (Wed, 14 Dec 2005) | 2 lines

- fixed SETDATE parser - was yielding incorrect results for >2  digit numbers

------------------------------------------------------------------------
r2163 | v1ctor | 2005-12-14 11:05:52 -0800 (Wed, 14 Dec 2005) | 2 lines

fixed: positive sign was never printed

------------------------------------------------------------------------
r2162 | v1ctor | 2005-12-13 17:19:39 -0800 (Tue, 13 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2161 | v1ctor | 2005-12-13 17:14:25 -0800 (Tue, 13 Dec 2005) | 2 lines

fixed: null pointer checks when -exx is used shouldn't be done when taking the address of the expression

------------------------------------------------------------------------
r2160 | v1ctor | 2005-12-12 12:31:22 -0800 (Mon, 12 Dec 2005) | 2 lines

fixed: pointers were not been converted to uint's, causing warning messages

------------------------------------------------------------------------
r2159 | v1ctor | 2005-12-12 12:30:47 -0800 (Mon, 12 Dec 2005) | 2 lines

fixed: the data type wasn't been set for check nodes

------------------------------------------------------------------------
r2158 | v1ctor | 2005-12-11 18:04:59 -0800 (Sun, 11 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2157 | v1ctor | 2005-12-11 17:35:45 -0800 (Sun, 11 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2156 | v1ctor | 2005-12-11 17:30:49 -0800 (Sun, 11 Dec 2005) | 2 lines

fixed: bitfields couldn't be used in "select case"

------------------------------------------------------------------------
r2155 | v1ctor | 2005-12-10 10:28:20 -0800 (Sat, 10 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2154 | v1ctor | 2005-12-10 10:21:44 -0800 (Sat, 10 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2153 | v1ctor | 2005-12-10 10:19:28 -0800 (Sat, 10 Dec 2005) | 2 lines

fixed: if destine wstring's size was unknown at compile time nothing was been copied

------------------------------------------------------------------------
r2152 | v1ctor | 2005-12-10 09:22:52 -0800 (Sat, 10 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2151 | lillo | 2005-12-09 01:52:24 -0800 (Fri, 09 Dec 2005) | 2 lines

Fixed PUT ALPHA bug

------------------------------------------------------------------------
r2150 | lillo | 2005-12-09 01:37:43 -0800 (Fri, 09 Dec 2005) | 2 lines

fixed gcc 4.x warnings

------------------------------------------------------------------------
r2149 | v1ctor | 2005-12-08 16:32:21 -0800 (Thu, 08 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2148 | v1ctor | 2005-12-08 11:57:44 -0800 (Thu, 08 Dec 2005) | 2 lines

internal changes: null-ptr and array bounds checks

------------------------------------------------------------------------
r2147 | v1ctor | 2005-12-08 11:56:35 -0800 (Thu, 08 Dec 2005) | 2 lines

small changes

------------------------------------------------------------------------
r2146 | v1ctor | 2005-12-08 11:55:50 -0800 (Thu, 08 Dec 2005) | 2 lines

fixed: registers used to save un-preserved regs when calling functions were not been freed when the IR list was flushed

------------------------------------------------------------------------
r2145 | v1ctor | 2005-12-08 11:53:55 -0800 (Thu, 08 Dec 2005) | 2 lines

added: when aborting, the name of the source file that caused the run-time error will also be printed

------------------------------------------------------------------------
r2144 | v1ctor | 2005-12-08 11:47:46 -0800 (Thu, 08 Dec 2005) | 2 lines

fixed: "any ptr" should taken as "byte ptr" or the calcs involving for example SDL->pixels wouldn't work

------------------------------------------------------------------------
r2143 | v1ctor | 2005-12-07 09:26:55 -0800 (Wed, 07 Dec 2005) | 2 lines

fixed: underline support missing in DEF###

------------------------------------------------------------------------
r2142 | v1ctor | 2005-12-04 14:25:34 -0800 (Sun, 04 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2141 | v1ctor | 2005-12-04 14:12:35 -0800 (Sun, 04 Dec 2005) | 2 lines

fixed: INPUT when stdin is been redirected

------------------------------------------------------------------------
r2140 | v1ctor | 2005-12-04 14:04:41 -0800 (Sun, 04 Dec 2005) | 2 lines

fixed: INPUT when stdin is been redirected

------------------------------------------------------------------------
r2139 | v1ctor | 2005-12-02 11:35:15 -0800 (Fri, 02 Dec 2005) | 2 lines

changed: version number

------------------------------------------------------------------------
r2138 | v1ctor | 2005-12-02 11:34:37 -0800 (Fri, 02 Dec 2005) | 2 lines

fixed: null pointer and bounds check when functions were been used

------------------------------------------------------------------------
r2137 | v1ctor | 2005-12-02 11:31:28 -0800 (Fri, 02 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2135 | v1ctor | 2005-12-01 09:29:26 -0800 (Thu, 01 Dec 2005) | 2 lines

added: optional sections

------------------------------------------------------------------------
r2134 | v1ctor | 2005-12-01 09:28:39 -0800 (Thu, 01 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2133 | v1ctor | 2005-12-01 08:55:14 -0800 (Thu, 01 Dec 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2132 | v1ctor | 2005-12-01 08:54:31 -0800 (Thu, 01 Dec 2005) | 2 lines

changed to win9x friendly version

------------------------------------------------------------------------
r2131 | i_am_drv | 2005-12-01 00:05:35 -0800 (Thu, 01 Dec 2005) | 2 lines

depth -> deep

------------------------------------------------------------------------
r2130 | v1ctor | 2005-11-30 16:56:34 -0800 (Wed, 30 Nov 2005) | 2 lines

fixed: FB_TK_PRAGMA missing when skipping #IF blocks

------------------------------------------------------------------------
r2129 | v1ctor | 2005-11-30 16:27:20 -0800 (Wed, 30 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2128 | i_am_drv | 2005-11-30 14:23:16 -0800 (Wed, 30 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2127 | v1ctor | 2005-11-30 13:49:32 -0800 (Wed, 30 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2126 | v1ctor | 2005-11-30 13:19:01 -0800 (Wed, 30 Nov 2005) | 2 lines

changed: POINT()'s single-version constant

------------------------------------------------------------------------
r2125 | v1ctor | 2005-11-30 13:15:43 -0800 (Wed, 30 Nov 2005) | 2 lines

changed: POINT()'s single-version constant

------------------------------------------------------------------------
r2124 | v1ctor | 2005-11-30 07:53:59 -0800 (Wed, 30 Nov 2005) | 2 lines

added: missing files

------------------------------------------------------------------------
r2123 | v1ctor | 2005-11-30 07:31:42 -0800 (Wed, 30 Nov 2005) | 2 lines

added: logo

------------------------------------------------------------------------
r2122 | v1ctor | 2005-11-30 06:22:59 -0800 (Wed, 30 Nov 2005) | 2 lines

fixed: off by one when reporting a dup-definition error

------------------------------------------------------------------------
r2121 | v1ctor | 2005-11-29 18:46:33 -0800 (Tue, 29 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2120 | v1ctor | 2005-11-29 16:49:32 -0800 (Tue, 29 Nov 2005) | 2 lines

added: section groups

------------------------------------------------------------------------
r2119 | v1ctor | 2005-11-29 16:45:42 -0800 (Tue, 29 Nov 2005) | 2 lines

added: section groups

------------------------------------------------------------------------
r2118 | i_am_drv | 2005-11-28 23:34:11 -0800 (Mon, 28 Nov 2005) | 2 lines

DOS VESA gfxlib driver

------------------------------------------------------------------------
r2117 | i_am_drv | 2005-11-28 23:27:42 -0800 (Mon, 28 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2116 | v1ctor | 2005-11-28 12:13:58 -0800 (Mon, 28 Nov 2005) | 2 lines

changed: s[w]printf removed from WRITE when writing strings, no more size limit

------------------------------------------------------------------------
r2115 | v1ctor | 2005-11-28 10:06:43 -0800 (Mon, 28 Nov 2005) | 2 lines

fixed: subtraction of 2 ptr operands was not been divided by length

------------------------------------------------------------------------
r2114 | v1ctor | 2005-11-28 10:01:56 -0800 (Mon, 28 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2113 | v1ctor | 2005-11-28 09:57:18 -0800 (Mon, 28 Nov 2005) | 2 lines

changed: RGB_() macro renamed to BGR()

------------------------------------------------------------------------
r2112 | v1ctor | 2005-11-28 08:52:10 -0800 (Mon, 28 Nov 2005) | 2 lines

fixed: warning message

------------------------------------------------------------------------
r2111 | v1ctor | 2005-11-28 08:50:26 -0800 (Mon, 28 Nov 2005) | 2 lines

stdio.h missing

------------------------------------------------------------------------
r2110 | i_am_drv | 2005-11-27 23:30:25 -0800 (Sun, 27 Nov 2005) | 2 lines

fixed video mode restore for dos gfxlib

------------------------------------------------------------------------
r2109 | mjscod | 2005-11-27 22:30:29 -0800 (Sun, 27 Nov 2005) | 6 lines

* Added my implementation of (L/R)TRIM(ANY/EX) because it doesn't rely
  on wmemchr and wmemcmp which seems to be unavailable on my MinGW
  installation
* Added tests for the new functions
* Changed the "cast"'s in lex-utf to cptr to avoid chicken/egg problem

------------------------------------------------------------------------
r2108 | v1ctor | 2005-11-27 19:30:45 -0800 (Sun, 27 Nov 2005) | 2 lines

fixed: no masking in U16_SWAP

------------------------------------------------------------------------
r2107 | v1ctor | 2005-11-27 18:47:20 -0800 (Sun, 27 Nov 2005) | 2 lines

added: snprintf() workaround

------------------------------------------------------------------------
r2106 | v1ctor | 2005-11-27 18:47:06 -0800 (Sun, 27 Nov 2005) | 2 lines

fixed: stack was been trashed

------------------------------------------------------------------------
r2105 | v1ctor | 2005-11-27 09:57:08 -0800 (Sun, 27 Nov 2005) | 2 lines

added: the TARGET_X86 define

------------------------------------------------------------------------
r2104 | v1ctor | 2005-11-26 18:44:50 -0800 (Sat, 26 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2103 | v1ctor | 2005-11-26 16:53:59 -0800 (Sat, 26 Nov 2005) | 2 lines

changed: RGB() macro renamed to RGB_()

------------------------------------------------------------------------
r2102 | v1ctor | 2005-11-26 16:50:36 -0800 (Sat, 26 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2101 | v1ctor | 2005-11-26 12:48:24 -0800 (Sat, 26 Nov 2005) | 2 lines

added: [L|R]Trim for wstrings

------------------------------------------------------------------------
r2100 | v1ctor | 2005-11-26 09:27:22 -0800 (Sat, 26 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2099 | v1ctor | 2005-11-25 22:43:51 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2098 | v1ctor | 2005-11-25 22:34:45 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2097 | v1ctor | 2005-11-25 22:21:49 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2096 | v1ctor | 2005-11-25 21:45:32 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2095 | v1ctor | 2005-11-25 21:19:57 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2094 | v1ctor | 2005-11-25 21:02:27 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2093 | v1ctor | 2005-11-25 16:17:30 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2092 | v1ctor | 2005-11-25 16:13:45 -0800 (Fri, 25 Nov 2005) | 2 lines

added: missing macros

------------------------------------------------------------------------
r2091 | v1ctor | 2005-11-25 15:19:24 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2090 | v1ctor | 2005-11-25 11:31:06 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2089 | v1ctor | 2005-11-25 11:29:35 -0800 (Fri, 25 Nov 2005) | 2 lines

changed: better exponent handling

------------------------------------------------------------------------
r2088 | v1ctor | 2005-11-25 10:54:46 -0800 (Fri, 25 Nov 2005) | 2 lines

fixed: overflow with too big exponents

------------------------------------------------------------------------
r2087 | v1ctor | 2005-11-25 09:29:25 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2086 | v1ctor | 2005-11-25 09:23:13 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2085 | v1ctor | 2005-11-25 08:26:33 -0800 (Fri, 25 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2084 | v1ctor | 2005-11-24 22:37:48 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2083 | v1ctor | 2005-11-24 22:21:21 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2082 | v1ctor | 2005-11-24 22:09:52 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2081 | v1ctor | 2005-11-24 21:51:09 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2080 | v1ctor | 2005-11-24 21:37:42 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2079 | v1ctor | 2005-11-24 21:30:29 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2078 | v1ctor | 2005-11-24 21:24:31 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2077 | v1ctor | 2005-11-24 21:12:46 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2076 | v1ctor | 2005-11-24 21:10:18 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2075 | v1ctor | 2005-11-24 19:16:24 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2074 | v1ctor | 2005-11-24 19:07:14 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2073 | v1ctor | 2005-11-24 18:56:14 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2072 | v1ctor | 2005-11-24 18:47:59 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2071 | v1ctor | 2005-11-24 18:39:14 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2070 | v1ctor | 2005-11-24 18:26:56 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2069 | v1ctor | 2005-11-24 18:13:10 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2068 | v1ctor | 2005-11-24 18:04:39 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2067 | v1ctor | 2005-11-24 17:22:14 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2066 | v1ctor | 2005-11-24 17:11:15 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2065 | v1ctor | 2005-11-24 17:03:20 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2064 | v1ctor | 2005-11-24 16:57:02 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2063 | v1ctor | 2005-11-24 16:36:03 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2062 | v1ctor | 2005-11-24 13:27:45 -0800 (Thu, 24 Nov 2005) | 2 lines

minor change: explicitly using cunsg() when dividing by a pow2 const

------------------------------------------------------------------------
r2061 | v1ctor | 2005-11-24 13:24:46 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2060 | v1ctor | 2005-11-24 13:16:17 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2059 | v1ctor | 2005-11-24 12:50:27 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2058 | v1ctor | 2005-11-24 12:48:58 -0800 (Thu, 24 Nov 2005) | 2 lines

fixed: 0 was been passed as invalid len, instead of -1

------------------------------------------------------------------------
r2057 | v1ctor | 2005-11-24 12:37:25 -0800 (Thu, 24 Nov 2005) | 2 lines

fixed: a signed integer division by a pow2 constant would give wrong results

------------------------------------------------------------------------
r2056 | v1ctor | 2005-11-24 12:37:11 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2055 | v1ctor | 2005-11-24 12:28:34 -0800 (Thu, 24 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2054 | v1ctor | 2005-11-24 12:19:52 -0800 (Thu, 24 Nov 2005) | 2 lines

fixed: a signed integer division by a pow2 constant would give wrong results

------------------------------------------------------------------------
r2053 | v1ctor | 2005-11-24 11:35:11 -0800 (Thu, 24 Nov 2005) | 2 lines

added: missing symbols

------------------------------------------------------------------------
r2052 | v1ctor | 2005-11-24 11:07:57 -0800 (Thu, 24 Nov 2005) | 2 lines

fixed: the text of the wrong line was been printed if an error occurred

------------------------------------------------------------------------
r2051 | v1ctor | 2005-11-24 10:41:10 -0800 (Thu, 24 Nov 2005) | 2 lines

added: missing symbols

------------------------------------------------------------------------
r2050 | v1ctor | 2005-11-24 08:34:36 -0800 (Thu, 24 Nov 2005) | 2 lines

added: missing symbols

------------------------------------------------------------------------
r2049 | v1ctor | 2005-11-24 07:26:09 -0800 (Thu, 24 Nov 2005) | 2 lines

added: missing symbols

------------------------------------------------------------------------
r2048 | v1ctor | 2005-11-23 16:57:12 -0800 (Wed, 23 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2047 | v1ctor | 2005-11-23 16:36:53 -0800 (Wed, 23 Nov 2005) | 2 lines

small fix

------------------------------------------------------------------------
r2046 | v1ctor | 2005-11-23 16:35:41 -0800 (Wed, 23 Nov 2005) | 2 lines

added: complete UTF encoding support for lex

------------------------------------------------------------------------
r2045 | v1ctor | 2005-11-23 16:32:59 -0800 (Wed, 23 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2044 | v1ctor | 2005-11-22 20:22:58 -0800 (Tue, 22 Nov 2005) | 2 lines

added: utf-8 input support

------------------------------------------------------------------------
r2043 | v1ctor | 2005-11-22 18:33:19 -0800 (Tue, 22 Nov 2005) | 2 lines

fixed: utf-8 conversion

------------------------------------------------------------------------
r2042 | v1ctor | 2005-11-22 17:07:48 -0800 (Tue, 22 Nov 2005) | 2 lines

fixed: Linux support for wchar_t

------------------------------------------------------------------------
r2041 | v1ctor | 2005-11-22 16:57:37 -0800 (Tue, 22 Nov 2005) | 2 lines

fixed: Linux support for wchar_t

------------------------------------------------------------------------
r2040 | v1ctor | 2005-11-22 16:34:12 -0800 (Tue, 22 Nov 2005) | 2 lines

added: wstring input functions

------------------------------------------------------------------------
r2039 | v1ctor | 2005-11-22 16:23:36 -0800 (Tue, 22 Nov 2005) | 2 lines

added: wstring input functions

------------------------------------------------------------------------
r2038 | v1ctor | 2005-11-22 16:02:08 -0800 (Tue, 22 Nov 2005) | 2 lines

added: wstring's requirements in Linux

------------------------------------------------------------------------
r2037 | v1ctor | 2005-11-22 15:57:56 -0800 (Tue, 22 Nov 2005) | 2 lines

added: wstring input functions

------------------------------------------------------------------------
r2036 | v1ctor | 2005-11-22 15:57:10 -0800 (Tue, 22 Nov 2005) | 2 lines

added: wstring input and output (no real support, just conversion to/from ascii)

------------------------------------------------------------------------
r2035 | v1ctor | 2005-11-22 05:48:27 -0800 (Tue, 22 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2034 | i_am_drv | 2005-11-21 12:35:48 -0800 (Mon, 21 Nov 2005) | 2 lines

added 16- and 32-bpp mouse support

------------------------------------------------------------------------
r2033 | i_am_drv | 2005-11-21 12:01:37 -0800 (Mon, 21 Nov 2005) | 2 lines

fixed typo

------------------------------------------------------------------------
r2032 | i_am_drv | 2005-11-21 11:33:36 -0800 (Mon, 21 Nov 2005) | 2 lines

moved VESA-specific code out of libfb_gfx_dos.c; fixed VESA driver somewhat

------------------------------------------------------------------------
r2031 | i_am_drv | 2005-11-20 22:54:16 -0800 (Sun, 20 Nov 2005) | 2 lines

changed $(EXEEXT) o $(EXEXT)...

------------------------------------------------------------------------
r2030 | i_am_drv | 2005-11-20 21:03:55 -0800 (Sun, 20 Nov 2005) | 2 lines

changed .exe to $(EXEEXT)

------------------------------------------------------------------------
r2029 | v1ctor | 2005-11-18 15:28:23 -0800 (Fri, 18 Nov 2005) | 2 lines

fixed: length is passed in characters, not in bytes

------------------------------------------------------------------------
r2028 | v1ctor | 2005-11-18 15:27:52 -0800 (Fri, 18 Nov 2005) | 2 lines

changed: no UTF output to files unless ENCODING is used

------------------------------------------------------------------------
r2027 | v1ctor | 2005-11-18 12:34:51 -0800 (Fri, 18 Nov 2005) | 2 lines

changed: newline is now CRLF in DOS, Win32 and Xbox

------------------------------------------------------------------------
r2026 | v1ctor | 2005-11-18 12:26:03 -0800 (Fri, 18 Nov 2005) | 2 lines

added: preliminary support for UTF encoding

------------------------------------------------------------------------
r2025 | v1ctor | 2005-11-18 10:48:26 -0800 (Fri, 18 Nov 2005) | 2 lines

changed: from array to variable + elements in GET# when reading UTF files

------------------------------------------------------------------------
r2024 | v1ctor | 2005-11-18 10:42:33 -0800 (Fri, 18 Nov 2005) | 2 lines

added: preliminary support for UTF encoding

------------------------------------------------------------------------
r2023 | v1ctor | 2005-11-18 10:40:01 -0800 (Fri, 18 Nov 2005) | 2 lines

added: the ENCODING option to OPEN

------------------------------------------------------------------------
r2022 | v1ctor | 2005-11-17 16:39:38 -0800 (Thu, 17 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2021 | v1ctor | 2005-11-17 16:32:59 -0800 (Thu, 17 Nov 2005) | 2 lines

added: swap for wstrings

------------------------------------------------------------------------
r2020 | v1ctor | 2005-11-17 16:32:28 -0800 (Thu, 17 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2019 | v1ctor | 2005-11-17 12:25:48 -0800 (Thu, 17 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2018 | v1ctor | 2005-11-17 12:21:51 -0800 (Thu, 17 Nov 2005) | 2 lines

added: the CAST() inline function

------------------------------------------------------------------------
r2017 | v1ctor | 2005-11-17 11:05:40 -0800 (Thu, 17 Nov 2005) | 2 lines

changed: better temp descriptor allocation for literals

------------------------------------------------------------------------
r2016 | v1ctor | 2005-11-17 11:04:59 -0800 (Thu, 17 Nov 2005) | 2 lines

small changes

------------------------------------------------------------------------
r2015 | v1ctor | 2005-11-16 20:30:40 -0800 (Wed, 16 Nov 2005) | 2 lines

fixed: illegal operations on char|wchar were been allowed due the internal changes

------------------------------------------------------------------------
r2014 | v1ctor | 2005-11-16 19:54:55 -0800 (Wed, 16 Nov 2005) | 2 lines

fixed: pointer +|- char|wchar was been allowed

------------------------------------------------------------------------
r2013 | v1ctor | 2005-11-16 11:02:58 -0800 (Wed, 16 Nov 2005) | 2 lines

fixed: pointer fields passed to GET or PUT

------------------------------------------------------------------------
r2012 | v1ctor | 2005-11-16 10:44:11 -0800 (Wed, 16 Nov 2005) | 2 lines

fixed: pointers passed to Pallete Using

------------------------------------------------------------------------
r2011 | v1ctor | 2005-11-16 10:16:20 -0800 (Wed, 16 Nov 2005) | 4 lines

fixed: stabs declaration for dynamic arrays of pointers

internal changes: "byval as string" args to "byval as zstring ptr"

------------------------------------------------------------------------
r2010 | v1ctor | 2005-11-15 19:39:55 -0800 (Tue, 15 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2009 | v1ctor | 2005-11-14 20:11:48 -0800 (Mon, 14 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2008 | v1ctor | 2005-11-14 19:47:52 -0800 (Mon, 14 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2007 | v1ctor | 2005-11-14 19:43:09 -0800 (Mon, 14 Nov 2005) | 2 lines

added: better pointer checking for function arguments

------------------------------------------------------------------------
r2006 | v1ctor | 2005-11-14 15:53:55 -0800 (Mon, 14 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2005 | v1ctor | 2005-11-14 11:58:53 -0800 (Mon, 14 Nov 2005) | 2 lines

fixed: len() for literal strings was not been evaluated at compile-time

------------------------------------------------------------------------
r2004 | v1ctor | 2005-11-14 11:57:39 -0800 (Mon, 14 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2003 | v1ctor | 2005-11-14 10:48:10 -0800 (Mon, 14 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r2002 | v1ctor | 2005-11-14 10:47:15 -0800 (Mon, 14 Nov 2005) | 2 lines

fixed: wstring's support

------------------------------------------------------------------------
r2001 | v1ctor | 2005-11-14 10:07:54 -0800 (Mon, 14 Nov 2005) | 2 lines

small fix

------------------------------------------------------------------------
r2000 | v1ctor | 2005-11-14 10:06:05 -0800 (Mon, 14 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1999 | v1ctor | 2005-11-14 10:05:28 -0800 (Mon, 14 Nov 2005) | 2 lines

fixed: bitfield initializing

------------------------------------------------------------------------
r1998 | v1ctor | 2005-11-14 10:04:53 -0800 (Mon, 14 Nov 2005) | 2 lines

added: overloaded assert() for wstring expressions

------------------------------------------------------------------------
r1997 | v1ctor | 2005-11-14 09:32:01 -0800 (Mon, 14 Nov 2005) | 2 lines

fixed: integer non-decimal literals were assumed to be unsigned integers

------------------------------------------------------------------------
r1996 | mjscod | 2005-11-14 08:13:11 -0800 (Mon, 14 Nov 2005) | 2 lines

* Fixed an off-by-one bug while printing TABs - sorry about this

------------------------------------------------------------------------
r1995 | v1ctor | 2005-11-13 16:52:52 -0800 (Sun, 13 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1994 | v1ctor | 2005-11-13 16:45:53 -0800 (Sun, 13 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1993 | v1ctor | 2005-11-13 16:45:42 -0800 (Sun, 13 Nov 2005) | 2 lines

added: source-level debugging support for inline asm blocks

------------------------------------------------------------------------
r1992 | v1ctor | 2005-11-12 20:13:46 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1991 | v1ctor | 2005-11-12 20:12:56 -0800 (Sat, 12 Nov 2005) | 2 lines

added: support for multiple typedef declarations and the inverse notation "TYPE AS DataType (Symbol ',')+"

------------------------------------------------------------------------
r1990 | v1ctor | 2005-11-12 19:33:08 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1989 | v1ctor | 2005-11-12 19:25:15 -0800 (Sat, 12 Nov 2005) | 2 lines

fixed: GET and PUT now support field arrays too

------------------------------------------------------------------------
r1988 | v1ctor | 2005-11-12 19:24:30 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1987 | v1ctor | 2005-11-12 19:01:32 -0800 (Sat, 12 Nov 2005) | 2 lines

moved the profiling routines out of rtl-gfx

------------------------------------------------------------------------
r1986 | v1ctor | 2005-11-12 18:49:40 -0800 (Sat, 12 Nov 2005) | 2 lines

fixed: enum's used as param to overloaded functions with no enum args

------------------------------------------------------------------------
r1985 | v1ctor | 2005-11-12 11:13:05 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1984 | v1ctor | 2005-11-12 11:04:30 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1983 | v1ctor | 2005-11-12 10:43:32 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1982 | v1ctor | 2005-11-12 10:35:10 -0800 (Sat, 12 Nov 2005) | 2 lines

small fix

------------------------------------------------------------------------
r1981 | v1ctor | 2005-11-12 09:38:04 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1980 | v1ctor | 2005-11-12 09:30:55 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1979 | v1ctor | 2005-11-12 09:13:32 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1978 | v1ctor | 2005-11-12 08:57:37 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1977 | v1ctor | 2005-11-12 08:45:42 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1976 | v1ctor | 2005-11-12 08:35:52 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1975 | v1ctor | 2005-11-12 08:29:46 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1974 | v1ctor | 2005-11-12 08:17:00 -0800 (Sat, 12 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1973 | v1ctor | 2005-11-11 19:42:51 -0800 (Fri, 11 Nov 2005) | 9 lines

added:
- UTF16 support to lex (and a wstring-aware pre-processor);
- an optional "elements" argument to GET# and PUT#;
- internal: no var-len strings are used anymore as fields;

fixed:
- overloading (was broken because the latest struct field changes);
- SWAP stopped working when pointers to UDT's were used (ditto);

------------------------------------------------------------------------
r1972 | v1ctor | 2005-11-11 19:32:31 -0800 (Fri, 11 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1971 | v1ctor | 2005-11-10 21:02:29 -0800 (Thu, 10 Nov 2005) | 2 lines

fixed: wstrings with unknown size weren't supported

------------------------------------------------------------------------
r1970 | v1ctor | 2005-11-10 20:53:26 -0800 (Thu, 10 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1969 | v1ctor | 2005-11-10 19:04:41 -0800 (Thu, 10 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1968 | mjscod | 2005-11-10 01:37:55 -0800 (Thu, 10 Nov 2005) | 4 lines

* Fixed padding for console mode (off by 1)
* Fixed padding for gfx mode (line length was modified during writing
  to screen directly)

------------------------------------------------------------------------
r1967 | i_am_drv | 2005-11-10 01:05:14 -0800 (Thu, 10 Nov 2005) | 3 lines

- fixed bug 1295848
- changed to recognize .bmp extension if filename contains multiple '.' chars

------------------------------------------------------------------------
r1966 | mjscod | 2005-11-10 00:27:27 -0800 (Thu, 10 Nov 2005) | 2 lines

* Make getkey blocking again (wait for a key press)

------------------------------------------------------------------------
r1965 | v1ctor | 2005-11-09 16:09:23 -0800 (Wed, 09 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1964 | v1ctor | 2005-11-09 15:59:25 -0800 (Wed, 09 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1963 | v1ctor | 2005-11-09 15:46:30 -0800 (Wed, 09 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1962 | i_am_drv | 2005-11-09 14:39:43 -0800 (Wed, 09 Nov 2005) | 2 lines

patch to allow compilation of cgi-util 2.2.1 with DJGPP

------------------------------------------------------------------------
r1961 | v1ctor | 2005-11-09 10:48:50 -0800 (Wed, 09 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1960 | mjscod | 2005-11-09 09:26:38 -0800 (Wed, 09 Nov 2005) | 2 lines

* Fixes SIGSEV when application quits while screen mode was active

------------------------------------------------------------------------
r1959 | v1ctor | 2005-11-08 21:26:03 -0800 (Tue, 08 Nov 2005) | 2 lines

fixed: DATA's initial label would be removed if declared only inside procs

------------------------------------------------------------------------
r1958 | v1ctor | 2005-11-08 21:25:20 -0800 (Tue, 08 Nov 2005) | 2 lines

fixed: chr() was allowing z or wstring's as parameter

------------------------------------------------------------------------
r1957 | v1ctor | 2005-11-08 21:18:21 -0800 (Tue, 08 Nov 2005) | 2 lines

fixed: chr() was allowing z or wstring's as parameter

------------------------------------------------------------------------
r1956 | v1ctor | 2005-11-08 20:18:51 -0800 (Tue, 08 Nov 2005) | 2 lines

fixed: literal wstring's null-term wasn't platform independent

------------------------------------------------------------------------
r1955 | v1ctor | 2005-11-08 19:11:32 -0800 (Tue, 08 Nov 2005) | 2 lines

internal changes: new field node and bitfield type added to make it easier to resolve symbols

------------------------------------------------------------------------
r1954 | v1ctor | 2005-11-08 17:34:43 -0800 (Tue, 08 Nov 2005) | 2 lines

changed: only deallocating the thread handle if wait() is called because the racing condition

------------------------------------------------------------------------
r1953 | v1ctor | 2005-11-08 17:33:24 -0800 (Tue, 08 Nov 2005) | 2 lines

changed: beginthreadex() instead of beginthread() so the handle isn't automatically deallocated

------------------------------------------------------------------------
r1952 | v1ctor | 2005-11-08 16:53:38 -0800 (Tue, 08 Nov 2005) | 2 lines

fixed: no error checking was been done

------------------------------------------------------------------------
r1951 | v1ctor | 2005-11-08 16:14:56 -0800 (Tue, 08 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1950 | v1ctor | 2005-11-06 10:49:22 -0800 (Sun, 06 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1949 | v1ctor | 2005-11-05 13:10:43 -0800 (Sat, 05 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1948 | v1ctor | 2005-11-05 13:08:35 -0800 (Sat, 05 Nov 2005) | 2 lines

exchanged by a header translated by SWIG

------------------------------------------------------------------------
r1947 | v1ctor | 2005-11-05 12:14:01 -0800 (Sat, 05 Nov 2005) | 2 lines

fixed: ascii + unicode string concatenation

------------------------------------------------------------------------
r1945 | v1ctor | 2005-11-05 12:13:28 -0800 (Sat, 05 Nov 2005) | 4 lines

added: compile-time literal strings convertion if ascii and unicode are been concatenated

fixed: ascii + unicode string concatenation

------------------------------------------------------------------------
r1944 | v1ctor | 2005-11-05 12:09:36 -0800 (Sat, 05 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1943 | v1ctor | 2005-11-05 09:40:38 -0800 (Sat, 05 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1942 | v1ctor | 2005-11-05 09:14:43 -0800 (Sat, 05 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1941 | v1ctor | 2005-11-05 09:04:17 -0800 (Sat, 05 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1940 | v1ctor | 2005-11-05 08:11:06 -0800 (Sat, 05 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1939 | v1ctor | 2005-11-05 08:10:24 -0800 (Sat, 05 Nov 2005) | 2 lines

fixed: suffixed byval string args would be missed when looked up if a global suffixed string with the same name existed

------------------------------------------------------------------------
r1938 | v1ctor | 2005-11-05 06:18:35 -0800 (Sat, 05 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1937 | mjscod | 2005-11-04 14:16:42 -0800 (Fri, 04 Nov 2005) | 3 lines

* Updated documentation to reflect that fb_FontType (see gfxlib.txt) now has
  an additional member "w" for the font width.

------------------------------------------------------------------------
r1936 | mjscod | 2005-11-02 03:46:34 -0800 (Wed, 02 Nov 2005) | 2 lines

* Fixed Makefile.in modification

------------------------------------------------------------------------
r1935 | v1ctor | 2005-11-01 16:51:05 -0800 (Tue, 01 Nov 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1934 | mjscod | 2005-10-31 04:06:29 -0800 (Mon, 31 Oct 2005) | 2 lines

* Updated patch so that icu-config is usable after "make install"

------------------------------------------------------------------------
r1933 | mjscod | 2005-10-30 16:45:07 -0800 (Sun, 30 Oct 2005) | 4 lines

* Fixed calling of "fb_Locate" - I overlooked that the default argument for the
  X/Y position isn't -1. Instead it's 0 which caused aborted locates returning wrong
  values and not updating the positions for SPC and TAB

------------------------------------------------------------------------
r1931 | mjscod | 2005-10-30 13:40:42 -0800 (Sun, 30 Oct 2005) | 3 lines

* Added patch to make ICU4C compilable for DJGPP to make FB/DOS'
  RTLIB/RTLIBX compilable.

------------------------------------------------------------------------
r1930 | mjscod | 2005-10-28 04:13:22 -0700 (Fri, 28 Oct 2005) | 7 lines

* Added a new variation that creates an installer with both prebuilt libraries
  and the sources
* Added a small readme that explains the usage
* Fixed the template to ensure that the short cut to start "start_shell" is
  created correctly (with the correct execution folder)
* Added a very small explanation at the top of the replacement files

------------------------------------------------------------------------
r1929 | v1ctor | 2005-10-27 14:46:43 -0700 (Thu, 27 Oct 2005) | 2 lines

added: more complete wstrings support

------------------------------------------------------------------------
r1928 | v1ctor | 2005-10-27 14:39:43 -0700 (Thu, 27 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1927 | v1ctor | 2005-10-27 14:35:34 -0700 (Thu, 27 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1926 | v1ctor | 2005-10-27 14:35:33 -0700 (Thu, 27 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1925 | v1ctor | 2005-10-27 14:12:38 -0700 (Thu, 27 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1924 | v1ctor | 2005-10-27 12:16:16 -0700 (Thu, 27 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1923 | v1ctor | 2005-10-27 12:15:52 -0700 (Thu, 27 Oct 2005) | 2 lines

added: unicode support

------------------------------------------------------------------------
r1922 | v1ctor | 2005-10-27 11:58:27 -0700 (Thu, 27 Oct 2005) | 2 lines

added: unicode support

------------------------------------------------------------------------
r1921 | mjscod | 2005-10-27 05:56:28 -0700 (Thu, 27 Oct 2005) | 2 lines

* Fixed temporary string descriptor leak in short form of OPEN command

------------------------------------------------------------------------
r1920 | v1ctor | 2005-10-26 22:54:19 -0700 (Wed, 26 Oct 2005) | 2 lines

changed: without "volatile" the inline asm block (that doesn't clobber any register) place inside an inline proc called by another inline proc would be completely skipped

------------------------------------------------------------------------
r1919 | mjscod | 2005-10-26 01:06:27 -0700 (Wed, 26 Oct 2005) | 2 lines

* Support for LPRINT for GDI printers

------------------------------------------------------------------------
r1918 | v1ctor | 2005-10-25 14:17:22 -0700 (Tue, 25 Oct 2005) | 2 lines

changed: better "too big" literals skipping

------------------------------------------------------------------------
r1917 | mjscod | 2005-10-25 13:32:37 -0700 (Tue, 25 Oct 2005) | 3 lines

* Fixed a problem where multiple empty lines were sent to printer and were crossing
  the page boundary (cursor Y position was not set correctly)

------------------------------------------------------------------------
r1916 | v1ctor | 2005-10-24 19:39:18 -0700 (Mon, 24 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1915 | v1ctor | 2005-10-24 17:31:43 -0700 (Mon, 24 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1914 | mjscod | 2005-10-24 17:12:52 -0700 (Mon, 24 Oct 2005) | 2 lines

* Fixed character conversion from ANSI CP to CP437 and exit key code to ALT+F4

------------------------------------------------------------------------
r1913 | mjscod | 2005-10-24 14:07:31 -0700 (Mon, 24 Oct 2005) | 2 lines

* Added an example that show the OPEN LPT extension for Win32

------------------------------------------------------------------------
r1912 | mjscod | 2005-10-24 13:59:07 -0700 (Mon, 24 Oct 2005) | 2 lines

* Forgot to emulate the "FormFeed" ( ASCII #12 / 0x0C )

------------------------------------------------------------------------
r1911 | v1ctor | 2005-10-24 13:43:03 -0700 (Mon, 24 Oct 2005) | 2 lines

fixed: wstring assignments and binary operations with scalars

------------------------------------------------------------------------
r1910 | mjscod | 2005-10-24 10:49:14 -0700 (Mon, 24 Oct 2005) | 2 lines

* Modified changelog to reflect the OPEN LPT enhancement for GDI printers

------------------------------------------------------------------------
r1909 | mjscod | 2005-10-24 10:39:09 -0700 (Mon, 24 Oct 2005) | 10 lines

* Printing on Win32 now requires libgdi32 too because of the new emulation
  modes to support PRINTing of OPEN LPT'd GDI printers too
* New Y coordinate after scrolling will now be set by "scroll" hook
  instead of the function that checks for scrolling.
  This affects DOS, Win32, and GFX (all platforms).
* Support for OPEN LPT options like:
  OPEN LPT "LPT:gdi printer name,TITLE=document title,EMU=TTY" AS 1
* Support for RAW and TTY emulation modes for GDI printers
  (opened by OPEN LPT "LPT:")

------------------------------------------------------------------------
r1908 | v1ctor | 2005-10-24 09:41:04 -0700 (Mon, 24 Oct 2005) | 2 lines

changed: comments emitted with double sharps (##) now or GAS would try to parse them(?)

------------------------------------------------------------------------
r1907 | mjscod | 2005-10-24 02:38:24 -0700 (Mon, 24 Oct 2005) | 3 lines

* Forgot to delete temporary string used for code page conversion from ANSI
  to CP437

------------------------------------------------------------------------
r1906 | v1ctor | 2005-10-23 09:41:24 -0700 (Sun, 23 Oct 2005) | 2 lines

fixed: fb_DrvIntlDateGetFormat() instead of fb_DrvIntlGetDateFormat(),

------------------------------------------------------------------------
r1905 | v1ctor | 2005-10-22 20:11:01 -0700 (Sat, 22 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1904 | v1ctor | 2005-10-22 20:08:44 -0700 (Sat, 22 Oct 2005) | 2 lines

fixed: sizeof( some_wstring ) should be multiplied by sizeof( wstring )

------------------------------------------------------------------------
r1903 | v1ctor | 2005-10-22 19:57:31 -0700 (Sat, 22 Oct 2005) | 2 lines

added: allow len() and sizeof() of wstring or zstring (w/o ptr)

------------------------------------------------------------------------
r1902 | v1ctor | 2005-10-22 19:46:51 -0700 (Sat, 22 Oct 2005) | 2 lines

more automation: only add the dirs to test - only one .bas per test, otherwise create a .mk and sub-dirs for more complex tests

------------------------------------------------------------------------
r1901 | v1ctor | 2005-10-22 19:40:11 -0700 (Sat, 22 Oct 2005) | 2 lines

more automation: only add the dirs to test - only one .bas per test, otherwise create a .mk and sub-dirs for more complex tests

------------------------------------------------------------------------
r1900 | v1ctor | 2005-10-22 18:29:38 -0700 (Sat, 22 Oct 2005) | 2 lines

more automation: only add the dirs to test - only one .bas per test

------------------------------------------------------------------------
r1899 | v1ctor | 2005-10-22 17:31:48 -0700 (Sat, 22 Oct 2005) | 2 lines

using VPATH to reduce duplication

------------------------------------------------------------------------
r1898 | v1ctor | 2005-10-22 14:56:29 -0700 (Sat, 22 Oct 2005) | 2 lines

changed: wchar to wstring

------------------------------------------------------------------------
r1897 | v1ctor | 2005-10-22 13:10:42 -0700 (Sat, 22 Oct 2005) | 2 lines

fixed: no integer digits quirk

------------------------------------------------------------------------
r1896 | mjscod | 2005-10-22 03:44:39 -0700 (Sat, 22 Oct 2005) | 2 lines

* Printing a WSTRING now works again

------------------------------------------------------------------------
r1895 | mjscod | 2005-10-22 03:22:48 -0700 (Sat, 22 Oct 2005) | 5 lines

* Avoid code duplication for libfb_con_print_raw/tty functions by moving
  common parts to the libfb_con_print_*_uni.h files
* libfb_con_print_tty_uni.h is aware of character sets with characters that
  use a varibale length of bytes (like UTF-8)

------------------------------------------------------------------------
r1894 | mjscod | 2005-10-22 02:49:29 -0700 (Sat, 22 Oct 2005) | 4 lines

* Compilation fixes for DOS
* Changed calls of conversion functions into macros to allow the platform
  specific unicode header to overwrite them

------------------------------------------------------------------------
r1893 | v1ctor | 2005-10-21 23:20:57 -0700 (Fri, 21 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1892 | v1ctor | 2005-10-21 23:18:45 -0700 (Fri, 21 Oct 2005) | 2 lines

changed: wchar to wstring

------------------------------------------------------------------------
r1891 | v1ctor | 2005-10-21 12:28:12 -0700 (Fri, 21 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1890 | v1ctor | 2005-10-21 12:21:55 -0700 (Fri, 21 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1889 | v1ctor | 2005-10-21 12:20:56 -0700 (Fri, 21 Oct 2005) | 2 lines

exchanged by a header translated by SWIG

------------------------------------------------------------------------
r1888 | v1ctor | 2005-10-21 12:08:31 -0700 (Fri, 21 Oct 2005) | 2 lines

exchanged by header translated by SWIG

------------------------------------------------------------------------
r1887 | v1ctor | 2005-10-21 12:07:12 -0700 (Fri, 21 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1886 | v1ctor | 2005-10-21 11:43:51 -0700 (Fri, 21 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1885 | v1ctor | 2005-10-20 20:50:45 -0700 (Thu, 20 Oct 2005) | 2 lines

changed: literal ascii strings are now allocated internally as char's instead of fixed-len's

------------------------------------------------------------------------
r1884 | v1ctor | 2005-10-20 20:09:15 -0700 (Thu, 20 Oct 2005) | 2 lines

changed: literal ascii strings are now allocated internally as char's instead of fixed-len's

------------------------------------------------------------------------
r1883 | v1ctor | 2005-10-20 17:24:50 -0700 (Thu, 20 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1882 | mjscod | 2005-10-20 15:59:08 -0700 (Thu, 20 Oct 2005) | 2 lines

* Activate changing the palette for FB/DOS, SCREEN 11 and 12

------------------------------------------------------------------------
r1881 | mjscod | 2005-10-20 15:24:06 -0700 (Thu, 20 Oct 2005) | 3 lines

* ISR functions: Added a quirk to allow more stable debugging of ISRs
* Added DOS screen modes 9 and 12

------------------------------------------------------------------------
r1880 | v1ctor | 2005-10-20 14:12:48 -0700 (Thu, 20 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1879 | v1ctor | 2005-10-20 13:46:51 -0700 (Thu, 20 Oct 2005) | 2 lines

added: preliminary wstring's support

------------------------------------------------------------------------
r1877 | v1ctor | 2005-10-20 13:43:14 -0700 (Thu, 20 Oct 2005) | 4 lines

changed: ast was split in different modules

added: preliminary wstring's support

------------------------------------------------------------------------
r1876 | v1ctor | 2005-10-20 13:22:37 -0700 (Thu, 20 Oct 2005) | 2 lines

headers exchanged by machine translated ones

------------------------------------------------------------------------
r1875 | mjscod | 2005-10-20 10:45:25 -0700 (Thu, 20 Oct 2005) | 4 lines

* DOS: Ensure that SLEEP works even when the keyboard input buffer
  is full. It's annoying but this also requires the usage of the keyboard ISR
  (multikey)

------------------------------------------------------------------------
r1874 | mjscod | 2005-10-20 07:33:42 -0700 (Thu, 20 Oct 2005) | 7 lines

* Support for screen modes with depth < 8
* Support for SCREEN 1, 2, 10, and 11 for DOS
* Fixed ISR:
  - segment registers weren*t restored correctly
  - fb_dos_cli and fb_dos_sti can now be used in ISRs too
* Fixed SLEEP for DOS graphics mode (endless loop if called without timeout)

------------------------------------------------------------------------
r1873 | v1ctor | 2005-10-19 12:50:10 -0700 (Wed, 19 Oct 2005) | 2 lines

headers exchanged by machine translated ones

------------------------------------------------------------------------
r1872 | v1ctor | 2005-10-19 12:50:01 -0700 (Wed, 19 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1871 | i_am_drv | 2005-10-19 01:26:52 -0700 (Wed, 19 Oct 2005) | 2 lines

- include dos-specific header from rtlib

------------------------------------------------------------------------
r1870 | v1ctor | 2005-10-17 18:19:16 -0700 (Mon, 17 Oct 2005) | 2 lines

changed: wstring routines' names

------------------------------------------------------------------------
r1869 | mjscod | 2005-10-17 13:16:03 -0700 (Mon, 17 Oct 2005) | 4 lines

* SLEEP must not be aborted when no key is in input queue
  -> reset "key input queue changed" flag when at least one
     key was removed from the input queue

------------------------------------------------------------------------
r1868 | v1ctor | 2005-10-17 10:20:15 -0700 (Mon, 17 Oct 2005) | 2 lines

changed: rtl was split in different modules

------------------------------------------------------------------------
r1867 | v1ctor | 2005-10-17 10:16:35 -0700 (Mon, 17 Oct 2005) | 2 lines

hSleep() to Delay()

------------------------------------------------------------------------
r1866 | v1ctor | 2005-10-16 21:26:48 -0700 (Sun, 16 Oct 2005) | 2 lines

fixed: byval udt's were been pushed in the inverse order

------------------------------------------------------------------------
r1865 | v1ctor | 2005-10-16 21:10:25 -0700 (Sun, 16 Oct 2005) | 2 lines

changed: number of max source files increased to 250

------------------------------------------------------------------------
r1864 | v1ctor | 2005-10-16 14:27:02 -0700 (Sun, 16 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1863 | v1ctor | 2005-10-16 14:21:32 -0700 (Sun, 16 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1862 | v1ctor | 2005-10-16 14:21:26 -0700 (Sun, 16 Oct 2005) | 2 lines

fixed: calling convention should be cdecl

------------------------------------------------------------------------
r1861 | mjscod | 2005-10-16 12:11:08 -0700 (Sun, 16 Oct 2005) | 2 lines

* Forgot to update changelog for INP/OUT declaration fix

------------------------------------------------------------------------
r1860 | v1ctor | 2005-10-16 10:16:27 -0700 (Sun, 16 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1859 | v1ctor | 2005-10-16 10:10:52 -0700 (Sun, 16 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1858 | v1ctor | 2005-10-16 10:08:00 -0700 (Sun, 16 Oct 2005) | 2 lines

fixed: byval udt's were been pushed in the inverse order

------------------------------------------------------------------------
r1857 | v1ctor | 2005-10-16 09:57:04 -0700 (Sun, 16 Oct 2005) | 2 lines

fixed: better range check for Select As Const

------------------------------------------------------------------------
r1856 | mjscod | 2005-10-16 05:19:33 -0700 (Sun, 16 Oct 2005) | 3 lines

* Added assertion to ensure that all procedures that require error checking
  return an INTEGER

------------------------------------------------------------------------
r1855 | mjscod | 2005-10-16 05:02:10 -0700 (Sun, 16 Oct 2005) | 3 lines

* Fixed declaration of fb_Out (must return INTEGER instead of VOID)
* Fixed declaration of fb_In (must not evaluate return value as error code)

------------------------------------------------------------------------
r1854 | v1ctor | 2005-10-15 20:41:56 -0700 (Sat, 15 Oct 2005) | 2 lines

minor cosmetic change

------------------------------------------------------------------------
r1853 | v1ctor | 2005-10-15 20:41:27 -0700 (Sat, 15 Oct 2005) | 2 lines

changed: rtl internal functions lookup (rtl.bas will be split next)

------------------------------------------------------------------------
r1852 | v1ctor | 2005-10-15 20:40:07 -0700 (Sat, 15 Oct 2005) | 2 lines

fixed: field arrays were not been recognized

------------------------------------------------------------------------
r1851 | v1ctor | 2005-10-15 18:03:04 -0700 (Sat, 15 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1850 | mjscod | 2005-10-15 14:02:01 -0700 (Sat, 15 Oct 2005) | 2 lines

* Added "deep sleep" in a way similar to "jofers" suggestion

------------------------------------------------------------------------
r1849 | v1ctor | 2005-10-15 13:29:03 -0700 (Sat, 15 Oct 2005) | 2 lines

added: by descriptor arguments support for overloaded functions

------------------------------------------------------------------------
r1848 | v1ctor | 2005-10-15 13:27:24 -0700 (Sat, 15 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1847 | v1ctor | 2005-10-15 11:55:48 -0700 (Sat, 15 Oct 2005) | 2 lines

fixed: optional params before the end of the param list were not been created for overloaded functions

------------------------------------------------------------------------
r1846 | v1ctor | 2005-10-15 10:36:58 -0700 (Sat, 15 Oct 2005) | 2 lines

fixed: optional params before the end of the param list were not been created for overloaded functions

------------------------------------------------------------------------
r1845 | mjscod | 2005-10-15 01:51:56 -0700 (Sat, 15 Oct 2005) | 4 lines

* Fixed macro to check if a symbol refers to an array
  Previously, it didn't check if the symbol is of class "VAR" first which causes
  a SIGSEGV (at least on my PC and I guess on  d.j.peters linux too)

------------------------------------------------------------------------
r1844 | mjscod | 2005-10-15 01:21:04 -0700 (Sat, 15 Oct 2005) | 5 lines

* Renamed fb_hSleep to fb_Delay to prepare for addition of
  jofers SLEEP extension
* Also added a "compatibility" function so that old applications calling
  fb_hSleep directly are still working

------------------------------------------------------------------------
r1843 | v1ctor | 2005-10-14 13:21:11 -0700 (Fri, 14 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1842 | v1ctor | 2005-10-14 11:05:29 -0700 (Fri, 14 Oct 2005) | 2 lines

added: msbitfields option when compiling in the Windows platform

------------------------------------------------------------------------
r1841 | v1ctor | 2005-10-14 11:04:34 -0700 (Fri, 14 Oct 2005) | 2 lines

fixed: bitfields packing when not using the M$-way

------------------------------------------------------------------------
r1840 | v1ctor | 2005-10-13 15:22:38 -0700 (Thu, 13 Oct 2005) | 2 lines

changed: hlp was split in different modules

------------------------------------------------------------------------
r1839 | v1ctor | 2005-10-13 15:22:04 -0700 (Thu, 13 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1838 | v1ctor | 2005-10-13 15:21:50 -0700 (Thu, 13 Oct 2005) | 2 lines

fixed: enum types couldn't be initialized

------------------------------------------------------------------------
r1837 | v1ctor | 2005-10-13 15:20:52 -0700 (Thu, 13 Oct 2005) | 2 lines

changed: lexpp was split in different modules

------------------------------------------------------------------------
r1836 | mjscod | 2005-10-13 01:10:29 -0700 (Thu, 13 Oct 2005) | 10 lines

* Fixed prototype for thread create implementation for DOS
* Make XBOX and DOS compilable again by adding a dummy typedef
  for FB_DIRCTX
* Adding SLEEP hook
* SLEEP now works exactly like in QB on Win32 CONSOLE, DOS CONSOLE
  and graphics mode. XBOX console simply isn't supported and I'm not
  experienced enough to create a Linux implementation. In a Linux console
  it behaves like SLEEP of 0.14b
* Added missing obj/cygwin folder for gfxlib2

------------------------------------------------------------------------
r1835 | v1ctor | 2005-10-12 22:55:35 -0700 (Wed, 12 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1834 | v1ctor | 2005-10-12 22:50:58 -0700 (Wed, 12 Oct 2005) | 2 lines

added: proxy threads to deallocate local contexts storage

------------------------------------------------------------------------
r1833 | v1ctor | 2005-10-12 22:39:40 -0700 (Wed, 12 Oct 2005) | 2 lines

added: proxy threads to deallocate local contexts storage

------------------------------------------------------------------------
r1832 | v1ctor | 2005-10-12 22:10:57 -0700 (Wed, 12 Oct 2005) | 2 lines

changed: symb was split in different modules

------------------------------------------------------------------------
r1831 | v1ctor | 2005-10-12 20:12:19 -0700 (Wed, 12 Oct 2005) | 2 lines

changed: the input buffer will not be cleared when using mt version

------------------------------------------------------------------------
r1830 | v1ctor | 2005-10-12 20:11:27 -0700 (Wed, 12 Oct 2005) | 2 lines

changed: the input buffer will not be cleared when using mt version

------------------------------------------------------------------------
r1829 | v1ctor | 2005-10-12 20:10:28 -0700 (Wed, 12 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1828 | v1ctor | 2005-10-12 19:03:40 -0700 (Wed, 12 Oct 2005) | 2 lines

added: proxy threads to deallocate local contexts storage

------------------------------------------------------------------------
r1827 | mjscod | 2005-10-12 16:45:36 -0700 (Wed, 12 Oct 2005) | 2 lines

* Forgot to add commands to remove both HTTP URL links from start menu

------------------------------------------------------------------------
r1826 | mjscod | 2005-10-12 16:31:57 -0700 (Wed, 12 Oct 2005) | 7 lines

* Re-added changelig entry regarding ALT+F4 for console mode
* Simplified the FB_LOCK/FB_UNLOCK usage a lot by creating the new
  macros FB_IO_EXIT_LOCK and FB_IO_EXIT_UNLOCK - no need to
  guard the FB_LOCK for "device close" functions with #if's
* moved fb_is_Exiting to init file (seems to be more natural and it uses
  only 4 bytes, so who cares?

------------------------------------------------------------------------
r1825 | v1ctor | 2005-10-12 15:52:13 -0700 (Wed, 12 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1824 | v1ctor | 2005-10-12 15:50:55 -0700 (Wed, 12 Oct 2005) | 2 lines

added: proxy threads to deallocate local contexts storage

------------------------------------------------------------------------
r1823 | mjscod | 2005-10-12 12:18:27 -0700 (Wed, 12 Oct 2005) | 5 lines

* Multithreading fixes
  -> Usage of FB_LOCK() / FB_UNLOCK() after end of fb_hEnd
  -> The way, fb_dirctx was accessed in fb_hEnd was wrong (had to use
      the FB_TLSGET macro)

------------------------------------------------------------------------
r1822 | v1ctor | 2005-10-12 09:17:40 -0700 (Wed, 12 Oct 2005) | 2 lines

changed: argc & argv handling

------------------------------------------------------------------------
r1821 | mjscod | 2005-10-12 02:20:26 -0700 (Wed, 12 Oct 2005) | 3 lines

* OPENHOOK header/example now reflects that OPEN COM only exists
  on Win32/Cygwin

------------------------------------------------------------------------
r1820 | mjscod | 2005-10-11 15:11:50 -0700 (Tue, 11 Oct 2005) | 3 lines

* Changed "CLOSE WINDOW" key code to CHR$(&HFF, &H6B) (ALT+F4)
* "CLOSE WINDOW" key code now works in Win32 console too

------------------------------------------------------------------------
r1819 | mjscod | 2005-10-11 14:49:09 -0700 (Tue, 11 Oct 2005) | 7 lines

* Simplified libfb_io_keys.c because the old fall-back method isn't required any
  more (AFAIK)
* Reverted previous change: ALT+NumKey can only reliably work when
  NUM-LOCK is ON, otherwise we get an interference with the ALT+CursorKeys
* The CLOSE key code is now changed to ALT+F4 for Win32. Other ports will
  follow

------------------------------------------------------------------------
r1818 | mjscod | 2005-10-11 14:18:25 -0700 (Tue, 11 Oct 2005) | 4 lines

* Fixed ALT+NumKey when NUM-LOCK is OFF
* ALT+NumKey doesn't produce the extra key codes any more to avoid
  confusion with the cursor key codes

------------------------------------------------------------------------
r1817 | mjscod | 2005-10-11 03:47:51 -0700 (Tue, 11 Oct 2005) | 7 lines

* Removed tmpl_src.nsi because template.nsi can be used too for the
  source distribution.
* Added distribution with already built import libraries (no need to call
  genimplibs any more)
* Use the uninstaller start and end page from the Modern UI instead of using
  dialog boxes (no need to translate more than required)

------------------------------------------------------------------------
r1816 | v1ctor | 2005-10-10 21:08:17 -0700 (Mon, 10 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1815 | v1ctor | 2005-10-10 21:07:52 -0700 (Mon, 10 Oct 2005) | 2 lines

added: portuguese i8n

------------------------------------------------------------------------
r1814 | mjscod | 2005-10-10 16:21:12 -0700 (Mon, 10 Oct 2005) | 2 lines

* ALT+NumKeys works now for Win32 console mode

------------------------------------------------------------------------
r1813 | mjscod | 2005-10-10 12:16:32 -0700 (Mon, 10 Oct 2005) | 2 lines

* Added some missing folder-creation guards for cygwin

------------------------------------------------------------------------
r1812 | mjscod | 2005-10-10 11:46:05 -0700 (Mon, 10 Oct 2005) | 3 lines

* Fix for screens where the default font doesn't fill the screen completely
  (i.e. screen_height / font_height <> screen_height \ font_height)

------------------------------------------------------------------------
r1811 | mjscod | 2005-10-10 09:57:48 -0700 (Mon, 10 Oct 2005) | 3 lines

* Forgot the handling of CLS with no arguments and no view port set for
  Gfx-Mode (same as CLS 2, otherwise CLS 1)

------------------------------------------------------------------------
r1810 | mjscod | 2005-10-10 08:45:46 -0700 (Mon, 10 Oct 2005) | 2 lines

* When using TinyPTC, INKEY$ returns the correct key codes now

------------------------------------------------------------------------
r1809 | mjscod | 2005-10-10 08:02:40 -0700 (Mon, 10 Oct 2005) | 3 lines

* Fixed a stupid bug for the screencopy/pcopy function when the arguments are
  from_page = -1 and to_page = 0 for a fb_mode->working_page = 0

------------------------------------------------------------------------
r1808 | mjscod | 2005-10-10 07:50:06 -0700 (Mon, 10 Oct 2005) | 4 lines

* Removed those stupid libfb_hook_locate.c and libfb_hook_locate_sub.c
  functions
* Added missing files for the LINE INPUT and SCREEN() support for gfx-mode

------------------------------------------------------------------------
r1807 | mjscod | 2005-10-10 07:46:00 -0700 (Mon, 10 Oct 2005) | 12 lines

* SCREEN function is now supported in graphics mode
* LINE INPUT is now fully supported in graphics mode
* The build mk_dat executable is now stored in the platform specific folder
  that contains the object files created during compilation of the library
* Made SCREEN function (fb_ReadXY) hookable
* Made LINE INPUT (fb_LineInput) hookable
* Source code clean-up for DOS port of gfxlib2
* Renamed libfb_hook_locate.c and libfb_hook_locate_sub.c to
  libfb_con_locate.c and libfb_con_locate_sub.c because those functions
  are just redirections to fb_LocateEx (libfb_hook_locate_ex.c)
* Made the line input method much more thread friendly

------------------------------------------------------------------------
r1806 | mjscod | 2005-10-09 17:53:41 -0700 (Sun, 09 Oct 2005) | 2 lines

* Updated documentation

------------------------------------------------------------------------
r1805 | mjscod | 2005-10-09 17:48:07 -0700 (Sun, 09 Oct 2005) | 3 lines

* Fixed: CLS 2 clears the VIEW PRINT range only, while CLS 1 does nothing
  in console mode

------------------------------------------------------------------------
r1804 | mjscod | 2005-10-09 17:38:35 -0700 (Sun, 09 Oct 2005) | 2 lines

* fb_GfxClear cleared too much - queried text height instead of font height

------------------------------------------------------------------------
r1803 | mjscod | 2005-10-09 17:35:26 -0700 (Sun, 09 Oct 2005) | 3 lines

* Clearing the entire screen moved the cursor to an invalid position
* fb_GfxClear always resetted the drawing position

------------------------------------------------------------------------
r1802 | mjscod | 2005-10-09 17:31:46 -0700 (Sun, 09 Oct 2005) | 10 lines

* Added missing serial I/O function stubs for DOS, Linux, and XBOIX
* VIEW PRINT doesn't rely on console-only functions any more
* Added hook for VIEW PRINT change notification
* Merged fb_Locate and fb_LocateSub into fb_LocateEx
* WIDTH now resets the cursor position for all platforms (not Win32 only)
* fb_GfxClear now respects VIEW PRINT
* fb_GfxClear will only change the cursor position if required
* fb_GfxClear will only change the drawing position if required
* SCREEN and fb_GfxWidth reset the view ports (VIEW PRINT and VIEW)

------------------------------------------------------------------------
r1801 | v1ctor | 2005-10-08 15:34:04 -0700 (Sat, 08 Oct 2005) | 2 lines

changed: parser2 was split in different modules

------------------------------------------------------------------------
r1800 | v1ctor | 2005-10-08 15:33:10 -0700 (Sat, 08 Oct 2005) | 2 lines

changed: parser3 renamed to parser-expr-variable

------------------------------------------------------------------------
r1799 | v1ctor | 2005-10-08 15:15:54 -0700 (Sat, 08 Oct 2005) | 2 lines

changed: parser7 renamed to parser-quirk-gfx

------------------------------------------------------------------------
r1798 | v1ctor | 2005-10-08 15:15:11 -0700 (Sat, 08 Oct 2005) | 2 lines

changed: parser7 renamed to parser-gfx

------------------------------------------------------------------------
r1797 | v1ctor | 2005-10-08 15:04:47 -0700 (Sat, 08 Oct 2005) | 2 lines

changed: parser6 was split in different modules

------------------------------------------------------------------------
r1796 | v1ctor | 2005-10-08 14:28:52 -0700 (Sat, 08 Oct 2005) | 2 lines

changed: parser5 renamed to parser-proc

------------------------------------------------------------------------
r1795 | v1ctor | 2005-10-08 12:39:57 -0700 (Sat, 08 Oct 2005) | 2 lines

fixed: longer strings should be allowed with CV#

------------------------------------------------------------------------
r1794 | v1ctor | 2005-10-07 21:43:32 -0700 (Fri, 07 Oct 2005) | 2 lines

changed: parser4 was split in different modules

------------------------------------------------------------------------
r1793 | v1ctor | 2005-10-07 18:32:36 -0700 (Fri, 07 Oct 2005) | 2 lines

changed: parser1 was split in different modules

------------------------------------------------------------------------
r1792 | i_am_drv | 2005-10-05 23:48:59 -0700 (Wed, 05 Oct 2005) | 2 lines

added quotes around executable path to allow for spaces in Program Files path

------------------------------------------------------------------------
r1791 | i_am_drv | 2005-10-05 23:05:23 -0700 (Wed, 05 Oct 2005) | 2 lines

added rules to make Setup.exe and SetupSource.exe

------------------------------------------------------------------------
r1790 | mjscod | 2005-10-03 13:23:56 -0700 (Mon, 03 Oct 2005) | 4 lines

* Forgot to add library user32 when OPEN SCRN was used
* Fixed a bug regarding multi-line scrolling on Win32 systems not clearing
  some regions of the console screen buffer

------------------------------------------------------------------------
r1789 | mjscod | 2005-10-03 09:06:25 -0700 (Mon, 03 Oct 2005) | 3 lines

* DOS console output speed-up because text is directly
  written into the screen memory (when it's safe)

------------------------------------------------------------------------
r1788 | v1ctor | 2005-10-02 13:59:14 -0700 (Sun, 02 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1787 | v1ctor | 2005-10-02 13:07:27 -0700 (Sun, 02 Oct 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1786 | v1ctor | 2005-10-02 13:03:34 -0700 (Sun, 02 Oct 2005) | 2 lines

fixed: var-initializing of non-local data was not taking the number of array elements into account, causing wrong alignments

------------------------------------------------------------------------
r1785 | i_am_drv | 2005-10-02 01:29:54 -0700 (Sun, 02 Oct 2005) | 2 lines

DOS joystick input - untested

------------------------------------------------------------------------
r1784 | mjscod | 2005-10-01 14:54:31 -0700 (Sat, 01 Oct 2005) | 4 lines

* Fixed a very stupid bug that overwrote the current cursor position with the
  wrong values. Instead of setting a new position, the variable dwNewCoord
  was renamed to dwMoveCoord and contains only relative cursor positions.

------------------------------------------------------------------------
r1783 | v1ctor | 2005-10-01 08:08:15 -0700 (Sat, 01 Oct 2005) | 2 lines

fixed: dup definition when including windows.bi too

------------------------------------------------------------------------
r1782 | v1ctor | 2005-10-01 08:08:06 -0700 (Sat, 01 Oct 2005) | 2 lines

fixed: dup definition when including crt.bi too

------------------------------------------------------------------------
r1781 | i_am_drv | 2005-09-30 13:14:34 -0700 (Fri, 30 Sep 2005) | 3 lines

- SQLite static library for DOS (DrV)
- stack size (-t) parameter for DOS (DrV)

------------------------------------------------------------------------
r1780 | i_am_drv | 2005-09-30 13:12:33 -0700 (Fri, 30 Sep 2005) | 2 lines

added -t (stack size) parameter for dos

------------------------------------------------------------------------
r1779 | v1ctor | 2005-09-30 07:50:06 -0700 (Fri, 30 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1778 | i_am_drv | 2005-09-29 23:32:56 -0700 (Thu, 29 Sep 2005) | 2 lines

patch for sqlite 3.2.7 to compile with DJGPP

------------------------------------------------------------------------
r1777 | i_am_drv | 2005-09-29 20:12:09 -0700 (Thu, 29 Sep 2005) | 2 lines

patch for sqlite 3.2.6 to compile with DJGPP

------------------------------------------------------------------------
r1776 | v1ctor | 2005-09-29 14:54:26 -0700 (Thu, 29 Sep 2005) | 2 lines

fixed: integer overflow was happening when near to the daily wrap-around (37 bits are need to express 86399 * 1000000), no wrap-around is done now

------------------------------------------------------------------------
r1775 | v1ctor | 2005-09-29 14:50:18 -0700 (Thu, 29 Sep 2005) | 2 lines

fixed: integer overflow was happening when near to daily wrap-around (37 bits are need to express 86399 * 1000000), no wrap-around is done now

------------------------------------------------------------------------
r1774 | v1ctor | 2005-09-28 23:56:39 -0700 (Wed, 28 Sep 2005) | 2 lines

added: math overflow handling

------------------------------------------------------------------------
r1773 | v1ctor | 2005-09-28 10:35:40 -0700 (Wed, 28 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1772 | v1ctor | 2005-09-28 10:35:10 -0700 (Wed, 28 Sep 2005) | 2 lines

fixed: ambiguity with colon ':' used after anonymous inner UDT's

------------------------------------------------------------------------
r1771 | v1ctor | 2005-09-28 09:50:10 -0700 (Wed, 28 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1770 | v1ctor | 2005-09-28 09:49:01 -0700 (Wed, 28 Sep 2005) | 2 lines

fixed: QB has a different order of precedence for the logical operators OR and AND

------------------------------------------------------------------------
r1769 | v1ctor | 2005-09-25 15:39:57 -0700 (Sun, 25 Sep 2005) | 2 lines

changed: no extended error msg if proc is part of the rtlib

------------------------------------------------------------------------
r1768 | v1ctor | 2005-09-25 15:30:35 -0700 (Sun, 25 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1767 | v1ctor | 2005-09-25 15:25:01 -0700 (Sun, 25 Sep 2005) | 2 lines

small change..

------------------------------------------------------------------------
r1766 | mjscod | 2005-09-25 13:05:50 -0700 (Sun, 25 Sep 2005) | 4 lines

- Non-Win32: Numerical constants are converted from unsigned to signed
  before converted to string to match the reported constant type.
  Fixes bug with constants >=&H80000000

------------------------------------------------------------------------
r1765 | mjscod | 2005-09-25 09:16:47 -0700 (Sun, 25 Sep 2005) | 6 lines

* Hopefully fixed all outstanding issues regarding key codes in Win32 gfx mode
* Removed assertion from Win32's PRINT implementation
* Made Win32's key code translation more flexible (to match DOS' behaviour)
* Ensure that the MULTIKEY array doesn't get updated when the application
  doesn't have the focus

------------------------------------------------------------------------
r1764 | mjscod | 2005-09-25 07:18:43 -0700 (Sun, 25 Sep 2005) | 2 lines

* Some compilation fixes for DOS (mk_dat.c) and Cygwin (other files)

------------------------------------------------------------------------
r1763 | mjscod | 2005-09-24 11:16:49 -0700 (Sat, 24 Sep 2005) | 2 lines

* Fixed the WRITE with empty strings

------------------------------------------------------------------------
r1762 | mjscod | 2005-09-24 11:00:56 -0700 (Sat, 24 Sep 2005) | 7 lines

* Removed debug output
* Fixed the dead-char behaviour
* Added character set conversion to 437
... however, ALT+1/2/4/5/6 aren't working the same way as under DOS
    but I don't see how to fix this now because we MUST NOT use ToAscii
    because this would break the dead-key behaviour ...

------------------------------------------------------------------------
r1761 | mjscod | 2005-09-24 07:19:34 -0700 (Sat, 24 Sep 2005) | 5 lines

* F10/F11/F12 are now returned again in Win32 gfx mode
* Moved Win32 specific translation from libfb_io_input.c to libfb_io_keys.c
  to allow the usage in gfx-mode too
* Removed libfb_gfx_data.c because it'll be regenerated anyways

------------------------------------------------------------------------
r1760 | mjscod | 2005-09-24 05:54:37 -0700 (Sat, 24 Sep 2005) | 6 lines

* I forgot to add those files ...
  - libfb_gfx_vars.c contains all global variables from libfb_gfx_screen.c
    (to keep the size of the resulting EXE as low as possible for mk_dat)
  - fb_gfx_data.h now contains the declarations of the data defined in
    fb_gfx_data.c

------------------------------------------------------------------------
r1759 | mjscod | 2005-09-24 05:52:39 -0700 (Sat, 24 Sep 2005) | 2 lines

* Reverted an wrong mode definition (for screen 12)

------------------------------------------------------------------------
r1758 | mjscod | 2005-09-24 05:39:25 -0700 (Sat, 24 Sep 2005) | 5 lines

* libfb_gfx_data.c will now be created during the build process by the
  "mk_dat" tool
* Setting SCREEN 0 on exit might be not required - query fb_mode first
* Fix multiple atexit's when using SCREEN multiple times

------------------------------------------------------------------------
r1757 | mjscod | 2005-09-23 00:53:12 -0700 (Fri, 23 Sep 2005) | 8 lines

* Simplified the rewritten PRINT stuff a lot
  - Locate hook isn't required any more
  - The "Write" hook must now write all data to the screen - otherwise it has to
    return an error
* Win32: Now uses direct screen buffer access
  Using this API it's not required any more to change the  screen buffer mode
  (should improve compatibility with Win9x systems)

------------------------------------------------------------------------
r1756 | mjscod | 2005-09-23 00:14:46 -0700 (Fri, 23 Sep 2005) | 4 lines

* Fix for the previous size and speed optimization - when VIEW PRINT was
  used, the console window was resized to the size of the VIEW which is a very
  bad thing ...

------------------------------------------------------------------------
r1755 | mjscod | 2005-09-22 23:52:36 -0700 (Thu, 22 Sep 2005) | 5 lines

* Reduced memory footprint by removing unnecessary calls to fb_ConsoleLocate
  and using only Win32 API calls for Win32's PrintBufferEx
* Speedup of console output on Win32 by remving unnecessary calls - instead we
  store all required information at the begin of a PRINT

------------------------------------------------------------------------
r1754 | mjscod | 2005-09-22 14:26:51 -0700 (Thu, 22 Sep 2005) | 4 lines

* DOS: Some cleanup in MULTIKEY
* DOS: Fixed memory leak in ISR routine
* DOS: Forgot to revert redirection of ISR's when appliation quits

------------------------------------------------------------------------
r1753 | mjscod | 2005-09-22 13:40:08 -0700 (Thu, 22 Sep 2005) | 19 lines

* DOS + Win32: TTY interpretation (PRINT) is now done by RTLIB itself
* Win32: Fixed the "normal" console application behaviour ... instead of
  scrolling up the text, it moves the window down
* Added a QB-like (LINE) INPUT function that replaces TABs by SPCs
  and allows navigation with the cursor
* DOS: Replaced many references to CONIO.H by calls to the respective BIOS
  interrupt functions
* DOS: Support scrolling in VIEW PRINT areas
* DOS: Fix the behaviour with the character at the lower right corner of the screen
* DOS: MULTIKEY now uses the new ISR functions. This means:
  - no need to do scancode->ASCII translations ourselves (important for
    non-US systems)
* DOS: MULTIKEY now respects the extended scan codes and suppresses 0x2A
  to get rid of the spurious SC_LSHIFT signals
* DOS: removed the keyboard interrupt handler from gfxlib2 because it can easily
  use the one for the console mode.
* A LINE INPUT via SCRN: is now redirected to the "normal" LINE INPUT
* Updated hello.bas example to turn the cursor on at the end

------------------------------------------------------------------------
r1752 | v1ctor | 2005-09-20 20:53:34 -0700 (Tue, 20 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1751 | v1ctor | 2005-09-20 19:20:10 -0700 (Tue, 20 Sep 2005) | 2 lines

fixed: the presence of an index was not been checked in arrays of UDT's

------------------------------------------------------------------------
r1750 | v1ctor | 2005-09-20 19:18:16 -0700 (Tue, 20 Sep 2005) | 2 lines

fixed: invalid PP symbols are now checked when a PP block is been skipped

------------------------------------------------------------------------
r1749 | v1ctor | 2005-09-20 18:40:12 -0700 (Tue, 20 Sep 2005) | 2 lines

fixed: fpoint to ulongint store was not been handled by a function

------------------------------------------------------------------------
r1748 | mjscod | 2005-09-20 01:29:00 -0700 (Tue, 20 Sep 2005) | 11 lines

* Fixed some (LINE) INPUT (for CONSOLE) problems:
  - The add_newline behaviour changed so that up to two empty lines
    were printed after the user completed the input
  - When the last character written was at the lower right corner of the screen
    buffer, the INPUT function didn't move to the beginning of the next line
* Added new PRINT flag which is currently only useful for the Win32 / CYGWIN
  platforms to allow the position correction after a character was printed to the
  lower right corner of the screen
* Fixed the detached execution of external processes on CYGWIN
* Added the missing prototypes for fb_PrintBuffer(Ex) to fb.h

------------------------------------------------------------------------
r1747 | mjscod | 2005-09-19 16:51:26 -0700 (Mon, 19 Sep 2005) | 3 lines

* Ensure that no intermediate files - which are created during driver
   compilation - are added to the resulting setup executable any more.

------------------------------------------------------------------------
r1746 | mjscod | 2005-09-19 14:27:27 -0700 (Mon, 19 Sep 2005) | 2 lines

* Fixed SIGSEGV when GETJOYSTICK was used while in CONSOLE (non-gfx) mode

------------------------------------------------------------------------
r1745 | v1ctor | 2005-09-19 12:53:19 -0700 (Mon, 19 Sep 2005) | 4 lines

fixed: byref args with optional values were reusing the value in case of strings and floating-point types

changed: byref args are now handled at AST so more address-of optimizations can be done

------------------------------------------------------------------------
r1744 | v1ctor | 2005-09-19 12:50:08 -0700 (Mon, 19 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1743 | mjscod | 2005-09-19 10:55:15 -0700 (Mon, 19 Sep 2005) | 2 lines

* Adding "fake" Unicode support for DOS by defining wchar_t as char

------------------------------------------------------------------------
r1742 | mjscod | 2005-09-19 10:12:04 -0700 (Mon, 19 Sep 2005) | 4 lines

* Now sets the last cursor position correctly when last character cell
  of screen was written
* WIDTH to a smaller screen size now works correctly

------------------------------------------------------------------------
r1741 | mjscod | 2005-09-19 08:25:48 -0700 (Mon, 19 Sep 2005) | 2 lines

* Fixed problem with scrolling when scrolling had to be turned off after last output

------------------------------------------------------------------------
r1740 | i_am_drv | 2005-09-17 10:14:15 -0700 (Sat, 17 Sep 2005) | 2 lines

fixed problem with wraparound when subtracting 1 from unsigned 0 in FD_SET_

------------------------------------------------------------------------
r1739 | v1ctor | 2005-09-15 08:06:27 -0700 (Thu, 15 Sep 2005) | 2 lines

fixed: error checking when parsing comments

------------------------------------------------------------------------
r1738 | v1ctor | 2005-09-15 08:02:41 -0700 (Thu, 15 Sep 2005) | 2 lines

fixed: reverted to the old working version

------------------------------------------------------------------------
r1737 | mjscod | 2005-09-14 12:08:56 -0700 (Wed, 14 Sep 2005) | 2 lines

* Forgot to rename all calls to fb_LocateFn ... sorry

------------------------------------------------------------------------
r1736 | mjscod | 2005-09-14 06:04:32 -0700 (Wed, 14 Sep 2005) | 2 lines

* Renamed fb_LocateFn back to fb_Locate to avoid a 1/2 chicken-egg problem

------------------------------------------------------------------------
r1735 | mjscod | 2005-09-13 20:28:33 -0700 (Tue, 13 Sep 2005) | 5 lines

* Split fb_Locate to fb_LocateFn and fb_LocateSub
* fb_LocateSub returns an error if called with invalid arguments
* compiler now distinguishes between SUB and FUNCTION variant of the
  LOCATE instruction

------------------------------------------------------------------------
r1734 | mjscod | 2005-09-13 19:16:29 -0700 (Tue, 13 Sep 2005) | 7 lines

* I (hopefully) fixed all remaining printing issues on Win32
  All problems regarding VIEW PRINT should be fixed too ...
  Interpretation of the control codes is now done by ourselves

* On Cygwin, the native Win32 Beep is used
  (otherwise we get endless recursion)

------------------------------------------------------------------------
r1733 | mjscod | 2005-09-13 04:30:12 -0700 (Tue, 13 Sep 2005) | 2 lines

* Added default OBJ folders

------------------------------------------------------------------------
r1732 | mjscod | 2005-09-12 04:28:28 -0700 (Mon, 12 Sep 2005) | 2 lines

* Definition of an argument-less overloaded function didn't work any more ...

------------------------------------------------------------------------
r1731 | v1ctor | 2005-09-11 21:45:22 -0700 (Sun, 11 Sep 2005) | 2 lines

fixed: function overloading didn't support arg-less procs

------------------------------------------------------------------------
r1730 | mjscod | 2005-09-11 10:43:51 -0700 (Sun, 11 Sep 2005) | 2 lines

* Updated changelog.txt to include the fix of the overload-0-arguments bug

------------------------------------------------------------------------
r1729 | mjscod | 2005-09-11 10:34:29 -0700 (Sun, 11 Sep 2005) | 2 lines

* Fix for the fix for the overloaded function with 0 arguments

------------------------------------------------------------------------
r1728 | mjscod | 2005-09-11 10:17:25 -0700 (Sun, 11 Sep 2005) | 2 lines

* "clean" now removes FreeBASIC_src.nsi too

------------------------------------------------------------------------
r1727 | mjscod | 2005-09-11 10:07:22 -0700 (Sun, 11 Sep 2005) | 2 lines

* Updated documentation

------------------------------------------------------------------------
r1726 | mjscod | 2005-09-11 10:00:24 -0700 (Sun, 11 Sep 2005) | 4 lines

* Added support for a QB-compatible POS(n)
  However, using POS() instead of POS(n) is faster - I just added POS(n)
  to support easier compilaton of old sources.

------------------------------------------------------------------------
r1725 | mjscod | 2005-09-11 09:53:34 -0700 (Sun, 11 Sep 2005) | 2 lines

* Hopefully fixed the problem with OVERLOAD functions with no arguments

------------------------------------------------------------------------
r1724 | mjscod | 2005-09-11 02:42:29 -0700 (Sun, 11 Sep 2005) | 3 lines

* Moved initialization of console input to Win32 init function to avoid
  strange behaviour before INKEY$ function (and similar) gets called

------------------------------------------------------------------------
r1723 | mjscod | 2005-09-11 02:36:28 -0700 (Sun, 11 Sep 2005) | 2 lines

* fb_ConsoleProcessEvents now reads multiple events for a single call

------------------------------------------------------------------------
r1722 | v1ctor | 2005-09-10 11:36:16 -0700 (Sat, 10 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1721 | v1ctor | 2005-09-10 10:16:10 -0700 (Sat, 10 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1720 | mjscod | 2005-09-10 02:31:06 -0700 (Sat, 10 Sep 2005) | 4 lines

* Restricts floating point number precision to 11 when no format specifier
  was given.
* Updated test case

------------------------------------------------------------------------
r1719 | mjscod | 2005-09-10 02:20:24 -0700 (Sat, 10 Sep 2005) | 3 lines

* Fixed format specifier (was a non-standard %lf)
* Updated test cases

------------------------------------------------------------------------
r1718 | v1ctor | 2005-09-09 22:42:06 -0700 (Fri, 09 Sep 2005) | 2 lines

fixed: precision of str2flt was a digit too wide

------------------------------------------------------------------------
r1717 | v1ctor | 2005-09-09 22:16:24 -0700 (Fri, 09 Sep 2005) | 2 lines

added: missing macros

------------------------------------------------------------------------
r1716 | v1ctor | 2005-09-09 17:57:34 -0700 (Fri, 09 Sep 2005) | 2 lines

fixed: address of a static initialized string var was not updating the access counter

------------------------------------------------------------------------
r1715 | v1ctor | 2005-09-09 17:54:06 -0700 (Fri, 09 Sep 2005) | 2 lines

fixed: - macro's stringize (#) can now be used with literal strings;

------------------------------------------------------------------------
r1714 | v1ctor | 2005-09-09 17:24:24 -0700 (Fri, 09 Sep 2005) | 2 lines

fixed: - macro's stringize (#) can now be used with literal strings;

------------------------------------------------------------------------
r1713 | v1ctor | 2005-09-09 15:04:47 -0700 (Fri, 09 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1712 | v1ctor | 2005-09-09 14:47:16 -0700 (Fri, 09 Sep 2005) | 4 lines

fixed:
- single's should be convert to double's when passed to var-args;
- Infinity's and NaN's floating-point constant's were not been handled correctly;

------------------------------------------------------------------------
r1711 | v1ctor | 2005-09-09 13:01:50 -0700 (Fri, 09 Sep 2005) | 2 lines

fixed: single's are now converted to double's to follow the C ABI

------------------------------------------------------------------------
r1710 | v1ctor | 2005-09-09 12:59:48 -0700 (Fri, 09 Sep 2005) | 2 lines

cosmetic changes..

------------------------------------------------------------------------
r1709 | v1ctor | 2005-09-08 20:19:26 -0700 (Thu, 08 Sep 2005) | 6 lines

fixed:
- function overloading resolution was broken since the last commit;

changed:
- macro's stringize (#) can now be used with literal strings;

------------------------------------------------------------------------
r1708 | mjscod | 2005-09-08 15:18:00 -0700 (Thu, 08 Sep 2005) | 3 lines

* Fixed a problem with the thousands separator not printed under certain
  conditions (see new test case)

------------------------------------------------------------------------
r1707 | mjscod | 2005-09-08 11:46:29 -0700 (Thu, 08 Sep 2005) | 2 lines

* Updated changelog to comment a fix previously made by mjs

------------------------------------------------------------------------
r1706 | mjscod | 2005-09-06 12:07:17 -0700 (Tue, 06 Sep 2005) | 8 lines

* openhook.bi now automatically includes the user32 library
* fixed the key dependencies for BASIC functions
* moved console part of libfb_file_lineinp.c to libfb_con_lineinp.c
* added explaining FIXME to libfb_time_sleep.c
* moved open hook pointer to libfb_init.c
* removed init of SCRN: read function from libfb_file_input_file.c
* libfb_dev_file_readline.c now uses fgets directly

------------------------------------------------------------------------
r1705 | mjscod | 2005-09-06 02:14:54 -0700 (Tue, 06 Sep 2005) | 2 lines

* Sleep will now always clear the input buffer

------------------------------------------------------------------------
r1704 | i_am_drv | 2005-09-05 21:45:32 -0700 (Mon, 05 Sep 2005) | 2 lines

allow 40 or 80 column modes

------------------------------------------------------------------------
r1703 | v1ctor | 2005-09-05 20:48:02 -0700 (Mon, 05 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1702 | v1ctor | 2005-09-05 20:29:00 -0700 (Mon, 05 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1701 | v1ctor | 2005-09-05 20:28:02 -0700 (Mon, 05 Sep 2005) | 5 lines

fixed:
- address of overloaded functions can now be taken correctly;
- wrong overload type could be returned if pointers to UDT's were used as parameters and arguments;
- PASCAL functions couldn't be overloaded;

------------------------------------------------------------------------
r1700 | mjscod | 2005-09-05 13:24:25 -0700 (Mon, 05 Sep 2005) | 4 lines

* Restored the SLEEP / (LINE) INPUT behaviour
  the character entered during a SLEEP is now taken as the input to (LINE) INPUT
  ... BTW: This doesn't work with OPEN "CON" yet ... but I'm too lazy to fix it now

------------------------------------------------------------------------
r1699 | i_am_drv | 2005-09-04 20:59:13 -0700 (Sun, 04 Sep 2005) | 2 lines

MINI terrain rendering library header

------------------------------------------------------------------------
r1698 | i_am_drv | 2005-09-04 20:57:32 -0700 (Sun, 04 Sep 2005) | 2 lines

C wrapper for MINI terrain rendering library

------------------------------------------------------------------------
r1697 | mjscod | 2005-09-04 12:02:01 -0700 (Sun, 04 Sep 2005) | 3 lines

* More TAB() fixes ... TAB also works when using "normal" file I/O
  but will be translated to PAD when using stdout/stderr (cons, err, pipe)

------------------------------------------------------------------------
r1696 | mjscod | 2005-09-04 11:40:38 -0700 (Sun, 04 Sep 2005) | 6 lines

* Make SPC() and TAB() work with "CON" file
* Added missing FB_UNLOCK() to SPC()
* SPC() and TAB() now support non-SCRN devices
* Added FLUSH hook required by SPC/TAB when used on a CON file
  -> has no corresponding BASIC command yet

------------------------------------------------------------------------
r1695 | mjscod | 2005-09-04 03:13:32 -0700 (Sun, 04 Sep 2005) | 3 lines

* Fixed CR/LF handling for INPUT #n that caused a non-EOF where an EOF
  should be signaled

------------------------------------------------------------------------
r1694 | mjscod | 2005-09-03 07:58:59 -0700 (Sat, 03 Sep 2005) | 4 lines

* Enhanced makescript to allow multiple configuration files to achieve some
  kind of inheritance
* Added configuration file and script template for Installer-Script with sources

------------------------------------------------------------------------
r1693 | mjscod | 2005-09-03 02:49:43 -0700 (Sat, 03 Sep 2005) | 2 lines

* Forgot to delete fblogo.ico and start_shell.exe during uninstall

------------------------------------------------------------------------
r1692 | v1ctor | 2005-09-02 22:27:59 -0700 (Fri, 02 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1691 | v1ctor | 2005-09-02 22:27:45 -0700 (Fri, 02 Sep 2005) | 2 lines

fixed: UDT results were been allowed to be passed by reference to functions

------------------------------------------------------------------------
r1690 | v1ctor | 2005-09-02 20:59:31 -0700 (Fri, 02 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1689 | v1ctor | 2005-09-02 20:57:47 -0700 (Fri, 02 Sep 2005) | 2 lines

fixed: string comp wasn't working with chars above 127

------------------------------------------------------------------------
r1688 | v1ctor | 2005-09-02 20:48:55 -0700 (Fri, 02 Sep 2005) | 2 lines

first unicode string routines commit..

------------------------------------------------------------------------
r1687 | v1ctor | 2005-09-02 20:45:21 -0700 (Fri, 02 Sep 2005) | 2 lines

cosmetic changes..

------------------------------------------------------------------------
r1686 | v1ctor | 2005-09-02 20:43:31 -0700 (Fri, 02 Sep 2005) | 2 lines

first unicode string routines commit..

------------------------------------------------------------------------
r1685 | mjscod | 2005-09-02 15:07:44 -0700 (Fri, 02 Sep 2005) | 2 lines

* Added program to create NSIS installer script

------------------------------------------------------------------------
r1684 | mjscod | 2005-09-02 13:56:11 -0700 (Fri, 02 Sep 2005) | 3 lines

* Fixed some problems with size_t defined in both stddef.bi and regex.bi
* It should also fix the problem of wchar_t between stddef.bi and winnt.bi

------------------------------------------------------------------------
r1683 | mjscod | 2005-09-02 08:31:45 -0700 (Fri, 02 Sep 2005) | 4 lines

* Fixed the example for the regex-subexpressions
  basename didn't return the ".name" parts of the file name that aren't part of the
  extension

------------------------------------------------------------------------
r1682 | mjscod | 2005-09-02 01:14:57 -0700 (Fri, 02 Sep 2005) | 2 lines

* Nice example that shows how to deal with sub-expressions in regular expressions

------------------------------------------------------------------------
r1681 | v1ctor | 2005-09-01 23:40:41 -0700 (Thu, 01 Sep 2005) | 4 lines

fixed:
lex -- sign was inverted;
ast -- sign-extension from int to ulongint was giving false warnings;

------------------------------------------------------------------------
r1680 | v1ctor | 2005-09-01 23:18:17 -0700 (Thu, 01 Sep 2005) | 2 lines

changed: small speed up

------------------------------------------------------------------------
r1679 | v1ctor | 2005-09-01 22:02:15 -0700 (Thu, 01 Sep 2005) | 2 lines

first unicode string routines commit..

------------------------------------------------------------------------
r1678 | v1ctor | 2005-09-01 21:51:07 -0700 (Thu, 01 Sep 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1677 | v1ctor | 2005-09-01 21:25:36 -0700 (Thu, 01 Sep 2005) | 2 lines

first unicode string routines commit..

------------------------------------------------------------------------
r1676 | v1ctor | 2005-09-01 21:24:36 -0700 (Thu, 01 Sep 2005) | 2 lines

small change..

------------------------------------------------------------------------
r1675 | v1ctor | 2005-09-01 21:12:42 -0700 (Thu, 01 Sep 2005) | 2 lines

changed: removed unused table

------------------------------------------------------------------------
r1674 | v1ctor | 2005-09-01 20:47:21 -0700 (Thu, 01 Sep 2005) | 2 lines

changed: small speed up

------------------------------------------------------------------------
r1673 | lillo | 2005-09-01 15:22:17 -0700 (Thu, 01 Sep 2005) | 2 lines

Fixed bug when resizing terminal window; fb_hResize() was being called also by the bg thread (by calling fb_hGetCh()), and that caused conflicts

------------------------------------------------------------------------
r1672 | lillo | 2005-09-01 14:08:28 -0700 (Thu, 01 Sep 2005) | 2 lines

Another round of fixes related to conversions and signedness, hopefully final fixes...

------------------------------------------------------------------------
r1671 | lillo | 2005-09-01 12:47:03 -0700 (Thu, 01 Sep 2005) | 2 lines

Another small fix

------------------------------------------------------------------------
r1670 | mjscod | 2005-09-01 12:16:56 -0700 (Thu, 01 Sep 2005) | 2 lines

* Enhanced the ISR function to get a stack size

------------------------------------------------------------------------
r1669 | lillo | 2005-09-01 10:52:15 -0700 (Thu, 01 Sep 2005) | 2 lines

Restored implicit conversion checks on constant negation, now works as expected

------------------------------------------------------------------------
r1668 | lillo | 2005-09-01 10:04:38 -0700 (Thu, 01 Sep 2005) | 2 lines

Fixed non-decimal numbers signedness bug

------------------------------------------------------------------------
r1667 | mjscod | 2005-09-01 02:40:33 -0700 (Thu, 01 Sep 2005) | 3 lines

* Renamed the .def file for ntoskrnl to be compliant with other library names
  generated by genimplibs

------------------------------------------------------------------------
r1666 | mjscod | 2005-09-01 00:35:33 -0700 (Thu, 01 Sep 2005) | 2 lines

* Fix rounding error on optimized build ... I HATE FLOATING POINT

------------------------------------------------------------------------
r1665 | v1ctor | 2005-08-31 20:24:13 -0700 (Wed, 31 Aug 2005) | 2 lines

fixed: wrong type for wchar_t

------------------------------------------------------------------------
r1664 | lillo | 2005-08-31 17:40:22 -0700 (Wed, 31 Aug 2005) | 2 lines

Overhaul of the Linux console routines, now using termcap for wide terminal compatibility

------------------------------------------------------------------------
r1663 | mjscod | 2005-08-31 16:42:39 -0700 (Wed, 31 Aug 2005) | 2 lines

* Fixed OPENHOOK example ... LPT: and COM: checks didn't work

------------------------------------------------------------------------
r1662 | mjscod | 2005-08-31 14:21:24 -0700 (Wed, 31 Aug 2005) | 2 lines

* Fixed documentation

------------------------------------------------------------------------
r1661 | mjscod | 2005-08-31 14:15:21 -0700 (Wed, 31 Aug 2005) | 2 lines

* Updated the ISR API to support chaining of ISR handlers

------------------------------------------------------------------------
r1660 | mjscod | 2005-08-31 14:07:01 -0700 (Wed, 31 Aug 2005) | 2 lines

* Added a small example for ISR handling stuff using DOS/DJGPP

------------------------------------------------------------------------
r1659 | mjscod | 2005-08-31 14:04:30 -0700 (Wed, 31 Aug 2005) | 5 lines

* Modified makefile to use assembler-with-cpp
* Fixed *.s files by removing .arch (only valid for ALPHA processors)
  Also fixed some issues with # comments in libfb_str_ftoa_x86.s
* Added generic ISR-handling stuff for DOS

------------------------------------------------------------------------
r1658 | v1ctor | 2005-08-30 23:33:29 -0700 (Tue, 30 Aug 2005) | 2 lines

fixed: function result assignment when the proc name is used in overloaded functions;

------------------------------------------------------------------------
r1657 | v1ctor | 2005-08-30 23:06:02 -0700 (Tue, 30 Aug 2005) | 2 lines

fixed: getOrgName() should be used to get the struct name because the symbol-tb changes in the last updates;

------------------------------------------------------------------------
r1656 | v1ctor | 2005-08-30 21:07:13 -0700 (Tue, 30 Aug 2005) | 2 lines

changed: faster erase with a single lock

------------------------------------------------------------------------
r1655 | mjscod | 2005-08-30 14:28:39 -0700 (Tue, 30 Aug 2005) | 2 lines

* Returned x/y values were 1 too high

------------------------------------------------------------------------
r1654 | mjscod | 2005-08-30 09:52:36 -0700 (Tue, 30 Aug 2005) | 5 lines

* Updated changelog
* Updated threads so that pressing a key doesn't make the application skip
  over all following SLEEPs
* user32 will be added whenever input # is used

------------------------------------------------------------------------
r1653 | mjscod | 2005-08-30 02:29:24 -0700 (Tue, 30 Aug 2005) | 2 lines

* Forgot to remove some dumb debug code I used to find the deadlock

------------------------------------------------------------------------
r1652 | i_am_drv | 2005-08-29 23:19:52 -0700 (Mon, 29 Aug 2005) | 2 lines

farptr memset routines

------------------------------------------------------------------------
r1651 | v1ctor | 2005-08-29 21:02:38 -0700 (Mon, 29 Aug 2005) | 2 lines

fixed: string's dead-lock when in mt mode

------------------------------------------------------------------------
r1650 | v1ctor | 2005-08-29 20:51:02 -0700 (Mon, 29 Aug 2005) | 2 lines

fixed: string's dead-lock when in mt mode

------------------------------------------------------------------------
r1649 | v1ctor | 2005-08-29 20:37:21 -0700 (Mon, 29 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1648 | v1ctor | 2005-08-29 20:30:02 -0700 (Mon, 29 Aug 2005) | 2 lines

fixed: string's dead-lock when in mt mode

------------------------------------------------------------------------
r1647 | v1ctor | 2005-08-29 20:17:53 -0700 (Mon, 29 Aug 2005) | 2 lines

fixed: string's dead-lock when in mt mode

------------------------------------------------------------------------
r1646 | mjscod | 2005-08-29 16:30:57 -0700 (Mon, 29 Aug 2005) | 8 lines

* Fixed the LPT port assignment on Win32
* Added the feature to specify the printer name directly using
  OPEN LPT "LPT:printer_name" AS 1
* Updated open hook example
* Non-default printers not attached to LPTx: ports are now mapped to
  LPT ports >= 128
* OPEN LPT now automatically adds winspool library to linker

------------------------------------------------------------------------
r1645 | mjscod | 2005-08-29 14:41:56 -0700 (Mon, 29 Aug 2005) | 3 lines

* Fixed FORMAT$ with thousands separators before current number
* Updated test cases

------------------------------------------------------------------------
r1644 | mjscod | 2005-08-29 06:36:57 -0700 (Mon, 29 Aug 2005) | 2 lines

* Reverted my debug output stuff

------------------------------------------------------------------------
r1643 | mjscod | 2005-08-29 06:33:33 -0700 (Mon, 29 Aug 2005) | 6 lines

* Added OPEN COM
* Implemented serial port access for Win32
* Enhanced OPENHOOK example
* Modified stdio header to contain declarations of *nprintf functions
* WIDTH now uses -1 as default value

------------------------------------------------------------------------
r1642 | lillo | 2005-08-29 03:46:16 -0700 (Mon, 29 Aug 2005) | 2 lines

maintainance

------------------------------------------------------------------------
r1641 | mjscod | 2005-08-29 02:45:24 -0700 (Mon, 29 Aug 2005) | 2 lines

* Made many changes to allow driver compilation with MinGW

------------------------------------------------------------------------
r1640 | mjscod | 2005-08-29 02:39:29 -0700 (Mon, 29 Aug 2005) | 2 lines

* Added some helpful tools

------------------------------------------------------------------------
r1639 | lillo | 2005-08-29 01:14:42 -0700 (Mon, 29 Aug 2005) | 2 lines

Added the GFX_NO_SWITCH flag to SCREEN, preventing user from switching fullscreen/windowed via either maximize button either ALT-Enter

------------------------------------------------------------------------
r1638 | v1ctor | 2005-08-29 00:27:42 -0700 (Mon, 29 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1637 | v1ctor | 2005-08-29 00:17:57 -0700 (Mon, 29 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1636 | v1ctor | 2005-08-29 00:16:25 -0700 (Mon, 29 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1635 | v1ctor | 2005-08-29 00:09:12 -0700 (Mon, 29 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1634 | v1ctor | 2005-08-28 23:59:29 -0700 (Sun, 28 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1633 | mjscod | 2005-08-28 10:50:44 -0700 (Sun, 28 Aug 2005) | 3 lines

* Added ntoskrnl.def which produces libntoskrnl.a which is required to
  build the portio driver using GCC

------------------------------------------------------------------------
r1632 | v1ctor | 2005-08-27 11:08:07 -0700 (Sat, 27 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1631 | v1ctor | 2005-08-27 11:04:29 -0700 (Sat, 27 Aug 2005) | 2 lines

fixed: path shouldn't be stripped from the include file name;

------------------------------------------------------------------------
r1630 | v1ctor | 2005-08-27 11:02:28 -0700 (Sat, 27 Aug 2005) | 2 lines

new headers compatible with the original C ones

------------------------------------------------------------------------
r1629 | v1ctor | 2005-08-27 10:43:11 -0700 (Sat, 27 Aug 2005) | 2 lines

new headers compatible with the original C ones

------------------------------------------------------------------------
r1628 | v1ctor | 2005-08-27 10:38:22 -0700 (Sat, 27 Aug 2005) | 2 lines

new headers compatible with the original C ones

------------------------------------------------------------------------
r1627 | v1ctor | 2005-08-27 10:32:39 -0700 (Sat, 27 Aug 2005) | 2 lines

new headers compatible with the original C ones

------------------------------------------------------------------------
r1626 | v1ctor | 2005-08-27 10:31:38 -0700 (Sat, 27 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1625 | mjscod | 2005-08-27 00:32:40 -0700 (Sat, 27 Aug 2005) | 2 lines

* Updated header for CYGWIN

------------------------------------------------------------------------
r1624 | v1ctor | 2005-08-26 13:52:29 -0700 (Fri, 26 Aug 2005) | 2 lines

fixed: null's to white-space's conversion

------------------------------------------------------------------------
r1623 | mjscod | 2005-08-25 23:48:26 -0700 (Thu, 25 Aug 2005) | 3 lines

* Added vsnprintf to crt.bi
* Added sqlite3_vmprintf to sqlite3.bi

------------------------------------------------------------------------
r1622 | mjscod | 2005-08-25 23:31:37 -0700 (Thu, 25 Aug 2005) | 2 lines

* Updated changelog for getmouse<->inkey$ fix on Win32

------------------------------------------------------------------------
r1621 | v1ctor | 2005-08-25 22:05:32 -0700 (Thu, 25 Aug 2005) | 2 lines

"byval as string" to "byval as zstring ptr" update

------------------------------------------------------------------------
r1620 | v1ctor | 2005-08-25 21:42:51 -0700 (Thu, 25 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1619 | v1ctor | 2005-08-25 21:39:20 -0700 (Thu, 25 Aug 2005) | 15 lines

added:
- SCOPE..END SCOPE blocks;
- the __FB_MAIN__ define;
- level of indirection checking when assigning or passing pointers;

changed:
- function names will only be preserved ALIAS is present, not LIB only;
- IF THEN followed by a stmt separator (':') will be seen as a multi-line IF to support complex macros;
- non-shared or public variables declared on module-level will be declared as Local to GDB;

fixed:
- passing a "byval as string" arg to a "byval as zstring ptr" one was creating a double dereference;
- loading integer constants to longint vars was not doing the right sign-extension;
- loading -1 to longint's was giving a warning;

------------------------------------------------------------------------
r1618 | mjscod | 2005-08-25 14:57:37 -0700 (Thu, 25 Aug 2005) | 2 lines

* Forgot to add user32.dll for the fb_ConsoleInput function

------------------------------------------------------------------------
r1617 | mjscod | 2005-08-25 11:03:27 -0700 (Thu, 25 Aug 2005) | 2 lines

* Removed the redefinitions of TRUE and FALSE

------------------------------------------------------------------------
r1616 | mjscod | 2005-08-24 16:00:11 -0700 (Wed, 24 Aug 2005) | 4 lines

* Fixed a conflict with TRUE from fb_gfx.h
* Format can now output any characters that aren't required to output
  the specific kind of information (number or date/time)

------------------------------------------------------------------------
r1615 | mjscod | 2005-08-23 19:00:14 -0700 (Tue, 23 Aug 2005) | 3 lines

* Fixed the new Win32 input using the Win32 API directly
  Now requires user32.dll - return codes are now (AFAIK) the same as under DOS

------------------------------------------------------------------------
r1614 | mjscod | 2005-08-23 14:39:40 -0700 (Tue, 23 Aug 2005) | 5 lines

* Implemented enhanced TRIM$() syntax:
  - TRIM(str, pattern)
  - TRIM(str, ANY pattern)
* Added test-cases for TRIM

------------------------------------------------------------------------
r1613 | mjscod | 2005-08-23 13:48:37 -0700 (Tue, 23 Aug 2005) | 3 lines

* Fixed SIGSEGV for GET #n,,s$ when s$=""
* Added comment about the weird RTRIM behaviour regarding NULs

------------------------------------------------------------------------
r1612 | mjscod | 2005-08-23 12:20:40 -0700 (Tue, 23 Aug 2005) | 5 lines

* Always open printer in BINARY mode on DOS and Win32
* Make all LPRINT's using a CR+LF for new-line
* Extract the file extension correctly - fixes SIGSEGV when point was last
  character in file name

------------------------------------------------------------------------
r1611 | mjscod | 2005-08-23 08:08:54 -0700 (Tue, 23 Aug 2005) | 3 lines

* The last change removed the wrong part of the console window position test ...
   now the console window should be restored correctly. Sorry about that ...

------------------------------------------------------------------------
r1610 | mjscod | 2005-08-23 06:36:35 -0700 (Tue, 23 Aug 2005) | 3 lines

* Fixed character set conversion for Win32 in GFX mode (uses CP437)
* Added localized german month and weekday names for DOS

------------------------------------------------------------------------
r1609 | lillo | 2005-08-22 04:43:32 -0700 (Mon, 22 Aug 2005) | 4 lines

- Added support for rxvt terminals under Linux
- Stability fixes related to escape sequences in Linux rtlib
- Profiling now also works for multithreaded apps

------------------------------------------------------------------------
r1608 | v1ctor | 2005-08-21 21:37:49 -0700 (Sun, 21 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1607 | v1ctor | 2005-08-21 15:38:52 -0700 (Sun, 21 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1606 | v1ctor | 2005-08-21 15:36:37 -0700 (Sun, 21 Aug 2005) | 2 lines

new headers compatible with the original C ones

------------------------------------------------------------------------
r1605 | v1ctor | 2005-08-21 14:51:12 -0700 (Sun, 21 Aug 2005) | 2 lines

new headers compatible with the original C ones

------------------------------------------------------------------------
r1604 | v1ctor | 2005-08-21 14:33:59 -0700 (Sun, 21 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1603 | v1ctor | 2005-08-21 13:45:47 -0700 (Sun, 21 Aug 2005) | 2 lines

new headers compatible with the original C ones

------------------------------------------------------------------------
r1602 | v1ctor | 2005-08-21 12:56:30 -0700 (Sun, 21 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1601 | v1ctor | 2005-08-21 12:55:27 -0700 (Sun, 21 Aug 2005) | 2 lines

new headers compatible with the original C ones

------------------------------------------------------------------------
r1600 | mjscod | 2005-08-21 09:09:32 -0700 (Sun, 21 Aug 2005) | 2 lines

* Just added a small comment to libfb_str_format.c about the source itself ...

------------------------------------------------------------------------
r1599 | mjscod | 2005-08-21 06:21:29 -0700 (Sun, 21 Aug 2005) | 7 lines

* Enhanced genimplibs to handle some useful arguments:
  -a (or --all) will cause the creation of all optional libraries
  -f (or --force) will create all libraries - and overwrite all existing libraries
  -p win32|cygwin selects the platform for which it should create the
      libraries
* Added install-script for cygwin

------------------------------------------------------------------------
r1598 | v1ctor | 2005-08-20 21:57:55 -0700 (Sat, 20 Aug 2005) | 2 lines

new headers compatible with the original C ones

------------------------------------------------------------------------
r1597 | v1ctor | 2005-08-20 21:45:33 -0700 (Sat, 20 Aug 2005) | 2 lines

new headers compatible with the original C ones

------------------------------------------------------------------------
r1596 | v1ctor | 2005-08-20 21:26:45 -0700 (Sat, 20 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1595 | v1ctor | 2005-08-20 21:19:11 -0700 (Sat, 20 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1594 | v1ctor | 2005-08-20 21:04:16 -0700 (Sat, 20 Aug 2005) | 2 lines

new headers compatible with the original C ones

------------------------------------------------------------------------
r1593 | v1ctor | 2005-08-20 20:51:14 -0700 (Sat, 20 Aug 2005) | 2 lines

replaced by new headers compatible with the original C ones

------------------------------------------------------------------------
r1592 | v1ctor | 2005-08-20 14:33:22 -0700 (Sat, 20 Aug 2005) | 4 lines

changed:
- dup definitions of EXTERN's are now allowed if they have the same types;
- anonymous UDT's are now allowed with unlimited nesting;

------------------------------------------------------------------------
r1591 | v1ctor | 2005-08-19 20:26:12 -0700 (Fri, 19 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1590 | v1ctor | 2005-08-19 20:23:41 -0700 (Fri, 19 Aug 2005) | 2 lines

fixed: using "END somefunction()" with profiling on would call prof_end() before the program ending

------------------------------------------------------------------------
r1589 | v1ctor | 2005-08-18 23:01:02 -0700 (Thu, 18 Aug 2005) | 4 lines

changed:
__main() will be called before init(), if target exe/dll is set to win32;
callCTORS() will be called after init() - moved to a different source file;

------------------------------------------------------------------------
r1588 | v1ctor | 2005-08-18 22:46:00 -0700 (Thu, 18 Aug 2005) | 2 lines

changed: __main() will be called after init(), if target exe/dll is set to win32

------------------------------------------------------------------------
r1587 | v1ctor | 2005-08-18 22:43:15 -0700 (Thu, 18 Aug 2005) | 2 lines

changed: removed __main() out of hinit()

------------------------------------------------------------------------
r1586 | v1ctor | 2005-08-18 21:34:39 -0700 (Thu, 18 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1585 | v1ctor | 2005-08-18 21:33:58 -0700 (Thu, 18 Aug 2005) | 2 lines

fixed: loadIDX was using the dtype from the var node, not the one from main node, causing errors with the UDT <-> function assignments when result is returned in registers

------------------------------------------------------------------------
r1584 | v1ctor | 2005-08-18 21:31:28 -0700 (Thu, 18 Aug 2005) | 2 lines

changed: dtype must now be passed to newIDX

------------------------------------------------------------------------
r1583 | v1ctor | 2005-08-18 21:29:12 -0700 (Thu, 18 Aug 2005) | 2 lines

fixed: STOREI2L was calling itself, instead of STOREI2I, causing a stack fault

------------------------------------------------------------------------
r1582 | v1ctor | 2005-08-18 20:22:04 -0700 (Thu, 18 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1581 | v1ctor | 2005-08-18 20:20:35 -0700 (Thu, 18 Aug 2005) | 2 lines

fixed: UDT padding and fields alignment now fully follows the GCC 3.x ABI

------------------------------------------------------------------------
r1580 | mjscod | 2005-08-18 11:43:10 -0700 (Thu, 18 Aug 2005) | 2 lines

* Fixed function declarations (missing AS INTEGER)

------------------------------------------------------------------------
r1579 | mjscod | 2005-08-18 11:13:36 -0700 (Thu, 18 Aug 2005) | 2 lines

* Activated lua_upvalueindex because it's very useful for C closures

------------------------------------------------------------------------
r1578 | v1ctor | 2005-08-17 20:35:32 -0700 (Wed, 17 Aug 2005) | 3 lines

fixed: unsigned literal numbers conversion to types < sizeof(int);
added: fixed-len, var-len and z-string params will be handled automatically when passed to zstring ptr args;

------------------------------------------------------------------------
r1577 | v1ctor | 2005-08-17 20:33:28 -0700 (Wed, 17 Aug 2005) | 2 lines

fixed: UDT padding when non-default alignment values were given

------------------------------------------------------------------------
r1576 | mjscod | 2005-08-17 12:13:26 -0700 (Wed, 17 Aug 2005) | 3 lines

* Must add additional ../ to path because after the move it's path is
  now one level deeper than before

------------------------------------------------------------------------
r1575 | mjscod | 2005-08-17 12:07:33 -0700 (Wed, 17 Aug 2005) | 2 lines

* Moved TinyPTC to contrib/ folder

------------------------------------------------------------------------
r1574 | mjscod | 2005-08-17 01:00:26 -0700 (Wed, 17 Aug 2005) | 2 lines

* Fixed PIPE: handling

------------------------------------------------------------------------
r1573 | v1ctor | 2005-08-16 22:22:00 -0700 (Tue, 16 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1572 | v1ctor | 2005-08-16 21:31:49 -0700 (Tue, 16 Aug 2005) | 2 lines

changed: using strassign() instead

------------------------------------------------------------------------
r1571 | mjscod | 2005-08-16 15:04:49 -0700 (Tue, 16 Aug 2005) | 4 lines

* Fixed character set conversion functions:
  - FB_STRUNLOCK was missing
  - Forgot to release temporary string

------------------------------------------------------------------------
r1570 | mjscod | 2005-08-16 14:50:31 -0700 (Tue, 16 Aug 2005) | 3 lines

* DATA can now be used in functions too
* Forgot to update changelog

------------------------------------------------------------------------
r1569 | mjscod | 2005-08-16 14:03:43 -0700 (Tue, 16 Aug 2005) | 6 lines

* Fixed the include file handling:
  - INCLUDE ONCE wasn't respecting the path
  - An included file was remembered in the environment without the path
    which caused unfound header files which were in the same path
* Fixed path- / filename extraction with mixed types of slashes

------------------------------------------------------------------------
r1568 | mjscod | 2005-08-16 12:54:36 -0700 (Tue, 16 Aug 2005) | 4 lines

* Fixed a string descriptor leak in the function to parse a date
* Converted all internal weekday name functions to return a FBSTRING*
* Converted returned weekday name to console codepage

------------------------------------------------------------------------
r1567 | mjscod | 2005-08-16 02:47:24 -0700 (Tue, 16 Aug 2005) | 3 lines

* Fix for the open hook when a string (with temp. descr. and static data) was
  passed as a file name to OPEN and the hook changes the file name.

------------------------------------------------------------------------
r1566 | v1ctor | 2005-08-16 01:02:39 -0700 (Tue, 16 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1565 | v1ctor | 2005-08-16 00:58:57 -0700 (Tue, 16 Aug 2005) | 2 lines

fixed: value returned was since the app started

------------------------------------------------------------------------
r1564 | v1ctor | 2005-08-16 00:46:37 -0700 (Tue, 16 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1563 | mjscod | 2005-08-16 00:33:32 -0700 (Tue, 16 Aug 2005) | 2 lines

* Fix for the OPEN HOOK ( temporary handle wasn't initialized )

------------------------------------------------------------------------
r1562 | mjscod | 2005-08-16 00:03:28 -0700 (Tue, 16 Aug 2005) | 4 lines

* Make OPEN hookable by BASIC applications. See example openhook.bas
  This feature is IMHO absolutely required to create a QB-compatible OPEN
  function for people that care about compatibility and not about code bloat.

------------------------------------------------------------------------
r1561 | v1ctor | 2005-08-15 22:05:23 -0700 (Mon, 15 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1560 | v1ctor | 2005-08-15 21:58:33 -0700 (Mon, 15 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1559 | v1ctor | 2005-08-15 21:24:25 -0700 (Mon, 15 Aug 2005) | 2 lines

changed: vsync off

------------------------------------------------------------------------
r1558 | mjscod | 2005-08-15 12:56:30 -0700 (Mon, 15 Aug 2005) | 3 lines

* Fixed an error in the macro to create an extended key value
* Simplyfied Win32 input code

------------------------------------------------------------------------
r1557 | mjscod | 2005-08-15 01:53:09 -0700 (Mon, 15 Aug 2005) | 3 lines

* Blind fix for ConsoleGetSize for Linux (lillo, please test) to avoid endless
  recursion.

------------------------------------------------------------------------
r1556 | mjscod | 2005-08-15 01:49:12 -0700 (Mon, 15 Aug 2005) | 2 lines

* Fixed compiler warning

------------------------------------------------------------------------
r1555 | mjscod | 2005-08-15 01:44:13 -0700 (Mon, 15 Aug 2005) | 2 lines

* More CYGWIN fixes

------------------------------------------------------------------------
r1554 | mjscod | 2005-08-15 01:38:14 -0700 (Mon, 15 Aug 2005) | 2 lines

* Ensure that object path exists

------------------------------------------------------------------------
r1553 | mjscod | 2005-08-15 01:34:11 -0700 (Mon, 15 Aug 2005) | 2 lines

* Forgot to remove CONIO header

------------------------------------------------------------------------
r1552 | mjscod | 2005-08-15 01:28:07 -0700 (Mon, 15 Aug 2005) | 8 lines

* Now uses a circular buffer for storing key events for Win32
* The platform-dependent implementation of MONTHNAME now returns
  a FBSTRING
* The MONTHNAME value is now returned in the current console's codepage
  for Win32
* Make tempdescz's argument CONST
* TODO: Convert WEEKDAYNAME too

------------------------------------------------------------------------
r1551 | v1ctor | 2005-08-14 12:33:22 -0700 (Sun, 14 Aug 2005) | 2 lines

changed: str() now converts constants at compile-time to better support the & operator

------------------------------------------------------------------------
r1550 | mjscod | 2005-08-14 12:28:38 -0700 (Sun, 14 Aug 2005) | 7 lines

* Added some more test cases
* Fixed two bugs with thousands-separators:
  - when more than one was required, the output was wrong
  - when the thousands separators are used to divide the number by
    10 ^ (3*n) then the output was wrong too because these "separators"
    must not be treated as thousands separators.

------------------------------------------------------------------------
r1549 | mjscod | 2005-08-14 11:17:15 -0700 (Sun, 14 Aug 2005) | 9 lines

* Added more tests for Format$
* Specified gdi32 as additional library to include with tinyptc
* Fixed a regression with PRINT "."; executed more often than the number of
  columns of a Win32 console buffer in the last line.
  The PRINT in the right bottom corner caused a "no-scroll" print which doesn't
  update the cursor position. This caused the next PRINT to be printed over the
  previous PRINT.
  Found during the Format$ test case update

------------------------------------------------------------------------
r1548 | mjscod | 2005-08-14 09:06:33 -0700 (Sun, 14 Aug 2005) | 2 lines

* Fixed line-endings ... sorry

------------------------------------------------------------------------
r1547 | mjscod | 2005-08-14 08:46:40 -0700 (Sun, 14 Aug 2005) | 4 lines

* Enhanced makefile to accept WINDOWED=1 or 0 to build the windowed library
  (if supported)
* Renamed libfb_gfx_inkey to libfb_ptc_inkey

------------------------------------------------------------------------
r1546 | mjscod | 2005-08-14 05:26:43 -0700 (Sun, 14 Aug 2005) | 4 lines

* Added TinyPTC 0.8 (Win32) to the repository.
* Added makefile
* Modifed TinyPTC (win32, ddraw) to use hooks to manage keyboard input

------------------------------------------------------------------------
r1545 | v1ctor | 2005-08-13 12:19:21 -0700 (Sat, 13 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1544 | mjscod | 2005-08-13 09:37:35 -0700 (Sat, 13 Aug 2005) | 2 lines

* Made console window restore a little bit more efficient

------------------------------------------------------------------------
r1543 | mjscod | 2005-08-11 22:42:51 -0700 (Thu, 11 Aug 2005) | 2 lines

* Fixed the header to declare data only when it's really available

------------------------------------------------------------------------
r1542 | mjscod | 2005-08-11 22:40:55 -0700 (Thu, 11 Aug 2005) | 4 lines

* Win32: Restore calls to restore original console window because the slow-down
  caused by the current implementation is not significant any more.
  This should really fix some messy application outputs ...

------------------------------------------------------------------------
r1541 | v1ctor | 2005-08-11 21:52:37 -0700 (Thu, 11 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1540 | v1ctor | 2005-08-11 21:45:28 -0700 (Thu, 11 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1539 | v1ctor | 2005-08-11 21:35:40 -0700 (Thu, 11 Aug 2005) | 2 lines

cosmetic changes

------------------------------------------------------------------------
r1538 | mjscod | 2005-08-11 12:58:57 -0700 (Thu, 11 Aug 2005) | 2 lines

* Fixed internationalization support for DOS - structure alignment was wrong.

------------------------------------------------------------------------
r1537 | mjscod | 2005-08-11 12:32:40 -0700 (Thu, 11 Aug 2005) | 2 lines

* Fixed an endless loop for DOS console output

------------------------------------------------------------------------
r1536 | mjscod | 2005-08-11 12:17:33 -0700 (Thu, 11 Aug 2005) | 3 lines

* Fixed printbuffer for DOS (compilation failed)
* Updated internationalization stuff for DOS

------------------------------------------------------------------------
r1535 | lillo | 2005-08-11 07:47:15 -0700 (Thu, 11 Aug 2005) | 2 lines

Made advapi32 to be linked in only when needed

------------------------------------------------------------------------
r1534 | lillo | 2005-08-11 06:44:44 -0700 (Thu, 11 Aug 2005) | 2 lines

small comment

------------------------------------------------------------------------
r1533 | lillo | 2005-08-11 06:24:17 -0700 (Thu, 11 Aug 2005) | 2 lines

Added the & string concatenation operator, automatically converting types to string if needed

------------------------------------------------------------------------
r1532 | lillo | 2005-08-11 02:28:33 -0700 (Thu, 11 Aug 2005) | 4 lines

- Made Linux fbc to work with relative paths to bin/inc/lib dirs like other ports
- Rewrote install.sh script to allow installing/uninstalling in custom directories
- Made Linux exec/chain/run to automatically convert backslashes to slashes

------------------------------------------------------------------------
r1531 | mjscod | 2005-08-11 01:58:40 -0700 (Thu, 11 Aug 2005) | 2 lines

* Updated test cases - two FIXME's aren't required any more

------------------------------------------------------------------------
r1530 | v1ctor | 2005-08-11 01:13:21 -0700 (Thu, 11 Aug 2005) | 2 lines

fixed: win32-linux cross-compiling

------------------------------------------------------------------------
r1529 | v1ctor | 2005-08-11 01:11:36 -0700 (Thu, 11 Aug 2005) | 2 lines

fixed: paths under Windows

------------------------------------------------------------------------
r1528 | v1ctor | 2005-08-10 21:30:32 -0700 (Wed, 10 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1527 | v1ctor | 2005-08-10 21:25:31 -0700 (Wed, 10 Aug 2005) | 2 lines

minor change

------------------------------------------------------------------------
r1526 | v1ctor | 2005-08-10 21:24:46 -0700 (Wed, 10 Aug 2005) | 2 lines

changed: fb_ prefix removed from prototypes

------------------------------------------------------------------------
r1525 | v1ctor | 2005-08-10 21:20:18 -0700 (Wed, 10 Aug 2005) | 2 lines

changed: assert() will be only emitted if DEBUG is set

------------------------------------------------------------------------
r1524 | v1ctor | 2005-08-10 20:59:55 -0700 (Wed, 10 Aug 2005) | 2 lines

minor changes

------------------------------------------------------------------------
r1523 | v1ctor | 2005-08-10 20:56:13 -0700 (Wed, 10 Aug 2005) | 2 lines

minor change

------------------------------------------------------------------------
r1522 | lillo | 2005-08-10 18:14:00 -0700 (Wed, 10 Aug 2005) | 2 lines

Made BLOAD to load 32bit BMPs correctly

------------------------------------------------------------------------
r1521 | lillo | 2005-08-10 17:15:02 -0700 (Wed, 10 Aug 2005) | 2 lines

Small fix for (u)longint implicit conversions

------------------------------------------------------------------------
r1520 | mjscod | 2005-08-10 15:38:27 -0700 (Wed, 10 Aug 2005) | 3 lines

* Updated makefile for CYGWIN
* Updated test case for numbers

------------------------------------------------------------------------
r1519 | mjscod | 2005-08-10 14:58:42 -0700 (Wed, 10 Aug 2005) | 2 lines

* Fixed compiler warning

------------------------------------------------------------------------
r1518 | mjscod | 2005-08-10 14:56:31 -0700 (Wed, 10 Aug 2005) | 4 lines

* Fixed a problem with -0.0 in fb_StrFormat causing the test to fail
* Added numbers test correctly to Makefile
* Fixed text_in.bas to allow ignoring the EOF character

------------------------------------------------------------------------
r1517 | mjscod | 2005-08-10 14:28:49 -0700 (Wed, 10 Aug 2005) | 5 lines

* Fixed line-endings for conmouse.bas
* Changed calling-convention to CDECL for fb_CpuDetect in rtl.bas
* Removed WORD PTR for fnstcw and its counterpart in libfb_str_ftoa_x86.s
* Removed a warning in libfb_sys_exec.c

------------------------------------------------------------------------
r1516 | lillo | 2005-08-10 10:18:09 -0700 (Wed, 10 Aug 2005) | 2 lines

Added check for host CPU at program startup if error checking is enabled

------------------------------------------------------------------------
r1515 | lillo | 2005-08-10 09:07:22 -0700 (Wed, 10 Aug 2005) | 2 lines

Added 386 CPU detection

------------------------------------------------------------------------
r1514 | lillo | 2005-08-10 08:52:52 -0700 (Wed, 10 Aug 2005) | 2 lines

Removed fb_hHasMMX(), now using more flexible fb_CpuDetect() from rtlib

------------------------------------------------------------------------
r1513 | lillo | 2005-08-10 08:51:28 -0700 (Wed, 10 Aug 2005) | 2 lines

enabled fb_CpuDetect()

------------------------------------------------------------------------
r1512 | lillo | 2005-08-10 08:22:41 -0700 (Wed, 10 Aug 2005) | 2 lines

Added numerical test

------------------------------------------------------------------------
r1511 | lillo | 2005-08-10 08:18:37 -0700 (Wed, 10 Aug 2005) | 6 lines

- Fixed some small bugs
- Implemented checks for too big numbers at lexer
- Made lexer more numerical type-aware, signedness included
- Implicit conversion checks are now much more strict, checking signedness too
- Fixed some implicit conversions in the compiler code reported by the new strict rules

------------------------------------------------------------------------
r1510 | mjscod | 2005-08-10 03:18:30 -0700 (Wed, 10 Aug 2005) | 3 lines

* Updated text input test for CYGWIN
* Fixed GETMOUSE to work correctly without INKEY$ between GETMOUSE calls

------------------------------------------------------------------------
r1509 | mjscod | 2005-08-10 01:38:30 -0700 (Wed, 10 Aug 2005) | 2 lines

* Forgot to remove "include conio.h"

------------------------------------------------------------------------
r1508 | mjscod | 2005-08-10 01:35:46 -0700 (Wed, 10 Aug 2005) | 7 lines

* More CYGWIN patches
* CYGWIN applications can now be built ( I use ar/as/ld from MinGW )
* Fixed the problem of the not-well responding keyboard when using the mouse
  (see new example conmouse.bas)
* Windows applications now read the input using the Windows console API instead
  of conio (required to fix the conmouse.bas example)

------------------------------------------------------------------------
r1507 | mjscod | 2005-08-10 00:12:58 -0700 (Wed, 10 Aug 2005) | 4 lines

* Fixed PrintBuff to print the lines (splitted by newline) in a separate function
  (reduces stack size usage).
* Added documentation about the LFN to SFN stuff under DOS

------------------------------------------------------------------------
r1506 | mjscod | 2005-08-10 00:11:22 -0700 (Wed, 10 Aug 2005) | 3 lines

* Updated documentation about strings
* Fixed the FB_TEMPSTRBIT stuff I messed up with the previous check-in

------------------------------------------------------------------------
r1505 | mjscod | 2005-08-09 07:06:58 -0700 (Tue, 09 Aug 2005) | 2 lines

* CYGWIN support isn't finished yet ... needs more testing ... sorry.

------------------------------------------------------------------------
r1504 | mjscod | 2005-08-09 06:48:37 -0700 (Tue, 09 Aug 2005) | 2 lines

* Fixed line endings

------------------------------------------------------------------------
r1503 | mjscod | 2005-08-09 06:47:46 -0700 (Tue, 09 Aug 2005) | 2 lines

* Added CYGWIN support (as a special case of Win32)

------------------------------------------------------------------------
r1502 | mjscod | 2005-08-09 06:44:29 -0700 (Tue, 09 Aug 2005) | 2 lines

* Removed CYGWIN hacks

------------------------------------------------------------------------
r1501 | mjscod | 2005-08-09 06:42:39 -0700 (Tue, 09 Aug 2005) | 2 lines

* Updated I18n support

------------------------------------------------------------------------
r1500 | mjscod | 2005-08-09 03:52:30 -0700 (Tue, 09 Aug 2005) | 6 lines

* Updated test cases
* Fixed bug under DOS where printing a constant string destroys this string
* Added some header files for libfbx
* Added RTLIBX -> libfbx and libfbxmt
  Contains all the date/time/internationalization/enhanced string stuff

------------------------------------------------------------------------
r1499 | mjscod | 2005-08-09 02:40:01 -0700 (Tue, 09 Aug 2005) | 10 lines

* Removed all date/time stuff from RTLIB
* Simplyfied the temporary string allocation a lot
  (fb_hStrAllocTemp can now allocate a handle itself)
* Modified all sources to use the new simplyfied string allocation
* Added some assertions to the source where there was no
  out-of-memory error handling for temporary string allocation
* Fixed LTRIM/RTRIM ANY + the extended variant
  (forgot to delete temporary pattern)
* Added __FB_MT__

------------------------------------------------------------------------
r1498 | mjscod | 2005-08-08 17:29:57 -0700 (Mon, 08 Aug 2005) | 7 lines

* Added fb_hStrSetLength to set the string length (without reallocation) while
  preserving any string descriptor flags
* Added query of localized date format for Win32 and Linux
* Added query of localized month / weekday names for Win32 and Linux
* Fixed some multithreading bugs (functions to query date format,
  month- and weekday names must be guarded by global RTL lock)

------------------------------------------------------------------------
r1497 | lillo | 2005-08-07 07:02:27 -0700 (Sun, 07 Aug 2005) | 2 lines

A couple of linux console fixes

------------------------------------------------------------------------
r1496 | mjscod | 2005-08-07 04:47:33 -0700 (Sun, 07 Aug 2005) | 2 lines

* Updated DATEADD test cases

------------------------------------------------------------------------
r1495 | mjscod | 2005-08-07 04:43:47 -0700 (Sun, 07 Aug 2005) | 4 lines

* Added DATEDIFF
* Added DATEPART/DATEADD/DATEDIFF test cases
* Fixed week handling stuff

------------------------------------------------------------------------
r1494 | v1ctor | 2005-08-06 22:26:42 -0700 (Sat, 06 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1493 | v1ctor | 2005-08-06 22:26:39 -0700 (Sat, 06 Aug 2005) | 2 lines

added: math overflow check for literal numbers passed as parameter

------------------------------------------------------------------------
r1492 | v1ctor | 2005-08-06 22:19:03 -0700 (Sat, 06 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1491 | mjscod | 2005-08-06 20:17:16 -0700 (Sat, 06 Aug 2005) | 3 lines

* Change constant names (for date functions) to a more suitable name
  (i.e. vb* -> fb*)

------------------------------------------------------------------------
r1490 | mjscod | 2005-08-06 19:56:12 -0700 (Sat, 06 Aug 2005) | 2 lines

* Fixed DATEADD for dates around 1899-12-30

------------------------------------------------------------------------
r1489 | mjscod | 2005-08-06 19:53:59 -0700 (Sat, 06 Aug 2005) | 2 lines

* Sorry, forgot to add WEEKDAYNAME implementation

------------------------------------------------------------------------
r1488 | mjscod | 2005-08-06 19:39:21 -0700 (Sat, 06 Aug 2005) | 6 lines

* Fixed DATEVALUE (failed on short dates)
* Added MONTHNAME
* Added WEEKDAYNAME
* Added DATEADD
* Some source cleanup

------------------------------------------------------------------------
r1487 | v1ctor | 2005-08-06 12:52:02 -0700 (Sat, 06 Aug 2005) | 2 lines

fixed: module name was not been checked for invalid chars

------------------------------------------------------------------------
r1486 | mjscod | 2005-08-06 03:54:51 -0700 (Sat, 06 Aug 2005) | 2 lines

* Updated keyword list

------------------------------------------------------------------------
r1485 | mjscod | 2005-08-06 03:47:54 -0700 (Sat, 06 Aug 2005) | 8 lines

* Make access to time/localtime guarded by FB_LOCK because they might not be
  thread-safe
* Added NOW
* YEAR, MONTH, DAY now require a DOUBLE argument
  Required for "correct" rounding to INTEGER
* Added HOUR/MINUTE/SECOND VB quirk (when fix(serial_time)=0)
* Updated test case

------------------------------------------------------------------------
r1484 | mjscod | 2005-08-06 02:44:17 -0700 (Sat, 06 Aug 2005) | 4 lines

* Added TIMEVALUE
* Made DATEVALUE more flexible (accepts times too but doesn't return them - just like VBDOS)
* Updated test cases

------------------------------------------------------------------------
r1483 | mjscod | 2005-08-06 01:41:23 -0700 (Sat, 06 Aug 2005) | 3 lines

* Fix for fb_DateParse (did always fail because the test for WS at end of string
  was done at the start of string)

------------------------------------------------------------------------
r1482 | mjscod | 2005-08-06 01:37:10 -0700 (Sat, 06 Aug 2005) | 7 lines

* Added QB quirk for DIM after COMMON
* Added DATEVALUE
* Added YEAR, MONTH, DAY, WEEKDAY
* Added TIMESERIAL
* Added HOUR, MINUTE, SECOND
* Added testcases for date/time functions

------------------------------------------------------------------------
r1481 | v1ctor | 2005-08-05 23:23:40 -0700 (Fri, 05 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1480 | v1ctor | 2005-08-05 22:59:17 -0700 (Fri, 05 Aug 2005) | 2 lines

bad newlines

------------------------------------------------------------------------
r1479 | mjscod | 2005-08-05 17:47:44 -0700 (Fri, 05 Aug 2005) | 6 lines

* Added DATEVALUE
* Added native functions to query month names and the date format
* Use macro for CHAR to INT conversion
* Updated test case for DATEVALUE
* Fixed inline assembler code for special case with length==0

------------------------------------------------------------------------
r1478 | mjscod | 2005-08-05 15:00:16 -0700 (Fri, 05 Aug 2005) | 3 lines

* Added DATESERIAL
* Fixed line-endings of files created by me

------------------------------------------------------------------------
r1477 | mjscod | 2005-08-05 13:16:48 -0700 (Fri, 05 Aug 2005) | 4 lines

* Added the enhanced form of LTRIM/RTRIM, see test cases for example
* Fixed FB_MEMCMP to get the same arguments as memcmp
* Fixed call of FB_MEMCMP in libfb_str_comp.c

------------------------------------------------------------------------
r1476 | mjscod | 2005-08-05 08:03:47 -0700 (Fri, 05 Aug 2005) | 7 lines

* Implemented INSTR ANY
* Implemented LTRIM ANY
* Implemented RTRIM ANY
* Moved most X86 specific ASM code to fb_x86.h
* Moved portable functions (counterpart of X86 specific ASM code) to fb_port.h
* Added a missing const to 2nd parameter of hStrCopy

------------------------------------------------------------------------
r1475 | mjscod | 2005-08-05 03:58:02 -0700 (Fri, 05 Aug 2005) | 2 lines

* Reverted all changes to __dpmi_simulate_real_mode_interrupt

------------------------------------------------------------------------
r1474 | v1ctor | 2005-08-05 00:37:26 -0700 (Fri, 05 Aug 2005) | 2 lines

added: overflow checking in assignaments

------------------------------------------------------------------------
r1473 | v1ctor | 2005-08-04 23:19:27 -0700 (Thu, 04 Aug 2005) | 2 lines

changed: using callbacks to add user32

------------------------------------------------------------------------
r1472 | v1ctor | 2005-08-04 21:00:58 -0700 (Thu, 04 Aug 2005) | 2 lines

chr$(0) cannot be part of strings

------------------------------------------------------------------------
r1471 | v1ctor | 2005-08-04 20:46:54 -0700 (Thu, 04 Aug 2005) | 2 lines

fixed: unsigned word multiplication when EAX is in use

------------------------------------------------------------------------
r1470 | mjscod | 2005-08-04 13:53:53 -0700 (Thu, 04 Aug 2005) | 2 lines

* Fix for the FIXSTR NUL fix (sorry)

------------------------------------------------------------------------
r1469 | mjscod | 2005-08-04 13:16:41 -0700 (Thu, 04 Aug 2005) | 4 lines

* Fixed assignment to fix-string where a sub-string was "\0"
  DIM St1 AS STRING * 10: St1 = "Ciao" + CHR$(0) + "Test"
  ... results in CHR$(0) missing in the fixed-length-string

------------------------------------------------------------------------
r1468 | v1ctor | 2005-08-04 12:57:54 -0700 (Thu, 04 Aug 2005) | 2 lines

fixed: local arrays plus x86's index*imm + index

------------------------------------------------------------------------
r1467 | v1ctor | 2005-08-04 09:49:18 -0700 (Thu, 04 Aug 2005) | 2 lines

fixed: local arrays plus x86's index*imm + index

------------------------------------------------------------------------
r1466 | lillo | 2005-08-04 08:07:25 -0700 (Thu, 04 Aug 2005) | 2 lines

Added support for custom source area for PUT

------------------------------------------------------------------------
r1465 | lillo | 2005-08-04 05:13:53 -0700 (Thu, 04 Aug 2005) | 2 lines

case fix

------------------------------------------------------------------------
r1464 | lillo | 2005-08-04 05:13:23 -0700 (Thu, 04 Aug 2005) | 2 lines

Linux OpenGL driver now tries lower depth visuals before failing

------------------------------------------------------------------------
r1463 | lillo | 2005-08-04 04:52:47 -0700 (Thu, 04 Aug 2005) | 2 lines

Fixed segmentation fault on exit if multithreaded rtlib is in use under Linux

------------------------------------------------------------------------
r1462 | mjscod | 2005-08-04 02:12:23 -0700 (Thu, 04 Aug 2005) | 2 lines

* Added some comments

------------------------------------------------------------------------
r1461 | mjscod | 2005-08-03 17:27:38 -0700 (Wed, 03 Aug 2005) | 6 lines

* INSTR now uses a Boyer-Moore search algorithm
  It was necessary to implement it "by hand" instead of using strstr because
  both strings (text and pattern) may contain NULs
* Fixed calling convention for in/out functions in libfb_gfx_vgaemu.c
* Added test case for INSTR

------------------------------------------------------------------------
r1460 | lillo | 2005-08-03 14:07:49 -0700 (Wed, 03 Aug 2005) | 2 lines

We require advapi32 now due to IN/OUT support under Win32

------------------------------------------------------------------------
r1459 | lillo | 2005-08-03 13:15:38 -0700 (Wed, 03 Aug 2005) | 2 lines

Added DOS ports I/O support

------------------------------------------------------------------------
r1458 | lillo | 2005-08-03 12:44:51 -0700 (Wed, 03 Aug 2005) | 12 lines

Added Win32 I/O ports support.
The asm IN/OUT instructions are priviledged instructions from Win2K and newer versions.
This means we have to somehow enable them... And the only way is via a kernel driver;
other libraries like PortTalk and InpOut32 do a similar approach, but they both require
a DLL; we use an embedded driver into the EXEs FB generates, if IN/OUT are used. On first
call to IN/OUT, this driver is installed and started; contrary to PortTalk or InpOut32,
this only provides two ioctl commands, one to get the driver version and another to
enable full I/O ports access to a given process id.
You need the Win32 DDK to compile the driver; the driver .sys is then converted to a
.h file holding the binary data using the provided makedriver app, and is then suitable
for inclusion in rtlib. I'm commiting this .h to CVS for simplicity.

------------------------------------------------------------------------
r1457 | lillo | 2005-08-03 07:38:07 -0700 (Wed, 03 Aug 2005) | 2 lines

Forgot to add these two files yesterday

------------------------------------------------------------------------
r1456 | mjscod | 2005-08-03 02:52:42 -0700 (Wed, 03 Aug 2005) | 2 lines

* Make LCASE/UCASE aware of internationalization (i.e. use toupper/tolower from the C RTL)

------------------------------------------------------------------------
r1455 | mjscod | 2005-08-03 02:38:29 -0700 (Wed, 03 Aug 2005) | 3 lines

* Fixed DOS' fb_ConsolePrintBufferEx to use CPUTS correctly
  (translates LF to CR/LF before writing to console)

------------------------------------------------------------------------
r1454 | v1ctor | 2005-08-02 22:19:54 -0700 (Tue, 02 Aug 2005) | 2 lines

for the last time: only cputs() can be used with conio

------------------------------------------------------------------------
r1453 | v1ctor | 2005-08-02 21:00:37 -0700 (Tue, 02 Aug 2005) | 2 lines

fforce-stackframe removed, the "debug" flag should be used instead

------------------------------------------------------------------------
r1452 | v1ctor | 2005-08-02 20:56:57 -0700 (Tue, 02 Aug 2005) | 2 lines

fixed: pointer passed to nullchk() is now converted to void ptr to remove the warnings

------------------------------------------------------------------------
r1451 | v1ctor | 2005-08-02 20:54:29 -0700 (Tue, 02 Aug 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1450 | v1ctor | 2005-08-02 20:54:03 -0700 (Tue, 02 Aug 2005) | 2 lines

calling fb_hRestoreConsoleWindow() makes PRINT 5 times slower than in 0.14, removed

------------------------------------------------------------------------
r1449 | mjscod | 2005-08-02 16:47:27 -0700 (Tue, 02 Aug 2005) | 4 lines

* Replaced __dpmi_int by __dpmi_simulate_real_mode_interrupt
  Otherwise the data put on the stack overwrites the HMA because __dpmi_int
  also sets SS:SP and the FLAGS before calling the interrupt.

------------------------------------------------------------------------
r1448 | mjscod | 2005-08-02 14:03:21 -0700 (Tue, 02 Aug 2005) | 5 lines

* Optimized printing a little bit when output is redirected
* Fixed Win32's fb_ConsoleGetScreenSize to return default values when
  output is redirected
* Added -fforce-stackframe to compiler (eases debugging with GDB)

------------------------------------------------------------------------
r1447 | mjscod | 2005-08-02 12:05:05 -0700 (Tue, 02 Aug 2005) | 2 lines

* Fixed redirected output

------------------------------------------------------------------------
r1446 | mjscod | 2005-08-02 11:57:56 -0700 (Tue, 02 Aug 2005) | 3 lines

* Removed some ocurrencies of srConsoleWindow (error-prone) with calls to
  fb_hConsoleGetWindow

------------------------------------------------------------------------
r1445 | mjscod | 2005-08-02 11:51:22 -0700 (Tue, 02 Aug 2005) | 2 lines

* Fixed the GPF (division by zero) in win32/libfb_io_printbuff.c

------------------------------------------------------------------------
r1444 | lillo | 2005-08-02 03:44:38 -0700 (Tue, 02 Aug 2005) | 2 lines

Added null gfx driver

------------------------------------------------------------------------
r1443 | lillo | 2005-08-02 02:56:45 -0700 (Tue, 02 Aug 2005) | 2 lines

Ops! Reverting unwanted change

------------------------------------------------------------------------
r1442 | lillo | 2005-08-02 02:44:51 -0700 (Tue, 02 Aug 2005) | 2 lines

Added real ports I/O support; Linux implementation and gfxlib modifications in place.

------------------------------------------------------------------------
r1441 | lillo | 2005-08-02 00:27:50 -0700 (Tue, 02 Aug 2005) | 2 lines

gfx WIDTH should return char cells width/height, not pixel width/height

------------------------------------------------------------------------
r1440 | v1ctor | 2005-08-01 20:31:46 -0700 (Mon, 01 Aug 2005) | 2 lines

splits + no console check in _init()

------------------------------------------------------------------------
r1439 | mjscod | 2005-08-01 01:50:48 -0700 (Mon, 01 Aug 2005) | 23 lines

* Optimize fb_hConvPath (faster)
* DOS: Only avoid scrolling if last character isn't a control character
* FIX: LINE INPUT executed in last line of the window
* Source clean-up regarding FB_CON_BOUNDS
* Added *Ex and *RawEx functions for almost all Win32 console I/O functions.
  This was primarily done to allow those I/O functions to be executed on
  non-active screen buffers.
* Made Win32 console I/O a bit faster by reading the current console
  window only when needed.
* Made Win32 console output more consistent by restoring the old console
  window position/size
* Fixed Win32 scrolling when number of lines > number of visible rows
* The Win32 console I/O now uses the foll screen buffer width but only the
  window height because using only the window width would mean a more complex
  parsing/interpretation of the string to print.

* I also updated the comfortable LINE INPUT but it's not activated yet because
  it's still buggy.
* Fixed the line_length when calling locate with col < 1
* Prepared for the better scrolling of VIEWs but not activated after feed-back in the
  Dev-Forums (-> leave it as-is). However, I leave it in the sources (but deactivated)
  for other devs that want to take a look at it.

------------------------------------------------------------------------
r1438 | v1ctor | 2005-07-31 22:16:50 -0700 (Sun, 31 Jul 2005) | 2 lines

changed: using callbacks to add user32

------------------------------------------------------------------------
r1437 | lillo | 2005-07-31 05:48:21 -0700 (Sun, 31 Jul 2005) | 2 lines

user32.lib is now required for console set/getmouse and multikey to work

------------------------------------------------------------------------
r1436 | lillo | 2005-07-31 05:19:06 -0700 (Sun, 31 Jul 2005) | 2 lines

Fixed 2 warnings, made linux rtlib to compile again

------------------------------------------------------------------------
r1435 | v1ctor | 2005-07-30 19:18:02 -0700 (Sat, 30 Jul 2005) | 2 lines

fb_atexit() was removed

------------------------------------------------------------------------
r1434 | v1ctor | 2005-07-29 20:41:25 -0700 (Fri, 29 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1433 | v1ctor | 2005-07-29 20:38:02 -0700 (Fri, 29 Jul 2005) | 2 lines

changed: file objects were been linked again because the extern flag

------------------------------------------------------------------------
r1432 | v1ctor | 2005-07-29 20:37:15 -0700 (Fri, 29 Jul 2005) | 2 lines

fixed: atoi() won't handle unsigned values over 2^31 in DOS or Linux

------------------------------------------------------------------------
r1431 | v1ctor | 2005-07-29 20:36:28 -0700 (Fri, 29 Jul 2005) | 2 lines

fixed: null zstrings compared to null var-len strings wouldn't return 0

------------------------------------------------------------------------
r1430 | v1ctor | 2005-07-29 20:32:42 -0700 (Fri, 29 Jul 2005) | 12 lines

fixed:
- the uinteger literals saturation in DOS and Linux;
- ## when white-spaces were around the macro argument;
- WITH when used with pointers;
- null zstring's compared to null var-len strings;

changed:
- VAL64 renamed to VALLNG;

added:
- VALUINT and VALULNG;

------------------------------------------------------------------------
r1429 | mjscod | 2005-07-29 15:52:14 -0700 (Fri, 29 Jul 2005) | 7 lines

* Fixed LINE INPUT in console mode when using fgets().
  The behaviour is different from QB(X) when the LINE INPUT occurs in the last
  line of the console window but I cannot do anything against it until
  FB_USE_COMFORTABLE_READLINE is set to TRUE in
  fb_dev_scrn_readline.c
* Made the VIEW stuff more robust for applications with std* redirection.

------------------------------------------------------------------------
r1428 | mjscod | 2005-07-29 03:02:49 -0700 (Fri, 29 Jul 2005) | 2 lines

* Fix for CLS while STDOUT is redirected to FILE (caused assertion)

------------------------------------------------------------------------
r1427 | mjscod | 2005-07-29 00:58:33 -0700 (Fri, 29 Jul 2005) | 2 lines

* Fixed INPUT$ not returning a NUL terminated string for SCRN

------------------------------------------------------------------------
r1426 | mjscod | 2005-07-29 00:45:06 -0700 (Fri, 29 Jul 2005) | 4 lines

* Fixed a SIGSEGV for read/write methods for files with len!=0 but without SEEK
  method
* Fixed clearing file handles when fb_Init wasn't executed

------------------------------------------------------------------------
r1425 | v1ctor | 2005-07-28 23:04:12 -0700 (Thu, 28 Jul 2005) | 2 lines

added: DllMain for Windows DLL's

------------------------------------------------------------------------
r1424 | v1ctor | 2005-07-28 21:11:17 -0700 (Thu, 28 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1423 | v1ctor | 2005-07-28 21:06:14 -0700 (Thu, 28 Jul 2005) | 2 lines

please, only add changes that are useful for the end users

------------------------------------------------------------------------
r1422 | v1ctor | 2005-07-28 21:00:51 -0700 (Thu, 28 Jul 2005) | 2 lines

fixed: WIDTH when used as function should support been called w/o ()'s

------------------------------------------------------------------------
r1421 | v1ctor | 2005-07-28 20:57:42 -0700 (Thu, 28 Jul 2005) | 4 lines

changed:
- dev_scrn was split;
- copyright notice;

------------------------------------------------------------------------
r1420 | v1ctor | 2005-07-28 20:52:39 -0700 (Thu, 28 Jul 2005) | 4 lines

changed:
- dev_scrn was split;
- copyright notice;

------------------------------------------------------------------------
r1419 | i_am_drv | 2005-07-28 19:00:20 -0700 (Thu, 28 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1418 | i_am_drv | 2005-07-28 18:57:39 -0700 (Thu, 28 Jul 2005) | 2 lines

- added #include for DOS header

------------------------------------------------------------------------
r1417 | i_am_drv | 2005-07-28 17:53:42 -0700 (Thu, 28 Jul 2005) | 2 lines

- Simple CPU detection

------------------------------------------------------------------------
r1416 | mjscod | 2005-07-28 12:51:44 -0700 (Thu, 28 Jul 2005) | 4 lines

* Updated misleading comment in fb_DevFileOpen function
* Fixed problem with scrolling in Win32 when text longer than window width
* Fixed problem with locate returning values larger than the window size

------------------------------------------------------------------------
r1415 | v1ctor | 2005-07-28 10:43:45 -0700 (Thu, 28 Jul 2005) | 2 lines

fixed: dimensions check in field arrays + missing bounds checking

------------------------------------------------------------------------
r1414 | mjscod | 2005-07-28 04:25:44 -0700 (Thu, 28 Jul 2005) | 2 lines

* Fixed a logic error for FileOpen fallback code. Sorry again. Only use fopen( fname, "rb" ) when "w+b" failed AND access mode == FB_FILE_ACCESS_ANY

------------------------------------------------------------------------
r1413 | mjscod | 2005-07-27 13:28:24 -0700 (Wed, 27 Jul 2005) | 3 lines

* Fix for the fix for the fix. The OPEN fallback mechanism is only allowed to open
  a file in "rb" mode when the access mode was FB_FILE_ACCESS_ANY

------------------------------------------------------------------------
r1412 | mjscod | 2005-07-27 13:20:51 -0700 (Wed, 27 Jul 2005) | 3 lines

* Fix for earlier fix regarding the OPEN fallback strategy when file has to be
  opened in R/W mode but didn't exist yet.

------------------------------------------------------------------------
r1411 | mjscod | 2005-07-27 11:55:35 -0700 (Wed, 27 Jul 2005) | 2 lines

* Fixed the line endings. I'm sorry about that.

------------------------------------------------------------------------
r1410 | mjscod | 2005-07-27 03:15:36 -0700 (Wed, 27 Jul 2005) | 7 lines

  Test applications were examples/file/*.bas
* Fall back for ANY access type didn't work
* GET for files with records didn't work
* LOC(n) returned offset+1
* Line length calculation for PUT on file with RECORDS didn't work
* EOF didn't test for position >= file size if normal EOF failed

------------------------------------------------------------------------
r1409 | mjscod | 2005-07-27 01:49:05 -0700 (Wed, 27 Jul 2005) | 2 lines

* Fixed the issue that CLOSE and other functions didn't emit an error for an unopened handle

------------------------------------------------------------------------
r1408 | v1ctor | 2005-07-26 23:05:15 -0700 (Tue, 26 Jul 2005) | 2 lines

fixed: types that should be structs declared as pointers

------------------------------------------------------------------------
r1407 | mjscod | 2005-07-26 15:13:37 -0700 (Tue, 26 Jul 2005) | 5 lines

* Fixed that FB was unable to find include files.
  This was caused by a handle that wasn't freed when opening a file failed.
  There was also a memory access error that didn't allocate not enough
  memory on the stack for the storage of the file name.

------------------------------------------------------------------------
r1406 | mjscod | 2005-07-26 14:28:48 -0700 (Tue, 26 Jul 2005) | 3 lines

* Fixed regression of file/* test
* Fixed makefile to reflect the changes of the main module handling

------------------------------------------------------------------------
r1405 | mjscod | 2005-07-26 12:54:47 -0700 (Tue, 26 Jul 2005) | 2 lines

* Updated FBC version to 0.15

------------------------------------------------------------------------
r1404 | mjscod | 2005-07-26 12:31:41 -0700 (Tue, 26 Jul 2005) | 4 lines

* Add an option to fb.h to make the OPEN compatible to the one provided by
  QB(X)/VBDOS (FB_QB_COMPATIBLE_OPEN), default = FALSE to make
  all people stop whining about code bloat.

------------------------------------------------------------------------
r1403 | mjscod | 2005-07-26 12:20:39 -0700 (Tue, 26 Jul 2005) | 4 lines

* Fixed compile bug when RTLIB is compiled as MT
  fb_FileReset was unable to access is_exiting variable - now renamed to
  __fb_is_exiting and made non-static

------------------------------------------------------------------------
r1402 | mjscod | 2005-07-26 12:00:28 -0700 (Tue, 26 Jul 2005) | 2 lines

* Compile fix for new function - forgot to include "fb_rterr.h"

------------------------------------------------------------------------
r1401 | mjscod | 2005-07-26 11:59:00 -0700 (Tue, 26 Jul 2005) | 4 lines

* Added fb_SetPos to set the line position for file handles
* I guess that gfxlib2 isn't thread save so it should use a thread safe
  function from RTLIB to set the line position for the screen

------------------------------------------------------------------------
r1400 | mjscod | 2005-07-26 11:53:11 -0700 (Tue, 26 Jul 2005) | 2 lines

* Fixed SendMessage declaration

------------------------------------------------------------------------
r1399 | mjscod | 2005-07-26 11:32:54 -0700 (Tue, 26 Jul 2005) | 6 lines

* splitted large files into multiple smaller files (libfb_print*, libfb_write*)
* removed almost all code that depends on FILE* directly
* implemented OPEN LPT/CONS/ERR/PIPE/SCRN exactly as discussed
* only add LPT code when LPRINT, LPOS, or WIDTH "device" is used
* modified libfb_gfx_print.c to use line_length for screen directly

------------------------------------------------------------------------
r1398 | v1ctor | 2005-07-26 01:39:08 -0700 (Tue, 26 Jul 2005) | 2 lines

changed: printer support shouldn't be linked by default, use hAddPrinterLibs() when LPRINT is decoupled from PRINT

------------------------------------------------------------------------
r1397 | v1ctor | 2005-07-26 01:19:37 -0700 (Tue, 26 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1396 | v1ctor | 2005-07-26 01:12:34 -0700 (Tue, 26 Jul 2005) | 2 lines

changed: some functions split to different files just to make it easier to change, no size gain..

------------------------------------------------------------------------
r1395 | mjscod | 2005-07-25 20:10:53 -0700 (Mon, 25 Jul 2005) | 2 lines

* Forgot to make SCREEN using window-local coordinates on Win32

------------------------------------------------------------------------
r1394 | mjscod | 2005-07-25 15:47:14 -0700 (Mon, 25 Jul 2005) | 3 lines

* Added support for LPOS(n)
* Fixed a bug with the wrap-around for devices with given width

------------------------------------------------------------------------
r1393 | mjscod | 2005-07-25 15:28:21 -0700 (Mon, 25 Jul 2005) | 4 lines

* Added all missing WIDTH variations
* Removed the GetWidth file pointer, uses "width" structure member instead
* Only one LPT1, LPT2, and LPT3 can be opened at the same time

------------------------------------------------------------------------
r1392 | mjscod | 2005-07-25 14:35:04 -0700 (Mon, 25 Jul 2005) | 2 lines

* quick fix ... SCRN: didn't work because it was zeroed after initialization

------------------------------------------------------------------------
r1391 | mjscod | 2005-07-25 12:53:19 -0700 (Mon, 25 Jul 2005) | 13 lines

* changed: FBC (Win32) now automatically adds -l winspool
* added: OPEN VFS is the "new" QB compatible OPEN
* added: LPRINT support
* added: comfortable string input from SCRN: (for all platforms)
* added: Doxygen file, started documentation of RTLIB
* added: Several macros to ease FB_FILE* handling required for the new *Ex() functions
* added: VIEW PRINT() now returns the top and bottom row
* added: fb_FilePushback function for device-independent ungetc()
* fixed: a question mark was never printed by LINE INPUT
* fixed: INPUT$(n) tried to read a complete line first
* fixed: WIDTH on Win32 returned a bad mix of Window/Screen buffer values
* fixed: (hopefully) PAD problem for PRINT

------------------------------------------------------------------------
r1390 | mjscod | 2005-07-25 04:50:24 -0700 (Mon, 25 Jul 2005) | 2 lines

* Added function to query sqlite version as number

------------------------------------------------------------------------
r1389 | v1ctor | 2005-07-24 23:45:55 -0700 (Sun, 24 Jul 2005) | 2 lines

fixed: file names with periods (.)'s

------------------------------------------------------------------------
r1388 | v1ctor | 2005-07-24 23:45:09 -0700 (Sun, 24 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1387 | v1ctor | 2005-07-24 21:45:32 -0700 (Sun, 24 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1386 | v1ctor | 2005-07-24 20:41:41 -0700 (Sun, 24 Jul 2005) | 2 lines

fixed: asin(), acos() and log() can't be done inline as they will use more than a single fpu register - negative numbers were not been handled correctly too

------------------------------------------------------------------------
r1385 | v1ctor | 2005-07-24 20:11:36 -0700 (Sun, 24 Jul 2005) | 2 lines

fixed: asin(), acos() and log() can't be done inline as they will use more than a single fpu register - negative numbers were not been handled correctly too

------------------------------------------------------------------------
r1384 | v1ctor | 2005-07-24 13:07:04 -0700 (Sun, 24 Jul 2005) | 2 lines

fixed: OFFSETOF can now be used in CONST's

------------------------------------------------------------------------
r1383 | v1ctor | 2005-07-24 10:01:59 -0700 (Sun, 24 Jul 2005) | 2 lines

fixed: dup definitions

------------------------------------------------------------------------
r1382 | lillo | 2005-07-24 02:18:25 -0700 (Sun, 24 Jul 2005) | 2 lines

Fixed linux side of things for allegro.bi, fixed a small bug in getjoystick for win32

------------------------------------------------------------------------
r1381 | i_am_drv | 2005-07-23 22:36:58 -0700 (Sat, 23 Jul 2005) | 2 lines

fixed line ending

------------------------------------------------------------------------
r1380 | v1ctor | 2005-07-23 21:43:55 -0700 (Sat, 23 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1379 | v1ctor | 2005-07-23 21:15:43 -0700 (Sat, 23 Jul 2005) | 2 lines

fixed: all ??dir functions had wrong BYVAL as string params - they should be BYREF now that the C functions aren't called directly

------------------------------------------------------------------------
r1378 | v1ctor | 2005-07-23 18:35:11 -0700 (Sat, 23 Jul 2005) | 2 lines

pointer typecasting update..

------------------------------------------------------------------------
r1377 | v1ctor | 2005-07-23 18:22:01 -0700 (Sat, 23 Jul 2005) | 2 lines

fixed: errno declaration

------------------------------------------------------------------------
r1375 | v1ctor | 2005-07-23 11:19:41 -0700 (Sat, 23 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1374 | v1ctor | 2005-07-23 10:39:53 -0700 (Sat, 23 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1373 | v1ctor | 2005-07-23 10:24:18 -0700 (Sat, 23 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1372 | v1ctor | 2005-07-23 10:15:47 -0700 (Sat, 23 Jul 2005) | 2 lines

fixed: abs address + ptr typecasting error

------------------------------------------------------------------------
r1371 | mjscod | 2005-07-23 04:04:03 -0700 (Sat, 23 Jul 2005) | 2 lines

* Line number and error code were flipped during call of fb_ThrowErrorEx from fb_ThrowError

------------------------------------------------------------------------
r1370 | v1ctor | 2005-07-23 00:45:08 -0700 (Sat, 23 Jul 2005) | 2 lines

small fix..

------------------------------------------------------------------------
r1369 | v1ctor | 2005-07-23 00:43:40 -0700 (Sat, 23 Jul 2005) | 2 lines

added: OFFSETOF(type,field) macro

------------------------------------------------------------------------
r1368 | v1ctor | 2005-07-22 21:13:57 -0700 (Fri, 22 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1367 | v1ctor | 2005-07-22 20:45:40 -0700 (Fri, 22 Jul 2005) | 2 lines

fixed: dup error message when an invalid cmd-line option was given

------------------------------------------------------------------------
r1366 | v1ctor | 2005-07-22 20:44:31 -0700 (Fri, 22 Jul 2005) | 2 lines

added: better dynamic array support - it stills limited, only the descriptor is shown

------------------------------------------------------------------------
r1365 | lillo | 2005-07-22 03:43:01 -0700 (Fri, 22 Jul 2005) | 2 lines

More possible win95-related fixes

------------------------------------------------------------------------
r1364 | v1ctor | 2005-07-22 00:18:43 -0700 (Fri, 22 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1363 | v1ctor | 2005-07-21 23:50:27 -0700 (Thu, 21 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1362 | v1ctor | 2005-07-21 23:35:51 -0700 (Thu, 21 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1361 | v1ctor | 2005-07-21 23:31:03 -0700 (Thu, 21 Jul 2005) | 6 lines

changed:
- UNDEF will now work with any kind of symbol;

fixed:
- "#define bar foo" now works, where "foo" is a macro

------------------------------------------------------------------------
r1360 | v1ctor | 2005-07-21 22:52:23 -0700 (Thu, 21 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1359 | v1ctor | 2005-07-21 20:58:18 -0700 (Thu, 21 Jul 2005) | 2 lines

fixed: register spills problems with IIF() and complex expressions

------------------------------------------------------------------------
r1358 | v1ctor | 2005-07-21 20:55:46 -0700 (Thu, 21 Jul 2005) | 2 lines

fixed: '..' and '.' used in paths could confuse getFileExt()

------------------------------------------------------------------------
r1357 | i_am_drv | 2005-07-21 16:27:21 -0700 (Thu, 21 Jul 2005) | 2 lines

fixed doubled DeleteCriticalSection

------------------------------------------------------------------------
r1356 | lillo | 2005-07-21 01:35:41 -0700 (Thu, 21 Jul 2005) | 2 lines

Linux OpenGL driver fixes

------------------------------------------------------------------------
r1355 | v1ctor | 2005-07-20 23:11:54 -0700 (Wed, 20 Jul 2005) | 2 lines

"name" can't be used as macro argument anymore..

------------------------------------------------------------------------
r1354 | v1ctor | 2005-07-20 22:44:10 -0700 (Wed, 20 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1353 | i_am_drv | 2005-07-20 21:13:20 -0700 (Wed, 20 Jul 2005) | 2 lines

- added DOS driver comments

------------------------------------------------------------------------
r1352 | i_am_drv | 2005-07-20 21:08:38 -0700 (Wed, 20 Jul 2005) | 3 lines

- VESA driver is disabled for now
- reset text mode attribute high bit to intense (not blink) when leaving video mode

------------------------------------------------------------------------
r1351 | v1ctor | 2005-07-20 20:38:04 -0700 (Wed, 20 Jul 2005) | 6 lines

added:
- null-pointer checking;

changed:
- bounds and null pointers checkings can be now "skipped" by error handlers;

------------------------------------------------------------------------
r1350 | v1ctor | 2005-07-20 20:35:41 -0700 (Wed, 20 Jul 2005) | 7 lines

added:
- null-pointer checking;
- the -exx (extra error checks) cmd-line option;

changed:
- bounds and null pointers checkings can be now "skipped" by error handlers;

------------------------------------------------------------------------
r1349 | v1ctor | 2005-07-20 20:30:52 -0700 (Wed, 20 Jul 2005) | 2 lines

fixed: .section's options not supported by GAS in Win and DOS

------------------------------------------------------------------------
r1348 | lillo | 2005-07-20 06:22:29 -0700 (Wed, 20 Jul 2005) | 2 lines

Dummy ctors/dtors not needed anymore under Linux

------------------------------------------------------------------------
r1347 | mjscod | 2005-07-20 04:29:05 -0700 (Wed, 20 Jul 2005) | 3 lines

* Added lillos hack for CTORS/DTORS on Linux again - but also test for
  NULL pointers during CTOR/DTOR execution.

------------------------------------------------------------------------
r1346 | lillo | 2005-07-20 02:58:56 -0700 (Wed, 20 Jul 2005) | 2 lines

it's now safe to call dir$ from different threads, thanks to data being stored in TLS

------------------------------------------------------------------------
r1345 | v1ctor | 2005-07-20 00:42:32 -0700 (Wed, 20 Jul 2005) | 2 lines

reverting the inline code used to do bounds checking to a function call due the register spills in complex expressions..

------------------------------------------------------------------------
r1344 | v1ctor | 2005-07-20 00:36:20 -0700 (Wed, 20 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1343 | v1ctor | 2005-07-19 22:38:15 -0700 (Tue, 19 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1342 | i_am_drv | 2005-07-19 22:19:45 -0700 (Tue, 19 Jul 2005) | 2 lines

forgot to select correct union member

------------------------------------------------------------------------
r1341 | v1ctor | 2005-07-19 22:19:35 -0700 (Tue, 19 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1340 | i_am_drv | 2005-07-19 22:17:08 -0700 (Tue, 19 Jul 2005) | 2 lines

- removed XBoxStartup entry point (emitted in source now)

------------------------------------------------------------------------
r1339 | i_am_drv | 2005-07-19 22:13:25 -0700 (Tue, 19 Jul 2005) | 2 lines

- rewrote parts of xbox rtlib (untested)

------------------------------------------------------------------------
r1338 | v1ctor | 2005-07-19 22:02:33 -0700 (Tue, 19 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1337 | v1ctor | 2005-07-19 22:00:54 -0700 (Tue, 19 Jul 2005) | 8 lines

changed (will cause chicken-egg problems):
- array descriptor got an "ubound" field to speed up bounds checking;
- error routines will take the line number too;
- redim preserve moved to its own source file;

added:
- ERL function (returns the line number where the rt error happened);

------------------------------------------------------------------------
r1336 | v1ctor | 2005-07-19 21:55:14 -0700 (Tue, 19 Jul 2005) | 11 lines

changed (will cause chicken-egg problems):
- array descriptor got an "ubound" field to speed up bounds checking;
- error routines will take the line number too;
- redim preserve moved to its own source file;

fixed:
- redim preserve leak if array had dynamic strings;

added:
- ERL function (returns the line number where the rt error happened);

------------------------------------------------------------------------
r1335 | i_am_drv | 2005-07-19 21:27:01 -0700 (Tue, 19 Jul 2005) | 2 lines

- changed to emit xbox entrypoint XBoxStartup (removed XBoxStartup from src/rtlib/xbox/libfb_hinit.c)

------------------------------------------------------------------------
r1334 | i_am_drv | 2005-07-19 20:49:18 -0700 (Tue, 19 Jul 2005) | 2 lines

mode x example for dos

------------------------------------------------------------------------
r1333 | i_am_drv | 2005-07-19 20:20:04 -0700 (Tue, 19 Jul 2005) | 2 lines

- changed WIN32 to TARGET_WIN32 to fix xbox port

------------------------------------------------------------------------
r1332 | i_am_drv | 2005-07-19 20:17:11 -0700 (Tue, 19 Jul 2005) | 2 lines

- changes to xbox target - set OPENXDK to path to openxdk directory

------------------------------------------------------------------------
r1331 | v1ctor | 2005-07-19 13:25:06 -0700 (Tue, 19 Jul 2005) | 2 lines

unused function removed

------------------------------------------------------------------------
r1330 | v1ctor | 2005-07-19 13:19:11 -0700 (Tue, 19 Jul 2005) | 2 lines

fixed: 0 passed as num

------------------------------------------------------------------------
r1329 | mjscod | 2005-07-19 11:40:53 -0700 (Tue, 19 Jul 2005) | 4 lines

* Fix for old LD not supporting LONG(0) in the linker scripts
* Replaced lillos changes with a more generic solution.
* DON'T USE KEEP IN THE LINKER SCRIPT if you want to avoid code bloat

------------------------------------------------------------------------
r1328 | lillo | 2005-07-19 09:15:08 -0700 (Tue, 19 Jul 2005) | 2 lines

Fixed profiler

------------------------------------------------------------------------
r1327 | lillo | 2005-07-19 09:03:57 -0700 (Tue, 19 Jul 2005) | 2 lines

Small fix to help creating shared libs to work again

------------------------------------------------------------------------
r1326 | lillo | 2005-07-19 08:43:19 -0700 (Tue, 19 Jul 2005) | 2 lines

Fixed Linux build, including ctors/dtors. Other small fixes here and there.

------------------------------------------------------------------------
r1325 | v1ctor | 2005-07-18 23:28:41 -0700 (Mon, 18 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1324 | i_am_drv | 2005-07-18 21:36:38 -0700 (Mon, 18 Jul 2005) | 2 lines

- rewrote HEX to return uppercase on all platforms

------------------------------------------------------------------------
r1323 | v1ctor | 2005-07-18 21:10:54 -0700 (Mon, 18 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1322 | v1ctor | 2005-07-18 20:44:31 -0700 (Mon, 18 Jul 2005) | 8 lines

changed:
- module-level code from other modules than the main one will be now executed, as in Pascal;
- Constructor and Destructor added as keywords;
- ASSERT now calls a rtlib function;

added:
- ASSERTWARN macro;

------------------------------------------------------------------------
r1321 | v1ctor | 2005-07-18 20:39:03 -0700 (Mon, 18 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1320 | mjscod | 2005-07-18 07:44:50 -0700 (Mon, 18 Jul 2005) | 3 lines

* Forgot to add changed *.x files (required for definition of .fb_ctors and .fb_dtors sections
* Added call to __main in fb_hInit for win32 and xbox target to call all constructor functions.

------------------------------------------------------------------------
r1319 | mjscod | 2005-07-18 07:37:55 -0700 (Mon, 18 Jul 2005) | 6 lines

* argc/argv will always be passed to fb_Init
* Added CONSTRUCTOR and DESTRUCTOR functions
* Fixed the function calls to "beep", "chdir", "mkdir", and "rmdir"
* Added compiler switch "-map <filename>" to produce a map file
* Changed beep frequency (was too short and high on my WinXP Pro SP2 machine)

------------------------------------------------------------------------
r1318 | i_am_drv | 2005-07-17 17:45:22 -0700 (Sun, 17 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1317 | v1ctor | 2005-07-17 10:55:53 -0700 (Sun, 17 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1316 | v1ctor | 2005-07-17 10:46:48 -0700 (Sun, 17 Jul 2005) | 2 lines

entry-point changes update..

------------------------------------------------------------------------
r1315 | i_am_drv | 2005-07-17 10:19:36 -0700 (Sun, 17 Jul 2005) | 2 lines

- added reference to OpenXDK

------------------------------------------------------------------------
r1314 | i_am_drv | 2005-07-17 09:01:25 -0700 (Sun, 17 Jul 2005) | 2 lines

- restore previous console settings on exit

------------------------------------------------------------------------
r1313 | v1ctor | 2005-07-16 18:43:22 -0700 (Sat, 16 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1312 | v1ctor | 2005-07-16 17:49:02 -0700 (Sat, 16 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1311 | v1ctor | 2005-07-16 17:09:17 -0700 (Sat, 16 Jul 2005) | 7 lines

changed:
- main() is now the entry-point (always);
- fbc win32 can now compile and link the other versions;

fixed:
- dos main() argc/argv pos;

------------------------------------------------------------------------
r1310 | lillo | 2005-07-14 03:01:45 -0700 (Thu, 14 Jul 2005) | 4 lines

- Entry point main proc has no return address, so offset of params on the stack (argc/argv) needs to be adjusted accordingly
- Fixed fbc compiling under Linux
- MULTIKEY/GETMOUSE/SETMOUSE do not require gfxlib to be linked in anymore

------------------------------------------------------------------------
r1309 | mjscod | 2005-07-14 01:39:29 -0700 (Thu, 14 Jul 2005) | 2 lines

* Forgot to add my IDs to the fixes/additions

------------------------------------------------------------------------
r1308 | v1ctor | 2005-07-13 20:57:57 -0700 (Wed, 13 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1307 | v1ctor | 2005-07-13 20:52:08 -0700 (Wed, 13 Jul 2005) | 6 lines

added:
- functions can now return UDT's;

changed:
- UDT assignaments are now done with inline code;

------------------------------------------------------------------------
r1306 | mjscod | 2005-07-13 13:51:37 -0700 (Wed, 13 Jul 2005) | 11 lines

* Support for NAME ... AS ...
* DIM of a DYNAMIC array now fails with an error when using invalid bounds
* Added some CONS: aliases
* Fixed a bug in the DIM test case that caused an exception
* Support DJGPP compilation under Win32 without using TARGET=dos
* Added support for numeric (non-string macros)
* Added version macros __FB_VER_MAJOR__, __FB_VER_MINOR__, __FB_VER_PATCH__, and __FB_MIN_VERSION__
* Make __LINE__ returning a number instead of a string
* Fixed a GLIB macro that uses the __LINE__ macro in an invalid way
* Cleanup of hMatchLPRNT, hMatchRPRNT, hMatchCOMMA, added hMatchToken

------------------------------------------------------------------------
r1305 | lillo | 2005-07-13 11:14:25 -0700 (Wed, 13 Jul 2005) | 2 lines

small fix

------------------------------------------------------------------------
r1304 | v1ctor | 2005-07-12 21:03:57 -0700 (Tue, 12 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1303 | v1ctor | 2005-07-12 20:57:08 -0700 (Tue, 12 Jul 2005) | 5 lines

major internal changes:
- parser won't can IR or EMIT anymore;
- AST will keep all function nodes until it's fully parsed;
- EMIT will also cache the output, using pseudo-ops;

------------------------------------------------------------------------
r1302 | lillo | 2005-07-12 13:41:11 -0700 (Tue, 12 Jul 2005) | 2 lines

We now use the low 3 bits of a GET/PUT buffer to store the image bpp, so PUT knows if an image is valid for the current depth

------------------------------------------------------------------------
r1301 | lillo | 2005-07-12 08:41:39 -0700 (Tue, 12 Jul 2005) | 2 lines

console mode multikey and getmouse now work only if terminal window has focus if under X

------------------------------------------------------------------------
r1300 | i_am_drv | 2005-07-11 16:08:29 -0700 (Mon, 11 Jul 2005) | 2 lines

- committing changes to dos gfxlib (not complete yet) (ntfs partition resizing time; cross your fingers :)

------------------------------------------------------------------------
r1299 | i_am_drv | 2005-07-11 16:05:29 -0700 (Mon, 11 Jul 2005) | 2 lines

- initial xbox port check-in (some stuff still todo)

------------------------------------------------------------------------
r1298 | lillo | 2005-07-11 07:05:05 -0700 (Mon, 11 Jul 2005) | 2 lines

Fixed wrong default parameter passed to VIEW

------------------------------------------------------------------------
r1297 | lillo | 2005-07-11 06:51:41 -0700 (Mon, 11 Jul 2005) | 2 lines

Fixed coordinates translation with PMAP

------------------------------------------------------------------------
r1296 | v1ctor | 2005-07-10 12:47:33 -0700 (Sun, 10 Jul 2005) | 2 lines

fixed: type casting used with function pointers

------------------------------------------------------------------------
r1295 | lillo | 2005-07-10 08:44:55 -0700 (Sun, 10 Jul 2005) | 2 lines

made multikey to work only if console window has focus

------------------------------------------------------------------------
r1294 | lillo | 2005-07-10 06:23:13 -0700 (Sun, 10 Jul 2005) | 2 lines

Fixed small BLOAD problem, made GETMOUSE to work under Linux console

------------------------------------------------------------------------
r1293 | v1ctor | 2005-07-09 15:53:07 -0700 (Sat, 09 Jul 2005) | 2 lines

changed: data read statements split in multiple sources

------------------------------------------------------------------------
r1292 | i_am_drv | 2005-07-08 17:54:30 -0700 (Fri, 08 Jul 2005) | 2 lines

- reverted to not restoring cursor state at exit

------------------------------------------------------------------------
r1291 | i_am_drv | 2005-07-07 18:03:05 -0700 (Thu, 07 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1290 | i_am_drv | 2005-07-06 17:05:11 -0700 (Wed, 06 Jul 2005) | 2 lines

fixed failure on DOS

------------------------------------------------------------------------
r1289 | v1ctor | 2005-07-05 23:15:38 -0700 (Tue, 05 Jul 2005) | 2 lines

fixed: compile-time enum to anything-but-integer conversion

------------------------------------------------------------------------
r1288 | v1ctor | 2005-07-05 20:51:19 -0700 (Tue, 05 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1287 | v1ctor | 2005-07-05 20:27:28 -0700 (Tue, 05 Jul 2005) | 6 lines

internal changes: IR will only pass operands by vreg to EMIT now;

fixed:
- overloaded functions could not use the function name to assign results;
- local undefined labels were silently ignored;

------------------------------------------------------------------------
r1286 | lillo | 2005-07-05 07:39:37 -0700 (Tue, 05 Jul 2005) | 2 lines

Added WIDTH support for Xterm

------------------------------------------------------------------------
r1285 | lillo | 2005-07-05 07:11:14 -0700 (Tue, 05 Jul 2005) | 2 lines

Small fix

------------------------------------------------------------------------
r1284 | lillo | 2005-07-05 03:00:46 -0700 (Tue, 05 Jul 2005) | 2 lines

Small fix

------------------------------------------------------------------------
r1283 | lillo | 2005-07-05 02:13:48 -0700 (Tue, 05 Jul 2005) | 2 lines

Replaced -noexport with -export

------------------------------------------------------------------------
r1282 | lillo | 2005-07-05 01:56:37 -0700 (Tue, 05 Jul 2005) | 2 lines

Fixed missing definitions

------------------------------------------------------------------------
r1281 | lillo | 2005-07-05 01:53:29 -0700 (Tue, 05 Jul 2005) | 2 lines

Small fixes to the new signals common infrastructure, made Linux to work again with new system

------------------------------------------------------------------------
r1280 | v1ctor | 2005-07-04 20:51:00 -0700 (Mon, 04 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1279 | v1ctor | 2005-07-04 20:42:57 -0700 (Mon, 04 Jul 2005) | 2 lines

added: signal and exception handlers

------------------------------------------------------------------------
r1278 | v1ctor | 2005-07-04 20:38:32 -0700 (Mon, 04 Jul 2005) | 2 lines

changed: VIEW globals removed

------------------------------------------------------------------------
r1277 | v1ctor | 2005-07-04 20:37:46 -0700 (Mon, 04 Jul 2005) | 2 lines

minor cleanups

------------------------------------------------------------------------
r1276 | v1ctor | 2005-07-04 20:36:21 -0700 (Mon, 04 Jul 2005) | 2 lines

added: signal and exception handlers

------------------------------------------------------------------------
r1275 | i_am_drv | 2005-07-04 17:36:59 -0700 (Mon, 04 Jul 2005) | 2 lines

changes for xbox support

------------------------------------------------------------------------
r1274 | lillo | 2005-07-04 10:54:53 -0700 (Mon, 04 Jul 2005) | 2 lines

Added GETMOUSE/SETMOUSE support for Linux console

------------------------------------------------------------------------
r1273 | i_am_drv | 2005-07-04 08:44:13 -0700 (Mon, 04 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1272 | v1ctor | 2005-07-04 00:58:47 -0700 (Mon, 04 Jul 2005) | 7 lines

added:
- better parameters pointer checking;

changed:
- no warnings if different pointers have the same type sizes;
- pointers can now be subtracted, becoming an uint;

------------------------------------------------------------------------
r1271 | v1ctor | 2005-07-03 22:06:08 -0700 (Sun, 03 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1270 | v1ctor | 2005-07-03 21:23:49 -0700 (Sun, 03 Jul 2005) | 2 lines

pointer type-casting updates..

------------------------------------------------------------------------
r1269 | v1ctor | 2005-07-03 21:17:19 -0700 (Sun, 03 Jul 2005) | 2 lines

pointer type-casting updates..

------------------------------------------------------------------------
r1268 | i_am_drv | 2005-07-03 10:02:39 -0700 (Sun, 03 Jul 2005) | 2 lines

updated for xbox

------------------------------------------------------------------------
r1267 | i_am_drv | 2005-07-03 09:57:24 -0700 (Sun, 03 Jul 2005) | 5 lines

changes to support xbox:
- changed #ifdef WIN32 to #ifdef TARGET_WIN32
- updated makefile
- defined FBCALL for xbox as __cdecl on win32

------------------------------------------------------------------------
r1266 | v1ctor | 2005-07-02 23:43:04 -0700 (Sat, 02 Jul 2005) | 2 lines

fixed: AS ANY check added for functions

------------------------------------------------------------------------
r1265 | v1ctor | 2005-07-02 23:42:04 -0700 (Sat, 02 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1264 | i_am_drv | 2005-07-02 21:28:26 -0700 (Sat, 02 Jul 2005) | 2 lines

- #ifdefs for PIPE: support (not available on xbox)

------------------------------------------------------------------------
r1263 | v1ctor | 2005-07-02 21:28:10 -0700 (Sat, 02 Jul 2005) | 2 lines

cputs() must be used or color attributes won't work

------------------------------------------------------------------------
r1262 | i_am_drv | 2005-07-02 21:16:46 -0700 (Sat, 02 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1261 | v1ctor | 2005-07-02 21:05:31 -0700 (Sat, 02 Jul 2005) | 2 lines

cprint() must be used or color attributes won't work

------------------------------------------------------------------------
r1260 | v1ctor | 2005-07-02 20:49:19 -0700 (Sat, 02 Jul 2005) | 2 lines

DJGPP check as for the rtlib

------------------------------------------------------------------------
r1259 | v1ctor | 2005-07-02 20:35:45 -0700 (Sat, 02 Jul 2005) | 2 lines

small fix

------------------------------------------------------------------------
r1258 | v1ctor | 2005-07-02 14:46:50 -0700 (Sat, 02 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1257 | v1ctor | 2005-07-02 14:00:59 -0700 (Sat, 02 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1256 | v1ctor | 2005-07-02 13:44:32 -0700 (Sat, 02 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1255 | v1ctor | 2005-07-02 13:19:36 -0700 (Sat, 02 Jul 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1254 | v1ctor | 2005-07-02 13:11:00 -0700 (Sat, 02 Jul 2005) | 2 lines

pointer type-casting update..

------------------------------------------------------------------------
r1253 | v1ctor | 2005-07-02 13:05:10 -0700 (Sat, 02 Jul 2005) | 2 lines

added: pointer type-casting

------------------------------------------------------------------------
r1252 | v1ctor | 2005-07-02 12:58:20 -0700 (Sat, 02 Jul 2005) | 2 lines

changed: sources moved around to decrease the bloat..

------------------------------------------------------------------------
r1251 | v1ctor | 2005-07-02 12:56:28 -0700 (Sat, 02 Jul 2005) | 2 lines

added: missing hooks

------------------------------------------------------------------------
r1250 | lillo | 2005-07-02 09:02:51 -0700 (Sat, 02 Jul 2005) | 2 lines

Added Linux console MULTIKEY support

------------------------------------------------------------------------
r1249 | lillo | 2005-06-30 07:05:28 -0700 (Thu, 30 Jun 2005) | 2 lines

Added scroll wheel support for getmouse in win32 console mode

------------------------------------------------------------------------
r1248 | i_am_drv | 2005-06-29 19:43:20 -0700 (Wed, 29 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1247 | i_am_drv | 2005-06-29 19:32:26 -0700 (Wed, 29 Jun 2005) | 2 lines

- INKEY, GETKEY, and KEYHIT now work after calling MULTIKEY (but capslock and numlock are ignored for now)

------------------------------------------------------------------------
r1246 | i_am_drv | 2005-06-29 19:00:59 -0700 (Wed, 29 Jun 2005) | 2 lines

- console MULTIKEY for DOS added (currently INKEY$ no longer works after MULTIKEY is called in console mode)

------------------------------------------------------------------------
r1245 | i_am_drv | 2005-06-29 17:38:36 -0700 (Wed, 29 Jun 2005) | 2 lines

- restore text cusror visibility on exit

------------------------------------------------------------------------
r1244 | i_am_drv | 2005-06-29 17:11:38 -0700 (Wed, 29 Jun 2005) | 2 lines

- console GETMOUSE functionality for DOS implemented

------------------------------------------------------------------------
r1243 | i_am_drv | 2005-06-29 16:24:32 -0700 (Wed, 29 Jun 2005) | 2 lines

- changed DOS detection to check DJGPP env var instead of OS=DOS

------------------------------------------------------------------------
r1242 | lillo | 2005-06-29 07:11:06 -0700 (Wed, 29 Jun 2005) | 3 lines

- Added support for MULTIKEY/GETMOUSE/SETMOUSE in plain console mode too (only win32 hooks implemented so far)
- Added Win32 specific header for rtlib; this slows down compiling a bit as it always includes windows.h, but makes code cleaner IMHO...

------------------------------------------------------------------------
r1241 | lillo | 2005-06-28 02:58:56 -0700 (Tue, 28 Jun 2005) | 2 lines

Restore console input state also under plain Linux console

------------------------------------------------------------------------
r1240 | lillo | 2005-06-28 02:36:04 -0700 (Tue, 28 Jun 2005) | 2 lines

Small fix

------------------------------------------------------------------------
r1239 | i_am_drv | 2005-06-27 17:43:15 -0700 (Mon, 27 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1238 | mjscod | 2005-06-27 12:22:50 -0700 (Mon, 27 Jun 2005) | 3 lines

* More CYGWIN fixes
* Forgot to mention in the last check-in that pthread mutexes are recursive now

------------------------------------------------------------------------
r1237 | mjscod | 2005-06-27 12:01:15 -0700 (Mon, 27 Jun 2005) | 6 lines

* Fixed padding (the COMMA with PRINT) for redirected output, console output, and file I/O
* Uses fb_PrintPad instead of fb_PrintTab (was already used)
* Make linux version compilable with cygwin environment (executable not working yet)
* Added the ability to select the naming convention independent of target selection
* Some C++ compilation fixes (experimental - only used when __cplusplus is defined)

------------------------------------------------------------------------
r1236 | lillo | 2005-06-27 10:03:46 -0700 (Mon, 27 Jun 2005) | 2 lines

PRESET now behaves correctly

------------------------------------------------------------------------
r1235 | lillo | 2005-06-27 09:40:12 -0700 (Mon, 27 Jun 2005) | 2 lines

Fixed bug in PUT for 8bit TRANS MMX routine

------------------------------------------------------------------------
r1234 | mjscod | 2005-06-26 05:55:41 -0700 (Sun, 26 Jun 2005) | 5 lines

- fb_*PrintBuffer*Ex() functions that take an additional "len" parameter
- fb_PrintTab function to hide the implementation for TAB widths <>8
- chr$(0) can now be output with PRINT too
- Some cleanup to make most part of the library compilable with C++ too

------------------------------------------------------------------------
r1233 | lillo | 2005-06-24 06:16:32 -0700 (Fri, 24 Jun 2005) | 2 lines

GDI fixes, Win32 vsync fixes, fixed a possible problem if SCREENCOPY was called when the screen lock was held

------------------------------------------------------------------------
r1232 | lillo | 2005-06-23 07:24:46 -0700 (Thu, 23 Jun 2005) | 3 lines

- Win32 OpenGL driver had problems with fullscreen modes on some machines
- Made all Win32 gfx drivers more vsync friendly

------------------------------------------------------------------------
r1231 | v1ctor | 2005-06-23 01:44:44 -0700 (Thu, 23 Jun 2005) | 2 lines

added: checks for periods used in field names or when declaring UDT args and vars

------------------------------------------------------------------------
r1230 | mjscod | 2005-06-22 23:48:57 -0700 (Wed, 22 Jun 2005) | 4 lines

* Added new test case because assigning MID$()= didn't release temporary strings
* Fixed the bug
* Updated changelog

------------------------------------------------------------------------
r1229 | mjscod | 2005-06-22 23:08:51 -0700 (Wed, 22 Jun 2005) | 6 lines

* text_in doesn't return with exit code 1 on linux systems any more
* added a small comment to cmmn_01.bas about the FIXME
* added additional check to submod_b.bas
* chr_0 rewritten to use ASSERTs instead
* supports FIXME instead of FIXIT in the makefile. FIXIT is still valid

------------------------------------------------------------------------
r1228 | v1ctor | 2005-06-22 22:45:06 -0700 (Wed, 22 Jun 2005) | 2 lines

changed: ASSERT is now a built-in macro

------------------------------------------------------------------------
r1227 | v1ctor | 2005-06-22 22:05:04 -0700 (Wed, 22 Jun 2005) | 2 lines

changed: using ASSERT's now

------------------------------------------------------------------------
r1226 | v1ctor | 2005-06-22 22:02:23 -0700 (Wed, 22 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1225 | v1ctor | 2005-06-22 21:47:19 -0700 (Wed, 22 Jun 2005) | 8 lines

changed: - cosmetic changes on constant names ('.' to '_');
- OPTION's will now be used too in the context of the file that included a header if it changed the default OPTION's;

added: - the '#' prefix to be used with macro arguments;
- the ASSERT built-in macro;

fixed: - ENUM's couldn't be mixed with integers in IIF();

------------------------------------------------------------------------
r1224 | mjscod | 2005-06-22 17:41:09 -0700 (Wed, 22 Jun 2005) | 3 lines

* Updated changelog (added my missing handles)
* Reverted lillos change in libfb_gfx_paint.c to restore v1ctor's fix. Even when mode!=PAINT_TYPE_PATTERN, pattern can still point to a temporary string. Caused regression with "sierpinski teppich" example posted in the forum.

------------------------------------------------------------------------
r1223 | mjscod | 2005-06-22 12:44:25 -0700 (Wed, 22 Jun 2005) | 2 lines

* Fix(?) for the chr$(0) problem (see changelog and tests/string/chr_0.bas)

------------------------------------------------------------------------
r1222 | mjscod | 2005-06-22 12:32:39 -0700 (Wed, 22 Jun 2005) | 4 lines

* Provide more information in submod_a file (ptr to array fron non-main module)
  This shows that this test still doesn't work even when FIXIT=1 was specified
* Added a new test for the "s$=chr$(0): print len(s$)=1" problem

------------------------------------------------------------------------
r1221 | lillo | 2005-06-22 01:17:00 -0700 (Wed, 22 Jun 2005) | 2 lines

Now also looks for include files in the directory of the includer source file

------------------------------------------------------------------------
r1220 | v1ctor | 2005-06-21 21:25:35 -0700 (Tue, 21 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1219 | v1ctor | 2005-06-21 21:16:39 -0700 (Tue, 21 Jun 2005) | 9 lines

changed: - ENUM's won't become integers anymore, to allow better debugging and overloading of enum args;

added: - labels already defined can now be referenced in inline ASM;
- #DEFINE macros can now be empty too;
- '##'s can be used to delimit macro arguments as in C;

fixed: - the ANY type was been allowed in DIM/REDIM;
- redimension of array fields was not resulting in a compile-time error;

------------------------------------------------------------------------
r1218 | mjscod | 2005-06-21 18:45:48 -0700 (Tue, 21 Jun 2005) | 2 lines

The _my_?s() functions now use FUNCTION as target in the inline assembler code. Looks much better.

------------------------------------------------------------------------
r1217 | mjscod | 2005-06-21 17:55:42 -0700 (Tue, 21 Jun 2005) | 2 lines

tried to compare with "=" instead of "==" ... was still in BASIC mode ...

------------------------------------------------------------------------
r1216 | mjscod | 2005-06-21 17:35:13 -0700 (Tue, 21 Jun 2005) | 4 lines

* Fix possible SIGSEGV on Linux (strlen on NULL pointer)
* DOS: fb_hGetCurrentDir returned invalid path length
* DOS: fb_hGetCurrentDir now returns path with backslashes

------------------------------------------------------------------------
r1215 | mjscod | 2005-06-21 15:19:46 -0700 (Tue, 21 Jun 2005) | 2 lines

Forgot to update the changelog about the last two fixes

------------------------------------------------------------------------
r1214 | mjscod | 2005-06-21 14:59:24 -0700 (Tue, 21 Jun 2005) | 2 lines

Fixed the _get_ds, _get_cs, and _get_ss functions ... but (IMHO) this is just a hack. There must be a better way.

------------------------------------------------------------------------
r1213 | lillo | 2005-06-21 14:48:02 -0700 (Tue, 21 Jun 2005) | 2 lines

small fix

------------------------------------------------------------------------
r1212 | mjscod | 2005-06-21 11:38:00 -0700 (Tue, 21 Jun 2005) | 2 lines

* Added a new test case about the common instruction. When you want to DIM the previously COMMON'ed file without using '$DYNAMIC, it complains about a duplicate definition. Is this correct?

------------------------------------------------------------------------
r1211 | mjscod | 2005-06-21 11:03:10 -0700 (Tue, 21 Jun 2005) | 2 lines

Enhanced the extern/dim test. Now should return 1 when the array pointer differs between submod_a and submod_b

------------------------------------------------------------------------
r1210 | mjscod | 2005-06-21 09:48:43 -0700 (Tue, 21 Jun 2005) | 2 lines

Added a new test case. This code should be valid but the resulting application crashes.

------------------------------------------------------------------------
r1209 | mjscod | 2005-06-21 05:50:57 -0700 (Tue, 21 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1208 | v1ctor | 2005-06-21 01:51:07 -0700 (Tue, 21 Jun 2005) | 2 lines

fixed: pattern string wasn't been freed if current pixel color was equal to the border color

------------------------------------------------------------------------
r1207 | mjscod | 2005-06-20 23:42:09 -0700 (Mon, 20 Jun 2005) | 2 lines

Fix EOF handling for INPUT files ... caused an endless loop in while not eof() ... code

------------------------------------------------------------------------
r1206 | mjscod | 2005-06-20 23:40:34 -0700 (Mon, 20 Jun 2005) | 2 lines

Added more "line input" test cases for binary files with EOF in between

------------------------------------------------------------------------
r1205 | mjscod | 2005-06-20 23:29:36 -0700 (Mon, 20 Jun 2005) | 2 lines

Added line input tests for files opened as BINARY

------------------------------------------------------------------------
r1204 | v1ctor | 2005-06-20 21:00:42 -0700 (Mon, 20 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1203 | lillo | 2005-06-20 12:51:35 -0700 (Mon, 20 Jun 2005) | 2 lines

Small fixes

------------------------------------------------------------------------
r1202 | mjscod | 2005-06-20 05:48:18 -0700 (Mon, 20 Jun 2005) | 2 lines

Allow compilation of libraries in DEBUG mode too (useful for debugging the libraries). To compile the libraries in debug mode set DEBUG=1.

------------------------------------------------------------------------
r1201 | v1ctor | 2005-06-19 21:15:41 -0700 (Sun, 19 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1200 | v1ctor | 2005-06-19 21:05:13 -0700 (Sun, 19 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1199 | v1ctor | 2005-06-19 20:28:35 -0700 (Sun, 19 Jun 2005) | 2 lines

fixed: inline asm more compatible with gcc 4.0

------------------------------------------------------------------------
r1198 | v1ctor | 2005-06-19 19:56:33 -0700 (Sun, 19 Jun 2005) | 2 lines

bufix 1223658

------------------------------------------------------------------------
r1197 | v1ctor | 2005-06-19 16:00:23 -0700 (Sun, 19 Jun 2005) | 2 lines

fixed: void deref check

------------------------------------------------------------------------
r1196 | v1ctor | 2005-06-19 15:51:23 -0700 (Sun, 19 Jun 2005) | 2 lines

fixed: target if a bydesc argument would trash the AST tree reusing the same node twice

------------------------------------------------------------------------
r1195 | lillo | 2005-06-19 07:20:13 -0700 (Sun, 19 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1194 | lillo | 2005-06-19 07:09:26 -0700 (Sun, 19 Jun 2005) | 2 lines

Made LOCATE to return current/last cursor state

------------------------------------------------------------------------
r1193 | i_am_drv | 2005-06-18 23:10:28 -0700 (Sat, 18 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1192 | i_am_drv | 2005-06-18 23:08:14 -0700 (Sat, 18 Jun 2005) | 2 lines

still some mem accessed in int handlers that isn't locked - run at your own risk

------------------------------------------------------------------------
r1191 | i_am_drv | 2005-06-18 22:40:16 -0700 (Sat, 18 Jun 2005) | 2 lines

changed to report correct refresh rate (70Hz)

------------------------------------------------------------------------
r1190 | lillo | 2005-06-18 17:31:31 -0700 (Sat, 18 Jun 2005) | 3 lines

- Recursive UDTs are now signaled as error
- Small fixes

------------------------------------------------------------------------
r1189 | v1ctor | 2005-06-18 12:07:38 -0700 (Sat, 18 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1188 | v1ctor | 2005-06-18 12:03:59 -0700 (Sat, 18 Jun 2005) | 2 lines

changed: COLOR and WIDTH will now return the last/current colors and dimensions, respectively

------------------------------------------------------------------------
r1187 | v1ctor | 2005-06-18 12:02:36 -0700 (Sat, 18 Jun 2005) | 2 lines

changed: COLOR and WIDTH will now return the last/current colors and dimensions, in that order

------------------------------------------------------------------------
r1186 | lillo | 2005-06-18 04:36:52 -0700 (Sat, 18 Jun 2005) | 3 lines

- rtlib/gfxlib makefiles now create obj and lib dirs if not present
- removed unnecessary signals setup in linux fb_hInitConsole

------------------------------------------------------------------------
r1185 | i_am_drv | 2005-06-17 17:03:57 -0700 (Fri, 17 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1184 | lillo | 2005-06-16 05:09:45 -0700 (Thu, 16 Jun 2005) | 4 lines

- Fixed a memory leak in gfxlib, had to modify rtlib for this.
- Modified compiler makefile to always use -target if under Win32; this
should fix both the DOS and Linux build problems.

------------------------------------------------------------------------
r1183 | v1ctor | 2005-06-15 14:41:20 -0700 (Wed, 15 Jun 2005) | 2 lines

fixed: mangling was not taking into account UDT arguments in overloaded functions

------------------------------------------------------------------------
r1182 | lillo | 2005-06-14 11:04:54 -0700 (Tue, 14 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1181 | lillo | 2005-06-14 10:57:13 -0700 (Tue, 14 Jun 2005) | 2 lines

small fix

------------------------------------------------------------------------
r1180 | lillo | 2005-06-14 03:35:26 -0700 (Tue, 14 Jun 2005) | 2 lines

now using /dev/tty for linux console input to it works in any case

------------------------------------------------------------------------
r1179 | lillo | 2005-06-14 02:55:52 -0700 (Tue, 14 Jun 2005) | 2 lines

Small changes

------------------------------------------------------------------------
r1178 | v1ctor | 2005-06-13 23:39:58 -0700 (Mon, 13 Jun 2005) | 2 lines

small fix..

------------------------------------------------------------------------
r1177 | lillo | 2005-06-13 06:10:42 -0700 (Mon, 13 Jun 2005) | 2 lines

Fixed bug that forced Linux console clear on program exit if CLS was used at least once in the program

------------------------------------------------------------------------
r1176 | i_am_drv | 2005-06-12 15:20:17 -0700 (Sun, 12 Jun 2005) | 3 lines

- added scancode to ascii translation to call fb_hPostKey
- added fb_hScreenInfo implementation

------------------------------------------------------------------------
r1175 | v1ctor | 2005-06-12 10:56:40 -0700 (Sun, 12 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1174 | v1ctor | 2005-06-12 09:40:22 -0700 (Sun, 12 Jun 2005) | 2 lines

fixed: module-level temp vars created by IR could be allocated on stack, due scope been changed before flushing IR

------------------------------------------------------------------------
r1173 | v1ctor | 2005-06-10 20:20:59 -0700 (Fri, 10 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1172 | v1ctor | 2005-06-09 12:19:49 -0700 (Thu, 09 Jun 2005) | 2 lines

added: proper escape sequences handling with ASC( literal )

------------------------------------------------------------------------
r1171 | lillo | 2005-06-09 08:03:32 -0700 (Thu, 09 Jun 2005) | 2 lines

This fixes small mis-alignment of window content and window border

------------------------------------------------------------------------
r1170 | lillo | 2005-06-09 01:49:18 -0700 (Thu, 09 Jun 2005) | 2 lines

Added DYLIBFREE

------------------------------------------------------------------------
r1169 | i_am_drv | 2005-06-08 20:15:53 -0700 (Wed, 08 Jun 2005) | 2 lines

moved VESA-specific definitions out of fb_gfx_dos.h

------------------------------------------------------------------------
r1168 | i_am_drv | 2005-06-08 20:14:55 -0700 (Wed, 08 Jun 2005) | 6 lines

some improvements:
- in paletted modes, finds the best colors for the mouse pointer
- even more code moved out of individual drivers and into shared libfb_gfx_dos.c
- etc...
still not all memory is locked - run at your own risk!!!

------------------------------------------------------------------------
r1167 | i_am_drv | 2005-06-08 16:01:52 -0700 (Wed, 08 Jun 2005) | 2 lines

renamed gfx_dos.h to fb_gfx_dos.h

------------------------------------------------------------------------
r1166 | i_am_drv | 2005-06-08 16:00:57 -0700 (Wed, 08 Jun 2005) | 3 lines

mostly-working dos gfxlib for 320x200x8 and 320x240x8
WARNING: do not run code as-is; missing inkey$ support and not all memory used in interrupt handlers is locked - could result in crashes if page fault occurs

------------------------------------------------------------------------
r1165 | i_am_drv | 2005-06-08 15:56:38 -0700 (Wed, 08 Jun 2005) | 2 lines

no-op DOS joystick stub

------------------------------------------------------------------------
r1164 | lillo | 2005-06-08 06:58:40 -0700 (Wed, 08 Jun 2005) | 2 lines

Clarified a bit the num_pages parameter for SCREEN

------------------------------------------------------------------------
r1163 | lillo | 2005-06-08 06:14:42 -0700 (Wed, 08 Jun 2005) | 2 lines

Some fixes to VIEW in gfx mode

------------------------------------------------------------------------
r1162 | v1ctor | 2005-06-07 11:27:25 -0700 (Tue, 07 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1161 | v1ctor | 2005-06-07 11:19:06 -0700 (Tue, 07 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1160 | lillo | 2005-06-06 02:50:16 -0700 (Mon, 06 Jun 2005) | 2 lines

Force profile.txt creation in exe directory

------------------------------------------------------------------------
r1159 | v1ctor | 2005-06-05 11:43:15 -0700 (Sun, 05 Jun 2005) | 2 lines

fixed: the arguments order vas inverted when finding the closest overloaded function

------------------------------------------------------------------------
r1158 | i_am_drv | 2005-06-03 19:36:33 -0700 (Fri, 03 Jun 2005) | 2 lines

commented out check for 'ifneq ($(TARGET),$(HOST))' - this makes the DOS build cross-compiling from Win32 not work, and even with an explicit -target win32, it works building the Win32 fbc

------------------------------------------------------------------------
r1157 | v1ctor | 2005-06-03 11:27:39 -0700 (Fri, 03 Jun 2005) | 2 lines

fixed: yet another print using quirks

------------------------------------------------------------------------
r1156 | v1ctor | 2005-06-03 11:22:31 -0700 (Fri, 03 Jun 2005) | 2 lines

yet another print using quirks

------------------------------------------------------------------------
r1155 | lillo | 2005-06-03 11:21:20 -0700 (Fri, 03 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1154 | lillo | 2005-06-03 11:10:55 -0700 (Fri, 03 Jun 2005) | 2 lines

Added refresh rate retrieving to SCREENINFO

------------------------------------------------------------------------
r1153 | lillo | 2005-06-03 05:41:48 -0700 (Fri, 03 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1152 | lillo | 2005-06-03 05:31:18 -0700 (Fri, 03 Jun 2005) | 2 lines

Delete string if temp

------------------------------------------------------------------------
r1151 | lillo | 2005-06-03 05:02:58 -0700 (Fri, 03 Jun 2005) | 2 lines

Reworked SCREENINFO support

------------------------------------------------------------------------
r1150 | v1ctor | 2005-06-02 12:26:22 -0700 (Thu, 02 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1149 | v1ctor | 2005-06-02 12:25:23 -0700 (Thu, 02 Jun 2005) | 2 lines

fixed: LINE INPUT should allow only strings as argument

------------------------------------------------------------------------
r1148 | lillo | 2005-06-02 02:10:14 -0700 (Thu, 02 Jun 2005) | 2 lines

Made OpenGL on Win32 single-threaded

------------------------------------------------------------------------
r1147 | v1ctor | 2005-06-01 18:07:44 -0700 (Wed, 01 Jun 2005) | 2 lines

changed: pointer arguments can now be overloaded

------------------------------------------------------------------------
r1146 | v1ctor | 2005-06-01 18:05:46 -0700 (Wed, 01 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1145 | v1ctor | 2005-06-01 18:05:39 -0700 (Wed, 01 Jun 2005) | 2 lines

internal changes: more logical asm suffixes for variables

------------------------------------------------------------------------
r1144 | v1ctor | 2005-06-01 14:12:10 -0700 (Wed, 01 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1143 | v1ctor | 2005-06-01 14:03:55 -0700 (Wed, 01 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1142 | v1ctor | 2005-06-01 14:02:52 -0700 (Wed, 01 Jun 2005) | 2 lines

fixed: null-ptr was not checked if a wrong number of dimensions was given

------------------------------------------------------------------------
r1141 | v1ctor | 2005-06-01 13:26:45 -0700 (Wed, 01 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1140 | v1ctor | 2005-06-01 13:24:52 -0700 (Wed, 01 Jun 2005) | 2 lines

fixed: byval string params passed byval to byref as any args..

------------------------------------------------------------------------
r1139 | v1ctor | 2005-06-01 13:04:51 -0700 (Wed, 01 Jun 2005) | 2 lines

added: missing byref's

------------------------------------------------------------------------
r1138 | v1ctor | 2005-06-01 12:31:37 -0700 (Wed, 01 Jun 2005) | 3 lines

fixed: - ON expr GOSUB would crash because the jump-table was not skipped as with GOTO;
- strptr(lit string) when used to initialize static or module-level variables would crash the compiler;

------------------------------------------------------------------------
r1137 | v1ctor | 2005-06-01 12:30:02 -0700 (Wed, 01 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1136 | v1ctor | 2005-06-01 12:29:25 -0700 (Wed, 01 Jun 2005) | 2 lines

added: the DEBUG option

------------------------------------------------------------------------
r1135 | lillo | 2005-06-01 08:30:38 -0700 (Wed, 01 Jun 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1134 | lillo | 2005-06-01 08:30:14 -0700 (Wed, 01 Jun 2005) | 2 lines

don't produce memory leaks on error

------------------------------------------------------------------------
r1133 | lillo | 2005-06-01 08:24:07 -0700 (Wed, 01 Jun 2005) | 2 lines

Added SCREENLIST win32 implementation

------------------------------------------------------------------------
r1132 | lillo | 2005-05-31 08:17:35 -0700 (Tue, 31 May 2005) | 2 lines

fixed 2/16 colors BMP loading via BLOAD

------------------------------------------------------------------------
r1131 | lillo | 2005-05-31 06:32:46 -0700 (Tue, 31 May 2005) | 2 lines

we do not support loading 1 or 4 bit BMPs

------------------------------------------------------------------------
r1130 | lillo | 2005-05-31 03:11:33 -0700 (Tue, 31 May 2005) | 2 lines

small change

------------------------------------------------------------------------
r1129 | lillo | 2005-05-31 03:09:23 -0700 (Tue, 31 May 2005) | 2 lines

Added SCREENLIST function to fetch available video modes

------------------------------------------------------------------------
r1128 | lillo | 2005-05-31 01:06:22 -0700 (Tue, 31 May 2005) | 2 lines

Wops, forgot to add these two files yesterday

------------------------------------------------------------------------
r1127 | v1ctor | 2005-05-30 19:43:08 -0700 (Mon, 30 May 2005) | 2 lines

fix for bug 1211699

------------------------------------------------------------------------
r1126 | lillo | 2005-05-30 08:24:32 -0700 (Mon, 30 May 2005) | 7 lines

- PALETTE now supports the new "PALETTE GET" construct to get instead of
set current palette colors. PALETTE GET USING is supported too.
- Added SCREENSYNC statement to wait for vertical blank synchronization.
These two additions should completely remove the need to remember the
old INP/OUT/WAIT methods for these tasks that are nonetheless still
emulated in FB...

------------------------------------------------------------------------
r1125 | lillo | 2005-05-30 03:29:41 -0700 (Mon, 30 May 2005) | 2 lines

Small fixes to make tuts to run on Linux

------------------------------------------------------------------------
r1124 | v1ctor | 2005-05-29 22:16:20 -0700 (Sun, 29 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1123 | v1ctor | 2005-05-29 22:03:03 -0700 (Sun, 29 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1122 | lillo | 2005-05-29 04:30:06 -0700 (Sun, 29 May 2005) | 2 lines

small update

------------------------------------------------------------------------
r1121 | lillo | 2005-05-29 04:23:25 -0700 (Sun, 29 May 2005) | 2 lines

Small fix

------------------------------------------------------------------------
r1120 | lillo | 2005-05-29 04:03:38 -0700 (Sun, 29 May 2005) | 2 lines

Made SCREENLOCK/UNLOCK safe to be used with gfx primitives

------------------------------------------------------------------------
r1119 | i_am_drv | 2005-05-28 20:02:49 -0700 (Sat, 28 May 2005) | 2 lines

untested port of JPGalleg header; needs testing/examples

------------------------------------------------------------------------
r1118 | lillo | 2005-05-28 06:39:40 -0700 (Sat, 28 May 2005) | 2 lines

Small fix

------------------------------------------------------------------------
r1117 | lillo | 2005-05-28 03:22:28 -0700 (Sat, 28 May 2005) | 2 lines

Force needed libs linking if header included

------------------------------------------------------------------------
r1116 | v1ctor | 2005-05-27 23:44:08 -0700 (Fri, 27 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1115 | v1ctor | 2005-05-27 23:05:38 -0700 (Fri, 27 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1114 | v1ctor | 2005-05-27 23:04:28 -0700 (Fri, 27 May 2005) | 2 lines

changed: field arrays of UDT's are now padded to follow GCC

------------------------------------------------------------------------
r1113 | v1ctor | 2005-05-27 23:03:29 -0700 (Fri, 27 May 2005) | 2 lines

fixed: array fields declared as pointers

------------------------------------------------------------------------
r1112 | v1ctor | 2005-05-27 22:43:32 -0700 (Fri, 27 May 2005) | 2 lines

fixed: array fields declared as pointer

------------------------------------------------------------------------
r1111 | v1ctor | 2005-05-27 19:39:49 -0700 (Fri, 27 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1110 | v1ctor | 2005-05-27 19:35:19 -0700 (Fri, 27 May 2005) | 2 lines

fixed: auxiliary regs were being freed even if the main regs weren't

------------------------------------------------------------------------
r1109 | v1ctor | 2005-05-25 21:21:50 -0700 (Wed, 25 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1108 | v1ctor | 2005-05-25 15:31:26 -0700 (Wed, 25 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1107 | lillo | 2005-05-25 03:27:39 -0700 (Wed, 25 May 2005) | 2 lines

Profiler public routines now use stdcall convention for consistency. Added strcmp4 to speed up string comparation. Fixed a problem with profiler and emit.

------------------------------------------------------------------------
r1106 | v1ctor | 2005-05-23 14:38:49 -0700 (Mon, 23 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1105 | v1ctor | 2005-05-23 14:31:17 -0700 (Mon, 23 May 2005) | 2 lines

fixed: the null-term was not being swapped or set, causing problems with fix-len strings

------------------------------------------------------------------------
r1104 | lillo | 2005-05-23 13:36:09 -0700 (Mon, 23 May 2005) | 2 lines

Style fixes

------------------------------------------------------------------------
r1103 | lillo | 2005-05-23 13:25:53 -0700 (Mon, 23 May 2005) | 2 lines

ops

------------------------------------------------------------------------
r1102 | lillo | 2005-05-23 13:22:50 -0700 (Mon, 23 May 2005) | 2 lines

Finished profiler rewrite

------------------------------------------------------------------------
r1101 | v1ctor | 2005-05-22 22:55:03 -0700 (Sun, 22 May 2005) | 2 lines

dup-definitions..

------------------------------------------------------------------------
r1100 | lillo | 2005-05-22 06:08:11 -0700 (Sun, 22 May 2005) | 2 lines

Forgot to add this

------------------------------------------------------------------------
r1099 | lillo | 2005-05-22 06:06:57 -0700 (Sun, 22 May 2005) | 2 lines

wops! reverting change

------------------------------------------------------------------------
r1098 | lillo | 2005-05-22 06:03:30 -0700 (Sun, 22 May 2005) | 5 lines

- New profiler code (work in progress...)
- Fixed bug for when STEP is passed to gfx primitives accepting only one pair of coordinates
- Fixed bug in WIDTH gfx hook when either one of width or height was not specified
- Added IMAGECREATE/IMAGEDESTROY to create/destroy gfx image buffers

------------------------------------------------------------------------
r1097 | v1ctor | 2005-05-21 13:08:40 -0700 (Sat, 21 May 2005) | 2 lines

fixes for 1206239 (endless QB quirks)

------------------------------------------------------------------------
r1096 | v1ctor | 2005-05-20 21:24:12 -0700 (Fri, 20 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1095 | v1ctor | 2005-05-18 22:56:09 -0700 (Wed, 18 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1094 | v1ctor | 2005-05-18 14:41:01 -0700 (Wed, 18 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1093 | v1ctor | 2005-05-18 14:26:04 -0700 (Wed, 18 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1092 | v1ctor | 2005-05-18 14:21:00 -0700 (Wed, 18 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1091 | v1ctor | 2005-05-18 14:05:00 -0700 (Wed, 18 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1090 | v1ctor | 2005-05-18 10:39:53 -0700 (Wed, 18 May 2005) | 4 lines

fixed:
- uinteger and pointer args were seen as the same when overloading;
- UDT's, ENUM's and TYPEDEF's with the same name were being allowed;

------------------------------------------------------------------------
r1089 | lillo | 2005-05-18 02:23:10 -0700 (Wed, 18 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1088 | v1ctor | 2005-05-17 17:47:35 -0700 (Tue, 17 May 2005) | 2 lines

internal changes: moved the debug stuff out of parser1

------------------------------------------------------------------------
r1087 | v1ctor | 2005-05-17 17:44:47 -0700 (Tue, 17 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1086 | v1ctor | 2005-05-17 13:18:25 -0700 (Tue, 17 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1085 | v1ctor | 2005-05-17 13:15:58 -0700 (Tue, 17 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1084 | v1ctor | 2005-05-16 08:34:30 -0700 (Mon, 16 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1083 | lillo | 2005-05-16 04:15:40 -0700 (Mon, 16 May 2005) | 2 lines

Small bugfix: check for passed pointers before arrays to avoid bad code generation in case of array of pointers

------------------------------------------------------------------------
r1082 | lillo | 2005-05-16 02:58:20 -0700 (Mon, 16 May 2005) | 2 lines

Updated keywords a bit

------------------------------------------------------------------------
r1081 | lillo | 2005-05-16 01:55:23 -0700 (Mon, 16 May 2005) | 2 lines

Made temp symbols invisible also under Linux

------------------------------------------------------------------------
r1080 | v1ctor | 2005-05-15 14:55:41 -0700 (Sun, 15 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1079 | lillo | 2005-05-15 06:08:15 -0700 (Sun, 15 May 2005) | 2 lines

Removed .arch specification as this prevents inlined MMX even if i686 is specified

------------------------------------------------------------------------
r1078 | lillo | 2005-05-15 06:07:06 -0700 (Sun, 15 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1077 | v1ctor | 2005-05-14 18:01:30 -0700 (Sat, 14 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1076 | v1ctor | 2005-05-14 17:58:12 -0700 (Sat, 14 May 2005) | 2 lines

changed: all constants prefixed with CGUI_

------------------------------------------------------------------------
r1075 | v1ctor | 2005-05-14 17:49:26 -0700 (Sat, 14 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1074 | v1ctor | 2005-05-14 17:49:12 -0700 (Sat, 14 May 2005) | 2 lines

minor changes..

------------------------------------------------------------------------
r1073 | lillo | 2005-05-14 08:38:22 -0700 (Sat, 14 May 2005) | 2 lines

Now using a default color special value less likely to conflict with user specified color

------------------------------------------------------------------------
r1072 | lillo | 2005-05-14 03:32:15 -0700 (Sat, 14 May 2005) | 2 lines

Added custom blenders support to PUT

------------------------------------------------------------------------
r1071 | lillo | 2005-05-13 15:10:46 -0700 (Fri, 13 May 2005) | 2 lines

Fixed bug with PUT ALPHA mode; added uniform alpha blending support. Easy translucent sprites drawing, here I come!

------------------------------------------------------------------------
r1070 | v1ctor | 2005-05-13 13:29:26 -0700 (Fri, 13 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1069 | v1ctor | 2005-05-13 13:26:22 -0700 (Fri, 13 May 2005) | 2 lines

added: better pointer checking

------------------------------------------------------------------------
r1068 | v1ctor | 2005-05-13 11:47:36 -0700 (Fri, 13 May 2005) | 2 lines

bug fix for 1201127

------------------------------------------------------------------------
r1067 | lillo | 2005-05-12 06:04:50 -0700 (Thu, 12 May 2005) | 2 lines

Added BMP saving capabilities to BSAVE

------------------------------------------------------------------------
r1066 | v1ctor | 2005-05-10 13:14:50 -0700 (Tue, 10 May 2005) | 2 lines

more debugging support..

------------------------------------------------------------------------
r1065 | lillo | 2005-05-10 07:51:22 -0700 (Tue, 10 May 2005) | 2 lines

Added -noexport command-line option

------------------------------------------------------------------------
r1064 | lillo | 2005-05-10 06:15:52 -0700 (Tue, 10 May 2005) | 4 lines

- Reduced initialized variables to reduce EXE size under Linux
- Fixed bad colors output while under Eterm
- Added support for 16 background colors if using Xterm

------------------------------------------------------------------------
r1063 | v1ctor | 2005-05-08 18:24:40 -0700 (Sun, 08 May 2005) | 2 lines

fixed: missing expressions errors were being silently skipped

------------------------------------------------------------------------
r1062 | i_am_drv | 2005-05-08 11:17:11 -0700 (Sun, 08 May 2005) | 2 lines

added empty function to get length of fb_hPostKey for DOS keyboard driver in order to lock it into physical memory so it can't be paged out during an interrupt

------------------------------------------------------------------------
r1061 | v1ctor | 2005-05-07 21:54:58 -0700 (Sat, 07 May 2005) | 2 lines

added: more debugging improvements..

------------------------------------------------------------------------
r1060 | v1ctor | 2005-05-07 15:01:34 -0700 (Sat, 07 May 2005) | 2 lines

added: new debug features..

------------------------------------------------------------------------
r1059 | v1ctor | 2005-05-06 22:39:43 -0700 (Fri, 06 May 2005) | 2 lines

cosmetic changes..

------------------------------------------------------------------------
r1058 | v1ctor | 2005-05-06 22:38:22 -0700 (Fri, 06 May 2005) | 2 lines

fixed: external static arrays had no descriptor emited, making them impossible to be passed by descriptor to functions

------------------------------------------------------------------------
r1057 | v1ctor | 2005-05-06 12:59:34 -0700 (Fri, 06 May 2005) | 2 lines

small check added to FB_STRSETUP..

------------------------------------------------------------------------
r1056 | v1ctor | 2005-05-06 12:59:04 -0700 (Fri, 06 May 2005) | 2 lines

fixed: print/writeVoid() + padding

------------------------------------------------------------------------
r1055 | v1ctor | 2005-05-06 12:09:20 -0700 (Fri, 06 May 2005) | 2 lines

makeTmpStr() tweaked again..

------------------------------------------------------------------------
r1054 | v1ctor | 2005-05-06 12:06:24 -0700 (Fri, 06 May 2005) | 2 lines

small fix..

------------------------------------------------------------------------
r1053 | v1ctor | 2005-05-05 21:19:10 -0700 (Thu, 05 May 2005) | 2 lines

emitdbg renamed to edbg_stab, so other formats could be added later..

------------------------------------------------------------------------
r1052 | v1ctor | 2005-05-05 21:17:53 -0700 (Thu, 05 May 2005) | 3 lines

added: support for UDT's to debug symbols
changed: emitdbg renamed to edbg_stab, so other formats could be added later..

------------------------------------------------------------------------
r1051 | v1ctor | 2005-05-05 15:33:25 -0700 (Thu, 05 May 2005) | 8 lines

fixed:
- alloctype was not being set in parser5 when "option private" was used;
- overloaded functions mangling was adding "@" twice in Win32;

internal changes:
- speeded up makeTmpStr() a bit;
- temp names now start with a "L" so GAS won't add them to the symbol table when --strip-local is used;

------------------------------------------------------------------------
r1050 | lillo | 2005-05-05 02:08:23 -0700 (Thu, 05 May 2005) | 2 lines

small fix

------------------------------------------------------------------------
r1049 | lillo | 2005-05-05 01:59:40 -0700 (Thu, 05 May 2005) | 2 lines

linux host autodetection

------------------------------------------------------------------------
r1048 | v1ctor | 2005-05-04 23:42:53 -0700 (Wed, 04 May 2005) | 2 lines

added: better cross-compilation support

------------------------------------------------------------------------
r1047 | v1ctor | 2005-05-04 23:28:07 -0700 (Wed, 04 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1046 | v1ctor | 2005-05-04 23:22:12 -0700 (Wed, 04 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1045 | v1ctor | 2005-05-04 23:12:44 -0700 (Wed, 04 May 2005) | 2 lines

fixed: argc & argv aren't passed in Linux the same way as in DOS..

------------------------------------------------------------------------
r1044 | v1ctor | 2005-05-04 21:55:37 -0700 (Wed, 04 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1043 | v1ctor | 2005-05-04 21:50:51 -0700 (Wed, 04 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1042 | v1ctor | 2005-05-04 21:50:47 -0700 (Wed, 04 May 2005) | 2 lines

added: better cross-compilation support

------------------------------------------------------------------------
r1041 | v1ctor | 2005-05-04 21:42:38 -0700 (Wed, 04 May 2005) | 2 lines

added: better cross-compilation support

------------------------------------------------------------------------
r1040 | v1ctor | 2005-05-04 19:22:29 -0700 (Wed, 04 May 2005) | 2 lines

small fixes..

------------------------------------------------------------------------
r1039 | v1ctor | 2005-05-04 19:14:57 -0700 (Wed, 04 May 2005) | 2 lines

added: better cross-compilation support

------------------------------------------------------------------------
r1038 | v1ctor | 2005-05-04 18:44:13 -0700 (Wed, 04 May 2005) | 2 lines

small fix..

------------------------------------------------------------------------
r1037 | v1ctor | 2005-05-04 14:52:12 -0700 (Wed, 04 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1036 | v1ctor | 2005-05-04 14:50:37 -0700 (Wed, 04 May 2005) | 2 lines

internal changes: fbc module was split in platform dependent versions

------------------------------------------------------------------------
r1035 | v1ctor | 2005-05-04 13:53:08 -0700 (Wed, 04 May 2005) | 2 lines

fixed: zero-len strings bug

------------------------------------------------------------------------
r1034 | v1ctor | 2005-05-04 10:09:47 -0700 (Wed, 04 May 2005) | 2 lines

fixed: ',' when used with ENUM's

------------------------------------------------------------------------
r1033 | i_am_drv | 2005-05-03 19:37:36 -0700 (Tue, 03 May 2005) | 2 lines

yet another fix for stupid mistakes on my part...

------------------------------------------------------------------------
r1032 | i_am_drv | 2005-05-03 19:34:50 -0700 (Tue, 03 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1031 | i_am_drv | 2005-05-03 19:34:16 -0700 (Tue, 03 May 2005) | 2 lines

changed del to rm in clean target for DOS (built into make)

------------------------------------------------------------------------
r1030 | v1ctor | 2005-05-03 13:57:57 -0700 (Tue, 03 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1029 | v1ctor | 2005-05-03 13:57:46 -0700 (Tue, 03 May 2005) | 4 lines

changed:
- allocTempStr() was split in multiple sources (zstrings and fixed-len are "seen" as different types now);
- PutStr() and GetStr() arguments had to be modified because allocTempDesc();

------------------------------------------------------------------------
r1028 | v1ctor | 2005-05-03 13:55:44 -0700 (Tue, 03 May 2005) | 2 lines

fixed: DIM and REDIM were not checking for invalid types as dimensions

------------------------------------------------------------------------
r1027 | v1ctor | 2005-05-03 13:54:42 -0700 (Tue, 03 May 2005) | 2 lines

fixed: the unary operator "+" was been allowed with string operands

------------------------------------------------------------------------
r1026 | v1ctor | 2005-05-03 13:54:16 -0700 (Tue, 03 May 2005) | 2 lines

small bug fix..

------------------------------------------------------------------------
r1025 | v1ctor | 2005-05-03 13:54:03 -0700 (Tue, 03 May 2005) | 2 lines

changed: as AST will now optimize multiple strings concatenations, EMIT can be much less clumsy..

------------------------------------------------------------------------
r1024 | v1ctor | 2005-05-03 13:52:27 -0700 (Tue, 03 May 2005) | 2 lines

added: multiple strings concatenation optimization

------------------------------------------------------------------------
r1023 | v1ctor | 2005-05-03 13:51:11 -0700 (Tue, 03 May 2005) | 2 lines

changed: allocTempStr() split in multiple sources (zstrings and fixed-len are "seen" as different types now)

------------------------------------------------------------------------
r1022 | v1ctor | 2005-05-03 13:49:17 -0700 (Tue, 03 May 2005) | 2 lines

changed: PutStr() and GetStr() arguments had to be modified because allocTempDesc() was changed too.. these new sources won't work with fbc bellow the 0.14 may/3 release

------------------------------------------------------------------------
r1021 | v1ctor | 2005-05-02 13:34:42 -0700 (Mon, 02 May 2005) | 2 lines

internal changes, zstring speed ups..

------------------------------------------------------------------------
r1020 | v1ctor | 2005-05-01 21:40:45 -0700 (Sun, 01 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1019 | v1ctor | 2005-05-01 21:36:21 -0700 (Sun, 01 May 2005) | 10 lines

changed: argc & argv are now passed to fb_init();
added: the -target option to the Win32 version;

note: to get these sources to recompile well with 0.14, do this:
1) compile using the current fbc and rtlib
2) update the rtlib and rebuild it
3) delete the old fbc.exe and rename fbc_new.exe to fbc.exe
4) rebuild the compiler (make clean followed by make), using the new one
5) repeat the step 3 again

------------------------------------------------------------------------
r1018 | v1ctor | 2005-05-01 21:30:58 -0700 (Sun, 01 May 2005) | 2 lines

changed: argc and argv are now passed to fb_init()

------------------------------------------------------------------------
r1017 | v1ctor | 2005-05-01 21:29:21 -0700 (Sun, 01 May 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1016 | v1ctor | 2005-04-30 13:00:18 -0700 (Sat, 30 Apr 2005) | 2 lines

"key" redeclared as an external array

------------------------------------------------------------------------
r1015 | v1ctor | 2005-04-30 12:50:56 -0700 (Sat, 30 Apr 2005) | 2 lines

reverting the fix..

------------------------------------------------------------------------
r1014 | v1ctor | 2005-04-30 12:10:56 -0700 (Sat, 30 Apr 2005) | 2 lines

small fix..

------------------------------------------------------------------------
r1013 | v1ctor | 2005-04-30 11:59:33 -0700 (Sat, 30 Apr 2005) | 3 lines

changed: couldn't get POW inlined to work, reverting to crt's implementation;
fixed: floor() should set the FPU CW to round down, not truncate;

------------------------------------------------------------------------
r1012 | v1ctor | 2005-04-30 11:57:57 -0700 (Sat, 30 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1011 | v1ctor | 2005-04-30 08:55:19 -0700 (Sat, 30 Apr 2005) | 2 lines

bug fix 1193019

------------------------------------------------------------------------
r1010 | v1ctor | 2005-04-29 18:35:36 -0700 (Fri, 29 Apr 2005) | 2 lines

dated and no more supported

------------------------------------------------------------------------
r1009 | v1ctor | 2005-04-29 12:35:01 -0700 (Fri, 29 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1008 | v1ctor | 2005-04-29 12:34:39 -0700 (Fri, 29 Apr 2005) | 2 lines

changed: INT() (floor()) implemented as inline now

------------------------------------------------------------------------
r1007 | v1ctor | 2005-04-29 11:41:57 -0700 (Fri, 29 Apr 2005) | 2 lines

some pow() changes as it's inline now..

------------------------------------------------------------------------
r1006 | lillo | 2005-04-29 06:49:21 -0700 (Fri, 29 Apr 2005) | 2 lines

Made x^y (POW) inlined too, speeded up PUT for PSET/PRESET/AND/OR/XOR drawing modes

------------------------------------------------------------------------
r1005 | lillo | 2005-04-29 02:23:50 -0700 (Fri, 29 Apr 2005) | 2 lines

Do not reset mouse wheel position when window is out of focus

------------------------------------------------------------------------
r1004 | v1ctor | 2005-04-28 23:40:03 -0700 (Thu, 28 Apr 2005) | 2 lines

small fix..

------------------------------------------------------------------------
r1003 | v1ctor | 2005-04-28 22:13:05 -0700 (Thu, 28 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1002 | v1ctor | 2005-04-28 22:10:15 -0700 (Thu, 28 Apr 2005) | 2 lines

changed: SIN, ASIN, COS, ACOS, TAN, ATN, SQR, LOG, ATAN2 functions are now implemented as inline

------------------------------------------------------------------------
r1001 | v1ctor | 2005-04-28 20:09:22 -0700 (Thu, 28 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r1000 | v1ctor | 2005-04-28 20:09:05 -0700 (Thu, 28 Apr 2005) | 2 lines

fixed: SWAP wasn't working with type fields

------------------------------------------------------------------------
r999 | v1ctor | 2005-04-28 14:38:17 -0700 (Thu, 28 Apr 2005) | 6 lines

fixed:
- profilling is working again, forgot to move the bsave and bload declarations at rtl.bas;

chaged:
- address of (@) won't set the data type as before to "help" DerefExpression, as that could cause problems with byval @ when used with string params;

------------------------------------------------------------------------
r998 | v1ctor | 2005-04-28 09:13:53 -0700 (Thu, 28 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r997 | v1ctor | 2005-04-28 09:13:32 -0700 (Thu, 28 Apr 2005) | 2 lines

fixed: token's sym field wasn't been reseted, causing pointer expressions to fail if used as parameters to functions

------------------------------------------------------------------------
r996 | v1ctor | 2005-04-28 08:23:00 -0700 (Thu, 28 Apr 2005) | 3 lines

changed: better macro expansion;
added: FUNCTION in asm blocks;

------------------------------------------------------------------------
r995 | v1ctor | 2005-04-28 08:17:04 -0700 (Thu, 28 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r994 | i_am_drv | 2005-04-27 19:41:02 -0700 (Wed, 27 Apr 2005) | 3 lines

- added Mode X (320x240x8) driver
- still much missing functionality

------------------------------------------------------------------------
r993 | i_am_drv | 2005-04-27 19:19:59 -0700 (Wed, 27 Apr 2005) | 2 lines

added rule to build assembler source files in OS-dependent directory

------------------------------------------------------------------------
r992 | v1ctor | 2005-04-27 15:52:10 -0700 (Wed, 27 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r991 | v1ctor | 2005-04-27 15:50:26 -0700 (Wed, 27 Apr 2005) | 2 lines

changed: to pass integer values to byval string args now the BYVAL modifier must be used

------------------------------------------------------------------------
r990 | v1ctor | 2005-04-27 15:48:32 -0700 (Wed, 27 Apr 2005) | 2 lines

internal changes: irGetDistance() algo efficiency changed from O(n) to O(1)

------------------------------------------------------------------------
r989 | v1ctor | 2005-04-27 15:44:54 -0700 (Wed, 27 Apr 2005) | 2 lines

internal changes: most dynamic strings converted to static zstring's in parser and symbol modules, emit will be the next..

------------------------------------------------------------------------
r988 | v1ctor | 2005-04-27 14:59:07 -0700 (Wed, 27 Apr 2005) | 2 lines

faster null-assigns for fixed-/z-strings

------------------------------------------------------------------------
r987 | lillo | 2005-04-27 09:09:49 -0700 (Wed, 27 Apr 2005) | 2 lines

Use read only data section for const data

------------------------------------------------------------------------
r986 | lillo | 2005-04-27 07:37:36 -0700 (Wed, 27 Apr 2005) | 3 lines

- Reworked RTlib hooks subsystem. No more hook setter functions, now uses a global hooks table; with this system now programs using Gfxlib link with all gfx hooks and only to those console hooks actually used, instead of linking with all console and all gfx hooks anyways. This reduces EXE size.
- Documented the RGBA built-in macro

------------------------------------------------------------------------
r985 | lillo | 2005-04-27 03:12:53 -0700 (Wed, 27 Apr 2005) | 3 lines

- Now using "-O2 -ffast-math" instead of "-O3" to compile both rtlib and gfxlib. Cons: no loops unrolling. Pros: more stable kind of optimization, producing working code also on old gcc versions (some old gcc's produced bad code with -O3); EXE size of FB apps is reduced by about 3-4K, with almost no noticeable performance loss.
- Made rtlib files table to be inited at fb_Init() time instead of at compile time, thus reducing EXEs size by another 5-6K.

------------------------------------------------------------------------
r984 | lillo | 2005-04-26 06:39:11 -0700 (Tue, 26 Apr 2005) | 2 lines

Implemented GETJOYSTICK for Linux and documented it.

------------------------------------------------------------------------
r983 | lillo | 2005-04-26 01:29:56 -0700 (Tue, 26 Apr 2005) | 2 lines

Removed warning under Linux

------------------------------------------------------------------------
r982 | v1ctor | 2005-04-25 11:34:26 -0700 (Mon, 25 Apr 2005) | 2 lines

major internal changes: AST nodes are now pointers, no shared arrays anymore..

------------------------------------------------------------------------
r981 | v1ctor | 2005-04-24 21:15:22 -0700 (Sun, 24 Apr 2005) | 2 lines

major changes: IR module using pointers now, no more dynamic arrays and indexes..

------------------------------------------------------------------------
r980 | v1ctor | 2005-04-24 21:04:59 -0700 (Sun, 24 Apr 2005) | 2 lines

function overloading was failing if pointers were processed (subtype shouldn't be checked)

------------------------------------------------------------------------
r979 | v1ctor | 2005-04-24 16:36:45 -0700 (Sun, 24 Apr 2005) | 2 lines

changed: actually gcc will convert \n to cr+lf, nevermind..

------------------------------------------------------------------------
r978 | v1ctor | 2005-04-24 16:23:19 -0700 (Sun, 24 Apr 2005) | 2 lines

changed: carriage-return added to PRINT and WRITE on anything but Linux

------------------------------------------------------------------------
r977 | v1ctor | 2005-04-24 16:22:40 -0700 (Sun, 24 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r976 | v1ctor | 2005-04-24 12:01:47 -0700 (Sun, 24 Apr 2005) | 2 lines

getjoystick decl was missing the overloaded option

------------------------------------------------------------------------
r975 | i_am_drv | 2005-04-23 21:41:29 -0700 (Sat, 23 Apr 2005) | 2 lines

partially working SCREEN 13 - see libfb_gfx_dos.c for more info

------------------------------------------------------------------------
r974 | v1ctor | 2005-04-23 12:40:28 -0700 (Sat, 23 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r973 | v1ctor | 2005-04-23 12:37:16 -0700 (Sat, 23 Apr 2005) | 2 lines

changed: INSTR, HEX, BIN, OCT redeclared using function overloading (this file will be removed soon)

------------------------------------------------------------------------
r972 | v1ctor | 2005-04-23 12:36:42 -0700 (Sat, 23 Apr 2005) | 2 lines

changed: INSTR, HEX, BIN, OCT redeclared using function overloading

------------------------------------------------------------------------
r971 | v1ctor | 2005-04-23 12:36:03 -0700 (Sat, 23 Apr 2005) | 3 lines

updated: function overloading and debugging support
fixed: the wrong param number was been shown by parser5 when an error occurred

------------------------------------------------------------------------
r970 | v1ctor | 2005-04-23 12:33:28 -0700 (Sat, 23 Apr 2005) | 2 lines

initial variables support added (no user def types yet)

------------------------------------------------------------------------
r969 | lillo | 2005-04-23 10:13:33 -0700 (Sat, 23 Apr 2005) | 4 lines

- Added joystick support via GETJOYSTICK function. New example in examples/gfx/fbgfx_input.bas
- Fixed bug in DRAW parsing when directed towards an offscreen GET/PUT buffer
- GETMOUSE/SETMOUSE now can be used as functions and they can also trigger runtime errors

------------------------------------------------------------------------
r968 | v1ctor | 2005-04-21 15:10:48 -0700 (Thu, 21 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r967 | v1ctor | 2005-04-21 14:59:38 -0700 (Thu, 21 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r966 | v1ctor | 2005-04-21 12:11:01 -0700 (Thu, 21 Apr 2005) | 2 lines

.BLOAD and .BSAVE were not removed from RTL's enum list..

------------------------------------------------------------------------
r965 | v1ctor | 2005-04-21 11:54:55 -0700 (Thu, 21 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r964 | v1ctor | 2005-04-21 11:53:44 -0700 (Thu, 21 Apr 2005) | 2 lines

added: function overloading

------------------------------------------------------------------------
r963 | v1ctor | 2005-04-21 11:45:45 -0700 (Thu, 21 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r962 | v1ctor | 2005-04-21 11:22:49 -0700 (Thu, 21 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r961 | v1ctor | 2005-04-21 11:19:21 -0700 (Thu, 21 Apr 2005) | 2 lines

cvshort() and family added..

------------------------------------------------------------------------
r960 | v1ctor | 2005-04-20 11:28:51 -0700 (Wed, 20 Apr 2005) | 2 lines

special callbacks added to rtl routines plus some clean up..

------------------------------------------------------------------------
r959 | v1ctor | 2005-04-20 11:26:34 -0700 (Wed, 20 Apr 2005) | 2 lines

callbacks added, @'s in DATA, won't compile with 0.13..

------------------------------------------------------------------------
r958 | v1ctor | 2005-04-19 21:00:49 -0700 (Tue, 19 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r957 | i_am_drv | 2005-04-19 18:19:30 -0700 (Tue, 19 Apr 2005) | 3 lines

fixed all issued warnings;
some known bugs still exist in PRINT and related rtlib routines

------------------------------------------------------------------------
r956 | i_am_drv | 2005-04-19 17:18:42 -0700 (Tue, 19 Apr 2005) | 2 lines

v1ctor->english

------------------------------------------------------------------------
r955 | v1ctor | 2005-04-18 22:27:45 -0700 (Mon, 18 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r954 | v1ctor | 2005-04-18 22:13:58 -0700 (Mon, 18 Apr 2005) | 2 lines

added proper support for @'s used in DATA's

------------------------------------------------------------------------
r953 | v1ctor | 2005-04-18 22:13:53 -0700 (Mon, 18 Apr 2005) | 2 lines

fix for @'s used in DATA's (warning: next commit will have @'s in DATA's, this version will be needed to proper rebuild the compiler)

------------------------------------------------------------------------
r952 | i_am_drv | 2005-04-18 19:05:05 -0700 (Mon, 18 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r951 | v1ctor | 2005-04-18 15:44:16 -0700 (Mon, 18 Apr 2005) | 2 lines

cleaned emitdbg a bit..

------------------------------------------------------------------------
r950 | lillo | 2005-04-18 06:51:22 -0700 (Mon, 18 Apr 2005) | 2 lines

Ops, reverting unwanted change

------------------------------------------------------------------------
r949 | lillo | 2005-04-18 06:49:50 -0700 (Mon, 18 Apr 2005) | 5 lines

- More thread-safetiness in platform dependent routines
- Made SHELL command into its own rtlib routine. Under Linux, this is
needed as we need to save/restore console state around subprocess call,
to let keyboard I/O to work in the subprocess

------------------------------------------------------------------------
r948 | v1ctor | 2005-04-17 19:40:14 -0700 (Sun, 17 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r947 | v1ctor | 2005-04-17 19:40:12 -0700 (Sun, 17 Apr 2005) | 2 lines

extern palette arrays redeclared

------------------------------------------------------------------------
r946 | v1ctor | 2005-04-17 15:54:30 -0700 (Sun, 17 Apr 2005) | 2 lines

fixed: "^^" works a bit more like in QB now, part 2..

------------------------------------------------------------------------
r945 | v1ctor | 2005-04-17 15:17:27 -0700 (Sun, 17 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r944 | v1ctor | 2005-04-17 15:16:14 -0700 (Sun, 17 Apr 2005) | 2 lines

fixed: "^^" works a bit more like in QB now..

------------------------------------------------------------------------
r943 | v1ctor | 2005-04-17 14:37:35 -0700 (Sun, 17 Apr 2005) | 2 lines

fixed: total bytes should be + 1 because the null-terminator

------------------------------------------------------------------------
r942 | v1ctor | 2005-04-16 18:51:10 -0700 (Sat, 16 Apr 2005) | 2 lines

changed: Insight/GDB can now Step; added arrays support for EXTERN;

------------------------------------------------------------------------
r941 | v1ctor | 2005-04-16 10:46:15 -0700 (Sat, 16 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r940 | v1ctor | 2005-04-16 08:34:56 -0700 (Sat, 16 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r939 | v1ctor | 2005-04-16 08:31:11 -0700 (Sat, 16 Apr 2005) | 2 lines

fixed: EQV can't be optimized, because NOT doesn't set any flags on x86

------------------------------------------------------------------------
r938 | lillo | 2005-04-16 03:59:50 -0700 (Sat, 16 Apr 2005) | 2 lines

Propagate null-op optimizations

------------------------------------------------------------------------
r937 | lillo | 2005-04-16 03:31:58 -0700 (Sat, 16 Apr 2005) | 3 lines

- Added alpha blending support to PUT (use ALPHA as drawing mode)
- Signed'ness corrections all over gfxlib

------------------------------------------------------------------------
r936 | lillo | 2005-04-14 16:47:17 -0700 (Thu, 14 Apr 2005) | 2 lines

This forces a flush of the configure events on fullscreen toggling failure, otherwise programs would enter a loop

------------------------------------------------------------------------
r935 | v1ctor | 2005-04-14 14:11:11 -0700 (Thu, 14 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r934 | v1ctor | 2005-04-14 10:33:08 -0700 (Thu, 14 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r933 | lillo | 2005-04-14 10:11:58 -0700 (Thu, 14 Apr 2005) | 2 lines

Small fix

------------------------------------------------------------------------
r932 | v1ctor | 2005-04-14 10:03:26 -0700 (Thu, 14 Apr 2005) | 2 lines

NULL pointer check added to symbgetlen()

------------------------------------------------------------------------
r931 | lillo | 2005-04-14 09:58:45 -0700 (Thu, 14 Apr 2005) | 2 lines

Small fixes for profiling code generation

------------------------------------------------------------------------
r930 | lillo | 2005-04-14 08:40:36 -0700 (Thu, 14 Apr 2005) | 2 lines

Reported new X11 requirements, added new gfxlib OpenGL example by Rel

------------------------------------------------------------------------
r929 | lillo | 2005-04-14 07:39:13 -0700 (Thu, 14 Apr 2005) | 2 lines

Vsync under X11 should work now

------------------------------------------------------------------------
r928 | lillo | 2005-04-14 07:31:47 -0700 (Thu, 14 Apr 2005) | 4 lines

- GLFW example now compiles also under Linux
- All X11 gfx drivers now display a maximize window button, behaving
like under Win32 (pressing it switches to fullscreen)

------------------------------------------------------------------------
r927 | lillo | 2005-04-14 04:29:43 -0700 (Thu, 14 Apr 2005) | 2 lines

Tiny fix

------------------------------------------------------------------------
r926 | v1ctor | 2005-04-13 21:43:24 -0700 (Wed, 13 Apr 2005) | 2 lines

changed: the PROC symbol was conflicting with windef.h

------------------------------------------------------------------------
r925 | v1ctor | 2005-04-13 21:43:01 -0700 (Wed, 13 Apr 2005) | 2 lines

changed: different dirs for the mt libs

------------------------------------------------------------------------
r924 | v1ctor | 2005-04-13 19:46:41 -0700 (Wed, 13 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r923 | v1ctor | 2005-04-13 18:47:41 -0700 (Wed, 13 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r922 | v1ctor | 2005-04-13 13:37:05 -0700 (Wed, 13 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r921 | v1ctor | 2005-04-13 12:51:46 -0700 (Wed, 13 Apr 2005) | 2 lines

changed: (chicken-egg) VALINT() is now used when strings are known to be integers, as VAL() would return DOUBLE's, that would saturate unsigned int's > 2^31 when converting to integers.

------------------------------------------------------------------------
r920 | v1ctor | 2005-04-13 12:24:43 -0700 (Wed, 13 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r919 | v1ctor | 2005-04-13 12:06:14 -0700 (Wed, 13 Apr 2005) | 2 lines

small fix

------------------------------------------------------------------------
r918 | v1ctor | 2005-04-13 11:41:01 -0700 (Wed, 13 Apr 2005) | 2 lines

added: ValueConv and NewCONST for multiple types

------------------------------------------------------------------------
r917 | v1ctor | 2005-04-13 11:40:32 -0700 (Wed, 13 Apr 2005) | 2 lines

added: VALINT()

------------------------------------------------------------------------
r916 | v1ctor | 2005-04-13 11:39:04 -0700 (Wed, 13 Apr 2005) | 2 lines

changed: intial FOR..NEXT branch elimination if conditions are known at compile-time

------------------------------------------------------------------------
r915 | v1ctor | 2005-04-13 11:36:43 -0700 (Wed, 13 Apr 2005) | 2 lines

adde: VALINT(); changed: READ using str2int instead of dbl2int with integer types

------------------------------------------------------------------------
r914 | lillo | 2005-04-13 07:30:29 -0700 (Wed, 13 Apr 2005) | 7 lines

- Fixed a bug in fbgfx_flame.bas example exposed by the recent unsigned
int fixes
- Made fbc to recognize both \ and / as path separators for include/lib
files on all platforms
- PRINT in gfx mode now detects CR+LF as a single new line
- Replaced Xxf86vidmode with Xrandr for fullscreen toggling under Linux

------------------------------------------------------------------------
r913 | v1ctor | 2005-04-13 00:32:01 -0700 (Wed, 13 Apr 2005) | 2 lines

changed: type-less REDIM's will check for already allocated arrays as in VB

------------------------------------------------------------------------
r912 | v1ctor | 2005-04-13 00:30:55 -0700 (Wed, 13 Apr 2005) | 2 lines

error check added to typedef's

------------------------------------------------------------------------
r911 | v1ctor | 2005-04-13 00:23:47 -0700 (Wed, 13 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r910 | v1ctor | 2005-04-13 00:23:05 -0700 (Wed, 13 Apr 2005) | 2 lines

headers exchanged by the ones generated by SWIG FB wrapper

------------------------------------------------------------------------
r909 | v1ctor | 2005-04-13 00:08:35 -0700 (Wed, 13 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r908 | i_am_drv | 2005-04-12 19:06:48 -0700 (Tue, 12 Apr 2005) | 2 lines

allegro header split into separate files like C headers

------------------------------------------------------------------------
r907 | i_am_drv | 2005-04-12 18:58:20 -0700 (Tue, 12 Apr 2005) | 2 lines

allegro header split into separate files like C headers

------------------------------------------------------------------------
r906 | v1ctor | 2005-04-12 13:27:34 -0700 (Tue, 12 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r905 | v1ctor | 2005-04-12 12:48:45 -0700 (Tue, 12 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r904 | v1ctor | 2005-04-12 12:48:26 -0700 (Tue, 12 Apr 2005) | 2 lines

changed type-less REDIM's will check for already allocated arrays as in VB

------------------------------------------------------------------------
r903 | v1ctor | 2005-04-12 10:37:39 -0700 (Tue, 12 Apr 2005) | 2 lines

not finished, does not compile..

------------------------------------------------------------------------
r902 | v1ctor | 2005-04-12 10:32:19 -0700 (Tue, 12 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r901 | v1ctor | 2005-04-12 10:14:16 -0700 (Tue, 12 Apr 2005) | 2 lines

dim's changed to typedef..

------------------------------------------------------------------------
r900 | v1ctor | 2005-04-12 10:05:31 -0700 (Tue, 12 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r899 | v1ctor | 2005-04-12 10:01:41 -0700 (Tue, 12 Apr 2005) | 2 lines

main changes: all integers constants handling is now done using integers internally, instead of doubles as before, due the double to unsigned problems that could happen

------------------------------------------------------------------------
r898 | v1ctor | 2005-04-12 00:21:44 -0700 (Tue, 12 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r897 | v1ctor | 2005-04-11 18:24:03 -0700 (Mon, 11 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r896 | v1ctor | 2005-04-11 18:19:53 -0700 (Mon, 11 Apr 2005) | 2 lines

OpenGL examples moved..

------------------------------------------------------------------------
r895 | lillo | 2005-04-11 12:48:20 -0700 (Mon, 11 Apr 2005) | 2 lines

Fixed a couple of gfxlib bugs

------------------------------------------------------------------------
r894 | v1ctor | 2005-04-11 12:33:04 -0700 (Mon, 11 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r893 | lillo | 2005-04-11 11:15:58 -0700 (Mon, 11 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r892 | v1ctor | 2005-04-11 11:15:25 -0700 (Mon, 11 Apr 2005) | 2 lines

unsigned longint to double conversion was missing..

------------------------------------------------------------------------
r891 | lillo | 2005-04-11 11:06:14 -0700 (Mon, 11 Apr 2005) | 3 lines

Incredibly, just today I found out fb didn't optimize a=b*1 to a=b and similar, and so I worked out this optimization myself. Then I did cvs update and found out an updated TODO with that item listed, the item I actually just coded :P
So here it goes.

------------------------------------------------------------------------
r890 | v1ctor | 2005-04-11 11:05:46 -0700 (Mon, 11 Apr 2005) | 2 lines

conversion from floating-point to unsigned integers added, that can't be done directly with the FPU on x86 as FISTP will saturate "too big" values, not wrap-around..

------------------------------------------------------------------------
r889 | v1ctor | 2005-04-11 11:01:43 -0700 (Mon, 11 Apr 2005) | 2 lines

floating-point to unsigned longint added..

------------------------------------------------------------------------
r888 | v1ctor | 2005-04-10 18:37:15 -0700 (Sun, 10 Apr 2005) | 8 lines

added:
- ADDRESS-OF for simple vars/procs will become an offset node at AST, no LEA is emited, so it's now possible to use @, varptr or procptr with DATA and var-initializers;

fixed:
- emiPOP for fpoint regs was wrong (fstp instead of fld);
- IIF() could gen bad code when there are functions in the true or false expressions;
- constant prefixes (!,#,etc) were not being preserved in #define macros;

------------------------------------------------------------------------
r887 | v1ctor | 2005-04-10 16:11:43 -0700 (Sun, 10 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r886 | v1ctor | 2005-04-10 16:04:19 -0700 (Sun, 10 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r885 | i_am_drv | 2005-04-10 13:44:53 -0700 (Sun, 10 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r884 | lillo | 2005-04-10 02:59:21 -0700 (Sun, 10 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r883 | v1ctor | 2005-04-09 15:03:30 -0700 (Sat, 09 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r882 | v1ctor | 2005-04-09 14:46:06 -0700 (Sat, 09 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r881 | v1ctor | 2005-04-09 14:18:32 -0700 (Sat, 09 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r880 | lillo | 2005-04-09 06:42:30 -0700 (Sat, 09 Apr 2005) | 2 lines

Reduced profiling performance penalty, more detailed statistics

------------------------------------------------------------------------
r879 | v1ctor | 2005-04-08 21:34:54 -0700 (Fri, 08 Apr 2005) | 2 lines

fixed: implicity boolean test for zstrings -- another zstring was being used instead of an integer

------------------------------------------------------------------------
r878 | v1ctor | 2005-04-08 20:57:00 -0700 (Fri, 08 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r877 | v1ctor | 2005-04-08 20:52:54 -0700 (Fri, 08 Apr 2005) | 2 lines

fixed: "(arg, )" was been accepted as valid

------------------------------------------------------------------------
r876 | v1ctor | 2005-04-08 20:45:35 -0700 (Fri, 08 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r875 | i_am_drv | 2005-04-08 18:27:06 -0700 (Fri, 08 Apr 2005) | 2 lines

removed '$inclib: 'libWXGTK' for linux

------------------------------------------------------------------------
r874 | v1ctor | 2005-04-08 14:50:19 -0700 (Fri, 08 Apr 2005) | 2 lines

can't assume D after a fp is for DOUBLE type, as D is used as E

------------------------------------------------------------------------
r873 | v1ctor | 2005-04-08 13:19:12 -0700 (Fri, 08 Apr 2005) | 2 lines

fixed: byref args were not being added as UINT at parser3, making it fail if arg were an UDT with field array's with different data classes than integer

------------------------------------------------------------------------
r872 | v1ctor | 2005-04-08 12:39:08 -0700 (Fri, 08 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r871 | v1ctor | 2005-04-08 12:39:05 -0700 (Fri, 08 Apr 2005) | 3 lines

fixed: too big id's inside comments;
added: LSET for UDT's;

------------------------------------------------------------------------
r870 | v1ctor | 2005-04-08 11:26:36 -0700 (Fri, 08 Apr 2005) | 2 lines

just removed some unused local vars..

------------------------------------------------------------------------
r869 | v1ctor | 2005-04-08 11:25:10 -0700 (Fri, 08 Apr 2005) | 2 lines

cleaned the \r\n\r sequences that mingw didn't seem to like too much

------------------------------------------------------------------------
r868 | v1ctor | 2005-04-08 11:19:16 -0700 (Fri, 08 Apr 2005) | 2 lines

StrAllocTempResult can't be profiled as EAX can be trashed

------------------------------------------------------------------------
r867 | v1ctor | 2005-04-08 10:40:31 -0700 (Fri, 08 Apr 2005) | 2 lines

fixed: zstring's data size was wrong

------------------------------------------------------------------------
r866 | lillo | 2005-04-08 09:46:32 -0700 (Fri, 08 Apr 2005) | 2 lines

Dunno why I did put a 4 there

------------------------------------------------------------------------
r865 | lillo | 2005-04-08 09:37:01 -0700 (Fri, 08 Apr 2005) | 2 lines

Ops, this undoes last change; yes, locally I'm profiling fbc

------------------------------------------------------------------------
r864 | lillo | 2005-04-08 09:36:03 -0700 (Fri, 08 Apr 2005) | 2 lines

Couple of small fixes

------------------------------------------------------------------------
r863 | lillo | 2005-04-08 08:37:41 -0700 (Fri, 08 Apr 2005) | 2 lines

Added functions profiling; compiling a program with -profile option will enable profiling code generation. When running a profiled program, on exit the profile.txt file will be generated, containing timing results for all function calls issued by the program.

------------------------------------------------------------------------
r862 | i_am_drv | 2005-04-07 19:37:04 -0700 (Thu, 07 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r861 | v1ctor | 2005-04-07 19:34:59 -0700 (Thu, 07 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r860 | v1ctor | 2005-04-07 19:27:35 -0700 (Thu, 07 Apr 2005) | 2 lines

added: support for dereference'd functions used at the left-side

------------------------------------------------------------------------
r859 | v1ctor | 2005-04-07 17:37:47 -0700 (Thu, 07 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r858 | lillo | 2005-04-07 01:16:59 -0700 (Thu, 07 Apr 2005) | 2 lines

Fixed small bug

------------------------------------------------------------------------
r857 | v1ctor | 2005-04-06 21:06:43 -0700 (Wed, 06 Apr 2005) | 2 lines

fixed: threadcreate() called as a SUB

------------------------------------------------------------------------
r856 | i_am_drv | 2005-04-06 16:30:26 -0700 (Wed, 06 Apr 2005) | 3 lines

include file and static lib for the libcaca library
include file and import lib for the wx-c library, ported by dumbledore

------------------------------------------------------------------------
r855 | i_am_drv | 2005-04-06 16:25:24 -0700 (Wed, 06 Apr 2005) | 2 lines

wx-c demo by dumbledore

------------------------------------------------------------------------
r854 | v1ctor | 2005-04-06 14:56:05 -0700 (Wed, 06 Apr 2005) | 2 lines

"=>" can be used with var-initializers too, as "=" would be ambiguous after "as string * 123"

------------------------------------------------------------------------
r853 | v1ctor | 2005-04-06 14:54:18 -0700 (Wed, 06 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r852 | v1ctor | 2005-04-06 14:25:09 -0700 (Wed, 06 Apr 2005) | 2 lines

bitfields update..

------------------------------------------------------------------------
r851 | v1ctor | 2005-04-06 14:23:42 -0700 (Wed, 06 Apr 2005) | 2 lines

fixed: ambiguity with ':' used after fields called TYPE, UNION, AS and END

------------------------------------------------------------------------
r850 | v1ctor | 2005-04-06 12:20:53 -0700 (Wed, 06 Apr 2005) | 2 lines

better gcc-ish bitfields support (i hope..)

------------------------------------------------------------------------
r849 | v1ctor | 2005-04-06 12:20:45 -0700 (Wed, 06 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r848 | lillo | 2005-04-06 02:20:36 -0700 (Wed, 06 Apr 2005) | 3 lines

- FB__* built-in #defines have now been replaced by __FB_*__ counterparts. All headers updated accordingly.
- Added new built-in dynamic #defines: __FILE__, __FUNCTION__, __LINE__, __DATE__ and __TIME__.

------------------------------------------------------------------------
r847 | v1ctor | 2005-04-05 21:54:33 -0700 (Tue, 05 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r846 | v1ctor | 2005-04-05 21:52:41 -0700 (Tue, 05 Apr 2005) | 2 lines

fixed: bitfield mask was inverted..

------------------------------------------------------------------------
r845 | v1ctor | 2005-04-05 21:46:01 -0700 (Tue, 05 Apr 2005) | 7 lines

added:
- bitfields support;
- function deref;

fixed:
- offset could be wrong with var-ini's 'cause type fields padding;

------------------------------------------------------------------------
r844 | v1ctor | 2005-04-05 21:40:25 -0700 (Tue, 05 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r843 | v1ctor | 2005-04-05 21:37:31 -0700 (Tue, 05 Apr 2005) | 2 lines

bitfields update..

------------------------------------------------------------------------
r842 | v1ctor | 2005-04-05 21:23:52 -0700 (Tue, 05 Apr 2005) | 2 lines

PRINTNUM's pad must be included with signed integers..

------------------------------------------------------------------------
r841 | v1ctor | 2005-04-05 21:22:11 -0700 (Tue, 05 Apr 2005) | 2 lines

inline asm for x86..

------------------------------------------------------------------------
r840 | i_am_drv | 2005-04-05 20:24:49 -0700 (Tue, 05 Apr 2005) | 2 lines

wx-c headers ported by dumbledore

------------------------------------------------------------------------
r839 | v1ctor | 2005-04-05 01:20:51 -0700 (Tue, 05 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r838 | v1ctor | 2005-04-04 17:14:37 -0700 (Mon, 04 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r837 | v1ctor | 2005-04-04 14:58:19 -0700 (Mon, 04 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r836 | v1ctor | 2005-04-04 14:57:43 -0700 (Mon, 04 Apr 2005) | 2 lines

var-ini update

------------------------------------------------------------------------
r835 | v1ctor | 2005-04-04 14:53:15 -0700 (Mon, 04 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r834 | v1ctor | 2005-04-04 14:33:41 -0700 (Mon, 04 Apr 2005) | 2 lines

added: run-time var-initializers

------------------------------------------------------------------------
r833 | v1ctor | 2005-04-04 13:51:54 -0700 (Mon, 04 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r832 | lillo | 2005-04-04 11:12:53 -0700 (Mon, 04 Apr 2005) | 2 lines

Fixed warnings pointed out by -Wall (Win32)

------------------------------------------------------------------------
r831 | lillo | 2005-04-04 10:20:18 -0700 (Mon, 04 Apr 2005) | 3 lines

- Fixed warnings pointed out by -Wall
- Filled SETTIME/SETDATE Linux implementations (there were empty functions)

------------------------------------------------------------------------
r830 | lillo | 2005-04-04 04:14:42 -0700 (Mon, 04 Apr 2005) | 2 lines

Fixed small bug

------------------------------------------------------------------------
r829 | lillo | 2005-04-04 03:47:34 -0700 (Mon, 04 Apr 2005) | 3 lines

- Added #ERROR metacommand to interrupt compiling with user-define errors
- Made GTK headers Linux-friendly

------------------------------------------------------------------------
r828 | lillo | 2005-04-04 01:39:24 -0700 (Mon, 04 Apr 2005) | 2 lines

Forgot string mutex for linux port

------------------------------------------------------------------------
r827 | v1ctor | 2005-04-03 21:02:44 -0700 (Sun, 03 Apr 2005) | 2 lines

added: UDT support to var-initializers;

------------------------------------------------------------------------
r826 | v1ctor | 2005-04-03 21:02:23 -0700 (Sun, 03 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r825 | v1ctor | 2005-04-03 19:46:31 -0700 (Sun, 03 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r824 | v1ctor | 2005-04-03 15:20:52 -0700 (Sun, 03 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r823 | v1ctor | 2005-04-03 15:17:12 -0700 (Sun, 03 Apr 2005) | 3 lines

fixed: BOP of a single and a double ops would not set the result type as double;
added: stack aligning before calling functions (disabled);

------------------------------------------------------------------------
r822 | v1ctor | 2005-04-03 14:54:56 -0700 (Sun, 03 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r821 | lillo | 2005-04-03 13:13:20 -0700 (Sun, 03 Apr 2005) | 6 lines

FB is now supposed to ship with two runtime libraries: one thread-unsafe, for single threaded
apps that do not need unnecessary locking (libfb.a), and another thread-safe, for multithreaded
apps. The compiler by default links the former, and detects if a source uses THREADCREATE, in
which case it links the latter. You can also force linking the thread-safe rtlib in any case
using the -mt commandline switch.

------------------------------------------------------------------------
r820 | lillo | 2005-04-03 13:08:55 -0700 (Sun, 03 Apr 2005) | 2 lines

Small fix

------------------------------------------------------------------------
r819 | i_am_drv | 2005-04-03 13:07:38 -0700 (Sun, 03 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r818 | i_am_drv | 2005-04-03 11:13:40 -0700 (Sun, 03 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r817 | i_am_drv | 2005-04-03 10:28:13 -0700 (Sun, 03 Apr 2005) | 2 lines

changed \n\r to \r\n and added Host field in GET header

------------------------------------------------------------------------
r816 | i_am_drv | 2005-04-03 10:18:09 -0700 (Sun, 03 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r815 | i_am_drv | 2005-04-03 10:11:07 -0700 (Sun, 03 Apr 2005) | 2 lines

added fb_ConsoleReadXY

------------------------------------------------------------------------
r814 | lillo | 2005-04-03 09:08:48 -0700 (Sun, 03 Apr 2005) | 2 lines

Finished making rtlib thread-safe

------------------------------------------------------------------------
r813 | v1ctor | 2005-04-03 01:22:54 -0800 (Sun, 03 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r812 | v1ctor | 2005-04-02 23:27:13 -0800 (Sat, 02 Apr 2005) | 2 lines

var-ini update

------------------------------------------------------------------------
r811 | v1ctor | 2005-04-02 22:46:16 -0800 (Sat, 02 Apr 2005) | 3 lines

added: initial var-init support;
fixed: ()'s with arg-less #define's only if they were defined with them

------------------------------------------------------------------------
r810 | v1ctor | 2005-04-02 19:18:45 -0800 (Sat, 02 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r809 | v1ctor | 2005-04-02 13:03:53 -0800 (Sat, 02 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r808 | v1ctor | 2005-04-02 13:01:50 -0800 (Sat, 02 Apr 2005) | 2 lines

argument-less #defines can now be declared and referenced using optional ()'s

------------------------------------------------------------------------
r807 | v1ctor | 2005-04-01 17:35:36 -0800 (Fri, 01 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r806 | v1ctor | 2005-04-01 17:28:01 -0800 (Fri, 01 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r805 | v1ctor | 2005-04-01 17:17:00 -0800 (Fri, 01 Apr 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r804 | lillo | 2005-04-01 15:28:48 -0800 (Fri, 01 Apr 2005) | 2 lines

Win32 OpenGL driver overhaul, should give no more crashes on fullscreen now

------------------------------------------------------------------------
r803 | lillo | 2005-04-01 15:27:32 -0800 (Fri, 01 Apr 2005) | 2 lines

Only defines necessary GUIDs, cutting another 3K from the object size

------------------------------------------------------------------------
r802 | lillo | 2005-04-01 13:43:42 -0800 (Fri, 01 Apr 2005) | 2 lines

Calling SCREENPTR should reset the line pointers to the work page memory

------------------------------------------------------------------------
r801 | v1ctor | 2005-03-31 22:10:52 -0800 (Thu, 31 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r800 | v1ctor | 2005-03-31 19:18:53 -0800 (Thu, 31 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r799 | v1ctor | 2005-03-31 18:42:00 -0800 (Thu, 31 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r798 | v1ctor | 2005-03-31 18:17:10 -0800 (Thu, 31 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r797 | v1ctor | 2005-03-31 17:45:08 -0800 (Thu, 31 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r796 | v1ctor | 2005-03-31 17:34:42 -0800 (Thu, 31 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r795 | v1ctor | 2005-03-31 15:27:29 -0800 (Thu, 31 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r794 | v1ctor | 2005-03-31 14:29:32 -0800 (Thu, 31 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r793 | v1ctor | 2005-03-30 22:55:53 -0800 (Wed, 30 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r792 | v1ctor | 2005-03-30 21:55:24 -0800 (Wed, 30 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r791 | v1ctor | 2005-03-30 21:52:15 -0800 (Wed, 30 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r790 | v1ctor | 2005-03-30 20:55:23 -0800 (Wed, 30 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r789 | v1ctor | 2005-03-30 11:36:13 -0800 (Wed, 30 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r788 | v1ctor | 2005-03-30 11:31:57 -0800 (Wed, 30 Mar 2005) | 2 lines

added the ugly ON ERROR GOTO 0

------------------------------------------------------------------------
r787 | v1ctor | 2005-03-30 11:15:57 -0800 (Wed, 30 Mar 2005) | 2 lines

OPEN, CLOSE#, GET# and PUT# can now be used as functions

------------------------------------------------------------------------
r786 | v1ctor | 2005-03-30 10:35:10 -0800 (Wed, 30 Mar 2005) | 2 lines

with byref as any args, the arg type should be the param type if a temp var is needed

------------------------------------------------------------------------
r785 | lillo | 2005-03-30 08:46:29 -0800 (Wed, 30 Mar 2005) | 2 lines

Force use of mode 13h when setting 320x200x8

------------------------------------------------------------------------
r784 | v1ctor | 2005-03-29 22:00:39 -0800 (Tue, 29 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r783 | v1ctor | 2005-03-29 21:35:58 -0800 (Tue, 29 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r782 | v1ctor | 2005-03-29 21:29:08 -0800 (Tue, 29 Mar 2005) | 2 lines

PIPE: support added to OPEN

------------------------------------------------------------------------
r781 | v1ctor | 2005-03-29 21:04:24 -0800 (Tue, 29 Mar 2005) | 2 lines

feof(), popen(), pclose() added

------------------------------------------------------------------------
r780 | v1ctor | 2005-03-29 18:41:19 -0800 (Tue, 29 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r779 | v1ctor | 2005-03-29 18:29:23 -0800 (Tue, 29 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r778 | v1ctor | 2005-03-29 17:26:22 -0800 (Tue, 29 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r777 | v1ctor | 2005-03-29 17:23:02 -0800 (Tue, 29 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r776 | v1ctor | 2005-03-29 14:43:49 -0800 (Tue, 29 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r775 | lillo | 2005-03-29 14:27:07 -0800 (Tue, 29 Mar 2005) | 2 lines

Restored INIT_ETERM as it happens I forgot ETerm requires special color handling

------------------------------------------------------------------------
r774 | v1ctor | 2005-03-29 13:25:33 -0800 (Tue, 29 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r773 | lillo | 2005-03-29 12:59:18 -0800 (Tue, 29 Mar 2005) | 2 lines

Various small fixes to the win32 port

------------------------------------------------------------------------
r772 | v1ctor | 2005-03-29 12:31:18 -0800 (Tue, 29 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r771 | v1ctor | 2005-03-29 12:29:00 -0800 (Tue, 29 Mar 2005) | 6 lines

small changes:
- #DEFINE won't report an error if declaring again a symbol with exactly the same text - won't work with macros;
- #UNDEF won't report an error if un-defining an non-existent symbol;
- TYPE and UNION fields can now be declared as "AS sometype a, b, c, d(123)" too;
- ENUM items can now be separated by commas too, as in "a, b, c = 2, d";

------------------------------------------------------------------------
r770 | lillo | 2005-03-29 09:29:09 -0800 (Tue, 29 Mar 2005) | 2 lines

Linux build now needs pthread to make rtlib thread-safe

------------------------------------------------------------------------
r769 | lillo | 2005-03-29 09:26:02 -0800 (Tue, 29 Mar 2005) | 2 lines

Removed warnings popped out with -Wall

------------------------------------------------------------------------
r768 | lillo | 2005-03-29 09:25:06 -0800 (Tue, 29 Mar 2005) | 2 lines

Added linux thread-safetiness

------------------------------------------------------------------------
r767 | v1ctor | 2005-03-29 00:07:40 -0800 (Tue, 29 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r766 | v1ctor | 2005-03-28 20:51:49 -0800 (Mon, 28 Mar 2005) | 2 lines

recursion has to be used with functions returning UDT's when checking for SINGLE/DOUBLE fields

------------------------------------------------------------------------
r765 | v1ctor | 2005-03-28 18:06:56 -0800 (Mon, 28 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r764 | v1ctor | 2005-03-28 17:37:21 -0800 (Mon, 28 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r763 | v1ctor | 2005-03-28 16:58:38 -0800 (Mon, 28 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r762 | v1ctor | 2005-03-28 15:31:01 -0800 (Mon, 28 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r761 | v1ctor | 2005-03-28 15:24:20 -0800 (Mon, 28 Mar 2005) | 2 lines

if full, the include file table will be reallocated now

------------------------------------------------------------------------
r760 | v1ctor | 2005-03-28 15:20:15 -0800 (Mon, 28 Mar 2005) | 2 lines

the temp string was not being deleted if file was not found, causing the compiler to fail when too many include files were tested with fileExists() (that calls OPEN)

------------------------------------------------------------------------
r759 | v1ctor | 2005-03-28 15:18:54 -0800 (Mon, 28 Mar 2005) | 2 lines

a bunch of warning fixes cause -Wall is being used now

------------------------------------------------------------------------
r758 | v1ctor | 2005-03-28 11:10:40 -0800 (Mon, 28 Mar 2005) | 4 lines

added:
- UDT's can now be returned by functions (prototypes only, that can't be done with FB procs yet);
- "FUNCTION =" can be used to set the current function's result, as in PB;

------------------------------------------------------------------------
r757 | v1ctor | 2005-03-27 17:29:29 -0800 (Sun, 27 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r756 | i_am_drv | 2005-03-27 12:27:38 -0800 (Sun, 27 Mar 2005) | 2 lines

changed to build os-dependent code in subdirs and moved dos entrypoint source

------------------------------------------------------------------------
r755 | i_am_drv | 2005-03-27 12:09:08 -0800 (Sun, 27 Mar 2005) | 2 lines

use rm instead of del on DOS (allows long file names; builtin)

------------------------------------------------------------------------
r754 | i_am_drv | 2005-03-27 12:05:43 -0800 (Sun, 27 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r753 | i_am_drv | 2005-03-27 12:04:23 -0800 (Sun, 27 Mar 2005) | 2 lines

use rm instead of del on DOS (built into make; allows LFN)

------------------------------------------------------------------------
r752 | i_am_drv | 2005-03-27 12:00:08 -0800 (Sun, 27 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r751 | i_am_drv | 2005-03-27 11:54:32 -0800 (Sun, 27 Mar 2005) | 2 lines

*** empty log message ***

------------------------------------------------------------------------
r750 | i_am_drv | 2005-03-27 11:48:46 -0800 (Sun, 27 Mar 2005) | 2 lines

beep function for DOS (equivalent to print chr(7))

------------------------------------------------------------------------
r749 | i_am_drv | 2005-03-26 23:21:59 -0800 (Sat, 26 Mar 2005) | 2 lines

*** empty log message ***

