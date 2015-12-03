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

#define CMD_RESET							0	// ��������� ��� ���� � ����� ������ (�� ������� ������)
#define CMD_RESET_FULL					1	// ��������� ��� ���� � ����� ������ (������� ������)
#define CMD_INIT_16						2	// (mask[2]) ����������� ����� �����-������
#define CMD_INIT_24						3	// (mask[3]) ����������� ����� �����-������
#define CMD_INIT_28						4	// (mask[4]) ����������� ����� �����-������
#define CMD_SET_16						5	// (mask0[2], mask1[2]) ���������� ���������� 0 � 1 �� ������� �� ������
#define CMD_SET_24						6	// (mask0[3], mask1[2]) ���������� ���������� 0 � 1 �� ������� �� ������
#define CMD_SET_28						7	// (mask0[4], mask1[3]) ���������� ���������� 0 � 1 �� ������� �� ������
#define CMD_TEST_16						8	// (mask0[2], mask1[2]) ���������, ��� �� ������� ���������� ��������� �������
#define CMD_TEST_24						9	// (mask0[3], mask1[2]) ���������, ��� �� ������� ���������� ��������� �������
#define CMD_TEST_28						10	// (mask0[4], mask1[3]) ���������, ��� �� ������� ���������� ��������� �������
#define CMD_PULSE_PLUS					11	// (pin[1]) ������ ������������� ������� (0 -> 1) �� �����
#define CMD_PULSE_MINUS					12	// (pin[1]) ������ ������������� ������� (1 -> 0) �� �����
#define CMD_SET_ALL_16					13	// (mask[2] ������������� ��� ���� �� �����)
#define CMD_SET_ALL_24					14	// (mask[3] ������������� ��� ���� �� �����)
#define CMD_SET_ALL_28					15	// (mask[4] ������������� ��� ���� �� �����)
#define CMD_TEST_ALL_16					16	// (mask[2] ��������� ������ �� ���� ����� �� �� �����)
#define CMD_TEST_ALL_24					17	// (mask[3] ��������� ������ �� ���� ����� �� �� �����)
#define CMD_TEST_ALL_28					18	// (mask[4] ��������� ������ �� ���� ����� �� �� �����)
#define CMD_SET_ALL_AND_TEST_16		19	// (mask_set[2], mask_test[2]) ������������� � ��������� ��� ����
#define CMD_SET_ALL_AND_TEST_24		20	// (mask_set[3], mask_test[3]) ������������� � ��������� ��� ����
#define CMD_SET_ALL_AND_TEST_28		21	// (mask_set[4], mask_test[4]) ������������� � ��������� ��� ����
#define CMD_LAST_PULSE					22	// ��������� ��������� �������
#define CMD_LAST_PULSE_AND_TEST_16	23	// (mask[2]) ��������� ��������� ������� � ��������� ��������� ���� �����
#define CMD_LAST_PULSE_AND_TEST_24	24	// (mask[3]) ��������� ��������� ������� � ��������� ��������� ���� �����
#define CMD_LAST_PULSE_AND_TEST_28	25	// (mask[4]) ��������� ��������� ������� � ��������� ��������� ���� �����
#define CMD_TEST_Z_16					26	// (mask[2]) ��������� ��� ���� ��������� � Z-���������
#define CMD_TEST_Z_24					27 // (mask[3]) ��������� ��� ���� ��������� � Z-���������
#define CMD_TEST_Z_28					28 // (mask[4]) ��������� ��� ���� ��������� � Z-���������
#define CMD_TEST_OC_16					29 // (mask[2]) ��������� ��� ���� �������� �������� � �������� �����������
#define CMD_TEST_OC_24					30 // (mask[3]) ��������� ��� ���� �������� �������� � �������� �����������
#define CMD_TEST_OC_28					31 // (mask[4]) ��������� ��� ���� �������� �������� � �������� �����������
#define CMD_REPEAT_PULSE				32	// (uint16) ��������� ��������� ������� ��������� ���

#define CMD_PRINT_14						200

#define CMD_END							0xff	// ������� ��������� �����

#define TYPE_TTL		0
#define TYPE_CMOS		1

bool TestLogic(uint8_t type);

/************************************************************************
  ��������� ����, ����� ��������� �� ������                            
  ����� ���������� ���������� ���������� ����� � ��������� ��������� ��
  ������ ��������� �������
************************************************************************/
bool TestData();

char* GetDeviceName();



#endif // TEST_H_