#include "NTC_B3950.h"
#include "adc.h"



/* USER CODE BEGIN 1 */
/*获取ADC的值8*/
uint16_t Get_adc(uint16_t ch)
{
    HAL_ADC_Start(&hadc3);  //先开启ADC
    HAL_ADC_PollForConversion(&hadc3,1);//查询函数，查询EOC标志位。每次采样，CUP在这里都要
    //等待采样完成才能进行下一步，这段时间CUP没有干其他
    //事，所以降低了CUP使用率

    return HAL_ADC_GetValue(&hadc3);    //得到ADC的值
}

/*获取times次采样值的平均值*/

uint16_t Get_ADC_Average(uint16_t ch,uint8_t times)
{
    uint32_t ADC_Sum=0;
    uint8_t i;
    for(i=0; i<times; i++)
    {
        ADC_Sum+=Get_adc(ch);
        HAL_Delay(5);
    }
    return ADC_Sum/times;                //对times次样品值取平均，使采样值更加准确
}


//输入参数：ADC值表  ADC值
//返回值：查表后的索引号
u16 NTC_Lookup(u16 *list,u16 data)
{
    u16 middle=0;
    u16 indexL=0;
    u16 indexR=NUM-1;
    if(data>=*(list+0))
        return 0;
    if(data<=*(list+NUM-1))
        return NUM-1;

    while((indexR-indexL)>1)
    {
        middle=(indexL+indexR)>>1;
        if(data==*(list+middle))
            return middle;
        else if(data>*(list+middle))
            indexR=middle;
        else if(data<*(list+middle))
            indexL=middle;
    }
    return indexL;
}

//例如我们采集到的ADC值是3000，经过二分法查找到的索引号是20，我们就可以知道温度在NTC3435_10K[20](0℃)和NTC3435_10K[21](1℃)之间。


//输入参数：ADC表  采集的ADC值   ADC表的起始温度值(-20℃=-200)
//返回值：温度值 单位0.1℃ 例如返回值是100，对应的就是100*0.1℃=10℃。
u16 Read_NTC_Temperature(u16 *list,u16 rADC,u16 BaseValue)
{
    u16 index=0;
    u16 deta=0;
    u16 t=0;
    u16 result=0;
    if(rADC>=list[0])
        return BaseValue;
    if(rADC<=*(list+NUM-1))
    {
        result=((NUM-1)*10+BaseValue);
        return result;
    }
    index=NTC_Lookup(list,rADC);
#if NTC_HIGH_PRECISION
    deta=list[index]-list[index+1];
    t=10*(list[index]-rADC)/deta;
#endif
    result=(BaseValue+index*10+t);
    return result;
}

