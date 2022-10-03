#include "sound.h"
#include "../types.h"
#include <seven/hw/dma.h>
#include <seven/hw/timer.h>
#include "../entity/soundlist.h"

void SoundInit(){
	REG_DMA1DST = (u32 *)0x40000A0;
	REG_DMA2DST = (u32 *)0x40000A4;
	
	REG_DMA1CNT = (DMA_DST_FIXED | DMA_SRC_INCREMENT | DMA_REPEAT | DMA_32BIT | DMA_START_SOUND);
	REG_DMA1CNT = (DMA_DST_FIXED | DMA_SRC_INCREMENT | DMA_REPEAT | DMA_32BIT | DMA_START_SOUND);
	
	REG_TM0VAL = 0xffff - 512 + 1; //32.768khz
	REG_TM1VAL = 0xffff - 512 + 1; //32.768khz
	
	REG_TM0CNT = (TIMER_FREQ_16MHZ | TIMER_ENABLE);
	REG_TM1CNT = (TIMER_FREQ_16MHZ | TIMER_ENABLE);
	REG_TM2CNT = (TIMER_FREQ_64KHZ);
	REG_TM3CNT = (TIMER_FREQ_64KHZ);
	
	
	*(vu16 *)0x4000084 = 0x80;	//DMA channel master enable
	*(vu16 *)0x4000082 = 0x730C; //enable both DMA channels at max volume on both left and right.

	
}

u32 CheckSoundProgress(SoundData *CurrentSound){ //returns how much over we are if we've reached the end
	static u16 timer2LastVal = 0;
	u16 timer2CurrentVal;
	static u16 timer3LastVal = 0;
	u16 timer3CurrentVal;
	static u32 channel0Duration = 0;
	static u32 channel1Duration = 0;
	u32 soundLength;
	static u32 testCounter = 0;
	
	if (CurrentSound->channel == 0){
		soundLength = CurrentSound->length;
		timer2CurrentVal = timerGetValue(2);
		channel0Duration += ((timer2CurrentVal - timer2LastVal) & 0xffff) >> 1;
		timer2LastVal = timer2CurrentVal;
		if(channel0Duration >= soundLength){
			testCounter += channel0Duration;
			channel0Duration -= soundLength;
			return channel0Duration;
		}
	}
	else {
		soundLength = CurrentSound->length;
		timer3CurrentVal = timerGetValue(3);
		channel1Duration += ((timer3CurrentVal - timer3LastVal) & 0xffff) >> 1;
		timer3LastVal = timer3CurrentVal;
		if(channel1Duration >= soundLength){
			channel1Duration -= soundLength;
			return channel1Duration;
		}
	}
	return 0;
}

void SoundPlay(SoundData *CurrentSound){
	if (CurrentSound->channel == 0){
		REG_DMA1SRC = CurrentSound->sound;
		dmaDisable(1);
		dmaEnable(1);
		REG_TM2VAL = 0; //used to measure DMA sound 0 duration
		timerDisable(2);
		timerEnable(2); //timer ticks twice per sample
	}
	else{
		REG_DMA2SRC = CurrentSound->sound;
		dmaDisable(2);
		dmaEnable(2);
		REG_TM3VAL = 0; //used to measure DMA sound 1 duration
		timerDisable(3);
		timerEnable(3); //timer ticks twice per sample
	}
}

void SoundRepeat(SoundData *CurrentSound, u32 soundDuration){
	if (CurrentSound->channel == 0){
		REG_DMA1SRC = CurrentSound->sound + soundDuration;
		dmaDisable(1);
		dmaEnable(1);
		REG_TM2VAL = 0; //used to measure DMA sound 0 duration
		timerDisable(2);
		timerEnable(2); //timer ticks twice per sample
	}
	else{
		u16 playedLength = timerGetValue(3) >> 1;
		REG_DMA1SRC = CurrentSound->sound + soundDuration;
		dmaDisable(2);
		dmaEnable(2);
		REG_TM3VAL = 0; //used to measure DMA sound 1 duration
		timerDisable(3);
		timerEnable(3); //timer ticks twice per sample
	}
}

void SoundStop(SoundData *CurrentSound){
	if(CurrentSound->channel == 0){
		dmaDisable(1);
	}
	else{
		dmaDisable(2);
	}
}