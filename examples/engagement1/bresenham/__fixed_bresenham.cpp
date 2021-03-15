extern "C" {
int __get_mutant();
int __is_neg(char *, int, char **, int, int *, int, char *, int, void **, int,
             double *);
int __abst_hole();
int __choose(char *);
}
/* original courtesy of player1537,
 * http://www.cemetech.net/forum/viewtopic.php?t=5401 */

#include <stdio.h>
#include <stdlib.h>
#include <utility>

#include "Bresenham.h"

void buggy(unsigned x1, unsigned y1, DiscreteLine &line) {
  short x0 = 0, y0 = 0;

  bool steep = abs((int)(y1 - y0)) > abs((int)(x1 - x0));
  short a;
  // prophet generated patch
  switch (__choose("__ID0")) {
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
#ifdef COMPILE_0
  case 1: {
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
#ifdef COMPILE_1
  case 2: {
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
#ifdef COMPILE_2
  case 3: {
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
#ifdef COMPILE_3
  case 4: {
    if (!__is_neg("0-4-0", 6, (char **){"steep", "x1", "y1", "x0", "y0", "a"},
                  6, (int *){steep, x1, y1, x0, y0, a}, 0, 0, 0, 0, 0, 0))
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
#ifdef COMPILE_4
  case 5: {
    if ((steep) &&
        !__is_neg("0-5-0", 6, (char **){"steep", "x1", "y1", "x0", "y0", "a"},
                  6, (int *){steep, x1, y1, x0, y0, a}, 0, 0, 0, 0, 0, 0)) {
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
#ifdef COMPILE_5
  case 6: {
    if ((steep) ||
        __is_neg("0-6-0", 6, (char **){"steep", "x1", "y1", "x0", "y0", "a"}, 6,
                 (int *){steep, x1, y1, x0, y0, a}, 0, 0, 0, 0, 0, 0)) {
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
#ifdef COMPILE_6
  case 7: {
    if (__is_neg("0-7-0", 6, (char **){"steep", "x1", "y1", "x0", "y0", "a"}, 6,
                 (int *){steep, x1, y1, x0, y0, a}, 0, 0, 0, 0, 0, 0))
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
#ifdef COMPILE_7
  case 8: {
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
#ifdef COMPILE_8
  case 9: {
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
#ifdef COMPILE_9
  case 10: {
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
#ifdef COMPILE_10
  case 11: {
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
#ifdef COMPILE_11
  case 12: {
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
#ifdef COMPILE_12
  case 13: {
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
#ifdef COMPILE_13
  case 14: {
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
#ifdef COMPILE_14
  case 15: {
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
#ifdef COMPILE_15
  case 16: {
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
#ifdef COMPILE_16
  case 17: {
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
  // prophet generated patch
  switch (__choose("__ID1")) {
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
#ifdef COMPILE_17
  case 1: {
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
#ifdef COMPILE_18
  case 2: {
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
#ifdef COMPILE_19
  case 3: {
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
#ifdef COMPILE_20
  case 4: {
    if (!__is_neg("1-4-0", 6, (char **){"x1", "x0", "y1", "a", "y0", "steep"},
                  6, (int *){x1, x0, y1, a, y0, steep}, 0, 0, 0, 0, 0, 0))
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
#ifdef COMPILE_21
  case 5: {
    if ((x0 > x1) &&
        !__is_neg("1-5-0", 6, (char **){"x1", "x0", "y1", "a", "y0", "steep"},
                  6, (int *){x1, x0, y1, a, y0, steep}, 0, 0, 0, 0, 0, 0)) {
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
#ifdef COMPILE_22
  case 6: {
    if ((x0 > x1) ||
        __is_neg("1-6-0", 6, (char **){"x1", "x0", "y1", "a", "y0", "steep"}, 6,
                 (int *){x1, x0, y1, a, y0, steep}, 0, 0, 0, 0, 0, 0)) {
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
#ifdef COMPILE_23
  case 7: {
    if (__is_neg("1-7-0", 6, (char **){"x1", "x0", "y1", "a", "y0", "steep"}, 6,
                 (int *){x1, x0, y1, a, y0, steep}, 0, 0, 0, 0, 0, 0))
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
#ifdef COMPILE_24
  case 8: {
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
#ifdef COMPILE_25
  case 9: {
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
#ifdef COMPILE_26
  case 10: {
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
#ifdef COMPILE_27
  case 11: {
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
#ifdef COMPILE_28
  case 12: {
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
#ifdef COMPILE_29
  case 13: {
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
#ifdef COMPILE_30
  case 14: {
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
#ifdef COMPILE_31
  case 15: {
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
#ifdef COMPILE_32
  case 16: {
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
#ifdef COMPILE_33
  case 17: {
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
  // prophet generated patch
  switch (__choose("__ID2")) {
  case 0: {
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#ifdef COMPILE_34
  case 1: {
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_35
  case 2: {
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_36
  case 3: {
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_37
  case 4: {
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_38
  case 5: {
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_39
  case 6: {
    if (y0 < y1)
      if (!__is_neg("2-6-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_40
  case 7: {
    if (y0 < y1)
      if (__is_neg("2-7-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_41
  case 8: {
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_42
  case 9: {
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_43
  case 10: {
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_44
  case 11: {
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_45
  case 12: {
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_46
  case 13: {
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_47
  case 14: {
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_48
  case 15: {
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_49
  case 16: {
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_50
  case 17: {
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_51
  case 18: {
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_52
  case 19: {
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_53
  case 20: {
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_54
  case 21: {
    a = x0;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_55
  case 22: {
    a = x0;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_56
  case 23: {
    a = x0;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_57
  case 24: {
    a = x0;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_58
  case 25: {
    a = x0;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_59
  case 26: {
    a = x0;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_60
  case 27: {
    a = x0;
    if (y0 < y1)
      if (!__is_neg("2-27-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_61
  case 28: {
    a = x0;
    if (y0 < y1)
      if (__is_neg("2-28-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_62
  case 29: {
    a = x0;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_63
  case 30: {
    a = x0;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_64
  case 31: {
    a = x0;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_65
  case 32: {
    a = x0;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_66
  case 33: {
    a = x0;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_67
  case 34: {
    a = x0;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_68
  case 35: {
    a = x0;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_69
  case 36: {
    a = x0;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_70
  case 37: {
    a = x0;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_71
  case 38: {
    a = x0;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_72
  case 39: {
    a = x0;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_73
  case 40: {
    a = x0;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_74
  case 41: {
    a = x0;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_75
  case 42: {
    a = x1;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_76
  case 43: {
    a = x1;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_77
  case 44: {
    a = x1;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_78
  case 45: {
    a = x1;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_79
  case 46: {
    a = x1;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_80
  case 47: {
    a = x1;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_81
  case 48: {
    a = x1;
    if (y0 < y1)
      if (!__is_neg("2-48-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_82
  case 49: {
    a = x1;
    if (y0 < y1)
      if (__is_neg("2-49-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_83
  case 50: {
    a = x1;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_84
  case 51: {
    a = x1;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_85
  case 52: {
    a = x1;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_86
  case 53: {
    a = x1;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_87
  case 54: {
    a = x1;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_88
  case 55: {
    a = x1;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_89
  case 56: {
    a = x1;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_90
  case 57: {
    a = x1;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_91
  case 58: {
    a = x1;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_92
  case 59: {
    a = x1;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_93
  case 60: {
    a = x1;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_94
  case 61: {
    a = x1;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_95
  case 62: {
    a = x1;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_96
  case 63: {
    a = y0;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_97
  case 64: {
    a = y0;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_98
  case 65: {
    a = y0;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_99
  case 66: {
    a = y0;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_100
  case 67: {
    a = y0;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_101
  case 68: {
    a = y0;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_102
  case 69: {
    a = y0;
    if (y0 < y1)
      if (!__is_neg("2-69-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_103
  case 70: {
    a = y0;
    if (y0 < y1)
      if (__is_neg("2-70-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_104
  case 71: {
    a = y0;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_105
  case 72: {
    a = y0;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_106
  case 73: {
    a = y0;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_107
  case 74: {
    a = y0;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_108
  case 75: {
    a = y0;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_109
  case 76: {
    a = y0;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_110
  case 77: {
    a = y0;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_111
  case 78: {
    a = y0;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_112
  case 79: {
    a = y0;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_113
  case 80: {
    a = y0;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_114
  case 81: {
    a = y0;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_115
  case 82: {
    a = y0;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_116
  case 83: {
    a = y0;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_117
  case 84: {
    error = error + deltaerr;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_118
  case 85: {
    error = error + deltaerr;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_119
  case 86: {
    error = error + deltaerr;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_120
  case 87: {
    error = error + deltaerr;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_121
  case 88: {
    error = error + deltaerr;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_122
  case 89: {
    error = error + deltaerr;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_123
  case 90: {
    error = error + deltaerr;
    if (y0 < y1)
      if (!__is_neg("2-90-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_124
  case 91: {
    error = error + deltaerr;
    if (y0 < y1)
      if (__is_neg("2-91-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_125
  case 92: {
    error = error + deltaerr;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_126
  case 93: {
    error = error + deltaerr;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_127
  case 94: {
    error = error + deltaerr;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_128
  case 95: {
    error = error + deltaerr;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_129
  case 96: {
    error = error + deltaerr;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_130
  case 97: {
    error = error + deltaerr;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_131
  case 98: {
    error = error + deltaerr;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_132
  case 99: {
    error = error + deltaerr;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_133
  case 100: {
    error = error + deltaerr;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_134
  case 101: {
    error = error + deltaerr;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_135
  case 102: {
    error = error + deltaerr;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_136
  case 103: {
    error = error + deltaerr;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_137
  case 104: {
    error = error + deltaerr;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_138
  case 105: {
    error = error - 1;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_139
  case 106: {
    error = error - 1;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_140
  case 107: {
    error = error - 1;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_141
  case 108: {
    error = error - 1;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_142
  case 109: {
    error = error - 1;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_143
  case 110: {
    error = error - 1;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_144
  case 111: {
    error = error - 1;
    if (y0 < y1)
      if (!__is_neg("2-111-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_145
  case 112: {
    error = error - 1;
    if (y0 < y1)
      if (__is_neg("2-112-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_146
  case 113: {
    error = error - 1;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_147
  case 114: {
    error = error - 1;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_148
  case 115: {
    error = error - 1;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_149
  case 116: {
    error = error - 1;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_150
  case 117: {
    error = error - 1;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_151
  case 118: {
    error = error - 1;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_152
  case 119: {
    error = error - 1;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_153
  case 120: {
    error = error - 1;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_154
  case 121: {
    error = error - 1;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_155
  case 122: {
    error = error - 1;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_156
  case 123: {
    error = error - 1;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_157
  case 124: {
    error = error - 1;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_158
  case 125: {
    error = error - 1;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_159
  case 126: {
    if (!__is_neg("2-126-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_160
  case 127: {
    if (!__is_neg("2-127-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_161
  case 128: {
    if (!__is_neg("2-128-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_162
  case 129: {
    if (!__is_neg("2-129-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_163
  case 130: {
    if (!__is_neg("2-130-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_164
  case 131: {
    if (!__is_neg("2-131-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_165
  case 132: {
    if (!__is_neg("2-132-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        if (steep) {
          a = x0;
          x0 = y0;
          y0 = a;
          a = x1;
          x1 = y1;
          y1 = a;
        }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_166
  case 133: {
    if (!__is_neg("2-133-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        if (x0 > x1) {
          a = x0;
          x0 = x1;
          x1 = a;
          a = y0;
          y0 = y1;
          y1 = a;
        }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_167
  case 134: {
    if (!__is_neg("2-134-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_168
  case 135: {
    if (!__is_neg("2-135-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_169
  case 136: {
    if (!__is_neg("2-136-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_170
  case 137: {
    if (!__is_neg("2-137-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_171
  case 138: {
    if (!__is_neg("2-138-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_172
  case 139: {
    if (!__is_neg("2-139-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_173
  case 140: {
    if (!__is_neg("2-140-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_174
  case 141: {
    if (!__is_neg("2-141-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_175
  case 142: {
    if (!__is_neg("2-142-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_176
  case 143: {
    if (!__is_neg("2-143-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_177
  case 144: {
    if (!__is_neg("2-144-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_178
  case 145: {
    if ((y0 < y1) &&
        !__is_neg("2-145-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_179
  case 146: {
    if ((y0 < y1) &&
        !__is_neg("2-146-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_180
  case 147: {
    if ((y0 < y1) &&
        !__is_neg("2-147-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_181
  case 148: {
    if ((y0 < y1) &&
        !__is_neg("2-148-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_182
  case 149: {
    if ((y0 < y1) &&
        !__is_neg("2-149-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_183
  case 150: {
    if ((y0 < y1) &&
        !__is_neg("2-150-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_184
  case 151: {
    if ((y0 < y1) &&
        !__is_neg("2-151-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_185
  case 152: {
    if ((y0 < y1) &&
        !__is_neg("2-152-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_186
  case 153: {
    if ((y0 < y1) &&
        !__is_neg("2-153-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_187
  case 154: {
    if ((y0 < y1) &&
        !__is_neg("2-154-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_188
  case 155: {
    if ((y0 < y1) &&
        !__is_neg("2-155-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_189
  case 156: {
    if ((y0 < y1) &&
        !__is_neg("2-156-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_190
  case 157: {
    if ((y0 < y1) &&
        !__is_neg("2-157-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_191
  case 158: {
    if ((y0 < y1) &&
        !__is_neg("2-158-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_192
  case 159: {
    if ((y0 < y1) &&
        !__is_neg("2-159-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_193
  case 160: {
    if ((y0 < y1) &&
        !__is_neg("2-160-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_194
  case 161: {
    if ((y0 < y1) &&
        !__is_neg("2-161-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_195
  case 162: {
    if ((y0 < y1) &&
        !__is_neg("2-162-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_196
  case 163: {
    if ((y0 < y1) &&
        !__is_neg("2-163-0", 9,
                  (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                            "a", "steep"},
                  9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep},
                  0, 0, 0, 0, 0, 0))
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_197
  case 164: {
    if ((y0 < y1) ||
        __is_neg("2-164-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_198
  case 165: {
    if ((y0 < y1) ||
        __is_neg("2-165-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_199
  case 166: {
    if ((y0 < y1) ||
        __is_neg("2-166-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_200
  case 167: {
    if ((y0 < y1) ||
        __is_neg("2-167-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_201
  case 168: {
    if ((y0 < y1) ||
        __is_neg("2-168-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_202
  case 169: {
    if ((y0 < y1) ||
        __is_neg("2-169-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_203
  case 170: {
    if ((y0 < y1) ||
        __is_neg("2-170-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_204
  case 171: {
    if ((y0 < y1) ||
        __is_neg("2-171-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_205
  case 172: {
    if ((y0 < y1) ||
        __is_neg("2-172-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_206
  case 173: {
    if ((y0 < y1) ||
        __is_neg("2-173-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_207
  case 174: {
    if ((y0 < y1) ||
        __is_neg("2-174-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_208
  case 175: {
    if ((y0 < y1) ||
        __is_neg("2-175-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_209
  case 176: {
    if ((y0 < y1) ||
        __is_neg("2-176-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_210
  case 177: {
    if ((y0 < y1) ||
        __is_neg("2-177-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_211
  case 178: {
    if ((y0 < y1) ||
        __is_neg("2-178-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_212
  case 179: {
    if ((y0 < y1) ||
        __is_neg("2-179-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_213
  case 180: {
    if ((y0 < y1) ||
        __is_neg("2-180-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_214
  case 181: {
    if ((y0 < y1) ||
        __is_neg("2-181-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_215
  case 182: {
    if ((y0 < y1) ||
        __is_neg("2-182-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_216
  case 183: {
    if (__is_neg("2-183-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_217
  case 184: {
    if (__is_neg("2-184-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_218
  case 185: {
    if (__is_neg("2-185-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_219
  case 186: {
    if (__is_neg("2-186-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_220
  case 187: {
    if (__is_neg("2-187-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_221
  case 188: {
    if (__is_neg("2-188-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_222
  case 189: {
    if (__is_neg("2-189-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_223
  case 190: {
    if (__is_neg("2-190-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_224
  case 191: {
    if (__is_neg("2-191-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_225
  case 192: {
    if (__is_neg("2-192-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_226
  case 193: {
    if (__is_neg("2-193-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_227
  case 194: {
    if (__is_neg("2-194-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_228
  case 195: {
    if (__is_neg("2-195-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_229
  case 196: {
    if (__is_neg("2-196-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_230
  case 197: {
    if (__is_neg("2-197-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_231
  case 198: {
    if (__is_neg("2-198-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_232
  case 199: {
    if (__is_neg("2-199-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_233
  case 200: {
    if (__is_neg("2-200-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_234
  case 201: {
    if (__is_neg("2-201-0", 9,
                 (char **){"y1", "y0", "ystep", "deltax", "deltay", "x1", "x0",
                           "a", "steep"},
                 9, (int *){y1, y0, ystep, deltax, deltay, x1, x0, a, steep}, 0,
                 0, 0, 0, 0, 0))
      return;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_235
  case 202: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_236
  case 203: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_237
  case 204: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_238
  case 205: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_239
  case 206: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_240
  case 207: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_241
  case 208: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      if (!__is_neg("2-208-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_242
  case 209: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      if (__is_neg("2-209-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_243
  case 210: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_244
  case 211: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_245
  case 212: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_246
  case 213: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_247
  case 214: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_248
  case 215: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_249
  case 216: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_250
  case 217: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_251
  case 218: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_252
  case 219: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_253
  case 220: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_254
  case 221: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_255
  case 222: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_256
  case 223: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_257
  case 224: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_258
  case 225: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_259
  case 226: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_260
  case 227: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_261
  case 228: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_262
  case 229: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      if (!__is_neg("2-229-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_263
  case 230: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      if (__is_neg("2-230-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_264
  case 231: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_265
  case 232: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_266
  case 233: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_267
  case 234: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_268
  case 235: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_269
  case 236: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_270
  case 237: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_271
  case 238: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_272
  case 239: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_273
  case 240: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_274
  case 241: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_275
  case 242: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_276
  case 243: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_277
  case 244: {
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_278
  case 245: {
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_279
  case 246: {
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_280
  case 247: {
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_281
  case 248: {
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_282
  case 249: {
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_283
  case 250: {
    if (y0 < y1)
      if (!__is_neg("2-250-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_284
  case 251: {
    if (y0 < y1)
      if (__is_neg("2-251-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_285
  case 252: {
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_286
  case 253: {
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_287
  case 254: {
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_288
  case 255: {
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_289
  case 256: {
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_290
  case 257: {
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_291
  case 258: {
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_292
  case 259: {
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_293
  case 260: {
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_294
  case 261: {
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_295
  case 262: {
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_296
  case 263: {
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_297
  case 264: {
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_298
  case 265: {
    throw;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_299
  case 266: {
    throw;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_300
  case 267: {
    throw;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_301
  case 268: {
    throw;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_302
  case 269: {
    throw;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_303
  case 270: {
    throw;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_304
  case 271: {
    throw;
    if (y0 < y1)
      if (!__is_neg("2-271-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_305
  case 272: {
    throw;
    if (y0 < y1)
      if (__is_neg("2-272-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_306
  case 273: {
    throw;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_307
  case 274: {
    throw;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_308
  case 275: {
    throw;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_309
  case 276: {
    throw;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_310
  case 277: {
    throw;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_311
  case 278: {
    throw;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_312
  case 279: {
    throw;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_313
  case 280: {
    throw;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_314
  case 281: {
    throw;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_315
  case 282: {
    throw;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_316
  case 283: {
    throw;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_317
  case 284: {
    throw;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_318
  case 285: {
    throw;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_319
  case 286: {
    x0 = x1;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_320
  case 287: {
    x0 = x1;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_321
  case 288: {
    x0 = x1;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_322
  case 289: {
    x0 = x1;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_323
  case 290: {
    x0 = x1;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_324
  case 291: {
    x0 = x1;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_325
  case 292: {
    x0 = x1;
    if (y0 < y1)
      if (!__is_neg("2-292-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_326
  case 293: {
    x0 = x1;
    if (y0 < y1)
      if (__is_neg("2-293-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_327
  case 294: {
    x0 = x1;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_328
  case 295: {
    x0 = x1;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_329
  case 296: {
    x0 = x1;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_330
  case 297: {
    x0 = x1;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_331
  case 298: {
    x0 = x1;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_332
  case 299: {
    x0 = x1;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_333
  case 300: {
    x0 = x1;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_334
  case 301: {
    x0 = x1;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_335
  case 302: {
    x0 = x1;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_336
  case 303: {
    x0 = x1;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_337
  case 304: {
    x0 = x1;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_338
  case 305: {
    x0 = x1;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_339
  case 306: {
    x0 = x1;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_340
  case 307: {
    x0 = y0;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_341
  case 308: {
    x0 = y0;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_342
  case 309: {
    x0 = y0;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_343
  case 310: {
    x0 = y0;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_344
  case 311: {
    x0 = y0;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_345
  case 312: {
    x0 = y0;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_346
  case 313: {
    x0 = y0;
    if (y0 < y1)
      if (!__is_neg("2-313-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_347
  case 314: {
    x0 = y0;
    if (y0 < y1)
      if (__is_neg("2-314-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_348
  case 315: {
    x0 = y0;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_349
  case 316: {
    x0 = y0;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_350
  case 317: {
    x0 = y0;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_351
  case 318: {
    x0 = y0;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_352
  case 319: {
    x0 = y0;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_353
  case 320: {
    x0 = y0;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_354
  case 321: {
    x0 = y0;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_355
  case 322: {
    x0 = y0;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_356
  case 323: {
    x0 = y0;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_357
  case 324: {
    x0 = y0;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_358
  case 325: {
    x0 = y0;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_359
  case 326: {
    x0 = y0;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_360
  case 327: {
    x0 = y0;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_361
  case 328: {
    x1 = a;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_362
  case 329: {
    x1 = a;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_363
  case 330: {
    x1 = a;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_364
  case 331: {
    x1 = a;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_365
  case 332: {
    x1 = a;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_366
  case 333: {
    x1 = a;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_367
  case 334: {
    x1 = a;
    if (y0 < y1)
      if (!__is_neg("2-334-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_368
  case 335: {
    x1 = a;
    if (y0 < y1)
      if (__is_neg("2-335-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_369
  case 336: {
    x1 = a;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_370
  case 337: {
    x1 = a;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_371
  case 338: {
    x1 = a;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_372
  case 339: {
    x1 = a;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_373
  case 340: {
    x1 = a;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_374
  case 341: {
    x1 = a;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_375
  case 342: {
    x1 = a;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_376
  case 343: {
    x1 = a;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_377
  case 344: {
    x1 = a;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_378
  case 345: {
    x1 = a;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_379
  case 346: {
    x1 = a;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_380
  case 347: {
    x1 = a;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_381
  case 348: {
    x1 = a;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_382
  case 349: {
    x1 = y1;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_383
  case 350: {
    x1 = y1;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_384
  case 351: {
    x1 = y1;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_385
  case 352: {
    x1 = y1;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_386
  case 353: {
    x1 = y1;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_387
  case 354: {
    x1 = y1;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_388
  case 355: {
    x1 = y1;
    if (y0 < y1)
      if (!__is_neg("2-355-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_389
  case 356: {
    x1 = y1;
    if (y0 < y1)
      if (__is_neg("2-356-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_390
  case 357: {
    x1 = y1;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_391
  case 358: {
    x1 = y1;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_392
  case 359: {
    x1 = y1;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_393
  case 360: {
    x1 = y1;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_394
  case 361: {
    x1 = y1;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_395
  case 362: {
    x1 = y1;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_396
  case 363: {
    x1 = y1;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_397
  case 364: {
    x1 = y1;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_398
  case 365: {
    x1 = y1;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_399
  case 366: {
    x1 = y1;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_400
  case 367: {
    x1 = y1;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_401
  case 368: {
    x1 = y1;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_402
  case 369: {
    x1 = y1;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_403
  case 370: {
    y0 = a;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_404
  case 371: {
    y0 = a;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_405
  case 372: {
    y0 = a;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_406
  case 373: {
    y0 = a;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_407
  case 374: {
    y0 = a;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_408
  case 375: {
    y0 = a;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_409
  case 376: {
    y0 = a;
    if (y0 < y1)
      if (!__is_neg("2-376-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_410
  case 377: {
    y0 = a;
    if (y0 < y1)
      if (__is_neg("2-377-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_411
  case 378: {
    y0 = a;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_412
  case 379: {
    y0 = a;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_413
  case 380: {
    y0 = a;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_414
  case 381: {
    y0 = a;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_415
  case 382: {
    y0 = a;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_416
  case 383: {
    y0 = a;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_417
  case 384: {
    y0 = a;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_418
  case 385: {
    y0 = a;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_419
  case 386: {
    y0 = a;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_420
  case 387: {
    y0 = a;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_421
  case 388: {
    y0 = a;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_422
  case 389: {
    y0 = a;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_423
  case 390: {
    y0 = a;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_424
  case 391: {
    y0 = y1;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_425
  case 392: {
    y0 = y1;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_426
  case 393: {
    y0 = y1;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_427
  case 394: {
    y0 = y1;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_428
  case 395: {
    y0 = y1;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_429
  case 396: {
    y0 = y1;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_430
  case 397: {
    y0 = y1;
    if (y0 < y1)
      if (!__is_neg("2-397-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_431
  case 398: {
    y0 = y1;
    if (y0 < y1)
      if (__is_neg("2-398-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_432
  case 399: {
    y0 = y1;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_433
  case 400: {
    y0 = y1;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_434
  case 401: {
    y0 = y1;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_435
  case 402: {
    y0 = y1;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_436
  case 403: {
    y0 = y1;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_437
  case 404: {
    y0 = y1;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_438
  case 405: {
    y0 = y1;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_439
  case 406: {
    y0 = y1;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_440
  case 407: {
    y0 = y1;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_441
  case 408: {
    y0 = y1;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_442
  case 409: {
    y0 = y1;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_443
  case 410: {
    y0 = y1;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_444
  case 411: {
    y0 = y1;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_445
  case 412: {
    y1 = a;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_446
  case 413: {
    y1 = a;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_447
  case 414: {
    y1 = a;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_448
  case 415: {
    y1 = a;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_449
  case 416: {
    y1 = a;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_450
  case 417: {
    y1 = a;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_451
  case 418: {
    y1 = a;
    if (y0 < y1)
      if (!__is_neg("2-418-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_452
  case 419: {
    y1 = a;
    if (y0 < y1)
      if (__is_neg("2-419-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_453
  case 420: {
    y1 = a;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_454
  case 421: {
    y1 = a;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_455
  case 422: {
    y1 = a;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_456
  case 423: {
    y1 = a;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_457
  case 424: {
    y1 = a;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_458
  case 425: {
    y1 = a;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_459
  case 426: {
    y1 = a;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_460
  case 427: {
    y1 = a;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_461
  case 428: {
    y1 = a;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_462
  case 429: {
    y1 = a;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_463
  case 430: {
    y1 = a;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_464
  case 431: {
    y1 = a;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_465
  case 432: {
    y1 = a;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_466
  case 433: {
    ystep = -1;
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_467
  case 434: {
    ystep = -1;
    if (y0 < y1)
      a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_468
  case 435: {
    ystep = -1;
    if (y0 < y1)
      a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_469
  case 436: {
    ystep = -1;
    if (y0 < y1)
      a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_470
  case 437: {
    ystep = -1;
    if (y0 < y1)
      error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_471
  case 438: {
    ystep = -1;
    if (y0 < y1)
      error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_472
  case 439: {
    ystep = -1;
    if (y0 < y1)
      if (!__is_neg("2-439-0", 9,
                    (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                              "x0", "steep", "a"},
                    9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                    0, 0, 0, 0, 0, 0))
        ystep = 1;
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_473
  case 440: {
    ystep = -1;
    if (y0 < y1)
      if (__is_neg("2-440-0", 9,
                   (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1",
                             "x0", "steep", "a"},
                   9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                   0, 0, 0, 0, 0, 0))
        return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_474
  case 441: {
    ystep = -1;
    if (y0 < y1)
      if (steep) {
        a = x0;
        x0 = y0;
        y0 = a;
        a = x1;
        x1 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_475
  case 442: {
    ystep = -1;
    if (y0 < y1)
      if (x0 > x1) {
        a = x0;
        x0 = x1;
        x1 = a;
        a = y0;
        y0 = y1;
        y1 = a;
      }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_476
  case 443: {
    ystep = -1;
    if (y0 < y1)
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_477
  case 444: {
    ystep = -1;
    if (y0 < y1)
      throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_478
  case 445: {
    ystep = -1;
    if (y0 < y1)
      x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_479
  case 446: {
    ystep = -1;
    if (y0 < y1)
      x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_480
  case 447: {
    ystep = -1;
    if (y0 < y1)
      x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_481
  case 448: {
    ystep = -1;
    if (y0 < y1)
      x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_482
  case 449: {
    ystep = -1;
    if (y0 < y1)
      y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_483
  case 450: {
    ystep = -1;
    if (y0 < y1)
      y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_484
  case 451: {
    ystep = -1;
    if (y0 < y1)
      y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_485
  case 452: {
    ystep = -1;
    if (y0 < y1)
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_486
  case 453: {
    ystep = -1;
    if (y0 < y1)
      ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_487
  case 454: {
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_488
  case 455: {
    a = x0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_489
  case 456: {
    a = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_490
  case 457: {
    a = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_491
  case 458: {
    error = error + deltaerr;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_492
  case 459: {
    error = error - 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_493
  case 460: {
    if (!__is_neg("2-460-0", 9,
                  (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1", "x0",
                            "steep", "a"},
                  9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a},
                  0, 0, 0, 0, 0, 0))
      ystep = 1;
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_494
  case 461: {
    if (__is_neg("2-461-0", 9,
                 (char **){"ystep", "y1", "y0", "deltax", "deltay", "x1", "x0",
                           "steep", "a"},
                 9, (int *){ystep, y1, y0, deltax, deltay, x1, x0, steep, a}, 0,
                 0, 0, 0, 0, 0))
      return;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_495
  case 462: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_496
  case 463: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_497
  case 464: {
    if (y0 < y1)
      ystep = 1;
    else
      ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_498
  case 465: {
    throw;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_499
  case 466: {
    x0 = x1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_500
  case 467: {
    x0 = y0;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_501
  case 468: {
    x1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_502
  case 469: {
    x1 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_503
  case 470: {
    y0 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_504
  case 471: {
    y0 = y1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_505
  case 472: {
    y1 = a;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_506
  case 473: {
    ystep = -1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_507
  case 474: {
    ystep = 1;
    ystep = 1;
    else ystep = -1;

    break;
  }
#endif
  }

  int y = y0;
  std::pair<int, int> var;
  for (int x = x0; x < x1; x++) {
    if (steep) {
      var.first = y;
      var.second = x;
      line.push_back(var);
    } else {
      // prophet generated patch
      switch (__choose("__ID3")) {
      case 0: {
        var.first = x;

        break;
      }
#ifdef COMPILE_508
      case 1: {
        a = x0;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_509
      case 2: {
        a = x1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_510
      case 3: {
        a = y0;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_511
      case 4: {
        error = error + deltaerr;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_512
      case 5: {
        error = error - 1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_513
      case 6: {
        if (!__is_neg(
                "3-6-0", 11,
                (char **){"x", "y", "steep", "ystep", "x1", "x0", "y0", "y1",
                          "deltax", "deltay", "a"},
                11,
                (int *){x, y, steep, ystep, x1, x0, y0, y1, deltax, deltay, a},
                0, 0, 0, 0, 0, 0))
          var.first = x;

        break;
      }
#endif
#ifdef COMPILE_514
      case 7: {
        if (__is_neg(
                "3-7-0", 11,
                (char **){"x", "y", "steep", "ystep", "x1", "x0", "y0", "y1",
                          "deltax", "deltay", "a"},
                11,
                (int *){x, y, steep, ystep, x1, x0, y0, y1, deltax, deltay, a},
                0, 0, 0, 0, 0, 0))
          break;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_515
      case 8: {
        if (__is_neg(
                "3-8-0", 11,
                (char **){"x", "y", "steep", "ystep", "x1", "x0", "y0", "y1",
                          "deltax", "deltay", "a"},
                11,
                (int *){x, y, steep, ystep, x1, x0, y0, y1, deltax, deltay, a},
                0, 0, 0, 0, 0, 0))
          return;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_516
      case 9: {
        if (error >= 0.5) {
          y += ystep;
          error = error - 1;
        }
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_517
      case 10: {
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
#ifdef COMPILE_518
      case 11: {
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
#ifdef COMPILE_519
      case 12: {
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
#ifdef COMPILE_520
      case 13: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_521
      case 14: {
        line.push_back(var);
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_522
      case 15: {
        throw;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_523
      case 16: {
        var.first = x;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_524
      case 17: {
        var.first = y;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_525
      case 18: {
        var.second = x;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_526
      case 19: {
        var.second = y;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_527
      case 20: {
        x0 = x1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_528
      case 21: {
        x0 = y0;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_529
      case 22: {
        x1 = a;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_530
      case 23: {
        x1 = y1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_531
      case 24: {
        y0 = a;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_532
      case 25: {
        y0 = y1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_533
      case 26: {
        y1 = a;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_534
      case 27: {
        ystep = -1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_535
      case 28: {
        ystep = 1;
        var.first = x;

        break;
      }
#endif
      }
      // prophet generated patch
      switch (__choose("__ID4")) {
      case 0: {
        var.second = y;

        break;
      }
#ifdef COMPILE_536
      case 1: {
        a = x0;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_537
      case 2: {
        a = x1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_538
      case 3: {
        a = y0;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_539
      case 4: {
        error = error + deltaerr;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_540
      case 5: {
        error = error - 1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_541
      case 6: {
        if (!__is_neg(
                "4-6-0", 11,
                (char **){"y", "x", "ystep", "steep", "x1", "x0", "y0", "y1",
                          "deltax", "deltay", "a"},
                11,
                (int *){y, x, ystep, steep, x1, x0, y0, y1, deltax, deltay, a},
                0, 0, 0, 0, 0, 0))
          var.second = y;

        break;
      }
#endif
#ifdef COMPILE_542
      case 7: {
        if (__is_neg(
                "4-7-0", 11,
                (char **){"y", "x", "ystep", "steep", "x1", "x0", "y0", "y1",
                          "deltax", "deltay", "a"},
                11,
                (int *){y, x, ystep, steep, x1, x0, y0, y1, deltax, deltay, a},
                0, 0, 0, 0, 0, 0))
          break;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_543
      case 8: {
        if (__is_neg(
                "4-8-0", 11,
                (char **){"y", "x", "ystep", "steep", "x1", "x0", "y0", "y1",
                          "deltax", "deltay", "a"},
                11,
                (int *){y, x, ystep, steep, x1, x0, y0, y1, deltax, deltay, a},
                0, 0, 0, 0, 0, 0))
          return;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_544
      case 9: {
        if (error >= 0.5) {
          y += ystep;
          error = error - 1;
        }
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_545
      case 10: {
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
#ifdef COMPILE_546
      case 11: {
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
#ifdef COMPILE_547
      case 12: {
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
#ifdef COMPILE_548
      case 13: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_549
      case 14: {
        line.push_back(var);
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_550
      case 15: {
        throw;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_551
      case 16: {
        var.first = x;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_552
      case 17: {
        var.first = y;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_553
      case 18: {
        var.second = x;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_554
      case 19: {
        var.second = y;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_555
      case 20: {
        x0 = x1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_556
      case 21: {
        x0 = y0;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_557
      case 22: {
        x1 = a;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_558
      case 23: {
        x1 = y1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_559
      case 24: {
        y0 = a;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_560
      case 25: {
        y0 = y1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_561
      case 26: {
        y1 = a;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_562
      case 27: {
        ystep = -1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_563
      case 28: {
        ystep = 1;
        var.second = y;

        break;
      }
#endif
      }
      // prophet generated patch
      switch (__choose("__ID5")) {
      case 0: {
        line.push_back(var);

        break;
      }
#ifdef COMPILE_564
      case 1: {
        a = x0;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_565
      case 2: {
        a = x1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_566
      case 3: {
        a = y0;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_567
      case 4: {
        error = error + deltaerr;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_568
      case 5: {
        error = error - 1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_569
      case 6: {
        if (!__is_neg(
                "5-6-0", 11,
                (char **){"y", "x", "ystep", "steep", "x1", "x0", "y0", "y1",
                          "deltax", "deltay", "a"},
                11,
                (int *){y, x, ystep, steep, x1, x0, y0, y1, deltax, deltay, a},
                0, 0, 0, 0, 0, 0))
          line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_570
      case 7: {
        if (__is_neg(
                "5-7-0", 11,
                (char **){"y", "x", "ystep", "steep", "x1", "x0", "y0", "y1",
                          "deltax", "deltay", "a"},
                11,
                (int *){y, x, ystep, steep, x1, x0, y0, y1, deltax, deltay, a},
                0, 0, 0, 0, 0, 0))
          break;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_571
      case 8: {
        if (__is_neg(
                "5-8-0", 11,
                (char **){"y", "x", "ystep", "steep", "x1", "x0", "y0", "y1",
                          "deltax", "deltay", "a"},
                11,
                (int *){y, x, ystep, steep, x1, x0, y0, y1, deltax, deltay, a},
                0, 0, 0, 0, 0, 0))
          return;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_572
      case 9: {
        if (error >= 0.5) {
          y += ystep;
          error = error - 1;
        }
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_573
      case 10: {
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
#ifdef COMPILE_574
      case 11: {
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
#ifdef COMPILE_575
      case 12: {
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
#ifdef COMPILE_576
      case 13: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_577
      case 14: {
        line.push_back(var);
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_578
      case 15: {
        throw;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_579
      case 16: {
        var.first = x;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_580
      case 17: {
        var.first = y;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_581
      case 18: {
        var.second = x;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_582
      case 19: {
        var.second = y;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_583
      case 20: {
        x0 = x1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_584
      case 21: {
        x0 = y0;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_585
      case 22: {
        x1 = a;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_586
      case 23: {
        x1 = y1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_587
      case 24: {
        y0 = a;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_588
      case 25: {
        y0 = y1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_589
      case 26: {
        y1 = a;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_590
      case 27: {
        ystep = -1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_591
      case 28: {
        ystep = 1;
        line.push_back(var);

        break;
      }
#endif
      }
    }
    // prophet generated patch
    switch (__choose("__ID6")) {
    case 0: {
      error = error + deltaerr;

      break;
    }
#ifdef COMPILE_592
    case 1: {
      a = x0;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_593
    case 2: {
      a = x1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_594
    case 3: {
      a = y0;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_595
    case 4: {
      error = error + deltaerr;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_596
    case 5: {
      error = error - 1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_597
    case 6: {
      if (!__is_neg(
              "6-6-0", 11,
              (char **){"y", "ystep", "x", "steep", "x1", "x0", "y0", "y1",
                        "deltax", "deltay", "a"},
              11,
              (int *){y, ystep, x, steep, x1, x0, y0, y1, deltax, deltay, a}, 0,
              0, 0, 0, 0, 0))
        error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_598
    case 7: {
      if (__is_neg(
              "6-7-0", 11,
              (char **){"y", "ystep", "x", "steep", "x1", "x0", "y0", "y1",
                        "deltax", "deltay", "a"},
              11,
              (int *){y, ystep, x, steep, x1, x0, y0, y1, deltax, deltay, a}, 0,
              0, 0, 0, 0, 0))
        break;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_599
    case 8: {
      if (__is_neg(
              "6-8-0", 11,
              (char **){"y", "ystep", "x", "steep", "x1", "x0", "y0", "y1",
                        "deltax", "deltay", "a"},
              11,
              (int *){y, ystep, x, steep, x1, x0, y0, y1, deltax, deltay, a}, 0,
              0, 0, 0, 0, 0))
        return;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_600
    case 9: {
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_601
    case 10: {
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
#ifdef COMPILE_602
    case 11: {
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
#ifdef COMPILE_603
    case 12: {
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
#ifdef COMPILE_604
    case 13: {
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_605
    case 14: {
      line.push_back(var);
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_606
    case 15: {
      throw;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_607
    case 16: {
      var.first = x;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_608
    case 17: {
      var.first = y;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_609
    case 18: {
      var.second = x;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_610
    case 19: {
      var.second = y;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_611
    case 20: {
      x0 = x1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_612
    case 21: {
      x0 = y0;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_613
    case 22: {
      x1 = a;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_614
    case 23: {
      x1 = y1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_615
    case 24: {
      y0 = a;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_616
    case 25: {
      y0 = y1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_617
    case 26: {
      y1 = a;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_618
    case 27: {
      ystep = -1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_619
    case 28: {
      ystep = 1;
      error = error + deltaerr;

      break;
    }
#endif
    }
    // prophet generated patch
    switch (__choose("__ID7")) {
    case 0: {
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#ifdef COMPILE_620
    case 1: {
      a = x0;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_621
    case 2: {
      a = x1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_622
    case 3: {
      a = y0;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_623
    case 4: {
      error = error + deltaerr;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_624
    case 5: {
      error = error - 1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_625
    case 6: {
      if (!__is_neg(
              "7-6-0", 11,
              (char **){"ystep", "y", "x", "steep", "x1", "x0", "y0", "y1",
                        "deltax", "deltay", "a"},
              11,
              (int *){ystep, y, x, steep, x1, x0, y0, y1, deltax, deltay, a}, 0,
              0, 0, 0, 0, 0))
        if (error >= 0.5) {
          y += ystep;
          error = error - 1;
        }

      break;
    }
#endif
#ifdef COMPILE_626
    case 7: {
      if ((error >= 0.5) &&
          !__is_neg(
              "7-7-0", 11,
              (char **){"ystep", "y", "x", "steep", "x1", "x0", "y0", "y1",
                        "deltax", "deltay", "a"},
              11,
              (int *){ystep, y, x, steep, x1, x0, y0, y1, deltax, deltay, a}, 0,
              0, 0, 0, 0, 0)) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_627
    case 8: {
      if ((error >= 0.5) ||
          __is_neg(
              "7-8-0", 11,
              (char **){"ystep", "y", "x", "steep", "x1", "x0", "y0", "y1",
                        "deltax", "deltay", "a"},
              11,
              (int *){ystep, y, x, steep, x1, x0, y0, y1, deltax, deltay, a}, 0,
              0, 0, 0, 0, 0)) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_628
    case 9: {
      if (__is_neg(
              "7-9-0", 11,
              (char **){"ystep", "y", "x", "steep", "x1", "x0", "y0", "y1",
                        "deltax", "deltay", "a"},
              11,
              (int *){ystep, y, x, steep, x1, x0, y0, y1, deltax, deltay, a}, 0,
              0, 0, 0, 0, 0))
        break;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_629
    case 10: {
      if (__is_neg(
              "7-10-0", 11,
              (char **){"ystep", "y", "x", "steep", "x1", "x0", "y0", "y1",
                        "deltax", "deltay", "a"},
              11,
              (int *){ystep, y, x, steep, x1, x0, y0, y1, deltax, deltay, a}, 0,
              0, 0, 0, 0, 0))
        return;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_630
    case 11: {
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
#ifdef COMPILE_631
    case 12: {
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
#ifdef COMPILE_632
    case 13: {
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
#ifdef COMPILE_633
    case 14: {
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
#ifdef COMPILE_634
    case 15: {
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
#ifdef COMPILE_635
    case 16: {
      line.push_back(var);
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_636
    case 17: {
      throw;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_637
    case 18: {
      var.first = x;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_638
    case 19: {
      var.first = y;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_639
    case 20: {
      var.second = x;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_640
    case 21: {
      var.second = y;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_641
    case 22: {
      x0 = x1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_642
    case 23: {
      x0 = y0;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_643
    case 24: {
      x1 = a;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_644
    case 25: {
      x1 = y1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_645
    case 26: {
      y0 = a;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_646
    case 27: {
      y0 = y1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_647
    case 28: {
      y1 = a;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_648
    case 29: {
      ystep = -1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_649
    case 30: {
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

int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "r");
  int x, y;
  // prophet generated patch
  switch (__choose("__ID8")) {
  case 0: {
    fscanf(f, "%d %d\n", &x, &y);

    break;
  }
#ifdef COMPILE_650
  case 1: {
    if (!__is_neg("8-1-0", 5, (char **){"f", "x", "y", "argv", "argc"}, 3,
                  (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      fscanf(f, "%d %d\n", &x, &y);

    break;
  }
#endif
#ifdef COMPILE_651
  case 2: {
    if (__is_neg("8-2-0", 5, (char **){"f", "x", "y", "argv", "argc"}, 3,
                 (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    fscanf(f, "%d %d\n", &x, &y);

    break;
  }
#endif
#ifdef COMPILE_652
  case 3: {
    if (__is_neg("8-3-0", 5, (char **){"f", "x", "y", "argv", "argc"}, 3,
                 (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    fscanf(f, "%d %d\n", &x, &y);

    break;
  }
#endif
#ifdef COMPILE_653
  case 4: {
    throw;
    fscanf(f, "%d %d\n", &x, &y);

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID9")) {
  case 0: {
    fclose(f);

    break;
  }
#ifdef COMPILE_654
  case 1: {
    if (!__is_neg("9-1-0", 5, (char **){"f", "x", "y", "argv", "argc"}, 3,
                  (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      fclose(f);

    break;
  }
#endif
#ifdef COMPILE_655
  case 2: {
    if (__is_neg("9-2-0", 5, (char **){"f", "x", "y", "argv", "argc"}, 3,
                 (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_656
  case 3: {
    if (__is_neg("9-3-0", 5, (char **){"f", "x", "y", "argv", "argc"}, 3,
                 (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_657
  case 4: {
    throw;
    fclose(f);

    break;
  }
#endif
  }
  DiscreteLine line;
  // prophet generated patch
  switch (__choose("__ID10")) {
  case 0: {
    buggy(x, y, line);

    break;
  }
#ifdef COMPILE_658
  case 1: {
    buggy(x, y, line);
    buggy(x, y, line);

    break;
  }
#endif
#ifdef COMPILE_659
  case 2: {
    if (!__is_neg("10-2-0", 5, (char **){"x", "y", "f", "argv", "argc"}, 3,
                  (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      buggy(x, y, line);

    break;
  }
#endif
#ifdef COMPILE_660
  case 3: {
    if (__is_neg("10-3-0", 5, (char **){"x", "y", "f", "argv", "argc"}, 3,
                 (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    buggy(x, y, line);

    break;
  }
#endif
#ifdef COMPILE_661
  case 4: {
    if (__is_neg("10-4-0", 5, (char **){"x", "y", "f", "argv", "argc"}, 3,
                 (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    buggy(x, y, line);

    break;
  }
#endif
#ifdef COMPILE_662
  case 5: {
    throw;
    buggy(x, y, line);

    break;
  }
#endif
  }

  // prophet generated patch
  switch (__choose("__ID11")) {
  case 0: {
    for (unsigned int i = 0, end = line.size(); i != line.size(); i++)
      printf("%d %d\n", line[i].first, line[i].second);

    break;
  }
#ifdef COMPILE_663
  case 1: {
    buggy(x, y, line);
    for (unsigned int i = 0, end = line.size(); i != line.size(); i++)
      printf("%d %d\n", line[i].first, line[i].second);

    break;
  }
#endif
#ifdef COMPILE_664
  case 2: {
    if (!__is_neg("11-2-0", 5, (char **){"x", "y", "f", "argv", "argc"}, 3,
                  (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      for (unsigned int i = 0, end = line.size(); i != line.size(); i++)
        printf("%d %d\n", line[i].first, line[i].second);

    break;
  }
#endif
#ifdef COMPILE_665
  case 3: {
    if (__is_neg("11-3-0", 5, (char **){"x", "y", "f", "argv", "argc"}, 3,
                 (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    for (unsigned int i = 0, end = line.size(); i != line.size(); i++)
      printf("%d %d\n", line[i].first, line[i].second);

    break;
  }
#endif
#ifdef COMPILE_666
  case 4: {
    if (__is_neg("11-4-0", 5, (char **){"x", "y", "f", "argv", "argc"}, 3,
                 (int *){x, y, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    for (unsigned int i = 0, end = line.size(); i != line.size(); i++)
      printf("%d %d\n", line[i].first, line[i].second);

    break;
  }
#endif
#ifdef COMPILE_667
  case 5: {
    throw;
    for (unsigned int i = 0, end = line.size(); i != line.size(); i++)
      printf("%d %d\n", line[i].first, line[i].second);

    break;
  }
#endif
  }
}
