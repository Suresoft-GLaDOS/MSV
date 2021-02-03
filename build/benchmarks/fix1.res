Rank 1
Priority -4.60517
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (((start > 0) && (start <= dst_len)) && !(__abst_hole(start, dst_len, dst, src, src_len, len))) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score 1.13531
========Score Detail======
NULL -4.01081
dst -3.95528
start 1.13531
len 0.26017
src -3.95528
src_len -2.07951
dst_len -0.42962
======================
Rank 2
Priority -4.60517
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (((start > 0) && (start <= dst_len)) || (__abst_hole(start, dst_len, dst, src, src_len, len))) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score 1.13531
========Score Detail======
NULL -4.01081
dst -3.95528
start 1.13531
len 0.26017
src -3.95528
src_len -2.07951
dst_len -0.42962
======================
Rank 3
Priority -4.60517
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((((start > 0)) || (__abst_hole(start, dst_len, dst, src, src_len, len))) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score 1.13531
========Score Detail======
NULL -4.01081
dst -3.95528
start 1.13531
len 0.26017
src -3.95528
src_len -2.07951
dst_len -0.42962
======================
Rank 4
Priority -4.60517
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (((len < 1) || (len > src_len)) || (__abst_hole(len, src_len, start, dst_len, dst, src)))
    len = src_len;


Score 0.37002
========Score Detail======
NULL -3.90096
dst -4.08209
start -0.78043
len 0.37002
src -4.08209
src_len -2.03787
dst_len -1.80808
======================
Rank 5
Priority -4.60517
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (((len < 1) || (len > src_len)) && !(__abst_hole(len, src_len, start, dst_len, dst, src)))
    len = src_len;


Score 0.37002
========Score Detail======
NULL -3.90096
dst -4.08209
start -0.78043
len 0.37002
src -4.08209
src_len -2.03787
dst_len -1.80808
======================
Rank 6
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(start, dst_len, dst, src, src_len, len)) && ((start > 0) && (start <= dst_len))) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -0.20009
========Score Detail======
NULL -5.34621
dst -5.29068
start -0.20009
len -1.07523
src -5.29068
src_len -3.41491
dst_len -1.76502
======================
Rank 7
Priority -4.60517
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((start + len > dst_len) && !(__abst_hole(start, len, dst_len, src_len, dst, src)))
    len = (dst_len - start);


Score -0.33611
========Score Detail======
NULL -4.60709
dst -4.78822
start -1.48656
len -0.33611
src -4.78822
src_len -2.74400
dst_len -2.51421
======================
Rank 8
Priority -4.60517
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((start + len > dst_len) || (__abst_hole(start, len, dst_len, src_len, dst, src)))
    len = (dst_len - start);


Score -0.33611
========Score Detail======
NULL -4.60709
dst -4.78822
start -1.48656
len -0.33611
src -4.78822
src_len -2.74400
dst_len -2.51421
======================
Rank 9
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(dst, src, start, len, dst->data, src->data, dst_len, src_len, dst->len, dst->size, src->len, src->size)))
    fb_hStrDelTemp_NoLock(dst);


Score -0.90590
========Score Detail======
NULL -6.05202
dst -6.31757
start -0.90590
len -1.78104
src -6.31757
src_len -4.18893
dst_len -2.53904
dst->data -5.08080
dst->len -5.64263
dst->size -5.98104
src->data -5.69825
src->len -5.64263
src->size -5.98104
======================
Rank 10
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(src, dst, start, len, dst->data, src->data, dst_len, src_len, dst->len, dst->size, src->len, src->size)))
    fb_hStrDelTemp_NoLock(src);


Score -0.91595
========Score Detail======
NULL -6.06207
dst -6.00654
start -0.91595
len -1.79109
src -6.32762
src_len -4.19898
dst_len -2.48088
dst->data -5.09085
dst->len -5.65268
dst->size -5.99109
src->data -5.70830
src->len -5.65268
src->size -5.99109
======================
Rank 11
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(len, src_len, start, dst_len, dst, src)) && ((len < 1) || (len > src_len)))
    len = src_len;


Score -0.94528
========Score Detail======
NULL -5.21626
dst -5.39739
start -2.09573
len -0.94528
src -5.39739
src_len -3.35317
dst_len -3.12338
======================
Rank 12
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(start, dst_len, dst, src, src_len, len)))
    if ((start > 0) && (start <= dst_len)) {
        --start;
        if ((len < 1) || (len > src_len))
            len = src_len;
        if (start + len > dst_len)
            len = (dst_len - start);
        memcpy(dst->data + start, src->data, len);
    }


Score -0.96620
========Score Detail======
NULL -6.11232
dst -6.05679
start -0.96620
len -1.84134
src -6.05679
src_len -4.18103
dst_len -2.53113
======================
Rank 13
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(dst, dst_len, ((FBSTRING *)dst)->len, src, src_len, ((FBSTRING *)src)->len, start, len, src->data, dst->data, ((FBSTRING *)src)->data, ((FBSTRING *)src)->size, ((FBSTRING *)dst)->data, ((FBSTRING *)dst)->size, dst->len, dst->size, src->len, src->size)))
    dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -1.30768
========Score Detail======
NULL -6.45380
dst -6.39827
start -1.30768
len -2.18282
src -4.64607
src_len -4.52250
dst_len -2.87261
((FBSTRING *)src)->data -6.76446
((FBSTRING *)src)->len -5.72333
((FBSTRING *)src)->size -6.38282
((FBSTRING *)dst)->data -6.76446
((FBSTRING *)dst)->len -5.72333
((FBSTRING *)dst)->size -6.38282
dst->data -5.48258
dst->len -6.04441
dst->size -6.38282
src->data -4.34783
src->len -6.04441
src->size -6.38282
======================
Rank 14
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(src, src_len, ((FBSTRING *)src)->len, dst, dst_len, ((FBSTRING *)dst)->len, start, len, src->data, dst->data, ((FBSTRING *)src)->data, ((FBSTRING *)src)->size, ((FBSTRING *)dst)->data, ((FBSTRING *)dst)->size, dst->len, dst->size, src->len, src->size)))
    src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -1.31773
========Score Detail======
NULL -6.46385
dst -4.65612
start -1.31773
len -2.19287
src -4.65612
src_len -4.53255
dst_len -2.88266
((FBSTRING *)src)->data -6.77451
((FBSTRING *)src)->len -5.73338
((FBSTRING *)src)->size -6.39287
((FBSTRING *)dst)->data -6.77451
((FBSTRING *)dst)->len -5.73338
((FBSTRING *)dst)->size -6.39287
dst->data -3.74043
dst->len -6.05446
dst->size -6.39287
src->data -4.35788
src->len -6.05446
src->size -6.39287
======================
Rank 15
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(start, len, dst_len, src_len, dst, src)) && (start + len > dst_len))
    len = (dst_len - start);


Score -1.64136
========Score Detail======
NULL -5.91234
dst -6.09347
start -2.79181
len -1.64136
src -6.09347
src_len -4.04925
dst_len -3.81946
======================
Rank 16
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(dst, start, len, src, dst->data, src->data, dst_len, src_len, dst->len, dst->size, src->len, src->size)))
    memcpy(dst->data + start, src->data, len);


Score -1.80114
========Score Detail======
NULL -6.07212
dst -6.25325
start -3.91915
len -1.80114
src -6.25325
src_len -4.20904
dst_len -3.97925
dst->data -5.10090
dst->len -5.66273
dst->size -6.00114
src->data -5.71835
src->len -5.66273
src->size -6.00114
======================
Rank 17
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(start, len, dst_len, src_len, dst, src)))
    if (start + len > dst_len)
        len = (dst_len - start);


Score -1.81119
========Score Detail======
NULL -6.08217
dst -6.26330
start -2.96165
len -1.81119
src -6.26330
src_len -4.21909
dst_len -3.98930
======================
Rank 18
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(len, src_len, start, dst_len, dst, src)))
    if ((len < 1) || (len > src_len))
        len = src_len;


Score -1.82124
========Score Detail======
NULL -6.09222
dst -6.27335
start -2.97170
len -1.82124
src -6.27335
src_len -4.22914
dst_len -3.99935
======================
Rank 19
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(start, len, src_len, dst_len, dst, src)))
    --start;


