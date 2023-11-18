#include "TMC4361_CFG.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef  huart1;

TMC4361TypeDef tmc4361List[1];
  
extern void Delay_Nus(unsigned int nus);


void TMC4361A_Print_Reg(void * reg,uint16_t charlen,uint8_t* regname)
{
	uint8_t * r=(uint8_t*)reg;
  uint8_t d;
  uint8_t dout[2];

  if(regname!=NULL)
  {
    for(uint16_t i=0;regname[i];i++)
    {
      HAL_UART_Transmit(&huart1,&regname[i],1,0xFFFF);
    }
    HAL_UART_Transmit(&huart1,(uint8_t*)":\r\n",3,0xFFFF);
  }


	for(uint16_t i=0;i<charlen;i++)
	{
      d=r[i]/16;
      switch (d)
      {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
        dout[0]=d+'0';
        break;
      
      case 10:
        dout[0]='A';
        break;
      case 11:
        dout[0]='B';
        break;
      case 12:
        dout[0]='C';
        break;
      case 13:
        dout[0]='D';
        break;
      case 14: 
        dout[0]='E';
        break;
      case 15:
        dout[0]='F';
        break;

    }

      d=r[i]%16;
      switch (d)
      {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
        dout[1]=d+'0';
        break;
      
      case 10:
        dout[1]='A';
        break;
      case 11:
        dout[1]='B';
        break;
      case 12:
        dout[1]='C';
        break;
      case 13:
        dout[1]='D';
        break;
      case 14: 
        dout[1]='E';
        break;
      case 15:
        dout[1]='F';
        break;
    }
      HAL_UART_Transmit(&huart1,dout,2,0xFFFF);
      HAL_UART_Transmit(&huart1,(uint8_t*)" ",1,0xFFFF);
	}
	HAL_UART_Transmit(&huart1,(uint8_t*)"\r\n",2,0xFFFF);
}



/*
  请在这里实现 SPI通信 
*/
void SpiWR_0(uint8_t * data)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);
	Delay_Nus(20);
	HAL_SPI_TransmitReceive(&hspi1,data,data,5,0xFFFF);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);
	Delay_Nus(20);
}

/*
  请在这里实现 中断 触发 捕捉 
*/

bool TMC4361_target_flag[2]={false,false};
bool TMC4361_event_flag[2]={false,false};

bool target0=false;


void EXTI1_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
  case GPIO_PIN_1://电机到达
    TMC4361_target_flag[0]=true;
    break;
  
  default:
    break;
  }
    
}

/*
  
*/
bool GetIntr_0(TMC4361TypeDef * tmc,uint16_t timeout,uint32_t e)
{
  uint16_t t=0;
  uint32_t event;
  while(1)
  {
    if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)==tmc->intrPol)
    {
      event=TMC4361_Event_Read(tmc);

      if(event&e)
      {
        TMC4361_Event_Clear(tmc,e);
        TMC4361_Event_Clear(tmc,e);
        printf("事件触发成功:%d\r\n",e);
        printf("电机运动完成\r\nTg:%d\r\nPs:%d\r\nEPs:%d\r\n",TMC4361_GetTargetPosition(tmc),TMC4361_GetPosition(tmc),TMC4361_GetEncPosition(tmc));
        return true;
      }
      else
      {
         printf("意外的事件触发:%d\r\n",e);
      }
    }

    HAL_Delay(10);
    if(++t>=timeout)
    {
      printf("事件触发超时:%d\r\n",e);
      return false;
    }
  }
}


