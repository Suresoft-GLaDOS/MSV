#define __COMPILE_0
#define __COMPILE_1
#define __COMPILE_2
#define __COMPILE_3
#define __COMPILE_4
#define __COMPILE_5
#define __COMPILE_6
#define __COMPILE_7
#define __COMPILE_8
#define __COMPILE_9
#define __COMPILE_10
#define __COMPILE_11
#define __COMPILE_12
#define __COMPILE_13
#define __COMPILE_14
#define __COMPILE_15
#define __COMPILE_16
#define __COMPILE_17
#define __COMPILE_18
#define __COMPILE_19
#define __COMPILE_20
#define __COMPILE_21
#define __COMPILE_22
#define __COMPILE_23
#define __COMPILE_24
#define __COMPILE_25
#define __COMPILE_26
#define __COMPILE_27
#define __COMPILE_28
#define __COMPILE_29
#define __COMPILE_30
#define __COMPILE_31
#define __COMPILE_32
#define __COMPILE_33
#define __COMPILE_34
#define __COMPILE_35
#define __COMPILE_36
#define __COMPILE_37
#define __COMPILE_38
#define __COMPILE_39
#define __COMPILE_40
#define __COMPILE_41
#define __COMPILE_42
#define __COMPILE_43
#define __COMPILE_44
#define __COMPILE_45
#define __COMPILE_46
#define __COMPILE_47
#define __COMPILE_48
#define __COMPILE_49
#define __COMPILE_50
#define __COMPILE_51
#define __COMPILE_52
#define __COMPILE_53
#define __COMPILE_54
#define __COMPILE_55
#define __COMPILE_56
#define __COMPILE_57
#define __COMPILE_58
#define __COMPILE_59
#define __COMPILE_60
#define __COMPILE_61
#define __COMPILE_62
#define __COMPILE_63
#define __COMPILE_64
#define __COMPILE_65
#define __COMPILE_66
#define __COMPILE_67
#define __COMPILE_68
#define __COMPILE_69
#define __COMPILE_70
#define __COMPILE_71
#define __COMPILE_72
#define __COMPILE_73
#define __COMPILE_74
#define __COMPILE_75
#define __COMPILE_76
#define __COMPILE_77
#define __COMPILE_78
#define __COMPILE_79
#define __COMPILE_80
#define __COMPILE_81
#define __COMPILE_82
#define __COMPILE_83
#define __COMPILE_84
#define __COMPILE_85
#define __COMPILE_86
#define __COMPILE_87
#define __COMPILE_88
#define __COMPILE_89
#define __COMPILE_90
#define __COMPILE_91
#define __COMPILE_92
#define __COMPILE_93
#define __COMPILE_94
#define __COMPILE_95
#define __COMPILE_96
#define __COMPILE_97
#define __COMPILE_98
#define __PROFILE_0
#define __PROFILE_1
#define __PROFILE_2
#define __PROFILE_3
#define __PROFILE_4
#define __PROFILE_5
#define __PROFILE_6
#define __PROFILE_7
#define __PROFILE_8
#define __PROFILE_9
#define __PROFILE_10
#define __PROFILE_11
#define __PROFILE_12
#define __PROFILE_13
#define __PROFILE_14
#define __PROFILE_15
#define __PROFILE_16
#define __PROFILE_17
#define __PROFILE_18
#define __PROFILE_19
#define __PROFILE_20
#define __PROFILE_21
#define __PROFILE_22
#define __PROFILE_23
#define __PROFILE_24
#define __PROFILE_25
#define __PROFILE_26
#define __PROFILE_27
#define __PROFILE_28
#define __PROFILE_29
#define __PROFILE_30
#define __PROFILE_31
#define __PROFILE_32
#define __PROFILE_33
#define __PROFILE_34
#define __PROFILE_35
#define __PROFILE_36
#define __PROFILE_37
#define __PROFILE_38
#define __PROFILE_39
#define __PROFILE_40
#define __PROFILE_41
#define __PROFILE_42
#define __PROFILE_43
#define __PROFILE_44
#define __PROFILE_45
#define __PROFILE_46
#define __PROFILE_47
#define __PROFILE_48
#define __PROFILE_49
#define __PROFILE_50
#define __PROFILE_51
#define __PROFILE_52
#define __PROFILE_53
// compile_fin
int __get_mutant(); int __is_neg(const char *location,int count, ...); int __abst_hole(); void *__profile_init(const char*);void __write_profile(void *,const char *,void *,int); __profile_close(void *);long long __mutate(const long long,const char *,const char *); int __choose(const char *);void* memset(void*, int, unsigned long); 
#include <stdio.h>

