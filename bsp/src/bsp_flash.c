/*
 * @Author       : stark1898y 1658608470@qq.com
 * @Date         : 2023-08-02 14:10:22
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2023-08-02 15:25:09
 * @FilePath     : \JY-DT-YJ1F02_RTT_Nano\bsp\src\bsp_flash.c
 * @Description  :
 *
 * Copyright (c) 2023 by yzy, All Rights Reserved.
 */
#include "bsp_flash.h"
// #include "bsp_bkp.h"
#include "lwutil.h"
#include "bsp_rtc.h"
// #include "bsp_wdg.h"
#include "bsp_mq.h"

#include <rtthread.h>
#include <rthw.h>

#define LOG_TAG     "bsp_flash"          // 该模块对应的标签。不定义时，默认：NO_TAG
#define LOG_LVL     LOG_LVL_DBG     // 该模块对应的日志输出级别。不定义时，默认：调试级别
#include <ulog.h>                   // 必须在 LOG_TAG 与 LOG_LVL 下面

const uint32_t hr_start_addr[7] = {FLASH_HR_ALARM_START_ADDR, FLASH_HR_ALARM_RCY_START_ADDR, \
        FLASH_HR_FAULT_START_ADDR, FLASH_HR_FAULT_RCY_START_ADDR, \
        FLASH_HR_POWER_FAILURE_START_ADDR, FLASH_HR_POWER_ON_START_ADDR, \
        FLASH_HR_SENSOR_FAILURE_START_ADDR
};

const uint8_t hr_record_max_num[7] = {HR_ALARM_MAX_NUM, HR_ALARM_RCY_MAX_NUM, \
        HR_FAULT_MAX_NUM, HR_FAULT_RCY_MAX_NUM, \
        HR_POWER_FAILURE_MAX_NUM, HR_POWER_ON_MAX_NUM, \
        HR_SENSOR_FAILURE_MAX_NUM
};

const uint8_t hr_record_pages[7] = {HR_ALARM_PAGES, HR_ALARM_RCY_PAGES, \
        HR_FAULT_PAGES, HR_FAULT_RCY_PAGES, \
        HR_POWER_FAILURE_PAGES, HR_POWER_ON_PAGES, \
        HR_SENSOR_FAILURE_PAGES
};

static rt_base_t interrupt_value;
static void BSP_Flash_UnLock(void)
{
#if (SystemCoreClock > SYSCLK_FREQ_96MHz_HSE)
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV2;
#endif
    interrupt_value = rt_hw_interrupt_disable();

    FLASH_Unlock();
}

static void BSP_Flash_Lock(void)
{
    FLASH_Lock();

#if (SystemCoreClock > SYSCLK_FREQ_96MHz_HSE)
    RCC->CFGR0 &= ~(uint32_t)RCC_HPRE_DIV2;
#endif
    rt_hw_interrupt_enable(interrupt_value);
}

static void BSP_Flash_FastUnLock(void)
{
#if (SystemCoreClock > SYSCLK_FREQ_96MHz_HSE)
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV2;
#endif
    interrupt_value = rt_hw_interrupt_disable();

    FLASH_Unlock_Fast();
}

static void BSP_Flash_FastLock(void)
{
    FLASH_Lock_Fast();

#if (SystemCoreClock > SYSCLK_FREQ_96MHz_HSE)
    RCC->CFGR0 &= ~(uint32_t)RCC_HPRE_DIV2;
#endif
    rt_hw_interrupt_enable(interrupt_value);
}

/**
 * @description: 在通用地址空间内进行直接寻址，
 *          任何8/16/32位数据的读操作都能访问闪存模块的内容并得到相应的数据
 * @param {uint32_t} addr
 * @param {uint8_t} *buf
 * @param {size_t} len
 * @return {*}
 */
static size_t Flash_Read(uint32_t addr, uint8_t *buf, size_t len)
{
    size_t read_len = 0;

    for (size_t i = 0; i < len; i++, buf++, addr++, read_len++)
    {
        *buf = *(uint8_t *) addr;
    }

    return read_len;
}