Score -2.19189
========Score Detail======
NULL -5.79844
dst -6.35969
start -3.42676
len -2.19189
src -6.35969
src_len -3.93536
dst_len -3.70557
======================
Rank 20
Priority -4.60517
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) && !(__abst_hole(dst, ((FBSTRING *)dst)->len, dst->data, src, src_len, dst_len, start, len, ((FBSTRING *)dst)->data, ((FBSTRING *)dst)->size, dst->len, dst->size))) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -3.18858
========Score Detail======
NULL -4.99631
dst -3.50966
start -4.67609
len -4.33768
src -3.18858
src_len -3.67051
dst_len -4.05980
((FBSTRING *)dst)->data -5.30697
((FBSTRING *)dst)->len -4.58692
((FBSTRING *)dst)->size -4.92533
dst->data -3.55477
dst->len -4.58692
dst->size -4.92533
======================
Rank 21
Priority -4.60517
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) || (__abst_hole(dst, ((FBSTRING *)dst)->len, dst->data, src, src_len, dst_len, start, len, ((FBSTRING *)dst)->data, ((FBSTRING *)dst)->size, dst->len, dst->size))) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -3.18858
========Score Detail======
NULL -4.99631
dst -3.50966
start -4.67609
len -4.33768
src -3.18858
src_len -3.67051
dst_len -4.05980
((FBSTRING *)dst)->data -5.30697
((FBSTRING *)dst)->len -4.58692
((FBSTRING *)dst)->size -4.92533
dst->data -3.55477
dst->len -4.58692
dst->size -4.92533
======================
Rank 22
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    if (!(__abst_hole(start, len, dst_len, dst, src, src_len)))
        len = (dst_len - start);
}


Score -3.32870
========Score Detail======
NULL -4.81608
dst -5.37733
start -3.94499
len -3.76816
src -5.37733
src_len -3.87957
dst_len -3.32870
======================
Rank 23
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    if (!(__abst_hole(len, src_len, start, dst_len, dst, src)))
        len = src_len;
}


Score -3.44794
========Score Detail======
NULL -4.70553
dst -5.26678
start -4.38531
len -3.65761
src -5.26678
src_len -3.44794
dst_len -3.76902
======================
Rank 24
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);


Score -3.50216
======================
Rank 25
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);


Score -3.50216
======================
Rank 26
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(start, len, src_len, dst_len, dst, src)))
    return;
--start;


Score -3.53062
========Score Detail======
NULL -6.88871
dst -6.56154
start -5.02882
len -3.53062
src -6.56154
src_len -6.23922
dst_len -6.00401
======================
Rank 27
Priority -4.60517
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) || (__abst_hole(src, ((FBSTRING *)src)->len, src->data, dst, src_len, dst_len, start, len, ((FBSTRING *)src)->data, ((FBSTRING *)src)->size, src->len, src->size))) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -3.82731
========Score Detail======
NULL -5.63504
dst -3.82731
start -5.31482
len -4.97641
src -3.82731
src_len -4.30923
dst_len -4.30923
((FBSTRING *)src)->data -5.94570
((FBSTRING *)src)->len -4.90457
((FBSTRING *)src)->size -5.56406
src->data -4.19350
src->len -5.22565
src->size -5.56406
======================
Rank 28
Priority -4.60517
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) && !(__abst_hole(src, ((FBSTRING *)src)->len, src->data, dst, src_len, dst_len, start, len, ((FBSTRING *)src)->data, ((FBSTRING *)src)->size, src->len, src->size))) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -3.82731
========Score Detail======
NULL -5.63504
dst -3.82731
start -5.31482
len -4.97641
src -3.82731
src_len -4.30923
dst_len -4.30923
((FBSTRING *)src)->data -5.94570
((FBSTRING *)src)->len -4.90457
((FBSTRING *)src)->size -5.56406
src->data -4.19350
src->len -5.22565
src->size -5.56406
======================
Rank 29
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    if ((__abst_hole(len, src_len, start, dst_len, dst, src)))
        return;
    len = src_len;
}


Score -3.89537
========Score Detail======
NULL -6.02502
dst -5.69785
start -5.83747
len -3.89537
src -5.69785
src_len -5.14137
dst_len -5.45270
======================
Rank 30
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (dst_len);
}


Score -3.98852
======================
Rank 31
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (start);
}


Score -3.98852
======================
Rank 32
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (len);
}


Score -3.98852
======================
Rank 33
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    if ((__abst_hole(start, len, dst_len, dst, src, src_len)))
        return;
    len = (dst_len - start);
}


Score -4.00592
========Score Detail======
NULL -6.13557
dst -5.80841
start -5.40149
len -4.00592
src -5.80841
src_len -5.56326
dst_len -5.01672
======================
Rank 34
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (len);
}


Score -4.09908
======================
Rank 35
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (src_len);
}


Score -4.09908
======================
Rank 36
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(dst, src, start, len, dst->data, src->data, dst_len, src_len, dst->len, dst->size, src->len, src->size)))
    return;
fb_hStrDelTemp_NoLock(dst);


Score -4.19363
========Score Detail======
NULL -8.06756
dst -7.76247
start -5.52441
len -4.19363
src -7.76247
src_len -7.41807
dst_len -6.26725
dst->data -8.27596
dst->len -8.49755
dst->size -8.82082
src->data -8.30892
src->len -8.49755
src->size -8.82082
======================
Rank 37
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(src, dst, start, len, dst->data, src->data, dst_len, src_len, dst->len, dst->size, src->len, src->size)))
    return;
fb_hStrDelTemp_NoLock(src);


Score -4.20368
========Score Detail======
NULL -8.07761
dst -7.46119
start -5.53446
len -4.20368
src -7.77252
src_len -7.42812
dst_len -4.96980
dst->data -8.28601
dst->len -8.50760
dst->size -8.83087
src->data -8.31897
src->len -8.50760
src->size -8.83087
======================
Rank 38
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);


Score -4.20405
======================
Rank 39
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);


Score -4.20405
======================
Rank 40
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(dst, start, len, src, dst->data, src->data, dst_len, src_len, dst->len, dst->size, src->len, src->size)))
    return;
memcpy(dst->data + start, src->data, len);


Score -4.21373
========Score Detail======
NULL -8.08766
dst -8.29660
start -6.80478
len -4.21373
src -8.29660
src_len -7.43817
dst_len -7.20296
dst->data -8.29606
dst->len -8.51765
dst->size -8.84092
src->data -8.32902
src->len -8.51765
src->size -8.84092
======================
Rank 41
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(start, len, dst_len, src_len, dst, src)))
    return;
if (start + len > dst_len)
    len = (dst_len - start);


Score -4.22378
========Score Detail======
NULL -8.09771
dst -8.30665
start -6.23601
len -4.22378
src -8.30665
src_len -7.44822
dst_len -7.21301
======================
Rank 42
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(len, src_len, start, dst_len, dst, src)))
    return;
if ((len < 1) || (len > src_len))
    len = src_len;


Score -4.23383
========Score Detail======
NULL -8.10776
dst -8.31670
start -6.24606
len -4.23383
src -8.31670
src_len -7.45827
dst_len -7.22306
======================
Rank 43
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (dst_len);
}


Score -4.25104
======================
Rank 44
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (start);
}


Score -4.25104
======================
Rank 45
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(start, dst_len, dst, src, src_len, len)))
    return;
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -4.25393
========Score Detail======
NULL -8.12786
dst -7.51145
start -5.58471
len -4.25393
src -7.51145
src_len -6.17087
dst_len -5.02005
======================
Rank 46
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));


Score -4.51085
======================
Rank 47
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (src_len);
}


Score -4.52950
======================
Rank 48
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);


Score -4.55234
======================
Rank 49
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(dst, ((FBSTRING *)dst)->len, dst->data, src, src_len, dst_len, start, len, ((FBSTRING *)dst)->data, ((FBSTRING *)dst)->size, dst->len, dst->size)) && ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0))) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -4.56419
========Score Detail======
NULL -6.37192
dst -4.88527
start -6.05170
len -5.71329
src -4.56419
src_len -5.04611
dst_len -5.43540
((FBSTRING *)dst)->data -6.68258
((FBSTRING *)dst)->len -5.96253
((FBSTRING *)dst)->size -6.30094
dst->data -4.93038
dst->len -5.96253
dst->size -6.30094
======================
Rank 50
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (2147483647);
}


Score -4.58471
======================
Rank 51
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (1);
}


Score -4.58471
======================
Rank 52
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (-2147483648);
}


Score -4.58471
======================
Rank 53
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (0);
}


Score -4.58471
======================
Rank 54
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(src, ((FBSTRING *)src)->len, src->data, dst, src_len, dst_len, start, len, ((FBSTRING *)src)->data, ((FBSTRING *)src)->size, src->len, src->size)))
    if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
        fb_hStrDelTemp_NoLock(src);
        fb_hStrDelTemp_NoLock(dst);
        ;
        return;
    }


