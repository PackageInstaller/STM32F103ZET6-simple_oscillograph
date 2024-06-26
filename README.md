# 基于野火指南者开发板的简易示波器，使用emwin544+freertos，用片上adc采样，最高1MHz采样


## 运行图片

测得另一个单片产生的PWM波

<img src="https://github.com/PackageInstaller/STM32F103ZET6-simple_oscillograph/blob/master/%E8%BF%90%E8%A1%8C%E6%88%AA%E5%9B%BE.jpg" alt="运行截图" style="zoom:33%;" />

## 功能总览
> 1. 界面完全更新
> 2. 可以显示波形或者FFT
> 3. 最高采样率1M，最低500
> 4. 使用FFT测量波形频率（在采样率很高时由于FFT点数依旧为1024会不准），显示峰峰值和最小值
> 5. 按键调整触发电平大小、采样率、电压倍率、水平位移、垂直位移、DAC频率、N个显示点代表1个采样点
> 6. 按键操作为：单击Key1为增大、上移、左移，Key2相反；双击Key1选择下一个参数，双击Key2选择上一个参数；三击调节DAC正弦波频率
> 7. 屏幕右上角的按钮可以调整显示FFT或者波形，左上角的按钮可以暂停波形
> 8. 只有自动触发
## 没有的功能（以后可能添加）
> 1. 正常触发
> 2. 波形保存
> 3. 自动设置
> 4. 多通道，目前只有一个通道
>

## 引脚说明

PA4和PA5输出DAC正弦波，PC5作为ADC的输入，两个按键分别为PA0和PC13，串口用USART1，PA9、PA10收发。
