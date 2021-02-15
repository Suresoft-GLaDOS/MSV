extern "C" { int __get_mutant(); int __is_neg(int, ...); int __abst_hole(); int __choose(char *);}
/* original courtesy of player1537, http://www.cemetech.net/forum/viewtopic.php?t=5401 */

#include <stdlib.h>
#include <stdio.h>
#include <utility>

#include "Bresenham.h"

  void buggy(unsigned x1, unsigned y1,
			    DiscreteLine & line)
  {
    short x0 = 0, y0 = 0;

    bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    short a;
     //prophet generated patch
     switch(__choose("__ID7")){
     	case 0: {
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #ifdef COMPILE_550
     	case 1: {
     ++this->_M_current;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_551
     	case 2: {
     ++this->_M_impl._M_finish;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_552
     	case 3: {
     ++this->_M_p;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_553
     	case 4: {
     ++this->current;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_554
     	case 5: {
     ++this->iter;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_555
     	case 6: {
     --this->_M_current;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_556
     	case 7: {
     --this->_M_impl._M_finish;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_557
     	case 8: {
     --this->_M_p;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_558
     	case 9: {
     --this->current;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_559
     	case 10: {
     _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_560
     	case 11: {
     _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_561
     	case 12: {
     _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_562
     	case 13: {
     _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_563
     	case 14: {
     a = x0;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_564
     	case 15: {
     a = x1;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_565
     	case 16: {
     a = y0;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_566
     	case 17: {
     if (!__is_neg())
         if (steep) {
             a = x0;
             x0 = y0;
             y0 = a;
             a = x1;
             x1 = y1;
             y1 = a;
         }
     
     break;
     }
     #endif
     #ifdef COMPILE_567
     	case 18: {
     if ((steep) && !__is_neg()) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_568
     	case 19: {
     if ((steep) || __is_neg()) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_569
     	case 20: {
     if (__is_neg())
         return;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_570
     	case 21: {
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_571
     	case 22: {
     if (this->_M_end_of_storage)
         return std::__addressof(this->_M_end_of_storage[-1]) + 1;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_572
     	case 23: {
     if (this->_M_offset++ == int(_S_word_bit) - 1) {
         this->_M_offset = 0;
         ++this->_M_p;
     }
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_573
     	case 24: {
     if (this->_M_offset-- == 0) {
         this->_M_offset = int(_S_word_bit) - 1;
         --this->_M_p;
     }
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_574
     	case 25: {
     if (this->capacity() - this->size() < int(_S_word_bit))
         return false;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_575
     	case 26: {
     if (this->capacity() == this->size())
         return false;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_576
     	case 27: {
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_577
     	case 28: {
     std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_578
     	case 29: {
     this->_M_bump_down();
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_579
     	case 30: {
     this->_M_bump_up();
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_580
     	case 31: {
     this->_M_deallocate();
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_581
     	case 32: {
     this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_582
     	case 33: {
     this->_M_erase_at_end(begin());
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_583
     	case 34: {
     this->_M_erase_at_end(this->_M_impl._M_start);
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_584
     	case 35: {
     this->_M_impl._M_end_of_storage = __new_start + __len;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_585
     	case 36: {
     this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_586
     	case 37: {
     this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_587
     	case 38: {
     this->_M_impl._M_end_of_storage = nullptr;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_588
     	case 39: {
     this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_589
     	case 40: {
     this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_590
     	case 41: {
     this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_591
     	case 42: {
     this->_M_impl._M_finish = __tmp + __old_size;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_592
     	case 43: {
     this->_M_impl._M_finish = nullptr;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_593
     	case 44: {
     this->_M_impl._M_finish = std::_Bit_iterator();
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_594
     	case 45: {
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_595
     	case 46: {
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_596
     	case 47: {
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_597
     	case 48: {
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_598
     	case 49: {
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_599
     	case 50: {
     this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_600
     	case 51: {
     this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_601
     	case 52: {
     this->_M_impl._M_finish = this->_M_impl._M_start;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_602
     	case 53: {
     this->_M_impl._M_start = nullptr;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_603
     	case 54: {
     this->_M_impl._M_start = std::_Bit_iterator();
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_604
     	case 55: {
     this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_605
     	case 56: {
     this->_M_offset = 0;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_606
     	case 57: {
     this->_M_offset = int(_S_word_bit) - 1;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_607
     	case 58: {
     this->_M_reallocate(this->size());
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_608
     	case 59: {
     this->_M_shrink_to_fit();
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_609
     	case 60: {
     this->clear();
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_610
     	case 61: {
     throw;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_611
     	case 62: {
     x0 = x1;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_612
     	case 63: {
     x0 = y0;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_613
     	case 64: {
     x1 = a;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_614
     	case 65: {
     x1 = y1;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_615
     	case 66: {
     y0 = a;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_616
     	case 67: {
     y0 = y1;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_617
     	case 68: {
     y1 = a;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     }
     //prophet generated patch
     switch(__choose("__ID8")){
     	case 0: {
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #ifdef COMPILE_618
     	case 1: {
     ++this->_M_current;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_619
     	case 2: {
     ++this->_M_impl._M_finish;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_620
     	case 3: {
     ++this->_M_p;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_621
     	case 4: {
     ++this->current;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_622
     	case 5: {
     ++this->iter;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_623
     	case 6: {
     --this->_M_current;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_624
     	case 7: {
     --this->_M_impl._M_finish;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_625
     	case 8: {
     --this->_M_p;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_626
     	case 9: {
     --this->current;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_627
     	case 10: {
     _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_628
     	case 11: {
     _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_629
     	case 12: {
     _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_630
     	case 13: {
     _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_631
     	case 14: {
     a = x0;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_632
     	case 15: {
     a = x1;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_633
     	case 16: {
     a = y0;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_634
     	case 17: {
     if (!__is_neg())
         if (x0 > x1) {
             a = x0;
             x0 = x1;
             x1 = a;
             a = y0;
             y0 = y1;
             y1 = a;
         }
     
     break;
     }
     #endif
     #ifdef COMPILE_635
     	case 18: {
     if ((x0 > x1) && !__is_neg()) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_636
     	case 19: {
     if ((x0 > x1) || __is_neg()) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_637
     	case 20: {
     if (__is_neg())
         return;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_638
     	case 21: {
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_639
     	case 22: {
     if (this->_M_end_of_storage)
         return std::__addressof(this->_M_end_of_storage[-1]) + 1;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_640
     	case 23: {
     if (this->_M_offset++ == int(_S_word_bit) - 1) {
         this->_M_offset = 0;
         ++this->_M_p;
     }
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_641
     	case 24: {
     if (this->_M_offset-- == 0) {
         this->_M_offset = int(_S_word_bit) - 1;
         --this->_M_p;
     }
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_642
     	case 25: {
     if (this->capacity() - this->size() < int(_S_word_bit))
         return false;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_643
     	case 26: {
     if (this->capacity() == this->size())
         return false;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_644
     	case 27: {
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_645
     	case 28: {
     std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_646
     	case 29: {
     this->_M_bump_down();
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_647
     	case 30: {
     this->_M_bump_up();
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_648
     	case 31: {
     this->_M_deallocate();
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_649
     	case 32: {
     this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_650
     	case 33: {
     this->_M_erase_at_end(begin());
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_651
     	case 34: {
     this->_M_erase_at_end(this->_M_impl._M_start);
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_652
     	case 35: {
     this->_M_impl._M_end_of_storage = __new_start + __len;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_653
     	case 36: {
     this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_654
     	case 37: {
     this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_655
     	case 38: {
     this->_M_impl._M_end_of_storage = nullptr;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_656
     	case 39: {
     this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_657
     	case 40: {
     this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_658
     	case 41: {
     this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_659
     	case 42: {
     this->_M_impl._M_finish = __tmp + __old_size;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_660
     	case 43: {
     this->_M_impl._M_finish = nullptr;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_661
     	case 44: {
     this->_M_impl._M_finish = std::_Bit_iterator();
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_662
     	case 45: {
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_663
     	case 46: {
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_664
     	case 47: {
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_665
     	case 48: {
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_666
     	case 49: {
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_667
     	case 50: {
     this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_668
     	case 51: {
     this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_669
     	case 52: {
     this->_M_impl._M_finish = this->_M_impl._M_start;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_670
     	case 53: {
     this->_M_impl._M_start = nullptr;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_671
     	case 54: {
     this->_M_impl._M_start = std::_Bit_iterator();
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_672
     	case 55: {
     this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_673
     	case 56: {
     this->_M_offset = 0;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_674
     	case 57: {
     this->_M_offset = int(_S_word_bit) - 1;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_675
     	case 58: {
     this->_M_reallocate(this->size());
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_676
     	case 59: {
     this->_M_shrink_to_fit();
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_677
     	case 60: {
     this->clear();
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_678
     	case 61: {
     throw;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_679
     	case 62: {
     x0 = x1;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_680
     	case 63: {
     x0 = y0;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_681
     	case 64: {
     x1 = a;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_682
     	case 65: {
     x1 = y1;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_683
     	case 66: {
     y0 = a;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_684
     	case 67: {
     y0 = y1;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     #ifdef COMPILE_685
     	case 68: {
     y1 = a;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     
     break;
     }
     #endif
     }

     int deltax = x1 - x0;
     int deltay = abs((int)(y1 - y0));
     float error = 0;
     float deltaerr = deltay / deltax;
     int ystep;
     if (y0 < y1)
       //prophet generated patch
           switch(__choose("__ID0")){
           	case 0: {
           ystep = 1;
           
           break;
           }
           #ifdef COMPILE_0
           	case 1: {
           ++this->_M_current;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_1
           	case 2: {
           ++this->_M_impl._M_finish;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_2
           	case 3: {
           ++this->_M_p;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_3
           	case 4: {
           ++this->current;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_4
           	case 5: {
           ++this->iter;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_5
           	case 6: {
           --this->_M_current;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_6
           	case 7: {
           --this->_M_impl._M_finish;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_7
           	case 8: {
           --this->_M_p;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_8
           	case 9: {
           --this->current;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_9
           	case 10: {
           _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_10
           	case 11: {
           _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_11
           	case 12: {
           _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_12
           	case 13: {
           _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_13
           	case 14: {
           a = x0;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_14
           	case 15: {
           a = x1;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_15
           	case 16: {
           a = y0;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_16
           	case 17: {
           error = error + deltaerr;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_17
           	case 18: {
           error = error - 1;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_18
           	case 19: {
           if (!__is_neg())
               ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_19
           	case 20: {
           if (__is_neg())
               return;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_20
           	case 21: {
           if (steep) {
               a = x0;
               x0 = y0;
               y0 = a;
               a = x1;
               x1 = y1;
               y1 = a;
           }
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_21
           	case 22: {
           if (this->_M_end_of_storage)
               return std::__addressof(this->_M_end_of_storage[-1]) + 1;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_22
           	case 23: {
           if (this->_M_offset++ == int(_S_word_bit) - 1) {
               this->_M_offset = 0;
               ++this->_M_p;
           }
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_23
           	case 24: {
           if (this->_M_offset-- == 0) {
               this->_M_offset = int(_S_word_bit) - 1;
               --this->_M_p;
           }
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_24
           	case 25: {
           if (this->capacity() - this->size() < int(_S_word_bit))
               return false;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_25
           	case 26: {
           if (this->capacity() == this->size())
               return false;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_26
           	case 27: {
           if (x0 > x1) {
               a = x0;
               x0 = x1;
               x1 = a;
               a = y0;
               y0 = y1;
               y1 = a;
           }
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_27
           	case 28: {
           if (y0 < y1)
               ystep = 1;
           else
               ystep = -1;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_28
           	case 29: {
           std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_29
           	case 30: {
           this->_M_bump_down();
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_30
           	case 31: {
           this->_M_bump_up();
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_31
           	case 32: {
           this->_M_deallocate();
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_32
           	case 33: {
           this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_33
           	case 34: {
           this->_M_erase_at_end(begin());
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_34
           	case 35: {
           this->_M_erase_at_end(this->_M_impl._M_start);
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_35
           	case 36: {
           this->_M_impl._M_end_of_storage = __new_start + __len;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_36
           	case 37: {
           this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_37
           	case 38: {
           this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_38
           	case 39: {
           this->_M_impl._M_end_of_storage = nullptr;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_39
           	case 40: {
           this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_40
           	case 41: {
           this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_41
           	case 42: {
           this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_42
           	case 43: {
           this->_M_impl._M_finish = __tmp + __old_size;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_43
           	case 44: {
           this->_M_impl._M_finish = nullptr;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_44
           	case 45: {
           this->_M_impl._M_finish = std::_Bit_iterator();
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_45
           	case 46: {
           this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_46
           	case 47: {
           this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_47
           	case 48: {
           this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_48
           	case 49: {
           this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_49
           	case 50: {
           this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_50
           	case 51: {
           this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_51
           	case 52: {
           this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_52
           	case 53: {
           this->_M_impl._M_finish = this->_M_impl._M_start;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_53
           	case 54: {
           this->_M_impl._M_start = nullptr;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_54
           	case 55: {
           this->_M_impl._M_start = std::_Bit_iterator();
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_55
           	case 56: {
           this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_56
           	case 57: {
           this->_M_offset = 0;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_57
           	case 58: {
           this->_M_offset = int(_S_word_bit) - 1;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_58
           	case 59: {
           this->_M_reallocate(this->size());
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_59
           	case 60: {
           this->_M_shrink_to_fit();
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_60
           	case 61: {
           this->clear();
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_61
           	case 62: {
           throw;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_62
           	case 63: {
           x0 = x1;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_63
           	case 64: {
           x0 = y0;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_64
           	case 65: {
           x1 = a;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_65
           	case 66: {
           x1 = y1;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_66
           	case 67: {
           y0 = a;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_67
           	case 68: {
           y0 = y1;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_68
           	case 69: {
           y1 = a;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_69
           	case 70: {
           ystep = -1;
           ystep = 1;
           
           break;
           }
           #endif
           #ifdef COMPILE_70
           	case 71: {
           ystep = 1;
           ystep = 1;
           
           break;
           }
           #endif
           }
     else
       ystep = -1;

     int y = y0;
     std::pair<int,int> var;
     for (int x = x0;x < x1;x++)
       {
	 if (steep){
	   var.first=y;
	   var.second=x;
	   line.push_back(var);}
	 else{
	   //prophet generated patch
	   switch(__choose("__ID1")){
	   	case 0: {
	   var.first = x;
	   
	   break;
	   }
	   #ifdef COMPILE_71
	   	case 1: {
	   ++this->_M_current;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_72
	   	case 2: {
	   ++this->_M_impl._M_finish;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_73
	   	case 3: {
	   ++this->_M_p;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_74
	   	case 4: {
	   ++this->current;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_75
	   	case 5: {
	   ++this->iter;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_76
	   	case 6: {
	   --this->_M_current;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_77
	   	case 7: {
	   --this->_M_impl._M_finish;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_78
	   	case 8: {
	   --this->_M_p;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_79
	   	case 9: {
	   --this->current;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_80
	   	case 10: {
	   _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_81
	   	case 11: {
	   _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_82
	   	case 12: {
	   _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_83
	   	case 13: {
	   _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_84
	   	case 14: {
	   a = x0;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_85
	   	case 15: {
	   a = x1;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_86
	   	case 16: {
	   a = y0;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_87
	   	case 17: {
	   error = error + deltaerr;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_88
	   	case 18: {
	   error = error - 1;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_89
	   	case 19: {
	   if (!__is_neg())
	       var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_90
	   	case 20: {
	   if (__is_neg())
	       break;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_91
	   	case 21: {
	   if (__is_neg())
	       return;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_92
	   	case 22: {
	   if (error >= 0.5) {
	       y += ystep;
	       error = error - 1;
	   }
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_93
	   	case 23: {
	   if (steep) {
	       a = x0;
	       x0 = y0;
	       y0 = a;
	       a = x1;
	       x1 = y1;
	       y1 = a;
	   }
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_94
	   	case 24: {
	   if (steep) {
	       var.first = y;
	       var.second = x;
	       line.push_back(var);
	   } else {
	       var.first = x;
	       var.second = y;
	       line.push_back(var);
	   }
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_95
	   	case 25: {
	   if (this->_M_end_of_storage)
	       return std::__addressof(this->_M_end_of_storage[-1]) + 1;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_96
	   	case 26: {
	   if (this->_M_offset++ == int(_S_word_bit) - 1) {
	       this->_M_offset = 0;
	       ++this->_M_p;
	   }
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_97
	   	case 27: {
	   if (this->_M_offset-- == 0) {
	       this->_M_offset = int(_S_word_bit) - 1;
	       --this->_M_p;
	   }
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_98
	   	case 28: {
	   if (this->capacity() - this->size() < int(_S_word_bit))
	       return false;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_99
	   	case 29: {
	   if (this->capacity() == this->size())
	       return false;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_100
	   	case 30: {
	   if (x0 > x1) {
	       a = x0;
	       x0 = x1;
	       x1 = a;
	       a = y0;
	       y0 = y1;
	       y1 = a;
	   }
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_101
	   	case 31: {
	   if (y0 < y1)
	       ystep = 1;
	   else
	       ystep = -1;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_102
	   	case 32: {
	   line.push_back(var);
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_103
	   	case 33: {
	   std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_104
	   	case 34: {
	   this->_M_bump_down();
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_105
	   	case 35: {
	   this->_M_bump_up();
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_106
	   	case 36: {
	   this->_M_deallocate();
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_107
	   	case 37: {
	   this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_108
	   	case 38: {
	   this->_M_erase_at_end(begin());
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_109
	   	case 39: {
	   this->_M_erase_at_end(this->_M_impl._M_start);
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_110
	   	case 40: {
	   this->_M_impl._M_end_of_storage = __new_start + __len;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_111
	   	case 41: {
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_112
	   	case 42: {
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_113
	   	case 43: {
	   this->_M_impl._M_end_of_storage = nullptr;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_114
	   	case 44: {
	   this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_115
	   	case 45: {
	   this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_116
	   	case 46: {
	   this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_117
	   	case 47: {
	   this->_M_impl._M_finish = __tmp + __old_size;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_118
	   	case 48: {
	   this->_M_impl._M_finish = nullptr;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_119
	   	case 49: {
	   this->_M_impl._M_finish = std::_Bit_iterator();
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_120
	   	case 50: {
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_121
	   	case 51: {
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_122
	   	case 52: {
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_123
	   	case 53: {
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_124
	   	case 54: {
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_125
	   	case 55: {
	   this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_126
	   	case 56: {
	   this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_127
	   	case 57: {
	   this->_M_impl._M_finish = this->_M_impl._M_start;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_128
	   	case 58: {
	   this->_M_impl._M_start = nullptr;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_129
	   	case 59: {
	   this->_M_impl._M_start = std::_Bit_iterator();
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_130
	   	case 60: {
	   this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_131
	   	case 61: {
	   this->_M_offset = 0;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_132
	   	case 62: {
	   this->_M_offset = int(_S_word_bit) - 1;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_133
	   	case 63: {
	   this->_M_reallocate(this->size());
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_134
	   	case 64: {
	   this->_M_shrink_to_fit();
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_135
	   	case 65: {
	   this->clear();
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_136
	   	case 66: {
	   throw;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_137
	   	case 67: {
	   var.first = x;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_138
	   	case 68: {
	   var.first = y;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_139
	   	case 69: {
	   var.second = x;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_140
	   	case 70: {
	   var.second = y;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_141
	   	case 71: {
	   x0 = x1;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_142
	   	case 72: {
	   x0 = y0;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_143
	   	case 73: {
	   x1 = a;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_144
	   	case 74: {
	   x1 = y1;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_145
	   	case 75: {
	   y0 = a;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_146
	   	case 76: {
	   y0 = y1;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_147
	   	case 77: {
	   y1 = a;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_148
	   	case 78: {
	   ystep = -1;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_149
	   	case 79: {
	   ystep = 1;
	   var.first = x;
	   
	   break;
	   }
	   #endif
	   }
	   //prophet generated patch
	   switch(__choose("__ID2")){
	   	case 0: {
	   var.second = y;
	   
	   break;
	   }
	   #ifdef COMPILE_150
	   	case 1: {
	   ++this->_M_current;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_151
	   	case 2: {
	   ++this->_M_impl._M_finish;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_152
	   	case 3: {
	   ++this->_M_p;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_153
	   	case 4: {
	   ++this->current;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_154
	   	case 5: {
	   ++this->iter;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_155
	   	case 6: {
	   --this->_M_current;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_156
	   	case 7: {
	   --this->_M_impl._M_finish;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_157
	   	case 8: {
	   --this->_M_p;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_158
	   	case 9: {
	   --this->current;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_159
	   	case 10: {
	   _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_160
	   	case 11: {
	   _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_161
	   	case 12: {
	   _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_162
	   	case 13: {
	   _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_163
	   	case 14: {
	   a = x0;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_164
	   	case 15: {
	   a = x1;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_165
	   	case 16: {
	   a = y0;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_166
	   	case 17: {
	   error = error + deltaerr;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_167
	   	case 18: {
	   error = error - 1;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_168
	   	case 19: {
	   if (!__is_neg())
	       var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_169
	   	case 20: {
	   if (__is_neg())
	       break;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_170
	   	case 21: {
	   if (__is_neg())
	       return;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_171
	   	case 22: {
	   if (error >= 0.5) {
	       y += ystep;
	       error = error - 1;
	   }
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_172
	   	case 23: {
	   if (steep) {
	       a = x0;
	       x0 = y0;
	       y0 = a;
	       a = x1;
	       x1 = y1;
	       y1 = a;
	   }
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_173
	   	case 24: {
	   if (steep) {
	       var.first = y;
	       var.second = x;
	       line.push_back(var);
	   } else {
	       var.first = x;
	       var.second = y;
	       line.push_back(var);
	   }
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_174
	   	case 25: {
	   if (this->_M_end_of_storage)
	       return std::__addressof(this->_M_end_of_storage[-1]) + 1;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_175
	   	case 26: {
	   if (this->_M_offset++ == int(_S_word_bit) - 1) {
	       this->_M_offset = 0;
	       ++this->_M_p;
	   }
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_176
	   	case 27: {
	   if (this->_M_offset-- == 0) {
	       this->_M_offset = int(_S_word_bit) - 1;
	       --this->_M_p;
	   }
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_177
	   	case 28: {
	   if (this->capacity() - this->size() < int(_S_word_bit))
	       return false;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_178
	   	case 29: {
	   if (this->capacity() == this->size())
	       return false;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_179
	   	case 30: {
	   if (x0 > x1) {
	       a = x0;
	       x0 = x1;
	       x1 = a;
	       a = y0;
	       y0 = y1;
	       y1 = a;
	   }
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_180
	   	case 31: {
	   if (y0 < y1)
	       ystep = 1;
	   else
	       ystep = -1;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_181
	   	case 32: {
	   line.push_back(var);
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_182
	   	case 33: {
	   std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_183
	   	case 34: {
	   this->_M_bump_down();
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_184
	   	case 35: {
	   this->_M_bump_up();
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_185
	   	case 36: {
	   this->_M_deallocate();
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_186
	   	case 37: {
	   this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_187
	   	case 38: {
	   this->_M_erase_at_end(begin());
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_188
	   	case 39: {
	   this->_M_erase_at_end(this->_M_impl._M_start);
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_189
	   	case 40: {
	   this->_M_impl._M_end_of_storage = __new_start + __len;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_190
	   	case 41: {
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_191
	   	case 42: {
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_192
	   	case 43: {
	   this->_M_impl._M_end_of_storage = nullptr;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_193
	   	case 44: {
	   this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_194
	   	case 45: {
	   this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_195
	   	case 46: {
	   this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_196
	   	case 47: {
	   this->_M_impl._M_finish = __tmp + __old_size;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_197
	   	case 48: {
	   this->_M_impl._M_finish = nullptr;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_198
	   	case 49: {
	   this->_M_impl._M_finish = std::_Bit_iterator();
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_199
	   	case 50: {
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_200
	   	case 51: {
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_201
	   	case 52: {
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_202
	   	case 53: {
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_203
	   	case 54: {
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_204
	   	case 55: {
	   this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_205
	   	case 56: {
	   this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_206
	   	case 57: {
	   this->_M_impl._M_finish = this->_M_impl._M_start;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_207
	   	case 58: {
	   this->_M_impl._M_start = nullptr;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_208
	   	case 59: {
	   this->_M_impl._M_start = std::_Bit_iterator();
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_209
	   	case 60: {
	   this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_210
	   	case 61: {
	   this->_M_offset = 0;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_211
	   	case 62: {
	   this->_M_offset = int(_S_word_bit) - 1;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_212
	   	case 63: {
	   this->_M_reallocate(this->size());
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_213
	   	case 64: {
	   this->_M_shrink_to_fit();
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_214
	   	case 65: {
	   this->clear();
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_215
	   	case 66: {
	   throw;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_216
	   	case 67: {
	   var.first = x;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_217
	   	case 68: {
	   var.first = y;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_218
	   	case 69: {
	   var.second = x;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_219
	   	case 70: {
	   var.second = y;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_220
	   	case 71: {
	   x0 = x1;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_221
	   	case 72: {
	   x0 = y0;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_222
	   	case 73: {
	   x1 = a;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_223
	   	case 74: {
	   x1 = y1;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_224
	   	case 75: {
	   y0 = a;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_225
	   	case 76: {
	   y0 = y1;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_226
	   	case 77: {
	   y1 = a;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_227
	   	case 78: {
	   ystep = -1;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_228
	   	case 79: {
	   ystep = 1;
	   var.second = y;
	   
	   break;
	   }
	   #endif
	   }
	   //prophet generated patch
	   switch(__choose("__ID3")){
	   	case 0: {
	   line.push_back(var);
	   
	   break;
	   }
	   #ifdef COMPILE_229
	   	case 1: {
	   ++this->_M_current;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_230
	   	case 2: {
	   ++this->_M_impl._M_finish;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_231
	   	case 3: {
	   ++this->_M_p;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_232
	   	case 4: {
	   ++this->current;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_233
	   	case 5: {
	   ++this->iter;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_234
	   	case 6: {
	   --this->_M_current;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_235
	   	case 7: {
	   --this->_M_impl._M_finish;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_236
	   	case 8: {
	   --this->_M_p;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_237
	   	case 9: {
	   --this->current;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_238
	   	case 10: {
	   _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_239
	   	case 11: {
	   _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_240
	   	case 12: {
	   _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_241
	   	case 13: {
	   _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_242
	   	case 14: {
	   a = x0;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_243
	   	case 15: {
	   a = x1;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_244
	   	case 16: {
	   a = y0;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_245
	   	case 17: {
	   error = error + deltaerr;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_246
	   	case 18: {
	   error = error - 1;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_247
	   	case 19: {
	   if (!__is_neg())
	       line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_248
	   	case 20: {
	   if (__is_neg())
	       break;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_249
	   	case 21: {
	   if (__is_neg())
	       return;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_250
	   	case 22: {
	   if (error >= 0.5) {
	       y += ystep;
	       error = error - 1;
	   }
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_251
	   	case 23: {
	   if (steep) {
	       a = x0;
	       x0 = y0;
	       y0 = a;
	       a = x1;
	       x1 = y1;
	       y1 = a;
	   }
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_252
	   	case 24: {
	   if (steep) {
	       var.first = y;
	       var.second = x;
	       line.push_back(var);
	   } else {
	       var.first = x;
	       var.second = y;
	       line.push_back(var);
	   }
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_253
	   	case 25: {
	   if (this->_M_end_of_storage)
	       return std::__addressof(this->_M_end_of_storage[-1]) + 1;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_254
	   	case 26: {
	   if (this->_M_offset++ == int(_S_word_bit) - 1) {
	       this->_M_offset = 0;
	       ++this->_M_p;
	   }
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_255
	   	case 27: {
	   if (this->_M_offset-- == 0) {
	       this->_M_offset = int(_S_word_bit) - 1;
	       --this->_M_p;
	   }
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_256
	   	case 28: {
	   if (this->capacity() - this->size() < int(_S_word_bit))
	       return false;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_257
	   	case 29: {
	   if (this->capacity() == this->size())
	       return false;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_258
	   	case 30: {
	   if (x0 > x1) {
	       a = x0;
	       x0 = x1;
	       x1 = a;
	       a = y0;
	       y0 = y1;
	       y1 = a;
	   }
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_259
	   	case 31: {
	   if (y0 < y1)
	       ystep = 1;
	   else
	       ystep = -1;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_260
	   	case 32: {
	   line.push_back(var);
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_261
	   	case 33: {
	   std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_262
	   	case 34: {
	   this->_M_bump_down();
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_263
	   	case 35: {
	   this->_M_bump_up();
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_264
	   	case 36: {
	   this->_M_deallocate();
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_265
	   	case 37: {
	   this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_266
	   	case 38: {
	   this->_M_erase_at_end(begin());
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_267
	   	case 39: {
	   this->_M_erase_at_end(this->_M_impl._M_start);
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_268
	   	case 40: {
	   this->_M_impl._M_end_of_storage = __new_start + __len;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_269
	   	case 41: {
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_270
	   	case 42: {
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_271
	   	case 43: {
	   this->_M_impl._M_end_of_storage = nullptr;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_272
	   	case 44: {
	   this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_273
	   	case 45: {
	   this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_274
	   	case 46: {
	   this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_275
	   	case 47: {
	   this->_M_impl._M_finish = __tmp + __old_size;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_276
	   	case 48: {
	   this->_M_impl._M_finish = nullptr;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_277
	   	case 49: {
	   this->_M_impl._M_finish = std::_Bit_iterator();
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_278
	   	case 50: {
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_279
	   	case 51: {
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_280
	   	case 52: {
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_281
	   	case 53: {
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_282
	   	case 54: {
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_283
	   	case 55: {
	   this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_284
	   	case 56: {
	   this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_285
	   	case 57: {
	   this->_M_impl._M_finish = this->_M_impl._M_start;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_286
	   	case 58: {
	   this->_M_impl._M_start = nullptr;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_287
	   	case 59: {
	   this->_M_impl._M_start = std::_Bit_iterator();
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_288
	   	case 60: {
	   this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_289
	   	case 61: {
	   this->_M_offset = 0;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_290
	   	case 62: {
	   this->_M_offset = int(_S_word_bit) - 1;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_291
	   	case 63: {
	   this->_M_reallocate(this->size());
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_292
	   	case 64: {
	   this->_M_shrink_to_fit();
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_293
	   	case 65: {
	   this->clear();
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_294
	   	case 66: {
	   throw;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_295
	   	case 67: {
	   var.first = x;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_296
	   	case 68: {
	   var.first = y;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_297
	   	case 69: {
	   var.second = x;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_298
	   	case 70: {
	   var.second = y;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_299
	   	case 71: {
	   x0 = x1;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_300
	   	case 72: {
	   x0 = y0;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_301
	   	case 73: {
	   x1 = a;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_302
	   	case 74: {
	   x1 = y1;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_303
	   	case 75: {
	   y0 = a;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_304
	   	case 76: {
	   y0 = y1;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_305
	   	case 77: {
	   y1 = a;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_306
	   	case 78: {
	   ystep = -1;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   #ifdef COMPILE_307
	   	case 79: {
	   ystep = 1;
	   line.push_back(var);
	   
	   break;
	   }
	   #endif
	   }
}
	 //prophet generated patch
	 switch(__choose("__ID5")){
	 	case 0: {
	 error = error + deltaerr;
	 
	 break;
	 }
	 #ifdef COMPILE_390
	 	case 1: {
	 ++this->_M_current;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_391
	 	case 2: {
	 ++this->_M_impl._M_finish;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_392
	 	case 3: {
	 ++this->_M_p;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_393
	 	case 4: {
	 ++this->current;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_394
	 	case 5: {
	 ++this->iter;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_395
	 	case 6: {
	 --this->_M_current;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_396
	 	case 7: {
	 --this->_M_impl._M_finish;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_397
	 	case 8: {
	 --this->_M_p;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_398
	 	case 9: {
	 --this->current;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_399
	 	case 10: {
	 _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_400
	 	case 11: {
	 _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_401
	 	case 12: {
	 _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_402
	 	case 13: {
	 _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_403
	 	case 14: {
	 a = x0;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_404
	 	case 15: {
	 a = x1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_405
	 	case 16: {
	 a = y0;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_406
	 	case 17: {
	 error = error + deltaerr;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_407
	 	case 18: {
	 error = error - 1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_408
	 	case 19: {
	 if (!__is_neg())
	     error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_409
	 	case 20: {
	 if (__is_neg())
	     break;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_410
	 	case 21: {
	 if (__is_neg())
	     return;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_411
	 	case 22: {
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_412
	 	case 23: {
	 if (steep) {
	     a = x0;
	     x0 = y0;
	     y0 = a;
	     a = x1;
	     x1 = y1;
	     y1 = a;
	 }
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_413
	 	case 24: {
	 if (steep) {
	     var.first = y;
	     var.second = x;
	     line.push_back(var);
	 } else {
	     var.first = x;
	     var.second = y;
	     line.push_back(var);
	 }
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_414
	 	case 25: {
	 if (this->_M_end_of_storage)
	     return std::__addressof(this->_M_end_of_storage[-1]) + 1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_415
	 	case 26: {
	 if (this->_M_offset++ == int(_S_word_bit) - 1) {
	     this->_M_offset = 0;
	     ++this->_M_p;
	 }
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_416
	 	case 27: {
	 if (this->_M_offset-- == 0) {
	     this->_M_offset = int(_S_word_bit) - 1;
	     --this->_M_p;
	 }
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_417
	 	case 28: {
	 if (this->capacity() - this->size() < int(_S_word_bit))
	     return false;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_418
	 	case 29: {
	 if (this->capacity() == this->size())
	     return false;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_419
	 	case 30: {
	 if (x0 > x1) {
	     a = x0;
	     x0 = x1;
	     x1 = a;
	     a = y0;
	     y0 = y1;
	     y1 = a;
	 }
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_420
	 	case 31: {
	 if (y0 < y1)
	     ystep = 1;
	 else
	     ystep = -1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_421
	 	case 32: {
	 line.push_back(var);
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_422
	 	case 33: {
	 std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_423
	 	case 34: {
	 this->_M_bump_down();
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_424
	 	case 35: {
	 this->_M_bump_up();
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_425
	 	case 36: {
	 this->_M_deallocate();
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_426
	 	case 37: {
	 this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_427
	 	case 38: {
	 this->_M_erase_at_end(begin());
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_428
	 	case 39: {
	 this->_M_erase_at_end(this->_M_impl._M_start);
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_429
	 	case 40: {
	 this->_M_impl._M_end_of_storage = __new_start + __len;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_430
	 	case 41: {
	 this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_431
	 	case 42: {
	 this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_432
	 	case 43: {
	 this->_M_impl._M_end_of_storage = nullptr;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_433
	 	case 44: {
	 this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_434
	 	case 45: {
	 this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_435
	 	case 46: {
	 this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_436
	 	case 47: {
	 this->_M_impl._M_finish = __tmp + __old_size;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_437
	 	case 48: {
	 this->_M_impl._M_finish = nullptr;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_438
	 	case 49: {
	 this->_M_impl._M_finish = std::_Bit_iterator();
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_439
	 	case 50: {
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_440
	 	case 51: {
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_441
	 	case 52: {
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_442
	 	case 53: {
	 this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_443
	 	case 54: {
	 this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_444
	 	case 55: {
	 this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_445
	 	case 56: {
	 this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_446
	 	case 57: {
	 this->_M_impl._M_finish = this->_M_impl._M_start;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_447
	 	case 58: {
	 this->_M_impl._M_start = nullptr;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_448
	 	case 59: {
	 this->_M_impl._M_start = std::_Bit_iterator();
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_449
	 	case 60: {
	 this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_450
	 	case 61: {
	 this->_M_offset = 0;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_451
	 	case 62: {
	 this->_M_offset = int(_S_word_bit) - 1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_452
	 	case 63: {
	 this->_M_reallocate(this->size());
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_453
	 	case 64: {
	 this->_M_shrink_to_fit();
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_454
	 	case 65: {
	 this->clear();
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_455
	 	case 66: {
	 throw;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_456
	 	case 67: {
	 var.first = x;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_457
	 	case 68: {
	 var.first = y;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_458
	 	case 69: {
	 var.second = x;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_459
	 	case 70: {
	 var.second = y;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_460
	 	case 71: {
	 x0 = x1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_461
	 	case 72: {
	 x0 = y0;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_462
	 	case 73: {
	 x1 = a;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_463
	 	case 74: {
	 x1 = y1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_464
	 	case 75: {
	 y0 = a;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_465
	 	case 76: {
	 y0 = y1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_466
	 	case 77: {
	 y1 = a;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_467
	 	case 78: {
	 ystep = -1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_468
	 	case 79: {
	 ystep = 1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 #endif
	 }
	 //prophet generated patch
	 switch(__choose("__ID6")){
	 	case 0: {
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #ifdef COMPILE_469
	 	case 1: {
	 ++this->_M_current;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_470
	 	case 2: {
	 ++this->_M_impl._M_finish;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_471
	 	case 3: {
	 ++this->_M_p;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_472
	 	case 4: {
	 ++this->current;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_473
	 	case 5: {
	 ++this->iter;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_474
	 	case 6: {
	 --this->_M_current;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_475
	 	case 7: {
	 --this->_M_impl._M_finish;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_476
	 	case 8: {
	 --this->_M_p;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_477
	 	case 9: {
	 --this->current;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_478
	 	case 10: {
	 _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_479
	 	case 11: {
	 _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_480
	 	case 12: {
	 _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_481
	 	case 13: {
	 _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_482
	 	case 14: {
	 a = x0;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_483
	 	case 15: {
	 a = x1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_484
	 	case 16: {
	 a = y0;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_485
	 	case 17: {
	 error = error + deltaerr;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_486
	 	case 18: {
	 error = error - 1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_487
	 	case 19: {
	 if (!__is_neg())
	     if (error >= 0.5) {
	         y += ystep;
	         error = error - 1;
	     }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_488
	 	case 20: {
	 if ((error >= 0.5) && !__is_neg()) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_489
	 	case 21: {
	 if ((error >= 0.5) || __is_neg()) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_490
	 	case 22: {
	 if (__is_neg())
	     break;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_491
	 	case 23: {
	 if (__is_neg())
	     return;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_492
	 	case 24: {
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_493
	 	case 25: {
	 if (steep) {
	     a = x0;
	     x0 = y0;
	     y0 = a;
	     a = x1;
	     x1 = y1;
	     y1 = a;
	 }
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_494
	 	case 26: {
	 if (steep) {
	     var.first = y;
	     var.second = x;
	     line.push_back(var);
	 } else {
	     var.first = x;
	     var.second = y;
	     line.push_back(var);
	 }
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_495
	 	case 27: {
	 if (this->_M_end_of_storage)
	     return std::__addressof(this->_M_end_of_storage[-1]) + 1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_496
	 	case 28: {
	 if (this->_M_offset++ == int(_S_word_bit) - 1) {
	     this->_M_offset = 0;
	     ++this->_M_p;
	 }
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_497
	 	case 29: {
	 if (this->_M_offset-- == 0) {
	     this->_M_offset = int(_S_word_bit) - 1;
	     --this->_M_p;
	 }
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_498
	 	case 30: {
	 if (this->capacity() - this->size() < int(_S_word_bit))
	     return false;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_499
	 	case 31: {
	 if (this->capacity() == this->size())
	     return false;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_500
	 	case 32: {
	 if (x0 > x1) {
	     a = x0;
	     x0 = x1;
	     x1 = a;
	     a = y0;
	     y0 = y1;
	     y1 = a;
	 }
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_501
	 	case 33: {
	 if (y0 < y1)
	     ystep = 1;
	 else
	     ystep = -1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_502
	 	case 34: {
	 line.push_back(var);
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_503
	 	case 35: {
	 std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_504
	 	case 36: {
	 this->_M_bump_down();
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_505
	 	case 37: {
	 this->_M_bump_up();
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_506
	 	case 38: {
	 this->_M_deallocate();
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_507
	 	case 39: {
	 this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_508
	 	case 40: {
	 this->_M_erase_at_end(begin());
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_509
	 	case 41: {
	 this->_M_erase_at_end(this->_M_impl._M_start);
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_510
	 	case 42: {
	 this->_M_impl._M_end_of_storage = __new_start + __len;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_511
	 	case 43: {
	 this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_512
	 	case 44: {
	 this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_513
	 	case 45: {
	 this->_M_impl._M_end_of_storage = nullptr;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_514
	 	case 46: {
	 this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_515
	 	case 47: {
	 this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_516
	 	case 48: {
	 this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_517
	 	case 49: {
	 this->_M_impl._M_finish = __tmp + __old_size;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_518
	 	case 50: {
	 this->_M_impl._M_finish = nullptr;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_519
	 	case 51: {
	 this->_M_impl._M_finish = std::_Bit_iterator();
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_520
	 	case 52: {
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_521
	 	case 53: {
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_522
	 	case 54: {
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_523
	 	case 55: {
	 this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_524
	 	case 56: {
	 this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_525
	 	case 57: {
	 this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_526
	 	case 58: {
	 this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_527
	 	case 59: {
	 this->_M_impl._M_finish = this->_M_impl._M_start;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_528
	 	case 60: {
	 this->_M_impl._M_start = nullptr;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_529
	 	case 61: {
	 this->_M_impl._M_start = std::_Bit_iterator();
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_530
	 	case 62: {
	 this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_531
	 	case 63: {
	 this->_M_offset = 0;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_532
	 	case 64: {
	 this->_M_offset = int(_S_word_bit) - 1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_533
	 	case 65: {
	 this->_M_reallocate(this->size());
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_534
	 	case 66: {
	 this->_M_shrink_to_fit();
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_535
	 	case 67: {
	 this->clear();
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_536
	 	case 68: {
	 throw;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_537
	 	case 69: {
	 var.first = x;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_538
	 	case 70: {
	 var.first = y;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_539
	 	case 71: {
	 var.second = x;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_540
	 	case 72: {
	 var.second = y;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_541
	 	case 73: {
	 x0 = x1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_542
	 	case 74: {
	 x0 = y0;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_543
	 	case 75: {
	 x1 = a;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_544
	 	case 76: {
	 x1 = y1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_545
	 	case 77: {
	 y0 = a;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_546
	 	case 78: {
	 y0 = y1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_547
	 	case 79: {
	 y1 = a;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_548
	 	case 80: {
	 ystep = -1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 #ifdef COMPILE_549
	 	case 81: {
	 ystep = 1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 #endif
	 }
       }
  }

int main(int argc, char **argv)
{
  FILE *f = fopen(argv[1], "r");
  int x, y;
  //prophet generated patch
  switch(__choose("__ID11")){
  	case 0: {
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #ifdef COMPILE_834
  	case 1: {
  ++this->_M_current;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_835
  	case 2: {
  ++this->_M_impl._M_finish;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_836
  	case 3: {
  ++this->_M_p;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_837
  	case 4: {
  ++this->current;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_838
  	case 5: {
  ++this->iter;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_839
  	case 6: {
  --this->_M_current;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_840
  	case 7: {
  --this->_M_impl._M_finish;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_841
  	case 8: {
  --this->_M_p;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_842
  	case 9: {
  --this->current;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_843
  	case 10: {
  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_844
  	case 11: {
  _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_845
  	case 12: {
  _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_846
  	case 13: {
  _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_847
  	case 14: {
  fscanf((f), "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_848
  	case 15: {
  fscanf(f, __is_neg(), &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_849
  	case 16: {
  if (!__is_neg())
      fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_850
  	case 17: {
  if (__is_neg())
      return 0;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_851
  	case 18: {
  if (__is_neg())
      return 1;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_852
  	case 19: {
  if (this->_M_end_of_storage)
      return std::__addressof(this->_M_end_of_storage[-1]) + 1;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_853
  	case 20: {
  if (this->_M_offset++ == int(_S_word_bit) - 1) {
      this->_M_offset = 0;
      ++this->_M_p;
  }
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_854
  	case 21: {
  if (this->_M_offset-- == 0) {
      this->_M_offset = int(_S_word_bit) - 1;
      --this->_M_p;
  }
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_855
  	case 22: {
  if (this->capacity() - this->size() < int(_S_word_bit))
      return false;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_856
  	case 23: {
  if (this->capacity() == this->size())
      return false;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_857
  	case 24: {
  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_858
  	case 25: {
  this->_M_bump_down();
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_859
  	case 26: {
  this->_M_bump_up();
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_860
  	case 27: {
  this->_M_deallocate();
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_861
  	case 28: {
  this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_862
  	case 29: {
  this->_M_erase_at_end(begin());
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_863
  	case 30: {
  this->_M_erase_at_end(this->_M_impl._M_start);
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_864
  	case 31: {
  this->_M_impl._M_end_of_storage = __new_start + __len;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_865
  	case 32: {
  this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_866
  	case 33: {
  this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_867
  	case 34: {
  this->_M_impl._M_end_of_storage = nullptr;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_868
  	case 35: {
  this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_869
  	case 36: {
  this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_870
  	case 37: {
  this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_871
  	case 38: {
  this->_M_impl._M_finish = __tmp + __old_size;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_872
  	case 39: {
  this->_M_impl._M_finish = nullptr;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_873
  	case 40: {
  this->_M_impl._M_finish = std::_Bit_iterator();
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_874
  	case 41: {
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_875
  	case 42: {
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_876
  	case 43: {
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_877
  	case 44: {
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_878
  	case 45: {
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_879
  	case 46: {
  this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_880
  	case 47: {
  this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_881
  	case 48: {
  this->_M_impl._M_finish = this->_M_impl._M_start;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_882
  	case 49: {
  this->_M_impl._M_start = nullptr;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_883
  	case 50: {
  this->_M_impl._M_start = std::_Bit_iterator();
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_884
  	case 51: {
  this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_885
  	case 52: {
  this->_M_offset = 0;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_886
  	case 53: {
  this->_M_offset = int(_S_word_bit) - 1;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_887
  	case 54: {
  this->_M_reallocate(this->size());
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_888
  	case 55: {
  this->_M_shrink_to_fit();
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_889
  	case 56: {
  this->clear();
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  #ifdef COMPILE_890
  	case 57: {
  throw;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  #endif
  }
  //prophet generated patch
  switch(__choose("__ID12")){
  	case 0: {
  fclose(f);
  
  break;
  }
  #ifdef COMPILE_891
  	case 1: {
  ++this->_M_current;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_892
  	case 2: {
  ++this->_M_impl._M_finish;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_893
  	case 3: {
  ++this->_M_p;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_894
  	case 4: {
  ++this->current;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_895
  	case 5: {
  ++this->iter;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_896
  	case 6: {
  --this->_M_current;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_897
  	case 7: {
  --this->_M_impl._M_finish;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_898
  	case 8: {
  --this->_M_p;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_899
  	case 9: {
  --this->current;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_900
  	case 10: {
  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_901
  	case 11: {
  _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_902
  	case 12: {
  _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_903
  	case 13: {
  _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_904
  	case 14: {
  fclose((f));
  
  break;
  }
  #endif
  #ifdef COMPILE_905
  	case 15: {
  if (!__is_neg())
      fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_906
  	case 16: {
  if (__is_neg())
      return 0;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_907
  	case 17: {
  if (__is_neg())
      return 1;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_908
  	case 18: {
  if (this->_M_end_of_storage)
      return std::__addressof(this->_M_end_of_storage[-1]) + 1;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_909
  	case 19: {
  if (this->_M_offset++ == int(_S_word_bit) - 1) {
      this->_M_offset = 0;
      ++this->_M_p;
  }
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_910
  	case 20: {
  if (this->_M_offset-- == 0) {
      this->_M_offset = int(_S_word_bit) - 1;
      --this->_M_p;
  }
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_911
  	case 21: {
  if (this->capacity() - this->size() < int(_S_word_bit))
      return false;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_912
  	case 22: {
  if (this->capacity() == this->size())
      return false;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_913
  	case 23: {
  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_914
  	case 24: {
  this->_M_bump_down();
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_915
  	case 25: {
  this->_M_bump_up();
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_916
  	case 26: {
  this->_M_deallocate();
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_917
  	case 27: {
  this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_918
  	case 28: {
  this->_M_erase_at_end(begin());
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_919
  	case 29: {
  this->_M_erase_at_end(this->_M_impl._M_start);
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_920
  	case 30: {
  this->_M_impl._M_end_of_storage = __new_start + __len;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_921
  	case 31: {
  this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_922
  	case 32: {
  this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_923
  	case 33: {
  this->_M_impl._M_end_of_storage = nullptr;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_924
  	case 34: {
  this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_925
  	case 35: {
  this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_926
  	case 36: {
  this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_927
  	case 37: {
  this->_M_impl._M_finish = __tmp + __old_size;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_928
  	case 38: {
  this->_M_impl._M_finish = nullptr;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_929
  	case 39: {
  this->_M_impl._M_finish = std::_Bit_iterator();
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_930
  	case 40: {
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_931
  	case 41: {
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_932
  	case 42: {
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_933
  	case 43: {
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_934
  	case 44: {
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_935
  	case 45: {
  this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_936
  	case 46: {
  this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_937
  	case 47: {
  this->_M_impl._M_finish = this->_M_impl._M_start;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_938
  	case 48: {
  this->_M_impl._M_start = nullptr;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_939
  	case 49: {
  this->_M_impl._M_start = std::_Bit_iterator();
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_940
  	case 50: {
  this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_941
  	case 51: {
  this->_M_offset = 0;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_942
  	case 52: {
  this->_M_offset = int(_S_word_bit) - 1;
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_943
  	case 53: {
  this->_M_reallocate(this->size());
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_944
  	case 54: {
  this->_M_shrink_to_fit();
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_945
  	case 55: {
  this->clear();
  fclose(f);
  
  break;
  }
  #endif
  #ifdef COMPILE_946
  	case 56: {
  throw;
  fclose(f);
  
  break;
  }
  #endif
  }
  DiscreteLine line;
  //prophet generated patch
  switch(__choose("__ID13")){
  	case 0: {
  buggy(x, y, line);
  
  break;
  }
  #ifdef COMPILE_947
  	case 1: {
  ++this->_M_current;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_948
  	case 2: {
  ++this->_M_impl._M_finish;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_949
  	case 3: {
  ++this->_M_p;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_950
  	case 4: {
  ++this->current;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_951
  	case 5: {
  ++this->iter;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_952
  	case 6: {
  --this->_M_current;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_953
  	case 7: {
  --this->_M_impl._M_finish;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_954
  	case 8: {
  --this->_M_p;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_955
  	case 9: {
  --this->current;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_956
  	case 10: {
  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_957
  	case 11: {
  _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_958
  	case 12: {
  _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_959
  	case 13: {
  _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_960
  	case 14: {
  buggy(x, y, line);
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_961
  	case 15: {
  if (!__is_neg())
      buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_962
  	case 16: {
  if (__is_neg())
      return 0;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_963
  	case 17: {
  if (__is_neg())
      return 1;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_964
  	case 18: {
  if (this->_M_end_of_storage)
      return std::__addressof(this->_M_end_of_storage[-1]) + 1;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_965
  	case 19: {
  if (this->_M_offset++ == int(_S_word_bit) - 1) {
      this->_M_offset = 0;
      ++this->_M_p;
  }
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_966
  	case 20: {
  if (this->_M_offset-- == 0) {
      this->_M_offset = int(_S_word_bit) - 1;
      --this->_M_p;
  }
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_967
  	case 21: {
  if (this->capacity() - this->size() < int(_S_word_bit))
      return false;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_968
  	case 22: {
  if (this->capacity() == this->size())
      return false;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_969
  	case 23: {
  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_970
  	case 24: {
  this->_M_bump_down();
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_971
  	case 25: {
  this->_M_bump_up();
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_972
  	case 26: {
  this->_M_deallocate();
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_973
  	case 27: {
  this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_974
  	case 28: {
  this->_M_erase_at_end(begin());
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_975
  	case 29: {
  this->_M_erase_at_end(this->_M_impl._M_start);
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_976
  	case 30: {
  this->_M_impl._M_end_of_storage = __new_start + __len;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_977
  	case 31: {
  this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_978
  	case 32: {
  this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_979
  	case 33: {
  this->_M_impl._M_end_of_storage = nullptr;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_980
  	case 34: {
  this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_981
  	case 35: {
  this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_982
  	case 36: {
  this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_983
  	case 37: {
  this->_M_impl._M_finish = __tmp + __old_size;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_984
  	case 38: {
  this->_M_impl._M_finish = nullptr;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_985
  	case 39: {
  this->_M_impl._M_finish = std::_Bit_iterator();
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_986
  	case 40: {
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_987
  	case 41: {
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_988
  	case 42: {
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_989
  	case 43: {
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_990
  	case 44: {
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_991
  	case 45: {
  this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_992
  	case 46: {
  this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_993
  	case 47: {
  this->_M_impl._M_finish = this->_M_impl._M_start;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_994
  	case 48: {
  this->_M_impl._M_start = nullptr;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_995
  	case 49: {
  this->_M_impl._M_start = std::_Bit_iterator();
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_996
  	case 50: {
  this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_997
  	case 51: {
  this->_M_offset = 0;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_998
  	case 52: {
  this->_M_offset = int(_S_word_bit) - 1;
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_999
  	case 53: {
  this->_M_reallocate(this->size());
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_1000
  	case 54: {
  this->_M_shrink_to_fit();
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_1001
  	case 55: {
  this->clear();
  buggy(x, y, line);
  
  break;
  }
  #endif
  #ifdef COMPILE_1002
  	case 56: {
  throw;
  buggy(x, y, line);
  
  break;
  }
  #endif
  }

  for (unsigned i = 0, end = line.size(); i != line.size(); i++)
    printf("%d %d\n", line[i].first, line[i].second);
}
