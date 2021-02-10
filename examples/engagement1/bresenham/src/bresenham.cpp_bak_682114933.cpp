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
    }
 int __is_neg(int, ...); int __abst_hole(); int __choose(char *);}
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
    
    break;
    }
    	case 2: {
    if (__is_neg())
        return;
    short x0 = 0, y0 = 0;
    
    break;
    }
    	case 3: {
    if (this->capacity() == this->size())
        return false;
    short x0 = 0, y0 = 0;
    
    break;
    }
    	case 4: {
    this->clear();
    short x0 = 0, y0 = 0;
    
    break;
    }
    	case 5: {
    throw;
    short x0 = 0, y0 = 0;
    
    break;
    }
    	case 6: {
    x1 = y1;
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
    
    break;
    }
    	case 2: {
    double steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    
    break;
    }
    	case 3: {
    if (__is_neg())
        return;
    bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    
    break;
    }
    	case 4: {
    if (this->capacity() == this->size())
        return false;
    bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    
    break;
    }
    	case 5: {
    this->clear();
    bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    
    break;
    }
    	case 6: {
    throw;
    bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    
    break;
    }
    	case 7: {
    x0 = x1;
    bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    
    break;
    }
    	case 8: {
    x0 = y0;
    bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    
    break;
    }
    	case 9: {
    x1 = y1;
    bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    
    break;
    }
    	case 10: {
    y0 = y1;
    bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
    
    break;
    }
    }
    //prophet generated patch
    switch(__choose("__ID34")){
    	case 0: {
    short a;
    
    break;
    }
    	case 1: {
    
    break;
    }
    	case 2: {
    double a;
    
    break;
    }
    	case 3: {
    if (__is_neg())
        return;
    short a;
    
    break;
    }
    	case 4: {
    if (this->capacity() == this->size())
        return false;
    short a;
    
    break;
    }
    	case 5: {
    this->clear();
    short a;
    
    break;
    }
    	case 6: {
    throw;
    short a;
    
    break;
    }
    	case 7: {
    x0 = x1;
    short a;
    
    break;
    }
    	case 8: {
    x0 = y0;
    short a;
    
    break;
    }
    	case 9: {
    x1 = y1;
    short a;
    
    break;
    }
    	case 10: {
    y0 = y1;
    short a;
    
    break;
    }
    }
     //prophet generated patch
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
    
    break;
    }
    	case 2: {
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
    	case 3: {
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
    	case 4: {
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
    	case 5: {
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
    	case 6: {
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
    	case 7: {
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
    	case 8: {
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
    	case 9: {
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
    	case 10: {
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
    	case 11: {
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
    	case 12: {
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
    	case 13: {
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
    	case 14: {
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
    	case 15: {
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
    	case 16: {
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
    	case 17: {
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
    	case 18: {
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
    	case 19: {
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
    	case 20: {
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
    }
     //prophet generated patch
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
    
    break;
    }
    	case 2: {
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
    	case 3: {
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
    	case 4: {
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
    	case 5: {
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
    	case 6: {
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
    	case 7: {
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
    	case 8: {
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
    	case 9: {
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
    	case 10: {
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
    	case 11: {
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
    	case 12: {
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
    	case 13: {
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
    	case 14: {
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
    	case 15: {
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
    	case 16: {
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
    	case 17: {
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
    	case 18: {
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
    	case 19: {
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
    	case 20: {
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
    }

     //prophet generated patch
     switch(__choose("__ID37")){
     	case 0: {
     int deltax = x1 - x0;
     
     break;
     }
     	case 1: {
     
     break;
     }
     	case 2: {
     a = x0;
     int deltax = x1 - x0;
     
     break;
     }
     	case 3: {
     a = x1;
     int deltax = x1 - x0;
     
     break;
     }
     	case 4: {
     a = y0;
     int deltax = x1 - x0;
     
     break;
     }
     	case 5: {
     double deltax = x1 - x0;
     
     break;
     }
     	case 6: {
     if (__is_neg())
         return;
     int deltax = x1 - x0;
     
     break;
     }
     	case 7: {
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     int deltax = x1 - x0;
     
     break;
     }
     	case 8: {
     if (this->capacity() == this->size())
         return false;
     int deltax = x1 - x0;
     
     break;
     }
     	case 9: {
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     int deltax = x1 - x0;
     
     break;
     }
     	case 10: {
     this->clear();
     int deltax = x1 - x0;
     
     break;
     }
     	case 11: {
     throw;
     int deltax = x1 - x0;
     
     break;
     }
     	case 12: {
     x0 = x1;
     int deltax = x1 - x0;
     
     break;
     }
     	case 13: {
     x0 = y0;
     int deltax = x1 - x0;
     
     break;
     }
     	case 14: {
     x1 = a;
     int deltax = x1 - x0;
     
     break;
     }
     	case 15: {
     x1 = y1;
     int deltax = x1 - x0;
     
     break;
     }
     	case 16: {
     y0 = a;
     int deltax = x1 - x0;
     
     break;
     }
     	case 17: {
     y0 = y1;
     int deltax = x1 - x0;
     
     break;
     }
     	case 18: {
     y1 = a;
     int deltax = x1 - x0;
     
     break;
     }
     }
     //prophet generated patch
     switch(__choose("__ID38")){
     	case 0: {
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 1: {
     
     break;
     }
     	case 2: {
     a = x0;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 3: {
     a = x1;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 4: {
     a = y0;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 5: {
     double deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 6: {
     if (__is_neg())
         return;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 7: {
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 8: {
     if (this->capacity() == this->size())
         return false;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 9: {
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 10: {
     this->clear();
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 11: {
     throw;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 12: {
     x0 = x1;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 13: {
     x0 = y0;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 14: {
     x1 = a;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 15: {
     x1 = y1;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 16: {
     y0 = a;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 17: {
     y0 = y1;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     	case 18: {
     y1 = a;
     int deltay = abs((int)(y1 - y0));
     
     break;
     }
     }
     //prophet generated patch
     switch(__choose("__ID39")){
     	case 0: {
     float error = 0;
     
     break;
     }
     	case 1: {
     
     break;
     }
     	case 2: {
     a = x0;
     float error = 0;
     
     break;
     }
     	case 3: {
     a = x1;
     float error = 0;
     
     break;
     }
     	case 4: {
     a = y0;
     float error = 0;
     
     break;
     }
     	case 5: {
     if (__is_neg())
         return;
     float error = 0;
     
     break;
     }
     	case 6: {
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     float error = 0;
     
     break;
     }
     	case 7: {
     if (this->capacity() == this->size())
         return false;
     float error = 0;
     
     break;
     }
     	case 8: {
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     float error = 0;
     
     break;
     }
     	case 9: {
     this->clear();
     float error = 0;
     
     break;
     }
     	case 10: {
     throw;
     float error = 0;
     
     break;
     }
     	case 11: {
     x0 = x1;
     float error = 0;
     
     break;
     }
     	case 12: {
     x0 = y0;
     float error = 0;
     
     break;
     }
     	case 13: {
     x1 = a;
     float error = 0;
     
     break;
     }
     	case 14: {
     x1 = y1;
     float error = 0;
     
     break;
     }
     	case 15: {
     y0 = a;
     float error = 0;
     
     break;
     }
     	case 16: {
     y0 = y1;
     float error = 0;
     
     break;
     }
     	case 17: {
     y1 = a;
     float error = 0;
     
     break;
     }
     }
     //prophet generated patch
     switch(__choose("__ID40")){
     	case 0: {
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 1: {
     
     break;
     }
     	case 2: {
     a = x0;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 3: {
     a = x1;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 4: {
     a = y0;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 5: {
     error = error - 1;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 6: {
     if (__is_neg())
         return;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 7: {
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 8: {
     if (this->capacity() == this->size())
         return false;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 9: {
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 10: {
     this->clear();
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 11: {
     throw;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 12: {
     x0 = x1;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 13: {
     x0 = y0;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 14: {
     x1 = a;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 15: {
     x1 = y1;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 16: {
     y0 = a;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 17: {
     y0 = y1;
     float deltaerr = deltay / deltax;
     
     break;
     }
     	case 18: {
     y1 = a;
     float deltaerr = deltay / deltax;
     
     break;
     }
     }
     //prophet generated patch
     switch(__choose("__ID41")){
     	case 0: {
     int ystep;
     
     break;
     }
     	case 1: {
     
     break;
     }
     	case 2: {
     a = x0;
     int ystep;
     
     break;
     }
     	case 3: {
     a = x1;
     int ystep;
     
     break;
     }
     	case 4: {
     a = y0;
     int ystep;
     
     break;
     }
     	case 5: {
     double ystep;
     
     break;
     }
     	case 6: {
     error = error + deltaerr;
     int ystep;
     
     break;
     }
     	case 7: {
     error = error - 1;
     int ystep;
     
     break;
     }
     	case 8: {
     if (__is_neg())
         return;
     int ystep;
     
     break;
     }
     	case 9: {
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     int ystep;
     
     break;
     }
     	case 10: {
     if (this->capacity() == this->size())
         return false;
     int ystep;
     
     break;
     }
     	case 11: {
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     int ystep;
     
     break;
     }
     	case 12: {
     this->clear();
     int ystep;
     
     break;
     }
     	case 13: {
     throw;
     int ystep;
     
     break;
     }
     	case 14: {
     x0 = x1;
     int ystep;
     
     break;
     }
     	case 15: {
     x0 = y0;
     int ystep;
     
     break;
     }
     	case 16: {
     x1 = a;
     int ystep;
     
     break;
     }
     	case 17: {
     x1 = y1;
     int ystep;
     
     break;
     }
     	case 18: {
     y0 = a;
     int ystep;
     
     break;
     }
     	case 19: {
     y0 = y1;
     int ystep;
     
     break;
     }
     	case 20: {
     y1 = a;
     int ystep;
     
     break;
     }
     }
     //prophet generated patch
     switch(__choose("__ID42")){
     	case 0: {
     if (y0 < y1)
             switch(__choose("__ID0")){
         	case 0: {
         ystep = 1
         break;
         }
         	case 1: {
         
         break;
         }
         	case 2: {
         a = x0;
         ystep = 1;
         
         break;
         }
         	case 3: {
         a = x1;
         ystep = 1;
         
         break;
         }
         	case 4: {
         a = y0;
         ystep = 1;
         
         break;
         }
         	case 5: {
         error = error + deltaerr;
         ystep = 1;
         
         break;
         }
         	case 6: {
         error = error - 1;
         ystep = 1;
         
         break;
         }
         	case 7: {
         if (!__is_neg())
             ystep = 1;
         
         break;
         }
         	case 8: {
         if (__is_neg())
             return;
         ystep = 1;
         
         break;
         }
         	case 9: {
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
         	case 10: {
         if (this->capacity() == this->size())
             return false;
         ystep = 1;
         
         break;
         }
         	case 11: {
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
         	case 12: {
         if (y0 < y1)
             ystep = 1;
         else
             ystep = -1;
         ystep = 1;
         
         break;
         }
         	case 13: {
         this->clear();
         ystep = 1;
         
         break;
         }
         	case 14: {
         throw;
         ystep = 1;
         
         break;
         }
         	case 15: {
         x0 = x1;
         ystep = 1;
         
         break;
         }
         	case 16: {
         x0 = y0;
         ystep = 1;
         
         break;
         }
         	case 17: {
         x1 = a;
         ystep = 1;
         
         break;
         }
         	case 18: {
         x1 = y1;
         ystep = 1;
         
         break;
         }
         	case 19: {
         y0 = a;
         ystep = 1;
         
         break;
         }
         	case 20: {
         y0 = y1;
         ystep = 1;
         
         break;
         }
         	case 21: {
         y1 = a;
         ystep = 1;
         
         break;
         }
         	case 22: {
         ystep = -1;
         ystep = 1;
         
         break;
         }
         	case 23: {
         ystep = 1;
         ystep = 1;
         
         break;
         }
         }
          else
            ystep = -1;

     //prophet generated patch
     switch(__choose("__ID43")){
     	case 0: {
     int y = y0;
     
     break;
     }
     	case 1: {
     
     break;
     }
     	case 2: {
     a = x0;
     int y = y0;
     
     break;
     }
     	case 3: {
     a = x1;
     int y = y0;
     
     break;
     }
     	case 4: {
     a = y0;
     int y = y0;
     
     break;
     }
     	case 5: {
     double y = y0;
     
     break;
     }
     	case 6: {
     error = error + deltaerr;
     int y = y0;
     
     break;
     }
     	case 7: {
     error = error - 1;
     int y = y0;
     
     break;
     }
     	case 8: {
     if (__is_neg())
         return;
     int y = y0;
     
     break;
     }
     	case 9: {
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     int y = y0;
     
     break;
     }
     	case 10: {
     if (this->capacity() == this->size())
         return false;
     int y = y0;
     
     break;
     }
     	case 11: {
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     int y = y0;
     
     break;
     }
     	case 12: {
     if (y0 < y1)
         ystep = 1;
     else
         ystep = -1;
     int y = y0;
     
     break;
     }
     	case 13: {
     this->clear();
     int y = y0;
     
     break;
     }
     	case 14: {
     throw;
     int y = y0;
     
     break;
     }
     	case 15: {
     x0 = x1;
     int y = y0;
     
     break;
     }
     	case 16: {
     x0 = y0;
     int y = y0;
     
     break;
     }
     	case 17: {
     x1 = a;
     int y = y0;
     
     break;
     }
     	case 18: {
     x1 = y1;
     int y = y0;
     
     break;
     }
     	case 19: {
     y0 = a;
     int y = y0;
     
     break;
     }
     	case 20: {
     y0 = y1;
     int y = y0;
     
     break;
     }
     	case 21: {
     y1 = a;
     int y = y0;
     
     break;
     }
     	case 22: {
     ystep = -1;
     int y = y0;
     
     break;
     }
     	case 23: {
     ystep = 1;
     int y = y0;
     
     break;
     }
     }
     //prophet generated patch
     switch(__choose("__ID44")){
     	case 0: {
     std::pair<int, int> var;
     
     break;
     }
     	case 1: {
     
     break;
     }
     	case 2: {
     a = x0;
     std::pair<int, int> var;
     
     break;
     }
     	case 3: {
     a = x1;
     std::pair<int, int> var;
     
     break;
     }
     	case 4: {
     a = y0;
     std::pair<int, int> var;
     
     break;
     }
     	case 5: {
     error = error + deltaerr;
     std::pair<int, int> var;
     
     break;
     }
     	case 6: {
     error = error - 1;
     std::pair<int, int> var;
     
     break;
     }
     	case 7: {
     if (__is_neg())
         return;
     std::pair<int, int> var;
     
     break;
     }
     	case 8: {
     if (error >= 0.5) {
         y += ystep;
         error = error - 1;
     }
     std::pair<int, int> var;
     
     break;
     }
     	case 9: {
     if (steep) {
         a = x0;
         x0 = y0;
         y0 = a;
         a = x1;
         x1 = y1;
         y1 = a;
     }
     std::pair<int, int> var;
     
     break;
     }
     	case 10: {
     if (this->capacity() == this->size())
         return false;
     std::pair<int, int> var;
     
     break;
     }
     	case 11: {
     if (x0 > x1) {
         a = x0;
         x0 = x1;
         x1 = a;
         a = y0;
         y0 = y1;
         y1 = a;
     }
     std::pair<int, int> var;
     
     break;
     }
     	case 12: {
     if (y0 < y1)
         ystep = 1;
     else
         ystep = -1;
     std::pair<int, int> var;
     
     break;
     }
     	case 13: {
     this->clear();
     std::pair<int, int> var;
     
     break;
     }
     	case 14: {
     throw;
     std::pair<int, int> var;
     
     break;
     }
     	case 15: {
     x0 = x1;
     std::pair<int, int> var;
     
     break;
     }
     	case 16: {
     x0 = y0;
     std::pair<int, int> var;
     
     break;
     }
     	case 17: {
     x1 = a;
     std::pair<int, int> var;
     
     break;
     }
     	case 18: {
     x1 = y1;
     std::pair<int, int> var;
     
     break;
     }
     	case 19: {
     y0 = a;
     std::pair<int, int> var;
     
     break;
     }
     	case 20: {
     y0 = y1;
     std::pair<int, int> var;
     
     break;
     }
     	case 21: {
     y1 = a;
     std::pair<int, int> var;
     
     break;
     }
     	case 22: {
     ystep = -1;
     std::pair<int, int> var;
     
     break;
     }
     	case 23: {
     ystep = 1;
     std::pair<int, int> var;
     
     break;
     }
     }
     for (int x = x0;x < x1;x++)
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
	   
	   break;
	   }
	   	case 2: {
	   a = x0;
	   var.first = x;
	   
	   break;
	   }
	   	case 3: {
	   a = x1;
	   var.first = x;
	   
	   break;
	   }
	   	case 4: {
	   a = y0;
	   var.first = x;
	   
	   break;
	   }
	   	case 5: {
	   error = error + deltaerr;
	   var.first = x;
	   
	   break;
	   }
	   	case 6: {
	   error = error - 1;
	   var.first = x;
	   
	   break;
	   }
	   	case 7: {
	   if (!__is_neg())
	       var.first = x;
	   
	   break;
	   }
	   	case 8: {
	   if (__is_neg())
	       break;
	   var.first = x;
	   
	   break;
	   }
	   	case 9: {
	   if (__is_neg())
	       return;
	   var.first = x;
	   
	   break;
	   }
	   	case 10: {
	   if (error >= 0.5) {
	       y += ystep;
	       error = error - 1;
	   }
	   var.first = x;
	   
	   break;
	   }
	   	case 11: {
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
	   	case 12: {
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
	   	case 13: {
	   if (this->capacity() == this->size())
	       return false;
	   var.first = x;
	   
	   break;
	   }
	   	case 14: {
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
	   	case 15: {
	   if (y0 < y1)
	       ystep = 1;
	   else
	       ystep = -1;
	   var.first = x;
	   
	   break;
	   }
	   	case 16: {
	   line.push_back(var);
	   var.first = x;
	   
	   break;
	   }
	   	case 17: {
	   this->clear();
	   var.first = x;
	   
	   break;
	   }
	   	case 18: {
	   throw;
	   var.first = x;
	   
	   break;
	   }
	   	case 19: {
	   var.first = x;
	   var.first = x;
	   
	   break;
	   }
	   	case 20: {
	   var.first = y;
	   var.first = x;
	   
	   break;
	   }
	   	case 21: {
	   var.second = x;
	   var.first = x;
	   
	   break;
	   }
	   	case 22: {
	   var.second = y;
	   var.first = x;
	   
	   break;
	   }
	   	case 23: {
	   x0 = x1;
	   var.first = x;
	   
	   break;
	   }
	   	case 24: {
	   x0 = y0;
	   var.first = x;
	   
	   break;
	   }
	   	case 25: {
	   x1 = a;
	   var.first = x;
	   
	   break;
	   }
	   	case 26: {
	   x1 = y1;
	   var.first = x;
	   
	   break;
	   }
	   	case 27: {
	   y0 = a;
	   var.first = x;
	   
	   break;
	   }
	   	case 28: {
	   y0 = y1;
	   var.first = x;
	   
	   break;
	   }
	   	case 29: {
	   y1 = a;
	   var.first = x;
	   
	   break;
	   }
	   	case 30: {
	   ystep = -1;
	   var.first = x;
	   
	   break;
	   }
	   	case 31: {
	   ystep = 1;
	   var.first = x;
	   
	   break;
	   }
	   }
	   //prophet generated patch
	   switch(__choose("__ID4")){
	   	case 0: {
	   var.second = y
	   break;
	   }
	   	case 1: {
	   
	   break;
	   }
	   	case 2: {
	   a = x0;
	   var.second = y;
	   
	   break;
	   }
	   	case 3: {
	   a = x1;
	   var.second = y;
	   
	   break;
	   }
	   	case 4: {
	   a = y0;
	   var.second = y;
	   
	   break;
	   }
	   	case 5: {
	   error = error + deltaerr;
	   var.second = y;
	   
	   break;
	   }
	   	case 6: {
	   error = error - 1;
	   var.second = y;
	   
	   break;
	   }
	   	case 7: {
	   if (!__is_neg())
	       var.second = y;
	   
	   break;
	   }
	   	case 8: {
	   if (__is_neg())
	       break;
	   var.second = y;
	   
	   break;
	   }
	   	case 9: {
	   if (__is_neg())
	       return;
	   var.second = y;
	   
	   break;
	   }
	   	case 10: {
	   if (error >= 0.5) {
	       y += ystep;
	       error = error - 1;
	   }
	   var.second = y;
	   
	   break;
	   }
	   	case 11: {
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
	   	case 12: {
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
	   	case 13: {
	   if (this->capacity() == this->size())
	       return false;
	   var.second = y;
	   
	   break;
	   }
	   	case 14: {
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
	   	case 15: {
	   if (y0 < y1)
	       ystep = 1;
	   else
	       ystep = -1;
	   var.second = y;
	   
	   break;
	   }
	   	case 16: {
	   line.push_back(var);
	   var.second = y;
	   
	   break;
	   }
	   	case 17: {
	   this->clear();
	   var.second = y;
	   
	   break;
	   }
	   	case 18: {
	   throw;
	   var.second = y;
	   
	   break;
	   }
	   	case 19: {
	   var.first = x;
	   var.second = y;
	   
	   break;
	   }
	   	case 20: {
	   var.first = y;
	   var.second = y;
	   
	   break;
	   }
	   	case 21: {
	   var.second = x;
	   var.second = y;
	   
	   break;
	   }
	   	case 22: {
	   var.second = y;
	   var.second = y;
	   
	   break;
	   }
	   	case 23: {
	   x0 = x1;
	   var.second = y;
	   
	   break;
	   }
	   	case 24: {
	   x0 = y0;
	   var.second = y;
	   
	   break;
	   }
	   	case 25: {
	   x1 = a;
	   var.second = y;
	   
	   break;
	   }
	   	case 26: {
	   x1 = y1;
	   var.second = y;
	   
	   break;
	   }
	   	case 27: {
	   y0 = a;
	   var.second = y;
	   
	   break;
	   }
	   	case 28: {
	   y0 = y1;
	   var.second = y;
	   
	   break;
	   }
	   	case 29: {
	   y1 = a;
	   var.second = y;
	   
	   break;
	   }
	   	case 30: {
	   ystep = -1;
	   var.second = y;
	   
	   break;
	   }
	   	case 31: {
	   ystep = 1;
	   var.second = y;
	   
	   break;
	   }
	   }
	   //prophet generated patch
	   switch(__choose("__ID5")){
	   	case 0: {
	   line.push_back(var)
	   break;
	   }
	   	case 1: {
	   
	   break;
	   }
	   	case 2: {
	   a = x0;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 3: {
	   a = x1;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 4: {
	   a = y0;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 5: {
	   error = error + deltaerr;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 6: {
	   error = error - 1;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 7: {
	   if (!__is_neg())
	       line.push_back(var);
	   
	   break;
	   }
	   	case 8: {
	   if (__is_neg())
	       break;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 9: {
	   if (__is_neg())
	       return;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 10: {
	   if (error >= 0.5) {
	       y += ystep;
	       error = error - 1;
	   }
	   line.push_back(var);
	   
	   break;
	   }
	   	case 11: {
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
	   	case 12: {
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
	   	case 13: {
	   if (this->capacity() == this->size())
	       return false;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 14: {
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
	   	case 15: {
	   if (y0 < y1)
	       ystep = 1;
	   else
	       ystep = -1;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 16: {
	   line.push_back(var);
	   line.push_back(var);
	   
	   break;
	   }
	   	case 17: {
	   this->clear();
	   line.push_back(var);
	   
	   break;
	   }
	   	case 18: {
	   throw;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 19: {
	   var.first = x;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 20: {
	   var.first = y;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 21: {
	   var.second = x;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 22: {
	   var.second = y;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 23: {
	   x0 = x1;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 24: {
	   x0 = y0;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 25: {
	   x1 = a;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 26: {
	   x1 = y1;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 27: {
	   y0 = a;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 28: {
	   y0 = y1;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 29: {
	   y1 = a;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 30: {
	   ystep = -1;
	   line.push_back(var);
	   
	   break;
	   }
	   	case 31: {
	   ystep = 1;
	   line.push_back(var);
	   
	   break;
	   }
	   }
}
	 //prophet generated patch
	 switch(__choose("__ID16")){
	 	case 0: {
	 error = error + deltaerr
	 break;
	 }
	 	case 1: {
	 
	 break;
	 }
	 	case 2: {
	 a = x0;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 3: {
	 a = x1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 4: {
	 a = y0;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 5: {
	 error = error + deltaerr;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 6: {
	 error = error - 1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 7: {
	 if (!__is_neg())
	     error = error + deltaerr;
	 
	 break;
	 }
	 	case 8: {
	 if (__is_neg())
	     break;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 9: {
	 if (__is_neg())
	     return;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 10: {
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 11: {
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
	 	case 12: {
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
	 	case 13: {
	 if (this->capacity() == this->size())
	     return false;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 14: {
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
	 	case 15: {
	 if (y0 < y1)
	     ystep = 1;
	 else
	     ystep = -1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 16: {
	 line.push_back(var);
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 17: {
	 this->clear();
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 18: {
	 throw;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 19: {
	 var.first = x;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 20: {
	 var.first = y;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 21: {
	 var.second = x;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 22: {
	 var.second = y;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 23: {
	 x0 = x1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 24: {
	 x0 = y0;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 25: {
	 x1 = a;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 26: {
	 x1 = y1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 27: {
	 y0 = a;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 28: {
	 y0 = y1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 29: {
	 y1 = a;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 30: {
	 ystep = -1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 	case 31: {
	 ystep = 1;
	 error = error + deltaerr;
	 
	 break;
	 }
	 }
	 //prophet generated patch
	 switch(__choose("__ID17")){
	 	case 0: {
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 1: {
	 
	 break;
	 }
	 	case 2: {
	 a = x0;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 3: {
	 a = x1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 4: {
	 a = y0;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 5: {
	 error = error + deltaerr;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 6: {
	 error = error - 1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 7: {
	 if (!__is_neg())
	     if (error >= 0.5) {
	         y += ystep;
	         error = error - 1;
	     }
	 
	 break;
	 }
	 	case 8: {
	 if ((error >= 0.5) && !__is_neg()) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 9: {
	 if ((error >= 0.5) || __is_neg()) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 10: {
	 if (__is_neg())
	     break;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 11: {
	 if (__is_neg())
	     return;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 12: {
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
	 	case 13: {
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
	 	case 14: {
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
	 	case 15: {
	 if (this->capacity() == this->size())
	     return false;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 16: {
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
	 	case 17: {
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
	 	case 18: {
	 line.push_back(var);
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 19: {
	 this->clear();
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 20: {
	 throw;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 21: {
	 var.first = x;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 22: {
	 var.first = y;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 23: {
	 var.second = x;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 24: {
	 var.second = y;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 25: {
	 x0 = x1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 26: {
	 x0 = y0;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 27: {
	 x1 = a;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 28: {
	 x1 = y1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 29: {
	 y0 = a;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 30: {
	 y0 = y1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 31: {
	 y1 = a;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 32: {
	 ystep = -1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 	case 33: {
	 ystep = 1;
	 if (error >= 0.5) {
	     y += ystep;
	     error = error - 1;
	 }
	 
	 break;
	 }
	 }
       }
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
  
  break;
  }
  	case 2: {
  if (__is_neg())
      return 0;
  FILE *f = fopen(argv[1], "r");
  
  break;
  }
  	case 3: {
  if (__is_neg())
      return 1;
  FILE *f = fopen(argv[1], "r");
  
  break;
  }
  	case 4: {
  if (this->capacity() == this->size())
      return false;
  FILE *f = fopen(argv[1], "r");
  
  break;
  }
  	case 5: {
  this->clear();
  FILE *f = fopen(argv[1], "r");
  
  break;
  }
  	case 6: {
  throw;
  FILE *f = fopen(argv[1], "r");
  
  break;
  }
  }
  //prophet generated patch
  switch(__choose("__ID71")){
  	case 0: {
  int x, y;
  
  break;
  }
  	case 1: {
  
  break;
  }
  	case 2: {
  if (__is_neg())
      return 0;
  int x, y;
  
  break;
  }
  	case 3: {
  if (__is_neg())
      return 1;
  int x, y;
  
  break;
  }
  	case 4: {
  if (this->capacity() == this->size())
      return false;
  int x, y;
  
  break;
  }
  	case 5: {
  this->clear();
  int x, y;
  
  break;
  }
  	case 6: {
  throw;
  int x, y;
  
  break;
  }
  }
  //prophet generated patch
  switch(__choose("__ID72")){
  	case 0: {
  fscanf(f, "%d %d\n", &x, &y)
  break;
  }
  	case 1: {
  
  break;
  }
  	case 2: {
  fscanf((f), "%d %d\n", &x, &y);
  
  break;
  }
  	case 3: {
  fscanf(f, __is_neg(), &x, &y);
  
  break;
  }
  	case 4: {
  if (!__is_neg())
      fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  	case 5: {
  if (__is_neg())
      return 0;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  	case 6: {
  if (__is_neg())
      return 1;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  	case 7: {
  if (this->capacity() == this->size())
      return false;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  	case 8: {
  this->clear();
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  	case 9: {
  throw;
  fscanf(f, "%d %d\n", &x, &y);
  
  break;
  }
  }
  //prophet generated patch
  switch(__choose("__ID73")){
  	case 0: {
  fclose(f)
  break;
  }
  	case 1: {
  
  break;
  }
  	case 2: {
  fclose((f));
  
  break;
  }
  	case 3: {
  if (!__is_neg())
      fclose(f);
  
  break;
  }
  	case 4: {
  if (__is_neg())
      return 0;
  fclose(f);
  
  break;
  }
  	case 5: {
  if (__is_neg())
      return 1;
  fclose(f);
  
  break;
  }
  	case 6: {
  if (this->capacity() == this->size())
      return false;
  fclose(f);
  
  break;
  }
  	case 7: {
  this->clear();
  fclose(f);
  
  break;
  }
  	case 8: {
  throw;
  fclose(f);
  
  break;
  }
  }
  //prophet generated patch
  switch(__choose("__ID74")){
  	case 0: {
  DiscreteLine line;
  
  break;
  }
  	case 1: {
  
  break;
  }
  	case 2: {
  if (__is_neg())
      return 0;
  DiscreteLine line;
  
  break;
  }
  	case 3: {
  if (__is_neg())
      return 1;
  DiscreteLine line;
  
  break;
  }
  	case 4: {
  if (this->capacity() == this->size())
      return false;
  DiscreteLine line;
  
  break;
  }
  	case 5: {
  this->clear();
  DiscreteLine line;
  
  break;
  }
  	case 6: {
  throw;
  DiscreteLine line;
  
  break;
  }
  }
  //prophet generated patch
  switch(__choose("__ID75")){
  	case 0: {
  buggy(x, y, line)
  break;
  }
  	case 1: {
  
  break;
  }
  	case 2: {
  buggy(x, y, line);
  buggy(x, y, line);
  
  break;
  }
  	case 3: {
  if (!__is_neg())
      buggy(x, y, line);
  
  break;
  }
  	case 4: {
  if (__is_neg())
      return 0;
  buggy(x, y, line);
  
  break;
  }
  	case 5: {
  if (__is_neg())
      return 1;
  buggy(x, y, line);
  
  break;
  }
  	case 6: {
  if (this->capacity() == this->size())
      return false;
  buggy(x, y, line);
  
  break;
  }
  	case 7: {
  this->clear();
  buggy(x, y, line);
  
  break;
  }
  	case 8: {
  throw;
  buggy(x, y, line);
  
  break;
  }
  }

  for (unsigned i = 0, end = line.size(); i != line.size(); i++)
    printf("%d %d\n", line[i].first, line[i].second);
}
