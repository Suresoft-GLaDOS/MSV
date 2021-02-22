extern "C" {
int __get_mutant();
int __is_neg(int, ...);
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
    if (!__is_neg(1))
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
    if ((steep) && !__is_neg(1)) {
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
    if ((steep) || __is_neg(1)) {
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
    if (__is_neg(1))
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
    if (!__is_neg(1))
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
    if ((x0 > x1) && !__is_neg(1)) {
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
    if ((x0 > x1) || __is_neg(1)) {
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
    if (__is_neg(1))
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
  switch (__choose("__ID3")) {
  case 0: {
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#ifdef COMPILE_54
  case 1: {
    a = x0;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_55
  case 2: {
    a = x1;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_56
  case 3: {
    a = y0;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_57
  case 4: {
    error = error + deltaerr;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_58
  case 5: {
    error = error - 1;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_59
  case 6: {
    if (!__is_neg(1))
      if (y0 < y1)
        switch (__choose("__ID2")) {
        case 0: {
          ystep = 1;

          break;
        }
#ifdef COMPILE_34
        case 1: {
          a = x0;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_35
        case 2: {
          a = x1;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_36
        case 3: {
          a = y0;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_37
        case 4: {
          error = error + deltaerr;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_38
        case 5: {
          error = error - 1;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_39
        case 6: {
          if (!__is_neg(1))
            ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_40
        case 7: {
          if (__is_neg(1))
            return;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_41
        case 8: {
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
#ifdef COMPILE_42
        case 9: {
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
#ifdef COMPILE_43
        case 10: {
          if (y0 < y1)
            ystep = 1;
          else
            ystep = -1;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_44
        case 11: {
          throw;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_45
        case 12: {
          x0 = x1;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_46
        case 13: {
          x0 = y0;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_47
        case 14: {
          x1 = a;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_48
        case 15: {
          x1 = y1;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_49
        case 16: {
          y0 = a;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_50
        case 17: {
          y0 = y1;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_51
        case 18: {
          y1 = a;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_52
        case 19: {
          ystep = -1;
          ystep = 1;

          break;
        }
#endif
#ifdef COMPILE_53
        case 20: {
          ystep = 1;
          ystep = 1;

          break;
        }
#endif
        }
      else
        ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_60
  case 7: {
    if ((y0 < y1) && !__is_neg(1))
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_61
  case 8: {
    if ((y0 < y1) || __is_neg(1))
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_62
  case 9: {
    if (__is_neg(1))
      return;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_63
  case 10: {
    if (steep) {
      a = x0;
      x0 = y0;
      y0 = a;
      a = x1;
      x1 = y1;
      y1 = a;
    }
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_64
  case 11: {
    if (x0 > x1) {
      a = x0;
      x0 = x1;
      x1 = a;
      a = y0;
      y0 = y1;
      y1 = a;
    }
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_65
  case 12: {
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_66
  case 13: {
    throw;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_67
  case 14: {
    x0 = x1;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_68
  case 15: {
    x0 = y0;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_69
  case 16: {
    x1 = a;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_70
  case 17: {
    x1 = y1;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_71
  case 18: {
    y0 = a;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_72
  case 19: {
    y0 = y1;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_73
  case 20: {
    y1 = a;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_74
  case 21: {
    ystep = -1;
    if (y0 < y1)
      switch (__choose("__ID2")) {
      case 0: {
        ystep = 1;

        break;
      }
#ifdef COMPILE_34
      case 1: {
        a = x0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_35
      case 2: {
        a = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_36
      case 3: {
        a = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_37
      case 4: {
        error = error + deltaerr;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_38
      case 5: {
        error = error - 1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_39
      case 6: {
        if (!__is_neg(1))
          ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_40
      case 7: {
        if (__is_neg(1))
          return;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_41
      case 8: {
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
#ifdef COMPILE_42
      case 9: {
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
#ifdef COMPILE_43
      case 10: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_44
      case 11: {
        throw;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_45
      case 12: {
        x0 = x1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_46
      case 13: {
        x0 = y0;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_47
      case 14: {
        x1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_48
      case 15: {
        x1 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_49
      case 16: {
        y0 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_50
      case 17: {
        y0 = y1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_51
      case 18: {
        y1 = a;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_52
      case 19: {
        ystep = -1;
        ystep = 1;

        break;
      }
#endif
#ifdef COMPILE_53
      case 20: {
        ystep = 1;
        ystep = 1;

        break;
      }
#endif
      }
    else
      ystep = -1;

    break;
  }
#endif
#ifdef COMPILE_75
  case 22: {
    switch (__choose("__ID2")) {
    case 0: {
      ystep = 1;

      break;
    }
#ifdef COMPILE_34
    case 1: {
      a = x0;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_35
    case 2: {
      a = x1;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_36
    case 3: {
      a = y0;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_37
    case 4: {
      error = error + deltaerr;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_38
    case 5: {
      error = error - 1;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_39
    case 6: {
      if (!__is_neg(1))
        ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_40
    case 7: {
      if (__is_neg(1))
        return;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_41
    case 8: {
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
#ifdef COMPILE_42
    case 9: {
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
#ifdef COMPILE_43
    case 10: {
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_44
    case 11: {
      throw;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_45
    case 12: {
      x0 = x1;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_46
    case 13: {
      x0 = y0;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_47
    case 14: {
      x1 = a;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_48
    case 15: {
      x1 = y1;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_49
    case 16: {
      y0 = a;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_50
    case 17: {
      y0 = y1;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_51
    case 18: {
      y1 = a;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_52
    case 19: {
      ystep = -1;
      ystep = 1;

      break;
    }
#endif
#ifdef COMPILE_53
    case 20: {
      ystep = 1;
      ystep = 1;

      break;
    }
#endif
    }
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
      switch (__choose("__ID4")) {
      case 0: {
        var.first = x;

        break;
      }
#ifdef COMPILE_76
      case 1: {
        a = x0;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_77
      case 2: {
        a = x1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_78
      case 3: {
        a = y0;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_79
      case 4: {
        error = error + deltaerr;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_80
      case 5: {
        error = error - 1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_81
      case 6: {
        if (!__is_neg(1))
          var.first = x;

        break;
      }
#endif
#ifdef COMPILE_82
      case 7: {
        if (__is_neg(1))
          break;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_83
      case 8: {
        if (__is_neg(1))
          return;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_84
      case 9: {
        if (error >= 0.5) {
          y += ystep;
          error = error - 1;
        }
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_85
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
#ifdef COMPILE_86
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
#ifdef COMPILE_87
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
#ifdef COMPILE_88
      case 13: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_89
      case 14: {
        line.push_back(var);
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_90
      case 15: {
        throw;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_91
      case 16: {
        var.first = x;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_92
      case 17: {
        var.first = y;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_93
      case 18: {
        var.second = x;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_94
      case 19: {
        var.second = y;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_95
      case 20: {
        x0 = x1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_96
      case 21: {
        x0 = y0;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_97
      case 22: {
        x1 = a;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_98
      case 23: {
        x1 = y1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_99
      case 24: {
        y0 = a;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_100
      case 25: {
        y0 = y1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_101
      case 26: {
        y1 = a;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_102
      case 27: {
        ystep = -1;
        var.first = x;

        break;
      }
#endif
#ifdef COMPILE_103
      case 28: {
        ystep = 1;
        var.first = x;

        break;
      }
#endif
      }
      // prophet generated patch
      switch (__choose("__ID5")) {
      case 0: {
        var.second = y;

        break;
      }
#ifdef COMPILE_104
      case 1: {
        a = x0;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_105
      case 2: {
        a = x1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_106
      case 3: {
        a = y0;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_107
      case 4: {
        error = error + deltaerr;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_108
      case 5: {
        error = error - 1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_109
      case 6: {
        if (!__is_neg(1))
          var.second = y;

        break;
      }
#endif
#ifdef COMPILE_110
      case 7: {
        if (__is_neg(1))
          break;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_111
      case 8: {
        if (__is_neg(1))
          return;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_112
      case 9: {
        if (error >= 0.5) {
          y += ystep;
          error = error - 1;
        }
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_113
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
#ifdef COMPILE_114
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
#ifdef COMPILE_115
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
#ifdef COMPILE_116
      case 13: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_117
      case 14: {
        line.push_back(var);
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_118
      case 15: {
        throw;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_119
      case 16: {
        var.first = x;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_120
      case 17: {
        var.first = y;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_121
      case 18: {
        var.second = x;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_122
      case 19: {
        var.second = y;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_123
      case 20: {
        x0 = x1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_124
      case 21: {
        x0 = y0;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_125
      case 22: {
        x1 = a;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_126
      case 23: {
        x1 = y1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_127
      case 24: {
        y0 = a;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_128
      case 25: {
        y0 = y1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_129
      case 26: {
        y1 = a;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_130
      case 27: {
        ystep = -1;
        var.second = y;

        break;
      }
#endif
#ifdef COMPILE_131
      case 28: {
        ystep = 1;
        var.second = y;

        break;
      }
#endif
      }
      // prophet generated patch
      switch (__choose("__ID6")) {
      case 0: {
        line.push_back(var);

        break;
      }
#ifdef COMPILE_132
      case 1: {
        a = x0;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_133
      case 2: {
        a = x1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_134
      case 3: {
        a = y0;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_135
      case 4: {
        error = error + deltaerr;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_136
      case 5: {
        error = error - 1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_137
      case 6: {
        if (!__is_neg(1))
          line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_138
      case 7: {
        if (__is_neg(1))
          break;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_139
      case 8: {
        if (__is_neg(1))
          return;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_140
      case 9: {
        if (error >= 0.5) {
          y += ystep;
          error = error - 1;
        }
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_141
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
#ifdef COMPILE_142
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
#ifdef COMPILE_143
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
#ifdef COMPILE_144
      case 13: {
        if (y0 < y1)
          ystep = 1;
        else
          ystep = -1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_145
      case 14: {
        line.push_back(var);
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_146
      case 15: {
        throw;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_147
      case 16: {
        var.first = x;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_148
      case 17: {
        var.first = y;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_149
      case 18: {
        var.second = x;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_150
      case 19: {
        var.second = y;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_151
      case 20: {
        x0 = x1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_152
      case 21: {
        x0 = y0;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_153
      case 22: {
        x1 = a;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_154
      case 23: {
        x1 = y1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_155
      case 24: {
        y0 = a;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_156
      case 25: {
        y0 = y1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_157
      case 26: {
        y1 = a;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_158
      case 27: {
        ystep = -1;
        line.push_back(var);

        break;
      }
#endif
#ifdef COMPILE_159
      case 28: {
        ystep = 1;
        line.push_back(var);

        break;
      }
#endif
      }
    }
    // prophet generated patch
    switch (__choose("__ID7")) {
    case 0: {
      error = error + deltaerr;

      break;
    }
#ifdef COMPILE_160
    case 1: {
      a = x0;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_161
    case 2: {
      a = x1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_162
    case 3: {
      a = y0;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_163
    case 4: {
      error = error + deltaerr;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_164
    case 5: {
      error = error - 1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_165
    case 6: {
      if (!__is_neg(1))
        error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_166
    case 7: {
      if (__is_neg(1))
        break;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_167
    case 8: {
      if (__is_neg(1))
        return;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_168
    case 9: {
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_169
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
#ifdef COMPILE_170
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
#ifdef COMPILE_171
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
#ifdef COMPILE_172
    case 13: {
      if (y0 < y1)
        ystep = 1;
      else
        ystep = -1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_173
    case 14: {
      line.push_back(var);
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_174
    case 15: {
      throw;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_175
    case 16: {
      var.first = x;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_176
    case 17: {
      var.first = y;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_177
    case 18: {
      var.second = x;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_178
    case 19: {
      var.second = y;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_179
    case 20: {
      x0 = x1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_180
    case 21: {
      x0 = y0;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_181
    case 22: {
      x1 = a;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_182
    case 23: {
      x1 = y1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_183
    case 24: {
      y0 = a;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_184
    case 25: {
      y0 = y1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_185
    case 26: {
      y1 = a;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_186
    case 27: {
      ystep = -1;
      error = error + deltaerr;

      break;
    }
#endif
#ifdef COMPILE_187
    case 28: {
      ystep = 1;
      error = error + deltaerr;

      break;
    }
#endif
    }
    // prophet generated patch
    switch (__choose("__ID8")) {
    case 0: {
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#ifdef COMPILE_188
    case 1: {
      a = x0;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_189
    case 2: {
      a = x1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_190
    case 3: {
      a = y0;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_191
    case 4: {
      error = error + deltaerr;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_192
    case 5: {
      error = error - 1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_193
    case 6: {
      if (!__is_neg(1))
        if (error >= 0.5) {
          y += ystep;
          error = error - 1;
        }

      break;
    }
#endif
#ifdef COMPILE_194
    case 7: {
      if ((error >= 0.5) && !__is_neg(1)) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_195
    case 8: {
      if ((error >= 0.5) || __is_neg(1)) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_196
    case 9: {
      if (__is_neg(1))
        break;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_197
    case 10: {
      if (__is_neg(1))
        return;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_198
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
#ifdef COMPILE_199
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
#ifdef COMPILE_200
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
#ifdef COMPILE_201
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
#ifdef COMPILE_202
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
#ifdef COMPILE_203
    case 16: {
      line.push_back(var);
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_204
    case 17: {
      throw;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_205
    case 18: {
      var.first = x;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_206
    case 19: {
      var.first = y;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_207
    case 20: {
      var.second = x;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_208
    case 21: {
      var.second = y;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_209
    case 22: {
      x0 = x1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_210
    case 23: {
      x0 = y0;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_211
    case 24: {
      x1 = a;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_212
    case 25: {
      x1 = y1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_213
    case 26: {
      y0 = a;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_214
    case 27: {
      y0 = y1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_215
    case 28: {
      y1 = a;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_216
    case 29: {
      ystep = -1;
      if (error >= 0.5) {
        y += ystep;
        error = error - 1;
      }

      break;
    }
#endif
#ifdef COMPILE_217
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
  switch (__choose("__ID9")) {
  case 0: {
    fscanf(f, "%d %d\n", &x, &y);

    break;
  }
#ifdef COMPILE_218
  case 1: {
    if (!__is_neg(1))
      fscanf(f, "%d %d\n", &x, &y);

    break;
  }
#endif
#ifdef COMPILE_219
  case 2: {
    if (__is_neg(1))
      return 0;
    fscanf(f, "%d %d\n", &x, &y);

    break;
  }
#endif
#ifdef COMPILE_220
  case 3: {
    if (__is_neg(1))
      return 1;
    fscanf(f, "%d %d\n", &x, &y);

    break;
  }
#endif
#ifdef COMPILE_221
  case 4: {
    throw;
    fscanf(f, "%d %d\n", &x, &y);

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID10")) {
  case 0: {
    fclose(f);

    break;
  }
#ifdef COMPILE_222
  case 1: {
    if (!__is_neg(1))
      fclose(f);

    break;
  }
#endif
#ifdef COMPILE_223
  case 2: {
    if (__is_neg(1))
      return 0;
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_224
  case 3: {
    if (__is_neg(1))
      return 1;
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_225
  case 4: {
    throw;
    fclose(f);

    break;
  }
#endif
  }
  DiscreteLine line;
  // prophet generated patch
  switch (__choose("__ID11")) {
  case 0: {
    buggy(x, y, line);

    break;
  }
#ifdef COMPILE_226
  case 1: {
    buggy(x, y, line);
    buggy(x, y, line);

    break;
  }
#endif
#ifdef COMPILE_227
  case 2: {
    if (!__is_neg(1))
      buggy(x, y, line);

    break;
  }
#endif
#ifdef COMPILE_228
  case 3: {
    if (__is_neg(1))
      return 0;
    buggy(x, y, line);

    break;
  }
#endif
#ifdef COMPILE_229
  case 4: {
    if (__is_neg(1))
      return 1;
    buggy(x, y, line);

    break;
  }
#endif
#ifdef COMPILE_230
  case 5: {
    throw;
    buggy(x, y, line);

    break;
  }
#endif
  }

  for (unsigned i = 0, end = line.size(); i != line.size(); i++)
    printf("%d %d\n", line[i].first, line[i].second);
}
