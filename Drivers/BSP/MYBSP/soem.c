/*
*********************************************************************************************************
*
*	ģ������ :soem��վӦ�ò�
*	�ļ����� : 
*	��    �� : V1.0
*	˵    �� : ���ƻ㴨�ŷ�
*						
*	�޸ļ�¼ :
*		�汾��     ����        ����      
*		V1.0    2025-05-14  shisanxiang1
*
*********************************************************************************************************
*/
#include "soem.h"
#include "stdio.h"
#include "string.h"
#include "stm32F4xx_hal.h"



/*  ����ͬ��ʱ�� */
#define SYNC0TIME 1000000
/*  sdoд����������Ϣ����� */
#define DEBUG 1


/* pdo�ṹ��  */
PDO_Outputs *outputs1;
PDO_Input *inputs1;

char IOmap[4096];
uint32_t Motor_Run=0;
int expectedWKC;




/*
*********************************************************************************************************
*	�� �� ��: int write8(uint16  slave, uint16  index, uint8  subindex, int value)
*	����˵��:sdoд8λ
*	��    ��: 
*********************************************************************************************************
*/
int write8(uint16  slave, uint16  index, uint8  subindex, int value)
{
	uint8 temp = value;
	/*  ���õ���soem��coe���װ��sdoд���� */
	int rtn = ec_SDOwrite(slave, index, subindex, FALSE, sizeof(temp), &temp, EC_TIMEOUTRXM );

	if (rtn == 0) { printf("SDOwrite to %#x failed !!! \r\n", index); }
	else if (DEBUG) { printf("SDOwrite to slave%d  index:%#x value:%x Successed !!! \r\n", slave, index, temp); }
	return rtn;
}

/*
*********************************************************************************************************
*	�� �� ��: int write16(uint16  slave, uint16  index, uint8  subindex, int value)
*	����˵��:sdoд16λ
*	��    ��: 
*********************************************************************************************************
*/
int write16(uint16  slave, uint16  index, uint8  subindex, int value)
{
	uint16 temp = value;

	int rtn = ec_SDOwrite(slave, index, subindex, FALSE, sizeof(temp), &temp, EC_TIMEOUTRXM * 20);

	if (rtn == 0) { printf("SDOwrite to %#x failed !!! \r\n", index); }
	else if (DEBUG) { printf("SDOwrite to slave%d  index:%#x value:%x Successed !!! \r\n", slave, index, temp); }
	return rtn;
}

/*
*********************************************************************************************************
*	�� �� ��: int write32(uint16 slave, uint16 index, uint8 subindex, int value)
*	����˵��:sdoд32λ
*	��    ��: 
*********************************************************************************************************
*/
int write32(uint16 slave, uint16 index, uint8 subindex, int value)
{
	uint32 temp = value;

	int rtn = ec_SDOwrite(slave, index, subindex, FALSE, sizeof(temp), &temp, EC_TIMEOUTRXM * 20);
	if (rtn == 0) { printf("SDOwrite to %#x failed !!! \r\n", index); }
	else if (DEBUG) { printf("SDOwrite to slave%d  index:%#x value:%x Successed !!! \r\n", slave, index, temp); }
	return rtn;
}

/*
*********************************************************************************************************
*	�� �� ��: INOVANCE_Nsetup(uint16 slvcnt)
*	����˵��:�㴨�ŷ��ɱ�pdoӳ������
*	��    ��: ��վ��
*********************************************************************************************************
*/
int INOVANCE_Nsetup(uint16 slvcnt)
{
	
	printf(" slvcnt = %d\r\n",slvcnt);
	/*  �����ʼrpdo���� */
	write8(slvcnt, 0x1C12, 00, 0);				      //��0x1C12����		
	write8(slvcnt, 0x1600, 00, 0);				      //��0x1601����		
	/* 1600hΪӳ��������ã�������������������6000h  */
	write32(slvcnt, 0x1600, 01, 0x60400010);		//����0x1601������		������
	write32(slvcnt, 0x1600, 02, 0x60830020);		//����0x1601������  	���ٶ�
	write32(slvcnt, 0x1600, 03, 0x60840020);		//����0x1601������		���ٶ�
	write32(slvcnt, 0x1600, 04, 0x60FF0020);		//����0x1601������		Ŀ���ٶ�
	/*  �����������ĸ�ӳ�� */
	write8(slvcnt, 0x1600, 00, 4);				      //��������������
	/*  1c12h�ǻ㴨�ŷ�Ψһ�ɱ��rpdo����pdoͨѶ���� */
	write16(slvcnt, 0x1C12, 01, 0x1600);			  //����RxPDOӳ��		
	/*  00�����Ǳ��������˼���pdo */	
	write8(slvcnt, 0x1C12, 00, 1);

	/* �ֲ�ԭ�ģ�д��PDOӳ���顣����������д��ӳ�������飬1C12h��Ԥд��1600h/1701h~1705h��ֵ��1C13h��
		Ԥд��1A00h/1B01h~1B04h��ֵ��ע�⣺ֻ��1600h��1A00h�ǿ�����ӳ���飬�����ǹ̶�ӳ����
		�ã� */
	
	/* ����tpdo������������  */
	write8(slvcnt, 0x1C13, 00, 00);				      //��0x1C12����							
	write8(slvcnt, 0x1A00, 00, 00);				      //��0x1600����							
	write32(slvcnt, 0x1A00, 01, 0x60410010);		//����0x1A01������			
  write32(slvcnt, 0x1A00, 02, 0x60640020);		//����0x1A01������	
  write32(slvcnt, 0x1A00, 03, 0x606C0020);		//����0x1A01������	
  write8(slvcnt, 0x1A00, 00, 03);				      //��������������
 
	write16(slvcnt, 0x1C13, 01, 0x1A00);			  //����TxPDOӳ��
	write8(slvcnt, 0x1C13, 00, 01);
	
	write8(slvcnt, 0x6060, 00, 3);                   //����Ϊ�ٶ�ģʽ

  return 0;
}

