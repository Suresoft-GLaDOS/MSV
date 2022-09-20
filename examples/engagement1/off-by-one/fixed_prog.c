int __get_mutant(); int __is_neg(const char *location,char *lid,int count, ...); int __abst_hole(); char *__stat_write_init(const char *func_name);void __write_stat(char *str,const char *var_name,void *var_addr,int size); void __stat_file_close(const char *func_name,char *str);long long __mutate(const long long,const char *,const char *); int __trident_choice(char* lid, char* typestr,int* rvals, char** rvals_ids, int rvals_size,int** lvals, char** lvals_ids, int lvals_size);int __trident_output(char* id, char* typestr, int value);float fabs_trident(float a);int __choose(const char *);void *__var_select(unsigned int var_count,void *vars[]);void *__var_select_2(unsigned int var_count,void *vars[]);long long __const_select(unsigned int const_count, ...);void* memset(void*, int, unsigned long); 
#include <stdio.h>
#define PATH_SIZE 60

int main(int argc, char* argv[]) {
    //prophet generated patch
    
    char filename[PATH_SIZE];
    
    //prophet generated patch
    
    FILE *fin;
    
    //prophet generated patch
    {
    int __choose0 = __choose("__SWITCH0");
    if (__choose0 == 0)
    {}
    #ifdef __COMPILE_0
    else if (__choose0 == 1)
    {
    //AddStmtAndReplaceAtomKind
    fclose((fin));
    }
    #endif
    #ifdef __COMPILE_1
    else if (__choose0 == 2)
    {
    //AddStmtAndReplaceAtomKind
    fin = (fin);
    }
    #endif
    #ifdef __COMPILE_2
    else if (__choose0 == 3)
    {
    //AddStmtAndReplaceAtomKind
    fin = fopen((argv)[1], "r");
    }
    #endif
    #ifdef __COMPILE_3
    else if (__choose0 == 4)
    {
    //AddStmtKind
    fclose(fin);
    }
    #endif
    #ifdef __COMPILE_4
    else if (__choose0 == 5)
    {
    //AddStmtKind
    fin = fopen(argv[1], "r");
    }
    #endif
    #ifdef __COMPILE_5
    else if (__choose0 == 6)
    {
    //IfExitKind
    if (__is_neg("0-6", "L8", 3, &(argv), sizeof (argv), &(fin),
    			 sizeof (fin), &(argc), sizeof (argc)))
        return -1;
    }
    #endif
    #ifdef __COMPILE_6
    else if (__choose0 == 7)
    {
    //IfExitKind
    if (__is_neg("0-7", "L8", 3, &(argv), sizeof (argv), &(fin),
    			 sizeof (fin), &(argc), sizeof (argc)))
        return 0;
    }
    #endif
    #ifdef __COMPILE_7
    else if (__choose0 == 8)
    {
    //IfExitKind
    if (__is_neg("0-8", "L8", 3, &(argv), sizeof (argv), &(fin),
    			 sizeof (fin), &(argc), sizeof (argc)))
        return 1;
    }
    #endif
    #ifdef __COMPILE_8
    else if (__choose0 == 9)
    {
    //IfExitKind
    if (__is_neg("0-9", "L8", 3, &(argv), sizeof (argv), &(fin),
    			 sizeof (fin), &(argc), sizeof (argc)))
        return 60;
    }
    #endif
    int __choose1 = __choose("__SWITCH1");
    if (__choose1 == 0)
    {
    fin  = fopen(argv[1], "r");
    
    }
    #ifdef __COMPILE_9
    else if (__choose1 == 1)
    {
    //GuardKind
    if (!__is_neg("1-1", "L8", 3, &(argv), sizeof (argv), &(fin), sizeof (fin), &(argc), sizeof (argc)))
        fin = fopen(argv[1], "r");
    
    }
    #endif
    #ifdef __COMPILE_10
    else if (__choose1 == 2)
    {
    //ReplaceFunctionKind
    fin = fopen(argv[1], "r");
    
    }
    #endif
    #ifdef __COMPILE_11
    else if (__choose1 == 3)
    {
    //ReplaceFunctionKind
    fin = popen(argv[1], "r");
    
    }
    #endif
    #ifdef __COMPILE_12
    else if (__choose1 == 4)
    {
    //ReplaceKind
    fin = (fin);
    
    }
    #endif
    #ifdef __COMPILE_13
    else if (__choose1 == 5)
    {
    //ReplaceKind
    fin = fopen((&argc)[1], "r");
    
    }
    #endif
    #ifdef __COMPILE_14
    else if (__choose1 == 6)
    {
    //ReplaceKind
    fin = fopen((&argv)[1], "r");
    
    }
    #endif
    #ifdef __COMPILE_15
    else if (__choose1 == 7)
    {
    //ReplaceKind
    fin = fopen((&filename)[1], "r");
    
    }
    #endif
    #ifdef __COMPILE_16
    else if (__choose1 == 8)
    {
    //ReplaceKind
    fin = fopen((&fin)[1], "r");
    
    }
    #endif
    #ifdef __COMPILE_17
    else if (__choose1 == 9)
    {
    //ReplaceKind
    fin = fopen((argv)[1], "r");
    
    }
    #endif
    #ifdef __COMPILE_18
    else if (__choose1 == 10)
    {
    //ReplaceKind
    fin = fopen(argv[(0 * argc)], "r");
    
    }
    #endif
    #ifdef __COMPILE_19
    else if (__choose1 == 11)
    {
    //ReplaceKind
    fin = fopen(argv[(0)], "r");
    
    }
    #endif
    #ifdef __COMPILE_20
    else if (__choose1 == 12)
    {
    //ReplaceKind
    fin = fopen(argv[(argc != 0)], "r");
    
    }
    #endif
    #ifdef __COMPILE_21
    else if (__choose1 == 13)
    {
    //ReplaceKind
    fin = fopen(argv[(argc + 0)], "r");
    
    }
    #endif
    #ifdef __COMPILE_22
    else if (__choose1 == 14)
    {
    //ReplaceKind
    fin = fopen(argv[(argc - 0)], "r");
    
    }
    #endif
    #ifdef __COMPILE_23
    else if (__choose1 == 15)
    {
    //ReplaceKind
    fin = fopen(argv[(argc == 0)], "r");
    
    }
    #endif
    #ifdef __COMPILE_24
    else if (__choose1 == 16)
    {
    //ReplaceKind
    fin = fopen(argv[(argv != 0)], "r");
    
    }
    #endif
    #ifdef __COMPILE_25
    else if (__choose1 == 17)
    {
    //ReplaceKind
    fin = fopen(argv[(argv == 0)], "r");
    
    }
    #endif
    #ifdef __COMPILE_26
    else if (__choose1 == 18)
    {
    //ReplaceKind
    fin = fopen(argv[(filename != 0)], "r");
    
    }
    #endif
    #ifdef __COMPILE_27
    else if (__choose1 == 19)
    {
    //ReplaceKind
    fin = fopen(argv[(filename == 0)], "r");
    
    }
    #endif
    #ifdef __COMPILE_28
    else if (__choose1 == 20)
    {
    //ReplaceKind
    fin = fopen(argv[(fin != 0)], "r");
    
    }
    #endif
    #ifdef __COMPILE_29
    else if (__choose1 == 21)
    {
    //ReplaceKind
    fin = fopen(argv[(fin == 0)], "r");
    
    }
    #endif
    #ifdef __COMPILE_30
    else if (__choose1 == 22)
    {
    //ReplaceKind
    fin = fopen(argv[1], (&argc));
    
    }
    #endif
    #ifdef __COMPILE_31
    else if (__choose1 == 23)
    {
    //ReplaceKind
    fin = fopen(argv[1], (&argv));
    
    }
    #endif
    #ifdef __COMPILE_32
    else if (__choose1 == 24)
    {
    //ReplaceKind
    fin = fopen(argv[1], (&filename));
    
    }
    #endif
    #ifdef __COMPILE_33
    else if (__choose1 == 25)
    {
    //ReplaceKind
    fin = fopen(argv[1], (&fin));
    
    }
    #endif
    #ifdef __COMPILE_34
    else if (__choose1 == 26)
    {
    //ReplaceKind
    fin = fopen(argv[1], (0));
    
    }
    #endif
    }
    //prophet generated patch
    
    int i;
    

    //prophet generated patch
    {
    int __choose2 = __choose("__SWITCH2");
    if (__choose2 == 0)
    {}
    #ifdef __COMPILE_35
    else if (__choose2 == 1)
    {
    //AddStmtAndReplaceAtomKind
    fclose((fin));
    }
    #endif
    #ifdef __COMPILE_36
    else if (__choose2 == 2)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (-1);
    }
    #endif
    #ifdef __COMPILE_37
    else if (__choose2 == 3)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (0);
    }
    #endif
    #ifdef __COMPILE_38
    else if (__choose2 == 4)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (1);
    }
    #endif
    #ifdef __COMPILE_39
    else if (__choose2 == 5)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (60);
    }
    #endif
    #ifdef __COMPILE_40
    else if (__choose2 == 6)
    {
    //AddStmtAndReplaceAtomKind
    fin = (fin);
    }
    #endif
    #ifdef __COMPILE_41
    else if (__choose2 == 7)
    {
    //AddStmtAndReplaceAtomKind
    fin = fopen((argv)[1], "r");
    }
    #endif
    #ifdef __COMPILE_42
    else if (__choose2 == 8)
    {
    //AddStmtKind
    fclose(fin);
    }
    #endif
    #ifdef __COMPILE_43
    else if (__choose2 == 9)
    {
    //AddStmtKind
    filename[i] = '\x00';
    }
    #endif
    #ifdef __COMPILE_44
    else if (__choose2 == 10)
    {
    //AddStmtKind
    fin = fopen(argv[1], "r");
    }
    #endif
    #ifdef __COMPILE_45
    else if (__choose2 == 11)
    {
    //AddStmtKind
    printf("%d\n", i);
    }
    #endif
    #ifdef __COMPILE_46
    else if (__choose2 == 12)
    {
    //IfExitKind
    if (__is_neg("2-12", "L11", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return -1;
    }
    #endif
    #ifdef __COMPILE_47
    else if (__choose2 == 13)
    {
    //IfExitKind
    if (__is_neg("2-13", "L11", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 0;
    }
    #endif
    #ifdef __COMPILE_48
    else if (__choose2 == 14)
    {
    //IfExitKind
    if (__is_neg("2-14", "L11", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 1;
    }
    #endif
    #ifdef __COMPILE_49
    else if (__choose2 == 15)
    {
    //IfExitKind
    if (__is_neg("2-15", "L11", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 60;
    }
    #endif
    int __choose10 = __choose("__SWITCH10");
    if (__choose10 == 0)
    {
    for (i = 0; i <= PATH_SIZE; i++) {
            
    char c;
    
    	{
    int __choose3 = __choose("__SWITCH3");
    if (__choose3 == 0)
    {}
    #ifdef __COMPILE_50
    else if (__choose3 == 1)
    {
    //AddStmtAndReplaceAtomKind
    c = (-1);
    }
    #endif
    #ifdef __COMPILE_51
    else if (__choose3 == 2)
    {
    //AddStmtAndReplaceAtomKind
    c = (0);
    }
    #endif
    #ifdef __COMPILE_52
    else if (__choose3 == 3)
    {
    //AddStmtAndReplaceAtomKind
    c = (1);
    }
    #endif
    #ifdef __COMPILE_53
    else if (__choose3 == 4)
    {
    //AddStmtAndReplaceAtomKind
    c = (60);
    }
    #endif
    #ifdef __COMPILE_54
    else if (__choose3 == 5)
    {
    //AddStmtAndReplaceAtomKind
    c = (c);
    }
    #endif
    #ifdef __COMPILE_55
    else if (__choose3 == 6)
    {
    //AddStmtAndReplaceAtomKind
    c = _IO_getc((fin));
    }
    #endif
    #ifdef __COMPILE_56
    else if (__choose3 == 7)
    {
    //AddStmtAndReplaceAtomKind
    fclose((fin));
    }
    #endif
    #ifdef __COMPILE_57
    else if (__choose3 == 8)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (-1);
    }
    #endif
    #ifdef __COMPILE_58
    else if (__choose3 == 9)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (0);
    }
    #endif
    #ifdef __COMPILE_59
    else if (__choose3 == 10)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (1);
    }
    #endif
    #ifdef __COMPILE_60
    else if (__choose3 == 11)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (60);
    }
    #endif
    #ifdef __COMPILE_61
    else if (__choose3 == 12)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (c);
    }
    #endif
    #ifdef __COMPILE_62
    else if (__choose3 == 13)
    {
    //AddStmtAndReplaceAtomKind
    fin = (fin);
    }
    #endif
    #ifdef __COMPILE_63
    else if (__choose3 == 14)
    {
    //AddStmtAndReplaceAtomKind
    fin = fopen((argv)[1], "r");
    }
    #endif
    #ifdef __COMPILE_64
    else if (__choose3 == 15)
    {
    //AddStmtKind
    c = _IO_getc(fin);
    }
    #endif
    #ifdef __COMPILE_65
    else if (__choose3 == 16)
    {
    //AddStmtKind
    fclose(fin);
    }
    #endif
    #ifdef __COMPILE_66
    else if (__choose3 == 17)
    {
    //AddStmtKind
    filename[i] = '\x00';
    }
    #endif
    #ifdef __COMPILE_67
    else if (__choose3 == 18)
    {
    //AddStmtKind
    filename[i] = c;
    }
    #endif
    #ifdef __COMPILE_68
    else if (__choose3 == 19)
    {
    //AddStmtKind
    fin = fopen(argv[1], "r");
    }
    #endif
    #ifdef __COMPILE_69
    else if (__choose3 == 20)
    {
    //AddStmtKind
    printf("%d\n", i);
    }
    #endif
    #ifdef __COMPILE_70
    else if (__choose3 == 21)
    {
    //IfExitKind
    if (__is_neg("3-21", "L13", 5, &(fin), sizeof (fin), &(c),
    			 sizeof (c), &(i), sizeof (i), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        break;
    }
    #endif
    #ifdef __COMPILE_71
    else if (__choose3 == 22)
    {
    //IfExitKind
    if (__is_neg("3-22", "L13", 5, &(fin), sizeof (fin), &(c),
    			 sizeof (c), &(i), sizeof (i), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return -1;
    }
    #endif
    #ifdef __COMPILE_72
    else if (__choose3 == 23)
    {
    //IfExitKind
    if (__is_neg("3-23", "L13", 5, &(fin), sizeof (fin), &(c),
    			 sizeof (c), &(i), sizeof (i), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return 0;
    }
    #endif
    #ifdef __COMPILE_73
    else if (__choose3 == 24)
    {
    //IfExitKind
    if (__is_neg("3-24", "L13", 5, &(fin), sizeof (fin), &(c),
    			 sizeof (c), &(i), sizeof (i), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return 1;
    }
    #endif
    #ifdef __COMPILE_74
    else if (__choose3 == 25)
    {
    //IfExitKind
    if (__is_neg("3-25", "L13", 5, &(fin), sizeof (fin), &(c),
    			 sizeof (c), &(i), sizeof (i), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return 60;
    }
    #endif
    int __choose4 = __choose("__SWITCH4");
    if (__choose4 == 0)
    {
    c  = getc(fin);
    
    }
    #ifdef __COMPILE_75
    else if (__choose4 == 1)
    {
    //GuardKind
    if (!__is_neg("4-1", "L13", 5, &(fin), sizeof (fin), &(c), sizeof (c), &(i), sizeof (i), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        c = _IO_getc(fin);
    
    }
    #endif
    #ifdef __COMPILE_76
    else if (__choose4 == 2)
    {
    //ReplaceFunctionKind
    c = _IO_feof(fin);
    
    }
    #endif
    #ifdef __COMPILE_77
    else if (__choose4 == 3)
    {
    //ReplaceFunctionKind
    c = _IO_ferror(fin);
    
    }
    #endif
    #ifdef __COMPILE_78
    else if (__choose4 == 4)
    {
    //ReplaceFunctionKind
    c = _IO_ftrylockfile(fin);
    
    }
    #endif
    #ifdef __COMPILE_79
    else if (__choose4 == 5)
    {
    //ReplaceFunctionKind
    c = _IO_peekc_locked(fin);
    
    }
    #endif
    #ifdef __COMPILE_80
    else if (__choose4 == 6)
    {
    //ReplaceFunctionKind
    c = __uflow(fin);
    
    }
    #endif
    #ifdef __COMPILE_81
    else if (__choose4 == 7)
    {
    //ReplaceFunctionKind
    c = __underflow(fin);
    
    }
    #endif
    #ifdef __COMPILE_82
    else if (__choose4 == 8)
    {
    //ReplaceFunctionKind
    c = fclose(fin);
    
    }
    #endif
    #ifdef __COMPILE_83
    else if (__choose4 == 9)
    {
    //ReplaceFunctionKind
    c = feof(fin);
    
    }
    #endif
    #ifdef __COMPILE_84
    else if (__choose4 == 10)
    {
    //ReplaceFunctionKind
    c = feof_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_85
    else if (__choose4 == 11)
    {
    //ReplaceFunctionKind
    c = ferror(fin);
    
    }
    #endif
    #ifdef __COMPILE_86
    else if (__choose4 == 12)
    {
    //ReplaceFunctionKind
    c = ferror_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_87
    else if (__choose4 == 13)
    {
    //ReplaceFunctionKind
    c = fflush(fin);
    
    }
    #endif
    #ifdef __COMPILE_88
    else if (__choose4 == 14)
    {
    //ReplaceFunctionKind
    c = fflush_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_89
    else if (__choose4 == 15)
    {
    //ReplaceFunctionKind
    c = fgetc(fin);
    
    }
    #endif
    #ifdef __COMPILE_90
    else if (__choose4 == 16)
    {
    //ReplaceFunctionKind
    c = fgetc_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_91
    else if (__choose4 == 17)
    {
    //ReplaceFunctionKind
    c = fileno(fin);
    
    }
    #endif
    #ifdef __COMPILE_92
    else if (__choose4 == 18)
    {
    //ReplaceFunctionKind
    c = fileno_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_93
    else if (__choose4 == 19)
    {
    //ReplaceFunctionKind
    c = ftrylockfile(fin);
    
    }
    #endif
    #ifdef __COMPILE_94
    else if (__choose4 == 20)
    {
    //ReplaceFunctionKind
    c = getc(fin);
    
    }
    #endif
    #ifdef __COMPILE_95
    else if (__choose4 == 21)
    {
    //ReplaceFunctionKind
    c = getc_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_96
    else if (__choose4 == 22)
    {
    //ReplaceFunctionKind
    c = getw(fin);
    
    }
    #endif
    #ifdef __COMPILE_97
    else if (__choose4 == 23)
    {
    //ReplaceFunctionKind
    c = pclose(fin);
    
    }
    #endif
    #ifdef __COMPILE_98
    else if (__choose4 == 24)
    {
    //ReplaceKind
    c = (-1);
    
    }
    #endif
    #ifdef __COMPILE_99
    else if (__choose4 == 25)
    {
    //ReplaceKind
    c = (0);
    
    }
    #endif
    #ifdef __COMPILE_100
    else if (__choose4 == 26)
    {
    //ReplaceKind
    c = (1);
    
    }
    #endif
    #ifdef __COMPILE_101
    else if (__choose4 == 27)
    {
    //ReplaceKind
    c = (60);
    
    }
    #endif
    #ifdef __COMPILE_102
    else if (__choose4 == 28)
    {
    //ReplaceKind
    c = (c);
    
    }
    #endif
    #ifdef __COMPILE_103
    else if (__choose4 == 29)
    {
    //ReplaceKind
    c = _IO_getc((&argc));
    
    }
    #endif
    #ifdef __COMPILE_104
    else if (__choose4 == 30)
    {
    //ReplaceKind
    c = _IO_getc((&argv));
    
    }
    #endif
    #ifdef __COMPILE_105
    else if (__choose4 == 31)
    {
    //ReplaceKind
    c = _IO_getc((&c));
    
    }
    #endif
    #ifdef __COMPILE_106
    else if (__choose4 == 32)
    {
    //ReplaceKind
    c = _IO_getc((&filename));
    
    }
    #endif
    #ifdef __COMPILE_107
    else if (__choose4 == 33)
    {
    //ReplaceKind
    c = _IO_getc((&fin));
    
    }
    #endif
    #ifdef __COMPILE_108
    else if (__choose4 == 34)
    {
    //ReplaceKind
    c = _IO_getc((&i));
    
    }
    #endif
    #ifdef __COMPILE_109
    else if (__choose4 == 35)
    {
    //ReplaceKind
    c = _IO_getc((fin));
    
    }
    #endif
    }
            {
    int __choose5 = __choose("__SWITCH5");
    if (__choose5 == 0)
    {}
    #ifdef __COMPILE_110
    else if (__choose5 == 1)
    {
    //AddStmtAndReplaceAtomKind
    c = (-1);
    }
    #endif
    #ifdef __COMPILE_111
    else if (__choose5 == 2)
    {
    //AddStmtAndReplaceAtomKind
    c = (0);
    }
    #endif
    #ifdef __COMPILE_112
    else if (__choose5 == 3)
    {
    //AddStmtAndReplaceAtomKind
    c = (1);
    }
    #endif
    #ifdef __COMPILE_113
    else if (__choose5 == 4)
    {
    //AddStmtAndReplaceAtomKind
    c = (60);
    }
    #endif
    #ifdef __COMPILE_114
    else if (__choose5 == 5)
    {
    //AddStmtAndReplaceAtomKind
    c = (c);
    }
    #endif
    #ifdef __COMPILE_115
    else if (__choose5 == 6)
    {
    //AddStmtAndReplaceAtomKind
    c = _IO_getc((fin));
    }
    #endif
    #ifdef __COMPILE_116
    else if (__choose5 == 7)
    {
    //AddStmtAndReplaceAtomKind
    fclose((fin));
    }
    #endif
    #ifdef __COMPILE_117
    else if (__choose5 == 8)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (-1);
    }
    #endif
    #ifdef __COMPILE_118
    else if (__choose5 == 9)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (0);
    }
    #endif
    #ifdef __COMPILE_119
    else if (__choose5 == 10)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (1);
    }
    #endif
    #ifdef __COMPILE_120
    else if (__choose5 == 11)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (60);
    }
    #endif
    #ifdef __COMPILE_121
    else if (__choose5 == 12)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (c);
    }
    #endif
    #ifdef __COMPILE_122
    else if (__choose5 == 13)
    {
    //AddStmtAndReplaceAtomKind
    fin = (fin);
    }
    #endif
    #ifdef __COMPILE_123
    else if (__choose5 == 14)
    {
    //AddStmtAndReplaceAtomKind
    fin = fopen((argv)[1], "r");
    }
    #endif
    #ifdef __COMPILE_124
    else if (__choose5 == 15)
    {
    //AddStmtKind
    c = _IO_getc(fin);
    }
    #endif
    #ifdef __COMPILE_125
    else if (__choose5 == 16)
    {
    //AddStmtKind
    fclose(fin);
    }
    #endif
    #ifdef __COMPILE_126
    else if (__choose5 == 17)
    {
    //AddStmtKind
    filename[i] = '\x00';
    }
    #endif
    #ifdef __COMPILE_127
    else if (__choose5 == 18)
    {
    //AddStmtKind
    filename[i] = c;
    }
    #endif
    #ifdef __COMPILE_128
    else if (__choose5 == 19)
    {
    //AddStmtKind
    fin = fopen(argv[1], "r");
    }
    #endif
    #ifdef __COMPILE_129
    else if (__choose5 == 20)
    {
    //AddStmtKind
    printf("%d\n", i);
    }
    #endif
    #ifdef __COMPILE_130
    else if (__choose5 == 21)
    {
    //IfExitKind
    if (__is_neg("5-21", "L14", 5, &(c), sizeof (c), &(fin),
    			 sizeof (fin), &(i), sizeof (i), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        break;
    }
    #endif
    #ifdef __COMPILE_131
    else if (__choose5 == 22)
    {
    //IfExitKind
    if (__is_neg("5-22", "L14", 5, &(c), sizeof (c), &(fin),
    			 sizeof (fin), &(i), sizeof (i), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return -1;
    }
    #endif
    #ifdef __COMPILE_132
    else if (__choose5 == 23)
    {
    //IfExitKind
    if (__is_neg("5-23", "L14", 5, &(c), sizeof (c), &(fin),
    			 sizeof (fin), &(i), sizeof (i), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return 0;
    }
    #endif
    #ifdef __COMPILE_133
    else if (__choose5 == 24)
    {
    //IfExitKind
    if (__is_neg("5-24", "L14", 5, &(c), sizeof (c), &(fin),
    			 sizeof (fin), &(i), sizeof (i), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return 1;
    }
    #endif
    #ifdef __COMPILE_134
    else if (__choose5 == 25)
    {
    //IfExitKind
    if (__is_neg("5-25", "L14", 5, &(c), sizeof (c), &(fin),
    			 sizeof (fin), &(i), sizeof (i), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return 60;
    }
    #endif
    int __choose6 = __choose("__SWITCH6");
    {
    int __temp6=(c == (-1)) ;
    if (__choose6 == 0)
    {}
    #ifdef __COMPILE_135
    else if (__choose6 == 1)
    {
    // LoosenConditionKind
    __temp6= ((c == (-1)) || __is_neg("6-1", "L14", 5, &(c), sizeof (c), &(fin), sizeof (fin), &(i), sizeof (i), &(argv), sizeof (argv), &(argc), sizeof (argc)));
    }
    #endif
    #ifdef __COMPILE_136
    else if (__choose6 == 2)
    {
    // MSVExtConditionKind
    __temp6= (__is_neg("6-2", "L14", 5, &(c), sizeof (c), &(fin), sizeof (fin), &(i), sizeof (i), &(argv), sizeof (argv), &(argc), sizeof (argc)));
    }
    #endif
    #ifdef __COMPILE_137
    else if (__choose6 == 3)
    {
    // TightenConditionKind
    __temp6= ((c == (-1)) && !__is_neg("6-3", "L14", 5, &(c), sizeof (c), &(fin), sizeof (fin), &(i), sizeof (i), &(argv), sizeof (argv), &(argc), sizeof (argc)));
    }
    #endif
    int __choose7 = __choose("__SWITCH7");
    if (__choose7 == 0)
    {
    if (__temp6) {
                filename[i] = '\0';
                break;
            }
    
    }
    #ifdef __COMPILE_138
    else if (__choose7 == 1)
    {
    //GuardKind
    if (!__is_neg("7-1", "L14", 5, &(c), sizeof (c), &(fin), sizeof (fin), &(i), sizeof (i), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        if (c == (-1)) {
            filename[i] = '\x00';
            break;
        }
    
    }
    #endif
    }
    }
            {
    int __choose8 = __choose("__SWITCH8");
    if (__choose8 == 0)
    {}
    #ifdef __COMPILE_139
    else if (__choose8 == 1)
    {
    //AddStmtAndReplaceAtomKind
    c = (-1);
    }
    #endif
    #ifdef __COMPILE_140
    else if (__choose8 == 2)
    {
    //AddStmtAndReplaceAtomKind
    c = (0);
    }
    #endif
    #ifdef __COMPILE_141
    else if (__choose8 == 3)
    {
    //AddStmtAndReplaceAtomKind
    c = (1);
    }
    #endif
    #ifdef __COMPILE_142
    else if (__choose8 == 4)
    {
    //AddStmtAndReplaceAtomKind
    c = (60);
    }
    #endif
    #ifdef __COMPILE_143
    else if (__choose8 == 5)
    {
    //AddStmtAndReplaceAtomKind
    c = (c);
    }
    #endif
    #ifdef __COMPILE_144
    else if (__choose8 == 6)
    {
    //AddStmtAndReplaceAtomKind
    c = _IO_getc((fin));
    }
    #endif
    #ifdef __COMPILE_145
    else if (__choose8 == 7)
    {
    //AddStmtAndReplaceAtomKind
    fclose((fin));
    }
    #endif
    #ifdef __COMPILE_146
    else if (__choose8 == 8)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (-1);
    }
    #endif
    #ifdef __COMPILE_147
    else if (__choose8 == 9)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (0);
    }
    #endif
    #ifdef __COMPILE_148
    else if (__choose8 == 10)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (1);
    }
    #endif
    #ifdef __COMPILE_149
    else if (__choose8 == 11)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (60);
    }
    #endif
    #ifdef __COMPILE_150
    else if (__choose8 == 12)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (c);
    }
    #endif
    #ifdef __COMPILE_151
    else if (__choose8 == 13)
    {
    //AddStmtAndReplaceAtomKind
    fin = (fin);
    }
    #endif
    #ifdef __COMPILE_152
    else if (__choose8 == 14)
    {
    //AddStmtAndReplaceAtomKind
    fin = fopen((argv)[1], "r");
    }
    #endif
    #ifdef __COMPILE_153
    else if (__choose8 == 15)
    {
    //AddStmtKind
    c = _IO_getc(fin);
    }
    #endif
    #ifdef __COMPILE_154
    else if (__choose8 == 16)
    {
    //AddStmtKind
    fclose(fin);
    }
    #endif
    #ifdef __COMPILE_155
    else if (__choose8 == 17)
    {
    //AddStmtKind
    filename[i] = '\x00';
    }
    #endif
    #ifdef __COMPILE_156
    else if (__choose8 == 18)
    {
    //AddStmtKind
    filename[i] = c;
    }
    #endif
    #ifdef __COMPILE_157
    else if (__choose8 == 19)
    {
    //AddStmtKind
    fin = fopen(argv[1], "r");
    }
    #endif
    #ifdef __COMPILE_158
    else if (__choose8 == 20)
    {
    //AddStmtKind
    printf("%d\n", i);
    }
    #endif
    #ifdef __COMPILE_159
    else if (__choose8 == 21)
    {
    //IfExitKind
    if (__is_neg("8-21", "L18", 5, &(i), sizeof (i), &(c),
    			 sizeof (c), &(fin), sizeof (fin), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        break;
    }
    #endif
    #ifdef __COMPILE_160
    else if (__choose8 == 22)
    {
    //IfExitKind
    if (__is_neg("8-22", "L18", 5, &(i), sizeof (i), &(c),
    			 sizeof (c), &(fin), sizeof (fin), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return -1;
    }
    #endif
    #ifdef __COMPILE_161
    else if (__choose8 == 23)
    {
    //IfExitKind
    if (__is_neg("8-23", "L18", 5, &(i), sizeof (i), &(c),
    			 sizeof (c), &(fin), sizeof (fin), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return 0;
    }
    #endif
    #ifdef __COMPILE_162
    else if (__choose8 == 24)
    {
    //IfExitKind
    if (__is_neg("8-24", "L18", 5, &(i), sizeof (i), &(c),
    			 sizeof (c), &(fin), sizeof (fin), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return 1;
    }
    #endif
    #ifdef __COMPILE_163
    else if (__choose8 == 25)
    {
    //IfExitKind
    if (__is_neg("8-25", "L18", 5, &(i), sizeof (i), &(c),
    			 sizeof (c), &(fin), sizeof (fin), &(argv), sizeof (argv), &(argc),
    			 sizeof (argc)))
        return 60;
    }
    #endif
    int __choose9 = __choose("__SWITCH9");
    if (__choose9 == 0)
    {
    filename[i] = c;
    
    }
    #ifdef __COMPILE_164
    else if (__choose9 == 1)
    {
    //GuardKind
    if (!__is_neg("9-1", "L18", 5, &(i), sizeof (i), &(c), sizeof (c), &(fin), sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        filename[i] = c;
    
    }
    #endif
    #ifdef __COMPILE_165
    else if (__choose9 == 2)
    {
    //ReplaceKind
    filename[i] = (-1);
    
    }
    #endif
    #ifdef __COMPILE_166
    else if (__choose9 == 3)
    {
    //ReplaceKind
    filename[i] = (0 * argc);
    
    }
    #endif
    #ifdef __COMPILE_167
    else if (__choose9 == 4)
    {
    //ReplaceKind
    filename[i] = (0 * c);
    
    }
    #endif
    #ifdef __COMPILE_168
    else if (__choose9 == 5)
    {
    //ReplaceKind
    filename[i] = (0 * i);
    
    }
    #endif
    #ifdef __COMPILE_169
    else if (__choose9 == 6)
    {
    //ReplaceKind
    filename[i] = (0);
    
    }
    #endif
    #ifdef __COMPILE_170
    else if (__choose9 == 7)
    {
    //ReplaceKind
    filename[i] = (1);
    
    }
    #endif
    #ifdef __COMPILE_171
    else if (__choose9 == 8)
    {
    //ReplaceKind
    filename[i] = (60);
    
    }
    #endif
    #ifdef __COMPILE_172
    else if (__choose9 == 9)
    {
    //ReplaceKind
    filename[i] = (argc != 0);
    
    }
    #endif
    #ifdef __COMPILE_173
    else if (__choose9 == 10)
    {
    //ReplaceKind
    filename[i] = (argc != c);
    
    }
    #endif
    #ifdef __COMPILE_174
    else if (__choose9 == 11)
    {
    //ReplaceKind
    filename[i] = (argc != i);
    
    }
    #endif
    #ifdef __COMPILE_175
    else if (__choose9 == 12)
    {
    //ReplaceKind
    filename[i] = (argc * c);
    
    }
    #endif
    #ifdef __COMPILE_176
    else if (__choose9 == 13)
    {
    //ReplaceKind
    filename[i] = (argc * i);
    
    }
    #endif
    #ifdef __COMPILE_177
    else if (__choose9 == 14)
    {
    //ReplaceKind
    filename[i] = (argc + 0);
    
    }
    #endif
    #ifdef __COMPILE_178
    else if (__choose9 == 15)
    {
    //ReplaceKind
    filename[i] = (argc + c);
    
    }
    #endif
    #ifdef __COMPILE_179
    else if (__choose9 == 16)
    {
    //ReplaceKind
    filename[i] = (argc + i);
    
    }
    #endif
    #ifdef __COMPILE_180
    else if (__choose9 == 17)
    {
    //ReplaceKind
    filename[i] = (argc - 0);
    
    }
    #endif
    #ifdef __COMPILE_181
    else if (__choose9 == 18)
    {
    //ReplaceKind
    filename[i] = (argc - c);
    
    }
    #endif
    #ifdef __COMPILE_182
    else if (__choose9 == 19)
    {
    //ReplaceKind
    filename[i] = (argc - i);
    
    }
    #endif
    #ifdef __COMPILE_183
    else if (__choose9 == 20)
    {
    //ReplaceKind
    filename[i] = (argc == 0);
    
    }
    #endif
    #ifdef __COMPILE_184
    else if (__choose9 == 21)
    {
    //ReplaceKind
    filename[i] = (argc == c);
    
    }
    #endif
    #ifdef __COMPILE_185
    else if (__choose9 == 22)
    {
    //ReplaceKind
    filename[i] = (argc == i);
    
    }
    #endif
    #ifdef __COMPILE_186
    else if (__choose9 == 23)
    {
    //ReplaceKind
    filename[i] = (argv != 0);
    
    }
    #endif
    #ifdef __COMPILE_187
    else if (__choose9 == 24)
    {
    //ReplaceKind
    filename[i] = (argv == 0);
    
    }
    #endif
    #ifdef __COMPILE_188
    else if (__choose9 == 25)
    {
    //ReplaceKind
    filename[i] = (c != 0);
    
    }
    #endif
    #ifdef __COMPILE_189
    else if (__choose9 == 26)
    {
    //ReplaceKind
    filename[i] = (c * argc);
    
    }
    #endif
    #ifdef __COMPILE_190
    else if (__choose9 == 27)
    {
    //ReplaceKind
    filename[i] = (c * i);
    
    }
    #endif
    #ifdef __COMPILE_191
    else if (__choose9 == 28)
    {
    //ReplaceKind
    filename[i] = (c + 0);
    
    }
    #endif
    #ifdef __COMPILE_192
    else if (__choose9 == 29)
    {
    //ReplaceKind
    filename[i] = (c - 0);
    
    }
    #endif
    #ifdef __COMPILE_193
    else if (__choose9 == 30)
    {
    //ReplaceKind
    filename[i] = (c == 0);
    
    }
    #endif
    #ifdef __COMPILE_194
    else if (__choose9 == 31)
    {
    //ReplaceKind
    filename[i] = (c);
    
    }
    #endif
    #ifdef __COMPILE_195
    else if (__choose9 == 32)
    {
    //ReplaceKind
    filename[i] = (filename != 0);
    
    }
    #endif
    #ifdef __COMPILE_196
    else if (__choose9 == 33)
    {
    //ReplaceKind
    filename[i] = (filename == 0);
    
    }
    #endif
    #ifdef __COMPILE_197
    else if (__choose9 == 34)
    {
    //ReplaceKind
    filename[i] = (fin != 0);
    
    }
    #endif
    #ifdef __COMPILE_198
    else if (__choose9 == 35)
    {
    //ReplaceKind
    filename[i] = (fin == 0);
    
    }
    #endif
    #ifdef __COMPILE_199
    else if (__choose9 == 36)
    {
    //ReplaceKind
    filename[i] = (i != 0);
    
    }
    #endif
    #ifdef __COMPILE_200
    else if (__choose9 == 37)
    {
    //ReplaceKind
    filename[i] = (i != c);
    
    }
    #endif
    #ifdef __COMPILE_201
    else if (__choose9 == 38)
    {
    //ReplaceKind
    filename[i] = (i * argc);
    
    }
    #endif
    #ifdef __COMPILE_202
    else if (__choose9 == 39)
    {
    //ReplaceKind
    filename[i] = (i * c);
    
    }
    #endif
    #ifdef __COMPILE_203
    else if (__choose9 == 40)
    {
    //ReplaceKind
    filename[i] = (i + 0);
    
    }
    #endif
    #ifdef __COMPILE_204
    else if (__choose9 == 41)
    {
    //ReplaceKind
    filename[i] = (i + c);
    
    }
    #endif
    #ifdef __COMPILE_205
    else if (__choose9 == 42)
    {
    //ReplaceKind
    filename[i] = (i - 0);
    
    }
    #endif
    #ifdef __COMPILE_206
    else if (__choose9 == 43)
    {
    //ReplaceKind
    filename[i] = (i - c);
    
    }
    #endif
    #ifdef __COMPILE_207
    else if (__choose9 == 44)
    {
    //ReplaceKind
    filename[i] = (i == 0);
    
    }
    #endif
    #ifdef __COMPILE_208
    else if (__choose9 == 45)
    {
    //ReplaceKind
    filename[i] = (i == c);
    
    }
    #endif
    }
        }
    
    }
    #ifdef __COMPILE_209
    else if (__choose10 == 1)
    {
    //GuardKind
    if (!__is_neg("10-1", "L11", 4, &(i), sizeof (i), &(fin), sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        for (i = 0; i <= 60; i++) {
            char c;
            c = _IO_getc(fin);
            if (c == (-1)) {
                filename[i] = '\x00';
                break;
            }
            filename[i] = c;
        }
    
    }
    #endif
    }
    //prophet generated patch
    {
    int __choose11 = __choose("__SWITCH11");
    if (__choose11 == 0)
    {}
    #ifdef __COMPILE_210
    else if (__choose11 == 1)
    {
    //AddStmtAndReplaceAtomKind
    fclose((fin));
    }
    #endif
    #ifdef __COMPILE_211
    else if (__choose11 == 2)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (-1);
    }
    #endif
    #ifdef __COMPILE_212
    else if (__choose11 == 3)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (0);
    }
    #endif
    #ifdef __COMPILE_213
    else if (__choose11 == 4)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (1);
    }
    #endif
    #ifdef __COMPILE_214
    else if (__choose11 == 5)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (60);
    }
    #endif
    #ifdef __COMPILE_215
    else if (__choose11 == 6)
    {
    //AddStmtAndReplaceAtomKind
    fin = (fin);
    }
    #endif
    #ifdef __COMPILE_216
    else if (__choose11 == 7)
    {
    //AddStmtAndReplaceAtomKind
    fin = fopen((argv)[1], "r");
    }
    #endif
    #ifdef __COMPILE_217
    else if (__choose11 == 8)
    {
    //AddStmtKind
    fclose(fin);
    }
    #endif
    #ifdef __COMPILE_218
    else if (__choose11 == 9)
    {
    //AddStmtKind
    filename[i] = '\x00';
    }
    #endif
    #ifdef __COMPILE_219
    else if (__choose11 == 10)
    {
    //AddStmtKind
    fin = fopen(argv[1], "r");
    }
    #endif
    #ifdef __COMPILE_220
    else if (__choose11 == 11)
    {
    //AddStmtKind
    printf("%d\n", i);
    }
    #endif
    #ifdef __COMPILE_221
    else if (__choose11 == 12)
    {
    //IfExitKind
    if (__is_neg("11-12", "L20", 4, &(fin), sizeof (fin), &(i),
    			 sizeof (i), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return -1;
    }
    #endif
    #ifdef __COMPILE_222
    else if (__choose11 == 13)
    {
    //IfExitKind
    if (__is_neg("11-13", "L20", 4, &(fin), sizeof (fin), &(i),
    			 sizeof (i), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 0;
    }
    #endif
    #ifdef __COMPILE_223
    else if (__choose11 == 14)
    {
    //IfExitKind
    if (__is_neg("11-14", "L20", 4, &(fin), sizeof (fin), &(i),
    			 sizeof (i), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 1;
    }
    #endif
    #ifdef __COMPILE_224
    else if (__choose11 == 15)
    {
    //IfExitKind
    if (__is_neg("11-15", "L20", 4, &(fin), sizeof (fin), &(i),
    			 sizeof (i), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 60;
    }
    #endif
    int __choose12 = __choose("__SWITCH12");
    if (__choose12 == 0)
    {
    fclose(fin);
    
    }
    #ifdef __COMPILE_225
    else if (__choose12 == 1)
    {
    //GuardKind
    if (!__is_neg("12-1", "L20", 4, &(fin), sizeof (fin), &(i), sizeof (i), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        fclose(fin);
    
    }
    #endif
    #ifdef __COMPILE_226
    else if (__choose12 == 2)
    {
    //ReplaceFunctionKind
    _IO_feof(fin);
    
    }
    #endif
    #ifdef __COMPILE_227
    else if (__choose12 == 3)
    {
    //ReplaceFunctionKind
    _IO_ferror(fin);
    
    }
    #endif
    #ifdef __COMPILE_228
    else if (__choose12 == 4)
    {
    //ReplaceFunctionKind
    _IO_flockfile(fin);
    
    }
    #endif
    #ifdef __COMPILE_229
    else if (__choose12 == 5)
    {
    //ReplaceFunctionKind
    _IO_free_backup_area(fin);
    
    }
    #endif
    #ifdef __COMPILE_230
    else if (__choose12 == 6)
    {
    //ReplaceFunctionKind
    _IO_ftrylockfile(fin);
    
    }
    #endif
    #ifdef __COMPILE_231
    else if (__choose12 == 7)
    {
    //ReplaceFunctionKind
    _IO_funlockfile(fin);
    
    }
    #endif
    #ifdef __COMPILE_232
    else if (__choose12 == 8)
    {
    //ReplaceFunctionKind
    _IO_getc(fin);
    
    }
    #endif
    #ifdef __COMPILE_233
    else if (__choose12 == 9)
    {
    //ReplaceFunctionKind
    _IO_peekc_locked(fin);
    
    }
    #endif
    #ifdef __COMPILE_234
    else if (__choose12 == 10)
    {
    //ReplaceFunctionKind
    __uflow(fin);
    
    }
    #endif
    #ifdef __COMPILE_235
    else if (__choose12 == 11)
    {
    //ReplaceFunctionKind
    __underflow(fin);
    
    }
    #endif
    #ifdef __COMPILE_236
    else if (__choose12 == 12)
    {
    //ReplaceFunctionKind
    clearerr(fin);
    
    }
    #endif
    #ifdef __COMPILE_237
    else if (__choose12 == 13)
    {
    //ReplaceFunctionKind
    clearerr_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_238
    else if (__choose12 == 14)
    {
    //ReplaceFunctionKind
    feof(fin);
    
    }
    #endif
    #ifdef __COMPILE_239
    else if (__choose12 == 15)
    {
    //ReplaceFunctionKind
    feof_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_240
    else if (__choose12 == 16)
    {
    //ReplaceFunctionKind
    ferror(fin);
    
    }
    #endif
    #ifdef __COMPILE_241
    else if (__choose12 == 17)
    {
    //ReplaceFunctionKind
    ferror_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_242
    else if (__choose12 == 18)
    {
    //ReplaceFunctionKind
    fflush(fin);
    
    }
    #endif
    #ifdef __COMPILE_243
    else if (__choose12 == 19)
    {
    //ReplaceFunctionKind
    fflush_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_244
    else if (__choose12 == 20)
    {
    //ReplaceFunctionKind
    fgetc(fin);
    
    }
    #endif
    #ifdef __COMPILE_245
    else if (__choose12 == 21)
    {
    //ReplaceFunctionKind
    fgetc_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_246
    else if (__choose12 == 22)
    {
    //ReplaceFunctionKind
    fileno(fin);
    
    }
    #endif
    #ifdef __COMPILE_247
    else if (__choose12 == 23)
    {
    //ReplaceFunctionKind
    fileno_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_248
    else if (__choose12 == 24)
    {
    //ReplaceFunctionKind
    flockfile(fin);
    
    }
    #endif
    #ifdef __COMPILE_249
    else if (__choose12 == 25)
    {
    //ReplaceFunctionKind
    ftell(fin);
    
    }
    #endif
    #ifdef __COMPILE_250
    else if (__choose12 == 26)
    {
    //ReplaceFunctionKind
    ftello(fin);
    
    }
    #endif
    #ifdef __COMPILE_251
    else if (__choose12 == 27)
    {
    //ReplaceFunctionKind
    ftrylockfile(fin);
    
    }
    #endif
    #ifdef __COMPILE_252
    else if (__choose12 == 28)
    {
    //ReplaceFunctionKind
    funlockfile(fin);
    
    }
    #endif
    #ifdef __COMPILE_253
    else if (__choose12 == 29)
    {
    //ReplaceFunctionKind
    getc(fin);
    
    }
    #endif
    #ifdef __COMPILE_254
    else if (__choose12 == 30)
    {
    //ReplaceFunctionKind
    getc_unlocked(fin);
    
    }
    #endif
    #ifdef __COMPILE_255
    else if (__choose12 == 31)
    {
    //ReplaceFunctionKind
    getw(fin);
    
    }
    #endif
    #ifdef __COMPILE_256
    else if (__choose12 == 32)
    {
    //ReplaceFunctionKind
    pclose(fin);
    
    }
    #endif
    #ifdef __COMPILE_257
    else if (__choose12 == 33)
    {
    //ReplaceFunctionKind
    rewind(fin);
    
    }
    #endif
    #ifdef __COMPILE_258
    else if (__choose12 == 34)
    {
    //ReplaceFunctionKind
    setlinebuf(fin);
    
    }
    #endif
    #ifdef __COMPILE_259
    else if (__choose12 == 35)
    {
    //ReplaceKind
    fclose((&argc));
    
    }
    #endif
    #ifdef __COMPILE_260
    else if (__choose12 == 36)
    {
    //ReplaceKind
    fclose((&argv));
    
    }
    #endif
    #ifdef __COMPILE_261
    else if (__choose12 == 37)
    {
    //ReplaceKind
    fclose((&filename));
    
    }
    #endif
    #ifdef __COMPILE_262
    else if (__choose12 == 38)
    {
    //ReplaceKind
    fclose((&fin));
    
    }
    #endif
    #ifdef __COMPILE_263
    else if (__choose12 == 39)
    {
    //ReplaceKind
    fclose((&i));
    
    }
    #endif
    #ifdef __COMPILE_264
    else if (__choose12 == 40)
    {
    //ReplaceKind
    fclose((fin));
    
    }
    #endif
    }
    //prophet generated patch
    {
    int __choose13 = __choose("__SWITCH13");
    if (__choose13 == 0)
    {}
    #ifdef __COMPILE_265
    else if (__choose13 == 1)
    {
    //AddStmtAndReplaceAtomKind
    fclose((fin));
    }
    #endif
    #ifdef __COMPILE_266
    else if (__choose13 == 2)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (-1);
    }
    #endif
    #ifdef __COMPILE_267
    else if (__choose13 == 3)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (0);
    }
    #endif
    #ifdef __COMPILE_268
    else if (__choose13 == 4)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (1);
    }
    #endif
    #ifdef __COMPILE_269
    else if (__choose13 == 5)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (60);
    }
    #endif
    #ifdef __COMPILE_270
    else if (__choose13 == 6)
    {
    //AddStmtAndReplaceAtomKind
    fin = (fin);
    }
    #endif
    #ifdef __COMPILE_271
    else if (__choose13 == 7)
    {
    //AddStmtAndReplaceAtomKind
    fin = fopen((argv)[1], "r");
    }
    #endif
    #ifdef __COMPILE_272
    else if (__choose13 == 8)
    {
    //AddStmtKind
    fclose(fin);
    }
    #endif
    #ifdef __COMPILE_273
    else if (__choose13 == 9)
    {
    //AddStmtKind
    filename[i] = '\x00';
    }
    #endif
    #ifdef __COMPILE_274
    else if (__choose13 == 10)
    {
    //AddStmtKind
    fin = fopen(argv[1], "r");
    }
    #endif
    #ifdef __COMPILE_275
    else if (__choose13 == 11)
    {
    //AddStmtKind
    printf("%d\n", i);
    }
    #endif
    #ifdef __COMPILE_276
    else if (__choose13 == 12)
    {
    //IfExitKind
    if (__is_neg("13-12", "L21", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return -1;
    }
    #endif
    #ifdef __COMPILE_277
    else if (__choose13 == 13)
    {
    //IfExitKind
    if (__is_neg("13-13", "L21", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 0;
    }
    #endif
    #ifdef __COMPILE_278
    else if (__choose13 == 14)
    {
    //IfExitKind
    if (__is_neg("13-14", "L21", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 1;
    }
    #endif
    #ifdef __COMPILE_279
    else if (__choose13 == 15)
    {
    //IfExitKind
    if (__is_neg("13-15", "L21", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 60;
    }
    #endif
    int __choose14 = __choose("__SWITCH14");
    if (__choose14 == 0)
    {
    printf("%d\n", i);
    
    }
    #ifdef __COMPILE_280
    else if (__choose14 == 1)
    {
    //GuardKind
    if (!__is_neg("14-1", "L21", 4, &(i), sizeof (i), &(fin), sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        printf("%d\n", i);
    
    }
    #endif
    #ifdef __COMPILE_281
    else if (__choose14 == 2)
    {
    //ReplaceFunctionKind
    __overflow("%d\n", i);
    
    }
    #endif
    #ifdef __COMPILE_282
    else if (__choose14 == 3)
    {
    //ReplaceFunctionKind
    printf("%d\n", i);
    
    }
    #endif
    #ifdef __COMPILE_283
    else if (__choose14 == 4)
    {
    //ReplaceKind
    printf("%d\n", (0 * argc));
    
    }
    #endif
    #ifdef __COMPILE_284
    else if (__choose14 == 5)
    {
    //ReplaceKind
    printf("%d\n", (0 * i));
    
    }
    #endif
    #ifdef __COMPILE_285
    else if (__choose14 == 6)
    {
    //ReplaceKind
    printf("%d\n", (argc != 0));
    
    }
    #endif
    #ifdef __COMPILE_286
    else if (__choose14 == 7)
    {
    //ReplaceKind
    printf("%d\n", (argc != i));
    
    }
    #endif
    #ifdef __COMPILE_287
    else if (__choose14 == 8)
    {
    //ReplaceKind
    printf("%d\n", (argc * i));
    
    }
    #endif
    #ifdef __COMPILE_288
    else if (__choose14 == 9)
    {
    //ReplaceKind
    printf("%d\n", (argc + 0));
    
    }
    #endif
    #ifdef __COMPILE_289
    else if (__choose14 == 10)
    {
    //ReplaceKind
    printf("%d\n", (argc + i));
    
    }
    #endif
    #ifdef __COMPILE_290
    else if (__choose14 == 11)
    {
    //ReplaceKind
    printf("%d\n", (argc - 0));
    
    }
    #endif
    #ifdef __COMPILE_291
    else if (__choose14 == 12)
    {
    //ReplaceKind
    printf("%d\n", (argc - i));
    
    }
    #endif
    #ifdef __COMPILE_292
    else if (__choose14 == 13)
    {
    //ReplaceKind
    printf("%d\n", (argc == 0));
    
    }
    #endif
    #ifdef __COMPILE_293
    else if (__choose14 == 14)
    {
    //ReplaceKind
    printf("%d\n", (argc == i));
    
    }
    #endif
    #ifdef __COMPILE_294
    else if (__choose14 == 15)
    {
    //ReplaceKind
    printf("%d\n", (argc));
    
    }
    #endif
    #ifdef __COMPILE_295
    else if (__choose14 == 16)
    {
    //ReplaceKind
    printf("%d\n", (argv != 0));
    
    }
    #endif
    #ifdef __COMPILE_296
    else if (__choose14 == 17)
    {
    //ReplaceKind
    printf("%d\n", (argv == 0));
    
    }
    #endif
    #ifdef __COMPILE_297
    else if (__choose14 == 18)
    {
    //ReplaceKind
    printf("%d\n", (filename != 0));
    
    }
    #endif
    #ifdef __COMPILE_298
    else if (__choose14 == 19)
    {
    //ReplaceKind
    printf("%d\n", (filename == 0));
    
    }
    #endif
    #ifdef __COMPILE_299
    else if (__choose14 == 20)
    {
    //ReplaceKind
    printf("%d\n", (fin != 0));
    
    }
    #endif
    #ifdef __COMPILE_300
    else if (__choose14 == 21)
    {
    //ReplaceKind
    printf("%d\n", (fin == 0));
    
    }
    #endif
    #ifdef __COMPILE_301
    else if (__choose14 == 22)
    {
    //ReplaceKind
    printf("%d\n", (i != 0));
    
    }
    #endif
    #ifdef __COMPILE_302
    else if (__choose14 == 23)
    {
    //ReplaceKind
    printf("%d\n", (i * argc));
    
    }
    #endif
    #ifdef __COMPILE_303
    else if (__choose14 == 24)
    {
    //ReplaceKind
    printf("%d\n", (i + 0));
    
    }
    #endif
    #ifdef __COMPILE_304
    else if (__choose14 == 25)
    {
    //ReplaceKind
    printf("%d\n", (i - 0));
    
    }
    #endif
    #ifdef __COMPILE_305
    else if (__choose14 == 26)
    {
    //ReplaceKind
    printf("%d\n", (i == 0));
    
    }
    #endif
    #ifdef __COMPILE_306
    else if (__choose14 == 27)
    {
    //ReplaceKind
    printf("%d\n", (i));
    
    }
    #endif
    #ifdef __COMPILE_307
    else if (__choose14 == 28)
    {
    //ReplaceKind
    printf((&argc), i);
    
    }
    #endif
    #ifdef __COMPILE_308
    else if (__choose14 == 29)
    {
    //ReplaceKind
    printf((&argv), i);
    
    }
    #endif
    #ifdef __COMPILE_309
    else if (__choose14 == 30)
    {
    //ReplaceKind
    printf((&filename), i);
    
    }
    #endif
    #ifdef __COMPILE_310
    else if (__choose14 == 31)
    {
    //ReplaceKind
    printf((&fin), i);
    
    }
    #endif
    #ifdef __COMPILE_311
    else if (__choose14 == 32)
    {
    //ReplaceKind
    printf((&i), i);
    
    }
    #endif
    #ifdef __COMPILE_312
    else if (__choose14 == 33)
    {
    //ReplaceKind
    printf((0), i);
    
    }
    #endif
    }
    //prophet generated patch
    {
    int __choose15 = __choose("__SWITCH15");
    if (__choose15 == 0)
    {}
    #ifdef __COMPILE_313
    else if (__choose15 == 1)
    {
    //AddStmtAndReplaceAtomKind
    fclose((fin));
    }
    #endif
    #ifdef __COMPILE_314
    else if (__choose15 == 2)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (-1);
    }
    #endif
    #ifdef __COMPILE_315
    else if (__choose15 == 3)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (0);
    }
    #endif
    #ifdef __COMPILE_316
    else if (__choose15 == 4)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (1);
    }
    #endif
    #ifdef __COMPILE_317
    else if (__choose15 == 5)
    {
    //AddStmtAndReplaceAtomKind
    filename[i] = (60);
    }
    #endif
    #ifdef __COMPILE_318
    else if (__choose15 == 6)
    {
    //AddStmtAndReplaceAtomKind
    fin = (fin);
    }
    #endif
    #ifdef __COMPILE_319
    else if (__choose15 == 7)
    {
    //AddStmtAndReplaceAtomKind
    fin = fopen((argv)[1], "r");
    }
    #endif
    #ifdef __COMPILE_320
    else if (__choose15 == 8)
    {
    //AddStmtKind
    fclose(fin);
    }
    #endif
    #ifdef __COMPILE_321
    else if (__choose15 == 9)
    {
    //AddStmtKind
    filename[i] = '\x00';
    }
    #endif
    #ifdef __COMPILE_322
    else if (__choose15 == 10)
    {
    //AddStmtKind
    fin = fopen(argv[1], "r");
    }
    #endif
    #ifdef __COMPILE_323
    else if (__choose15 == 11)
    {
    //AddStmtKind
    printf("%d\n", i);
    }
    #endif
    #ifdef __COMPILE_324
    else if (__choose15 == 12)
    {
    //IfExitKind
    if (__is_neg("15-12", "L22", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return -1;
    }
    #endif
    #ifdef __COMPILE_325
    else if (__choose15 == 13)
    {
    //IfExitKind
    if (__is_neg("15-13", "L22", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 0;
    }
    #endif
    #ifdef __COMPILE_326
    else if (__choose15 == 14)
    {
    //IfExitKind
    if (__is_neg("15-14", "L22", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 1;
    }
    #endif
    #ifdef __COMPILE_327
    else if (__choose15 == 15)
    {
    //IfExitKind
    if (__is_neg("15-15", "L22", 4, &(i), sizeof (i), &(fin),
    			 sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 60;
    }
    #endif
    int __choose16 = __choose("__SWITCH16");
    if (__choose16 == 0)
    {
    return 0;
    
    }
    #ifdef __COMPILE_328
    else if (__choose16 == 1)
    {
    //GuardKind
    if (!__is_neg("16-1", "L22", 4, &(i), sizeof (i), &(fin), sizeof (fin), &(argv), sizeof (argv), &(argc), sizeof (argc)))
        return 0;
    
    }
    #endif
    }
}