Score -4.66617
========Score Detail======
NULL -6.47390
dst -4.66617
start -6.15368
len -5.81527
src -4.66617
src_len -5.14809
dst_len -5.14809
((FBSTRING *)src)->data -6.78456
((FBSTRING *)src)->len -5.74343
((FBSTRING *)src)->size -6.40292
src->data -5.03236
src->len -6.06451
src->size -6.40292
======================
Rank 55
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(dst, ((FBSTRING *)dst)->len, dst->data, src, src_len, dst_len, start, len, ((FBSTRING *)dst)->data, ((FBSTRING *)dst)->size, dst->len, dst->size)))
    if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
        fb_hStrDelTemp_NoLock(src);
        fb_hStrDelTemp_NoLock(dst);
        ;
        return;
    }


Score -4.67622
========Score Detail======
NULL -6.48395
dst -4.99730
start -6.16373
len -5.82532
src -4.67622
src_len -5.15814
dst_len -5.54744
((FBSTRING *)dst)->data -6.79461
((FBSTRING *)dst)->len -6.07456
((FBSTRING *)dst)->size -6.41297
dst->data -5.04241
dst->len -6.07456
dst->size -6.41297
======================
Rank 56
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (2147483647);
}


Score -4.69527
======================
Rank 57
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (0);
}


Score -4.69527
======================
Rank 58
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (-2147483648);
}


Score -4.69527
======================
Rank 59
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (1);
}


Score -4.69527
======================
Rank 60
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));


Score -4.89412
======================
Rank 61
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(dst, dst_len, ((FBSTRING *)dst)->len, src, src_len, ((FBSTRING *)src)->len, start, len, src->data, dst->data, ((FBSTRING *)src)->data, ((FBSTRING *)src)->size, ((FBSTRING *)dst)->data, ((FBSTRING *)dst)->size, dst->len, dst->size, src->len, src->size)))
    return;
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -4.99816
========Score Detail======
NULL -8.87209
dst -8.25567
start -6.32894
len -4.99816
src -8.14763
src_len -6.91510
dst_len -5.76428
((FBSTRING *)src)->data -9.62928
((FBSTRING *)src)->len -8.99075
((FBSTRING *)src)->size -9.62535
((FBSTRING *)dst)->data -9.62928
((FBSTRING *)dst)->len -8.99075
((FBSTRING *)dst)->size -9.62535
dst->data -9.08049
dst->len -9.30208
dst->size -9.62535
src->data -9.00541
src->len -9.30208
src->size -9.62535
======================
Rank 62
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(src, src_len, ((FBSTRING *)src)->len, dst, dst_len, ((FBSTRING *)dst)->len, start, len, src->data, dst->data, ((FBSTRING *)src)->data, ((FBSTRING *)src)->size, ((FBSTRING *)dst)->data, ((FBSTRING *)dst)->size, dst->len, dst->size, src->len, src->size)))
    return;
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -5.00821
========Score Detail======
NULL -8.88214
dst -8.15769
start -6.33899
len -5.00821
src -8.15769
src_len -6.92515
dst_len -5.77433
((FBSTRING *)src)->data -9.63933
((FBSTRING *)src)->len -9.00080
((FBSTRING *)src)->size -9.63540
((FBSTRING *)dst)->data -9.63933
((FBSTRING *)dst)->len -9.00080
((FBSTRING *)dst)->size -9.63540
dst->data -8.98250
dst->len -9.31213
dst->size -9.63540
src->data -9.01546
src->len -9.31213
src->size -9.63540
======================
Rank 63
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (!(__abst_hole(src, ((FBSTRING *)src)->len, src->data, dst, src_len, dst_len, start, len, ((FBSTRING *)src)->data, ((FBSTRING *)src)->size, src->len, src->size)) && ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0))) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -5.19286
========Score Detail======
NULL -7.00060
dst -5.19286
start -6.68037
len -6.34196
src -5.19286
src_len -5.67479
dst_len -5.67479
((FBSTRING *)src)->data -7.31125
((FBSTRING *)src)->len -6.27012
((FBSTRING *)src)->size -6.92961
src->data -5.55905
src->len -6.59120
src->size -6.92961
======================
Rank 64
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memmove(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 65
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
bcmp(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 66
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
scanf(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 67
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
scanf(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 68
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
scanf(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 69
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memmove(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 70
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcmp(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 71
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcmp(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 72
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
printf(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 73
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
printf(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 74
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_DevLptWrite(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 75
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_PrinterWrite(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 76
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_DevFileWrite(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 77
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_DevScrnWrite(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 78
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_FilePutBackEx(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 79
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hFilePrintBufferEx(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 80
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
XSetOMValues(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 81
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
XSetICValues(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 82
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
FB_MEMCPYX(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 83
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
wscanf(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 84
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
wscanf(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 85
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
wprintf(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 86
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
XGetICValues(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 87
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
FB_MEMCMP(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 88
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
FB_MEMCPY(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 89
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
XGetOMValues(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 90
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
XGetOCValues(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 91
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
XSetIMValues(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 92
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
XCreateIC(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 93
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
XGetIMValues(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 94
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
XCreateOC(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 95
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
XSetOCValues(dst->data + start, src->data, len);


Score -5.37956
======================
Rank 96
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));


Score -5.38223
======================
Rank 97
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));


Score -5.39228
======================
Rank 98
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);


Score -5.67201
======================
Rank 99
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -5.77149
======================
Rank 100
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);


Score -5.78039
======================
Rank 101
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -5.90936
======================
Rank 102
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    memcpy((src)->data + start, src->data, len);
    len = src_len;
}


Score -5.95648
======================
Rank 103
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    memcpy(dst->data + start, (dst)->data, len);
    len = src_len;
}


Score -5.95648
======================
Rank 104
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    memcpy(dst->data + start, (src)->data, len);
    len = src_len;
}


Score -5.95648
======================
Rank 105
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    memcpy(dst->data + start, src->data, len);
    len = src_len;
}


Score -5.95648
======================
Rank 106
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    memcpy((dst)->data + start, src->data, len);
    len = src_len;
}


Score -5.95648
======================
Rank 107
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_LCASE(dst);


Score -6.06963
======================
Rank 108
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_SetDate(dst);


Score -6.06963
======================
Rank 109
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_IsDate(dst);


Score -6.06963
======================
Rank 110
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_DateValue(dst);


Score -6.06963
======================
Rank 111
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_TimeValue(dst);


Score -6.06963
======================
Rank 112
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hTimeGetIntervalType(dst);


Score -6.06963
======================
Rank 113
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_StrAllocTempDescV(dst);


Score -6.06963
======================
Rank 114
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_UCASE(dst);


Score -6.06963
======================
Rank 115
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_DylibLoad(dst);


Score -6.06963
======================
Rank 116
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_SetTime(dst);


Score -6.06963
======================
Rank 117
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_FileKill(dst);


Score -6.06963
======================
Rank 118
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_Shell(dst);


Score -6.06963
======================
Rank 119
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_Run(dst);


Score -6.06963
======================
Rank 120
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_VAL(dst);


Score -6.06963
======================
Rank 121
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_LTRIM(dst);


Score -6.06963
======================
Rank 122
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_TRIM(dst);


Score -6.06963
======================
Rank 123
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_RTRIM(dst);


Score -6.06963
======================
Rank 124
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_CVD(dst);


Score -6.06963
======================
Rank 125
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_CVI(dst);


Score -6.06963
======================
Rank 126
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTempDesc(dst);


Score -6.06963
======================
Rank 127
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_StrDelete(dst);


Score -6.06963
======================
Rank 128
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_StrAllocTempResult(dst);


Score -6.06963
======================
Rank 129
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_Chain(dst);


Score -6.06963
======================
Rank 130
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_IsDate(src);


Score -6.07968
======================
Rank 131
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_DylibLoad(src);


Score -6.07968
======================
Rank 132
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_RTRIM(src);


Score -6.07968
======================
Rank 133
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_FileKill(src);


Score -6.07968
======================
Rank 134
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_DateValue(src);


Score -6.07968
======================
Rank 135
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hTimeGetIntervalType(src);


Score -6.07968
======================
Rank 136
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_TimeValue(src);


Score -6.07968
======================
Rank 137
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_SetTime(src);


Score -6.07968
======================
Rank 138
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_SetDate(src);


Score -6.07968
======================
Rank 139
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_TRIM(src);


Score -6.07968
======================
Rank 140
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_Shell(src);


Score -6.07968
======================
Rank 141
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_Chain(src);


Score -6.07968
======================
Rank 142
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_Run(src);


Score -6.07968
======================
Rank 143
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_LCASE(src);


Score -6.07968
======================
Rank 144
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_UCASE(src);


Score -6.07968
======================
Rank 145
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_StrDelete(src);


Score -6.07968
======================
Rank 146
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_VAL(src);


Score -6.07968
======================
Rank 147
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_CVI(src);


Score -6.07968
======================
Rank 148
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_LTRIM(src);


Score -6.07968
======================
Rank 149
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_CVD(src);


Score -6.07968
======================
Rank 150
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_StrAllocTempResult(src);


Score -6.07968
======================
Rank 151
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_StrAllocTempDescV(src);


Score -6.07968
======================
Rank 152
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTempDesc(src);


Score -6.07968
======================
Rank 153
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);


Score -6.08956
======================
Rank 154
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);


Score -6.13314
======================
Rank 155
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);


Score -6.14720
======================
Rank 156
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    memcpy(dst->data + start, (dst)->data, len);
    len = (dst_len - start);
}


Score -6.42247
======================
Rank 157
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    memcpy(dst->data + start, (src)->data, len);
    len = (dst_len - start);
}


Score -6.42247
======================
Rank 158
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    memcpy(dst->data + start, src->data, len);
    len = (dst_len - start);
}


Score -6.42247
======================
Rank 159
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    memcpy((dst)->data + start, src->data, len);
    len = (dst_len - start);
}


Score -6.42247
======================
Rank 160
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    memcpy((src)->data + start, src->data, len);
    len = (dst_len - start);
}


Score -6.42247
======================
Rank 161
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);


Score -6.52835
======================
Rank 162
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);


Score -6.52835
======================
Rank 163
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp(dst);


Score -6.55773
======================
Rank 164
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp(src);


Score -6.56778
======================
Rank 165
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);


Score -6.57193
======================
Rank 166
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
--start;


Score -6.65782
======================
Rank 167
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(src, ((FBSTRING *)src)->len, src->data, dst, src_len, dst_len, start, len, ((FBSTRING *)src)->data, ((FBSTRING *)src)->size, src->len, src->size)))
    return;
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -6.70104
========Score Detail======
NULL -8.89219
dst -8.16774
start -8.70464
len -8.38137
src -8.16774
src_len -6.70104
dst_len -6.70104
((FBSTRING *)src)->data -9.64938
((FBSTRING *)src)->len -9.01085
((FBSTRING *)src)->size -9.64545
src->data -9.54135
src->len -9.32218
src->size -9.64545
======================
Rank 168
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(dst, ((FBSTRING *)dst)->len, dst->data, src, src_len, dst_len, start, len, ((FBSTRING *)dst)->data, ((FBSTRING *)dst)->size, dst->len, dst->size)))
    return;
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -6.71109
========Score Detail======
NULL -8.90224
dst -8.48911
start -8.71469
len -8.39142
src -8.17779
src_len -6.71109
dst_len -8.32992
((FBSTRING *)dst)->data -9.65944
((FBSTRING *)dst)->len -9.33223
((FBSTRING *)dst)->size -9.65550
dst->data -9.55140
dst->len -9.33223
dst->size -9.65550
======================
Rank 169
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);


Score -6.76911
======================
Rank 170
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);


Score -6.76911
======================
Rank 171
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);


Score -6.76911
======================
Rank 172
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);


Score -6.76911
======================
Rank 173
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    --start;
    len = src_len;
}


