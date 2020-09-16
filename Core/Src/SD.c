/*
 * SD.c
 *
 *  Created on: Sep 14, 2020
 *      Author: linus
 */

#include "SD.h"
#include "fatfs.h"


FRESULT fresult;  // to store the result
UINT br, bw;   // file read/write count

FATFS fs;  // file system
FIL data_file;  // file
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
					printf("%s/%s\n", SDPath, fn);
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

	printf("mounting SD card...\n");

	FRESULT ret = f_mount(&fs, SDPath, 1);
	if (ret == FR_OK)
	{
	  printf("mounted SD card\n");
	} else {
	  printf("no SD connection established\n");
	  printf("error: %d\n",ret);
	  return 0;
	}

	/* Check free space */
	f_getfree(SDPath, &fre_clust, &pfs);

	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	printf("SD CARD Total Size: \t%lu\n",total);
	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	printf("SD CARD Free Space: \t%lu\n",free_space);

	DIR dirs;
	char *fn;
	FILINFO Finfo;

	if ((fresult = f_opendir(&dirs, SDPath)) == FR_OK) {
			while (((fresult = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
				fn = Finfo.fname;

				if (_FS_RPATH && fn[0] == '.') continue;

				if ((fn[0] == 'F') & (fn[1] == 'L'))
				{
					printf("found flight log: %s \n",fn);
					*file_count ++;
				} else if ((fn[0] == 'L') & (fn[1] == 'O'))
				{
					printf("found log file: %s \n",fn);
					*log_count ++;
				}

			}
	}
	printf("\n this is the %hu th flight. \n", *file_count);
	return 1;
}

uint8_t init_file(uint16_t * file_count, uint16_t * log_count){


	char FILE_NAME[12];
	sprintf(FILE_NAME,"FL%04d.CSV",*file_count);
	printf("saving %s ...",FILE_NAME);

	char LOG_NAME[11];
	sprintf(LOG_NAME,"LOG%02d.CSV",*file_count);
	printf("saving %s ...",LOG_NAME);

	/**************** The following operation is using f_write and f_read **************************/

	/* Create second file with read write access and open it */
	fresult = f_open(&data_file, FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE );

	/* Writing text */
	//char myData[] = "xyx,ghj,acceleration,target position\n 1000,231.25,10,500.5";
	char myData[] = "Time,Armed,Event,State2,h,v,H_SHT,T_SHT,T_CPU,T_BARO1,T_BARO2,T_IMU1,T_IMU2,P_BARO1,P_BARO2,Ax_IMU1,Ay_IMU1,Az_IMU1,Gx_IMU1,Gy_IMU1,Gz_IMU1,Ax_IMU2,Ay_IMU2,Az_IMU2,Gx_IMU2,Gy_IMU2,Gz_IMU2,Ax_H3L,Ay_H3L,Az_H3L,I_BAT1,I_BAT2,V_BAT1,V_BAT2,V_LDR,V_TD1,V_TD2\n";

	fresult = f_write(&data_file, myData, sizeof(myData), &bw);
	printf ("log.txt created and data is written\n");

	/* Close file */
	f_close(&data_file);

	//bufclear();

	/* Open file to write/ create a file if it doesn't exist */
	fresult = f_open(&log_file, LOG_NAME, FA_CREATE_NEW | FA_WRITE );

	if (fresult != FR_OK){
		printf("trying to open setuplog file, error-code: %d \n",fresult);
		return 0;
	}


	/* Writing text */

	//sprintf(buffer,"time: %ld,SHT STATE: %d\n ,BARO1 STATE: %d\n ,BARO2 STATE: %d \n ,IMU1 STATE %d\n ,IMU2 STATE %d\n, H3L STATE %d\n",HAL_GetTick(), SHT_STATE,BARO1_STATE,BARO2_STATE,IMU1_STATE,IMU2_STATE,H3L_STATE);

	char myLog[128];
	sprintf(myLog, "TIMESTAMP, STATE, MSG \n %ld, SD WRITE OK, file_count: %d \n", HAL_GetTick(), *file_count);
	fresult = f_write(&log_file, myLog, sizeof(myLog), &bw);

	printf ("log.txt created and data is written %d \n",fresult);

	/* Close file */
	fresult = f_close(&log_file);

	printf("trying to close the file, error-code: %d \n",fresult);

	// clearing buffer to show that result obtained is from the file
	//bufclear();

	return 1;

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

	//bufclear();

}


