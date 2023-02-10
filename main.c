#include"std_types.h"
#include"Bit_Math.h"

#include"MDIO_interface.h"
#include"MTMR_interface.h"

#include"HKPD_interface.h"
#include"HLCD_interface.h"
#include"DC_Motor.h"
#include"HSERVO.h"

#define F_CPU 8000000UL
#include <avr/delay.h>

#define SREG_REG      *((volatile u8*)0x5f)

u8 Login(void);
u8 CheckPass(u8* arr);

int main (void){
	SET_BIT(SREG_REG,7);
	MDIO_u8INIT();
	HLCD_voidINIT();
	u8 testLogin , Local_digitsHelper=1;
	u8 Local_dc_counter=0;
	u8 ReturnKeyValue,ReturnKeyValue_2;
	u8 digits[5];
	u8 dcDutyCycle = 0,dcFlag=0,servoFlag = 0;
	u8 servoAngel = 0 ,exitFlag = 0;
	while(1){
		HLCD_u8ClearDisplay();
		_delay_ms(100);
		HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_4);
		HLCD_u8SendString("Welcom to",4);
		HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_4);
		HLCD_u8SendString("Dashboard",4);
		testLogin = Login();
		if(testLogin){
			while(1){
			HLCD_u8ClearDisplay();
			_delay_ms(100);
			HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_0);
			HLCD_u8SendString("1-DC  2-Servo",0);
			HLCD_u8SetCursor(HLCD_u8_LINE_2 , HLCD_u8_Position_0);
			HLCD_u8SendString("3-Exit :",0);
			/*get the user choice*/
			HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_9);
			while(1){
			 HKPD_u8GetKey(&ReturnKeyValue);
			 if(ReturnKeyValue != HKPD_u8_NO_KEY_PRESSED){
				 if(ReturnKeyValue == '=')
					 break;
				 else{
			     HLCD_u8SendChar(ReturnKeyValue);
			     ReturnKeyValue_2 = ReturnKeyValue;
				 }
			 }
			}
			while(1){
			if(ReturnKeyValue_2=='1')
			{
				while(1){
				HLCD_u8ClearDisplay();
				_delay_ms(100);
				HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
				HLCD_u8SendString("1-the Duty Cycle",0);
				HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
				HLCD_u8SendString("2-Back :",0);
				/*drive the motor by the user's duty cycle based on PWM*/
				HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_9);
				while(1){
				HKPD_u8GetKey(&ReturnKeyValue);
				if(ReturnKeyValue != HKPD_u8_NO_KEY_PRESSED){
					if(ReturnKeyValue == '=')
						break;
					else{
						HLCD_u8SendChar(ReturnKeyValue);
						ReturnKeyValue_2 = ReturnKeyValue;
					}
				}
			}
				if(ReturnKeyValue_2 == '2'){
						dcFlag=1;
						break;
					}
				if(ReturnKeyValue_2 == '1'){
				HLCD_u8ClearDisplay();
				_delay_ms(100);
				HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
				HLCD_u8SendString("the Duty Cycle",0);
				HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
				while(1){
			    HKPD_u8GetKey(&ReturnKeyValue);
				   if(ReturnKeyValue!=HKPD_u8_NO_KEY_PRESSED){
					HLCD_u8SendChar(ReturnKeyValue);
					if(ReturnKeyValue == '='){
						break;
					}
					else{
						digits[Local_dc_counter++]=ReturnKeyValue-48;
					}
				}
			}
				for(u8 i =0 ; i< Local_dc_counter-1 ; i++){
					Local_digitsHelper*=10;
				}
				for(u8 i=0 ; i < Local_dc_counter ; i++){
					dcDutyCycle+= (digits[i]*Local_digitsHelper);
					Local_digitsHelper/=10;
				}
				if(Local_dc_counter > 3 || dcDutyCycle > 100){
					HLCD_u8ClearDisplay();
					_delay_ms(100);
					HLCD_u8SendString("Wrong Input",0);
					Local_dc_counter = 0;
					Local_digitsHelper = 1;
					dcDutyCycle = 0;
					_delay_ms(2000);
				}
				else{
					HDC_StartMotor(dcDutyCycle);
					Local_dc_counter = 0;
					Local_digitsHelper = 1;
					dcDutyCycle = 0;
					break;
				}
			}
		}
				if(dcFlag){
					dcFlag = 0;
					break;
				}
				HLCD_u8ClearDisplay();
				_delay_ms(100);
				HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_0);
				HLCD_u8SendString("1- Stop Motor",0);
				HLCD_u8SetCursor(HLCD_u8_LINE_2 , HLCD_u8_Position_0);
				HLCD_u8SendString("2- Back: ",0);
					while(1){
					HKPD_u8GetKey(&ReturnKeyValue);
					if(ReturnKeyValue!=HKPD_u8_NO_KEY_PRESSED){
						if(ReturnKeyValue == '=')
							break;
						else{
						HLCD_u8SendChar(ReturnKeyValue);
						ReturnKeyValue_2 = ReturnKeyValue;
					}
				}
			}
						if(ReturnKeyValue_2 == '1'){
							HDC_StopMotor();
						}
						else if(ReturnKeyValue_2 == '2'){
							HDC_StopMotor();
							break;
						}
				}
			else if(ReturnKeyValue_2=='2')
			{
				while(1){
				   HLCD_u8ClearDisplay();
				   _delay_ms(100);
				   HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
				   HLCD_u8SendString("1-Set Angel ",0);
				   HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
				   HLCD_u8SendString("2-Back : ",0);
				   while(1){
				   HKPD_u8GetKey(&ReturnKeyValue);
				   if(ReturnKeyValue != HKPD_u8_NO_KEY_PRESSED){
				   	if(ReturnKeyValue == '=')
				   		break;
				   	else{
				   		HLCD_u8SendChar(ReturnKeyValue);
				   		ReturnKeyValue_2 = ReturnKeyValue;
				   	}
				  }
			  }
				   if(ReturnKeyValue_2 == '2'){
					  servoFlag = 1;
				      break;
				   }

				   if(ReturnKeyValue_2 == '1'){
						HLCD_u8ClearDisplay();
						_delay_ms(100);
						HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
						HLCD_u8SendString("Angel (0 to 180)",0);
						HLCD_u8SetCursor(HLCD_u8_LINE_2,HLCD_u8_Position_0);
						while(1){
					    HKPD_u8GetKey(&ReturnKeyValue);
						   if(ReturnKeyValue!=HKPD_u8_NO_KEY_PRESSED){
							HLCD_u8SendChar(ReturnKeyValue);
							if(ReturnKeyValue == '='){
								break;
							}
							else{
								digits[Local_dc_counter++]=ReturnKeyValue-48;
							}
						}
					}
						for(u8 i =0 ; i< Local_dc_counter-1 ; i++){
							Local_digitsHelper*=10;
						}
						for(u8 i=0 ; i < Local_dc_counter ; i++){
							servoAngel+= (digits[i]*Local_digitsHelper);
							Local_digitsHelper/=10;
						}
						if(Local_dc_counter > 3 || servoAngel > 180){
							servoAngel = 0;
							Local_dc_counter = 0;
							Local_digitsHelper = 1;
							HLCD_u8ClearDisplay();
							_delay_ms(100);
							HLCD_u8SendString("Wrong Input",0);
							_delay_ms(2000);
						}
						else{
							HSV_SetAngle(servoAngel);
							servoAngel = 0;
							Local_dc_counter = 0;
							Local_digitsHelper = 1;
						}
				   }
			 }
		  }
			else if(ReturnKeyValue_2=='3'){
				exitFlag = 1;
				break;
			}
			else{
				HLCD_u8ClearDisplay();
				_delay_ms(100);
				HLCD_u8SendString("Wrong Choice",0);
				_delay_ms(2000);
				break;
			}
			if(servoFlag || dcFlag){
				servoFlag = 0;
				dcFlag = 0;
				break;
			}
			}
		}
			if(exitFlag)
						break;
	}

}

	return 0;
}



