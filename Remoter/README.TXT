this project is a embedded code based on STM32F103C8T6
check "mindmap" with mindMaster to see the structure of these project
2021/4/7
basic function finished , need to improve:
1: CRC check of transmitted data
2: low pass filter for each channel
3: hardware desgin for safaty-----automatic read a disable signal when the plug is loose







debug log:
when powered at the same time, 2 lora chips will couldn't establish communication propelly, user can reset the remote controler or receiver to avoid this situation.
