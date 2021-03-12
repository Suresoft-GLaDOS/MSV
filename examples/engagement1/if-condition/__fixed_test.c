int __get_mutant();
int __is_neg(char *, int, char **, int, int *, int, char *, int, void **, int,
             double *);
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
    if (!__is_neg("0-8-0", 5, (char **){"f", "a", "b", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_8
  case 9: {
    if (__is_neg("0-9-0", 5, (char **){"f", "a", "b", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    fscanf(f, "%d %d\n", &a, &b);

    break;
  }
#endif
#ifdef COMPILE_9
  case 10: {
    if (__is_neg("0-10-0", 5, (char **){"f", "a", "b", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
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
    if (!__is_neg("1-36-0", 5, (char **){"f", "a", "b", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      fclose(f);

    break;
  }
#endif
#ifdef COMPILE_53
  case 37: {
    if (__is_neg("1-37-0", 5, (char **){"f", "a", "b", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    fclose(f);

    break;
  }
#endif
#ifdef COMPILE_54
  case 38: {
    if (__is_neg("1-38-0", 5, (char **){"f", "a", "b", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
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
      if (!__is_neg("2-6-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                    (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
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
      if (__is_neg("2-7-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
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
      if (__is_neg("2-8-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
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
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_75
  case 15: {
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_76
  case 16: {
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_77
  case 17: {
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_78
  case 18: {
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_79
  case 19: {
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (!__is_neg("2-19-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                    (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_80
  case 20: {
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-20-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 0;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_81
  case 21: {
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-21-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 1;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_82
  case 22: {
    fclose((f));
    if (a > b) {
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
#ifdef COMPILE_83
  case 23: {
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_84
  case 24: {
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_85
  case 25: {
    fclose((f));
    if (a > b) {
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_86
  case 26: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_87
  case 27: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_88
  case 28: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_89
  case 29: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_90
  case 30: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_91
  case 31: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_92
  case 32: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (!__is_neg("2-32-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                    (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_93
  case 33: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-33-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 0;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_94
  case 34: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-34-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 1;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_95
  case 35: {
    fclose(f);
    if (a > b) {
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
#ifdef COMPILE_96
  case 36: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_97
  case 37: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_98
  case 38: {
    fclose(f);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_99
  case 39: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_100
  case 40: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_101
  case 41: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_102
  case 42: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_103
  case 43: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_104
  case 44: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_105
  case 45: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (!__is_neg("2-45-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                    (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_106
  case 46: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-46-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 0;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_107
  case 47: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-47-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 1;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_108
  case 48: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
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
#ifdef COMPILE_109
  case 49: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_110
  case 50: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_111
  case 51: {
    fscanf((f), "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_112
  case 52: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_113
  case 53: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_114
  case 54: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_115
  case 55: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_116
  case 56: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_117
  case 57: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_118
  case 58: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (!__is_neg("2-58-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                    (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_119
  case 59: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-59-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 0;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_120
  case 60: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-60-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 1;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_121
  case 61: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
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
#ifdef COMPILE_122
  case 62: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_123
  case 63: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_124
  case 64: {
    fscanf(f, "%d %d\n", &a, &b);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_125
  case 65: {
    if (!__is_neg("2-65-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0) &&
        (a > b)) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_126
  case 66: {
    if (!__is_neg("2-66-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0) &&
        (a > b)) {
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_127
  case 67: {
    if (!__is_neg("2-67-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0) &&
        (a > b)) {
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_128
  case 68: {
    if (!__is_neg("2-68-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0) &&
        (a > b)) {
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_129
  case 69: {
    if (!__is_neg("2-69-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0) &&
        (a > b)) {
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_130
  case 70: {
    if (!__is_neg("2-70-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0) &&
        (a > b)) {
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_131
  case 71: {
    if (!__is_neg("2-71-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0) &&
        (a > b)) {
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
#ifdef COMPILE_132
  case 72: {
    if (!__is_neg("2-72-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0) &&
        (a > b)) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_133
  case 73: {
    if (!__is_neg("2-73-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0) &&
        (a > b)) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_134
  case 74: {
    if (!__is_neg("2-74-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0) &&
        (a > b)) {
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_135
  case 75: {
    if (!__is_neg("2-75-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      if (a > b) {
        printf("%d\n", 0);
      } else {
        printf("%d\n", 1);
      }

    break;
  }
#endif
#ifdef COMPILE_136
  case 76: {
    if (!__is_neg("2-76-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      if (a > b) {
        printf("%d\n", 0);
      } else {
        __overflow("%d\n", 1);
      }

    break;
  }
#endif
#ifdef COMPILE_137
  case 77: {
    if (!__is_neg("2-77-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      if (a > b) {
        printf("%d\n", 0);
      } else {
        fclose((f));
        printf("%d\n", 1);
      }

    break;
  }
#endif
#ifdef COMPILE_138
  case 78: {
    if (!__is_neg("2-78-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      if (a > b) {
        printf("%d\n", 0);
      } else {
        fclose(f);
        printf("%d\n", 1);
      }

    break;
  }
#endif
#ifdef COMPILE_139
  case 79: {
    if (!__is_neg("2-79-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      if (a > b) {
        printf("%d\n", 0);
      } else {
        fscanf((f), "%d %d\n", &a, &b);
        printf("%d\n", 1);
      }

    break;
  }
#endif
#ifdef COMPILE_140
  case 80: {
    if (!__is_neg("2-80-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      if (a > b) {
        printf("%d\n", 0);
      } else {
        fscanf(f, "%d %d\n", &a, &b);
        printf("%d\n", 1);
      }

    break;
  }
#endif
#ifdef COMPILE_141
  case 81: {
    if (!__is_neg("2-81-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      if (a > b) {
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
#ifdef COMPILE_142
  case 82: {
    if (!__is_neg("2-82-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      if (a > b) {
        printf("%d\n", 0);
      } else {
        printf("%d\n", 0);
        printf("%d\n", 1);
      }

    break;
  }
#endif
#ifdef COMPILE_143
  case 83: {
    if (!__is_neg("2-83-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      if (a > b) {
        printf("%d\n", 0);
      } else {
        printf("%d\n", 1);
        printf("%d\n", 1);
      }

    break;
  }
#endif
#ifdef COMPILE_144
  case 84: {
    if (!__is_neg("2-84-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      if (a > b) {
        printf("%d\n", 0);
      } else {
        scanf("%d\n", 1);
      }

    break;
  }
#endif
#ifdef COMPILE_145
  case 85: {
    if ((a > b) &&
        !__is_neg("2-85-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_146
  case 86: {
    if ((a > b) &&
        !__is_neg("2-86-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_147
  case 87: {
    if ((a > b) &&
        !__is_neg("2-87-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_148
  case 88: {
    if ((a > b) &&
        !__is_neg("2-88-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_149
  case 89: {
    if ((a > b) &&
        !__is_neg("2-89-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_150
  case 90: {
    if ((a > b) &&
        !__is_neg("2-90-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_151
  case 91: {
    if ((a > b) &&
        !__is_neg("2-91-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
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
#ifdef COMPILE_152
  case 92: {
    if ((a > b) &&
        !__is_neg("2-92-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_153
  case 93: {
    if ((a > b) &&
        !__is_neg("2-93-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_154
  case 94: {
    if ((a > b) &&
        !__is_neg("2-94-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                  (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_155
  case 95: {
    if ((a > b) ||
        __is_neg("2-95-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_156
  case 96: {
    if ((a > b) ||
        __is_neg("2-96-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_157
  case 97: {
    if ((a > b) ||
        __is_neg("2-97-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_158
  case 98: {
    if ((a > b) ||
        __is_neg("2-98-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_159
  case 99: {
    if ((a > b) ||
        __is_neg("2-99-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_160
  case 100: {
    if ((a > b) ||
        __is_neg("2-100-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_161
  case 101: {
    if ((a > b) ||
        __is_neg("2-101-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
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
#ifdef COMPILE_162
  case 102: {
    if ((a > b) ||
        __is_neg("2-102-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_163
  case 103: {
    if ((a > b) ||
        __is_neg("2-103-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_164
  case 104: {
    if ((a > b) ||
        __is_neg("2-104-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0)) {
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_165
  case 105: {
    if (__is_neg("2-105-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_166
  case 106: {
    if (__is_neg("2-106-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_167
  case 107: {
    if (__is_neg("2-107-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_168
  case 108: {
    if (__is_neg("2-108-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_169
  case 109: {
    if (__is_neg("2-109-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_170
  case 110: {
    if (__is_neg("2-110-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_171
  case 111: {
    if (__is_neg("2-111-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    if (a > b) {
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
#ifdef COMPILE_172
  case 112: {
    if (__is_neg("2-112-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_173
  case 113: {
    if (__is_neg("2-113-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_174
  case 114: {
    if (__is_neg("2-114-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 0;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_175
  case 115: {
    if (__is_neg("2-115-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_176
  case 116: {
    if (__is_neg("2-116-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_177
  case 117: {
    if (__is_neg("2-117-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_178
  case 118: {
    if (__is_neg("2-118-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_179
  case 119: {
    if (__is_neg("2-119-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_180
  case 120: {
    if (__is_neg("2-120-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_181
  case 121: {
    if (__is_neg("2-121-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    if (a > b) {
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
#ifdef COMPILE_182
  case 122: {
    if (__is_neg("2-122-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_183
  case 123: {
    if (__is_neg("2-123-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_184
  case 124: {
    if (__is_neg("2-124-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                 (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
      return 1;
    if (a > b) {
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_185
  case 125: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_186
  case 126: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_187
  case 127: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_188
  case 128: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_189
  case 129: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_190
  case 130: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_191
  case 131: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (!__is_neg("2-131-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                    (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_192
  case 132: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-132-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 0;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_193
  case 133: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-133-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 1;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_194
  case 134: {
    if (a > b) {
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
#ifdef COMPILE_195
  case 135: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_196
  case 136: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_197
  case 137: {
    if (a > b) {
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_198
  case 138: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_199
  case 139: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      __overflow("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_200
  case 140: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose((f));
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_201
  case 141: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fclose(f);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_202
  case 142: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf((f), "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_203
  case 143: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      fscanf(f, "%d %d\n", &a, &b);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_204
  case 144: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (!__is_neg("2-144-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                    (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_205
  case 145: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-145-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 0;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_206
  case 146: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      if (__is_neg("2-146-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                   (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
        return 1;
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_207
  case 147: {
    printf("%d\n", 0);
    if (a > b) {
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
#ifdef COMPILE_208
  case 148: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 0);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_209
  case 149: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      printf("%d\n", 1);
      printf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_210
  case 150: {
    printf("%d\n", 0);
    if (a > b) {
      printf("%d\n", 0);
    } else {
      scanf("%d\n", 1);
    }

    break;
  }
#endif
#ifdef COMPILE_211
  case 151: {
    printf("%d\n", 1);
  }

  break;
  }
#endif
#ifdef COMPILE_212
case 152: {
  __overflow("%d\n", 1);
}

break;
}
#endif
#ifdef COMPILE_213
case 153: {
  fclose((f));
  printf("%d\n", 1);
}

break;
}
#endif
#ifdef COMPILE_214
case 154: {
  fclose(f);
  printf("%d\n", 1);
}

break;
}
#endif
#ifdef COMPILE_215
case 155: {
  fscanf((f), "%d %d\n", &a, &b);
  printf("%d\n", 1);
}

break;
}
#endif
#ifdef COMPILE_216
case 156: {
  fscanf(f, "%d %d\n", &a, &b);
  printf("%d\n", 1);
}

break;
}
#endif
#ifdef COMPILE_217
case 157: {
  if (!__is_neg("2-157-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
                (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
    printf("%d\n", 1);
}

break;
}
#endif
#ifdef COMPILE_218
case 158: {
  if (__is_neg("2-158-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
               (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
    return 0;
  printf("%d\n", 1);
}

break;
}
#endif
#ifdef COMPILE_219
case 159: {
  if (__is_neg("2-159-0", 5, (char **){"a", "b", "f", "argv", "argc"}, 3,
               (int *){a, b, argc}, 0, 0, 2, (void **){f, argv}, 0, 0))
    return 1;
  printf("%d\n", 1);
}

break;
}
#endif
#ifdef COMPILE_220
case 160: {
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
#ifdef COMPILE_221
case 161: {
  printf("%d\n", 0);
  printf("%d\n", 1);
}

break;
}
#endif
#ifdef COMPILE_222
case 162: {
  printf("%d\n", 1);
  printf("%d\n", 1);
}

break;
}
#endif
#ifdef COMPILE_223
case 163: {
  scanf("%d\n", 1);
}

break;
}
#endif
}

return 0;
}
