/*
工具函数
by LiuP
*/

#ifndef UTIL_H
#define UTIL_H

// 产生随机数
void InitRandom();
int Random();

// 匹配字符串
bool ContainString(const char *str, int strLength, const char *pattern, int patternLength);

bool IsNumeric(char c);

int ToNumeric(char c);

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

#endif
