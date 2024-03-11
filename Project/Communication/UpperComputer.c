#pragma diag_suppress 111
#include "UpperComputer.h"
#include "Config.h"

typedef struct __attribute__((packed))
{
    UpperComputer_ParamInfo_t channelParam[CONTROL_CHANNELS_NUM];
    uint16_t crc;
} UpperComputer_FlashData_t;

void UpperComputer_Init(void)
{
    if(UPPERCOMPUTER_ISACTIVE == 0) { return; }
    //初始化Flash，并将数据读取到Sram
    upperComputer = UPPERCOMPUTER_INIT;
    upperComputer.uartstream.dataProcess = UpperComputer_DataProcess;
	HAL_Delay(10);
    FlashBsp_Init(&upperComputer.flashmemory);
    //以UpperComputer_FlashData_t的形式获得Sram数据
    UpperComputer_FlashData_t* pflashData = (UpperComputer_FlashData_t*)FlashBsp_GetAddr(&upperComputer.flashmemory, 0);
    //如果CRC校验通过，则将数据写入通道参数
	//pflashData->crc = 1;
    if(pflashData->crc == MathBsp_Crc16(&pflashData->channelParam, sizeof(UpperComputer_ParamInfo_t) * CONTROL_CHANNELS_NUM))
    {
        for(uint8_t i = 0; i < CONTROL_CHANNELS_NUM; i++)
        {
            Channel_t* channel = Control_GetChannel(i);
            UpperComputer_ParamInfo_t* param = &pflashData->channelParam[i];
			channel->pid.target = param->target;
            channel->pid.kp = param->kp;
            channel->pid.ki = param->ki;
            channel->pid.kd = param->kd;
            channel->pid.integral_startzone = param->integralStartzone;
            channel->pid.integral_deadband = param->integralDeadband;
            channel->pid.integral_max = param->integralMax;
            channel->pid.integral_min = param->integralMin;
            channel->pid.output_max = param->outputMax;
            channel->pid.output_min = param->outputMin;
            channel->sample.source_voltage.map_k = param->mapping_inputK;
            channel->sample.source_voltage.map_b = param->mapping_inputB;
            channel->sample.output_voltage.map_k = param->mapping_outputK;
            channel->sample.output_voltage.map_b = param->mapping_outputB;
        }
    }
    else
    {
        //CRC校验失败，将默认参数写入Flash
        for(uint8_t i = 0; i < CONTROL_CHANNELS_NUM; i++)
        {
            Channel_t* channel = Control_GetChannel(i);
            UpperComputer_ParamInfo_t* param = &pflashData->channelParam[i];
			param->target = channel->pid.target;
            param->kp = channel->pid.kp;
            param->ki = channel->pid.ki;
            param->kd = channel->pid.kd;
            param->integralStartzone = channel->pid.integral_startzone;
            param->integralDeadband = channel->pid.integral_deadband;
            param->integralMax = channel->pid.integral_max;
            param->integralMin = channel->pid.integral_min;
            param->outputMax = channel->pid.output_max;
            param->outputMin = channel->pid.output_min;
            param->mapping_inputK = channel->sample.source_voltage.map_k;
            param->mapping_inputB = channel->sample.source_voltage.map_b;
            param->mapping_outputK = channel->sample.output_voltage.map_k;
            param->mapping_outputB = channel->sample.output_voltage.map_b;
        }
        pflashData->crc = MathBsp_Crc16(&pflashData->channelParam, sizeof(UpperComputer_ParamInfo_t) * CONTROL_CHANNELS_NUM);
        FlashBsp_Write(&upperComputer.flashmemory, (uint8_t*)pflashData, 0, sizeof(UpperComputer_FlashData_t));
        FlashBsp_Program(&upperComputer.flashmemory);
    }
    UartBsp_Init(&upperComputer.uartstream);
}
//处理上位机下发包的Lable部分到回传包
void _UpperComputer_Responce_Lable(void* pmaster, void* pslave)
{
    UpperComputer_Lable_t* master = (UpperComputer_Lable_t*)pmaster;
    UpperComputer_Lable_t* slave = (UpperComputer_Lable_t*)pslave;

    slave->cmd = master->cmd;
    slave->channel = master->channel;
    slave->ch_num = CONTROL_CHANNELS_NUM;
    slave->synCode = ~master->synCode;
}
//处理上位机下发包的ChannelState部分到回传包
void _UpperComputer_Responce_ChannelState(void* pstate, uint8_t ch_num)
{
    UpperComputer_ChannelState_t* state = (UpperComputer_ChannelState_t*)pstate;
    Channel_t* channel = Control_GetChannel(ch_num);
    state->currentVoltage = channel->pid.measure;
    state->targetVoltage = channel->pid.target;
}
//处理上位机的Heartbeat下发包并响应回传包
void UpperComputer_Responce_Heartbeat(UpperComputer_t* upper, UpperComputer_HeartbeatM_t* heartbeat)
{
    UpperComputer_HeartbeatS_t responce;
    _UpperComputer_Responce_Lable(&heartbeat->lable, &responce.lable);
    _UpperComputer_Responce_ChannelState(&responce.channelState, heartbeat->lable.channel);
    UartBsp_Send(&upper->uartstream, (uint8_t*)&responce, sizeof(UpperComputer_HeartbeatS_t));
}
//处理上位机的ReadFlash下发包并响应回传包
void UpperComputer_Responce_ReadFlash(UpperComputer_t* upper, UpperComputer_ReadFlashM_t* readFlash)
{
    UpperComputer_ReadFlashS_t responce;
    _UpperComputer_Responce_Lable(&readFlash->lable, &responce.lable);
    _UpperComputer_Responce_ChannelState(&responce.channelState, readFlash->lable.channel);
    Channel_t* channel = Control_GetChannel(readFlash->lable.channel);
	
	UpperComputer_FlashData_t* pflashData = (UpperComputer_FlashData_t*)FlashBsp_GetAddr(&upperComputer.flashmemory, 0);
	UpperComputer_ParamInfo_t* param = &pflashData->channelParam[responce.lable.channel];
	channel->pid.target = param->target;
	channel->pid.kp = param->kp;
	channel->pid.ki = param->ki;
	channel->pid.kd = param->kd;
	channel->pid.integral_startzone = param->integralStartzone;
	channel->pid.integral_deadband = param->integralDeadband;
	channel->pid.integral_max = param->integralMax;
	channel->pid.integral_min = param->integralMin;
	channel->pid.output_max = param->outputMax;
	channel->pid.output_min = param->outputMin;
	channel->sample.source_voltage.map_k = param->mapping_inputK;
	channel->sample.source_voltage.map_b = param->mapping_inputB;
	channel->sample.output_voltage.map_k = param->mapping_outputK;
	channel->sample.output_voltage.map_b = param->mapping_outputB;
	
    //写入通道对应的数据
	responce.paramInfo.target = channel->pid.target;
    responce.paramInfo.kp = channel->pid.kp;
    responce.paramInfo.ki = channel->pid.ki;
    responce.paramInfo.kd = channel->pid.kd;
    responce.paramInfo.integralStartzone = channel->pid.integral_startzone;
    responce.paramInfo.integralDeadband = channel->pid.integral_deadband;
    responce.paramInfo.integralMax = channel->pid.integral_max;
    responce.paramInfo.integralMin = channel->pid.integral_min;
    responce.paramInfo.outputMax = channel->pid.output_max;
    responce.paramInfo.outputMin = channel->pid.output_min;
    responce.paramInfo.mapping_inputK = channel->sample.source_voltage.map_k;
    responce.paramInfo.mapping_inputB = channel->sample.source_voltage.map_b;
    responce.paramInfo.mapping_outputK = channel->sample.output_voltage.map_k;
    responce.paramInfo.mapping_outputB = channel->sample.output_voltage.map_b;

    UartBsp_Send(&upper->uartstream, (uint8_t*)&responce, sizeof(UpperComputer_ReadFlashS_t));
}
//处理上位机的SetParam下发包并响应回传包
void UpperComputer_Responce_SetParam(UpperComputer_t* upper, UpperComputer_SetParamM_t* setParam)
{
    //处理常规回传数据
    UpperComputer_SetParamS_t responce;
    _UpperComputer_Responce_Lable(&setParam->lable, &responce.lable);
    _UpperComputer_Responce_ChannelState(&responce.channelState, setParam->lable.channel);
    Channel_t* channel = Control_GetChannel(setParam->lable.channel);
    //向指定通道写入参数
	channel->pid.target = setParam->paramInfo.target;
    channel->pid.kp = setParam->paramInfo.kp;
    channel->pid.ki = setParam->paramInfo.ki;
    channel->pid.kd = setParam->paramInfo.kd;
    channel->pid.integral_startzone = setParam->paramInfo.integralStartzone;
    channel->pid.integral_deadband = setParam->paramInfo.integralDeadband;
    channel->pid.integral_max = setParam->paramInfo.integralMax;
    channel->pid.integral_min = setParam->paramInfo.integralMin;
    channel->pid.output_max = setParam->paramInfo.outputMax;
    channel->pid.output_min = setParam->paramInfo.outputMin;
    channel->sample.source_voltage.map_k = setParam->paramInfo.mapping_inputK;
    channel->sample.source_voltage.map_b = setParam->paramInfo.mapping_inputB;
    channel->sample.output_voltage.map_k = setParam->paramInfo.mapping_outputK;
    channel->sample.output_voltage.map_b = setParam->paramInfo.mapping_outputB;

    UartBsp_Send(&upper->uartstream, (uint8_t*)&responce, sizeof(UpperComputer_SetParamS_t));
}
//处理上位机的DownloadFlash下发包并响应回传包
void UpperComputer_Responce_DownloadFlash(UpperComputer_t* upper, UpperComputer_DownloadFlashM_t* downloadFlash)
{
    UpperComputer_DownloadFlashS_t responce;
    _UpperComputer_Responce_Lable(&downloadFlash->lable, &responce.lable);
    _UpperComputer_Responce_ChannelState(&responce.channelState, downloadFlash->lable.channel);
	UpperComputer_ParamInfo_t info;
    UpperComputer_FlashData_t* pflashData = (UpperComputer_FlashData_t*)FlashBsp_GetAddr(&upper->flashmemory, 0);
	for(uint8_t i = 0; i < CONTROL_CHANNELS_NUM; i++)
	{
		Channel_t* channel = Control_GetChannel(i);
		info.target = channel->pid.target;
		info.kp = channel->pid.kp;
		info.ki = channel->pid.ki;
		info.kd = channel->pid.kd;
		info.integralStartzone = channel->pid.integral_startzone;
		info.integralDeadband = channel->pid.integral_deadband;
		info.integralMax = channel->pid.integral_max;
		info.integralMin = channel->pid.integral_min;
		info.outputMax = channel->pid.output_max;
		info.outputMin = channel->pid.output_min;
		info.mapping_inputK = channel->sample.source_voltage.map_k;
		info.mapping_inputB = channel->sample.source_voltage.map_b;
		info.mapping_outputK = channel->sample.output_voltage.map_k;
		info.mapping_outputB = channel->sample.output_voltage.map_b;
		pflashData->channelParam[i] = info;		
	}
    pflashData->crc = MathBsp_Crc16(&pflashData->channelParam, sizeof(UpperComputer_ParamInfo_t) * CONTROL_CHANNELS_NUM);
    FlashBsp_Write(&upper->flashmemory, (uint8_t*)pflashData, 0, sizeof(UpperComputer_FlashData_t));
	//upper->flashProgramFlag = 1;
	FlashBsp_Program(&upper->flashmemory);
    UartBsp_Send(&upper->uartstream, (uint8_t*)&responce, sizeof(UpperComputer_DownloadFlashM_t));
}

void UpperComputer_DataProcess(uint8_t* data, uint16_t len)
{
    UNUSED(len);
    UpperComputer_Lable_t* lable = (UpperComputer_Lable_t*)data;

    switch (lable->cmd)
    {
    case UPPERCOMPUTER_CMD_HEARTBEAT:
        UpperComputer_Responce_Heartbeat(&upperComputer, (UpperComputer_HeartbeatM_t*)data);
        break;
    case UPPERCOMPUTER_CMD_SETPARAM:
        UpperComputer_Responce_SetParam(&upperComputer, (UpperComputer_SetParamM_t*)data);
        break;
    case UPPERCOMPUTER_CMD_READFLASH:
        UpperComputer_Responce_ReadFlash(&upperComputer, (UpperComputer_ReadFlashM_t*)data);
        break;
    case UPPERCOMPUTER_CMD_DOWNLOADFLASH:
        UpperComputer_Responce_DownloadFlash(&upperComputer, (UpperComputer_DownloadFlashM_t*)data);
        break;
    default:
        break;
    }
}
