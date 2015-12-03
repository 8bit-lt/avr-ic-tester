/*
 * test.c
 *
 * Created: 15.07.2015 13:17:54
 *  Author: Trol
 */ 

#include "test.h"

#include "tester_io.h"
#include "debug.h"

#include "data_ttl.h"
#include "data_cmos.h"
#include "ui.h"
#include "keyboard.h"

#include <util/delay.h>
#include <string.h>

char chipName[64];
char testResultName[128];
uint8_t lastPulse;

uint8_t *ptr;

static void pgm_read_val16(val16_t *val) {
	val->b0 = pgm_read_byte(ptr++);
	val->b1 = pgm_read_byte(ptr++);
}

static void pgm_read_val24(val24_t *val) {
	val->b0 = pgm_read_byte(ptr++);
	val->b1 = pgm_read_byte(ptr++);
	val->b2 = pgm_read_byte(ptr++);
}

static void pgm_read_val28(val28_t *val) {
	val->b0 = pgm_read_byte(ptr++);
	val->b1 = pgm_read_byte(ptr++);
	val->b2 = pgm_read_byte(ptr++);
	val->b3 = pgm_read_byte(ptr++);
}

static void pulse() {
	if (lastPulse < 0x80) {
		TesterSetPin(lastPulse, 0);
		_delay_us(1);
		TesterSetPin(lastPulse, 1);
	} else {
		TesterSetPin(lastPulse-0x80, 1);
		_delay_us(1);
		TesterSetPin(lastPulse-0x80, 0);		
	}
}