Score -7.06821
======================
Rank 174
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
if (start + len > dst_len)
    len = (dst_len - start);


Score -7.20194
======================
Rank 175
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
if ((len < 1) || (len > src_len))
    len = src_len;


Score -7.21199
======================
Rank 176
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
fb_hStrDelTemp_NoLock(dst);


Score -7.23965
======================
Rank 177
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);


Score -7.24013
======================
Rank 178
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);


Score -7.24013
======================
Rank 179
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);


Score -7.24013
======================
Rank 180
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);


Score -7.24013
======================
Rank 181
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
fb_hStrDelTemp_NoLock(src);


Score -7.24970
======================
Rank 182
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -7.28932
======================
Rank 183
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
memcpy(dst->data + start, src->data, len);


Score -7.29630
======================
Rank 184
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -7.29937
======================
Rank 185
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -7.29995
======================
Rank 186
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -7.31000
======================
Rank 187
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
int src_len, dst_len;


Score -7.31947
======================
Rank 188
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
--start;
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -7.32005
======================
Rank 189
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    if ((len < 1) || (len > src_len))
        len = src_len;
    len = src_len;
}


Score -7.44373
======================
Rank 190
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    --start;
    len = (dst_len - start);
}


Score -7.47118
======================
Rank 191
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((__abst_hole(dst, src, start, len)))
    return;
int src_len, dst_len;


Score -7.50357
========Score Detail======
NULL -8.34657
dst -7.93345
start -8.15902
len -7.83575
src -7.50357
======================
Rank 192
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    if ((len < 1) || (len > src_len))
        len = src_len;
    len = (dst_len - start);
}


Score -7.55428
======================
Rank 193
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (start);
    len = src_len;
}


Score -7.62412
======================
Rank 194
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(src, 0, sizeof (*(src)));
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -7.64595
======================
Rank 195
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(dst, 0, sizeof (*(dst)));
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -7.64595
======================
Rank 196
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (src_len);
    len = src_len;
}


Score -7.67699
======================
Rank 197
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (start);
    len = (dst_len - start);
}


Score -7.68418
======================
Rank 198
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
--start;


Score -7.69495
======================
Rank 199
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
--start;


Score -7.69495
======================
Rank 200
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
--start;


Score -7.69495
======================
Rank 201
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
--start;


Score -7.69495
======================
Rank 202
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
--start;


Score -7.69495
======================
Rank 203
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (len);
    len = src_len;
}


Score -7.70959
======================
Rank 204
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (start);
    len = (dst_len - start);
}


Score -7.75798
======================
Rank 205
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (len);
    len = (dst_len - start);
}


Score -7.82015
======================
Rank 206
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (((FBSTRING *)src)->len & ~2147483648UL);
    len = src_len;
}


Score -7.82523
======================
Rank 207
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    fb_hStrDelTemp_NoLock((dst));
    len = src_len;
}


Score -7.84564
======================
Rank 208
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    fb_hStrDelTemp_NoLock((src));
    len = src_len;
}


Score -7.84564
======================
Rank 209
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    fb_hStrDelTemp_NoLock(src);
    len = src_len;
}


Score -7.84564
======================
Rank 210
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    fb_hStrDelTemp_NoLock(dst);
    len = src_len;
}


Score -7.84564
======================
Rank 211
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (dst_len);
    len = src_len;
}


Score -7.89865
======================
Rank 212
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (src_len);
    len = src_len;
}


Score -7.89865
======================
Rank 213
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (2147483647);
    len = src_len;
}


Score -7.91685
======================
Rank 214
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (0);
    len = src_len;
}


Score -7.91685
======================
Rank 215
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (1);
    len = src_len;
}


Score -7.91685
======================
Rank 216
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (-2147483648);
    len = src_len;
}


Score -7.91685
======================
Rank 217
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (len);
    len = src_len;
}


Score -7.93125
======================
Rank 218
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
int src_len, dst_len;


Score -7.95608
======================
Rank 219
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    fb_hStrDelTemp_NoLock((dst));
    len = (dst_len - start);
}


Score -7.95619
======================
Rank 220
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    fb_hStrDelTemp_NoLock(dst);
    len = (dst_len - start);
}


Score -7.95619
======================
Rank 221
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    fb_hStrDelTemp_NoLock(src);
    len = (dst_len - start);
}


Score -7.95619
======================
Rank 222
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    fb_hStrDelTemp_NoLock((src));
    len = (dst_len - start);
}


Score -7.95619
======================
Rank 223
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (dst_len);
    len = (dst_len - start);
}


Score -7.95871
======================
Rank 224
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
    len = (dst_len - start);
}


Score -7.95909
======================
Rank 225
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
fb_hStrDelTemp_NoLock(src);


Score -7.96618
======================
Rank 226
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
fb_hStrDelTemp_NoLock(src);


Score -7.96618
======================
Rank 227
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(dst, 0, sizeof (*(dst)));
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -7.97184
======================
Rank 228
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -8.01643
======================
Rank 229
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -8.01643
======================
Rank 230
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -8.01643
======================
Rank 231
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -8.01643
======================
Rank 232
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (src_len);
    len = (dst_len - start);
}


Score -8.03251
======================
Rank 233
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (dst_len);
    len = (dst_len - start);
}


Score -8.03251
======================
Rank 234
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (len);
    len = (dst_len - start);
}


Score -8.06511
======================
Rank 235
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(((FBSTRING *)dst), 0, sizeof (*(((FBSTRING *)dst))));
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -8.07454
======================
Rank 236
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (-2147483648);
    len = (dst_len - start);
}


