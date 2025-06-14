/************************************************************************************************************************************************************
 * @Name         : RAVIKUMAR
 * @Date         : 30-05-2023
 * @Description  : Implement c code for frame format of "Glowfy serial communication Protocol".
 a. Frame format max size is 62 Bytes
 b. The frame format is containing 10 field bits and following are: Header, version, sequence, command, data length, data unit,
 and checksum bits
 * @Arguments    :
 * @returntype   :
 **************************************************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define HEADER_1B       0X55
#define HEADER_2B       0XAA
#define VERSION         0X02
#define SUCCESS         0
#define IN_PROCESS      1
#define HEADER_ERROR    2
#define VERSION_ERROR   3
#define CHECK_SUM_ERROR 4

#define GLOWFY_SER_CMD_MOD_RST       0x00
#define GLOWFY_SER_CMD_MOD_QRY       0x01
#define GLOWFY_SER_CMD_DAL_CTL       0x04
#define GLOWFY_SER_CMD_DAL_CTL_RESP  0x05
#define GLOWFY_SER_CMD_DAL_QRY       0x28
#define GLOWFY_SER_CMD_DAL_QRY_RESP  0x06

#define GLOWFY_MOD_CNT_CON_CMD_RST            0X00 //1
#define GLOWFY_MOD_CNT_CON_CMD_ADD            0x01 //2
#define GLOWFY_MOD_CNT_CON_CMD_ARC_PWR        0x02 //3
#define GLOWFY_MOD_CNT_CON_CMD_TOGGLE         0x03 //4
#define GLOWFY_MOD_CNT_CON_CMD_SCENE          0x04 //5
#define GLOWFY_MOD_CNT_CON_CMD_B_CONF         0x05 //6
#define GLOWFY_MOD_CNT_CON_CMD_GRP_CONF       0x06 //7
#define GLOWFY_MOD_CNT_CON_CMD_ADD_SCENE_CONF 0x07 //8
#define GLOWFY_MOD_CNT_CON_CMD_RMV_SCENE_CONF 0x08 //9
#define GLOWFY_MOD_CNT_CON_CMD_IDNT_DEV       0X09 //10


struct frame_format
{
	uint8_t header[2];
	uint8_t version;
	uint8_t sequence[2];
	uint8_t command;
	//uint16_t data_len;
	uint8_t data_len[2];
	uint8_t data[53];
	uint8_t check_sum[1];
};

uint8_t received_frame(uint8_t *glowfy_rx_buf, struct frame_format* glowfly, uint8_t rx_buf_len);

uint8_t glowfy_pkt_transmit(uint8_t *glowfy_tx_buf, uint8_t tx_buf_len)
{
    uint8_t i;

    printf("\n\rTx Buffer: ");
    for(i=0;i<tx_buf_len;i++)
    {
        printf(" %02X",glowfy_tx_buf[i]);
    }
}

uint8_t main()
{
	struct frame_format glowfly;

	uint8_t i, packets[62], res = 0, packets_temp, glowfy_rx_buf[130], indx = 0, pkt_start_flag=1, sum = 0;

	uint8_t glowfy_dul;

	printf("Enter the packets:\n");

	while(1)
	{
		scanf("%hhX", &packets_temp);
		glowfy_rx_buf[indx++] = packets_temp;
		//res = received_frame(glowfy_rx_buf, &glowfly);
		//printf("\n Got ret val %d\n",res);

		printf("\n index: %x index-1: %x\n",glowfy_rx_buf[indx-1],glowfy_rx_buf[indx-2]);
		if(glowfy_rx_buf[indx-1] == 0XAA && glowfy_rx_buf[indx-2] == 0X55)
		{
			pkt_start_flag = 1;
			indx = 0;
			glowfy_rx_buf[0] = 0x55;
			glowfy_rx_buf[1] = 0XAA;
			indx = 2;
		}
		if(pkt_start_flag == 1)
		{

			if(indx == 8)
            {
				glowfy_dul = glowfy_rx_buf[indx-2] + glowfy_rx_buf[indx-1];

				printf("\n size0: %x size1: %x\n",glowfy_rx_buf[indx-2],glowfy_rx_buf[indx-1]);
			}

				if(indx == glowfy_dul + 9)
                    {
                    for(i = 0; i < indx; i++)
                        {
                        printf(" %x", glowfy_rx_buf[i]);
                        }
                    pkt_start_flag=0;
                    received_frame(glowfy_rx_buf, &glowfly,indx);
                }
		}

	}




	/*if(pkt_start_flag == 0)
    {
		res = received_frame(glowfy_rx_buf, &glowfly);
	}
	if(res == SUCCESS)
	{
		printf("\nComplete packets received & Verified successfully..!\n");
	}
	else if(res == HEADER_ERROR)
	{
		printf("\nHeader error..!\n");
	}
	else if(res == VERSION_ERROR)
	{
		printf("\nVersion error..!\n");
	}
	else if(res == CHECK_SUM_ERROR)
	{
		printf("\nData mismatched\n");
	}
	else
	{
		printf("\nUnknown..!\n");
	}*/
	return 0;
}