bool TestData() {
//	uint8_t succesCnt = 0;
	uint8_t failureCnt = 0;
	
	// �������� ��� ����������
	for (uint8_t i = 0; i < sizeof(chipName); i++) {
		char ch = pgm_read_byte(ptr++);
		chipName[i] = ch;
		if (ch == 0) {
			break;
		}
	}
	
	TesterReset(true);
		
	for (;;) {
		uint8_t cmd = pgm_read_byte(ptr++);
		MSG_DEC("cmd ", cmd);
		
		switch (cmd) {
			case CMD_RESET:				// ��������� ��� ���� � ����� ������ (�� ������� ������)
				TesterReset(false);
				break;
			case CMD_RESET_FULL:		// ��������� ��� ���� � ����� ������ (������� ������)
				TesterReset(true);
				break;
			case CMD_INIT_16:			// (mask[2]) ����������� ����� �����-������
				{
					val16_t mask;
					pgm_read_val16(&mask);
					TesterConfig16(&mask);
				}
				break;
			case CMD_INIT_24:			// (mask[3]) ����������� ����� �����-������
				{
					val24_t mask;
					pgm_read_val24(&mask);
					TesterConfig24(&mask);
				}
				break;				
			case CMD_INIT_28:			// (mask[4]) ����������� ����� �����-������
				{
					val28_t mask;
					pgm_read_val28(&mask);
					TesterConfig28(&mask);
				}
				break;
			case CMD_SET_16:			// (mask0[2], mask1[2]) ���������� ���������� 0 � 1 �� ������� �� ������
				{
					val16_t mask0, mask1;
					pgm_read_val16(&mask0);
					pgm_read_val16(&mask1);
					
					if (failureCnt == 0) {
						TesterSet16(&mask0, &mask1);
					}
				}			
				break;
			case CMD_SET_24:			// (mask0[3], mask1[3]) ���������� ���������� 0 � 1 �� ������� �� ������
				{
					val24_t mask0, mask1;
					pgm_read_val24(&mask0);
					pgm_read_val24(&mask1);
				
					if (failureCnt == 0) {
						TesterSet24(&mask0, &mask1);
					}
				}
				break;
			case CMD_SET_28:			// (mask0[4], mask1[4]) ���������� ���������� 0 � 1 �� ������� �� ������
				{
					val28_t mask0, mask1;
					pgm_read_val28(&mask0);
					pgm_read_val28(&mask1);
					
					if (failureCnt == 0) {
						TesterSet28(&mask0, &mask1);
					}
				}
				break;
			case CMD_TEST_16:			// (mask0[2], mask1[2]) ���������, ��� �� ������� ���������� ��������� �������
				{
					val16_t mask0, mask1;
					pgm_read_val16(&mask0);
					pgm_read_val16(&mask1);
										
					if (failureCnt == 0) {
						if (TesterTest16(&mask0, &mask1)) {
	MSG("success");
							//succesCnt++;
						} else {
	MSG("fail");
							failureCnt++;
						}
					}
				}
				break;
			case CMD_TEST_24:			// (mask0[3], mask1[3]) ���������, ��� �� ������� ���������� ��������� �������
				{
					val24_t mask0, mask1;
					pgm_read_val24(&mask0);
					pgm_read_val24(&mask1);
				
					if (failureCnt == 0) {
						if (TesterTest24(&mask0, &mask1)) {
							MSG("success");
							//succesCnt++;
						} else {
							MSG("fail");
							failureCnt++;
						}
					}
				}
				break;
			case CMD_TEST_28:			// (mask0[4], mask1[4]) ���������, ��� �� ������� ���������� ��������� �������
				{
					val28_t mask0, mask1;
					pgm_read_val28(&mask0);
					pgm_read_val28(&mask1);
					
					if (failureCnt == 0) {
						if (TesterTest28(&mask0, &mask1)) {
							MSG("success");
							//succesCnt++;
						} else {
							MSG("fail");
							failureCnt++;
						}
					}
				}
				break;
				
			case CMD_PULSE_PLUS:	// pin[1] ������ ������������� ������� (0 -> 1) �� �����
				{
					uint8_t pin = pgm_read_byte(ptr);
					ptr++;
					lastPulse = pin;
					pulse();
				}
				break;
				
			case CMD_PULSE_MINUS:	// pin[1] ������ ������������� ������� (1 -> 0) �� �����
				{
					uint8_t pin = pgm_read_byte(ptr);
					ptr++;
					lastPulse = pin + 0x80;
					pulse();
				}			
				break;
			case CMD_SET_ALL_AND_TEST_16:	// (mask_set[2], mask_test[2]) ������������� � ��������� ��� ����
			case CMD_SET_ALL_16:	// (mask[2] ������������� ��� ���� �� �����)
				{
					val16_t mask;
					pgm_read_val16(&mask);
					
					if (failureCnt == 0) {
						TesterSetAll16(&mask);
					}
				}
				if (cmd == CMD_SET_ALL_16) {
					break;
				}
			
			case CMD_TEST_ALL_16:	// (mask[2] ��������� ������ �� ���� ����� �� �� �����)
				{
					val16_t mask;
					pgm_read_val16(&mask);
					
					if (failureCnt == 0) {
						if (TesterTestAll16(&mask)) {
							MSG("success");
							//succesCnt++;
						} else {
							MSG("fail");
							failureCnt++;
						}
					}
				}			
				break;
			case CMD_SET_ALL_AND_TEST_24:	// (mask_set[3], mask_test[3]) ������������� � ��������� ��� ����
			case CMD_SET_ALL_24:	// (mask[3] ������������� ��� ���� �� �����)
				{
					val24_t mask;
					pgm_read_val24(&mask);
					
					if (failureCnt == 0) {
						TesterSetAll24(&mask);
					}
				}
				if (cmd == CMD_SET_ALL_24) {
					break;
				}
			case CMD_TEST_ALL_24:	// (mask[3] ��������� ������ �� ���� ����� �� �� �����)
				{
					val24_t mask;
					pgm_read_val24(&mask);
					
					if (failureCnt == 0) {
						if (TesterTestAll24(&mask)) {
							MSG("success");
							//succesCnt++;
						} else {
							MSG("fail");
							failureCnt++;
						}
					}
				}
				break;
			case CMD_SET_ALL_AND_TEST_28:	// (mask_set[4], mask_test[4]) ������������� � ��������� ��� ����				
			case CMD_SET_ALL_28:	// (mask[4] ������������� ��� ���� �� �����)
				{
					val28_t mask;
					pgm_read_val28(&mask);
					
					if (failureCnt == 0) {
						TesterSetAll28(&mask);
					}
				}
				if (cmd == CMD_SET_ALL_28) {
					break;
				}
			case CMD_TEST_ALL_28:	// (mask[4] ��������� ������ �� ���� ����� �� �� �����)
				{
					val28_t mask;
					pgm_read_val28(&mask);
					
					if (failureCnt == 0) {
						if (TesterTestAll28(&mask)) {
							MSG("success");
							//succesCnt++;
						} else {
							MSG("fail");
							failureCnt++;
						}
					}
				}
				break;

			case CMD_LAST_PULSE:	// ��������� ��������� �������
				if (lastPulse < 0x80) {
					TesterSetPin(lastPulse, 0);
					_delay_us(1);
					TesterSetPin(lastPulse, 1);	
				} else {
					TesterSetPin(lastPulse-0x80, 1);
					_delay_us(1);
					TesterSetPin(lastPulse-0x80, 0);					
				}
				break;
			case CMD_LAST_PULSE_AND_TEST_16:	// (mask[2]) ��������� ��������� ������� � ��������� ��������� ���� �����
				pulse();
				{
					val16_t mask;
					pgm_read_val16(&mask);
					
					if (failureCnt == 0) {
						if (TesterTestAll16(&mask)) {
							MSG("success");
							//succesCnt++;
						} else {
							MSG("fail");
							failureCnt++;
						}
					}
				}
				break;
			case CMD_LAST_PULSE_AND_TEST_24:	// (mask[3]) ��������� ��������� ������� � ��������� ��������� ���� �����
				pulse();
				{
					val24_t mask;
					pgm_read_val24(&mask);
					
					if (failureCnt == 0) {
						if (TesterTestAll24(&mask)) {
							MSG("success");
							//succesCnt++;
						} else {
							MSG("fail");
							failureCnt++;
						}
					}
				}
				break;
			case CMD_LAST_PULSE_AND_TEST_28:	// (mask[4]) ��������� ��������� ������� � ��������� ��������� ���� �����
				pulse();
				{
					val28_t mask;
					pgm_read_val28(&mask);
					
					if (failureCnt == 0) {
						if (TesterTestAll28(&mask)) {
							MSG("success");
							//succesCnt++;
						} else {
							MSG("fail");
							failureCnt++;
						}
					}
				}
				break;
				
			case CMD_TEST_Z_16:	// (mask[2]) ��������� ��� ���� ��������� � Z-���������
				{
					val16_t mask;
					pgm_read_val16(&mask);
					
					if (failureCnt == 0) {
						uint8_t save_a = PORTA;
						uint8_t save_b = PORTB;
						uint8_t save_c = PORTC;
						uint8_t save_d = PORTD;
						
						TesterSet16(&mask, 0);
						if (!TesterTest16(&mask, 0)) {
							MSG("fail-1");
							failureCnt++;
						} else {
							TesterSet16(0, &mask);
							if (!TesterTest16(0, &mask)) {
								MSG("fail-2");
								failureCnt++;
							} else {
								MSG("success");
							}
						}
						PORTA = save_a;
						PORTB = save_b;
						PORTC = save_c;
						PORTD = save_d;
					}
				}
				break;

			case CMD_TEST_Z_24:	// (mask[3]) ��������� ��� ���� ��������� � Z-���������
				{
					val24_t mask;
					pgm_read_val24(&mask);
					
					if (failureCnt == 0) {
						uint8_t save_a = PORTA;
						uint8_t save_b = PORTB;
						uint8_t save_c = PORTC;
						uint8_t save_d = PORTD;
						
						TesterSet24(&mask, 0);
						if (!TesterTest24(&mask, 0)) {
							MSG("fail-1");
							failureCnt++;
						} else {
							TesterSet24(0, &mask);
							if (!TesterTest24(0, &mask)) {
								MSG("fail-2");
								failureCnt++;
							} else {
								MSG("success");
							}
						}
						PORTA = save_a;
						PORTB = save_b;
						PORTC = save_c;
						PORTD = save_d;						
					}
				}				
				break;
				
			case CMD_TEST_Z_28:	// (mask[4]) ��������� ��� ���� ��������� � Z-���������
				{
					val28_t mask;
					pgm_read_val28(&mask);
					
					if (failureCnt == 0) {
						uint8_t save_a = PORTA;
						uint8_t save_b = PORTB;
						uint8_t save_c = PORTC;
						uint8_t save_d = PORTD;
						
						TesterSet28(&mask, 0);
						if (!TesterTest28(&mask, 0)) {
							MSG("fail-1");
							failureCnt++;
						} else {
							TesterSet28(0, &mask);
							if (!TesterTest28(0, &mask)) {
								MSG("fail-2");
								failureCnt++;
							} else {
								MSG("success");
							}
						}
						PORTA = save_a;
						PORTB = save_b;
						PORTC = save_c;
						PORTD = save_d;						
					}
				}				
				break;
				
			case CMD_TEST_OC_16:	// (mask[2]) ��������� ��� ���� �������� �������� � �������� �����������
				break;
				
			case CMD_TEST_OC_24:	// (mask[3]) ��������� ��� ���� �������� �������� � �������� �����������
				break;
				
			case CMD_TEST_OC_28:	// (mask[4]) ��������� ��� ���� �������� �������� � �������� �����������
				break;
				
			case CMD_REPEAT_PULSE: // (uint16) ��������� ��������� ������� ��������� ���
				{
					uint16_t value;
					pgm_read_val16(&value);
					
					for (uint16_t i = 0; i < value; i++) {
						if (lastPulse < 0x80) {
							TesterSetPin(lastPulse, 0);
							_delay_us(1);
							TesterSetPin(lastPulse, 1);	
						} else {
							TesterSetPin(lastPulse-0x80, 1);
							_delay_us(1);
							TesterSetPin(lastPulse-0x80, 0);					
						}
					}
				}
				break;
				
			case CMD_END:				// ������� ��������� �����
				return failureCnt == 0;
				
			case CMD_PRINT_14:
				TesterDebugStatus(14);
				break;
		}
	}
}


bool TestLogic(uint8_t type) {
	if (type == TYPE_TTL) {
		ptr = (uint8_t*)&LOGIC_DATA_TTL[0];
	} else if (type == TYPE_CMOS) {
		ptr = (uint8_t*)&LOGIC_DATA_CMOS[0];
	}
	testResultName[0] = 0;
	bool result = false;
	while (true) {
		uint8_t cmd = pgm_read_byte(ptr);
		if (cmd == CMD_END) {
			break;
		}
		if (TestData()) {
			result = true;
			uint8_t len = strlen(testResultName);
			if (len > 0) {
				testResultName[len] = ',';
				testResultName[len+1] = 0;
			}
			strcat(testResultName, chipName);
		}
	}
	TesterReset(false);
	
	return result;
}

char* GetDeviceName() {
	return testResultName;
}
