#include "bsp_pid.h"
#include "math.h"
#include "protocol.h"
#include "tim.h"

//定义全局变量

_pid pid;
double pwm_value = 0.0;//加热丝的PWM的输出值
extern float tep;
/**
  * @brief  PID参数初始化
	*	@note 	无
  * @retval 无
  */
void PID_param_init()
{
    /* 初始化参数 */
//    printf("PID_init begin \n");
    pid.target_val=0.0;
    pid.actual_val=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.integral=0.0;
//		pid.Kp = 0.31;
//		pid.Ki = 0.070;
//		pid.Kd = 0.3;
//		pid.Kp = 0.21;
//		pid.Ki = 0.070;
//		pid.Kd = 0.3;
    pid.Kp = 400;//24
    pid.Ki = 0.52;
    pid.Kd = 0.07;
	pid.Out0 = -750;//PID修正值
	pid.cycle = 1500;//PID限幅
	


//#if defined(PID_ASSISTANT_EN)
//    float pid_temp[3] = {pid.Kp, pid.Ki, pid.Kd};
//    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // 给通道 1 发送 P I D 值
//#endif

}

/**
  * @brief  设置目标值
  * @param  val		目标值
	*	@note 	无
  * @retval 无
  */
void set_pid_target(float temp_val)
{
    pid.target_val = temp_val;    // 设置当前的目标值
}

/**
  * @brief  获取目标值
  * @param  无
	*	@note 	无
  * @retval 目标值
  */
float get_pid_target(void)
{
    return pid.target_val;    // 得到当前的目标值
}

/**
  * @brief  设置比例、积分、微分系数
  * @param  p：比例系数 P
  * @param  i：积分系数 i
  * @param  d：微分系数 d
	*	@note 	无
  * @retval 无
  */
void set_p_i_d(float p, float i, float d)
{
    pid.Kp = p;    // 设置比例系数 P
    pid.Ki = i;    // 设置积分系数 I
    pid.Kd = d;    // 设置微分系数 D
}

/**
  * @brief  PID算法实现
  * @param  val		实际值
  * @note 	无
  * @retval 通过PID计算后的输出
  */


float PID_realize(float temp_val)
{
	
	 float Pout,Iout,Dout;
	 float out;
	 
	
			/*传入目标值*/
//    pid.target_val=temp_val;
    /*计算目标值与实际值的误差*/
    pid.err=pid.target_val-temp_val;
    /*误差累积*/
    pid.integral+=pid.err;
	
	Pout = pid.Kp * pid.err;  //比例控制
	
	Dout = pid.Kd * (pid.err - pid.err_last);  //微分控制
	
	if(temp_val>(pid.target_val-1))   //当测量值非常接近目标值的时候加入积分控制
    {
      Iout = pid.Ki * pid.integral;           //积分控制
      out = Pout + Iout + Dout + pid.Out0; 
    }
	
	else   out  = Pout + Dout + pid.Out0;  //测量值距离目标值较远时只使用PD控制
	
	
    /*PID算法实现*/
//    pid.actual_val=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);
//	pwm_value=(pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last));

    /*返回当前实际值*/
//	pwm_value = pid.actual_val;//把累差值反馈给PWM,来慢慢的达到温度的动态平衡。
	
	/***制冷的话，需要对out取绝对值，不然out始终都是负的***/
	out = fabs(out);
	
	/*** 约束占空比的值 ***/
	if(out > pid.cycle)
	{
		out = pid.cycle;
	
	}
	else if(out < 0)
	{
		out = 0;
	
	}	
	pwm_value = out;
	TIM_SetTIM2Compare2(pwm_value);
	    /*误差传递*/
    pid.err_last=pid.err;
//	printf("此刻pwm_value的值是：%2.f\r\n",pwm_value);
//    return pid.actual_val;
}


/**
  * @brief  定时器周期调用函数
  * @param  无
	*@note 	无
  * @retval 无
  */
float set_point = 0;
int pid_status=0;

void time_period_fun()
{

    if(!pid_status)
    {
//        float val=PID_realize(tep);
//		printf("act的实际值：%f\n",pid.actual_val);
         int temps = tep;    // 上位机需要整数参数，转换一下
        set_computer_value(SEND_FACT_CMD, CURVES_CH1, &temps, 1);  

		
		// 给通道 1 发送实际值
//		set_computer_value(SEND_TARGET_CMD, CURVES_CH1, &set_point, 1);     // 给通道 1 发送目标值
//		printf("act的实际值：%f\n",val);
		
		
//		while(pid.target_val >30 && pid.target_val <120)
//		{
////			PID_realize(tep);
////			float val = PID_realize(tep);
////			int temps = val;    // 上位机需要整数参数，转换一下
//			set_computer_value(SEND_FACT_CMD, CURVES_CH1, &tep, 1); 
//		}

    }
	

}







