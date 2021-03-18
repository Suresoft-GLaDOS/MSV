int __get_mutant();
int __is_neg(const char *, int, const int *, int, const char *, int,
             const void **, int, const double *, int, ...);
int __abst_hole();
int __choose(char *);
void *memset(void *, int, unsigned long);
#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *f = fopen(argv[1], "r");
  int a, b;
  // prophet generated patch
  switch (__choose("__ID0")) {
  case 0: {
    fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#ifdef COMPILE_0
  case 1: {
    _IO_vfscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_1
  case 2: {
    fclose((f));
    fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_2
  case 3: {
    fclose(f);
    fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_3
  case 4: {
    fprintf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_4
  case 5: {
    fscanf((f), "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_5
  case 6: {
    fscanf((f), "%d %d\n", &a, &b);
    fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_6
  case 7: {
    fscanf(f, "%d %d\n", &a, &b);
    fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_7
  case 8: {
    if (!__is_neg("0-8", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0,
                  0, 5, "f", "a", "b", "argv", "argc"))
      fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_8
  case 9: {
    if (__is_neg("0-9", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0,
                 0, 5, "f", "a", "b", "argv", "argc"))
      return 0;
    fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_9
  case 10: {
    if (__is_neg("0-10", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0,
                 0, 5, "f", "a", "b", "argv", "argc"))
      return 1;
    fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_10
  case 11: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }
    fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_11
  case 12: {
    printf("%d\n", 0);
    fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_12
  case 13: {
    printf("%d\n", 1);
    fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_13
  case 14: {
    printf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_14
  case 15: {
    scanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_15
  case 16: {
    sprintf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_16
  case 17: {
    sscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
  }
  // prophet generated patch
  switch (__choose("__ID1")) {
  case 0: {
    fclose(f);

    break;
  }
#ifdef COMPILE_17
  case 1: {
    _IO_feof(f);

    break;
  }
#endif
#ifdef COMPILE_18
  case 2: {
    _IO_ferror(f);

    break;
  }
#endif
#ifdef COMPILE_19
  case 3: {
    _IO_flockfile(f);

    break;
  }
#endif
#ifdef COMPILE_20
  case 4: {
    _IO_free_backup_area(f);

    break;
  }
#endif
#ifdef COMPILE_21
  case 5: {
    _IO_ftrylockfile(f);

    break;
  }
#endif
#ifdef COMPILE_22
  case 6: {
    _IO_funlockfile(f);

    break;
  }
#endif
#ifdef COMPILE_23
  case 7: {
    _IO_getc(f);

    break;
  }
#endif
#ifdef COMPILE_24
  case 8: {
    _IO_peekc_locked(f);

    break;
  }
#endif
#ifdef COMPILE_25
  case 9: {
    __uflow(f);

    break;
  }
#endif
#ifdef COMPILE_26
  case 10: {
    __underflow(f);

    break;
  }
#endif
#ifdef COMPILE_27
  case 11: {
    clearerr(f);

    break;
  }
#endif
#ifdef COMPILE_28
  case 12: {
    clearerr_unlocked(f);

    break;
  }
#endif
#ifdef COMPILE_29
  case 13: {
    fclose((f));

    break;
  }
#endif
#ifdef COMPILE_30
  case 14: {
    fclose((f));
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_31
  case 15: {
    fclose(f);
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_32
  case 16: {
    feof(f);

    break;
  }
#endif
#ifdef COMPILE_33
  case 17: {
    feof_unlocked(f);

    break;
  }
#endif
#ifdef COMPILE_34
  case 18: {
    ferror(f);

    break;
  }
#endif
#ifdef COMPILE_35
  case 19: {
    ferror_unlocked(f);

    break;
  }
#endif
#ifdef COMPILE_36
  case 20: {
    fflush(f);

    break;
  }
#endif
#ifdef COMPILE_37
  case 21: {
    fflush_unlocked(f);

    break;
  }
#endif
#ifdef COMPILE_38
  case 22: {
    fgetc(f);

    break;
  }
#endif
#ifdef COMPILE_39
  case 23: {
    fgetc_unlocked(f);

    break;
  }
#endif
#ifdef COMPILE_40
  case 24: {
    fileno(f);

    break;
  }
#endif
#ifdef COMPILE_41
  case 25: {
    fileno_unlocked(f);

    break;
  }
#endif
#ifdef COMPILE_42
  case 26: {
    flockfile(f);

    break;
  }
#endif
#ifdef COMPILE_43
  case 27: {
    fscanf((f), "%d %d\n", &a, &b);
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_44
  case 28: {
    fscanf(f, "%d %d\n", &a, &b);
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_45
  case 29: {
    ftell(f);

    break;
  }
#endif
#ifdef COMPILE_46
  case 30: {
    ftello(f);

    break;
  }
#endif
#ifdef COMPILE_47
  case 31: {
    ftrylockfile(f);

    break;
  }
#endif
#ifdef COMPILE_48
  case 32: {
    funlockfile(f);

    break;
  }
#endif
#ifdef COMPILE_49
  case 33: {
    getc(f);

    break;
  }
#endif
#ifdef COMPILE_50
  case 34: {
    getc_unlocked(f);

    break;
  }
#endif
#ifdef COMPILE_51
  case 35: {
    getw(f);

    break;
  }
#endif
#ifdef COMPILE_52
  case 36: {
    if (!__is_neg("1-36", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv},
                  0, 0, 5, "f", "a", "b", "argv", "argc"))
      fclose(f);

    break;
  }
#endif
#ifdef COMPILE_53
  case 37: {
    if (__is_neg("1-37", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0,
                 0, 5, "f", "a", "b", "argv", "argc"))
      return 0;
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_54
  case 38: {
    if (__is_neg("1-38", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0,
                 0, 5, "f", "a", "b", "argv", "argc"))
      return 1;
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_55
  case 39: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_56
  case 40: {
    pclose(f);

    break;
  }
#endif
#ifdef COMPILE_57
  case 41: {
    printf("%d\n", 0);
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_58
  case 42: {
    printf("%d\n", 1);
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_59
  case 43: {
    rewind(f);

    break;
  }
#endif
#ifdef COMPILE_60
  case 44: {
    setlinebuf(f);

    break;
  }
#endif
  }

  // prophet generated patch
  switch (__choose("__ID2")) {
  case 0: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#ifdef COMPILE_61
  case 1: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_62
  case 2: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_63
  case 3: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_64
  case 4: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_65
  case 5: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_66
  case 6: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      if (!__is_neg("2-6", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv},
                    0, 0, 5, "a", "b", "f", "argv", "argc"))
        printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_67
  case 7: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-7", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv},
                   0, 0, 5, "a", "b", "f", "argv", "argc"))
        return 0;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_68
  case 8: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-8", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv},
                   0, 0, 5, "a", "b", "f", "argv", "argc"))
        return 1;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_69
  case 9: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      if (a > b) {
        printf("%d\n", 0);
      } else {
        printf("%d\n", 1);
      }
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_70
  case 10: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_71
  case 11: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_72
  case 12: {
    if (a > b) { // >=
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_73
  case 13: {
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_74
  case 14: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_75
  case 15: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_76
  case 16: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_77
  case 17: {
    if (!__is_neg("2-17", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv},
                  0, 0, 5, "a", "b", "f", "argv", "argc") &&
        (a > b)) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_78
  case 18: {
    if (!__is_neg("2-18", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv},
                  0, 0, 5, "a", "b", "f", "argv", "argc"))
      if (a > b) {
        printf("%d\n", 0);
      } else {
        printf("%d\n", 1);
      }

    break;
  }
#endif
#ifdef COMPILE_79
  case 19: {
    if ((a > b) &&
        !__is_neg("2-19", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv},
                  0, 0, 5, "a", "b", "f", "argv", "argc")) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_80
  case 20: {
    if ((a > b) ||
        __is_neg("2-20", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0,
                 0, 5, "a", "b", "f", "argv", "argc")) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_81
  case 21: {
    if (__is_neg("2-21", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0,
                 0, 5, "a", "b", "f", "argv", "argc"))
      return 0;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_82
  case 22: {
    if (__is_neg("2-22", 3, (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0,
                 0, 5, "a", "b", "f", "argv", "argc"))
      return 1;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_83
  case 23: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_84
  case 24: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_85
  case 25: {
    printf("%d\n", 1);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
  }

  return 0;
}
