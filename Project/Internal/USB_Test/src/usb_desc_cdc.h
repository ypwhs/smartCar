/*
 *
 * 文件名: USB_DESC.h
 * 用途: 在该文件中定义了USB 描述符
 *
 * 说明:本页代码基于Freescale官方示例代码修改，源代码文件为 USB_DESC.h
 *    在本文件中将设备描述符中的bcdUSB和bcdDevice修改成2.0版本
 */
#ifndef __USB_DESC_H__
#define __USB_DESC_H__


const u8 Device_Descriptor[18]= 
{
 	0x12,		      //blength           设备描述符的字节长度
	0x01,		      //bDescriptor       表示该描述符的类型
	0x00,0x02,            //bcdUSB ver R=1.10 设置USB的版本为USB2.0,
	0x02,		      //bDeviceClass      设置CDC类
	0x00,		      //bDeviceSubClass			
	0x00,		      //bDeviceProtocol			
	0x40,		      //bMaxPacketSize0   长度为64个字节
	0xA2,0x15,	      //idVendor - 0x15A2(freescale Vendor ID)
	0x0F,0xA5,	      //idProduct
	0x00,0x02,	      //bcdDevice - Version 2.00
	0x01,                 //iManufacturer - Index to string Manufacturer descriptor
	0x02,	              //iProduct  - Index to string product descriptor 
	0x03,		      //iSerialNumber - Index to string serial number 
	0x01	              //bNumConfigurations - # of config. at current speed,
};


//字符串描述符0
const u8 String_Descriptor0[4] = 
{
	0x04,		   //bLength域，长度：4字节
	0x03,		   //bDescriptorType域，字符串描述符
	//0x0409为美式英语的ID
	0x09,0x04	   //wLANDID0域，英语
};

const u8 String_Descriptor1[22]={
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

const u8 String_Descriptor2[22]={
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
const u8 String_Descriptor3[] = 
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



/********************************************************************
*       Configuration Descriptor
********************************************************************/
const u8 Configuration_Descriptor[0x43]= 
{
	0x09,		    //blength
	0x02,		    //bDescriptor
	0x43,0x00,	    //wTotalLength - # of bytes including interface and endpoint descpt.
	0x02,		    //bNumInterfaces - at least 1 data interface
	0x01,		    //bConfigurationValue - 
	0x00,		    //iConfiguration - index to string descriptor	
	0xC0,		    //bmAttributes - 	            bit 7- Compatibility with USB 1.0
			    //bit 6 if 1 self powered else Bus powered
			    //bit 5-remote wakeup
		            //bit 4-0-reserved
	0x32,		    //bMaxPower - 100mA
/********************************************************************
*       Interface Descriptor
********************************************************************/
	0x09,	            //blength
	0x04,		    //bDescriptorType - Interface descriptor
	0x00,		    //bInterfaceNumber - Zero based value identifying the index of the config.
	0x00,		    //bAlternateSetting;
	0x01,		    //bNumEndpoints - 1 endpoints
	0x02,		    //bInterfaceClass - CDC
	0x02,		    //bInterfaceSubClass 
	0x01,		    //bInterfaceProtocol 
	0x01,		    //iInterface - Index to String descriptor
        /*Header Functional Descriptor*/
        0x05,               //bFunctionLength 
        0x24,               //bDescriptorType: CS_INTERFACE 
        0x00,               // bDescriptorSubtype: Call Management Func Desc 
        0x10,               // bmCapabilities: D0+D1 
        0x01,               // bDataInterface: 1 
        /*Call Managment Functional Descriptor*/
        0x05,
        0x24,
        0x01,
        0x00,
        0x01,
        /*ACM Functional Descriptor*/
        0x04,
        0x24,
        0x02,
        0x00,
        /*Union Functional Descriptor*/
        0x05,
        0x24,
        0x06,
        0x00,
        0x01, 
/********************************************************************
*       Endpoint  Descriptor
********************************************************************/
	0x07,           //blength
	0x05,           //bDescriptorType - EndPoint
	0x81,           //bEndpointAddress    
	0x03,           //bmAttributes
	0x40,0x00,      //wMaxPacketSize           
	0x02,           //bInterval
/********************************************************************
*       Interface Descriptor
********************************************************************/
	0x09,           //blength
	0x04,           //bDescriptorType - Interface descriptor
	0x01,           //bInterfaceNumber - Zero based value identifying the index of the config.
	0x00,           //bAlternateSetting;
	0x02,           //bNumEndpoints - 2 endpoints
	0x0A,           //bInterfaceClass - mass storage 
	0x00,           //bInterfaceSubClass - SCSI Transparent command Set
	0x00,           //bInterfaceProtocol - Bulk-Only transport
	0x01,           //iInterface - Index to String descriptor
/********************************************************************
*       Endpoint OUT Descriptor
********************************************************************/
	0x07,           //blength
	0x05,           //bDescriptorType - EndPoint
	0x82,           //bEndpointAddress    
	0x02,           //bmAttributes
	0x40,0x00,      //wMaxPacketSize           
	0x00,           //bInterval
/********************************************************************
*       Endpoint IN Descriptor
********************************************************************/
	0x07,           //blength
	0x05,           //bDescriptorType - EndPoint
	0x03,           //bEndpointAddress    
	0x02,           //bmAttributes
	0x40,0x00,      //wMaxPacketSize           
	0x00,           //bInterval	
};
#endif /* __USB_Desc__ */