void func(int a, int b){
  //prophet generated patch
  if (__choose("__MUTATE_0"))
  	a = __mutate(a, "__MUTATE_OPER_0", "__MUTATE_CONST_0");
  if (__choose("__MUTATE_1"))
  	b = __mutate(b, "__MUTATE_OPER_1", "__MUTATE_CONST_1");
  {
  int __choose0 = __choose("__SWITCH0");
  if (__choose0 == 0)
  {}
  #ifdef __COMPILE_0
  else if (__choose0 == 1)
  {
  //AddAndReplaceKind
  if (a == 1 && b == 1) {
      printf("%d\n", 0);
  } else {
      printf("%d\n", 1);
  }
  }
  #endif
  #ifdef __COMPILE_1
  else if (__choose0 == 2)
  {
  //AddAndReplaceKind
  printf("%d\n", 0);
  }
  #endif
  #ifdef __COMPILE_2
  else if (__choose0 == 3)
  {
  //AddAndReplaceKind
  printf("%d\n", 1);
  }
  #endif
  #ifdef __COMPILE_3
  else if (__choose0 == 4)
  {
  //IfExitKind
  if (__is_neg("0-4", 2, &(a), sizeof (a), &(b), sizeof (b)))
      {
void *__file0=__profile_init("func");

#ifdef __PROFILE_0
__write_profile(__file0, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_1
__write_profile(__file0, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_2
__write_profile(__file0, "__choose0", &(__choose0), sizeof (__choose0));

#endif
__profile_close(__file0);
return;
}

  }
  #endif
  int __choose1 = __choose("__SWITCH1");
  {
  int __temp1=(a == 1 && b == 1) ;
  if (__choose1 == 0)
  {}
  #ifdef __COMPILE_4
  else if (__choose1 == 1)
  {
  __temp1= (((a == 1) || __is_neg("1-1", 2, &(a), sizeof (a), &(b), sizeof (b))) && b == 1);
  }
  #endif
  #ifdef __COMPILE_5
  else if (__choose1 == 2)
  {
  __temp1= ((a == 1 && b == 1) || __is_neg("1-2", 2, &(a), sizeof (a), &(b), sizeof (b)));
  }
  #endif
  #ifdef __COMPILE_6
  else if (__choose1 == 3)
  {
  __temp1= ((a == 1 && b == 1) && !__is_neg("1-3", 2, &(a), sizeof (a), &(b), sizeof (b)));
  }
  #endif
  int __choose4 = __choose("__SWITCH4");
  if (__choose4 == 0)
  {
  if (__temp1) { // (a == 1 && b == 1) || b == 2
      printf("%d\n", 0);
    } else {
      {
  int __choose2 = __choose("__SWITCH2");
  if (__choose2 == 0)
  {}
  #ifdef __COMPILE_7
  else if (__choose2 == 1)
  {
  //AddAndReplaceKind
  if (a == 1 && b == 1) {
      printf("%d\n", 0);
  } else {
      printf("%d\n", 1);
  }
  }
  #endif
  #ifdef __COMPILE_8
  else if (__choose2 == 2)
  {
  //AddAndReplaceKind
  printf("%d\n", 0);
  }
  #endif
  #ifdef __COMPILE_9
  else if (__choose2 == 3)
  {
  //AddAndReplaceKind
  printf("%d\n", 1);
  }
  #endif
  #ifdef __COMPILE_10
  else if (__choose2 == 4)
  {
  //IfExitKind
  if (__is_neg("2-4", 2, &(a), sizeof (a), &(b), sizeof (b)))
      {
void *__file1=__profile_init("func");

#ifdef __PROFILE_3
__write_profile(__file1, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_4
__write_profile(__file1, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_5
__write_profile(__file1, "__choose0", &(__choose0), sizeof (__choose0));

#endif

#ifdef __PROFILE_6
__write_profile(__file1, "__choose1", &(__choose1), sizeof (__choose1));

#endif

#ifdef __PROFILE_7
__write_profile(__file1, "__temp1", &(__temp1), sizeof (__temp1));

#endif

#ifdef __PROFILE_8
__write_profile(__file1, "__choose4", &(__choose4), sizeof (__choose4));

#endif

#ifdef __PROFILE_9
__write_profile(__file1, "__choose2", &(__choose2), sizeof (__choose2));

#endif
__profile_close(__file1);
return;
}

  }
  #endif
  int __choose3 = __choose("__SWITCH3");
  if (__choose3 == 0)
  {
  printf("%d\n", 1);
  
  }
  #ifdef __COMPILE_11
  else if (__choose3 == 1)
  {
  //AddAndReplaceKind
  __overflow("%d\n", 1);
  
  }
  #endif
  #ifdef __COMPILE_12
  else if (__choose3 == 2)
  {
  //AddAndReplaceKind
  printf("%d\n", 1);
  
  }
  #endif
  #ifdef __COMPILE_13
  else if (__choose3 == 3)
  {
  //AddAndReplaceKind
  scanf("%d\n", 1);
  
  }
  #endif
  #ifdef __COMPILE_14
  else if (__choose3 == 4)
  {
  //GuardKind
  if (!__is_neg("3-4", 2, &(a), sizeof (a), &(b), sizeof (b)))
      printf("%d\n", 1);
  
  }
  #endif
  }
    }
  
  }
  #ifdef __COMPILE_15
  else if (__choose4 == 1)
  {
  //GuardKind
  if (!__is_neg("4-1", 2, &(a), sizeof (a), &(b), sizeof (b)))
      if (a == 1 && b == 1) {
          printf("%d\n", 0);
      } else {
          printf("%d\n", 1);
      }
  
  }
  #endif
  #ifdef __COMPILE_16
  else if (__choose4 == 2)
  {
  //SpecialGuardKind
  if (!__is_neg("4-2", 2, &(a), sizeof (a), &(b), sizeof (b)) && (a == 1 && b == 1)) {
      printf("%d\n", 0);
  } else {
      printf("%d\n", 1);
  }
  
  }
  #endif
  }
  }void *__file2=__profile_init("func");

#ifdef __PROFILE_10
__write_profile(__file2, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_11
__write_profile(__file2, "b", &(b), sizeof (b));

#endif
__profile_close(__file2);

}

int main(int argc, char *argv[]) {
  //prophet generated patch
  if (__choose("__MUTATE_2"))
  	argc = __mutate(argc, "__MUTATE_OPER_2", "__MUTATE_CONST_2");
  
  FILE *f = fopen(argv[1], "r");
  
  //prophet generated patch
  
  int a, b;
  
  //prophet generated patch
  {
  int __choose5 = __choose("__SWITCH5");
  if (__choose5 == 0)
  {}
  #ifdef __COMPILE_17
  else if (__choose5 == 1)
  {
  //AddAndReplaceKind
  fclose((f));
  }
  #endif
  #ifdef __COMPILE_18
  else if (__choose5 == 2)
  {
  //AddAndReplaceKind
  fclose(f);
  }
  #endif
  #ifdef __COMPILE_19
  else if (__choose5 == 3)
  {
  //AddAndReplaceKind
  fscanf((f), "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_20
  else if (__choose5 == 4)
  {
  //AddAndReplaceKind
  fscanf(f, "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_21
  else if (__choose5 == 5)
  {
  //AddAndReplaceKind
  func(a, b);
  }
  #endif
  #ifdef __COMPILE_22
  else if (__choose5 == 6)
  {
  //AddAndReplaceKind
  printf("%d\n", 0);
  }
  #endif
  #ifdef __COMPILE_23
  else if (__choose5 == 7)
  {
  //AddAndReplaceKind
  printf("%d\n", 1);
  }
  #endif
  #ifdef __COMPILE_24
  else if (__choose5 == 8)
  {
  //IfExitKind
  if (__is_neg("5-8", 5, &(f), sizeof (f), &(a), sizeof (a),
  			 &(b), sizeof (b), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      {
void *__file3=__profile_init("main");

#ifdef __PROFILE_12
__write_profile(__file3, "argc", &(argc), sizeof (argc));

#endif

#ifdef __PROFILE_13
__write_profile(__file3, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_14
__write_profile(__file3, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_15
__write_profile(__file3, "__choose5", &(__choose5), sizeof (__choose5));

#endif
__profile_close(__file3);
return 0;
}

  }
  #endif
  #ifdef __COMPILE_25
  else if (__choose5 == 9)
  {
  //IfExitKind
  if (__is_neg("5-9", 5, &(f), sizeof (f), &(a), sizeof (a),
  			 &(b), sizeof (b), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      {
void *__file4=__profile_init("main");

#ifdef __PROFILE_16
__write_profile(__file4, "argc", &(argc), sizeof (argc));

#endif

#ifdef __PROFILE_17
__write_profile(__file4, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_18
__write_profile(__file4, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_19
__write_profile(__file4, "__choose5", &(__choose5), sizeof (__choose5));

#endif
__profile_close(__file4);
return 1;
}

  }
  #endif
  int __choose6 = __choose("__SWITCH6");
  if (__choose6 == 0)
  {
  fscanf(f, "%d %d\n", &a, &b);
  
  }
  #ifdef __COMPILE_26
  else if (__choose6 == 1)
  {
  //AddAndReplaceKind
  _IO_vfscanf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_27
  else if (__choose6 == 2)
  {
  //AddAndReplaceKind
  fprintf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_28
  else if (__choose6 == 3)
  {
  //AddAndReplaceKind
  fscanf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_29
  else if (__choose6 == 4)
  {
  //AddAndReplaceKind
  printf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_30
  else if (__choose6 == 5)
  {
  //AddAndReplaceKind
  scanf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_31
  else if (__choose6 == 6)
  {
  //AddAndReplaceKind
  sprintf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_32
  else if (__choose6 == 7)
  {
  //AddAndReplaceKind
  sscanf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_33
  else if (__choose6 == 8)
  {
  //GuardKind
  if (!__is_neg("6-8", 5, &(f), sizeof (f), &(a), sizeof (a), &(b), sizeof (b), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      fscanf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_34
  else if (__choose6 == 9)
  {
  //ReplaceKind
  fscanf((f), "%d %d\n", &a, &b);
  
  }
  #endif
  }
  //prophet generated patch
  {
  int __choose7 = __choose("__SWITCH7");
  if (__choose7 == 0)
  {}
  #ifdef __COMPILE_35
  else if (__choose7 == 1)
  {
  //AddAndReplaceKind
  fclose((f));
  }
  #endif
  #ifdef __COMPILE_36
  else if (__choose7 == 2)
  {
  //AddAndReplaceKind
  fclose(f);
  }
  #endif
  #ifdef __COMPILE_37
  else if (__choose7 == 3)
  {
  //AddAndReplaceKind
  fscanf((f), "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_38
  else if (__choose7 == 4)
  {
  //AddAndReplaceKind
  fscanf(f, "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_39
  else if (__choose7 == 5)
  {
  //AddAndReplaceKind
  func(a, b);
  }
  #endif
  #ifdef __COMPILE_40
  else if (__choose7 == 6)
  {
  //AddAndReplaceKind
  printf("%d\n", 0);
  }
  #endif
  #ifdef __COMPILE_41
  else if (__choose7 == 7)
  {
  //AddAndReplaceKind
  printf("%d\n", 1);
  }
  #endif
  #ifdef __COMPILE_42
  else if (__choose7 == 8)
  {
  //IfExitKind
  if (__is_neg("7-8", 5, &(f), sizeof (f), &(a), sizeof (a),
  			 &(b), sizeof (b), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      {
void *__file5=__profile_init("main");

#ifdef __PROFILE_20
__write_profile(__file5, "argc", &(argc), sizeof (argc));

#endif

#ifdef __PROFILE_21
__write_profile(__file5, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_22
__write_profile(__file5, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_23
__write_profile(__file5, "__choose7", &(__choose7), sizeof (__choose7));

#endif
__profile_close(__file5);
return 0;
}

  }
  #endif
  #ifdef __COMPILE_43
  else if (__choose7 == 9)
  {
  //IfExitKind
  if (__is_neg("7-9", 5, &(f), sizeof (f), &(a), sizeof (a),
  			 &(b), sizeof (b), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      {
void *__file6=__profile_init("main");

#ifdef __PROFILE_24
__write_profile(__file6, "argc", &(argc), sizeof (argc));

#endif

#ifdef __PROFILE_25
__write_profile(__file6, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_26
__write_profile(__file6, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_27
__write_profile(__file6, "__choose7", &(__choose7), sizeof (__choose7));

#endif
__profile_close(__file6);
return 1;
}

  }
  #endif
  int __choose8 = __choose("__SWITCH8");
  if (__choose8 == 0)
  {
  fclose(f);
  
  }
  #ifdef __COMPILE_44
  else if (__choose8 == 1)
  {
  //AddAndReplaceKind
  _IO_feof(f);
  
  }
  #endif
  #ifdef __COMPILE_45
  else if (__choose8 == 2)
  {
  //AddAndReplaceKind
  _IO_ferror(f);
  
  }
  #endif
  #ifdef __COMPILE_46
  else if (__choose8 == 3)
  {
  //AddAndReplaceKind
  _IO_flockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_47
  else if (__choose8 == 4)
  {
  //AddAndReplaceKind
  _IO_free_backup_area(f);
  
  }
  #endif
  #ifdef __COMPILE_48
  else if (__choose8 == 5)
  {
  //AddAndReplaceKind
  _IO_ftrylockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_49
  else if (__choose8 == 6)
  {
  //AddAndReplaceKind
  _IO_funlockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_50
  else if (__choose8 == 7)
  {
  //AddAndReplaceKind
  _IO_getc(f);
  
  }
  #endif
  #ifdef __COMPILE_51
  else if (__choose8 == 8)
  {
  //AddAndReplaceKind
  _IO_peekc_locked(f);
  
  }
  #endif
  #ifdef __COMPILE_52
  else if (__choose8 == 9)
  {
  //AddAndReplaceKind
  __uflow(f);
  
  }
  #endif
  #ifdef __COMPILE_53
  else if (__choose8 == 10)
  {
  //AddAndReplaceKind
  __underflow(f);
  
  }
  #endif
  #ifdef __COMPILE_54
  else if (__choose8 == 11)
  {
  //AddAndReplaceKind
  clearerr(f);
  
  }
  #endif
  #ifdef __COMPILE_55
  else if (__choose8 == 12)
  {
  //AddAndReplaceKind
  clearerr_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_56
  else if (__choose8 == 13)
  {
  //AddAndReplaceKind
  feof(f);
  
  }
  #endif
  #ifdef __COMPILE_57
  else if (__choose8 == 14)
  {
  //AddAndReplaceKind
  feof_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_58
  else if (__choose8 == 15)
  {
  //AddAndReplaceKind
  ferror(f);
  
  }
  #endif
  #ifdef __COMPILE_59
  else if (__choose8 == 16)
  {
  //AddAndReplaceKind
  ferror_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_60
  else if (__choose8 == 17)
  {
  //AddAndReplaceKind
  fflush(f);
  
  }
  #endif
  #ifdef __COMPILE_61
  else if (__choose8 == 18)
  {
  //AddAndReplaceKind
  fflush_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_62
  else if (__choose8 == 19)
  {
  //AddAndReplaceKind
  fgetc(f);
  
  }
  #endif
  #ifdef __COMPILE_63
  else if (__choose8 == 20)
  {
  //AddAndReplaceKind
  fgetc_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_64
  else if (__choose8 == 21)
  {
  //AddAndReplaceKind
  fileno(f);
  
  }
  #endif
  #ifdef __COMPILE_65
  else if (__choose8 == 22)
  {
  //AddAndReplaceKind
  fileno_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_66
  else if (__choose8 == 23)
  {
  //AddAndReplaceKind
  flockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_67
  else if (__choose8 == 24)
  {
  //AddAndReplaceKind
  ftell(f);
  
  }
  #endif
  #ifdef __COMPILE_68
  else if (__choose8 == 25)
  {
  //AddAndReplaceKind
  ftello(f);
  
  }
  #endif
  #ifdef __COMPILE_69
  else if (__choose8 == 26)
  {
  //AddAndReplaceKind
  ftrylockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_70
  else if (__choose8 == 27)
  {
  //AddAndReplaceKind
  funlockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_71
  else if (__choose8 == 28)
  {
  //AddAndReplaceKind
  getc(f);
  
  }
  #endif
  #ifdef __COMPILE_72
  else if (__choose8 == 29)
  {
  //AddAndReplaceKind
  getc_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_73
  else if (__choose8 == 30)
  {
  //AddAndReplaceKind
  getw(f);
  
  }
  #endif
  #ifdef __COMPILE_74
  else if (__choose8 == 31)
  {
  //AddAndReplaceKind
  pclose(f);
  
  }
  #endif
  #ifdef __COMPILE_75
  else if (__choose8 == 32)
  {
  //AddAndReplaceKind
  rewind(f);
  
  }
  #endif
  #ifdef __COMPILE_76
  else if (__choose8 == 33)
  {
  //AddAndReplaceKind
  setlinebuf(f);
  
  }
  #endif
  #ifdef __COMPILE_77
  else if (__choose8 == 34)
  {
  //GuardKind
  if (!__is_neg("8-34", 5, &(f), sizeof (f), &(a), sizeof (a), &(b), sizeof (b), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      fclose(f);
  
  }
  #endif
  #ifdef __COMPILE_78
  else if (__choose8 == 35)
  {
  //ReplaceKind
  fclose((f));
  
  }
  #endif
  }
  
  //prophet generated patch
  {
  int __choose9 = __choose("__SWITCH9");
  if (__choose9 == 0)
  {}
  #ifdef __COMPILE_79
  else if (__choose9 == 1)
  {
  //AddAndReplaceKind
  fclose((f));
  }
  #endif
  #ifdef __COMPILE_80
  else if (__choose9 == 2)
  {
  //AddAndReplaceKind
  fclose(f);
  }
  #endif
  #ifdef __COMPILE_81
  else if (__choose9 == 3)
  {
  //AddAndReplaceKind
  fscanf((f), "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_82
  else if (__choose9 == 4)
  {
  //AddAndReplaceKind
  fscanf(f, "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_83
  else if (__choose9 == 5)
  {
  //AddAndReplaceKind
  func(a, b);
  }
  #endif
  #ifdef __COMPILE_84
  else if (__choose9 == 6)
  {
  //AddAndReplaceKind
  printf("%d\n", 0);
  }
  #endif
  #ifdef __COMPILE_85
  else if (__choose9 == 7)
  {
  //AddAndReplaceKind
  printf("%d\n", 1);
  }
  #endif
  #ifdef __COMPILE_86
  else if (__choose9 == 8)
  {
  //IfExitKind
  if (__is_neg("9-8", 5, &(a), sizeof (a), &(b), sizeof (b),
  			 &(f), sizeof (f), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      {
void *__file7=__profile_init("main");

#ifdef __PROFILE_28
__write_profile(__file7, "argc", &(argc), sizeof (argc));

#endif

#ifdef __PROFILE_29
__write_profile(__file7, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_30
__write_profile(__file7, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_31
__write_profile(__file7, "__choose9", &(__choose9), sizeof (__choose9));

#endif
__profile_close(__file7);
return 0;
}

  }
  #endif
  #ifdef __COMPILE_87
  else if (__choose9 == 9)
  {
  //IfExitKind
  if (__is_neg("9-9", 5, &(a), sizeof (a), &(b), sizeof (b),
  			 &(f), sizeof (f), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      {
void *__file8=__profile_init("main");

#ifdef __PROFILE_32
__write_profile(__file8, "argc", &(argc), sizeof (argc));

#endif

#ifdef __PROFILE_33
__write_profile(__file8, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_34
__write_profile(__file8, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_35
__write_profile(__file8, "__choose9", &(__choose9), sizeof (__choose9));

#endif
__profile_close(__file8);
return 1;
}

  }
  #endif
  int __choose10 = __choose("__SWITCH10");
  if (__choose10 == 0)
  {
  func(a, b);
  
  }
  #ifdef __COMPILE_88
  else if (__choose10 == 1)
  {
  //GuardKind
  if (!__is_neg("10-1", 5, &(a), sizeof (a), &(b), sizeof (b), &(f), sizeof (f), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      func(a, b);
  
  }
  #endif
  }
  //prophet generated patch
  {
  int __choose11 = __choose("__SWITCH11");
  if (__choose11 == 0)
  {}
  #ifdef __COMPILE_89
  else if (__choose11 == 1)
  {
  //AddAndReplaceKind
  fclose((f));
  }
  #endif
  #ifdef __COMPILE_90
  else if (__choose11 == 2)
  {
  //AddAndReplaceKind
  fclose(f);
  }
  #endif
  #ifdef __COMPILE_91
  else if (__choose11 == 3)
  {
  //AddAndReplaceKind
  fscanf((f), "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_92
  else if (__choose11 == 4)
  {
  //AddAndReplaceKind
  fscanf(f, "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_93
  else if (__choose11 == 5)
  {
  //AddAndReplaceKind
  func(a, b);
  }
  #endif
  #ifdef __COMPILE_94
  else if (__choose11 == 6)
  {
  //AddAndReplaceKind
  printf("%d\n", 0);
  }
  #endif
  #ifdef __COMPILE_95
  else if (__choose11 == 7)
  {
  //AddAndReplaceKind
  printf("%d\n", 1);
  }
  #endif
  #ifdef __COMPILE_96
  else if (__choose11 == 8)
  {
  //IfExitKind
  if (__is_neg("11-8", 5, &(a), sizeof (a), &(b), sizeof (b),
  			 &(f), sizeof (f), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      {
void *__file9=__profile_init("main");

#ifdef __PROFILE_36
__write_profile(__file9, "argc", &(argc), sizeof (argc));

#endif

#ifdef __PROFILE_37
__write_profile(__file9, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_38
__write_profile(__file9, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_39
__write_profile(__file9, "__choose11", &(__choose11), sizeof (__choose11));

#endif
__profile_close(__file9);
return 0;
}

  }
  #endif
  #ifdef __COMPILE_97
  else if (__choose11 == 9)
  {
  //IfExitKind
  if (__is_neg("11-9", 5, &(a), sizeof (a), &(b), sizeof (b),
  			 &(f), sizeof (f), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      {
void *__file10=__profile_init("main");

#ifdef __PROFILE_40
__write_profile(__file10, "argc", &(argc), sizeof (argc));

#endif

#ifdef __PROFILE_41
__write_profile(__file10, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_42
__write_profile(__file10, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_43
__write_profile(__file10, "__choose11", &(__choose11), sizeof (__choose11));

#endif
__profile_close(__file10);
return 1;
}

  }
  #endif
  int __choose12 = __choose("__SWITCH12");
  if (__choose12 == 0)
  {
  {
void *__file12=__profile_init("main");

#ifdef __PROFILE_49
__write_profile(__file12, "argc", &(argc), sizeof (argc));

#endif

#ifdef __PROFILE_50
__write_profile(__file12, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_51
__write_profile(__file12, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_52
__write_profile(__file12, "__choose11", &(__choose11), sizeof (__choose11));

#endif

#ifdef __PROFILE_53
__write_profile(__file12, "__choose12", &(__choose12), sizeof (__choose12));

#endif
__profile_close(__file12);
return 0;
}

  
  }
  #ifdef __COMPILE_98
  else if (__choose12 == 1)
  {
  //GuardKind
  if (!__is_neg("12-1", 5, &(a), sizeof (a), &(b), sizeof (b), &(f), sizeof (f), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      {
void *__file11=__profile_init("main");

#ifdef __PROFILE_44
__write_profile(__file11, "argc", &(argc), sizeof (argc));

#endif

#ifdef __PROFILE_45
__write_profile(__file11, "a", &(a), sizeof (a));

#endif

#ifdef __PROFILE_46
__write_profile(__file11, "b", &(b), sizeof (b));

#endif

#ifdef __PROFILE_47
__write_profile(__file11, "__choose11", &(__choose11), sizeof (__choose11));

#endif

#ifdef __PROFILE_48
__write_profile(__file11, "__choose12", &(__choose12), sizeof (__choose12));

#endif
__profile_close(__file11);
return 0;
}

  
  }
  #endif
  }
}