Score -8.11279
======================
Rank 237
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (1);
    len = (dst_len - start);
}


Score -8.11279
======================
Rank 238
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (2147483647);
    len = (dst_len - start);
}


Score -8.11279
======================
Rank 239
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (0);
    len = (dst_len - start);
}


Score -8.11279
======================
Rank 240
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(src, 0, sizeof (*(src)));
memcpy(dst->data + start, src->data, len);


Score -8.17208
======================
Rank 241
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(dst, 0, sizeof (*(dst)));
memcpy(dst->data + start, src->data, len);


Score -8.17208
======================
Rank 242
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);
--start;


Score -8.20265
======================
Rank 243
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (-2147483648);
    len = src_len;
}


Score -8.20374
======================
Rank 244
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (2147483647);
    len = src_len;
}


Score -8.20374
======================
Rank 245
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (0);
    len = src_len;
}


Score -8.20374
======================
Rank 246
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (1);
    len = src_len;
}


Score -8.20374
======================
Rank 247
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (start);
    len = src_len;
}


Score -8.20460
======================
Rank 248
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
--start;


Score -8.22595
======================
Rank 249
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (dst_len);
    len = src_len;
}


Score -8.23044
======================
Rank 250
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (src_len);
    len = src_len;
}


Score -8.23044
======================
Rank 251
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = src_len;
    len = src_len;
}


Score -8.23044
======================
Rank 252
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (start);
    len = src_len;
}


Score -8.23044
======================
Rank 253
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (len);
    len = src_len;
}


Score -8.23044
======================
Rank 254
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);
--start;


Score -8.27607
======================
Rank 255
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);
--start;


Score -8.29938
======================
Rank 256
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);
--start;


Score -8.29938
======================
Rank 257
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (-2147483648);
    len = (dst_len - start);
}


Score -8.31429
======================
Rank 258
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (1);
    len = (dst_len - start);
}


Score -8.31429
======================
Rank 259
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (2147483647);
    len = (dst_len - start);
}


Score -8.31429
======================
Rank 260
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (0);
    len = (dst_len - start);
}


Score -8.31429
======================
Rank 261
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);
--start;


Score -8.33624
======================
Rank 262
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);
--start;


Score -8.33624
======================
Rank 263
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);
--start;


Score -8.33624
======================
Rank 264
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);
--start;


Score -8.33624
======================
Rank 265
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = src_len;
    len = (dst_len - start);
}


Score -8.34100
======================
Rank 266
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (src_len);
    len = (dst_len - start);
}


Score -8.34100
======================
Rank 267
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (len);
    len = (dst_len - start);
}


Score -8.34100
======================
Rank 268
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -8.34232
======================
Rank 269
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -8.34232
======================
Rank 270
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
int src_len, dst_len;


Score -8.37760
======================
Rank 271
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
    len = src_len;
}


Score -8.40571
======================
Rank 272
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
--start;


Score -8.40856
======================
Rank 273
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
--start;


Score -8.40856
======================
Rank 274
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
--start;


Score -8.40856
======================
Rank 275
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
--start;


Score -8.40856
======================
Rank 276
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
memcpy(dst->data + start, src->data, len);


Score -8.41556
======================
Rank 277
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
memcpy(dst->data + start, src->data, len);


Score -8.41556
======================
Rank 278
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
memcpy(dst->data + start, src->data, len);


Score -8.41556
======================
Rank 279
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
memcpy(dst->data + start, src->data, len);


Score -8.41556
======================
Rank 280
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
memcpy(dst->data + start, src->data, len);


Score -8.41556
======================
Rank 281
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);
--start;


Score -8.42163
======================
Rank 282
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);
--start;


Score -8.42163
======================
Rank 283
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);
--start;


Score -8.42163
======================
Rank 284
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);
--start;


Score -8.42163
======================
Rank 285
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(((FBSTRING *)src), 0, sizeof (*(((FBSTRING *)src))));
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -8.43816
======================
Rank 286
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);
--start;


Score -8.44724
======================
Rank 287
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(((FBSTRING *)src), 0, sizeof (*(((FBSTRING *)src))));
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -8.44821
======================
Rank 288
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);
--start;


Score -8.47313
======================
Rank 289
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    dst_len = (dst_len);
    len = src_len;
}


Score -8.47913
======================
Rank 290
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (start);
    len = (dst_len - start);
}


Score -8.51216
======================
Rank 291
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (dst_len);
    len = (dst_len - start);
}


Score -8.51216
======================
Rank 292
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (((FBSTRING *)src)->len & ~2147483648UL);
    len = (dst_len - start);
}


Score -8.51627
======================
Rank 293
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -8.55387
======================
Rank 294
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -8.55387
======================
Rank 295
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -8.55387
======================
Rank 296
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -8.55387
======================
Rank 297
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -8.55387
======================
Rank 298
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -8.56392
======================
Rank 299
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -8.56392
======================
Rank 300
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -8.56392
======================
Rank 301
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -8.56392
======================
Rank 302
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -8.56392
======================
Rank 303
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(dst, 0, sizeof (*(dst)));
fb_hStrDelTemp_NoLock(dst);


Score -8.56896
======================
Rank 304
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(src, 0, sizeof (*(src)));
fb_hStrDelTemp_NoLock(src);


Score -8.57901
======================
Rank 305
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    src_len = (src_len);
    len = (dst_len - start);
}


Score -8.58969
======================
Rank 306
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((len < 1) || (len > src_len))
    len = src_len;
--start;


Score -8.59417
======================
Rank 307
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);
--start;


Score -8.62099
======================
Rank 308
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);
--start;


Score -8.62224
======================
Rank 309
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
memcpy(dst->data + start, src->data, len);


Score -8.62612
======================
Rank 310
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
memcpy(dst->data + start, src->data, len);


Score -8.62612
======================
Rank 311
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
memcpy(dst->data + start, src->data, len);


Score -8.62612
======================
Rank 312
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
memcpy(dst->data + start, src->data, len);


Score -8.62612
======================
Rank 313
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
if (start + len > dst_len)
    len = (dst_len - start);


Score -8.63617
======================
Rank 314
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
if (start + len > dst_len)
    len = (dst_len - start);


Score -8.63617
======================
Rank 315
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
if (start + len > dst_len)
    len = (dst_len - start);


Score -8.63617
======================
Rank 316
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
if (start + len > dst_len)
    len = (dst_len - start);


Score -8.63617
======================
Rank 317
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);
--start;


Score -8.64554
======================
Rank 318
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
if ((len < 1) || (len > src_len))
    len = src_len;


Score -8.64622
======================
Rank 319
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -8.64622
======================
Rank 320
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -8.64622
======================
Rank 321
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
if ((len < 1) || (len > src_len))
    len = src_len;


Score -8.64622
======================
Rank 322
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (dst_len - start);
    len = src_len;
}


Score -8.72105
======================
Rank 323
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
int src_len, dst_len;


Score -8.80914
======================
Rank 324
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
int src_len, dst_len;


Score -8.80914
======================
Rank 325
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (2147483647);
    len = src_len;
}


Score -8.82663
======================
Rank 326
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (0);
    len = src_len;
}


Score -8.82663
======================
Rank 327
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (1);
    len = src_len;
}


Score -8.82663
======================
Rank 328
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (-2147483648);
    len = src_len;
}


Score -8.82663
======================
Rank 329
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    if (start + len > dst_len)
        len = (dst_len - start);
    len = (dst_len - start);
}


Score -8.84560
======================
Rank 330
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (dst_len - start);
    len = (dst_len - start);
}


Score -8.85047
======================
Rank 331
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = src_len;
--start;


Score -8.86062
======================
Rank 332
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
--start;


Score -8.86062
======================
Rank 333
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (src_len);
--start;


Score -8.86062
======================
Rank 334
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
int src_len, dst_len;


Score -8.86941
======================
Rank 335
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
int src_len, dst_len;


Score -8.86941
======================
Rank 336
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
int src_len, dst_len;


Score -8.86941
======================
Rank 337
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (2147483647);
    len = (dst_len - start);
}


Score -8.93719
======================
Rank 338
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (1);
    len = (dst_len - start);
}


Score -8.93719
======================
Rank 339
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (-2147483648);
    len = (dst_len - start);
}


Score -8.93719
======================
Rank 340
Priority -4.83633
At location fbc-src/src/rtlib/libfb_str_midassign.c:79
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    len = (0);
    len = (dst_len - start);
}


Score -8.93719
======================
Rank 341
Priority -4.72577
At location fbc-src/src/rtlib/libfb_str_midassign.c:76
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
{
    if (start + len > dst_len)
        len = (dst_len - start);
    len = src_len;
}


