#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <vector>

using namespace std;

//обозначение пунктов меню
enum InputMenu { consoleInput = 1, fileInput, randomInput, exitApp };

//пропуск строки при неправильном вводе
void IgnoreLine();

//считывание с консоли пункта меню
int GetMenuOption(string message);

//считывание с консоли целого числа
int GetInt(string message);

//считывание пути к файлу
string GetLink(string message);

//считывание ответа формата (y/n)
char GetAnswer(string message);
