#ifndef BUS_CFG_H_
#define BUS_CFG_H_
/************************************************************
*
*  
*
*************************************************************/


#define  BUS_LOG						printf

#define  BUS_BRAODCAST_ID               0xff


#define  BUS_PACKAGE_MAX_LEN            10000   //每一包的最大的包长度
#define  BUS_WAITE_ACK_TIME				500     // 等待应答的时间
#define  BUS_WAITE_RETURN_TIME          1000	    // 等待Return的时间
#define  BUS_ROUTER_ONE_ADP_ID_CNT      20      //在路由功能中 一个port能挂多少个端点  


#define  BUS_ROUTER_SELF_ID             1
#define  BUS_USE_ROUTER_FUNC            0		//是否启用路由功能
#define  BUS_UART3_PC_ENABLE             0
#define  BUS_UART6_PC_ENABLE             0
#define  LOCAL_ID                       1

#endif