bool GetTarget_0(TMC4361TypeDef * tmc,uint16_t timeout)
{
  uint16_t t=0;
  // printf("----------------------------------------\r\n");
  // printf("新的电机运动\r\nTg:%d\r\nPs:%d\r\nEPs:%d\r\n",TMC4361_GetTargetPosition(tmc),TMC4361_GetPosition(tmc),TMC4361_GetEncPosition(tmc));
  TMC4361_target_flag[0]=false;
  while(1)
  {
    if(TMC4361_target_flag[0]==true)
    {
      HAL_NVIC_DisableIRQ(EXTI1_IRQn);
      HAL_Delay(10);
      //  printf("电机运动完成\r\nTg:%d\r\nPs:%d\r\nEPs:%d\r\n",TMC4361_GetTargetPosition(tmc),TMC4361_GetPosition(tmc),TMC4361_GetEncPosition(tmc));
      //  printf("----------------------------------------\r\n");
      HAL_NVIC_EnableIRQ(EXTI1_IRQn);
      TMC4361_target_flag[0]=false;
      return true;
    }

    HAL_Delay(10);
    
    if(++t>=timeout)
    {  
      // printf("电机运动超时\r\nTg:%d\r\nPs:%d\r\nEPs:%d\r\n",TMC4361_GetTargetPosition(tmc),TMC4361_GetPosition(tmc),TMC4361_GetEncPosition(tmc));
      // printf("----------------------------------------\r\n");      
      return false;
    }

  }
}




/*
再这个函数内 按顺序实现 电机的初始化 和配置工作
*/
void TMC4361A_CFG_INIT(void)
{
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);//start信号

  tmc4361List[0].spi.spiHd=&hspi1;
  tmc4361List[0].spi.spiWR=SpiWR_0;

  tmc4361List[0].intrPol=0;//中断信号为低电平
  tmc4361List[0].targetPol=1;//到达信号为高电平

  tmc4361List[0].getInter=GetIntr_0;
  tmc4361List[0].getReached=GetTarget_0;


  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pin : PD1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  TMC4361A_Init(&tmc4361List[0]);