/**
  * ��������: ecat��ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void ecat_init(void)
{
	int chk, slc;
	int i;
	
    /* initialise SOEM, bind socket to ifname */
  if (ec_init("ServoDrive"))
	{
		printf("ec_init succeeded.\r\n");
        
		if ( ec_config_init(FALSE) > 0 )
		{
			printf("%x slaves found and configured.%x \r\n",ec_slave[1].eep_man,ec_slave[1].eep_id);
            
			if ( ec_slavecount >= 1 ) 
			{
				for(slc = 1; slc <= ec_slavecount; slc++)
				 {									 
					 //�㴨�ŷ�
					 if((ec_slave[slc].eep_man == 0x100000) && (ec_slave[slc].eep_id == 0xc010d))
					 {
							 printf("Found %s at position %d\r\n", ec_slave[slc].name, slc);
							 printf("Found %x at position %d\r\n", ec_slave[slc].configadr, slc);
							 ec_slave[slc].PO2SOconfig = &INOVANCE_Nsetup; 
					 }
				 }
       }
            
      ec_configdc();

			
			ec_dcsync0(1, TRUE, SYNC0TIME, 0); // SYNC0 on slave 1
			 
		  ec_config_map(&IOmap);
			 
			printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

			ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE);
	
		  printf("Request operational state for all slaves\n");
		  expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
		  printf("Calculated workcounter %d\n", expectedWKC);
		  ec_slave[0].state = EC_STATE_OPERATIONAL;

			ec_send_processdata();
			
			ec_receive_processdata(EC_TIMEOUTRET);
				
			ec_writestate(0);
			do
		  {
				ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
		  }     
		  while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));			
				 
			printf("DC capable : %d\r\n",ec_configdc());
			
			//HAL_Delay(100);
			
			for( i=0; i<10; i++ )
			{
				ec_slave[0].state = EC_STATE_OPERATIONAL;
				ec_writestate(0);
			}
				 
			if (ec_slave[0].state == EC_STATE_OPERATIONAL )
			{					
				outputs1 = (PDO_Outputs *)ec_slave[1].outputs;
				inputs1  = (PDO_Input *)ec_slave[1].inputs;

				Motor_Run = 1;

				printf("all slaves reached operational state.\r\n");
			}			
			else
			{
				printf("E/BOX not found in slave configuration.\r\n");
			}
		}
		else
		{
			printf("No slaves found!\r\n");
		}
	}
	else
	{
		printf("No socket connection Excecute as root\r\n");
	}
    
}

/*
*********************************************************************************************************
*	�� �� ��: ecat_loop(void)
*	����˵��:	pdo���ͺͽ���ѭ������tim4���жϻص��е���
*	��    ��: 
*********************************************************************************************************
*/
void ecat_loop(void)
{
	if (Motor_Run>0)
	{
		ec_send_processdata();
		
		ec_receive_processdata(EC_TIMEOUTRET);        
		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ctrl_state(void)
*	����˵��:�ŷ�����
*	��    ��: 
*********************************************************************************************************
*/
void ctrl_state(void)
{
    switch (outputs1->controlword)
    {
			/*  ������Ϊ0ʱ */
    case 0:
	      outputs1->decelerAtion=139810130;
		    outputs1->accelerAtion=139810130;
		/*
				ÿ��27962026��ָ����������Ϊ23Ϊ�����ӳ��ֱ�Ĭ��Ϊ1��ÿȦ��2^23Ϊ8388608/1
				27962026*1/83880=3r/s
		*/
		    outputs1->targetVelocity=27962026;
		/*  �ŷ��޹��ϡ��ŷ�׼���� */
        outputs1->controlword = 6;
        break;
		/*  ������Ϊ6ʱ */
    case 6:
				/*  �ŷ�׼���á��ȴ����ŷ�ʹ�� */
        outputs1->controlword = 7;
        break;
		/*  ������Ϊ7ʱ */
    case 7:
			/*  �ȴ����ŷ�ʹ�ܡ��ŷ����� */
        outputs1->controlword = 0x0f;
        break;
    case 0x0f:
        break;
    default:
        outputs1->controlword = 6;
        break;
    }
    printf("\r\nctl = %d  \r\n",outputs1->controlword);
}