/**
 * @description:
 * @param {uint32_t} addr
 * @param {uint8_t} *buf
 * @param {size_t} len
 * @return {*}
 */
static size_t Flash_Write(uint32_t addr, uint8_t *buf, size_t len)
{
    size_t i = 0;
    __attribute__ ((aligned (2))) uint16_t write_data;

    if(addr % 2 != 0)
       return 0;

    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP |FLASH_FLAG_WRPRTERR);

    BSP_Flash_UnLock();

    for (i = 0; i < len; i += 2, buf += 2, addr += 2)
    {
        rt_memcpy(&write_data, buf, 2); //用以保证HAL_FLASH_Program的write_data是内存首地址对齐
        FLASH_ProgramHalfWord(addr, write_data);

        /* You can add your code under here. */
        if (*(uint16_t *)addr != write_data)
        {
            BSP_Flash_Lock();
            LOG_E("Flash_Write Error");
            return 0;
        }
        // else
        // {
			//FLash操作可能非常耗时，如果有看门狗需要喂狗，以下代码由用户实现
            // HAL_FeedDog();
        // }
    }

    BSP_Flash_Lock();

    return i;
}

/**
 * @description: Flash Erase Page
 * @param {TeRecord} record
 * @param {uint8_t} page
 * @return {*}
 */
static ErrorStatus Flash_ErasePage_Records(TeRecord record, uint8_t page_offset)
{
    ErrorStatus flag = READY;

    uint8_t erase_page = 0;

    if(page_offset <= hr_record_pages[record] - 1)
    {
        erase_page = page_offset;
    }
    else
    {
        erase_page = hr_record_pages[record] - 1;
    }

    BSP_Flash_FastUnLock();
    FLASH_ErasePage_Fast(hr_start_addr[record] + FLASH_PAGE_SIZE * erase_page);
    BSP_Flash_FastLock();

    for (uint8_t i = 0; i < (FLASH_PAGE_SIZE / 2); i++)
    {
        if(*(uint16_t*)((hr_start_addr[record] + FLASH_PAGE_SIZE * erase_page) + 2 * i) != 0xE339)
        {
            flag = NoREADY;
        }
    }

    LOG_D("/**Flash_Erase(%d)Page(%d)=%d**/", record, erase_page, flag);

    return flag;
}

static void Flash_Erase_Records(TeRecord record)
{
    for (uint8_t erase_page = 0; erase_page < hr_record_pages[record]; erase_page++)
    {
        Flash_ErasePage_Records(record, erase_page);
    }
    LOG_D("Flash_Erase_Records(%d)", record);
}
MSH_CMD_EXPORT(Flash_Erase_Records, "TeRecord(0~6)");

/*
 * @description:
 * @param {TeRecord} record
 * @return {*}
 */
static uint16_t Flash_GetMaxIndex_Records(TeRecord record)
{
    uint16_t index_max = 0;

    for (uint8_t page = 0; page < hr_record_pages[record]; page++)
    {
        for (uint8_t i = 0; i < FLASH_PAGE_HR_RECORD_NUM; i++)
        {
            if(*(uint16_t*)(hr_start_addr[record] + FLASH_PAGE_SIZE * page + FLASH_HR_ONE_RECORD_SIZE * i) == HR_RECORD_FRAME_HEADER)
            {
                index_max = LWUTIL_MAX(index_max, *(uint16_t *)(hr_start_addr[record] + FLASH_PAGE_SIZE * page + FLASH_HR_ONE_RECORD_SIZE * i + 2));
            }
        }
    }

    LOG_D("Flash_Get_Records(%d)MaxNum = %d", record, index_max);

    return index_max;
}
MSH_CMD_EXPORT(Flash_GetMaxIndex_Records, record(0~6));

/**
 * @description:
 * @param {TeRecord} record
 * @return {*}
 */
