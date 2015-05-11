#ifndef __USBDESC_H_
#define	__USBDESC_H_

//设备描述符
const uint8_t Device_Descriptor[18]= 
{
 	0x12,	   //bLength域，描述符的长度：18字节
	0x01,	   //bDescriptorType域，描述符类型：0x01表示本描述符为设备描述符）
	0x00,0x02, //bcdUSB域，USB规范版本号（采用BCD码）：2.0 
	0x00,	   //bDeviceClass域，设备类代码 一般为0
	0x00,	   //bDeviceSubClass域，设备子类代码	bDeviceClass为0时bDeviceSubClass也必须为0
	0x00,	   //bDeviceProtocol域，设备协议代码（0x00表示不使用任何设备类协议）			
	0x20,	   //bMaxPacketSize0域，端点0支持最大数据包的长度：32字节
	0x88,0x88, //idVendor域，供应商ID（VID）
	0x01,0x00, //idProduct域，产品ID（PID）
	0x00,0x00, //bcdDevice域，设备版本号（采用BCD码）
	0x01,      //iManufacturer域，供应商的字符串描述符索引：1
	0x02,	   //iProduct域，产品的字符串描述符索引：2
	0x03,	   //iSerialNumber域，设备序号的字符串描述符索引：3
	0x01	   //bNumConfigurations域，该USB设备支持的配置数目：1个
};