Score -8.96390
======================
Rank 342
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len);
--start;


Score -9.03178
======================
Rank 343
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
fb_hStrDelTemp_NoLock(dst);


Score -9.03227
======================
Rank 344
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
fb_hStrDelTemp_NoLock(dst);


Score -9.03227
======================
Rank 345
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
fb_hStrDelTemp_NoLock(dst);


Score -9.03227
======================
Rank 346
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
fb_hStrDelTemp_NoLock(dst);


Score -9.03227
======================
Rank 347
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
fb_hStrDelTemp_NoLock(dst);


Score -9.03227
======================
Rank 348
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
--start;


Score -9.03731
======================
Rank 349
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
fb_hStrDelTemp_NoLock(src);


Score -9.04232
======================
Rank 350
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((len < 1) || (len > src_len))
    len = src_len;
fb_hStrDelTemp_NoLock(dst);


Score -9.07141
======================
Rank 351
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((len < 1) || (len > src_len))
    len = src_len;
fb_hStrDelTemp_NoLock(src);


Score -9.08146
======================
Rank 352
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((len < 1) || (len > src_len))
    len = src_len;
memcpy(dst->data + start, src->data, len);


Score -9.09151
======================
Rank 353
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((len < 1) || (len > src_len))
    len = src_len;
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.10156
======================
Rank 354
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((len < 1) || (len > src_len))
    len = src_len;
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.11161
======================
Rank 355
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((len < 1) || (len > src_len))
    len = src_len;
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.13171
======================
Rank 356
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);
fb_hStrDelTemp_NoLock(src);


Score -9.13739
======================
Rank 357
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);
memcpy(dst->data + start, src->data, len);


Score -9.16158
======================
Rank 358
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.18764
======================
Rank 359
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
fb_hStrDelTemp_NoLock(dst);


Score -9.19229
======================
Rank 360
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
fb_hStrDelTemp_NoLock(dst);


Score -9.19229
======================
Rank 361
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
fb_hStrDelTemp_NoLock(dst);


Score -9.19229
======================
Rank 362
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
fb_hStrDelTemp_NoLock(dst);


Score -9.19229
======================
Rank 363
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
fb_hStrDelTemp_NoLock(src);


Score -9.20234
======================
Rank 364
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
fb_hStrDelTemp_NoLock(src);


Score -9.20234
======================
Rank 365
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.22117
======================
Rank 366
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.24070
======================
Rank 367
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.24070
======================
Rank 368
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.25075
======================
Rank 369
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -9.26468
======================
Rank 370
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -9.26468
======================
Rank 371
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.27473
======================
Rank 372
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.27473
======================
Rank 373
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.27473
======================
Rank 374
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.27473
======================
Rank 375
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.28478
======================
Rank 376
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.28478
======================
Rank 377
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.28478
======================
Rank 378
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.28478
======================
Rank 379
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((src));
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.29483
======================
Rank 380
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(src);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.29483
======================
Rank 381
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(src, 0, sizeof (*(src)));
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.30775
======================
Rank 382
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);
memcpy(dst->data + start, src->data, len);


Score -9.30944
======================
Rank 383
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
fb_hStrDelTemp_NoLock(src);


Score -9.31204
======================
Rank 384
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
fb_hStrDelTemp_NoLock(src);


Score -9.31204
======================
Rank 385
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
fb_hStrDelTemp_NoLock(src);


Score -9.31204
======================
Rank 386
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
fb_hStrDelTemp_NoLock(src);


Score -9.31204
======================
Rank 387
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.31767
======================
Rank 388
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.31767
======================
Rank 389
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(src, 0, sizeof (*(src)));
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.31780
======================
Rank 390
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(dst, 0, sizeof (*(dst)));
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.31780
======================
Rank 391
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.32772
======================
Rank 392
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.32772
======================
Rank 393
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(src, 0, sizeof (*(src)));
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.32785
======================
Rank 394
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.36229
======================
Rank 395
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.36229
======================
Rank 396
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.36229
======================
Rank 397
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.36229
======================
Rank 398
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.36229
======================
Rank 399
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.38752
======================
Rank 400
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.38904
======================
Rank 401
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.39757
======================
Rank 402
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(((FBSTRING *)dst), 0, sizeof (*(((FBSTRING *)dst))));
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.44158
======================
Rank 403
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);
memcpy(dst->data + start, src->data, len);


Score -9.49733
======================
Rank 404
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.50050
======================
Rank 405
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.50050
======================
Rank 406
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.50050
======================
Rank 407
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.50050
======================
Rank 408
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.50738
======================
Rank 409
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.51055
======================
Rank 410
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.51055
======================
Rank 411
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.51743
======================
Rank 412
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
memcpy(dst->data + start, src->data, len);


Score -9.52063
======================
Rank 413
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.53068
======================
Rank 414
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.53538
======================
Rank 415
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.53702
======================
Rank 416
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.54073
======================
Rank 417
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);
fb_hStrDelTemp_NoLock(dst);


Score -9.54390
======================
Rank 418
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.54543
======================
Rank 419
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
int src_len, dst_len;


Score -9.54954
======================
Rank 420
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
int src_len, dst_len;


Score -9.54954
======================
Rank 421
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);
fb_hStrDelTemp_NoLock(dst);


Score -9.55288
======================
Rank 422
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);
fb_hStrDelTemp_NoLock(src);


Score -9.55395
======================
Rank 423
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);
fb_hStrDelTemp_NoLock(src);


Score -9.55760
======================
Rank 424
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);
fb_hStrDelTemp_NoLock(src);


Score -9.55760
======================
Rank 425
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -9.56378
======================
Rank 426
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);
memcpy(dst->data + start, src->data, len);


Score -9.56401
======================
Rank 427
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.57383
======================
Rank 428
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.57406
======================
Rank 429
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.58411
======================
Rank 430
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);
fb_hStrDelTemp_NoLock(dst);


Score -9.58641
======================
Rank 431
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);
memcpy(dst->data + start, src->data, len);


Score -9.58731
======================
Rank 432
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);
memcpy(dst->data + start, src->data, len);


Score -9.58731
======================
Rank 433
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);
fb_hStrDelTemp_NoLock(src);


Score -9.59646
======================
Rank 434
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -9.59731
======================
Rank 435
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.59736
======================
Rank 436
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.59736
======================
Rank 437
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
--start;


Score -9.60173
======================
Rank 438
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
--start;


Score -9.60173
======================
Rank 439
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
--start;


Score -9.60173
======================
Rank 440
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
--start;


Score -9.60173
======================
Rank 441
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);
fb_hStrDelTemp_NoLock(dst);


Score -9.60407
======================
Rank 442
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);
fb_hStrDelTemp_NoLock(dst);


Score -9.60407
======================
Rank 443
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);
fb_hStrDelTemp_NoLock(dst);


Score -9.60407
======================
Rank 444
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);
fb_hStrDelTemp_NoLock(dst);


Score -9.60407
======================
Rank 445
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (start);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.60736
======================
Rank 446
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.60741
======================
Rank 447
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.60741
======================
Rank 448
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.60785
======================
Rank 449
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.60785
======================
Rank 450
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);
fb_hStrDelTemp_NoLock(src);


Score -9.61412
======================
Rank 451
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);
fb_hStrDelTemp_NoLock(src);


Score -9.61412
======================
Rank 452
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);
fb_hStrDelTemp_NoLock(src);


Score -9.61412
======================
Rank 453
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);
fb_hStrDelTemp_NoLock(src);


Score -9.61412
======================
Rank 454
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);
memcpy(dst->data + start, src->data, len);


Score -9.62417
======================
Rank 455
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);
memcpy(dst->data + start, src->data, len);


Score -9.62417
======================
Rank 456
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);
memcpy(dst->data + start, src->data, len);


Score -9.62417
======================
Rank 457
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);
memcpy(dst->data + start, src->data, len);


Score -9.62417
======================
Rank 458
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);
fb_hStrDelTemp_NoLock(src);


Score -9.62440
======================
Rank 459
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.63422
======================
Rank 460
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.63422
======================
Rank 461
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.63422
======================
Rank 462
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.63422
======================
Rank 463
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.64138
======================
Rank 464
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.64427
======================
Rank 465
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.64427
======================
Rank 466
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.64427
======================
Rank 467
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.64427
======================
Rank 468
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.67465
======================
Rank 469
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -9.68818
======================
Rank 470
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);
memcpy(dst->data + start, src->data, len);


Score -9.70957
======================
Rank 471
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);
memcpy(dst->data + start, src->data, len);


Score -9.70957
======================
Rank 472
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);
memcpy(dst->data + start, src->data, len);


