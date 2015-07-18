/*
 * test.h
 *
 * Created: 15.07.2015 13:17:42
 *  Author: Trol
 */ 


#ifndef TEST_H_
#define TEST_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define CMD_RESET			0	// ��������� ��� ���� � ����� ������ (�� ������� ������)
#define CMD_RESET_FULL		1	// ��������� ��� ���� � ����� ������ (������� ������)
#define CMD_INIT_16			2	// (mask[2]) ����������� ����� �����-������
#define CMD_INIT_28			3	// (mask[3]) ����������� ����� �����-������
#define CMD_SET_16			4	// (mask0[2], mask1[2]) ���������� ���������� 0 � 1 �� ������� �� ������
#define CMD_SET_28			5	// (mask0[3], mask1[3]) ���������� ���������� 0 � 1 �� ������� �� ������
#define CMD_TEST_16			6	// (mask0[2], mask1[2]) ���������, ��� �� ������� ���������� ��������� �������
#define CMD_TEST_28			7	// (mask0[3], mask1[3]) ���������, ��� �� ������� ���������� ��������� �������

#define CMD_END				0xff	// ������� ��������� �����

bool TestLogic();

/************************************************************************
  ��������� ����, ����� ��������� �� ������                            
  ����� ���������� ���������� ���������� ����� � ��������� ��������� ��
  ������ ��������� �������
************************************************************************/
bool TestData(uint8_t **buffer);

const char* GetDeviceName();



#endif /* TEST_H_ */