//USB报告描述符的定义
const uint8_t Report_Descriptor[]=
{
 //每行开始的第一字节为该条目的前缀，前缀的格式为：
 //D7~D4：bTag。D3~D2：bType；D1~D0：bSize。以下分别对每个条目注释。
 //这是一个全局（bType为1）条目，选择用途页为普通桌面Generic Desktop Page(0x01)
 //后面跟一字节数据（bSize为1），后面的字节数就不注释了，
 //自己根据bSize来判断。
 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
 //这是一个局部（bType为2）条目，说明接下来的应用集合用途用于鼠标
 0x09, 0x02, // USAGE (Mouse)
 //这是一个主条目（bType为0）条目，开集合，后面跟的数据0x01表示
 //该集合是一个应用集合。它的性质在前面由用途页和用途定义为
 //普通桌面用的鼠标。
 0xa1, 0x01, // COLLECTION (Application)
 //这是一个局部条目。说明用途为指针集合
 0x09, 0x01, //   USAGE (Pointer)
 //这是一个主条目，开集合，后面跟的数据0x00表示该集合是一个
 //物理集合，用途由前面的局部条目定义为指针集合。
 0xa1, 0x00, //   COLLECTION (Physical)
 //这是一个全局条目，选择用途页为按键（Button Page(0x09)）
 0x05, 0x09, //     USAGE_PAGE (Button)
 //这是一个局部条目，说明用途的最小值为1。实际上是鼠标左键。
 0x19, 0x01, //     USAGE_MINIMUM (Button 1)
 //这是一个局部条目，说明用途的最大值为3。实际上是鼠标中键。
 0x29, 0x03, //     USAGE_MAXIMUM (Button 3)
 //这是一个全局条目，说明返回的数据的逻辑值（就是我们返回的数据域的值啦）
 //最小为0。因为我们这里用Bit来表示一个数据域，因此最小为0，最大为1。
 0x15, 0x00, //     LOGICAL_MINIMUM (0)
 //这是一个全局条目，说明逻辑值最大为1。
 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
 //这是一个全局条目，说明数据域的数量为三个。
 0x95, 0x03, //     REPORT_COUNT (3)
 //这是一个全局条目，说明每个数据域的长度为1个bit。
 0x75, 0x01, //     REPORT_SIZE (1)
 //这是一个主条目，说明有3个长度为1bit的数据域（数量和长度
 //由前面的两个全局条目所定义）用来做为输入，
 //属性为：Data,Var,Abs。Data表示这些数据可以变动，Var表示
 //这些数据域是独立的，每个域表示一个意思。Abs表示绝对值。
 //这样定义的结果就是，第一个数据域bit0表示按键1（左键）是否按下，
 //第二个数据域bit1表示按键2（右键）是否按下，第三个数据域bit2表示
 //按键3（中键）是否按下。
 0x81, 0x02, //     INPUT (Data,Var,Abs)
 //这是一个全局条目，说明数据域数量为1个
 0x95, 0x01, //     REPORT_COUNT (1)
 //这是一个全局条目，说明每个数据域的长度为5bit。
 0x75, 0x05, //     REPORT_SIZE (5)
 //这是一个主条目，输入用，由前面两个全局条目可知，长度为5bit，
 //数量为1个。它的属性为常量（即返回的数据一直是0）。
 //这个只是为了凑齐一个字节（前面用了3个bit）而填充的一些数据
 //而已，所以它是没有实际用途的。
 0x81, 0x03, //     INPUT (Cnst,Var,Abs)
 //这是一个全局条目，选择用途页为普通桌面Generic Desktop Page(0x01)
 0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
 //这是一个局部条目，说明用途为X轴
 0x09, 0x30, //     USAGE (X)
 //这是一个局部条目，说明用途为Y轴
 0x09, 0x31, //     USAGE (Y)
 //这是一个局部条目，说明用途为滚轮
 0x09, 0x38, //     USAGE (Wheel)
 //下面两个为全局条目，说明返回的逻辑最小和最大值。
 //因为鼠标指针移动时，通常是用相对值来表示的，
 //相对值的意思就是，当指针移动时，只发送移动量。
 //往右移动时，X值为正；往下移动时，Y值为正。
 //对于滚轮，当滚轮往上滚时，值为正。
 0x15, 0x81, //     LOGICAL_MINIMUM (-127)
 0x25, 0x7f, //     LOGICAL_MAXIMUM (127)
 //这是一个全局条目，说明数据域的长度为8bit。
 0x75, 0x08, //     REPORT_SIZE (8)
 //这是一个全局条目，说明数据域的个数为3个。
 0x95, 0x03, //     REPORT_COUNT (3)
 //这是一个主条目。它说明这三个8bit的数据域是输入用的，
 //属性为：Data,Var,Rel。Data说明数据是可以变的，Var说明
 //这些数据域是独立的，即第一个8bit表示X轴，第二个8bit表示
 //Y轴，第三个8bit表示滚轮。Rel表示这些值是相对值。
 0x81, 0x06, //     INPUT (Data,Var,Rel)
 //下面这两个主条目用来关闭前面的集合用。
 //我们开了两个集合，所以要关两次。bSize为0，所以后面没数据。
 0xc0,       //   END_COLLECTION
 0xc0        // END_COLLECTION
};
//通过上面的报告描述符的定义，我们知道返回的输入报告具有4字节。
//第一字节的低3位用来表示按键是否按下的，高5位为常数0，无用。
//第二字节表示X轴改的变量，第三字节表示Y轴的改变量，第四字节表示
//滚轮的改变量。我们在中断端点1中应该要按照上面的格式返回实际的
//鼠标数据。
///////////////////////////报告描述符完毕////////////////////////////
//字符串描述符0
const uint8_t String_Descriptor0[4] = 
{
	0x04,		   //bLength域，长度：4字节
	0x03,		   //bDescriptorType域，字符串描述符
	//0x0409为美式英语的ID
	0x09,0x04	   //wLANDID0域，英语
};

const uint8_t String_Descriptor1[22]={
22,         //该描述符的长度为22字节
0x03,       //字符串描述符的类型编码为0x03
0x59, 0x00, //Y
0x61, 0x00, //a
0x6e, 0x00, //n
0x64, 0x00, //d
0x6c, 0x00, //l
0x64, 0x00, //d
0x2d, 0x00, //-
0x48, 0x00, //H
0x49, 0x00, //I
0x44, 0x00, //D
//注：请将最后一个多余的逗号删除
};

const uint8_t String_Descriptor2[22]={
22,         //该描述符的长度为22字节
0x03,       //字符串描述符的类型编码为0x03
0x59, 0x00, //Y
0x61, 0x00, //a
0x6e, 0x00, //n
0x64, 0x00, //d
0x6c, 0x00, //l
0x64, 0x00, //d
0x2d, 0x00, //-
0x48, 0x00, //H
0x49, 0x00, //I
0x44, 0x00, //D
//注：请将最后一个多余的逗号删除
};