Score -9.70957
======================
Rank 473
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);
memcpy(dst->data + start, src->data, len);


Score -9.70957
======================
Rank 474
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.71962
======================
Rank 475
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.71962
======================
Rank 476
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.71962
======================
Rank 477
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.71962
======================
Rank 478
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.72526
======================
Rank 479
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.72967
======================
Rank 480
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.72967
======================
Rank 481
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.72967
======================
Rank 482
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.72967
======================
Rank 483
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);
memcpy(dst->data + start, src->data, len);


Score -9.73517
======================
Rank 484
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.74522
======================
Rank 485
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (start + len > dst_len)
    len = (dst_len - start);
--start;


Score -9.74844
======================
Rank 486
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.75527
======================
Rank 487
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);
fb_hStrDelTemp_NoLock(dst);


Score -9.76272
======================
Rank 488
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);
fb_hStrDelTemp_NoLock(dst);


Score -9.76272
======================
Rank 489
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -9.76518
======================
Rank 490
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.77503
======================
Rank 491
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.77503
======================
Rank 492
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.77503
======================
Rank 493
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.77503
======================
Rank 494
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.77523
======================
Rank 495
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (src_len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.78508
======================
Rank 496
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.78508
======================
Rank 497
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);
fb_hStrDelTemp_NoLock(dst);


Score -9.79625
======================
Rank 498
Priority -4.90668
At location fbc-src/src/rtlib/libfb_str_midassign.c:73
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len - start);
--start;


Score -9.80705
======================
Rank 499
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.80949
======================
Rank 500
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.81954
======================
Rank 501
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((len < 1) || (len > src_len))
    len = src_len;
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -9.82181
======================
Rank 502
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((len < 1) || (len > src_len))
    len = src_len;
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.83186
======================
Rank 503
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = src_len;
fb_hStrDelTemp_NoLock(dst);


Score -9.83377
======================
Rank 504
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
fb_hStrDelTemp_NoLock(dst);


Score -9.83377
======================
Rank 505
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (src_len);
fb_hStrDelTemp_NoLock(dst);


Score -9.83377
======================
Rank 506
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
fb_hStrDelTemp_NoLock(src);


Score -9.84382
======================
Rank 507
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = src_len;
fb_hStrDelTemp_NoLock(src);


Score -9.84382
======================
Rank 508
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (src_len);
fb_hStrDelTemp_NoLock(src);


Score -9.84382
======================
Rank 509
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.85086
======================
Rank 510
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = src_len;
memcpy(dst->data + start, src->data, len);


Score -9.85387
======================
Rank 511
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
memcpy(dst->data + start, src->data, len);


Score -9.85387
======================
Rank 512
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (src_len);
memcpy(dst->data + start, src->data, len);


Score -9.85387
======================
Rank 513
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.86393
======================
Rank 514
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = src_len;
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.86393
======================
Rank 515
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (src_len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -9.86393
======================
Rank 516
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = src_len;
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.87398
======================
Rank 517
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (src_len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.87398
======================
Rank 518
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -9.87398
======================
Rank 519
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
fb_hStrDelTemp_NoLock(src);


Score -9.89382
======================
Rank 520
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = src_len;
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.89408
======================
Rank 521
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (src_len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.89408
======================
Rank 522
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.89408
======================
Rank 523
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);
fb_hStrDelTemp_NoLock(dst);


Score -9.91276
======================
Rank 524
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);
fb_hStrDelTemp_NoLock(dst);


Score -9.91276
======================
Rank 525
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);
fb_hStrDelTemp_NoLock(dst);


Score -9.91276
======================
Rank 526
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);
fb_hStrDelTemp_NoLock(dst);


Score -9.91276
======================
Rank 527
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -9.91315
======================
Rank 528
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.92320
======================
Rank 529
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((len < 1) || (len > src_len))
    len = src_len;
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.94329
======================
Rank 530
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -9.94408
======================
Rank 531
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if ((len < 1) || (len > src_len))
    len = src_len;
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.95334
======================
Rank 532
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);
fb_hStrDelTemp_NoLock(dst);


Score -9.96976
======================
Rank 533
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -9.97617
======================
Rank 534
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);
fb_hStrDelTemp_NoLock(src);


Score -9.97981
======================
Rank 535
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -9.98399
======================
Rank 536
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -9.98399
======================
Rank 537
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.98602
======================
Rank 538
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.98602
======================
Rank 539
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.98622
======================
Rank 540
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.99404
======================
Rank 541
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -9.99404
======================
Rank 542
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (((FBSTRING *)src)->len & ~2147483648UL);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -9.99607
======================
Rank 543
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);
fb_hStrDelTemp_NoLock(dst);


Score -9.99901
======================
Rank 544
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len);
fb_hStrDelTemp_NoLock(dst);


Score -10.00494
======================
Rank 545
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
int src_len, dst_len;


Score -10.00583
======================
Rank 546
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
int src_len, dst_len;


Score -10.00583
======================
Rank 547
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
int src_len, dst_len;


Score -10.00583
======================
Rank 548
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
int src_len, dst_len;


Score -10.00583
======================
Rank 549
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);
fb_hStrDelTemp_NoLock(src);


Score -10.00906
======================
Rank 550
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
fb_hStrDelTemp_NoLock(dst);


Score -10.01046
======================
Rank 551
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.01095
======================
Rank 552
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.01095
======================
Rank 553
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.01095
======================
Rank 554
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.01095
======================
Rank 555
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len);
fb_hStrDelTemp_NoLock(src);


Score -10.01499
======================
Rank 556
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.01752
======================
Rank 557
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);
memcpy(dst->data + start, src->data, len);


Score -10.01911
======================
Rank 558
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
fb_hStrDelTemp_NoLock(src);


Score -10.02051
======================
Rank 559
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
memcpy(dst->data + start, src->data, len);


Score -10.02504
======================
Rank 560
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len);
memcpy(dst->data + start, src->data, len);


Score -10.02504
======================
Rank 561
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.02757
======================
Rank 562
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -10.02917
======================
Rank 563
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -10.03509
======================
Rank 564
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -10.03922
======================
Rank 565
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
if (start + len > dst_len)
    len = (dst_len - start);


Score -10.04061
======================
Rank 566
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);
memcpy(dst->data + start, src->data, len);


Score -10.04242
======================
Rank 567
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -10.04514
======================
Rank 568
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -10.05066
======================
Rank 569
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.05079
======================
Rank 570
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);
if (start + len > dst_len)
    len = (dst_len - start);


Score -10.05247
======================
Rank 571
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.06084
======================
Rank 572
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -10.06252
======================
Rank 573
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.06524
======================
Rank 574
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.07076
======================
Rank 575
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (src_len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.08572
======================
Rank 576
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (dst_len);
fb_hStrDelTemp_NoLock(src);


Score -10.11343
======================
Rank 577
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);
fb_hStrDelTemp_NoLock(dst);


Score -10.13665
======================
Rank 578
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
fb_hStrDelTemp_NoLock(dst);


Score -10.18857
======================
Rank 579
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.20503
======================
Rank 580
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
int src_len, dst_len;


Score -10.26497
======================
Rank 581
Priority -4.97703
At location fbc-src/src/rtlib/libfb_str_midassign.c:48
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
int src_len, dst_len;


Score -10.26497
======================
Rank 582
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);
fb_hStrDelTemp_NoLock(src);


Score -10.26938
======================
Rank 583
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);
fb_hStrDelTemp_NoLock(src);


Score -10.26938
======================
Rank 584
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);
fb_hStrDelTemp_NoLock(src);


Score -10.26938
======================
Rank 585
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);
fb_hStrDelTemp_NoLock(src);