//开始配置之前先 配置引脚输出
  TMC4361A_OUTPUT_PIN_REG output_pin_reg={0};

  output_pin_reg.output_pin_mode=0;//设为推挽输出

  output_pin_reg.intr_pol=tmc4361List[0].intrPol;//设为 中断触发为低电平

  output_pin_reg.target_reached= tmc4361List[0].targetPol;// 设为 到达触发为高电平

  TMC4361A_Print_Reg(&output_pin_reg,sizeof(TMC4361A_OUTPUT_PIN_REG),"outputpin_reg");
  TMC4361A_Set_OutputPin_Reg(&tmc4361List[0],output_pin_reg);

  output_pin_reg=TMC4361A_Get_OutputPin_Reg(&tmc4361List[0]);
  TMC4361A_Print_Reg(&output_pin_reg,sizeof(TMC4361A_OUTPUT_PIN_REG),NULL);

  //第一步 配置 编码器和电机参数  内部斜坡发生器

  
  TMC4361A_STEP_REG setpReg={0};
  setpReg.setp_fs_per_rev=200;//360° 需要走200整步
  setpReg.setp_per_fs=0;//细分256微步
  setpReg.step_tr_tolerance=256;

  TMC4361A_Print_Reg(&setpReg,sizeof(TMC4361A_STEP_REG),"setpReg");
  TMC4361A_Set_Step_Reg(&tmc4361List[0],setpReg);

  setpReg=TMC4361A_Get_Step_Reg(&tmc4361List[0]);
  TMC4361A_Print_Reg(&setpReg,sizeof(TMC4361A_STEP_REG),NULL);


  TMC4361A_ENC_REG encReg={0};

  encReg.enc_const_manual=false;
  encReg.enc_in_mode =0x00;//ABN增量编码器
  encReg.enc_in_diff_disable=true;

  encReg.enc_in_invert_dir=true;

  encReg.enc_in_res=1600;
  encReg.enc_vmean_wait=128;
  encReg.enc_vmean_filter=7;
  encReg.enc_vmean_int=256;

  encReg.enc_clear_on_n=false;

  TMC4361A_Print_Reg(&encReg,sizeof(TMC4361A_ENC_REG),"encreg");
  TMC4361A_Set_Enc_Reg(&tmc4361List[0],encReg);

  encReg=TMC4361A_Get_Enc_Reg(&tmc4361List[0]);
  TMC4361A_Print_Reg(&encReg,sizeof(TMC4361A_ENC_REG),NULL);


  //斜坡发生器
  TMC4361A_RAMP_REG rampReg={0};

  rampReg.ramp_dir_setup_time=600;
  rampReg.ramp_stp_length_add=300;

  rampReg.ramp_profile_mode=2;//s


  rampReg.ramp_sd_indirect_control=true;
  rampReg.ramp_sdin_mode=0;//内部步进控制 使用内部斜坡发生器
  rampReg.ramp_stdby_clk_pin_assignment=0;//STDBY_CLK 待机引脚信号。//该引脚没有使用

  rampReg.ramp_pol_dir_out=false;

  rampReg.ramp_pol_dir_in=false;
  rampReg.ramp_reverse_motor_dir=false;
  rampReg.ramp_step_inactive_pol=false;
  rampReg.ramp_toggle_step=false;
 
  rampReg.astart=0;
  rampReg.dfinal=0;


  rampReg.ramp_amax=51200<<2;//AMAX
  rampReg.ramp_dmax=51200<<2;//DMAX

  rampReg.ramp_bow1=5120;
  rampReg.ramp_bow2=2560;
  rampReg.ramp_bow3=2560;
  rampReg.ramp_bow4=5120;
 
  TMC4361A_Print_Reg(&rampReg,sizeof(TMC4361A_RAMP_REG),"rampReg");
  TMC4361A_Set_Ramp_Reg(&tmc4361List[0],rampReg);

  rampReg=TMC4361A_Get_Ramp_Reg(&tmc4361List[0]);
  TMC4361A_Print_Reg(&rampReg,sizeof(TMC4361A_RAMP_REG),NULL);



  //第二步 配置 SPI输出 以及事件寄存器
  TMC4361A_SPIOUT_REG spiReg={0};

  spiReg.spi_out_enable=true;
  spiReg.spi_cover_done_only_for_covers=true;
  spiReg.spi_auto_cover_en=false;
  spiReg.spi_val_transfer_en=true;
  spiReg.spi_disable_polling=false;

  spiReg.spi_out_forma=0x0C;
  spiReg.spi_out_data_len=0;

  spiReg.spi_out_high_time=4;
  spiReg.spi_out_low_time=4;
  spiReg.spi_out_block_time=8;

  spiReg.spi_out_poll_block_mult=2;//
  TMC4361A_Set_SpiOut_Reg(&tmc4361List[0],spiReg);
  TMC4361A_Print_Reg(&spiReg,sizeof(TMC4361A_SPIOUT_REG),"spi_reg");

  spiReg=TMC4361A_Get_SpiOut_Reg(&tmc4361List[0]);
  TMC4361A_Print_Reg(&spiReg,sizeof(TMC4361A_SPIOUT_REG),NULL);

