
2020/5/18 12:11 
该文件基于之前之前rasperry pi作为顶层控制的底层逻辑版本，为了简化结构和接线，去除rasperry pi的顶层逻辑，全部由底层实现遥控器接收机的信号处理（UART+DMA）,IMU的信号接收处理（UART+DMA)
12:55
调试成功，在主函数中显示收到的数据。下一步将信息处理函数放在中断中。
15:00 
SBUS中断解析成功，下一步逐渐测试新板功能
2020/5/20
遥控器数据转换
2021/4/21
新版遥控器取代SBUS初步成功，需要注意通信速度高了之后DMA接收会错位（1个字节），应该有更好的解决方案，参考https://github.com/Jelly-LY/RcProtocolAnalysis.git
目前用的暴力方法，接收buff为2个完整数据包大小(44 byte), 接收中断中判断帧头位置取一个完整的数据包。



Error info
red----lose remote signal 
Green-----balance 数据稳定之后亮起

5-28原定擦窗取消，回来修正：
计划修改：propeller roller 同时初始化，初始化成功之前锁死cable motor;
cable motor功率检测;
led 灯条

5-29尝试改解锁逻辑

6-5 给balance模式下的propeller输出限幅

6-10 modbus返回数据的分析

6-11 按键控制通讯，回传数据直接返回PC

6-11 返回数据正常，加入DMA接收判断返回数据长度

	18:38 调试成功，串口输出两边功率
12:26测试git

7-17: 功率超出加上5s停飞限制

