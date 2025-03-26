#### 修改记录

| 日期       | 修改内容                                                     |
| ---------- | ------------------------------------------------------------ |
| 2025-03-25 | 增加README.md、LICENSE、.gitignore文件                       |
| 2025-03-25 | 增加ADC、蜂鸣器、按键、LCD、LED、PWM、舵机、串口和WS2812部分的功能 |
| 2025-03-26 | 完善串口代码，增加RS485模块的功能                            |

#### 功能说明

- ADC
  - 采集PB0引脚（目前暂时与PB1(PWM)引脚连接在一起）的转换值
  - 采集内部温度传感器的转换值并根据数据手册提供的公式计算出温度值
  - 采集内部1.2V参考电压的转换值，并计算出电压值
- 蜂鸣器
  - 开机响一声
  - 按键短按提示音
  - 按键长按提示音
- 按键
  - MD键短按，在主界面模式下，切换LCD主界面
  - MD键长按，切换LCD显示模式（主界面模式和设置模式）
  - LT键短按，在舵机主界面下，控制舵机顺时针转动1度
  - LT键长按，在舵机主界面下，控制舵机一直顺时针转动
  - RT键短按，在舵机主界面下，控制舵机逆时针转动1度
  - RT键长按，在舵机主界面下，控制舵机一直逆时针转动
- LCD
  - 显示ADC转换值
  - 显示舵机角度
  - 有按键按下时显示按键状态
- LED
  - LED1支持呼吸灯、闪烁灯和开关灯模式
  - LED2仅支持开关灯模式
- PWM
  - 可设置频率范围16Hz - 100kHz
  - 可设置占空比（频率较高时实际占空比会有误差）
- RS485
	- 通过DMA发送和接收
	- MODBUS单个寄存器的读写
	- 自定义MODBUS格式
- 舵机
  - 可设置角度0 - 180度
- 串口
  - 通过DMA发送和接收
  - 回显接收到的数据
- WS2812
  - 以红、黄、绿、青、蓝、紫的顺序循环实现呼吸灯