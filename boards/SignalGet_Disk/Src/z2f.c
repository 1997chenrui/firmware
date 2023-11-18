#include "z2f.h"
z2f z2f_Info[4];//初始化对应的脉冲数组   记录最靠进光源孔的位置
L4node_arr z2f_Info_pulse0[36];
L4node_arr z2f_Info_pulse1[36];
L4node_arr z2f_Info_pulse2[36];
L4node_arr z2f_Info_pulse3[36];

L4node_arr z2f_Info_time0[2];
L4node_arr z2f_Info_time1[2];
L4node_arr z2f_Info_time2[2];
L4node_arr z2f_Info_time3[2];


z2f cal_z2f_off32(z2f *me ,const DriverInfo *info,const unsigned short z2f_t)
{
    const double girth = 32768.0;//(double)girth_all;
    double OneStepAngel = girth/360.0;//精度91.02222个脉冲/1°
    double q0_Angel = z2f_t *360.0/girth;//计算定标值占多少角度
    double q1_Angel = q0_Angel - (info ->m_l2s);//最后一个孔到光源的角度
   //此处限制安装位置  不允许爪靠近光源
    
    double q2_Angel = 
    (info->m_l2l >0)?((info->m_s2s)*(info ->m_sector_hole -1 - (info->m_s2s_32_num))):(info->m_s2s);//扇区内所有孔的角度（去掉大间隙的孔数）
    double q3_Angel = info->m_l2l;//扇区的角度
    double q4_Angel = info->m_s2s_32;//扇区内大间隙内孔角度
    
    
    double q4 = 0.0;
    double q4_integer = 0.0;
    double q4_decimal = 0.0;
    
    double q5;
    double q5_integer = 0.0;
    double q5_decimal = 0.0;
    
    double q6 = 0.0;
    double q6_integer = 0.0;
    double q6_decimal = 0.0;
    z2f result;
    
    
    
    


}
/*
1217：
1.把24孔盘片每个扇区的内孔数定为1
2.适配其他带扇区距离的盘片
3.其他3个光源孔的偏移  （未测试）

*/
z2f cal_z2f_off(z2f *me ,const DriverInfo *info,const unsigned short z2f_t,double off_angel)
{
    const double girth = 32768.0;//(double)girth_all;
    double OneStepAngel = girth/360.0;//精度91.02222个脉冲/1°
    double q0_Angel = z2f_t *360.0/girth;//计算定标值占多少角度
    double q1_Angel = q0_Angel - (info ->m_l2s);//最后一个孔到光源的角度
    //此处限制安装位置  不允许爪靠近光源
    
    double q2_Angel = (info->m_l2l >0)?((info->m_s2s)*(info ->m_sector_hole -1)):(info->m_s2s);//扇区内所有孔的角度（第一个到最后一个）
    double q3_Angel = info->m_l2l;//扇区的角度
    
    double q4 = 0.0;
    double q4_integer = 0.0;
    double q4_decimal = 0.0;
    
    double q5;
    double q5_integer = 0.0;
    double q5_decimal = 0.0;
    
    double q6 = 0.0;
    double q6_integer = 0.0;
    double q6_decimal = 0.0;
    z2f result;
    
    if(q1_Angel < (q2_Angel + q3_Angel))//定标值减去豁口偏差后角度不满一个扇区+内孔
    {
        q4 = q1_Angel/(info->m_s2s);
        q4_decimal = modf(q4,&q4_integer);//分解整数和小数部分
        result.z2f_num = info->m_max_hole- 1 - (short)q4_integer;//计算该孔是几号孔
        //计算该孔到光源的脉冲个数
        q5 = q4_decimal*(info->m_s2s)*OneStepAngel;
        q5_decimal = modf(q5,&q5_integer);
        result.hole1_angel =  (short)(q4_decimal*(info->m_s2s));//第一个孔到光源的角度
        printf("hole1_angel=%d\r\n",result.hole1_angel);
    }
    else
    {
        q4 = q1_Angel/(q2_Angel+q3_Angel);
        q4_decimal = modf(q4,&q4_integer);//分解整数和小数部分
        result.z2f_num = (info->m_max_hole- 1)-((short)q4_integer*(info ->m_sector_hole));
        printf("q4_decimal = %1f\r\n",q4_decimal);

        q6 = q4_decimal * (q2_Angel+q3_Angel);
        //q6一定落在内孔之间
        q5 = q6/(info->m_s2s);
        q5_decimal = modf(q5,&q5_integer);
        result.z2f_num -=(short)q5_integer;
        //计算脉冲数
        q5 = q5_decimal*(info->m_s2s)*OneStepAngel;
        q5_decimal = modf(q5,&q5_integer);
        result.hole1_angel =  (short)(q5_decimal*(info->m_s2s));//第一个孔到光源的角度
        printf("q5_decimal=%1f\nq5_integer =%1f\r\n",q5_decimal,q5_integer);
        printf("hole1_angel=%d\r\n",result.hole1_angel);
    }
    /*以上计算出靠近光源的第一个孔到光源的脉冲整数部分和误差部分，改孔是几号孔
      以下求出其他孔的脉冲数，进行误差弥补
    */
    printf("result.z2f_num =%d\r\n",result.z2f_num);
    me->z2f_num = result.z2f_num;
    me->encoder_data[result.z2f_num].encoder_data = (short)q5_integer;
    /*计算内孔误差   扇区误差
    */
    q4 = (info->m_s2s)*OneStepAngel;q4_decimal = modf(q4,&q4_integer);
    q6 = q3_Angel*OneStepAngel;q6_decimal = modf(q6,&q6_integer);
    unsigned short q_integer4_tmp =(unsigned short)q4_integer;
    unsigned short q_integer6_tmp =(unsigned short)q6_integer;
    double q_decimal4_tmp = q4_decimal;
    double q_decimal6_tmp = q6_decimal;
    if(q3_Angel <=0.0)
    {
        q_integer6_tmp = q_integer4_tmp; 
        q_decimal6_tmp = q_decimal4_tmp;
    }
    unsigned short q_store_integer =q5_integer;
    double q_store_decimal = q5_decimal;
    /*计算和最后一个孔的孔位数目*/
    for(u8 i=(result.z2f_num +1); i<info->m_max_hole;i++)
    {
       div_t num = div((info->m_max_hole-i),(info->m_sector_hole));
       printf("quot =%d\n rem=%d\r\n",num.quot,num.rem);//
       if(num.rem ==0)//下一个紧挨着扇区
       {
            q_store_decimal += q_decimal6_tmp;
            if(q_store_decimal >1.0)
            {
                q_store_decimal -=1.0;
                q_store_integer +=1;
            }
            q_store_integer += q_integer6_tmp;
       }
       else
       {
           q_store_decimal += q_decimal4_tmp;
           if(q_store_decimal >1.0)
           {
               q_store_decimal -=1.0;
                q_store_integer +=1;
           }
            q_store_integer += q_integer4_tmp;
            
       }
        printf("q_store_integer =%d\r\n",q_store_integer);//最后一个孔到光源的脉冲
        printf("q_store_decimal:%1f\r\n",q_store_decimal);
        me->encoder_data[i].encoder_data = q_store_integer;
    }
    printf("--------------------\r\n");
    for(u8 i=0; i<result.z2f_num;i++)
    {
       div_t num = div(i,(info->m_sector_hole));
       printf("quot =%d\n rem=%d\r\n",num.quot,num.rem);//
       if(num.rem ==0)
       {
            q_store_decimal += q_decimal6_tmp;
            if(q_store_decimal >1.0)
            {
                q_store_decimal -=1.0;
                q_store_integer +=1;
            }
            q_store_integer += q_integer6_tmp;
       }
       else
       {
           q_store_decimal += q_decimal4_tmp;
           if(q_store_decimal >1.0)
           {
               q_store_decimal -=1.0;
                q_store_integer +=1;
           }
            q_store_integer += q_integer4_tmp;
       }
        printf("q_store_integer =%d\r\n",q_store_integer);//最后一个孔到光源的脉冲
        printf("q_store_decimal:%1f\r\n",q_store_decimal);
       me->encoder_data[i].encoder_data = q_store_integer;
    }
    return result;
}

