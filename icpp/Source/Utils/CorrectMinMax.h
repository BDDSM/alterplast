#ifndef UTILS_CORRECT_MIN_MAX_H
#define UTILS_CORRECT_MIN_MAX_H

/* ���� ��� ����������� �������� � min � max, ������������� � SDK.
�.�. ��� �������� �� std::min � std::max ����� ������������� ����������
����������, �� ���� ��������� �������� ��� ����. � ����� ���� ����������
�������� ���� ���� */

#pragma once

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <algorithm>

#endif // UTILS_CORRECT_MIN_MAX_H