Score -10.26938
======================
Rank 586
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = src_len;
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.27021
======================
Rank 587
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.27021
======================
Rank 588
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (src_len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.27021
======================
Rank 589
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = src_len;
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.28026
======================
Rank 590
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (src_len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.28026
======================
Rank 591
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.28026
======================
Rank 592
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.30533
======================
Rank 593
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.30533
======================
Rank 594
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.30533
======================
Rank 595
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.30533
======================
Rank 596
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.30620
======================
Rank 597
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (start);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.30620
======================
Rank 598
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memset(dst, 0, sizeof (*(dst)));
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.31117
======================
Rank 599
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.31964
======================
Rank 600
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.31964
======================
Rank 601
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.31964
======================
Rank 602
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.31964
======================
Rank 603
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.32021
======================
Rank 604
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.35496
======================
Rank 605
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.35496
======================
Rank 606
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.35496
======================
Rank 607
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.35496
======================
Rank 608
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.35496
======================
Rank 609
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.35496
======================
Rank 610
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.35496
======================
Rank 611
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.35496
======================
Rank 612
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.36501
======================
Rank 613
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.36501
======================
Rank 614
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.36501
======================
Rank 615
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.36501
======================
Rank 616
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (start + len > dst_len)
    len = (dst_len - start);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.37256
======================
Rank 617
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.38709
======================
Rank 618
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.38709
======================
Rank 619
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.38709
======================
Rank 620
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.38709
======================
Rank 621
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (2147483647);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.39714
======================
Rank 622
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (1);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.39714
======================
Rank 623
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (0);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.39714
======================
Rank 624
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
src_len = (-2147483648);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.39714
======================
Rank 625
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.44138
======================
Rank 626
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.44690
======================
Rank 627
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.45143
======================
Rank 628
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.45695
======================
Rank 629
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.52200
======================
Rank 630
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock((dst));
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.53099
======================
Rank 631
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
fb_hStrDelTemp_NoLock(dst);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.53099
======================
Rank 632
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (dst_len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.58073
======================
Rank 633
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.59311
======================
Rank 634
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = src_len;
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.62262
======================
Rank 635
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (src_len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.62262
======================
Rank 636
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.62262
======================
Rank 637
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = src_len;
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.63267
======================
Rank 638
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (src_len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.63267
======================
Rank 639
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.69577
======================
Rank 640
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.69577
======================
Rank 641
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.69577
======================
Rank 642
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -10.69577
======================
Rank 643
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (1);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.70582
======================
Rank 644
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (-2147483648);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.70582
======================
Rank 645
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (0);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.70582
======================
Rank 646
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
dst_len = (2147483647);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -10.70582
======================
Rank 647
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.85849
======================
Rank 648
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.85849
======================
Rank 649
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.85849
======================
Rank 650
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.85849
======================
Rank 651
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.86854
======================
Rank 652
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.86854
======================
Rank 653
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.86854
======================
Rank 654
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -10.86854
======================
Rank 655
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
fb_hStrDelTemp_NoLock(dst);


Score -10.89605
======================
Rank 656
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
fb_hStrDelTemp_NoLock(dst);


Score -10.89605
======================
Rank 657
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
fb_hStrDelTemp_NoLock(dst);


Score -10.89605
======================
Rank 658
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
fb_hStrDelTemp_NoLock(dst);


Score -10.89605
======================
Rank 659
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
fb_hStrDelTemp_NoLock(src);


Score -10.90610
======================
Rank 660
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
fb_hStrDelTemp_NoLock(src);


Score -10.90610
======================
Rank 661
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
fb_hStrDelTemp_NoLock(src);


Score -10.90610
======================
Rank 662
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
fb_hStrDelTemp_NoLock(src);


Score -10.90610
======================
Rank 663
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
memcpy(dst->data + start, src->data, len);


Score -10.91615
======================
Rank 664
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
memcpy(dst->data + start, src->data, len);


Score -10.91615
======================
Rank 665
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
memcpy(dst->data + start, src->data, len);


Score -10.91615
======================
Rank 666
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
memcpy(dst->data + start, src->data, len);


Score -10.91615
======================
Rank 667
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
if (start + len > dst_len)
    len = (dst_len - start);


Score -10.92620
======================
Rank 668
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
if (start + len > dst_len)
    len = (dst_len - start);


Score -10.92620
======================
Rank 669
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
if (start + len > dst_len)
    len = (dst_len - start);


Score -10.92620
======================
Rank 670
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
if (start + len > dst_len)
    len = (dst_len - start);


Score -10.92620
======================
Rank 671
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -10.93625
======================
Rank 672
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -10.93625
======================
Rank 673
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -10.93625
======================
Rank 674
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -10.93625
======================
Rank 675
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.95635
======================
Rank 676
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.95635
======================
Rank 677
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.95635
======================
Rank 678
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -10.95635
======================
Rank 679
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (start + len > dst_len)
    len = (dst_len - start);
memcpy(dst->data + start, src->data, len);


Score -11.05936
======================
Rank 680
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (dst)->data, len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -11.09498
======================
Rank 681
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, (src)->data, len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -11.09498
======================
Rank 682
Priority -4.87653
At location fbc-src/src/rtlib/libfb_str_midassign.c:81
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len - start);
memcpy(dst->data + start, src->data, len);


Score -11.11559
======================
Rank 683
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -11.11763
======================
Rank 684
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -11.11763
======================
Rank 685
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -11.12768
======================
Rank 686
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (start);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -11.12768
======================
Rank 687
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (len);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -11.12768
======================
Rank 688
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -11.16190
======================
Rank 689
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -11.16190
======================
Rank 690
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -11.16190
======================
Rank 691
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((dst)->data + start, src->data, len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -11.17195
======================
Rank 692
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy((src)->data + start, src->data, len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -11.17195
======================
Rank 693
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (start + len > dst_len)
    len = (dst_len - start);
if (start + len > dst_len)
    len = (dst_len - start);


Score -11.18155
======================
Rank 694
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (start + len > dst_len)
    len = (dst_len - start);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -11.19160
======================
Rank 695
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (start + len > dst_len)
    len = (dst_len - start);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -11.21995
======================
Rank 696
Priority -4.88658
At location fbc-src/src/rtlib/libfb_str_midassign.c:78
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len - start);
if (start + len > dst_len)
    len = (dst_len - start);


Score -11.23778
======================
Rank 697
Priority -4.89663
At location fbc-src/src/rtlib/libfb_str_midassign.c:75
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len - start);
if ((len < 1) || (len > src_len))
    len = src_len;


Score -11.24783
======================
Rank 698
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -11.33249
======================
Rank 699
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -11.33249
======================
Rank 700
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -11.33249
======================
Rank 701
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -11.33249
======================
Rank 702
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (1);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -11.34254
======================
Rank 703
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (2147483647);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -11.34254
======================
Rank 704
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (-2147483648);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -11.34254
======================
Rank 705
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (0);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -11.34254
======================
Rank 706
Priority -4.94688
At location fbc-src/src/rtlib/libfb_str_midassign.c:60
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len - start);
if ((src == ((void *)0)) || (src->data == ((void *)0)) || ((((FBSTRING *)src)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -11.57531
======================
Rank 707
Priority -4.95693
At location fbc-src/src/rtlib/libfb_str_midassign.c:52
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len - start);
if ((dst == ((void *)0)) || (dst->data == ((void *)0)) || ((((FBSTRING *)dst)->len & ~2147483648UL) == 0)) {
    fb_hStrDelTemp_NoLock(src);
    fb_hStrDelTemp_NoLock(dst);
    ;
    return;
}


Score -11.61829
======================
Rank 708
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
memcpy(dst->data + start, src->data, len);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -11.66377
======================
Rank 709
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len - start);
fb_hStrDelTemp_NoLock(dst);


Score -11.78636
======================
Rank 710
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (start + len > dst_len)
    len = (dst_len - start);
fb_hStrDelTemp_NoLock(src);


Score -11.86442
======================
Rank 711
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (start + len > dst_len)
    len = (dst_len - start);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -11.91467
======================
Rank 712
Priority -4.86648
At location fbc-src/src/rtlib/libfb_str_midassign.c:85
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len - start);
fb_hStrDelTemp_NoLock(src);


Score -11.98432
======================
Rank 713
Priority -4.91673
At location fbc-src/src/rtlib/libfb_str_midassign.c:71
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len - start);
if ((start > 0) && (start <= dst_len)) {
    --start;
    if ((len < 1) || (len > src_len))
        len = src_len;
    if (start + len > dst_len)
        len = (dst_len - start);
    memcpy(dst->data + start, src->data, len);
}


Score -12.03457
======================
Rank 714
Priority -4.85643
At location fbc-src/src/rtlib/libfb_str_midassign.c:86
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (start + len > dst_len)
    len = (dst_len - start);
fb_hStrDelTemp_NoLock(dst);


Score -12.37246
======================
Rank 715
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len - start);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -12.41071
======================
Rank 716
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
len = (dst_len - start);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -12.42076
======================
Rank 717
Priority -4.92678
At location fbc-src/src/rtlib/libfb_str_midassign.c:69
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (start + len > dst_len)
    len = (dst_len - start);
dst_len = (((FBSTRING *)dst)->len & ~2147483648UL);


Score -12.60477
======================
Rank 718
Priority -4.93683
At location fbc-src/src/rtlib/libfb_str_midassign.c:68
--Src File: fbc-src/src/rtlib/libfb_str_midassign.c
Fragment 0:
if (start + len > dst_len)
    len = (dst_len - start);
src_len = (((FBSTRING *)src)->len & ~2147483648UL);


Score -12.61482
======================