uint8_t Flash_GetNum_Records(TeRecord record)
{
    uint8_t num = 0;
    uint16_t num_max = Flash_GetMaxIndex_Records(record);

    if(num_max <= hr_record_max_num[record])
    {
        num = num_max;
    }
    else
    {
        num = hr_record_max_num[record];
    }

    LOG_D("Flash_GetNum_(%d)Records = %d", record, num);

    return num;
}
MSH_CMD_EXPORT(Flash_GetNum_Records, record(0~6));

/**
 * @description:
 * @param {TuFlashHrRecordFrame*} pHrRecord
 * @param {TeRecord} record
 * @param {uint8_t} index
 * @return {*}
 */
void Flash_Read_Record(TuFlashHrRecordFrame* pHrRecord, TeRecord record, uint8_t index)
{
    LOG_D("/*********Flash_Read_(%d)Record(%d)***************/", record, index);
    uint16_t index_max = Flash_GetMaxIndex_Records(record);

    uint8_t page_offset = 0 ;
    uint8_t in_page_offset = 0;

    if(index <= index_max)
    {
        if(index_max > hr_record_max_num[record])
        {
            if(record < kRecordSensorFailure)
            {
                // index_redirect = index_max - (hr_record_max_num[record] - index);
                uint16_t index_redirect = index_max - (50 - index);
                LOG_D("index_redirect = %d", index_redirect);
                page_offset = ((index_redirect - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
                in_page_offset = ((index_redirect - 1) % FLASH_PAGE_HR_RECORD_NUM) * FLASH_HR_ONE_RECORD_SIZE;
            }
            else
            {
                page_offset = 0;
                in_page_offset = ((index_max - 1) % FLASH_PAGE_HR_RECORD_NUM) * FLASH_HR_ONE_RECORD_SIZE;
            }
        }
        else
        {
            page_offset = ((index - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
            in_page_offset = ((index - 1) % FLASH_PAGE_HR_RECORD_NUM) * FLASH_HR_ONE_RECORD_SIZE;
        }
        LOG_D("page_offset = %d", page_offset);
        LOG_D("in_page_offset = %d", in_page_offset);

        uint32_t addr = (hr_start_addr[record] + FLASH_PAGE_SIZE * page_offset + in_page_offset);
        LOG_D("addr = %04X", addr);

        Flash_Read(addr, pHrRecord->buf, HR_RECORD_FRAME_LEN);

        LOG_D("Flash_Read_(%d)Record(%d):%04d-%02d-%02d,%02d:%02d", record, index, \
            pHrRecord->Struct.year, pHrRecord->Struct.month, pHrRecord->Struct.day,
            pHrRecord->Struct.hour, pHrRecord->Struct.minute);
    }
    else
    {
        rt_memset(pHrRecord->buf, 0, HR_RECORD_FRAME_LEN);
        LOG_E("Flash_GetMaxNum_(%d)Records Error!: (index_max)%d < %d", record, index_max, index);
    }
}

/**
 * @description:
 * @param {TuFlashHrRecordFrame*} pHrRecord
 * @param {uint16_t} index
 * @return {*}
 */
void Flash_Write_RecordIndex(TuFlashHrRecordFrame* pHrRecord, TeRecord record, uint16_t index)
{
    LOG_D("/*********Flash_Write_RecordIndex***************/");

    uint8_t page_offset = ((index - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
    LOG_D("page_offset = %d", page_offset);
    uint8_t in_page_offset = ((index - 1) % FLASH_PAGE_HR_RECORD_NUM) * FLASH_HR_ONE_RECORD_SIZE;
    LOG_D("in_page_offset = %d", in_page_offset);

    uint32_t addr = (hr_start_addr[record] + FLASH_PAGE_SIZE * page_offset + in_page_offset);
    LOG_D("addr = %04X", addr);
    Flash_Write(addr, pHrRecord->buf, HR_RECORD_FRAME_LEN);

    TuFlashHrRecordFrame pReadRecord;
    Flash_Read(addr, pReadRecord.buf, HR_RECORD_FRAME_LEN);

    LOG_D("(%04X)Flash_Write_(%d)RecordIndex(%d):%04d-%02d-%02d,%02d:%02d", \
            pReadRecord.Struct.header, record, pReadRecord.Struct.index, \
            pReadRecord.Struct.year, pReadRecord.Struct.month, pReadRecord.Struct.day,
            pReadRecord.Struct.hour, pReadRecord.Struct.minute);
}

void Flash_Write_Record(TeRecord record)
{
    LOG_D("/*********Flash_Write_Record***************/");

    uint16_t index_max = Flash_GetMaxIndex_Records(record);
    uint16_t index_new = index_max + 1;
    LOG_D("index_new = %d", index_new);

    uint16_t index_start;
    uint8_t index_start_page_offset;

    uint8_t index_max_page_offset;
    uint8_t index_max_in_page_offset;

    if(index_max <= hr_record_max_num[record])
    {
        index_start = 0;
        index_start_page_offset = 0;
        index_max_page_offset = ((index_max - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
        LOG_D("index_max_page_offset = %d", index_max_page_offset);
    }
    else
    {
        index_start = index_max - (hr_record_max_num[record] - 1);
        LOG_D("index_start = %d", index_start);

        index_start_page_offset = ((index_start - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
        LOG_D("index_start_page_offset = %d", index_start_page_offset);

        index_max_page_offset = ((index_max - 1) / FLASH_PAGE_HR_RECORD_NUM) % hr_record_pages[record];
    }
    index_max_in_page_offset = (index_max - 1) % FLASH_PAGE_HR_RECORD_NUM;
    LOG_D("index_max_in_page_offset = %d", index_max_in_page_offset);

    if((index_max_in_page_offset + 1) == (FLASH_PAGE_HR_RECORD_NUM - 1) \
        && index_max_page_offset == (hr_record_pages[record] - 1))
    {
        Flash_ErasePage_Records(record, 0);
    }
    else if ((index_max_in_page_offset + 1) == (FLASH_PAGE_HR_RECORD_NUM - 1) \
        && index_max_page_offset == index_start_page_offset - 1)
    {
        Flash_ErasePage_Records(record, index_max_page_offset + 1);
    }

    RTC_Get_Time();

    TuFlashHrRecordFrame HrRecord;
    HrRecord.Struct.header = HR_RECORD_FRAME_HEADER;
    HrRecord.Struct.index = index_new;
    HrRecord.Struct.year = RtcDateTime.year;
    HrRecord.Struct.month = RtcDateTime.month;
    HrRecord.Struct.day = RtcDateTime.day;
    HrRecord.Struct.hour = RtcDateTime.hour;
    HrRecord.Struct.minute = RtcDateTime.minute;

    Flash_Write_RecordIndex(&HrRecord, record, index_new);
}

static void TEST_Flash_Write_Record(TeRecord record, uint16_t num)
{
   for (uint16_t i = 0; i < num; i++)
   {
       Flash_Write_Record(record);
       rt_thread_mdelay(2);
       // BSP_WDG_Loop();
   }
}
MSH_CMD_EXPORT(TEST_Flash_Write_Record, "record(0~6), num");

void Flash_ErasePage_ConfigInfo(void)
{
    BSP_Flash_FastUnLock();
    FLASH_ErasePage_Fast(FLASH_CONFIG_INFO_START_ADDR);
    BSP_Flash_FastLock();
}

/**
 * @description: 擦除配置页前保存当前数据到page_buf
 * @param {uint8_t} *page_buf
 * @return {*}
 */
void Flash_ErasePage_ReadConfigInfo(uint8_t *page_buf)
{
    Flash_Read(FLASH_CONFIG_INFO_START_ADDR, page_buf, FLASH_PAGE_SIZE);

    Flash_ErasePage_ConfigInfo();
}

void Flash_Write_ConfigInfo(uint8_t *page_buf)
{
    Flash_Write(FLASH_CONFIG_INFO_START_ADDR, page_buf, FLASH_PAGE_SIZE);
}

ErrorStatus Flash_GetProductTimeLimit(TuFlashProductTimeLimitFrame *pLimitTime,
                                TeFlashProductTimeLimitId id)
{
    ErrorStatus flag = NoREADY;
    uint32_t addr = 0;

    if(id == kFactoryTimeId)
    {
        addr = FLASH_FACTORY_TIME_START_ADDR;
    }
    else if(id == kExpirationTimeId)
    {
        addr = FLASH_EXPIRATION_TIME_START_ADDR;
    }
    if (addr != 0)
    {
        Flash_Read(addr, pLimitTime->buf, sizeof(TuFlashProductTimeLimitFrame));

        if(pLimitTime->Struct.header == FLASH_PRODUCT_TIME_FRAME_HEADER)
        {
            flag = READY;
            LOG_D("Flash_GetProductTimeLimit(%d):%04d-%02d-%02d,%02d:%02d:%02d", id, \
                    pLimitTime->Struct.year, pLimitTime->Struct.month, pLimitTime->Struct.day,
                    pLimitTime->Struct.hour, pLimitTime->Struct.minute, pLimitTime->Struct.second);
        }
        else
        {
            LOG_E("Flash_GetProductTimeLimit Error!");
        }
    }

    return flag;
}

static void TEST_Flash_GetProductTimeLimit(TeFlashProductTimeLimitId id)
{
   TuFlashProductTimeLimitFrame LimitTime;
   Flash_GetProductTimeLimit(&LimitTime, id);
}
MSH_CMD_EXPORT(TEST_Flash_GetProductTimeLimit, "id(0:FACTORY; 1:EXPIRATION)");

void Flash_SetProductTimeLimit(uint16_t year, uint8_t mon, uint8_t day
        ,uint8_t hour, uint8_t min, uint8_t second, TeFlashProductTimeLimitId id)
{
    TuFlashProductTimeLimitFrame LimitTime;

    uint8_t page_buf[FLASH_PAGE_SIZE];
    Flash_ErasePage_ReadConfigInfo(page_buf);

    LimitTime.Struct.header = FLASH_PRODUCT_TIME_FRAME_HEADER;
    LimitTime.Struct.year = year;
    LimitTime.Struct.month = mon;
    LimitTime.Struct.day = day;
    LimitTime.Struct.hour = hour;
    LimitTime.Struct.minute = min;
    LimitTime.Struct.second = second;

    uint8_t in_page_offset = 0;
    if(id == kFactoryTimeId)
    {
        in_page_offset = (FLASH_FACTORY_TIME_START_ADDR - FLASH_CONFIG_INFO_START_ADDR);
    }
    else if(id == kExpirationTimeId)
    {
        in_page_offset = (FLASH_EXPIRATION_TIME_START_ADDR - FLASH_CONFIG_INFO_START_ADDR);
    }

    for (uint8_t i = 0; i < sizeof(TuFlashProductTimeLimitFrame); i++)
    {
        page_buf[in_page_offset + i] = LimitTime.buf[i];
    }

    Flash_Write_ConfigInfo(page_buf);

    TuFlashProductTimeLimitFrame ReadLimitTime;
    Flash_GetProductTimeLimit(&ReadLimitTime, id);
}
MSH_CMD_EXPORT(Flash_SetProductTimeLimit, "y m d h m s, id(0:FACTORY; 1:EXPIRATION)");

void Set_FactoryRtcTime(void)
{
    RTC_Get_Time();
    LOG_D("%4d-%02d-%02d, %02d:%02d:%02d", RtcDateTime.year, RtcDateTime.month, RtcDateTime.day, \
            RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second);

    Flash_SetProductTimeLimit(RtcDateTime.year, RtcDateTime.month, RtcDateTime.day
        , RtcDateTime.hour, RtcDateTime.minute, RtcDateTime.second, kFactoryTimeId);
}
MSH_CMD_EXPORT(Set_FactoryRtcTime, "Use RTC time Set_FactoryRtcTime");

void Set_ExpirationTime(uint16_t days)
{
    uint32_t factory_seconds, expiration_seconds;
    TuFlashProductTimeLimitFrame LimitTime;

    if (Flash_GetProductTimeLimit(&LimitTime, kFactoryTimeId) == READY)
    {
        factory_seconds = DateTime2Seconds(LimitTime.Struct.year
            , LimitTime.Struct.month, LimitTime.Struct.day, LimitTime.Struct.hour
            , LimitTime.Struct.minute, LimitTime.Struct.second);
        LOG_D("FactoryTime:%04d-%02d-%02d,%02d:%02d"
            , LimitTime.Struct.year, LimitTime.Struct.month, LimitTime.Struct.day
            , LimitTime.Struct.hour, LimitTime.Struct.minute, LimitTime.Struct.second);

        expiration_seconds = factory_seconds + days * 24 * 3600;
        // LOG_D("factory_seconds = %d, expiration_seconds = %d", factory_seconds, expiration_seconds);

        TsRtcDateTime DateTime;
        Seconds2DateTime(expiration_seconds, &DateTime);
        Flash_SetProductTimeLimit(DateTime.year, DateTime.month, DateTime.day
            , DateTime.hour, DateTime.minute, DateTime.second, kExpirationTimeId);

        if (Flash_GetProductTimeLimit(&LimitTime, kExpirationTimeId) == READY)
        {
            LOG_D("Flash_GetProductTimeLimit:%04d-%02d-%02d,%02d:%02d:%02d"
                , LimitTime.Struct.year, LimitTime.Struct.month, LimitTime.Struct.day
                , LimitTime.Struct.hour, LimitTime.Struct.minute, LimitTime.Struct.second);
        }
    }
    // TuFlashProductTimeLimitFrame LimitTime;
    if (Flash_GetProductTimeLimit(&LimitTime, kExpirationTimeId) == READY)
    {
        Mq.expiration_seconds = DateTime2Seconds(LimitTime.Struct.year, \
            LimitTime.Struct.month, LimitTime.Struct.day, LimitTime.Struct.hour, \
            LimitTime.Struct.minute, LimitTime.Struct.second);
        LOG_D("mq_expiration_time:%04d-%02d-%02d,%02d:%02d", \
                    LimitTime.Struct.year, LimitTime.Struct.month, LimitTime.Struct.day,
                    LimitTime.Struct.hour, LimitTime.Struct.minute, LimitTime.Struct.second);
    }
}
MSH_CMD_EXPORT(Set_ExpirationTime, "(uint16_t) days");

uint16_t Flash_GetMQ_AlarmValue(void)
{
    uint16_t value = 0;

    TuFlashMqAlarmFrame MqCall;

    Flash_Read(FLASH_MQ_ALARM_VALUE_ADDR, MqCall.buf, sizeof(TuFlashMqAlarmFrame));

    if(MqCall.Struct.header == FLASH_CONFIG_INFO_FRAME_HEADER)
    {
        value = MqCall.Struct.value;
        LOG_D("Flash_GetMQ_AlarmValue: %d", value);
    }
    else
    {
        LOG_E("Flash_GetMQ_AlarmValue Error!");
    }

    return value;
}
MSH_CMD_EXPORT(Flash_GetMQ_AlarmValue, "Flash_GetMQ_AlarmValue");

void Flash_SetMQ_AlarmValue(uint16_t value)
{
    TuFlashMqAlarmFrame MqCal;

    uint8_t page_buf[FLASH_PAGE_SIZE];
    Flash_ErasePage_ReadConfigInfo(page_buf);

    MqCal.Struct.header = FLASH_CONFIG_INFO_FRAME_HEADER;
    MqCal.Struct.value = value;

    uint8_t in_page_offset = (FLASH_MQ_ALARM_VALUE_ADDR - FLASH_CONFIG_INFO_START_ADDR);

    for (uint8_t i = 0; i < sizeof(TuFlashMqAlarmFrame); i++)
    {
        page_buf[in_page_offset + i] = MqCal.buf[i];
    }

    Flash_Write_ConfigInfo(page_buf);

    LOG_D("Flash_GetMQ_AlarmValue() = %d", Flash_GetMQ_AlarmValue());
}
MSH_CMD_EXPORT(Flash_SetMQ_AlarmValue, "value(uint16_t)");

ErrorStatus Flash_GetTotalRecord(TsTotalRecords *pTotalRecords)
{
    ErrorStatus flag = NoREADY;

    pTotalRecords->alarm = Flash_GetNum_Records(kRecordAlarm);
    pTotalRecords->alarm_rcy = Flash_GetNum_Records(kRecordAlarmRcy);
    pTotalRecords->fault = Flash_GetNum_Records(kRecordFault);
    pTotalRecords->fault_rcy = Flash_GetNum_Records(kRecordFaultRcy);
    pTotalRecords->power_failure =Flash_GetNum_Records(kRecordPowerFailure);
    pTotalRecords->power_on = Flash_GetNum_Records(kRecordPowerOn);
    pTotalRecords->sensor_failure = Flash_GetNum_Records(kRecordSensorFailure);

    flag = READY;

    return flag;
}

ErrorStatus Flash_GetRecord(TeFrameC2 record_type, uint8_t index, TsRecordsTime *pRecordsTime)
{
    ErrorStatus flag = NoREADY;

    if(kNumOfRecords < record_type && record_type < kGetCurrentTime)
    {
        TuFlashHrRecordFrame pHrReadRecord;
        Flash_Read_Record(&pHrReadRecord, record_type - 1, index);

        LOG_D("(%d)Flash_GetRecord(%d):%04d-%02d-%02d,%02d:%02d", \
            record_type - 1, pHrReadRecord.Struct.index, \
            pHrReadRecord.Struct.year, pHrReadRecord.Struct.month, pHrReadRecord.Struct.day,
            pHrReadRecord.Struct.hour, pHrReadRecord.Struct.minute);

        pRecordsTime->year_h = pHrReadRecord.Struct.year / 256;
        pRecordsTime->year_l = pHrReadRecord.Struct.year % 256;
        pRecordsTime->month = pHrReadRecord.Struct.month;
        pRecordsTime->day = pHrReadRecord.Struct.day;
        pRecordsTime->hour = pHrReadRecord.Struct.hour;
        pRecordsTime->minute = pHrReadRecord.Struct.minute;

        flag = READY;
    }

    return flag;
}

/**
 * @description:
 * @return {*}
 */
void BSP_Flash_EraseRecodrs(void)
{
    for (uint8_t record = 0; record <= kRecordSensorFailure; record++)
    {
        for (uint8_t i = 0; i < hr_record_pages[record]; i++)
        {
            Flash_ErasePage_Records(record, i);
        }
        // LOG_D("/**BSP_Flash_First_Init (%d)**/", record);
    }
    Flash_ErasePage_ConfigInfo();
}

int BSP_Flash_Init(void)
{
    if(*(uint16_t *)FLASH_INIT_FLAG_ADDR != FLASH_FIRST_INIT_VALUE)
    {
        LOG_D("BSP_Flash_EraseRecodrs!");
    // if (SysControl.status == kSysProductionModeStatus)
    // {
        // SYS_SetStatus(kSysProductionModeStatus);

        uint8_t flash_init_flag[2];

        flash_init_flag[0] = FLASH_FIRST_INIT_VALUE % 256;
        flash_init_flag[1] = FLASH_FIRST_INIT_VALUE / 256;

        BSP_Flash_EraseRecodrs();

        Flash_Write(FLASH_INIT_FLAG_ADDR, flash_init_flag, sizeof(flash_init_flag));

        Flash_SetProductTimeLimit(2023, 3, 22, 13, 58, 20, kFactoryTimeId);
        Flash_SetProductTimeLimit(2028, 3, 22, 13, 58, 20, kExpirationTimeId);
    }
    return 0;
}
#ifdef RT_USING_COMPONENTS_INIT
INIT_BOARD_EXPORT(BSP_Flash_Init);
#endif

