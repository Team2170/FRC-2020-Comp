@echo off
set /p filename="Enter FileName: "
scp lvuser@10.21.70.2:/home/lvuser/VisionLogs/%filename% Vision_Logs/%filename%
PAUSE