//事件配置
  TMC4361A_EVENT_REG eventReg={0};
  eventReg.event_clrar_conf=0x02000000;
  eventReg.event_events=0x0;
  eventReg.event_intr_conf=0x02000000;//打开覆盖数据报中断 关闭位置到达中断
  eventReg.event_intr_pol=false;
  eventReg.event_spi_status_selection=0x00;
  eventReg.event_status_flags=0x00;

  TMC4361A_Print_Reg(&eventReg,sizeof(TMC4361A_EVENT_REG),"eventReg");
  TMC4361A_Set_Event_Reg(&tmc4361List[0],eventReg);

  eventReg=TMC4361A_Get_Event_Reg(&tmc4361List[0]);
  TMC4361A_Print_Reg(&eventReg,sizeof(TMC4361A_EVENT_REG),NULL);

  HAL_Delay(1000);
  //第三步 配置 后置电机驱动寄存器

   TMC4361A_REARDRIVE_REG rearDriveReg[]=
 {
	{.adder=0x00,.data=0x00},//
	{.adder=0x6c,.data=0x10223},//斩波器和驱动器配置
  {.adder=0x10,.data=0x50A0A},//驱动电流等
	{.adder=0xFF,.data=0x00}
 };

 TMC4361A_Set_RearDrive_Reg(&tmc4361List[0],rearDriveReg);
	
  
 //如果需要校准 请在这里进行

  bool caliRt=TMC4361_SimpleCali_Start(&tmc4361List[0]);
  if(caliRt!=TM4361_CALI_EQUAL)
  {
      HAL_UART_Transmit(&huart1,(uint8_t*)"定标错误!!!\r\n",18,0xFFFF);
      TMC4361_SimpleCali_End(&tmc4361List[0]);
  }
  else
  {
    HAL_Delay(10);
    printf("电机校准结束,偏差为%d\r\n",TMC4361_SimpleCali_End(&tmc4361List[0]));
  }

  
 //校准结束


// 四-1  设置速度环
  TMC4361A_CLOSEDLOOP_SPEED_REG closedLoopSpeedReg ={0};
  closedLoopSpeedReg.cl_velocity_mode_en=true;
  closedLoopSpeedReg.cl_vlimit_en=true;

// PID_I_CLIP ≤ PID_DV_CLIP / PID_I

  closedLoopSpeedReg.cl_vmax_calc_i=50;//pid_i 
  closedLoopSpeedReg.cl_vmax_calc_p=1000;

  closedLoopSpeedReg.pid_dv_clip=50000;
  closedLoopSpeedReg.pid_i_clip=1000;



  TMC4361A_Print_Reg(&closedLoopSpeedReg,sizeof(TMC4361A_CLOSEDLOOP_SPEED_REG),"clspeedreg");
  TMC4361A_Set_ClosedLoop_Speed_Reg(&tmc4361List[0],closedLoopSpeedReg);

  closedLoopSpeedReg=TMC4361A_Get_ClosedLoop_Speed_Reg(&tmc4361List[0]);
  TMC4361A_Print_Reg(&closedLoopSpeedReg,sizeof(TMC4361A_CLOSEDLOOP_SPEED_REG),NULL);




//电流 和反电势补偿 是可选的 不是必须
//电流配置
  TMC4361A_CLOSEDLOOP_CUR_REG closeLoopCurReg={0};

  closeLoopCurReg.cl_cur_boost_after_scale_en=false;
  closeLoopCurReg.cl_cur_boost_on_acc_scale_en=false;
  closeLoopCurReg.cl_cur_boost_on_dec_scale_en=false;
  closeLoopCurReg.cl_cur_freewheeling_scale_en=false;
  closeLoopCurReg.cl_cur_hold_scale_en=false;
  closeLoopCurReg.cl_cur_sec_scale_en=false;
  closeLoopCurReg.cl_cur_drive_scale_en=false;


  closeLoopCurReg.cl_cur_scale_en=true;//如果使能 以上6个自己设置

  closeLoopCurReg.cl_imin=3;
  closeLoopCurReg.cl_imax=20;

  closeLoopCurReg.cl_start_up=50;

  closeLoopCurReg.cl_start_down=255;//设为0 自动加载cl——beat的值


  closeLoopCurReg.cl_up_scale_delay=200;
  closeLoopCurReg.cl_dn_scale_delay=100000;

  closeLoopCurReg.cl_stdby_delay=2000;


  TMC4361A_Print_Reg(&closeLoopCurReg,sizeof(TMC4361A_CLOSEDLOOP_CUR_REG),"clcurreg");
  TMC4361A_Set_ClosedLoop_Cur_Reg(&tmc4361List[0],closeLoopCurReg);

  closeLoopCurReg=TMC4361A_Get_ClosedLoop_Cur_Reg(&tmc4361List[0]);
  TMC4361A_Print_Reg(&closeLoopCurReg,sizeof(TMC4361A_CLOSEDLOOP_CUR_REG),NULL);

