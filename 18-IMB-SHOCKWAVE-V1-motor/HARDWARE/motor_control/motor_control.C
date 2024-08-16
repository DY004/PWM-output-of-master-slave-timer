#include "motor_control.h"
#include "gpio.h"
#include "key.h"
#include "delay.h"
#include "main.h"
#include "tim.h"

void _motor_control(void)
{
	if(KEY1 == GPIO_PIN_RESET)
	{
		HAL_Delay(20);
		if(KEY1 == GPIO_PIN_RESET)
		{
			__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_3,500);//修改占空比比较值，一直保持占空比50%
			__HAL_TIM_SET_AUTORELOAD(&htim3,500-1); //设置要输出的PWM脉冲数1000个
			HAL_TIM_Base_Start_IT(&htim3);             //启动从定时器
			HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);

			while(!KEY1);//等待按键松开
	
		}
	}
}