uint8_t glowfy_ser_pkt_chksm(uint8_t* glowfy_rx_buf, uint8_t rx_buf_len)
{
   uint32_t sum=0;
   uint8_t i,check_sum=0;

   for(i = 0; i < rx_buf_len-1; i++)
	{
		sum = sum + glowfy_rx_buf[i];
	}
    printf("\nTotal sum(header to data) = %X", sum);
    check_sum = (sum % 256);

    printf("\ncheck_sum = %X", check_sum);
    if(check_sum == glowfy_rx_buf[rx_buf_len-1])
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t glowfy_ser_com_control(uint8_t* glowfy_rx_buf, uint8_t rx_buf_len)
{
  switch(glowfy_rx_buf[4])
  {
        case GLOWFY_MOD_CNT_CON_CMD_RST:            //1
                                        break;
        case GLOWFY_MOD_CNT_CON_CMD_ADD:            //2
                                        break;
        case GLOWFY_MOD_CNT_CON_CMD_ARC_PWR         //3
                                        break;
        case GLOWFY_MOD_CNT_CON_CMD_TOGGLE          //4
                                        break;
        case GLOWFY_MOD_CNT_CON_CMD_SCENE           //5
                                        break;
        case GLOWFY_MOD_CNT_CON_CMD_B_CONF          //6
                                        break;
        case GLOWFY_MOD_CNT_CON_CMD_GRP_CONF        //7
                                        break;
        case GLOWFY_MOD_CNT_CON_CMD_ADD_SCENE_CONF  //8
                                        break;
        case GLOWFY_MOD_CNT_CON_CMD_RMV_SCENE_CONF  //9
                                        break;
        case GLOWFY_MOD_CNT_CON_CMD_IDNT_DEV        //10
                                        break;
  }
}

uint8_t received_frame(uint8_t* glowfy_rx_buf, struct frame_format* glowfly, uint8_t rx_buf_len)
{
	uint8_t i, h, v, seq, flag_h = 0, flag_v = 0, temp2 = 0, temp4 = 0, temp5 = 0, key = 0, check_sum = 0;
	uint16_t temp1 = 0, temp3 = 0, len = 0;
	uint32_t temp6 = 0, sum = 0;
	//static uint16_t len = 0;
    uint8_t num = 0, num2 = 0, num3 = 0;

    if(!glowfy_ser_pkt_chksm(glowfy_rx_buf,rx_buf_len))
    {
        printf("\n\rChecksum valid");

        switch(glowfy_rx_buf[5])
        {
        case GLOWFY_SER_CMD_MOD_RST: glowfy_pkt_transmit(glowfy_rx_buf,10);
                                    break;

        case GLOWFY_SER_CMD_MOD_QRY: glowfy_pkt_transmit(glowfy_rx_buf,10);
                                    break;

        case GLOWFY_SER_CMD_DAL_CTL: glowfy_ser_com_control(glowfy_rx_buf,rx_buf_len);
                                     glowfy_tx_buf[10]
                                     glowfy_pkt_transmit(glowfy_tx_buf,10);
                                    break;

        case GLOWFY_SER_CMD_DAL_CTL_RESP:
                                    break;

        case GLOWFY_SER_CMD_DAL_QRY:glowfy_pkt_transmit(glowfy_rx_buf,15);
                                    break;

        case GLOWFY_SER_CMD_DAL_QRY_RESP:
                                    break;
        }
    }
    else
    {
        printf("\n\rChecksum invalid");
    }

/*

	for(i = 0; i < 8; i++)
    {
		if(i < 2)
        {
			glowfly->header[i] = glowfy_rx_buf[i];
			printf("\nbuffer[%d]:%X", i, glowfy_rx_buf[i]);
			printf("\nheader: %X", glowfly->header[i]);
		}
		else if(i == 2)
        {
			glowfly->version = glowfy_rx_buf[i];
			printf("\nbuffer[%d]:%X", i, glowfy_rx_buf[2]);
			printf("\n\nversion: %X\n", glowfly->version);
		}
		//else if(i < 5)
        else if(i == 3)
		{
			glowfly->sequence[1] = glowfy_rx_buf[i];
			printf("\nbuffer[%d]:%X", i, glowfy_rx_buf[i]);
			printf("\nsequence: %X\n", glowfly->sequence[i]);
		}
		else if(i == 4)
        {
            glowfly->sequence[2] = glowfy_rx_buf[i];
            printf("\nbuffer[%d]:%X", i, glowfy_rx_buf[i]);
			printf("\nsequence: %X\n", glowfly->sequence[i]);
		}
		else if(i == 5)
		{
			glowfly->command = glowfy_rx_buf[i];
			printf("\nbuffer[%d]:%X", i, glowfy_rx_buf[5]);
			printf("\ncommand: %X\n", glowfly->command);
		}
		else if(i < 8)
		{
			glowfly->data_len[num2] = glowfy_rx_buf[i];
			num2++;
			printf("\nbuffer[%d]:%X", i, glowfy_rx_buf[i]);
			printf("\ndata_len: %X\n", glowfly->data_len[i]);
		}
	}
	for(i = 8; i < 15; i++)
	{
		glowfly->data[num3] = glowfy_rx_buf[i];
		num3++;
		printf("\nbuffer[%d]:%X", i, glowfy_rx_buf[i]);
		printf("\nData:%X\n", glowfly -> data[i]);
		if(i == 15)
		{
			glowfly->check_sum[0] = glowfy_rx_buf[i];
			printf("\nData:%X", glowfly -> data[i]);
		}
	}

	for(i = 0; i < 2; i++)
	{
	   len = len + glowfly -> data_len[i];
	   printf("\nbuffer[%d]:%X", i, glowfy_rx_buf[i]);
	   printf("\nData len value: %X", glowfly->data_len[i]);
    }
    printf("\nTotal Data len value: %X", len);

	//if(temp > (9 + data_len))

		/*
		for(v = 0; v < 7; v++){
			if(v<2){
				sum = sum + glowfly->header[v];
				printf("\nsum1 = %X", sum);
			}
			else if(v==2){
				sum = sum + glowfly->version;
				printf("\nsum2 = %X", sum);
			}
			else if(v<5)
			{
				if(v==3){
					sum = sum + glowfly->sequence[0];
					printf("\nsum3 = %X", sum);
				}
				if(v==4){
					sum = sum + glowfly->sequence[1];
					printf("\nsum4 = %X", sum);
				}
			}
			else if(v==5){
				sum = sum + glowfly->command;
				printf("\nsum5 = %X", sum);
			}
			else if(v==6){
				sum = sum + glowfly->data_len;
				printf("\nsum6 = %X", sum);
			}
		}
		for(v = 0; v < len; v++)
		{
			printf("\ndata[%d]:sum = %X",v, glowfly->data[v]);
		}
		printf("\n\n");

		for(v = 0; v < glowfly->data_len; v++)
		{
			sum = sum + glowfly->data[v];
			printf("\ndata_len  = %x",len);

		}
		*/

	/*for(i = 0; i < 15; i++)
	{
		sum = sum + glowfy_rx_buf[i];
		printf("\n buff inside function : %X\n", glowfy_rx_buf[i]);
	}
    printf("\nTotal sum(header to data) = %X", sum);
    check_sum = (sum % 256);

    printf("\ncheck_sum = %X", check_sum);
    if(check_sum == glowfy_rx_buf[15])
    {
        return 0;
    }
    else
    {
        return 4;
    }
    */

}

