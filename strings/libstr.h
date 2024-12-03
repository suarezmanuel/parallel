#ifndef LIBSTR_H
#define LIBSTR_H

#define MAX_STR 256

extern int hamming_dist(char str1[MAX_STR], char str2[MAX_STR]);

extern int b64_distance(char str1[MAX_STR], char str2[MAX_STR]);

#endif