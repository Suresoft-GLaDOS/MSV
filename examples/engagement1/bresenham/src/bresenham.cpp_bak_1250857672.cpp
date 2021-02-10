bool count=true;
//prophet generated patch
    switch(__choose("__ID2")){
    	case 0: {
    if (__is_neg())
        return;
    
    break;
    }
    	case 1: {
    if (__is_neg())
        return;
    
    break;
    }
    } int __is_neg(int, ...); int __abst_hole(); int __choose(char *);}
/* original courtesy of player1537, http://www.cemetech.net/forum/viewtopic.php?t=5401 */

#include <stdlib.h>
#include <stdio.h>
#include <utility>

#include "Bresenham.h"

  void buggy(unsigned x1, unsigned y1,
			    DiscreteLine & line)
  {
    //prophet generated patch
    switch(__choose("__ID32")){
    	case 0: {
    short x0 = 0, y0 = 0;
    
    break;
    }
    	case 1: {
    if (this->_M_offset++ == int(_S_word_bit) - 1) {
        this->_M_offset = 0;
        ++this->_M_p;
    }
    this->_M_offset = 0;
    this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
    ++this->_M_p;
    ++this->_M_current;
    ++this->iter;
    ++this->current;
    --this->_M_current;
    --this->current;
    --this->_M_p;
    --this->_M_impl._M_finish;
    throw;
    ++this->_M_impl._M_finish;
    this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    this->clear();
    _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
    this->_M_deallocate();
    std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
    this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
    this->_M_impl._M_start = std::_Bit_iterator();
    this->_M_impl._M_finish = std::_Bit_iterator();
    this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
    this->_M_impl._M_end_of_storage = nullptr;
    this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
    this->_M_impl._M_start = nullptr;
    this->_M_impl._M_finish = this->_M_impl._M_start;
    this->_M_impl._M_finish = nullptr;
    this->_M_impl._M_finish = __tmp + __old_size;
    this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
    this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
    _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
    this->_M_erase_at_end(this->_M_impl._M_start);
    this->_M_shrink_to_fit();
    this->_M_reallocate(this->size());
    this->_M_erase_at_end(begin());
    this->_M_bump_up();
    this->_M_bump_down();
    if (this->capacity() == this->size())
        return false;
    if (this->capacity() - this->size() < int(_S_word_bit))
        return false;
    this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
    this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
    x1 = y1;
    if (__is_neg())
        return;
    this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
    this->_M_impl._M_end_of_storage = __new_start + __len;
    this->_M_offset = int(_S_word_bit) - 1;
    if (this->_M_offset-- == 0) {
        this->_M_offset = int(_S_word_bit) - 1;
        --this->_M_p;
    }
    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
    if (this->_M_end_of_storage)
        return std::__addressof(this->_M_end_of_storage[-1]) + 1;
    short x0 = 0, y0 = 0;
    
    break;
    }
    }
    //prophet generated patch
    switch(__choose("__ID33")){
    	case 0: {
    bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    
    break;
    }
    	case 1: {
    double steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    this->_M_offset = 0;
    if (this->_M_offset++ == int(_S_word_bit) - 1) {
        this->_M_offset = 0;
        ++this->_M_p;
    }
    this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
    y0 = y1;
    x0 = x1;
    x1 = y1;
    --this->_M_p;
    ++this->iter;
    --this->_M_current;
    ++this->current;
    ++this->_M_current;
    ++this->_M_p;
    --this->current;
    --this->_M_impl._M_finish;
    ++this->_M_impl._M_finish;
    throw;
    this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    this->clear();
    this->_M_deallocate();
    std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
    _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
    this->_M_impl._M_start = std::_Bit_iterator();
    this->_M_impl._M_finish = std::_Bit_iterator();
    this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
    this->_M_impl._M_start = nullptr;
    this->_M_impl._M_finish = this->_M_impl._M_start;
    this->_M_impl._M_finish = nullptr;
    this->_M_impl._M_finish = __tmp + __old_size;
    this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
    this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
    this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
    this->_M_impl._M_end_of_storage = nullptr;
    this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
    _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
    this->_M_shrink_to_fit();
    if (this->capacity() - this->size() < int(_S_word_bit))
        return false;
    this->_M_reallocate(this->size());
    this->_M_erase_at_end(this->_M_impl._M_start);
    this->_M_erase_at_end(begin());
    this->_M_bump_up();
    this->_M_bump_down();
    if (this->capacity() == this->size())
        return false;
    x0 = y0;
    this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
    this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
    if (__is_neg())
        return;
    this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
    this->_M_impl._M_end_of_storage = __new_start + __len;
    this->_M_offset = int(_S_word_bit) - 1;
    if (this->_M_offset-- == 0) {
        this->_M_offset = int(_S_word_bit) - 1;
        --this->_M_p;
    }
    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
    if (this->_M_end_of_storage)
        return std::__addressof(this->_M_end_of_storage[-1]) + 1;
    bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    
    break;
    }
    }    //prophet generated patch
    switch(__choose("__ID34")){
    	case 0: {
    short a;
    
    break;
    }
    	case 1: {
    double a;
    if (this->_M_offset++ == int(_S_word_bit) - 1) {
        this->_M_offset = 0;
        ++this->_M_p;
    }
    this->_M_offset = 0;
    this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
    --this->_M_p;
    --this->current;
    --this->_M_current;
    ++this->iter;
    ++this->_M_p;
    ++this->current;
    ++this->_M_current;
    --this->_M_impl._M_finish;
    ++this->_M_impl._M_finish;
    throw;
    this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
    _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    this->clear();
    this->_M_deallocate();
    std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
    this->_M_impl._M_start = std::_Bit_iterator();
    this->_M_impl._M_finish = std::_Bit_iterator();
    this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
    this->_M_impl._M_start = nullptr;
    this->_M_impl._M_finish = this->_M_impl._M_start;
    this->_M_impl._M_finish = nullptr;
    this->_M_impl._M_finish = __tmp + __old_size;
    this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
    this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
    this->_M_impl._M_end_of_storage = nullptr;
    this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
    this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
    _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
    this->_M_shrink_to_fit();
    this->_M_reallocate(this->size());
    this->_M_erase_at_end(this->_M_impl._M_start);
    this->_M_erase_at_end(begin());
    this->_M_bump_up();
    this->_M_bump_down();
    if (this->capacity() == this->size())
        return false;
    if (this->capacity() - this->size() < int(_S_word_bit))
        return false;
    this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
    this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
    if (__is_neg())
        return;
    y0 = y1;
    x1 = y1;
    x0 = x1;
    x0 = y0;
    this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
    this->_M_impl._M_end_of_storage = __new_start + __len;
    this->_M_offset = int(_S_word_bit) - 1;
    if (this->_M_offset-- == 0) {
        this->_M_offset = int(_S_word_bit) - 1;
        --this->_M_p;
    }
    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
    if (this->_M_end_of_storage)
        return std::__addressof(this->_M_end_of_storage[-1]) + 1;
    short a;
    
    break;
    }
    }     //prophet generated patch
    switch(__choose("__ID35")){
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
    	case 1: {
    if ((steep) || __is_neg()) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
    }
    if ((steep) && !__is_neg()) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
    }
    if (!__is_neg())
        if (steep) {
            a = x0;
            x0 = y0;
            y0 = a;
            a = x1;
            x1 = y1;
            y1 = a;
        }
    this->_M_offset = 0;
    if (this->_M_offset++ == int(_S_word_bit) - 1) {
        this->_M_offset = 0;
        ++this->_M_p;
    }
    this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
    ++this->iter;
    --this->current;
    --this->_M_current;
    --this->_M_p;
    ++this->_M_current;
    ++this->current;
    ++this->_M_p;
    throw;
    --this->_M_impl._M_finish;
    ++this->_M_impl._M_finish;
    this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    this->clear();
    this->_M_deallocate();
    std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
    _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
    this->_M_impl._M_start = std::_Bit_iterator();
    this->_M_impl._M_finish = std::_Bit_iterator();
    this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
    this->_M_impl._M_finish = this->_M_impl._M_start;
    this->_M_impl._M_start = nullptr;
    this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
    this->_M_impl._M_finish = __tmp + __old_size;
    this->_M_impl._M_finish = nullptr;
    this->_M_impl._M_end_of_storage = nullptr;
    this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
    this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
    this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
    x0 = x1;
    x1 = a;
    a = x1;
    x1 = y1;
    y1 = a;
    y0 = y1;
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
    _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
    this->_M_reallocate(this->size());
    this->_M_shrink_to_fit();
    this->_M_erase_at_end(begin());
    this->_M_erase_at_end(this->_M_impl._M_start);
    this->_M_bump_down();
    this->_M_bump_up();
    if (this->capacity() == this->size())
        return false;
    if (this->capacity() - this->size() < int(_S_word_bit))
        return false;
    this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
    this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
    a = x0;
    y0 = a;
    x0 = y0;
    a = y0;
    if (__is_neg())
        return;
    this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
    this->_M_impl._M_end_of_storage = __new_start + __len;
    this->_M_offset = int(_S_word_bit) - 1;
    if (this->_M_offset-- == 0) {
        this->_M_offset = int(_S_word_bit) - 1;
        --this->_M_p;
    }
    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
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
    }     //prophet generated patch
    switch(__choose("__ID36")){
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
    	case 1: {
    if ((x0 > x1) && !__is_neg()) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
    }
    if ((x0 > x1) || __is_neg()) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
    }
    if (!__is_neg())
        if (x0 > x1) {
            a = x0;
            x0 = x1;
            x1 = a;
            a = y0;
            y0 = y1;
            y1 = a;
        }
    this->_M_offset = 0;
    if (this->_M_offset++ == int(_S_word_bit) - 1) {
        this->_M_offset = 0;
        ++this->_M_p;
    }
    this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
    if (__is_neg())
        return;
    a = x1;
    x0 = x1;
    x1 = a;
    y0 = y1;
    x1 = y1;
    y1 = a;
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
    a = x0;
    a = y0;
    x0 = y0;
    y0 = a;
    --this->current;
    --this->_M_p;
    --this->_M_current;
    ++this->iter;
    ++this->current;
    ++this->_M_p;
    ++this->_M_current;
    --this->_M_impl._M_finish;
    ++this->_M_impl._M_finish;
    throw;
    this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
    this->clear();
    this->_M_deallocate();
    std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
    this->_M_impl._M_start = std::_Bit_iterator();
    this->_M_impl._M_finish = std::_Bit_iterator();
    this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
    this->_M_impl._M_finish = nullptr;
    this->_M_impl._M_start = nullptr;
    this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
    this->_M_impl._M_finish = __tmp + __old_size;
    this->_M_impl._M_finish = this->_M_impl._M_start;
    this->_M_impl._M_end_of_storage = nullptr;
    this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
    this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
    this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
    _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
    this->_M_reallocate(this->size());
    this->_M_erase_at_end(this->_M_impl._M_start);
    this->_M_erase_at_end(begin());
    this->_M_bump_up();
    this->_M_bump_down();
    if (this->capacity() == this->size())
        return false;
    if (this->capacity() - this->size() < int(_S_word_bit))
        return false;
    this->_M_shrink_to_fit();
    this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
    this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
    this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
    this->_M_impl._M_end_of_storage = __new_start + __len;
    this->_M_offset = int(_S_word_bit) - 1;
    if (this->_M_offset-- == 0) {
        this->_M_offset = int(_S_word_bit) - 1;
        --this->_M_p;
    }
    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
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
    }
     //prophet generated patch
     switch(__choose("__ID37")){
     	case 0: {
     int deltax = x1 - x0;
     
     break;
     }
     	case 1: {
     double deltax = x1 - x0;
     this->_M_offset = 0;
     if (this->_M_offset++ == int(_S_word_bit) - 1) {
         this->_M_offset = 0;
         ++this->_M_p;
     }
     this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
     if (__is_neg())
         return;
     --this->_M_current;
     ++this->iter;
     ++this->current;
     --this->current;
     --this->_M_p;
     ++this->_M_p;
     ++this->_M_current;
     throw;
     --this->_M_impl._M_finish;
     ++this->_M_impl._M_finish;
     this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     this->clear();
     this->_M_deallocate();
     std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
     _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
     this->_M_impl._M_start = std::_Bit_iterator();
     this->_M_impl._M_finish = std::_Bit_iterator();
     this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
     this->_M_impl._M_start = nullptr;
     this->_M_impl._M_finish = this->_M_impl._M_start;
     this->_M_impl._M_finish = nullptr;
     this->_M_impl._M_finish = __tmp + __old_size;
     this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
     this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
     this->_M_impl._M_end_of_storage = nullptr;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
     this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
     x1 = a;
     x0 = x1;
     a = x1;
     y1 = a;
     y0 = y1;
     x1 = y1;
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
     _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
     this->_M_shrink_to_fit();
     this->_M_reallocate(this->size());
     this->_M_erase_at_end(this->_M_impl._M_start);
     this->_M_erase_at_end(begin());
     this->_M_bump_up();
     this->_M_bump_down();
     if (this->capacity() == this->size())
         return false;
     if (this->capacity() - this->size() < int(_S_word_bit))
         return false;
     this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
     a = x0;
     y0 = a;
     x0 = y0;
     a = y0;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
     this->_M_impl._M_end_of_storage = __new_start + __len;
     if (this->_M_offset-- == 0) {
         this->_M_offset = int(_S_word_bit) - 1;
         --this->_M_p;
     }
     this->_M_offset = int(_S_word_bit) - 1;
     _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
     if (this->_M_end_of_storage)
         return std::__addressof(this->_M_end_of_storage[-1]) + 1;
     int deltax = x1 - x0;
     
     break;
     }
     }     //prophet generated patch
     switch(__choose("__ID38")){
     	case 0: {
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 1: {
     double deltay = abs((int)(y1 - y0));
     this->_M_offset = 0;
     if (this->_M_offset++ == int(_S_word_bit) - 1) {
         this->_M_offset = 0;
         ++this->_M_p;
     }
     this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
     if (__is_neg())
         return;
     x1 = y1;
     y1 = a;
     y0 = y1;
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
     y0 = a;
     x0 = y0;
     a = y0;
     --this->current;
     --this->_M_p;
     --this->_M_current;
     ++this->iter;
     ++this->current;
     ++this->_M_p;
     ++this->_M_current;
     --this->_M_impl._M_finish;
     ++this->_M_impl._M_finish;
     throw;
     this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_deallocate();
     _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
     this->clear();
     this->_M_impl._M_start = std::_Bit_iterator();
     this->_M_impl._M_finish = std::_Bit_iterator();
     this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
     this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
     this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
     this->_M_impl._M_end_of_storage = nullptr;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
     this->_M_impl._M_finish = nullptr;
     this->_M_impl._M_start = nullptr;
     this->_M_impl._M_finish = __tmp + __old_size;
     this->_M_impl._M_finish = this->_M_impl._M_start;
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
     x0 = x1;
     a = x1;
     x1 = a;
     _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
     this->_M_shrink_to_fit();
     this->_M_reallocate(this->size());
     this->_M_erase_at_end(this->_M_impl._M_start);
     this->_M_erase_at_end(begin());
     this->_M_bump_up();
     this->_M_bump_down();
     if (this->capacity() == this->size())
         return false;
     if (this->capacity() - this->size() < int(_S_word_bit))
         return false;
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
     this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
     a = x0;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
     this->_M_impl._M_end_of_storage = __new_start + __len;
     if (this->_M_offset-- == 0) {
         this->_M_offset = int(_S_word_bit) - 1;
         --this->_M_p;
     }
     this->_M_offset = int(_S_word_bit) - 1;
     _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
     if (this->_M_end_of_storage)
         return std::__addressof(this->_M_end_of_storage[-1]) + 1;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     }     //prophet generated patch
     switch(__choose("__ID39")){
     	case 0: {
     float error = 0;
     
     break;
     }
     	case 1: {
     this->_M_offset = 0;
     if (this->_M_offset++ == int(_S_word_bit) - 1) {
         this->_M_offset = 0;
         ++this->_M_p;
     }
     this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
     if (__is_neg())
         return;
     --this->_M_current;
     ++this->_M_current;
     ++this->current;
     ++this->_M_p;
     --this->current;
     --this->_M_p;
     ++this->iter;
     throw;
     --this->_M_impl._M_finish;
     ++this->_M_impl._M_finish;
     this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     this->clear();
     _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
     this->_M_deallocate();
     _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_impl._M_start = std::_Bit_iterator();
     this->_M_impl._M_finish = std::_Bit_iterator();
     this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
     this->_M_impl._M_finish = this->_M_impl._M_start;
     this->_M_impl._M_start = nullptr;
     this->_M_impl._M_finish = nullptr;
     this->_M_impl._M_finish = __tmp + __old_size;
     this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
     this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
     this->_M_impl._M_end_of_storage = nullptr;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
     this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
     x1 = a;
     x0 = x1;
     a = x1;
     _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
     this->_M_reallocate(this->size());
     this->_M_erase_at_end(this->_M_impl._M_start);
     this->_M_bump_down();
     this->_M_bump_up();
     this->_M_erase_at_end(begin());
     if (this->capacity() == this->size())
         return false;
     this->_M_shrink_to_fit();
     if (this->capacity() - this->size() < int(_S_word_bit))
         return false;
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
     this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
     y1 = a;
     y0 = y1;
     x1 = y1;
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
     a = x0;
     a = y0;
     x0 = y0;
     y0 = a;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
     this->_M_impl._M_end_of_storage = __new_start + __len;
     this->_M_offset = int(_S_word_bit) - 1;
     if (this->_M_offset-- == 0) {
         this->_M_offset = int(_S_word_bit) - 1;
         --this->_M_p;
     }
     _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
     if (this->_M_end_of_storage)
         return std::__addressof(this->_M_end_of_storage[-1]) + 1;
     float error = 0;
     
     break;
     }
     }     //prophet generated patch
     switch(__choose("__ID40")){
     	case 0: {
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 1: {
     if (this->_M_offset++ == int(_S_word_bit) - 1) {
         this->_M_offset = 0;
         ++this->_M_p;
     }
     this->_M_offset = 0;
     this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
     ++this->current;
     ++this->iter;
     --this->_M_p;
     --this->_M_current;
     ++this->_M_current;
     ++this->_M_p;
     --this->current;
     --this->_M_impl._M_finish;
     ++this->_M_impl._M_finish;
     throw;
     this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     this->clear();
     std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_deallocate();
     _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
     _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     this->_M_impl._M_start = std::_Bit_iterator();
     this->_M_impl._M_finish = std::_Bit_iterator();
     this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
     this->_M_impl._M_finish = this->_M_impl._M_start;
     this->_M_impl._M_start = nullptr;
     this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
     this->_M_impl._M_finish = __tmp + __old_size;
     this->_M_impl._M_finish = nullptr;
     this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
     this->_M_impl._M_end_of_storage = nullptr;
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
     this->_M_reallocate(this->size());
     this->_M_shrink_to_fit();
     this->_M_erase_at_end(this->_M_impl._M_start);
     this->_M_erase_at_end(begin());
     this->_M_bump_up();
     this->_M_bump_down();
     if (this->capacity() - this->size() < int(_S_word_bit))
         return false;
     if (this->capacity() == this->size())
         return false;
     this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
     if (__is_neg())
         return;
     x1 = a;
     x0 = x1;
     a = x1;
     y0 = y1;
     x1 = y1;
     y1 = a;
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
     this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
     this->_M_impl._M_end_of_storage = __new_start + __len;
     a = x0;
     this->_M_offset = int(_S_word_bit) - 1;
     if (this->_M_offset-- == 0) {
         this->_M_offset = int(_S_word_bit) - 1;
         --this->_M_p;
     }
     _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
     x0 = y0;
     y0 = a;
     a = y0;
     error = error - 1;
     if (this->_M_end_of_storage)
         return std::__addressof(this->_M_end_of_storage[-1]) + 1;
     float deltaerr = deltay / deltax;
     
     break;
     }
     }     //prophet generated patch
     switch(__choose("__ID41")){
     	case 0: {
     int ystep;
     
     break;
     }
     	case 1: {
     double ystep;
     this->_M_offset = 0;
     if (this->_M_offset++ == int(_S_word_bit) - 1) {
         this->_M_offset = 0;
         ++this->_M_p;
     }
     this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
     --this->_M_p;
     --this->_M_current;
     --this->current;
     ++this->_M_p;
     ++this->current;
     ++this->iter;
     ++this->_M_current;
     throw;
     --this->_M_impl._M_finish;
     ++this->_M_impl._M_finish;
     this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     this->clear();
     std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_deallocate();
     _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
     _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     this->_M_impl._M_start = std::_Bit_iterator();
     this->_M_impl._M_finish = std::_Bit_iterator();
     this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
     this->_M_impl._M_start = nullptr;
     this->_M_impl._M_finish = this->_M_impl._M_start;
     this->_M_impl._M_finish = __tmp + __old_size;
     this->_M_impl._M_finish = nullptr;
     this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
     this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
     this->_M_impl._M_end_of_storage = nullptr;
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
     _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
     this->_M_shrink_to_fit();
     this->_M_reallocate(this->size());
     this->_M_erase_at_end(begin());
     this->_M_erase_at_end(this->_M_impl._M_start);
     if (this->capacity() - this->size() < int(_S_word_bit))
         return false;
     this->_M_bump_up();
     this->_M_bump_down();
     if (this->capacity() == this->size())
         return false;
     this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
     if (__is_neg())
         return;
     x1 = a;
     x0 = x1;
     a = x1;
     y1 = a;
     x1 = y1;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     y0 = y1;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     this->_M_impl._M_end_of_storage = __new_start + __len;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
     a = x0;
     error = error + deltaerr;
     this->_M_offset = int(_S_word_bit) - 1;
     if (this->_M_offset-- == 0) {
         this->_M_offset = int(_S_word_bit) - 1;
         --this->_M_p;
     }
     _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
     x0 = y0;
     y0 = a;
     a = y0;
     error = error - 1;
     if (this->_M_end_of_storage)
         return std::__addressof(this->_M_end_of_storage[-1]) + 1;
     int ystep;
     
     break;
     }
     }     //prophet generated patch
     switch(__choose("__ID42")){
     	case 0: {
     if (y0 < y1)
             switch(__choose("__ID0")){
         	case 0: {
         ystep = 1
         break;
         }
         	case 1: {
         if (__is_neg())
             return;
         if (!__is_neg())
             ystep = 1;
         this->_M_offset = 0;
         if (this->_M_offset++ == int(_S_word_bit) - 1) {
             this->_M_offset = 0;
             ++this->_M_p;
         }
         this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
         --this->current;
         --this->_M_p;
         --this->_M_current;
         ++this->iter;
         ++this->current;
         ++this->_M_p;
         ++this->_M_current;
         throw;
         --this->_M_impl._M_finish;
         ++this->_M_impl._M_finish;
         this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
         this->clear();
         this->_M_deallocate();
         std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
         _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
         _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
         this->_M_impl._M_finish = std::_Bit_iterator();
         this->_M_impl._M_start = std::_Bit_iterator();
         this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
         this->_M_impl._M_start = nullptr;
         this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
         this->_M_impl._M_finish = this->_M_impl._M_start;
         this->_M_impl._M_finish = __tmp + __old_size;
         this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
         this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
         this->_M_impl._M_end_of_storage = nullptr;
         this->_M_impl._M_finish = nullptr;
         this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
         this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
         this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
         this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
         this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
         _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
         this->_M_shrink_to_fit();
         this->_M_reallocate(this->size());
         this->_M_erase_at_end(this->_M_impl._M_start);
         this->_M_erase_at_end(begin());
         this->_M_bump_up();
         this->_M_bump_down();
         if (this->capacity() - this->size() < int(_S_word_bit))
             return false;
         if (this->capacity() == this->size())
             return false;
         this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
         this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
         x1 = a;
         x0 = x1;
         x1 = y1;
         y0 = y1;
         y1 = a;
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
         a = x1;
         this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
         this->_M_impl._M_end_of_storage = __new_start + __len;
         y0 = a;
         x0 = y0;
         error = error + deltaerr;
         a = y0;
         a = x0;
         if (this->_M_offset-- == 0) {
             this->_M_offset = int(_S_word_bit) - 1;
             --this->_M_p;
         }
         this->_M_offset = int(_S_word_bit) - 1;
         _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
         ystep = -1;
         if (y0 < y1)
             ystep = 1;
         else
             ystep = -1;
         error = error - 1;
         if (this->_M_end_of_storage)
             return std::__addressof(this->_M_end_of_storage[-1]) + 1;
         ystep = 1;
         ystep = 1;
         
         break;
         }
         }     else
            ystep = -1;

     //prophet generated patch
     switch(__choose("__ID43")){
     	case 0: {
     int y = y0;
     
     break;
     }
     	case 1: {
     double y = y0;
     if (__is_neg())
         return;
     this->_M_offset = 0;
     if (this->_M_offset++ == int(_S_word_bit) - 1) {
         this->_M_offset = 0;
         ++this->_M_p;
     }
     this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
     --this->current;
     ++this->iter;
     ++this->current;
     ++this->_M_p;
     ++this->_M_current;
     --this->_M_current;
     --this->_M_p;
     ++this->_M_impl._M_finish;
     --this->_M_impl._M_finish;
     throw;
     this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
     _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     this->clear();
     std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_deallocate();
     this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
     this->_M_impl._M_start = std::_Bit_iterator();
     this->_M_impl._M_finish = std::_Bit_iterator();
     this->_M_impl._M_finish = nullptr;
     this->_M_impl._M_end_of_storage = nullptr;
     this->_M_impl._M_finish = this->_M_impl._M_start;
     this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
     this->_M_impl._M_start = nullptr;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
     this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
     this->_M_impl._M_finish = __tmp + __old_size;
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
     _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
     this->_M_shrink_to_fit();
     if (this->capacity() == this->size())
         return false;
     this->_M_reallocate(this->size());
     this->_M_bump_down();
     this->_M_erase_at_end(begin());
     this->_M_erase_at_end(this->_M_impl._M_start);
     this->_M_bump_up();
     if (this->capacity() - this->size() < int(_S_word_bit))
         return false;
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
     this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
     y1 = a;
     y0 = y1;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     a = x1;
     x1 = y1;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     x1 = a;
     x0 = x1;
     error = error + deltaerr;
     this->_M_impl._M_end_of_storage = __new_start + __len;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
     y0 = a;
     a = y0;
     x0 = y0;
     a = x0;
     this->_M_offset = int(_S_word_bit) - 1;
     if (this->_M_offset-- == 0) {
         this->_M_offset = int(_S_word_bit) - 1;
         --this->_M_p;
     }
     _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
     if (y0 < y1)
         ystep = 1;
     else
         ystep = -1;
     ystep = -1;
     ystep = 1;
     error = error - 1;
     if (this->_M_end_of_storage)
         return std::__addressof(this->_M_end_of_storage[-1]) + 1;
     int y = y0;
     
     break;
     }
     }     //prophet generated patch
     switch(__choose("__ID44")){
     	case 0: {
     std::pair<int, int> var;
     
     break;
     }
     	case 1: {
     if (__is_neg())
         return;
     if (this->_M_offset++ == int(_S_word_bit) - 1) {
         this->_M_offset = 0;
         ++this->_M_p;
     }
     this->_M_offset = 0;
     this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
     ++this->_M_current;
     ++this->current;
     ++this->_M_p;
     --this->_M_p;
     --this->current;
     --this->_M_current;
     ++this->iter;
     throw;
     --this->_M_impl._M_finish;
     ++this->_M_impl._M_finish;
     this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
     this->_M_deallocate();
     this->clear();
     std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
     _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
     this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
     this->_M_impl._M_start = std::_Bit_iterator();
     this->_M_impl._M_finish = std::_Bit_iterator();
     this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
     this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
     this->_M_impl._M_finish = nullptr;
     this->_M_impl._M_finish = __tmp + __old_size;
     this->_M_impl._M_start = nullptr;
     this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
     this->_M_impl._M_finish = this->_M_impl._M_start;
     this->_M_impl._M_end_of_storage = nullptr;
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
     this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
     _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
     if (this->capacity() - this->size() < int(_S_word_bit))
         return false;
     this->_M_erase_at_end(this->_M_impl._M_start);
     this->_M_erase_at_end(begin());
     this->_M_bump_up();
     this->_M_shrink_to_fit();
     this->_M_bump_down();
     this->_M_reallocate(this->size());
     if (this->capacity() == this->size())
         return false;
     this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
     this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
     y0 = y1;
     y1 = a;
     x1 = y1;
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     x0 = x1;
     x1 = a;
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     a = x1;
     error = error + deltaerr;
     this->_M_impl._M_end_of_storage = __new_start + __len;
     this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
     y0 = a;
     a = y0;
     a = x0;
     x0 = y0;
     this->_M_offset = int(_S_word_bit) - 1;
     if (this->_M_offset-- == 0) {
         this->_M_offset = int(_S_word_bit) - 1;
         --this->_M_p;
     }
     _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
     ystep = 1;
     if (y0 < y1)
         ystep = 1;
     else
         ystep = -1;
     ystep = -1;
     error = error - 1;
     if (error >= 0.5) {
         y += ystep;
         error = error - 1;
     }
     if (this->_M_end_of_storage)
         return std::__addressof(this->_M_end_of_storage[-1]) + 1;
     std::pair<int, int> var;
     
     break;
     }
     }     for (int x = x0;x < x1;x++)
       {
	 if (steep){
	   var.first=y;
	   var.second=x;
	   line.push_back(var);}
	 else{
	   //prophet generated patch
	   switch(__choose("__ID3")){
	   	case 0: {
	   var.first = x
	   break;
	   }
	   	case 1: {
	   if (!__is_neg())
	       var.first = x;
	   if (__is_neg())
	       return;
	   if (__is_neg())
	       break;
	   this->_M_offset = 0;
	   if (this->_M_offset++ == int(_S_word_bit) - 1) {
	       this->_M_offset = 0;
	       ++this->_M_p;
	   }
	   this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
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
	   --this->current;
	   --this->_M_p;
	   ++this->iter;
	   --this->_M_current;
	   ++this->_M_p;
	   ++this->current;
	   ++this->_M_current;
	   throw;
	   --this->_M_impl._M_finish;
	   ++this->_M_impl._M_finish;
	   this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   this->clear();
	   this->_M_deallocate();
	   std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
	   this->_M_impl._M_start = std::_Bit_iterator();
	   this->_M_impl._M_finish = std::_Bit_iterator();
	   this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
	   this->_M_impl._M_finish = __tmp + __old_size;
	   this->_M_impl._M_finish = this->_M_impl._M_start;
	   this->_M_impl._M_start = nullptr;
	   this->_M_impl._M_finish = nullptr;
	   this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
	   this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
	   this->_M_impl._M_end_of_storage = nullptr;
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
	   this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
	   var.second = y;
	   var.first = y;
	   var.second = x;
	   _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
	   if (this->capacity() == this->size())
	       return false;
	   this->_M_shrink_to_fit();
	   this->_M_reallocate(this->size());
	   this->_M_erase_at_end(begin());
	   this->_M_erase_at_end(this->_M_impl._M_start);
	   this->_M_bump_up();
	   this->_M_bump_down();
	   if (this->capacity() - this->size() < int(_S_word_bit))
	       return false;
	   this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
	   y1 = a;
	   x0 = x1;
	   y0 = y1;
	   x1 = y1;
	   x1 = a;
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
	   a = x1;
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
	   this->_M_impl._M_end_of_storage = __new_start + __len;
	   x0 = y0;
	   y0 = a;
	   error = error + deltaerr;
	   a = y0;
	   a = x0;
	   if (this->_M_offset-- == 0) {
	       this->_M_offset = int(_S_word_bit) - 1;
	       --this->_M_p;
	   }
	   this->_M_offset = int(_S_word_bit) - 1;
	   _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
	   if (y0 < y1)
	       ystep = 1;
	   else
	       ystep = -1;
	   ystep = 1;
	   if (error >= 0.5) {
	       y += ystep;
	       error = error - 1;
	   }
	   error = error - 1;
	   ystep = -1;
	   if (this->_M_end_of_storage)
	       return std::__addressof(this->_M_end_of_storage[-1]) + 1;
	   var.first = x;
	   var.first = x;
	   
	   break;
	   }
	   }	   //prophet generated patch
	   switch(__choose("__ID4")){
	   	case 0: {
	   var.second = y
	   break;
	   }
	   	case 1: {
	   if (!__is_neg())
	       var.second = y;
	   if (steep) {
	       var.first = y;
	       var.second = x;
	       line.push_back(var);
	   } else {
	       var.first = x;
	       var.second = y;
	       line.push_back(var);
	   }
	   if (__is_neg())
	       return;
	   if (__is_neg())
	       break;
	   line.push_back(var);
	   this->_M_offset = 0;
	   if (this->_M_offset++ == int(_S_word_bit) - 1) {
	       this->_M_offset = 0;
	       ++this->_M_p;
	   }
	   this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
	   var.second = x;
	   var.first = x;
	   var.first = y;
	   --this->current;
	   --this->_M_p;
	   --this->_M_current;
	   ++this->current;
	   ++this->iter;
	   ++this->_M_p;
	   ++this->_M_current;
	   ++this->_M_impl._M_finish;
	   throw;
	   --this->_M_impl._M_finish;
	   this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   this->clear();
	   this->_M_deallocate();
	   std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
	   _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   this->_M_impl._M_start = std::_Bit_iterator();
	   this->_M_impl._M_finish = std::_Bit_iterator();
	   this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
	   this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
	   this->_M_impl._M_finish = this->_M_impl._M_start;
	   this->_M_impl._M_start = nullptr;
	   this->_M_impl._M_finish = nullptr;
	   this->_M_impl._M_finish = __tmp + __old_size;
	   this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
	   this->_M_impl._M_end_of_storage = nullptr;
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
	   this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
	   _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
	   this->_M_shrink_to_fit();
	   this->_M_reallocate(this->size());
	   this->_M_erase_at_end(this->_M_impl._M_start);
	   this->_M_erase_at_end(begin());
	   this->_M_bump_up();
	   this->_M_bump_down();
	   if (this->capacity() == this->size())
	       return false;
	   if (this->capacity() - this->size() < int(_S_word_bit))
	       return false;
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
	   y1 = a;
	   y0 = y1;
	   x1 = y1;
	   x0 = x1;
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
	   a = x1;
	   x1 = a;
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
	   this->_M_impl._M_end_of_storage = __new_start + __len;
	   a = x0;
	   x0 = y0;
	   a = y0;
	   y0 = a;
	   error = error + deltaerr;
	   this->_M_offset = int(_S_word_bit) - 1;
	   if (this->_M_offset-- == 0) {
	       this->_M_offset = int(_S_word_bit) - 1;
	       --this->_M_p;
	   }
	   _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
	   if (y0 < y1)
	       ystep = 1;
	   else
	       ystep = -1;
	   ystep = 1;
	   error = error - 1;
	   ystep = -1;
	   if (error >= 0.5) {
	       y += ystep;
	       error = error - 1;
	   }
	   if (this->_M_end_of_storage)
	       return std::__addressof(this->_M_end_of_storage[-1]) + 1;
	   var.second = y;
	   var.second = y;
	   
	   break;
	   }
	   }	   //prophet generated patch
	   switch(__choose("__ID5")){
	   	case 0: {
	   line.push_back(var)
	   break;
	   }
	   	case 1: {
	   if (!__is_neg())
	       line.push_back(var);
	   if (!__is_neg())
	       line.push_back(var);
	   this->_M_offset = 0;
	   if (this->_M_offset++ == int(_S_word_bit) - 1) {
	       this->_M_offset = 0;
	       ++this->_M_p;
	   }
	   this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
	   if (steep) {
	       var.first = y;
	       var.second = x;
	       line.push_back(var);
	   } else {
	       var.first = x;
	       var.second = y;
	       line.push_back(var);
	   }
	   if (__is_neg())
	       return;
	   if (__is_neg())
	       break;
	   ++this->_M_p;
	   --this->_M_p;
	   --this->current;
	   ++this->current;
	   ++this->iter;
	   --this->_M_current;
	   ++this->_M_current;
	   throw;
	   --this->_M_impl._M_finish;
	   ++this->_M_impl._M_finish;
	   this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   this->clear();
	   this->_M_deallocate();
	   std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
	   _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	   this->_M_impl._M_finish = std::_Bit_iterator();
	   this->_M_impl._M_start = std::_Bit_iterator();
	   this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
	   this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
	   this->_M_impl._M_finish = __tmp + __old_size;
	   this->_M_impl._M_finish = this->_M_impl._M_start;
	   this->_M_impl._M_start = nullptr;
	   this->_M_impl._M_finish = nullptr;
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
	   this->_M_impl._M_end_of_storage = nullptr;
	   this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	   this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
	   _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
	   this->_M_reallocate(this->size());
	   this->_M_shrink_to_fit();
	   this->_M_erase_at_end(begin());
	   this->_M_erase_at_end(this->_M_impl._M_start);
	   this->_M_bump_down();
	   this->_M_bump_up();
	   if (this->capacity() - this->size() < int(_S_word_bit))
	       return false;
	   if (this->capacity() == this->size())
	       return false;
	   this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
	   this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
	   x0 = x1;
	   x1 = y1;
	   y0 = y1;
	   y1 = a;
	   x1 = a;
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
	   a = x1;
	   var.second = y;
	   var.first = y;
	   var.second = x;
	   var.first = x;
	   this->_M_impl._M_end_of_storage = __new_start + __len;
	   this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
	   y0 = a;
	   x0 = y0;
	   error = error + deltaerr;
	   a = y0;
	   a = x0;
	   this->_M_offset = int(_S_word_bit) - 1;
	   if (this->_M_offset-- == 0) {
	       this->_M_offset = int(_S_word_bit) - 1;
	       --this->_M_p;
	   }
	   _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
	   error = error - 1;
	   ystep = -1;
	   if (error >= 0.5) {
	       y += ystep;
	       error = error - 1;
	   }
	   if (y0 < y1)
	       ystep = 1;
	   else
	       ystep = -1;
	   ystep = 1;
	   if (this->_M_end_of_storage)
	       return std::__addressof(this->_M_end_of_storage[-1]) + 1;
	   line.push_back(var);
	   
	   break;
	   }
	   }}
	 //prophet generated patch
	 switch(__choose("__ID16")){
	 	case 0: {
	 error = error + deltaerr
	 break;
	 }
	 	case 1: {
	 if (!__is_neg())
	     error = error + deltaerr;
	 if (__is_neg())
	     break;
	 if (__is_neg())
	     return;
	 this->_M_offset = 0;
	 if (this->_M_offset++ == int(_S_word_bit) - 1) {
	     this->_M_offset = 0;
	     ++this->_M_p;
	 }
	 this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
	 ++this->_M_p;
	 --this->_M_p;
	 ++this->_M_current;
	 ++this->current;
	 --this->current;
	 ++this->iter;
	 --this->_M_current;
	 ++this->_M_impl._M_finish;
	 --this->_M_impl._M_finish;
	 throw;
	 this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	 std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
	 _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
	 this->clear();
	 _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	 this->_M_deallocate();
	 this->_M_impl._M_start = std::_Bit_iterator();
	 this->_M_impl._M_finish = std::_Bit_iterator();
	 this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
	 this->_M_impl._M_finish = this->_M_impl._M_start;
	 this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
	 this->_M_impl._M_start = nullptr;
	 this->_M_impl._M_finish = nullptr;
	 this->_M_impl._M_finish = __tmp + __old_size;
	 this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
	 this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
	 this->_M_impl._M_end_of_storage = nullptr;
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
	 this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
	 this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	 line.push_back(var);
	 _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
	 this->_M_bump_down();
	 if (this->capacity() - this->size() < int(_S_word_bit))
	     return false;
	 this->_M_reallocate(this->size());
	 this->_M_erase_at_end(this->_M_impl._M_start);
	 this->_M_bump_up();
	 if (this->capacity() == this->size())
	     return false;
	 this->_M_erase_at_end(begin());
	 this->_M_shrink_to_fit();
	 this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
	 this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
	 y1 = a;
	 x1 = a;
	 x0 = x1;
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
	 y0 = y1;
	 x1 = y1;
	 a = x1;
	 this->_M_impl._M_end_of_storage = __new_start + __len;
	 this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
	 y0 = a;
	 a = x0;
	 x0 = y0;
	 a = y0;
	 this->_M_offset = int(_S_word_bit) - 1;
	 if (this->_M_offset-- == 0) {
	     this->_M_offset = int(_S_word_bit) - 1;
	     --this->_M_p;
	 }
	 _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
	 var.first = x;
	 var.second = y;
	 var.second = x;
	 var.first = y;
	 if (steep) {
	     var.first = y;
	     var.second = x;
	     line.push_back(var);
	 } else {
	     var.first = x;
	     var.second = y;
	     line.push_back(var);
	 }
	 error = error - 1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 ystep = 1;
	 ystep = -1;
	 if (y0 < y1)
	     ystep = 1;
	 else
	     ystep = -1;
	 if (this->_M_end_of_storage)
	     return std::__addressof(this->_M_end_of_storage[-1]) + 1;
	 error = error + deltaerr;
	 error = error + deltaerr;
	 
	 break;
	 }
	 }	 //prophet generated patch
	 switch(__choose("__ID17")){
	 	case 0: {
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 1: {
	 if ((error >= 0.5) && !__is_neg()) {
	     y += ystep;
	     error = error - 1;
	 }
	 if ((error >= 0.5) || __is_neg()) {
	     y += ystep;
	     error = error - 1;
	 }
	 if (!__is_neg())
	     if (error >= 0.5) {
	         y += ystep;
	         error = error - 1;
	     }
	 this->_M_offset = 0;
	 if (this->_M_offset++ == int(_S_word_bit) - 1) {
	     this->_M_offset = 0;
	     ++this->_M_p;
	 }
	 this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
	 if (__is_neg())
	     break;
	 if (__is_neg())
	     return;
	 error = error + deltaerr;
	 ++this->current;
	 --this->_M_current;
	 --this->current;
	 --this->_M_p;
	 ++this->iter;
	 ++this->_M_current;
	 ++this->_M_p;
	 ++this->_M_impl._M_finish;
	 throw;
	 --this->_M_impl._M_finish;
	 this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	 this->clear();
	 this->_M_deallocate();
	 std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
	 _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
	 _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
	 this->_M_impl._M_start = std::_Bit_iterator();
	 this->_M_impl._M_finish = std::_Bit_iterator();
	 this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
	 this->_M_impl._M_finish = __tmp + __old_size;
	 this->_M_impl._M_start = nullptr;
	 this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
	 this->_M_impl._M_finish = nullptr;
	 this->_M_impl._M_end_of_storage = nullptr;
	 this->_M_impl._M_finish = this->_M_impl._M_start;
	 this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
	 this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
	 this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
	 this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	 this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
	 line.push_back(var);
	 _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
	 this->_M_reallocate(this->size());
	 this->_M_shrink_to_fit();
	 this->_M_erase_at_end(this->_M_impl._M_start);
	 this->_M_erase_at_end(begin());
	 if (this->capacity() - this->size() < int(_S_word_bit))
	     return false;
	 this->_M_bump_down();
	 this->_M_bump_up();
	 if (this->capacity() == this->size())
	     return false;
	 this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
	 this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
	 a = x1;
	 x0 = x1;
	 x1 = a;
	 x1 = y1;
	 if (steep) {
	     a = x0;
	     x0 = y0;
	     y0 = a;
	     a = x1;
	     x1 = y1;
	     y1 = a;
	 }
	 y0 = y1;
	 if (x0 > x1) {
	     a = x0;
	     x0 = x1;
	     x1 = a;
	     a = y0;
	     y0 = y1;
	     y1 = a;
	 }
	 y1 = a;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 error = error - 1;
	 this->_M_impl._M_end_of_storage = __new_start + __len;
	 this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
	 y0 = a;
	 x0 = y0;
	 a = x0;
	 a = y0;
	 if (this->_M_offset-- == 0) {
	     this->_M_offset = int(_S_word_bit) - 1;
	     --this->_M_p;
	 }
	 this->_M_offset = int(_S_word_bit) - 1;
	 _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
	 var.second = y;
	 var.first = x;
	 var.first = y;
	 var.second = x;
	 if (steep) {
	     var.first = y;
	     var.second = x;
	     line.push_back(var);
	 } else {
	     var.first = x;
	     var.second = y;
	     line.push_back(var);
	 }
	 ystep = 1;
	 ystep = -1;
	 if (y0 < y1)
	     ystep = 1;
	 else
	     ystep = -1;
	 if (this->_M_end_of_storage)
	     return std::__addressof(this->_M_end_of_storage[-1]) + 1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 }       }
  }

int main(int argc, char **argv)
{
  //prophet generated patch
  switch(__choose("__ID70")){
  	case 0: {
  FILE *f = fopen(argv[1], "r");
  
  break;
  }
  	case 1: {
  this->_M_offset = 0;
  if (this->_M_offset++ == int(_S_word_bit) - 1) {
      this->_M_offset = 0;
      ++this->_M_p;
  }
  this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
  if (__is_neg())
      return 0;
  ++this->_M_p;
  --this->_M_p;
  --this->current;
  ++this->current;
  ++this->iter;
  --this->_M_current;
  ++this->_M_current;
  throw;
  --this->_M_impl._M_finish;
  ++this->_M_impl._M_finish;
  this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  this->clear();
  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
  this->_M_deallocate();
  _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
  _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  this->_M_impl._M_start = std::_Bit_iterator();
  this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
  this->_M_impl._M_finish = std::_Bit_iterator();
  this->_M_impl._M_finish = this->_M_impl._M_start;
  this->_M_impl._M_start = nullptr;
  this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
  this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
  this->_M_impl._M_finish = __tmp + __old_size;
  this->_M_impl._M_finish = nullptr;
  this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
  this->_M_impl._M_end_of_storage = nullptr;
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
  this->_M_reallocate(this->size());
  this->_M_shrink_to_fit();
  this->_M_erase_at_end(this->_M_impl._M_start);
  this->_M_erase_at_end(begin());
  this->_M_bump_down();
  this->_M_bump_up();
  if (this->capacity() - this->size() < int(_S_word_bit))
      return false;
  if (this->capacity() == this->size())
      return false;
  this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
  this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
  this->_M_impl._M_end_of_storage = __new_start + __len;
  this->_M_offset = int(_S_word_bit) - 1;
  if (this->_M_offset-- == 0) {
      this->_M_offset = int(_S_word_bit) - 1;
      --this->_M_p;
  }
  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
  if (__is_neg())
      return 1;
  if (this->_M_end_of_storage)
      return std::__addressof(this->_M_end_of_storage[-1]) + 1;
  FILE *f = fopen(argv[1], "r");
  
  break;
  }
  }  //prophet generated patch
  switch(__choose("__ID71")){
  	case 0: {
  int x, y;
  
  break;
  }
  	case 1: {
  this->_M_offset = 0;
  if (this->_M_offset++ == int(_S_word_bit) - 1) {
      this->_M_offset = 0;
      ++this->_M_p;
  }
  this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
  ++this->iter;
  --this->_M_current;
  --this->current;
  --this->_M_p;
  ++this->_M_current;
  ++this->_M_p;
  ++this->current;
  throw;
  --this->_M_impl._M_finish;
  ++this->_M_impl._M_finish;
  this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  this->clear();
  this->_M_deallocate();
  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
  _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
  this->_M_impl._M_start = std::_Bit_iterator();
  this->_M_impl._M_finish = std::_Bit_iterator();
  this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
  this->_M_impl._M_finish = this->_M_impl._M_start;
  this->_M_impl._M_start = nullptr;
  this->_M_impl._M_finish = __tmp + __old_size;
  this->_M_impl._M_finish = nullptr;
  this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
  this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
  this->_M_impl._M_end_of_storage = nullptr;
  this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
  _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
  this->_M_reallocate(this->size());
  this->_M_shrink_to_fit();
  this->_M_erase_at_end(this->_M_impl._M_start);
  this->_M_bump_up();
  this->_M_erase_at_end(begin());
  if (this->capacity() - this->size() < int(_S_word_bit))
      return false;
  this->_M_bump_down();
  if (this->capacity() == this->size())
      return false;
  this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
  if (__is_neg())
      return 0;
  this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
  this->_M_impl._M_end_of_storage = __new_start + __len;
  this->_M_offset = int(_S_word_bit) - 1;
  if (this->_M_offset-- == 0) {
      this->_M_offset = int(_S_word_bit) - 1;
      --this->_M_p;
  }
  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
  if (__is_neg())
      return 1;
  if (this->_M_end_of_storage)
      return std::__addressof(this->_M_end_of_storage[-1]) + 1;
  int x, y;
  
  break;
  }
  }  //prophet generated patch
  switch(__choose("__ID72")){
  	case 0: {
  fscanf(f, "%d %d\n", &x, &y)
  break;
  }
  	case 1: {
  fscanf(f, __is_neg(), &x, &y);
  if (this->_M_offset++ == int(_S_word_bit) - 1) {
      this->_M_offset = 0;
      ++this->_M_p;
  }
  this->_M_offset = 0;
  this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
  if (!__is_neg())
      fscanf(f, "%d %d\n", &x, &y);
  ++this->current;
  --this->_M_p;
  --this->current;
  ++this->iter;
  --this->_M_current;
  ++this->_M_current;
  ++this->_M_p;
  throw;
  --this->_M_impl._M_finish;
  ++this->_M_impl._M_finish;
  this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  this->clear();
  _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
  this->_M_deallocate();
  _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
  this->_M_impl._M_start = std::_Bit_iterator();
  this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
  this->_M_impl._M_finish = std::_Bit_iterator();
  this->_M_impl._M_start = nullptr;
  this->_M_impl._M_finish = this->_M_impl._M_start;
  this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
  this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
  this->_M_impl._M_finish = nullptr;
  this->_M_impl._M_finish = __tmp + __old_size;
  this->_M_impl._M_end_of_storage = nullptr;
  this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
  this->_M_reallocate(this->size());
  this->_M_shrink_to_fit();
  this->_M_erase_at_end(begin());
  this->_M_erase_at_end(this->_M_impl._M_start);
  this->_M_bump_down();
  this->_M_bump_up();
  if (this->capacity() - this->size() < int(_S_word_bit))
      return false;
  if (this->capacity() == this->size())
      return false;
  this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
  if (__is_neg())
      return 0;
  fscanf((f), "%d %d\n", &x, &y);
  this->_M_impl._M_end_of_storage = __new_start + __len;
  this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
  this->_M_offset = int(_S_word_bit) - 1;
  if (this->_M_offset-- == 0) {
      this->_M_offset = int(_S_word_bit) - 1;
      --this->_M_p;
  }
  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
  if (__is_neg())
      return 1;
  if (this->_M_end_of_storage)
      return std::__addressof(this->_M_end_of_storage[-1]) + 1;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  }  //prophet generated patch
  switch(__choose("__ID73")){
  	case 0: {
  fclose(f)
  break;
  }
  	case 1: {
  if (!__is_neg())
      fclose(f);
  fclose((f));
  this->_M_offset = 0;
  if (this->_M_offset++ == int(_S_word_bit) - 1) {
      this->_M_offset = 0;
      ++this->_M_p;
  }
  this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
  if (__is_neg())
      return 0;
  ++this->current;
  --this->_M_p;
  --this->current;
  ++this->iter;
  --this->_M_current;
  ++this->_M_p;
  ++this->_M_current;
  throw;
  ++this->_M_impl._M_finish;
  --this->_M_impl._M_finish;
  this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  this->clear();
  this->_M_deallocate();
  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
  _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
  this->_M_impl._M_start = std::_Bit_iterator();
  this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
  this->_M_impl._M_finish = std::_Bit_iterator();
  this->_M_impl._M_start = nullptr;
  this->_M_impl._M_finish = this->_M_impl._M_start;
  this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
  this->_M_impl._M_finish = __tmp + __old_size;
  this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
  this->_M_impl._M_end_of_storage = nullptr;
  this->_M_impl._M_finish = nullptr;
  this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
  this->_M_reallocate(this->size());
  this->_M_shrink_to_fit();
  this->_M_erase_at_end(begin());
  this->_M_erase_at_end(this->_M_impl._M_start);
  this->_M_bump_down();
  if (this->capacity() - this->size() < int(_S_word_bit))
      return false;
  if (this->capacity() == this->size())
      return false;
  this->_M_bump_up();
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
  this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
  this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
  this->_M_impl._M_end_of_storage = __new_start + __len;
  if (this->_M_offset-- == 0) {
      this->_M_offset = int(_S_word_bit) - 1;
      --this->_M_p;
  }
  this->_M_offset = int(_S_word_bit) - 1;
  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
  if (__is_neg())
      return 1;
  if (this->_M_end_of_storage)
      return std::__addressof(this->_M_end_of_storage[-1]) + 1;
  fclose(f);
  
  break;
  }
  }  //prophet generated patch
  switch(__choose("__ID74")){
  	case 0: {
  DiscreteLine line;
  
  break;
  }
  	case 1: {
  this->_M_offset = 0;
  if (this->_M_offset++ == int(_S_word_bit) - 1) {
      this->_M_offset = 0;
      ++this->_M_p;
  }
  this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
  --this->_M_current;
  --this->_M_p;
  --this->current;
  ++this->current;
  ++this->iter;
  ++this->_M_p;
  ++this->_M_current;
  throw;
  --this->_M_impl._M_finish;
  ++this->_M_impl._M_finish;
  this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  this->clear();
  this->_M_deallocate();
  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
  _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
  _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
  this->_M_impl._M_start = std::_Bit_iterator();
  this->_M_impl._M_finish = std::_Bit_iterator();
  this->_M_impl._M_finish = this->_M_impl._M_start;
  this->_M_impl._M_start = nullptr;
  this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
  this->_M_impl._M_finish = nullptr;
  this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
  this->_M_impl._M_finish = __tmp + __old_size;
  this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
  this->_M_impl._M_end_of_storage = nullptr;
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
  if (__is_neg())
      return 0;
  _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
  this->_M_reallocate(this->size());
  this->_M_erase_at_end(begin());
  this->_M_bump_down();
  this->_M_bump_up();
  if (this->capacity() == this->size())
      return false;
  if (this->capacity() - this->size() < int(_S_word_bit))
      return false;
  this->_M_shrink_to_fit();
  this->_M_erase_at_end(this->_M_impl._M_start);
  this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
  this->_M_impl._M_end_of_storage = __new_start + __len;
  this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
  if (this->_M_offset-- == 0) {
      this->_M_offset = int(_S_word_bit) - 1;
      --this->_M_p;
  }
  this->_M_offset = int(_S_word_bit) - 1;
  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
  if (__is_neg())
      return 1;
  if (this->_M_end_of_storage)
      return std::__addressof(this->_M_end_of_storage[-1]) + 1;
  DiscreteLine line;
  
  break;
  }
  }  //prophet generated patch
  switch(__choose("__ID75")){
  	case 0: {
  buggy(x, y, line)
  break;
  }
  	case 1: {
  if (!__is_neg())
      buggy(x, y, line);
  this->_M_offset = 0;
  if (this->_M_offset++ == int(_S_word_bit) - 1) {
      this->_M_offset = 0;
      ++this->_M_p;
  }
  this->_M_impl._M_start = std::vector<bool, type-parameter-0-0>::iterator(0, 0);
  buggy(x, y, line);
  if (__is_neg())
      return 0;
  --this->_M_current;
  --this->_M_p;
  --this->current;
  ++this->_M_p;
  ++this->iter;
  ++this->current;
  ++this->_M_current;
  throw;
  --this->_M_impl._M_finish;
  ++this->_M_impl._M_finish;
  this->_M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  this->_M_deallocate();
  std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());
  _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
  this->clear();
  _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
  this->_M_impl._M_start = std::_Bit_iterator();
  this->_M_impl._M_finish = std::_Bit_iterator();
  this->_M_impl._M_end_of_storage = std::_Bvector_base::_Bit_pointer();
  this->_M_impl._M_finish = this->_M_impl._M_start;
  this->_M_impl._M_start = nullptr;
  this->_M_impl._M_finish = nullptr;
  this->_M_impl._M_end_of_storage = this->_M_impl._M_finish;
  this->_M_impl._M_end_of_storage = std::vector<bool, type-parameter-0-0>::_Bit_pointer();
  this->_M_impl._M_end_of_storage = __q + _S_nword(__n);
  this->_M_impl._M_end_of_storage = nullptr;
  this->_M_impl._M_finish = __tmp + __old_size;
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__x.begin(), __x.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_move_a(__rv.begin(), __rv.end(), this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__first, __last, this->_M_impl._M_start, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_copy_a(__mid, __last, this->_M_impl._M_finish, _M_get_Tp_allocator());
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value, _M_get_Tp_allocator());
  _Alloc_traits::destroy(this->_M_this->_M_impl, this->_M_ptr());
  this->_M_erase_at_end(this->_M_impl._M_start);
  this->_M_shrink_to_fit();
  this->_M_reallocate(this->size());
  this->_M_erase_at_end(begin());
  this->_M_bump_down();
  this->_M_bump_up();
  if (this->capacity() == this->size())
      return false;
  if (this->capacity() - this->size() < int(_S_word_bit))
      return false;
  this->_M_impl._M_finish = this->_M_copy_aligned(__x.begin(), __x.end(), begin());
  this->_M_impl._M_finish = std::__uninitialized_fill_n_a(this->_M_impl._M_finish, __n - this->size(), __val, _M_get_Tp_allocator());
  this->_M_impl._M_end_of_storage = __q + _S_nword(__len);
  this->_M_impl._M_end_of_storage = __new_start + __len;
  this->_M_offset = int(_S_word_bit) - 1;
  if (this->_M_offset-- == 0) {
      this->_M_offset = int(_S_word_bit) - 1;
      --this->_M_p;
  }
  _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, std::move(*(this->_M_impl._M_finish - 1)));
  if (__is_neg())
      return 1;
  if (this->_M_end_of_storage)
      return std::__addressof(this->_M_end_of_storage[-1]) + 1;
  buggy(x, y, line);
  
  break;
  }
  }
  for (unsigned i = 0, end = line.size(); i != line.size(); i++)
    printf("%d %d\n", line[i].first, line[i].second);
}
