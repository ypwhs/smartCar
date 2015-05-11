#include "gui_appdef.h"

void GUI_AppAutoCalibration(const char* path)
{
    int hFile;
    GUI_TouchData tData;
    hFile = open(path, O_RDONLY , 0);
    if(hFile < 0)
    {
//        GUI_MessageBox("No touch data\r\n", path, GUI_MESSAGEBOX_CF_MODAL);
        hFile = open(path, O_RDWR | O_CREAT, 0);
        tData.magic = 'T';
        GUI_ExecCalibrationDialog(&tData);
        write(hFile, &tData, sizeof(GUI_TouchData));
    }
    else
    {
        read(hFile, &tData, sizeof(GUI_TouchData));
        GUI_TOUCH_Calibrate(0, tData.aLogX[0], tData.aLogX[1], tData.aPhysX[0], tData.aPhysX[1]); /* Calibrate X-axis */
        GUI_TOUCH_Calibrate(1, tData.aLogY[0], tData.aLogY[1], tData.aPhysY[0], tData.aPhysY[1]); /* Calibrate Y-axis */
    }
    close(hFile);
}

#ifdef FINSH_USING_MSH
#include "finsh.h"
extern char working_directory[];

int ui_calibration(int argc, char** argv)
{
    gui_msg_t msg;
    char *fullpath;
    if(argc != 2)
    {
        return -1;
    }
     
    fullpath = dfs_normalize_path(NULL, argv[1]);

    msg.cmd = 2;
    msg.exec = GUI_AppAutoCalibration;
    msg.parameter = (void *)fullpath;
    rt_mq_send(guimq, &msg, sizeof(msg));
    return 0;
}

MSH_CMD_EXPORT(ui_calibration, show a picture file.);
#endif