//反电动势配置
  TMC4361A_CLOSEDLOOP_BACKEMF_REG closedLoopBackEmfReg={0};
  closedLoopBackEmfReg.cl_backEmf_en=false;
  closedLoopBackEmfReg.cl_gamma=255;
  closedLoopBackEmfReg.cl_vadd=0xFF;
  closedLoopBackEmfReg.cl_vmin=0x00;

  TMC4361A_Print_Reg(&closedLoopBackEmfReg,sizeof(TMC4361A_CLOSEDLOOP_BACKEMF_REG),"clbackemf");
  TMC4361A_Set_ClosedLoop_BackEmf_Reg(&tmc4361List[0],closedLoopBackEmfReg);

  closedLoopBackEmfReg=TMC4361A_Get_ClosedLoop_BackEmf_Reg(&tmc4361List[0]);
  TMC4361A_Print_Reg(&closedLoopBackEmfReg,sizeof(TMC4361A_CLOSEDLOOP_BACKEMF_REG),NULL);
  

    //第四步 配置闭环相关设置
  TMC4361A_CLOSEDLOOP_REG closedLoopReg={0};

  closedLoopReg=TMC4361A_Get_ClosedLoop_Reg(&tmc4361List[0]);

  closedLoopReg.cl_beta=255;//最佳性能 
  //closedLoopReg.cl_offset=0x00;

  closedLoopReg.cl_tolerance=48;
  closedLoopReg.cl_delta_p=65536;
  closedLoopReg.cl_cycle=5;

  closedLoopReg.cl_enable=true;

  TMC4361A_Print_Reg(&closedLoopReg,sizeof(TMC4361A_CLOSEDLOOP_REG),"clreg");
  TMC4361A_Set_ClosedLoop_Reg(&tmc4361List[0],closedLoopReg);

  closedLoopReg=TMC4361A_Get_ClosedLoop_Reg(&tmc4361List[0]);
  TMC4361A_Print_Reg(&closedLoopReg,sizeof(TMC4361A_CLOSEDLOOP_REG),NULL);


    //斜坡发生器
  //TMC4361A_RAMP_REG rampReg={0};

  rampReg.ramp_dir_setup_time=0;
  rampReg.ramp_stp_length_add=14;

  rampReg.ramp_profile_mode=2;//s


  rampReg.ramp_sd_indirect_control=true;
  rampReg.ramp_sdin_mode=0;//内部步进控制 使用内部斜坡发生器
  rampReg.ramp_stdby_clk_pin_assignment=0;//STDBY_CLK 待机引脚信号。//该引脚没有使用

  rampReg.ramp_pol_dir_out=false;

  rampReg.ramp_pol_dir_in=false;
  rampReg.ramp_reverse_motor_dir=false;
  rampReg.ramp_step_inactive_pol=false;
  rampReg.ramp_toggle_step=false;
 

  rampReg.ramp_direct_acc_val_en=1;
  rampReg.ramp_direct_bow_val_en=1;

  rampReg.astart=0;
  rampReg.dfinal=0;


  rampReg.ramp_amax=10000<<2;//AMAX
  rampReg.ramp_dmax=10000<<2;//DMAX

  rampReg.ramp_bow1=500;
  rampReg.ramp_bow2=25;
  rampReg.ramp_bow3=25;
  rampReg.ramp_bow4=500;
 
  TMC4361A_Print_Reg(&rampReg,sizeof(TMC4361A_RAMP_REG),"rampReg");
  TMC4361A_Set_Ramp_Reg(&tmc4361List[0],rampReg);

  rampReg=TMC4361A_Get_Ramp_Reg(&tmc4361List[0]);
  TMC4361A_Print_Reg(&rampReg,sizeof(TMC4361A_RAMP_REG),NULL);

}

  


