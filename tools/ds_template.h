//#ifndef DS_TEMPLATE_H_
//#define DS_TEMPLATE_H_


////------------------- 下面是map  的模板 ----------------------------

//#define   MAP_CREATE(cnt,keyT,valueT ,name ,container )			                                                \
//                        void name##_insert(keyT *key, valueT *value)					\
//						{																						\
//                            CPU_SR_ALLOC();                            \
//                            OS_ENTER_CRITICAL();                                            \
//                            for (int i = 0; i<cnt; i++)															\
//							{																					\
//                                if (!(container)[i].isUsed)											\
//								{																				\
//                                    (container)[i].isUsed = true;										\
//                                    (container)[i].key = *key;										\
//                                    (container)[i].value = *value;									\
//                                    OS_EXIT_CRITICAL();        \
//                                    return;																		\
//								}																				\
//							}																					\
//							/*BUS_ASSERT(true, "no map space");*/													\
//							OS_EXIT_CRITICAL(); \
//						}																						\
//						valueT * name##_serach(keyT *key)								\
//						{																						\
//                            CPU_SR_ALLOC();                            \
//                            OS_ENTER_CRITICAL();                                            \
//                            for (int i = 0; i < cnt; i++)															\
//							{																					\
//                                if ((container)[i].isUsed)											\
//								{																				\
//                                    if (name##_key_compare(key, &(container)[i].key) == 0)			\
//									{																			\
//                                        OS_EXIT_CRITICAL(); \
//                                        return &(container)[i].value;									\
//									}																			\
//								}																				\
//							}																					\
//                            OS_EXIT_CRITICAL(); \
//							return NULL;																		\
//						}																						\
//						void name##_del(keyT *key)												\
//						{																						\
//                            CPU_SR_ALLOC();                            \
//                            OS_ENTER_CRITICAL();                                            \
//                            for (int i = 0; i < cnt; i++)															\
//							{																					\
//                                if ((container)[i].isUsed)											\
//								{																				\
//                                    if (name##_key_compare(key, &(container)[i].key) == 0)			\
//									{																			\
//                                        (container)[i].isUsed = false;								\
//                                        OS_EXIT_CRITICAL(); \
//                                        return;																	\
//									}																			\
//								}																				\
//							}																					\
//							/*BUS_ASSERT(true, "no ths key");*/														\
//							OS_EXIT_CRITICAL(); \
//						}


//#define   MAP_CONATNER(cnt,keyT,valueT ,name  )						   \
//												struct				   \
//												{					   \
//													keyT key;		   \
//													valueT value;		   \
//													bool isUsed;		   \
//												}name[cnt]

//#define MAP_KEY_COMPARE(name)         int name##_key_compare(void * l, void *r)


////============================== 队列 模板 ========================================================================                                             
//                                                
//                                                
//#define QUEUE_CONTAINER(container,eleT,size,bitSize)		\
//					struct                                               \
//					{													 \
//						eleT element[(size)];					 \
//						unsigned int r_idx : bitSize;	 \
//						unsigned int w_idx : bitSize;	 \
//						int ele_size;									 \
//																		 \
//					}container

//#define QUEUE_CREATE(name,container,eleT)															\
//						bool name##_Enter(eleT *data)										\
//						{																	\
//							if (container.ele_size >= SEQ_QUEUE_SIZE)								\
//							{																\
//								return false;												\
//							}																\
//							else															\
//							{																\
//								CPU_SR_ALLOC();												\
//								OS_ENTER_CRITICAL();										\
//								container.element[container.w_idx] = *data;									\
//								container.w_idx = (container.w_idx + 1) % SEQ_QUEUE_SIZE;					\
//								container.ele_size++;												\
//								name##_post();													\
//								OS_EXIT_CRITICAL();											\
//								return true;												\
//							}																\
//						}																	\
//						bool name##_Delete(eleT *data)									\
//						{																	\
//							if (container.ele_size <= 0)											\
//							{																\
//								return false;												\
//							}																\
//							else															\
//							{																\
//								CPU_SR_ALLOC();												\
//								OS_ENTER_CRITICAL();										\
//								*data = container.element[container.r_idx];									\
//								container.r_idx = (container.r_idx + 1) % SEQ_QUEUE_SIZE;					\
//								container.ele_size--;												\
//								OS_EXIT_CRITICAL();											\
//																							\
//								return true;												\
//							}																\
//						}																	\
//						bool name##_Waite(void)											\
//						{																	\
//							return name##_pend();												\
//						}																	\
//						int name##_GetSize(void)											\
//						{																	\
//							return container.ele_size;												\
//						}

//#define QUEUE_PEND(name)		bool name##_pend(void)
//#define QUEUE_POST(name)		bool name##_post(void)                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                
//                                                

//#endif