//字符串描述符3
const uint8_t String_Descriptor3[] = 
{
	0x14,			//bLength域，长度：20字节
	0x03,		    //bDescriptorType域，字符串描述符
	'T',0x00,	    //
	'E',0x00,	
	'S',0x00,	
	'T',0x00,
	'_',0x00,	    //
  'v',0x00,       //
	'2',0x00,	
	'.',0x00,	
	'2',0x00
};	 


//配置描述符
const uint8_t Configuration_Descriptor[9+9+9+7+7]= 
{
	//配置描述符
	0x09,		    //bLength域，配置描述符的长度：9字节
	0x02,		    //bDescriptorType域：0x02表示本描述符为配置描述符
	sizeof(Configuration_Descriptor)&0xFF,(sizeof(Configuration_Descriptor)>>8)&0xFF,	    //wTotalLength域，配置信息的总长度（包括配置、接口和端点）：32字节
	0x01,		    //bNumInterfaces域，该配置所支持的接口数（至少一个）：1
	0x01,		    //bConfigurationValue域，配置值：1 
	0x00,		    //iConfiguration域，配置字符串描述符索引：0
	0x80,		    //bmAttibutes域，配置的属性(具有总线供电、自供电及过程唤醒的特性) 
	                //位7：1-必须为1；位6：0-自供电；位5：0-不支持远程唤醒
	0x32,		    //MaxPower域，设备从总线提取的最大电流以2mA为单位：50*2mA＝100mA

	//接口描述符
	0x09,           //bLength域，接口描述符长度：9字节
	0x04,           //bDescriptorType域：0x04表示本描述符为接口描述符
	0x00,           //bInterfaceNumber域，接口号
	0x00,           //bAlternateSetting域，接口的可替换设置值
	0x02,           //bNumEndpoints域，接口使用的端点数（除端点0）：2
	0x03,           //bInterfaceClass域，接口所属的USB设备类：0xFF表示供应商自定义
	0x02,           //bInterfaceSubClass域，接口所属的USB设备子类：0xFF表示供应商自定义
	0x01,           //bInterfaceProtocol域，接口采用的USB设备类协议：0xFF表示供应商自定义
	0x00,           //iInterface域，接口字符串描述符的索引：0

 /******************HID描述符************************/
 //bLength字段。本HID描述符下只有一个下级描述符。所以长度为9字节。
 0x09,
 //bDescriptorType字段。HID描述符的编号为0x21。
 0x21,
 //bcdHID字段。本协议使用的HID1.1协议。注意低字节在先。
 0x10,
 0x01,
 //bCountyCode字段。设备适用的国家代码，这里选择为美国，代码0x21。
 0x21,
 //bNumDescriptors字段。下级描述符的数目。我们只有一个报告描述符。
 0x01,
 //bDescriptorType字段。下级描述符的类型，为报告描述符，编号为0x22。
 0x22,
 //bDescriptorLength字段。下级描述符的长度。下级描述符为报告描述符。
 sizeof(Report_Descriptor)&0xFF,
 (sizeof(Report_Descriptor)>>8)&0xFF,  
  
	 //端点OUT描述符
	0x07,           //bLength域，端点描述符长度：7字节
	0x05,           //bDescriptorType域：0x05表示本描述符为端点描述符
	0x82,           //bEndpointAddress域，端点号和传输方向：端点2、IN
	0x03,           //bmAttributes域，端点特性：数据端点、块传输
	0x20,0x00,      //wMaxPacketSize域，端点支持最大数据包长度：32字节          
	0x64,           //bInterval域，轮询间隔，以ms为单位。

	 //端点IN描述符
	0x07,           //bLength域，端点描述符长度：7字节
	0x05,           //bDescriptorType域：0x05表示本描述符为端点描述符
	0x03,           //bEndpointAddress域，端点号和传输方向：端点3、OUT   
	0x03,           //bmAttributes域，端点特性：数据端点、块传输
	0x20,0x00,      //wMaxPacketSize域，端点支持最大数据包长度：32字节        
	0x64,           //bInterval域，轮询间隔，以ms为单位。
};

#endif

