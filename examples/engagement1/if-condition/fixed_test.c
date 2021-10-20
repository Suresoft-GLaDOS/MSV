int __get_mutant(); int __is_neg(const char *location,int count, ...); int __abst_hole(); void *__profile_init(const char*);void __write_profile(void *,const char *,void *,int); __profile_close(void *);long long __mutate(const long long,const char *,const char *); int __choose(const char *);void* memset(void*, int, unsigned long); 
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
#define __COMPILE_73
#define __COMPILE_88
#define __COMPILE_97
#define __COMPILE_113
#define __COMPILE_128
#define __COMPILE_137
#define __COMPILE_140
#define __COMPILE_141
#define __COMPILE_142
#define __COMPILE_143
#define __COMPILE_144
#define __COMPILE_145
#define __COMPILE_146
#define __COMPILE_147
#define __COMPILE_148
#define __COMPILE_149
#define __COMPILE_150
#define __COMPILE_151
#define __COMPILE_152
#define __COMPILE_153
#define __COMPILE_154
#define __COMPILE_155
#define __COMPILE_156
#define __COMPILE_157
#define __COMPILE_158
#define __COMPILE_159
#define __COMPILE_160
#define __COMPILE_161
#define __COMPILE_162
#define __COMPILE_163
#define __COMPILE_164
#define __COMPILE_165
#define __COMPILE_166
#define __COMPILE_167
#define __COMPILE_168
#define __COMPILE_169
#define __COMPILE_170
#define __COMPILE_171
#define __COMPILE_172
#define __COMPILE_173
#define __COMPILE_174
#define __COMPILE_175
#define __COMPILE_176
#define __COMPILE_177
#define __COMPILE_178
#define __COMPILE_179
#define __COMPILE_180
#define __COMPILE_181
#define __COMPILE_182
#define __COMPILE_183
#define __COMPILE_184
#define __COMPILE_185
#define __COMPILE_186
#define __COMPILE_187
#define __COMPILE_188
#define __COMPILE_189
#define __COMPILE_190
#define __COMPILE_191
#define __COMPILE_192
#define __COMPILE_193
#define __COMPILE_194
#define __COMPILE_195
#define __COMPILE_196
#define __COMPILE_197
#define __COMPILE_198
#define __COMPILE_199
#define __COMPILE_200
#define __COMPILE_201
#define __COMPILE_202
#define __COMPILE_203
#define __COMPILE_204
#define __COMPILE_205
#define __COMPILE_206
#define __COMPILE_207
#define __COMPILE_208
#define __COMPILE_209
#define __COMPILE_210
#define __COMPILE_211
#define __COMPILE_212
#define __COMPILE_213
#define __COMPILE_214
#define __COMPILE_215
#define __COMPILE_216
#define __COMPILE_217
#define __COMPILE_218
#define __COMPILE_219
#define __COMPILE_220
#define __COMPILE_221
#define __COMPILE_222
#define __COMPILE_223
#define __COMPILE_224
#define __COMPILE_225
#define __COMPILE_226
#define __COMPILE_227
#define __COMPILE_228
#define __COMPILE_229
#define __COMPILE_230
#define __COMPILE_231
#define __COMPILE_232
#define __COMPILE_233
#define __COMPILE_234
#define __COMPILE_235
#define __COMPILE_236
#define __COMPILE_237
#define __COMPILE_238
#define __COMPILE_239
#define __COMPILE_240
#define __COMPILE_241
#define __COMPILE_242
#define __COMPILE_243
#define __COMPILE_244
#define __COMPILE_245
#define __COMPILE_246
#define __COMPILE_247
#define __COMPILE_248
#define __COMPILE_249
#define __COMPILE_250
#define __COMPILE_251
#define __COMPILE_252
#define __COMPILE_253
#define __COMPILE_254
#define __COMPILE_255
#define __COMPILE_256
#define __COMPILE_257
#define __COMPILE_258
#define __COMPILE_259
#define __COMPILE_260
#define __COMPILE_261
#define __COMPILE_262
#define __COMPILE_263
#define __COMPILE_264
#define __COMPILE_265
#define __COMPILE_266
#define __COMPILE_267
#define __COMPILE_268
#define __COMPILE_269
#define __COMPILE_270
#define __COMPILE_271
#define __COMPILE_272
#define __COMPILE_273
#define __COMPILE_274
#define __COMPILE_275
#define __COMPILE_276
#define __COMPILE_277
#define __COMPILE_278
#define __COMPILE_279
#define __COMPILE_280
#define __COMPILE_281
#define __COMPILE_282
#define __COMPILE_283
#define __COMPILE_284
#define __COMPILE_285
#define __COMPILE_286
#define __COMPILE_287
#define __COMPILE_288
#define __COMPILE_289
#define __COMPILE_290
#define __COMPILE_291
#define __COMPILE_292
#define __COMPILE_293
#define __COMPILE_294
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
  #ifdef __COMPILE_15
  else if (__choose3 == 5)
  {
  //ReplaceKind
  printf("%d\n", (0 * a));
  
  }
  #endif
  #ifdef __COMPILE_16
  else if (__choose3 == 6)
  {
  //ReplaceKind
  printf("%d\n", (0 * b));
  
  }
  #endif
  #ifdef __COMPILE_17
  else if (__choose3 == 7)
  {
  //ReplaceKind
  printf("%d\n", (0));
  
  }
  #endif
  #ifdef __COMPILE_18
  else if (__choose3 == 8)
  {
  //ReplaceKind
  printf("%d\n", (a != 0));
  
  }
  #endif
  #ifdef __COMPILE_19
  else if (__choose3 == 9)
  {
  //ReplaceKind
  printf("%d\n", (a != b));
  
  }
  #endif
  #ifdef __COMPILE_20
  else if (__choose3 == 10)
  {
  //ReplaceKind
  printf("%d\n", (a * b));
  
  }
  #endif
  #ifdef __COMPILE_21
  else if (__choose3 == 11)
  {
  //ReplaceKind
  printf("%d\n", (a + 0));
  
  }
  #endif
  #ifdef __COMPILE_22
  else if (__choose3 == 12)
  {
  //ReplaceKind
  printf("%d\n", (a + b));
  
  }
  #endif
  #ifdef __COMPILE_23
  else if (__choose3 == 13)
  {
  //ReplaceKind
  printf("%d\n", (a - 0));
  
  }
  #endif
  #ifdef __COMPILE_24
  else if (__choose3 == 14)
  {
  //ReplaceKind
  printf("%d\n", (a - b));
  
  }
  #endif
  #ifdef __COMPILE_25
  else if (__choose3 == 15)
  {
  //ReplaceKind
  printf("%d\n", (a == 0));
  
  }
  #endif
  #ifdef __COMPILE_26
  else if (__choose3 == 16)
  {
  //ReplaceKind
  printf("%d\n", (a == b));
  
  }
  #endif
  #ifdef __COMPILE_27
  else if (__choose3 == 17)
  {
  //ReplaceKind
  printf("%d\n", (b != 0));
  
  }
  #endif
  #ifdef __COMPILE_28
  else if (__choose3 == 18)
  {
  //ReplaceKind
  printf("%d\n", (b * a));
  
  }
  #endif
  #ifdef __COMPILE_29
  else if (__choose3 == 19)
  {
  //ReplaceKind
  printf("%d\n", (b + 0));
  
  }
  #endif
  #ifdef __COMPILE_30
  else if (__choose3 == 20)
  {
  //ReplaceKind
  printf("%d\n", (b - 0));
  
  }
  #endif
  #ifdef __COMPILE_31
  else if (__choose3 == 21)
  {
  //ReplaceKind
  printf("%d\n", (b == 0));
  
  }
  #endif
  #ifdef __COMPILE_32
  else if (__choose3 == 22)
  {
  //ReplaceKind
  printf((&a), 1);
  
  }
  #endif
  #ifdef __COMPILE_33
  else if (__choose3 == 23)
  {
  //ReplaceKind
  printf((&b), 1);
  
  }
  #endif
  #ifdef __COMPILE_34
  else if (__choose3 == 24)
  {
  //ReplaceKind
  printf((0), 1);
  
  }
  #endif
  }
    }
  
  }
  #ifdef __COMPILE_35
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
  #ifdef __COMPILE_36
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
  }
	void *__file2=__profile_init("func");

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
  #ifdef __COMPILE_37
  else if (__choose5 == 1)
  {
  //AddAndReplaceKind
  fclose((f));
  }
  #endif
  #ifdef __COMPILE_38
  else if (__choose5 == 2)
  {
  //AddAndReplaceKind
  fclose(f);
  }
  #endif
  #ifdef __COMPILE_39
  else if (__choose5 == 3)
  {
  //AddAndReplaceKind
  fscanf((f), "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_40
  else if (__choose5 == 4)
  {
  //AddAndReplaceKind
  fscanf(f, "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_41
  else if (__choose5 == 5)
  {
  //AddAndReplaceKind
  func(a, b);
  }
  #endif
  #ifdef __COMPILE_42
  else if (__choose5 == 6)
  {
  //AddAndReplaceKind
  printf("%d\n", 0);
  }
  #endif
  #ifdef __COMPILE_43
  else if (__choose5 == 7)
  {
  //AddAndReplaceKind
  printf("%d\n", 1);
  }
  #endif
  #ifdef __COMPILE_44
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
  #ifdef __COMPILE_45
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
  #ifdef __COMPILE_46
  else if (__choose6 == 1)
  {
  //AddAndReplaceKind
  _IO_vfscanf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_47
  else if (__choose6 == 2)
  {
  //AddAndReplaceKind
  fprintf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_48
  else if (__choose6 == 3)
  {
  //AddAndReplaceKind
  fscanf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_49
  else if (__choose6 == 4)
  {
  //AddAndReplaceKind
  printf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_50
  else if (__choose6 == 5)
  {
  //AddAndReplaceKind
  scanf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_51
  else if (__choose6 == 6)
  {
  //AddAndReplaceKind
  sprintf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_52
  else if (__choose6 == 7)
  {
  //AddAndReplaceKind
  sscanf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_53
  else if (__choose6 == 8)
  {
  //GuardKind
  if (!__is_neg("6-8", 5, &(f), sizeof (f), &(a), sizeof (a), &(b), sizeof (b), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      fscanf(f, "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_54
  else if (__choose6 == 9)
  {
  //ReplaceKind
  fscanf((&a), "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_55
  else if (__choose6 == 10)
  {
  //ReplaceKind
  fscanf((&argc), "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_56
  else if (__choose6 == 11)
  {
  //ReplaceKind
  fscanf((&argv), "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_57
  else if (__choose6 == 12)
  {
  //ReplaceKind
  fscanf((&b), "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_58
  else if (__choose6 == 13)
  {
  //ReplaceKind
  fscanf((&f), "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_59
  else if (__choose6 == 14)
  {
  //ReplaceKind
  fscanf((f), "%d %d\n", &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_60
  else if (__choose6 == 15)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(0 * a), &b);
  
  }
  #endif
  #ifdef __COMPILE_61
  else if (__choose6 == 16)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(0 * argc), &b);
  
  }
  #endif
  #ifdef __COMPILE_62
  else if (__choose6 == 17)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(0 * b), &b);
  
  }
  #endif
  #ifdef __COMPILE_63
  else if (__choose6 == 18)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(a != 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_64
  else if (__choose6 == 19)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(a != b), &b);
  
  }
  #endif
  #ifdef __COMPILE_65
  else if (__choose6 == 20)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(a * argc), &b);
  
  }
  #endif
  #ifdef __COMPILE_66
  else if (__choose6 == 21)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(a * b), &b);
  
  }
  #endif
  #ifdef __COMPILE_67
  else if (__choose6 == 22)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(a + 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_68
  else if (__choose6 == 23)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(a + b), &b);
  
  }
  #endif
  #ifdef __COMPILE_69
  else if (__choose6 == 24)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(a - 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_70
  else if (__choose6 == 25)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(a - b), &b);
  
  }
  #endif
  #ifdef __COMPILE_71
  else if (__choose6 == 26)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(a == 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_72
  else if (__choose6 == 27)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(a == b), &b);
  
  }
  #endif
  #ifdef __COMPILE_73
  else if (__choose6 == 28)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(a), &b);
  
  }
  #endif
  #ifdef __COMPILE_74
  else if (__choose6 == 29)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc != 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_75
  else if (__choose6 == 30)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc != a), &b);
  
  }
  #endif
  #ifdef __COMPILE_76
  else if (__choose6 == 31)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc != b), &b);
  
  }
  #endif
  #ifdef __COMPILE_77
  else if (__choose6 == 32)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc * a), &b);
  
  }
  #endif
  #ifdef __COMPILE_78
  else if (__choose6 == 33)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc * b), &b);
  
  }
  #endif
  #ifdef __COMPILE_79
  else if (__choose6 == 34)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc + 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_80
  else if (__choose6 == 35)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc + a), &b);
  
  }
  #endif
  #ifdef __COMPILE_81
  else if (__choose6 == 36)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc + b), &b);
  
  }
  #endif
  #ifdef __COMPILE_82
  else if (__choose6 == 37)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc - 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_83
  else if (__choose6 == 38)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc - a), &b);
  
  }
  #endif
  #ifdef __COMPILE_84
  else if (__choose6 == 39)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc - b), &b);
  
  }
  #endif
  #ifdef __COMPILE_85
  else if (__choose6 == 40)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc == 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_86
  else if (__choose6 == 41)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc == a), &b);
  
  }
  #endif
  #ifdef __COMPILE_87
  else if (__choose6 == 42)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc == b), &b);
  
  }
  #endif
  #ifdef __COMPILE_88
  else if (__choose6 == 43)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argc), &b);
  
  }
  #endif
  #ifdef __COMPILE_89
  else if (__choose6 == 44)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argv != 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_90
  else if (__choose6 == 45)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(argv == 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_91
  else if (__choose6 == 46)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(b != 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_92
  else if (__choose6 == 47)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(b * a), &b);
  
  }
  #endif
  #ifdef __COMPILE_93
  else if (__choose6 == 48)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(b * argc), &b);
  
  }
  #endif
  #ifdef __COMPILE_94
  else if (__choose6 == 49)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(b + 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_95
  else if (__choose6 == 50)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(b - 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_96
  else if (__choose6 == 51)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(b == 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_97
  else if (__choose6 == 52)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(b), &b);
  
  }
  #endif
  #ifdef __COMPILE_98
  else if (__choose6 == 53)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(f != 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_99
  else if (__choose6 == 54)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &(f == 0), &b);
  
  }
  #endif
  #ifdef __COMPILE_100
  else if (__choose6 == 55)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(0 * a));
  
  }
  #endif
  #ifdef __COMPILE_101
  else if (__choose6 == 56)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(0 * argc));
  
  }
  #endif
  #ifdef __COMPILE_102
  else if (__choose6 == 57)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(0 * b));
  
  }
  #endif
  #ifdef __COMPILE_103
  else if (__choose6 == 58)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(a != 0));
  
  }
  #endif
  #ifdef __COMPILE_104
  else if (__choose6 == 59)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(a != b));
  
  }
  #endif
  #ifdef __COMPILE_105
  else if (__choose6 == 60)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(a * argc));
  
  }
  #endif
  #ifdef __COMPILE_106
  else if (__choose6 == 61)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(a * b));
  
  }
  #endif
  #ifdef __COMPILE_107
  else if (__choose6 == 62)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(a + 0));
  
  }
  #endif
  #ifdef __COMPILE_108
  else if (__choose6 == 63)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(a + b));
  
  }
  #endif
  #ifdef __COMPILE_109
  else if (__choose6 == 64)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(a - 0));
  
  }
  #endif
  #ifdef __COMPILE_110
  else if (__choose6 == 65)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(a - b));
  
  }
  #endif
  #ifdef __COMPILE_111
  else if (__choose6 == 66)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(a == 0));
  
  }
  #endif
  #ifdef __COMPILE_112
  else if (__choose6 == 67)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(a == b));
  
  }
  #endif
  #ifdef __COMPILE_113
  else if (__choose6 == 68)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(a));
  
  }
  #endif
  #ifdef __COMPILE_114
  else if (__choose6 == 69)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc != 0));
  
  }
  #endif
  #ifdef __COMPILE_115
  else if (__choose6 == 70)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc != a));
  
  }
  #endif
  #ifdef __COMPILE_116
  else if (__choose6 == 71)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc != b));
  
  }
  #endif
  #ifdef __COMPILE_117
  else if (__choose6 == 72)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc * a));
  
  }
  #endif
  #ifdef __COMPILE_118
  else if (__choose6 == 73)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc * b));
  
  }
  #endif
  #ifdef __COMPILE_119
  else if (__choose6 == 74)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc + 0));
  
  }
  #endif
  #ifdef __COMPILE_120
  else if (__choose6 == 75)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc + a));
  
  }
  #endif
  #ifdef __COMPILE_121
  else if (__choose6 == 76)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc + b));
  
  }
  #endif
  #ifdef __COMPILE_122
  else if (__choose6 == 77)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc - 0));
  
  }
  #endif
  #ifdef __COMPILE_123
  else if (__choose6 == 78)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc - a));
  
  }
  #endif
  #ifdef __COMPILE_124
  else if (__choose6 == 79)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc - b));
  
  }
  #endif
  #ifdef __COMPILE_125
  else if (__choose6 == 80)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc == 0));
  
  }
  #endif
  #ifdef __COMPILE_126
  else if (__choose6 == 81)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc == a));
  
  }
  #endif
  #ifdef __COMPILE_127
  else if (__choose6 == 82)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc == b));
  
  }
  #endif
  #ifdef __COMPILE_128
  else if (__choose6 == 83)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argc));
  
  }
  #endif
  #ifdef __COMPILE_129
  else if (__choose6 == 84)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argv != 0));
  
  }
  #endif
  #ifdef __COMPILE_130
  else if (__choose6 == 85)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(argv == 0));
  
  }
  #endif
  #ifdef __COMPILE_131
  else if (__choose6 == 86)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(b != 0));
  
  }
  #endif
  #ifdef __COMPILE_132
  else if (__choose6 == 87)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(b * a));
  
  }
  #endif
  #ifdef __COMPILE_133
  else if (__choose6 == 88)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(b * argc));
  
  }
  #endif
  #ifdef __COMPILE_134
  else if (__choose6 == 89)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(b + 0));
  
  }
  #endif
  #ifdef __COMPILE_135
  else if (__choose6 == 90)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(b - 0));
  
  }
  #endif
  #ifdef __COMPILE_136
  else if (__choose6 == 91)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(b == 0));
  
  }
  #endif
  #ifdef __COMPILE_137
  else if (__choose6 == 92)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(b));
  
  }
  #endif
  #ifdef __COMPILE_138
  else if (__choose6 == 93)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(f != 0));
  
  }
  #endif
  #ifdef __COMPILE_139
  else if (__choose6 == 94)
  {
  //ReplaceKind
  fscanf(f, "%d %d\n", &a, &(f == 0));
  
  }
  #endif
  #ifdef __COMPILE_140
  else if (__choose6 == 95)
  {
  //ReplaceKind
  fscanf(f, (&a), &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_141
  else if (__choose6 == 96)
  {
  //ReplaceKind
  fscanf(f, (&argc), &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_142
  else if (__choose6 == 97)
  {
  //ReplaceKind
  fscanf(f, (&argv), &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_143
  else if (__choose6 == 98)
  {
  //ReplaceKind
  fscanf(f, (&b), &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_144
  else if (__choose6 == 99)
  {
  //ReplaceKind
  fscanf(f, (&f), &a, &b);
  
  }
  #endif
  #ifdef __COMPILE_145
  else if (__choose6 == 100)
  {
  //ReplaceKind
  fscanf(f, (0), &a, &b);
  
  }
  #endif
  }
  //prophet generated patch
  {
  int __choose7 = __choose("__SWITCH7");
  if (__choose7 == 0)
  {}
  #ifdef __COMPILE_146
  else if (__choose7 == 1)
  {
  //AddAndReplaceKind
  fclose((f));
  }
  #endif
  #ifdef __COMPILE_147
  else if (__choose7 == 2)
  {
  //AddAndReplaceKind
  fclose(f);
  }
  #endif
  #ifdef __COMPILE_148
  else if (__choose7 == 3)
  {
  //AddAndReplaceKind
  fscanf((f), "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_149
  else if (__choose7 == 4)
  {
  //AddAndReplaceKind
  fscanf(f, "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_150
  else if (__choose7 == 5)
  {
  //AddAndReplaceKind
  func(a, b);
  }
  #endif
  #ifdef __COMPILE_151
  else if (__choose7 == 6)
  {
  //AddAndReplaceKind
  printf("%d\n", 0);
  }
  #endif
  #ifdef __COMPILE_152
  else if (__choose7 == 7)
  {
  //AddAndReplaceKind
  printf("%d\n", 1);
  }
  #endif
  #ifdef __COMPILE_153
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
  #ifdef __COMPILE_154
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
  #ifdef __COMPILE_155
  else if (__choose8 == 1)
  {
  //AddAndReplaceKind
  _IO_feof(f);
  
  }
  #endif
  #ifdef __COMPILE_156
  else if (__choose8 == 2)
  {
  //AddAndReplaceKind
  _IO_ferror(f);
  
  }
  #endif
  #ifdef __COMPILE_157
  else if (__choose8 == 3)
  {
  //AddAndReplaceKind
  _IO_flockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_158
  else if (__choose8 == 4)
  {
  //AddAndReplaceKind
  _IO_free_backup_area(f);
  
  }
  #endif
  #ifdef __COMPILE_159
  else if (__choose8 == 5)
  {
  //AddAndReplaceKind
  _IO_ftrylockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_160
  else if (__choose8 == 6)
  {
  //AddAndReplaceKind
  _IO_funlockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_161
  else if (__choose8 == 7)
  {
  //AddAndReplaceKind
  _IO_getc(f);
  
  }
  #endif
  #ifdef __COMPILE_162
  else if (__choose8 == 8)
  {
  //AddAndReplaceKind
  _IO_peekc_locked(f);
  
  }
  #endif
  #ifdef __COMPILE_163
  else if (__choose8 == 9)
  {
  //AddAndReplaceKind
  __uflow(f);
  
  }
  #endif
  #ifdef __COMPILE_164
  else if (__choose8 == 10)
  {
  //AddAndReplaceKind
  __underflow(f);
  
  }
  #endif
  #ifdef __COMPILE_165
  else if (__choose8 == 11)
  {
  //AddAndReplaceKind
  clearerr(f);
  
  }
  #endif
  #ifdef __COMPILE_166
  else if (__choose8 == 12)
  {
  //AddAndReplaceKind
  clearerr_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_167
  else if (__choose8 == 13)
  {
  //AddAndReplaceKind
  feof(f);
  
  }
  #endif
  #ifdef __COMPILE_168
  else if (__choose8 == 14)
  {
  //AddAndReplaceKind
  feof_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_169
  else if (__choose8 == 15)
  {
  //AddAndReplaceKind
  ferror(f);
  
  }
  #endif
  #ifdef __COMPILE_170
  else if (__choose8 == 16)
  {
  //AddAndReplaceKind
  ferror_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_171
  else if (__choose8 == 17)
  {
  //AddAndReplaceKind
  fflush(f);
  
  }
  #endif
  #ifdef __COMPILE_172
  else if (__choose8 == 18)
  {
  //AddAndReplaceKind
  fflush_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_173
  else if (__choose8 == 19)
  {
  //AddAndReplaceKind
  fgetc(f);
  
  }
  #endif
  #ifdef __COMPILE_174
  else if (__choose8 == 20)
  {
  //AddAndReplaceKind
  fgetc_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_175
  else if (__choose8 == 21)
  {
  //AddAndReplaceKind
  fileno(f);
  
  }
  #endif
  #ifdef __COMPILE_176
  else if (__choose8 == 22)
  {
  //AddAndReplaceKind
  fileno_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_177
  else if (__choose8 == 23)
  {
  //AddAndReplaceKind
  flockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_178
  else if (__choose8 == 24)
  {
  //AddAndReplaceKind
  ftell(f);
  
  }
  #endif
  #ifdef __COMPILE_179
  else if (__choose8 == 25)
  {
  //AddAndReplaceKind
  ftello(f);
  
  }
  #endif
  #ifdef __COMPILE_180
  else if (__choose8 == 26)
  {
  //AddAndReplaceKind
  ftrylockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_181
  else if (__choose8 == 27)
  {
  //AddAndReplaceKind
  funlockfile(f);
  
  }
  #endif
  #ifdef __COMPILE_182
  else if (__choose8 == 28)
  {
  //AddAndReplaceKind
  getc(f);
  
  }
  #endif
  #ifdef __COMPILE_183
  else if (__choose8 == 29)
  {
  //AddAndReplaceKind
  getc_unlocked(f);
  
  }
  #endif
  #ifdef __COMPILE_184
  else if (__choose8 == 30)
  {
  //AddAndReplaceKind
  getw(f);
  
  }
  #endif
  #ifdef __COMPILE_185
  else if (__choose8 == 31)
  {
  //AddAndReplaceKind
  pclose(f);
  
  }
  #endif
  #ifdef __COMPILE_186
  else if (__choose8 == 32)
  {
  //AddAndReplaceKind
  rewind(f);
  
  }
  #endif
  #ifdef __COMPILE_187
  else if (__choose8 == 33)
  {
  //AddAndReplaceKind
  setlinebuf(f);
  
  }
  #endif
  #ifdef __COMPILE_188
  else if (__choose8 == 34)
  {
  //GuardKind
  if (!__is_neg("8-34", 5, &(f), sizeof (f), &(a), sizeof (a), &(b), sizeof (b), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      fclose(f);
  
  }
  #endif
  #ifdef __COMPILE_189
  else if (__choose8 == 35)
  {
  //ReplaceKind
  fclose((&a));
  
  }
  #endif
  #ifdef __COMPILE_190
  else if (__choose8 == 36)
  {
  //ReplaceKind
  fclose((&argc));
  
  }
  #endif
  #ifdef __COMPILE_191
  else if (__choose8 == 37)
  {
  //ReplaceKind
  fclose((&argv));
  
  }
  #endif
  #ifdef __COMPILE_192
  else if (__choose8 == 38)
  {
  //ReplaceKind
  fclose((&b));
  
  }
  #endif
  #ifdef __COMPILE_193
  else if (__choose8 == 39)
  {
  //ReplaceKind
  fclose((&f));
  
  }
  #endif
  #ifdef __COMPILE_194
  else if (__choose8 == 40)
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
  #ifdef __COMPILE_195
  else if (__choose9 == 1)
  {
  //AddAndReplaceKind
  fclose((f));
  }
  #endif
  #ifdef __COMPILE_196
  else if (__choose9 == 2)
  {
  //AddAndReplaceKind
  fclose(f);
  }
  #endif
  #ifdef __COMPILE_197
  else if (__choose9 == 3)
  {
  //AddAndReplaceKind
  fscanf((f), "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_198
  else if (__choose9 == 4)
  {
  //AddAndReplaceKind
  fscanf(f, "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_199
  else if (__choose9 == 5)
  {
  //AddAndReplaceKind
  func(a, b);
  }
  #endif
  #ifdef __COMPILE_200
  else if (__choose9 == 6)
  {
  //AddAndReplaceKind
  printf("%d\n", 0);
  }
  #endif
  #ifdef __COMPILE_201
  else if (__choose9 == 7)
  {
  //AddAndReplaceKind
  printf("%d\n", 1);
  }
  #endif
  #ifdef __COMPILE_202
  else if (__choose9 == 8)
  {
  //IfExitKind
  if (__is_neg("9-8", 5, &(b), sizeof (b), &(a), sizeof (a),
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
  #ifdef __COMPILE_203
  else if (__choose9 == 9)
  {
  //IfExitKind
  if (__is_neg("9-9", 5, &(b), sizeof (b), &(a), sizeof (a),
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
  #ifdef __COMPILE_204
  else if (__choose10 == 1)
  {
  //GuardKind
  if (!__is_neg("10-1", 5, &(a), sizeof (a), &(b), sizeof (b), &(f), sizeof (f), &(argv), sizeof (argv), &(argc), sizeof (argc)))
      func(a, b);
  
  }
  #endif
  #ifdef __COMPILE_205
  else if (__choose10 == 2)
  {
  //ReplaceKind
  func((0 * a), b);
  
  }
  #endif
  #ifdef __COMPILE_206
  else if (__choose10 == 3)
  {
  //ReplaceKind
  func((0 * argc), b);
  
  }
  #endif
  #ifdef __COMPILE_207
  else if (__choose10 == 4)
  {
  //ReplaceKind
  func((0 * b), b);
  
  }
  #endif
  #ifdef __COMPILE_208
  else if (__choose10 == 5)
  {
  //ReplaceKind
  func((a != 0), b);
  
  }
  #endif
  #ifdef __COMPILE_209
  else if (__choose10 == 6)
  {
  //ReplaceKind
  func((a != b), b);
  
  }
  #endif
  #ifdef __COMPILE_210
  else if (__choose10 == 7)
  {
  //ReplaceKind
  func((a * argc), b);
  
  }
  #endif
  #ifdef __COMPILE_211
  else if (__choose10 == 8)
  {
  //ReplaceKind
  func((a * b), b);
  
  }
  #endif
  #ifdef __COMPILE_212
  else if (__choose10 == 9)
  {
  //ReplaceKind
  func((a + 0), b);
  
  }
  #endif
  #ifdef __COMPILE_213
  else if (__choose10 == 10)
  {
  //ReplaceKind
  func((a + b), b);
  
  }
  #endif
  #ifdef __COMPILE_214
  else if (__choose10 == 11)
  {
  //ReplaceKind
  func((a - 0), b);
  
  }
  #endif
  #ifdef __COMPILE_215
  else if (__choose10 == 12)
  {
  //ReplaceKind
  func((a - b), b);
  
  }
  #endif
  #ifdef __COMPILE_216
  else if (__choose10 == 13)
  {
  //ReplaceKind
  func((a == 0), b);
  
  }
  #endif
  #ifdef __COMPILE_217
  else if (__choose10 == 14)
  {
  //ReplaceKind
  func((a == b), b);
  
  }
  #endif
  #ifdef __COMPILE_218
  else if (__choose10 == 15)
  {
  //ReplaceKind
  func((a), b);
  
  }
  #endif
  #ifdef __COMPILE_219
  else if (__choose10 == 16)
  {
  //ReplaceKind
  func((argc != 0), b);
  
  }
  #endif
  #ifdef __COMPILE_220
  else if (__choose10 == 17)
  {
  //ReplaceKind
  func((argc != a), b);
  
  }
  #endif
  #ifdef __COMPILE_221
  else if (__choose10 == 18)
  {
  //ReplaceKind
  func((argc != b), b);
  
  }
  #endif
  #ifdef __COMPILE_222
  else if (__choose10 == 19)
  {
  //ReplaceKind
  func((argc * a), b);
  
  }
  #endif
  #ifdef __COMPILE_223
  else if (__choose10 == 20)
  {
  //ReplaceKind
  func((argc * b), b);
  
  }
  #endif
  #ifdef __COMPILE_224
  else if (__choose10 == 21)
  {
  //ReplaceKind
  func((argc + 0), b);
  
  }
  #endif
  #ifdef __COMPILE_225
  else if (__choose10 == 22)
  {
  //ReplaceKind
  func((argc + a), b);
  
  }
  #endif
  #ifdef __COMPILE_226
  else if (__choose10 == 23)
  {
  //ReplaceKind
  func((argc + b), b);
  
  }
  #endif
  #ifdef __COMPILE_227
  else if (__choose10 == 24)
  {
  //ReplaceKind
  func((argc - 0), b);
  
  }
  #endif
  #ifdef __COMPILE_228
  else if (__choose10 == 25)
  {
  //ReplaceKind
  func((argc - a), b);
  
  }
  #endif
  #ifdef __COMPILE_229
  else if (__choose10 == 26)
  {
  //ReplaceKind
  func((argc - b), b);
  
  }
  #endif
  #ifdef __COMPILE_230
  else if (__choose10 == 27)
  {
  //ReplaceKind
  func((argc == 0), b);
  
  }
  #endif
  #ifdef __COMPILE_231
  else if (__choose10 == 28)
  {
  //ReplaceKind
  func((argc == a), b);
  
  }
  #endif
  #ifdef __COMPILE_232
  else if (__choose10 == 29)
  {
  //ReplaceKind
  func((argc == b), b);
  
  }
  #endif
  #ifdef __COMPILE_233
  else if (__choose10 == 30)
  {
  //ReplaceKind
  func((argc), b);
  
  }
  #endif
  #ifdef __COMPILE_234
  else if (__choose10 == 31)
  {
  //ReplaceKind
  func((argv != 0), b);
  
  }
  #endif
  #ifdef __COMPILE_235
  else if (__choose10 == 32)
  {
  //ReplaceKind
  func((argv == 0), b);
  
  }
  #endif
  #ifdef __COMPILE_236
  else if (__choose10 == 33)
  {
  //ReplaceKind
  func((b != 0), b);
  
  }
  #endif
  #ifdef __COMPILE_237
  else if (__choose10 == 34)
  {
  //ReplaceKind
  func((b * a), b);
  
  }
  #endif
  #ifdef __COMPILE_238
  else if (__choose10 == 35)
  {
  //ReplaceKind
  func((b * argc), b);
  
  }
  #endif
  #ifdef __COMPILE_239
  else if (__choose10 == 36)
  {
  //ReplaceKind
  func((b + 0), b);
  
  }
  #endif
  #ifdef __COMPILE_240
  else if (__choose10 == 37)
  {
  //ReplaceKind
  func((b - 0), b);
  
  }
  #endif
  #ifdef __COMPILE_241
  else if (__choose10 == 38)
  {
  //ReplaceKind
  func((b == 0), b);
  
  }
  #endif
  #ifdef __COMPILE_242
  else if (__choose10 == 39)
  {
  //ReplaceKind
  func((b), b);
  
  }
  #endif
  #ifdef __COMPILE_243
  else if (__choose10 == 40)
  {
  //ReplaceKind
  func((f != 0), b);
  
  }
  #endif
  #ifdef __COMPILE_244
  else if (__choose10 == 41)
  {
  //ReplaceKind
  func((f == 0), b);
  
  }
  #endif
  #ifdef __COMPILE_245
  else if (__choose10 == 42)
  {
  //ReplaceKind
  func(a, (0 * a));
  
  }
  #endif
  #ifdef __COMPILE_246
  else if (__choose10 == 43)
  {
  //ReplaceKind
  func(a, (0 * argc));
  
  }
  #endif
  #ifdef __COMPILE_247
  else if (__choose10 == 44)
  {
  //ReplaceKind
  func(a, (0 * b));
  
  }
  #endif
  #ifdef __COMPILE_248
  else if (__choose10 == 45)
  {
  //ReplaceKind
  func(a, (a != 0));
  
  }
  #endif
  #ifdef __COMPILE_249
  else if (__choose10 == 46)
  {
  //ReplaceKind
  func(a, (a != b));
  
  }
  #endif
  #ifdef __COMPILE_250
  else if (__choose10 == 47)
  {
  //ReplaceKind
  func(a, (a * argc));
  
  }
  #endif
  #ifdef __COMPILE_251
  else if (__choose10 == 48)
  {
  //ReplaceKind
  func(a, (a * b));
  
  }
  #endif
  #ifdef __COMPILE_252
  else if (__choose10 == 49)
  {
  //ReplaceKind
  func(a, (a + 0));
  
  }
  #endif
  #ifdef __COMPILE_253
  else if (__choose10 == 50)
  {
  //ReplaceKind
  func(a, (a + b));
  
  }
  #endif
  #ifdef __COMPILE_254
  else if (__choose10 == 51)
  {
  //ReplaceKind
  func(a, (a - 0));
  
  }
  #endif
  #ifdef __COMPILE_255
  else if (__choose10 == 52)
  {
  //ReplaceKind
  func(a, (a - b));
  
  }
  #endif
  #ifdef __COMPILE_256
  else if (__choose10 == 53)
  {
  //ReplaceKind
  func(a, (a == 0));
  
  }
  #endif
  #ifdef __COMPILE_257
  else if (__choose10 == 54)
  {
  //ReplaceKind
  func(a, (a == b));
  
  }
  #endif
  #ifdef __COMPILE_258
  else if (__choose10 == 55)
  {
  //ReplaceKind
  func(a, (a));
  
  }
  #endif
  #ifdef __COMPILE_259
  else if (__choose10 == 56)
  {
  //ReplaceKind
  func(a, (argc != 0));
  
  }
  #endif
  #ifdef __COMPILE_260
  else if (__choose10 == 57)
  {
  //ReplaceKind
  func(a, (argc != a));
  
  }
  #endif
  #ifdef __COMPILE_261
  else if (__choose10 == 58)
  {
  //ReplaceKind
  func(a, (argc != b));
  
  }
  #endif
  #ifdef __COMPILE_262
  else if (__choose10 == 59)
  {
  //ReplaceKind
  func(a, (argc * a));
  
  }
  #endif
  #ifdef __COMPILE_263
  else if (__choose10 == 60)
  {
  //ReplaceKind
  func(a, (argc * b));
  
  }
  #endif
  #ifdef __COMPILE_264
  else if (__choose10 == 61)
  {
  //ReplaceKind
  func(a, (argc + 0));
  
  }
  #endif
  #ifdef __COMPILE_265
  else if (__choose10 == 62)
  {
  //ReplaceKind
  func(a, (argc + a));
  
  }
  #endif
  #ifdef __COMPILE_266
  else if (__choose10 == 63)
  {
  //ReplaceKind
  func(a, (argc + b));
  
  }
  #endif
  #ifdef __COMPILE_267
  else if (__choose10 == 64)
  {
  //ReplaceKind
  func(a, (argc - 0));
  
  }
  #endif
  #ifdef __COMPILE_268
  else if (__choose10 == 65)
  {
  //ReplaceKind
  func(a, (argc - a));
  
  }
  #endif
  #ifdef __COMPILE_269
  else if (__choose10 == 66)
  {
  //ReplaceKind
  func(a, (argc - b));
  
  }
  #endif
  #ifdef __COMPILE_270
  else if (__choose10 == 67)
  {
  //ReplaceKind
  func(a, (argc == 0));
  
  }
  #endif
  #ifdef __COMPILE_271
  else if (__choose10 == 68)
  {
  //ReplaceKind
  func(a, (argc == a));
  
  }
  #endif
  #ifdef __COMPILE_272
  else if (__choose10 == 69)
  {
  //ReplaceKind
  func(a, (argc == b));
  
  }
  #endif
  #ifdef __COMPILE_273
  else if (__choose10 == 70)
  {
  //ReplaceKind
  func(a, (argc));
  
  }
  #endif
  #ifdef __COMPILE_274
  else if (__choose10 == 71)
  {
  //ReplaceKind
  func(a, (argv != 0));
  
  }
  #endif
  #ifdef __COMPILE_275
  else if (__choose10 == 72)
  {
  //ReplaceKind
  func(a, (argv == 0));
  
  }
  #endif
  #ifdef __COMPILE_276
  else if (__choose10 == 73)
  {
  //ReplaceKind
  func(a, (b != 0));
  
  }
  #endif
  #ifdef __COMPILE_277
  else if (__choose10 == 74)
  {
  //ReplaceKind
  func(a, (b * a));
  
  }
  #endif
  #ifdef __COMPILE_278
  else if (__choose10 == 75)
  {
  //ReplaceKind
  func(a, (b * argc));
  
  }
  #endif
  #ifdef __COMPILE_279
  else if (__choose10 == 76)
  {
  //ReplaceKind
  func(a, (b + 0));
  
  }
  #endif
  #ifdef __COMPILE_280
  else if (__choose10 == 77)
  {
  //ReplaceKind
  func(a, (b - 0));
  
  }
  #endif
  #ifdef __COMPILE_281
  else if (__choose10 == 78)
  {
  //ReplaceKind
  func(a, (b == 0));
  
  }
  #endif
  #ifdef __COMPILE_282
  else if (__choose10 == 79)
  {
  //ReplaceKind
  func(a, (b));
  
  }
  #endif
  #ifdef __COMPILE_283
  else if (__choose10 == 80)
  {
  //ReplaceKind
  func(a, (f != 0));
  
  }
  #endif
  #ifdef __COMPILE_284
  else if (__choose10 == 81)
  {
  //ReplaceKind
  func(a, (f == 0));
  
  }
  #endif
  }
  //prophet generated patch
  {
  int __choose11 = __choose("__SWITCH11");
  if (__choose11 == 0)
  {}
  #ifdef __COMPILE_285
  else if (__choose11 == 1)
  {
  //AddAndReplaceKind
  fclose((f));
  }
  #endif
  #ifdef __COMPILE_286
  else if (__choose11 == 2)
  {
  //AddAndReplaceKind
  fclose(f);
  }
  #endif
  #ifdef __COMPILE_287
  else if (__choose11 == 3)
  {
  //AddAndReplaceKind
  fscanf((f), "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_288
  else if (__choose11 == 4)
  {
  //AddAndReplaceKind
  fscanf(f, "%d %d\n", &a, &b);
  }
  #endif
  #ifdef __COMPILE_289
  else if (__choose11 == 5)
  {
  //AddAndReplaceKind
  func(a, b);
  }
  #endif
  #ifdef __COMPILE_290
  else if (__choose11 == 6)
  {
  //AddAndReplaceKind
  printf("%d\n", 0);
  }
  #endif
  #ifdef __COMPILE_291
  else if (__choose11 == 7)
  {
  //AddAndReplaceKind
  printf("%d\n", 1);
  }
  #endif
  #ifdef __COMPILE_292
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
  #ifdef __COMPILE_293
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
__write_profile(__file12, "__choose12", &(__choose12), sizeof (__choose12));

#endif

#ifdef __PROFILE_53
__write_profile(__file12, "__choose11", &(__choose11), sizeof (__choose11));

#endif
__profile_close(__file12);
return 0;
}

  
  }
  #ifdef __COMPILE_294
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
__write_profile(__file11, "__choose12", &(__choose12), sizeof (__choose12));

#endif

#ifdef __PROFILE_48
__write_profile(__file11, "__choose11", &(__choose11), sizeof (__choose11));

#endif
__profile_close(__file11);
return 0;
}

  
  }
  #endif
  }
}