u8 Login(void){
	u8 ReturnKeyValue;
	u8 passFlag = 0;
	u8 counter =0;
	u8 arr[4];
	_delay_ms(3000);
	HLCD_u8ClearDisplay();
	_delay_ms(100);
	HLCD_u8SetCursor(HLCD_u8_LINE_1, HLCD_u8_Position_0);
	HLCD_u8SendString("Enter the pass",0);
	HLCD_u8SetCursor(HLCD_u8_LINE_2, HLCD_u8_Position_0);
	for(u8 i=0 ; i<3 ; i++){
			while(1)
				   {
						HKPD_u8GetKey(&ReturnKeyValue);
						if(ReturnKeyValue!=HKPD_u8_NO_KEY_PRESSED)
						{
							if(ReturnKeyValue=='=')
								break;
							else{
							HLCD_u8SetCursor(HLCD_u8_LINE_2, counter);
							HLCD_u8SendChar(ReturnKeyValue);
							_delay_ms(250);
							HLCD_u8SetCursor(HLCD_u8_LINE_2, counter);
							HLCD_u8SendChar('*');
							arr[counter++] = ReturnKeyValue;
						}
					}
				}
		if(CheckPass(&arr[0])){
			passFlag = 1;
			counter = 0;
			break;
		}
		else{
			passFlag = 0;
			counter = 0;
			HLCD_u8ClearDisplay();
			_delay_ms(100);
			HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_0);
			HLCD_u8SendString("wrong,try again",0);
			HLCD_u8SetCursor(HLCD_u8_LINE_2,0);
		}
	}
	if(!passFlag){
		HLCD_u8ClearDisplay();
		_delay_ms(100);
		HLCD_u8SetCursor(HLCD_u8_LINE_1 , HLCD_u8_Position_0);
		HLCD_u8SendString("No more trials",0);
		MDIO_u8SetPinValue(MDIO_u8_PORTB,MDIO_u8_PIN_0,MDIO_u8_HIGH);
		_delay_ms(5000);
		MDIO_u8SetPinValue(MDIO_u8_PORTB,MDIO_u8_PIN_0,MDIO_u8_LOW);
	}
	else{
		HLCD_u8ClearDisplay();
		_delay_ms(100);
		HLCD_u8SetCursor(HLCD_u8_LINE_1,HLCD_u8_Position_0);
		HLCD_u8SendString("pass is correct",0);
		_delay_ms(3000);
	}
	return passFlag;
}


u8 CheckPass(u8* arr){
	u8 flag=1;
	u8 pass[4] = {'1','2','3','4'};
	for(u8 j =0 ;j<4 ; j++){
		if(arr[j] != pass[j]){
			flag = 0;
			break;
		}
	}
	return flag;
}

