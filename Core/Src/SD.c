/*
 * SD.c
 *
 *  Created on: Sep 14, 2020
 *      Author: linus
 */

#include "SD.h"
#include "fatfs.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

FRESULT fresult;  // to store the result
UINT br, bw;   // file read/write count

FATFS fs;  // file system
FIL data_file;  // file
FIL fake_file;  // file
FIL log_file;  // file

uint16_t bufsize (char *buf)
{
	int i=0;
	while (*buf++ != '\0') i++;
	return i;
}


void bufclear (char * buffer)  // clear buffer
{
	for (int i=0; i<BUFLEN; i++)
	{
		buffer[i] = '\0';
	}
}


uint8_t find_file(void){
	DIR dirs;
	BYTE i;
	char *fn;
	FILINFO Finfo;
	WORD AccFiles, AccDirs;
	DWORD AccSize;				/* Work register for fs command */

	if ((fresult = f_opendir(&dirs, SDPath)) == FR_OK) {
			i = strlen(SDPath);
			while (((fresult = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
				if (_FS_RPATH && Finfo.fname[0] == '.') continue;
				fn = Finfo.fname;

				if (Finfo.fattrib & AM_DIR) {
					AccDirs++;
					*(SDPath+i) = '/'; strcpy(SDPath+i+1, fn);
					fresult = find_file();
					*(SDPath+i) = '\0';
					if (fresult != FR_OK) break;
				} else {
					if (DEBUG_PRINT == 1) printf("%s/%s\n", SDPath, fn);
					AccFiles++;
					AccSize += Finfo.fsize;
				}
			}
			return 1;
		}
	return 0;
}


uint8_t init_sd(uint16_t * file_count, uint16_t * log_count){
	/* capacity related variables */
	FATFS *pfs;
	DWORD fre_clust;
	uint32_t total, free_space;

	if (DEBUG_PRINT == 1) printf("mounting SD card...\n");

	FRESULT ret = f_mount(&fs, SDPath, 1);
	if (ret == FR_OK)
	{
		if (DEBUG_PRINT == 1) printf("mounted SD card\n");
	} else {
		if (DEBUG_PRINT == 1) printf("no SD connection established\n");
	  if (DEBUG_PRINT == 1) printf("error: %d\n",ret);
	  return 0;
	}

	/* Check free space */
	f_getfree(SDPath, &fre_clust, &pfs);

	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	if (DEBUG_PRINT == 1) printf("SD CARD Total Size: \t%lu\n",total);
	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	if (DEBUG_PRINT == 1) printf("SD CARD Free Space: \t%lu\n",free_space);

	DIR dirs;
	char *fn;
	FILINFO Finfo;

	uint16_t cnt1 = 0;
	uint16_t cnt2 = 0;

	if ((fresult = f_opendir(&dirs, SDPath)) == FR_OK)
	{
			while (((fresult = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0])
			{
				fn = Finfo.fname;
				if (_FS_RPATH && fn[0] == '.') continue;

				if ((fn[0] == 'F') & (fn[1] == 'L'))
				{
					if (DEBUG_PRINT == 1) printf("found flight log: %s \n",fn);
					cnt1 ++;
				} else if ((fn[0] == 'L') & (fn[1] == 'O'))
				{
					if (DEBUG_PRINT == 1) printf("found log file: %s \n",fn);
					cnt2 ++;
				}

			}
	}
	*file_count = cnt1;
	*log_count = cnt2;
	if (DEBUG_PRINT == 1) printf("\n this is the %hu th flight. \n", *file_count);
	if (DEBUG_PRINT == 1) printf("\n this is the %hu th log file. \n", *log_count);
	return 1;
}

uint8_t init_file(char * FILE_NAME, char * LOG_NAME){



	/**************** The following operation is using f_write and f_read **************************/

	/* Create second file with read write access and open it */
	fresult = f_open(&data_file, FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE );

	if (fresult != FR_OK){
		if (DEBUG_PRINT == 1) printf("trying to open datalog file, error-code: %d \n",fresult);
		return 0;
	}
	/* Writing text */
	//char myData[] = "xyx,ghj,acceleration,target position\n 1000,231.25,10,500.5";
	char myData[] = "Time,Armed,Event,State2,h,v,H_SHT,T_SHT,T_CPU,T_BARO1,T_BARO2,T_IMU1,T_IMU2,P_BARO1,P_BARO2,Ax_IMU1,Ay_IMU1,Az_IMU1,Gx_IMU1,Gy_IMU1,Gz_IMU1,Ax_IMU2,Ay_IMU2,Az_IMU2,Gx_IMU2,Gy_IMU2,Gz_IMU2,Ax_H3L,Ay_H3L,Az_H3L,I_BAT1,I_BAT2,V_BAT1,V_BAT2,V_LDR,V_TD1,V_TD2\n";

	fresult = f_write(&data_file, myData, sizeof(myData), &bw);
	/* Close file */
	f_close(&data_file);

	if (fresult != FR_OK){
		if (DEBUG_PRINT == 1) printf ("FLIGHT FILE not created, error-code: %d \n",fresult);
		return 0;
	}
	//bufclear();

	HAL_Delay(100);

	/* Open file to write/ create a file if it doesn't exist */
	fresult = f_open(&log_file, LOG_NAME, FA_CREATE_NEW | FA_WRITE );

	if (fresult != FR_OK){
		if (DEBUG_PRINT == 1) printf("trying to open setuplog file, error-code: %d \n",fresult);
		return 0;
	}


	/* Writing text */

	//sprintf(buffer,"time: %ld,SHT STATE: %d\n ,BARO1 STATE: %d\n ,BARO2 STATE: %d \n ,IMU1 STATE %d\n ,IMU2 STATE %d\n, H3L STATE %d\n",HAL_GetTick(), SHT_STATE,BARO1_STATE,BARO2_STATE,IMU1_STATE,IMU2_STATE,H3L_STATE);

	bw = 0;
	char myLog[128];

	for (int i=0; i<128; i++)
	{
		myLog[i] = '\0';
	}

	sprintf(myLog, "TIMESTAMP, STATE, MSG \n %ld, SD WRITE OK, file name: %s \n", HAL_GetTick(), FILE_NAME);
	fresult = f_write(&log_file, myLog, sizeof(myLog), &bw);

	if (fresult != FR_OK){
		if (DEBUG_PRINT == 1) printf ("LOG FILE not created, error-code: %d \n",fresult);
		return 0;
	}

	/* Close file */
	f_close(&log_file);

	// clearing buffer to show that result obtained is from the file
	//bufclear();

	return 1;

}

float extract_from_str(char* buffer, uint8_t *start, uint8_t *end){
	uint8_t x = *start;
	uint8_t y = *end;
    char c[30];
	for (int j=0; j<30; j++) {
		c[j] = '\0';
	}
	while (buffer[y] != ','){
		y ++;
	}
	*end = y;
	strncpy(c, buffer + x, y - x);
	float ret = strtof(c,NULL);
	return ret;
}

void read_from_SD(char * FILE_NAME, float * TIME, float * P1, float * P2, float * Ax1, float * Ay1, float * Az1, float * Ax2, float * Ay2, float * Az2){

	printf("reading FF file: \n");
	fresult = f_open(&fake_file, FILE_NAME, FA_READ);
	printf("trying to read the file, error-code: %d \n",fresult);

	char buffer[FAKE_LINE_LEN];
	uint8_t x = 0;
	uint8_t y = 0;
	for (int i = 1; i < FAKE_FILE_LEN; i++){
	        f_gets(buffer, f_size(&fake_file), &fake_file);
	        printf("reading line: %d  \n",i);
	        x = 0;
	        y = 0;

			TIME[i] = extract_from_str(buffer, &x, &y);

			x = y + 1;
			y = y + 1;

			Ax1[i] = extract_from_str(buffer, &x, &y);

			x = y + 1;
			y = y + 1;

	 		Ay1[i] = extract_from_str(buffer, &x, &y);

	 		x = y + 1;
			y = y + 1;

			Az1[i] = extract_from_str(buffer, &x, &y);

			x = y + 1;
			y = y + 1;

	 		Ax2[i] = extract_from_str(buffer, &x, &y);

	 		x = y + 1;
			y = y + 1;

	 		Ay2[i] = extract_from_str(buffer, &x, &y);

	 		x = y + 1;
			y = y + 1;

			Az2[i] = extract_from_str(buffer, &x, &y);

			x = y + 1;
			y = y + 1;

			P1[i] = extract_from_str(buffer, &x, &y);

			x = y + 1;
			y = y + 1;

			//printf("%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f \n",TIME[i],P1[i],P2[i],Ax1[i],Ay1[i],Az1[i],Ax2[i],Ay2[i],Az2[i]);

		    char c[30];

			for (int j=0; j<30; j++) {
				c[j] = '\0';
			}

			strncpy(c, buffer + x, 10);

	 		P2[i] = strtof(c,NULL);
	    }

		// Close file
		f_close(&fake_file);


}


void write_to_SD(char * FILE_NAME, char * buffer){

	/*
	char FILE_NAME[11];
	sprintf(FILE_NAME,"FL%05d.CSV",file_count);
	*/

	// Open the file with write access
	fresult = f_open(&data_file, FILE_NAME, FA_OPEN_ALWAYS | FA_WRITE);



	if (fresult == FR_OK)
	{

		if (DEBUG_PRINT == 1) printf("opened file \n");

	} else {

		if (DEBUG_PRINT == 1) printf("error opening file for writing\n");
		if (DEBUG_PRINT == 1) printf("error: %d\n",fresult);

		fresult = f_mount(NULL, SDPath, 1);
		if (fresult == FR_OK) printf ("SD CARD UNMOUNTED successfully...\n");

		FRESULT ret = f_mount(&fs, SDPath, 1);
		if (ret == FR_OK)
		{
			if (DEBUG_PRINT == 1) printf("mounted SD card\n");
		} else {
			if (DEBUG_PRINT == 1) printf("no SD connection established\n");
			if (DEBUG_PRINT == 1) printf("error: %d\n",ret);
		}
	}

	// Move to offset to the end of the file
	fresult = f_lseek((&data_file),f_size(&data_file));

	// write the string to the file
	fresult = f_puts(buffer, &data_file);

	// Close file
	f_close(&data_file);

	bufclear(buffer);

}

void log_to_SD(char * FILE_NAME, char * buffer){

	/*
	char FILE_NAME[11];
	sprintf(FILE_NAME,"FL%05d.CSV",file_count);
	*/

	// Open the file with write access
	fresult = f_open(&log_file, FILE_NAME, FA_OPEN_ALWAYS | FA_WRITE);



	if (fresult == FR_OK)
	{

		if (DEBUG_PRINT == 1) printf("opened file \n");

	} else {

		if (DEBUG_PRINT == 1) printf("error opening log file for writing\n");
		if (DEBUG_PRINT == 1) printf("error: %d\n",fresult);

		fresult = f_mount(NULL, SDPath, 1);
		if (fresult == FR_OK) printf ("SD CARD UNMOUNTED successfully...\n");

		FRESULT ret = f_mount(&fs, SDPath, 1);
		if (ret == FR_OK)
		{
			if (DEBUG_PRINT == 1) printf("mounted SD card\n");
		} else {
			if (DEBUG_PRINT == 1) printf("no SD connection established\n");
			if (DEBUG_PRINT == 1) printf("error: %d\n",ret);
		}
	}

	// Move to offset to the end of the file
	fresult = f_lseek((&log_file),f_size(&log_file));

	// write the string to the file
	fresult = f_puts(buffer, &log_file);

	// Close file
	f_close(&log_file);

	bufclear(buffer);

}


