M3508 BLDC & C620 ESC 3 loop position control 
原始电流数据经过一阶低通滤波器，采样频率1khz，截止频率30hz
曾经试过产生4阶巴特沃斯滤波器，效果差不多。

debug log：
先调内环再调外环
内环的调节ramp要大大大于外环，内环如果太小可能导致overshot太大。
