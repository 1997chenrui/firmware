#include "off_32.h"



void off_graph(DriverInfo* Info,hole_angel_list *list)
{
    DriverInfo *info = Info;
    float a1,a2;
    a1 = info->m_s2s;
    a2 = info->m_l2l;
    
    float off_t =0.0;
    list[0].hole = 0;
    list[0].angel = info->m_z2s;
    for(char hole =1; hole<(info->m_max_hole); hole++)
    {
        list[hole].hole = hole;
        if((info->m_sector_hole >1) && (hole % (info->m_sector_hole)) == 0)
        {
           off_t = a2;
        }
        else
        {
           off_t = a1;
        }
        list[hole].angel = list[hole -1].angel + off_t;
    }
}

static double Round(double dVal, short iPlaces) {
    double dRetval;
    double dMod = 0.0000001;
    if (dVal<0.0) dMod=-0.0000001;
    dRetval=dVal;
    dRetval+=(5.0/pow(10.0,iPlaces+1.0));
    dRetval*=pow(10.0,iPlaces);
    dRetval=floor(dRetval+dMod);
    dRetval/=pow(10.0,iPlaces);
    return(dRetval);
}



/*逆时针开始  一个扇区为密集区+两个间隙*/
PosInfo GetDetInfo_Purple_32proxity(float angle,DriverInfo* Info)
{
//    float m_n2y;//两个光源孔的距离
//	float m_z2s;//豁口到第一个孔的距离
//	float m_l2l;//扇区之间的距离
//	float m_s2s;//扇区内孔之间的距离
//	float m_l2s;//豁口到最后一个孔的距离
//	char  m_max_hole;//孔的数量
//	char  m_sector_hole;//每个扇区多少个孔
    const char A =2;
    PosInfo result;
    DriverInfo *info = Info;
	double q0_Angel = fmod(angle,360);//倒转定标角度
    double hole =0.0;
    double hole_integer = 0.0;
    double hole_decimal = 0.0;
    double off =0.0;
    double x0,x1,x2,x3,x4,x5,x6,x7,a1,a2,b1;
    x0 =  Round(info->m_z2s,A);
    x1  = Round(info->m_z2s +info->m_s2s*6,A);
    x2  = Round((info->m_z2s +info->m_s2s*6 +info->m_l2l+info->m_l2l),A);
    x3  = Round((info->m_z2s +(info->m_s2s*6)*2 +info->m_l2l+info->m_l2l),A);
    x4  = Round(info->m_z2s +(info->m_s2s*6)*2 +(info->m_l2l+info->m_l2l)*2,A);
    x5  = Round(info->m_z2s +(info->m_s2s*6)*3 +(info->m_l2l+info->m_l2l)*2,A);
    x6  = Round(info->m_z2s +(info->m_s2s*6)*3 +(info->m_l2l+info->m_l2l)*3,A);
    x7  = Round(info->m_z2s +(info->m_s2s*6)*4 +(info->m_l2l+info->m_l2l)*3,A);
    a1 = Round((info->m_s2s),A);
    a2 = Round((info->m_l2l),A);
    b1 = Round((info->m_z2s),A);
//    /*判断定标角落定区间*/
    if(q0_Angel >x0 && q0_Angel <= x1)//1
    {
        hole = 1.0 *q0_Angel/a1 - b1/a1;
    }
    else if(q0_Angel >x1 && q0_Angel <= x2)//2
    {
       hole = 1.0*q0_Angel/a2 + 6.0-((b1+6*a1)/a2);
    }
    else if(q0_Angel >x2 && q0_Angel <= x3)//3
    {
       hole = 1.0 *q0_Angel/a1 +2.0- (b1 +2*a2)/(a1);
    }
    else if(q0_Angel > x3 &&q0_Angel <=x4)//4
    {
       hole = 1.0*q0_Angel/a2 + 12.0- ((b1+12*a1)/(a2));
    }
    else if(q0_Angel >x4 && q0_Angel <=x5)//5
    {
        hole = 1.0 *q0_Angel/a1 + 16.0-x4/a1;
    }
    else if(q0_Angel > x5 &&q0_Angel <=x6)//6
    {
        hole = 1.0*q0_Angel/(a2) + 22.0 - x5/a2;
    }
    else if(q0_Angel >x6&& q0_Angel <=x7 )//7
    {
         hole = 1.0 *q0_Angel/(a1) + 24.0 - x6/a1;
    }
    else if(q0_Angel > x7)//31  -0 //8
    {
        hole = 1.0*q0_Angel/(a2) +30.0 -x7/a2;
    }
    else if(q0_Angel >=0 && q0_Angel<=x0)
    {
        hole = 1.0 *(q0_Angel +360.0)/(a2) +30.0 -x7/a2;
    }
    
    hole = Round(hole,3);
    hole_decimal = modf(hole,&hole_integer);
//    //孔距1000细分  每0.001 -   0.01°
    if(hole_decimal <= 0.001)
    {
        if(hole_integer >31)
            result.hole = 0;
        else
            result.hole =  (int)hole_integer;
    }
    else
    {
        hole_integer +=1;
        if(hole_integer >31)
        {
            result.hole =0;
           
        }
        else
            result.hole =  (int)hole_integer;
    }
//    /*孔数求角度*/
    q0_Angel = Round(q0_Angel,2);
    if(result.hole >0 && result.hole <=6)
    {
        off = 10.0*result.hole +b1 - q0_Angel;
    }
    else if(result.hole >6 &&result.hole <=8)
    {
        off = 15.0*result.hole +(x1-90.0) -q0_Angel;
    }
    else if(result.hole >8 &&result.hole <=14)
    {
        off = 10.0*result.hole+(x2-80.0) - q0_Angel;
    }
    else if(result.hole >14 &&result.hole<=16)
    {
        off = 15.0 *result.hole +(x3-210.0)-q0_Angel;
    }
    else if(result.hole >16 &&result.hole <=22)
    {
        off = 10.0*result.hole+(x4-160.0) - q0_Angel;
    }
    else if(result.hole >22 &&result.hole <=24)
    {
        off = 15.0*result.hole+(x5-330.0) -q0_Angel;
    }
    else if(result.hole >24 && result.hole <=30)
    {
        off = 10.0*result.hole+(x6 - 240.0) - q0_Angel;
    }
    else if(hole_integer >30 && hole_integer <=32)
    {
        off = 15.0*hole_integer+(x7 - 450.0) - q0_Angel;
        if(off >=360.0)
        {
            off = off -360;
        }
    }
    
    result.offset =  off;
    return result;    
}
PosInfo GetDetInfo_Yellow_32proxity(float angle,DriverInfo* Info)
{
    return GetDetInfo_Purple_32proxity(angle-(Info->m_n2y),Info);
}
PosInfo GetHloe_Pos(float angle,DriverInfo* Info,const hole_angel_list *list)
{
    //    float m_n2y;//两个光源孔的距离
//	float m_z2s;//豁口到第一个孔的距离
//	float m_l2l;//扇区之间的距离
//	float m_s2s;//扇区内孔之间的距离
//	float m_l2s;//豁口到最后一个孔的距离
//	char  m_max_hole;//孔的数量
//	char  m_sector_hole;//每个扇区多少个孔
    //const char A =2;
    PosInfo result;
    DriverInfo *info = Info;
    const hole_angel_list *List_t = list;
	double q0_Angel = fmod(angle,360);//倒转定标角度
    double q0_Angel_tmp =0.0;
    double hole =0.0;
    double hole_integer = 0.0;
    double hole_decimal = 0.0;
    float off =0.0;
    /**
    先分段，判断盘片分成几段式函数解析式
    */
    unsigned char section = 0;
    double a1,a2; 
    section = (info->m_l2l >0)?(unsigned char)(info->m_max_hole)*2 / (info->m_sector_hole):1;
    double b[section +1];
    char c[section];
   // if(O == 0)
    {
        b[0] = info->m_z2s;
        for(char i=1;i<section+1;i++)
        {
            if(i % 2 == 0)
            {
                b[i] = b[i-1] + info->m_l2l;
            }
            else
            {
                b[i] = b[i-1] +(info->m_s2s *(info->m_sector_hole -1));
            }
            b[i] = b[i];
        }
        a1 = info->m_s2s;
        a2 = info->m_l2l;
        for(char i =0;i<section +1;i++)
        {
            if(i % 2 == 0)
            {
                c[i] =  info->m_sector_hole *i/2;
            }
            else
            {
                c[i] = info->m_sector_hole *i/2 + info->m_sector_hole - 3;
            }
        }
    }
    
    if(section == 1)
    {
       hole = 1.0 *q0_Angel/a1 - b[0]/a1;
       if(hole <0)
       {
           hole_decimal = modf(hole,&hole_integer);
           off = List_t[(char)hole_integer].angel - q0_Angel;
           result.hole =0;
       }
       else
       {
            hole_decimal = modf(hole,&hole_integer);
            off = List_t[(char)hole_integer].angel - q0_Angel;
            if(off <-0.005 || off > 0.005)
            {
                hole_integer +=1;
                if(hole_integer >(info->m_max_hole -1))
                {
                    result.hole =0;
                }
                else
                    result.hole =  (int)hole_integer;
            }
            else
                result.hole =  (int)hole_integer;
       }
    }
    else
    {
        //计算通项式
        float dta = Round((b[1] - b[0]),2);
        float ka = (float)(info->m_sector_hole -1)/dta;
        float kb1 = (float)((info->m_sector_hole-1)*b[0])/dta;
        float kb2 = (info->m_sector_hole) - b[2] *ka;
        float dtb = fabs(kb2) - fabs(kb1);//Round(fabs(kb2) - fabs(kb1),2);
        
        float kc = 1.0/a2;
        float kbc1 = (info->m_sector_hole -1) - b[1]/a2;
        float kbc2 = (2*info->m_sector_hole - 1) - b[3]/a2;
        float dtcb = fabs(kbc2) - fabs(kbc1);//Round(fabs(kbc2) - fabs(kbc1),2);
        for(char i=0;i< section;i++)
        {
           if(i <(section -1)&& q0_Angel >b[i] && q0_Angel<=b[i+1])
           {
               if(i % 2 == 0)
               {
                    hole = ka * q0_Angel - (kb1 + (i/2)*dtb); 
               }
               else
               {
                    hole = kc * q0_Angel + (kbc1 +(i-1)/2 * dtcb);
               }
               break;
           }
           else if(q0_Angel >b[section -1] || q0_Angel <b[0])
           {
               q0_Angel_tmp = q0_Angel;
               if(q0_Angel >=0 &&q0_Angel <b[0])
                   q0_Angel_tmp +=360.0;
               hole = kc * q0_Angel_tmp + (kbc1 +(section-1-1)/2 * dtcb);break;
           }
        }
    }
//    hole_decimal = modf(hole,&hole_integer);
//    off = List_t[(char)hole_integer].angel - q0_Angel;
//    if(off <-0.005 || off > 0.005)
//    {
//        hole_integer +=1;
//        if(hole_integer >(info->m_max_hole -1))
//        {
//            result.hole =0;
//        }
//        else
//            result.hole =  (int)hole_integer;
//    }
//    else
//        result.hole =  (int)hole_integer;
    
    if(result.hole == 0)
    {
        if(q0_Angel > List_t[(char)(info->m_max_hole -1)].angel  &&  q0_Angel <=360)
            off = List_t[(char)result.hole].angel +(360.0 - q0_Angel);
        else
            off = List_t[(char)result.hole].angel - q0_Angel;
    }
    else
        off = List_t[(char)result.hole].angel - q0_Angel;
    
    if(off >=360.0)
    {
        off = off -360.0;
    }
    result.offset =  fabs(off);
    return result;  
}