void get_hole_data(z2f *me)
{
   for(u8 i =0;i<(me->max_hole);i++)
    {
       
    }
}

void z2f_Init(z2f *me,L4node_arr *encode,L4node_arr*tim,LIGHT_E type)
{
    me ->type = type;
    me->encoder_data = encode;
    me ->time_data = tim;
    memset(me->encoder_data,0,sizeof(L4node_arr)*HOLE_NUM);
   
}

unsigned short z2f_off_cal(const DriverInfo *info,const unsigned short z2f_t)
{
    const double girth = 32768.0;//(double)girth_all;
    double OneStepAngel = girth/360.0;//精度91.02222个脉冲/1°
    double q0_Angel = z2f_t *360.0/girth +(info->m_n2y);//计算定标值 +光源偏移占多少角度
    return (unsigned short)(q0_Angel * OneStepAngel);
}

void cali_time(z2f *me)
{
   me->time_data[me->z2f_num].encoder_data =  (me->encoder_data[me->z2f_num].encoder_data - 3) * 6;
   me->time_data[me->z2f_num +1].encoder_data = (me->encoder_data[me->z2f_num+1].encoder_data - 
                                                (me->encoder_data[me->z2f_num].encoder_data) - 3) * 6;
    printf("hole0time = %d\r\n",me->time_data[me->z2f_num].encoder_data);
    printf("hole1time = %d\r\n",me->time_data[me->z2f_num+1].encoder_data);  
}














    










