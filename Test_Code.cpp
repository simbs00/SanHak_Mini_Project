#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

void main(int argc, char **argv)
{
	double t_end=20000.0;
	double n_send, n_rcvd, n_acked_dt, n_dt_error, n_ack_error, n_retran;
	double t1, t_waiting;
	//double t_rtx_timeout;
	int	customer=1, event;
	int i1, i2, i3;
	bool flag1;
	/* Definition of system parameters */
	double	PKT_DATA_LENGTH;		
	double 	PKT_ACK_LENGTH;		
	double	LINK_RATE;					
	double 	PKT_DT_ERROR_PROB;	
	double 	PKT_ACK_ERROR_PROB;
	double	tm_out_duration;
	double	tm_inter_packet;
	double	tm_oneway_data;
	double	tm_oneway_ack;
	double	tm_propag;
	double	distance;
	char		arq_type[30];
	const double cspeed = 3.0E8;

	srand((unsigned int)time(NULL));
	PKT_ACK_LENGTH = 20*8.0;
	if(argc == 1)
	{
		PKT_DATA_LENGTH = 1024*8.0;		
		LINK_RATE = 1.0E6;	
		distance = 1.0E3;
		PKT_DT_ERROR_PROB = 0.0;	
		PKT_ACK_ERROR_PROB = 0.0;

		printf("***** <<Stop and wait ARQ 프로토콜 시뮬레이션을 시작합니다.>> ****\n");
		printf("***** 기본 파라미터는 다음과 같습니다. \n");
		printf("***** Link 속도: %.0lf [Mbps], 거리: %.0lf [km], 패킷 길이: %.0lf [Byte] \n", LINK_RATE/1E6, 
			distance/1.0E3, PKT_DATA_LENGTH/8.0);
		printf("***** 데이터 전송 오류 확률: %.1lf, ACK 전송 오류 확률: %.1lf \n", PKT_DT_ERROR_PROB, 
			PKT_ACK_ERROR_PROB);
		printf(" 시뮬레이션을 진행하시겠습니까? \n\n");
		
	}
	else
	{
		switch(atoi(argv[1]))
		{
		case 1:
			strcpy(arq_type, "Stop & Wait ARQ");
			break;
		case 2:
			strcpy(arq_type, "Go Back-N ARQ");
			break;
		case 3:
			strcpy(arq_type, "Selective Repeat ARQ");
		}
		
		switch(atoi(argv[2]))	// 전송 속도 선택
		{
		case 1:
			LINK_RATE = 1.0E6;
			break;
		case 2:
			LINK_RATE = 1.0E7;
			break;
		case 3:
			LINK_RATE = 1.0E8;
		}
		switch(atoi(argv[3]))	// 송수신 사이의 거리 선택
		{
		case 1:
			distance = 1.0E3;
			break;
		case 2:
			distance = 1.0E4;
			break;
		case 3:
			distance = 1.0E5;
		}
		switch(atoi(argv[4]))	// 패킷 길이 선택
		{
		case 1:
			PKT_DATA_LENGTH = 1024*8.0;	
			break;
		case 2:
			PKT_DATA_LENGTH = 1500*8.0;	
			break;
		case 3:
			PKT_DATA_LENGTH = 8192*8.0;	
		}
		switch(atoi(argv[5]))
		{
		case 1:
			PKT_DT_ERROR_PROB = 0.0;	
			break;
		case 2:
			PKT_DT_ERROR_PROB = 0.1;
			break;
		case 3:
			PKT_DT_ERROR_PROB = 0.5;
		}
		switch(atoi(argv[6]))
		{
		case 1:
			PKT_ACK_ERROR_PROB = 0.0;	
			break;
		case 2:
			PKT_ACK_ERROR_PROB = 0.1;
			break;
		case 3:
			PKT_ACK_ERROR_PROB = 0.5;
		}

		printf("***** <<%s 프로토콜 시뮬레이션을 시작합니다.>> ****\n", arq_type);
		printf("***** 선택된 파라미터는 다음과 같습니다. \n");
		printf("***** Link 속도: %.0lf [Mbps], 거리: %.0lf [km], 패킷 길이: %.0lf [Byte] \n", LINK_RATE/1E6, 
			distance/1.0E3, PKT_DATA_LENGTH/8.0);
		printf("***** 데이터 전송 오류 확률: %.1lf, ACK 전송 오류 확률: %.1lf \n", PKT_DT_ERROR_PROB, 
			PKT_ACK_ERROR_PROB);
		printf(" 시뮬레이션을 진행하시겠습니까? \n\n");
		
	}
	tm_propag = distance/cspeed;
	tm_oneway_data = PKT_DATA_LENGTH/LINK_RATE + tm_propag;
	tm_oneway_ack = PKT_ACK_LENGTH/LINK_RATE + tm_propag;
	tm_out_duration = tm_oneway_data + tm_oneway_ack + 1.0E-4;
	tm_inter_packet = PKT_DATA_LENGTH/LINK_RATE;	

	printf("**** 시스템 파라미터 **** \n");
	printf("(1) 단방향 전파지연시간: %lf \n", tm_propag);
	printf("(2) 재전송 타이머 값: %lf \n", tm_out_duration);
	printf("(3) 데이터 패킷 전송시간: %lf \n", tm_inter_packet);
	printf("(4) 데이터 단방향 지연시간: %lf \n", tm_oneway_data);
	printf("(5) ACK 단방향 지연시간: %lf \n\n", tm_oneway_ack);
	

	printf("\n\n");
	/* Report the results */	
	n_send = 1.0E5;
	n_rcvd = 1.0E5*rand()/32767.0;
	n_acked_dt = 0.9E5;
	n_retran = 1.0E2;
	n_dt_error = 0.9E2;
	n_ack_error = 0.0;
	t_waiting = 2.0E1*rand()/32767.0;
	
	printf("   System throughput : %.1lf [bps]\n", (n_rcvd*PKT_DATA_LENGTH)/t_end);
	printf("   Average system delay : %.5lf [ms]\n", t_waiting/n_rcvd/1.0E-3);
	printf("   Utilization : %.5lf \n", (n_rcvd*PKT_DATA_LENGTH/LINK_RATE)/t_end);
		
	
}

