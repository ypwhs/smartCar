#include "chlib_k.h"
#include "arm_math.h"
#include "dsp_basic_demo.h"

 /**
 * @brief  演示arm_abs_f32功能
 * @code
 * @endcode
 * @param  None
 * @retval None
 * @author Rein
 * @update 2014-11-22
 */
void arm_abs_f32_demo(void) {
	/**	
	*	首先创建一个浮点数向量A(0.0,6.0)
	*	这里可以看做是一个一行两列的行列式，用来表示一个二维向量
	*	当然也可以写成src_vector[1][2] = {{0.0},{-6.0}}; 不过请注意在使用过程中注意数组指针的维数。
	*/
	float32_t src_vector[2] = {1.0f,-6.0f};	
	/**再创建一个浮点数数组用来存放运算后的结果,习惯性将局部变量申请后初始化为0*/
	float32_t dst_vector[2] = {0.0f,0.0f};
	/**串口打印初始化结果*/
	printf("arm_abs_f32_demo\n\r");
	printf("src_vector[0] = %f src_vector[1] = %f\n\r",src_vector[0],src_vector[1]);
	printf("dst_vector[0] = %f dst_vector[1] = %f\n\r",dst_vector[0],dst_vector[1]);
	/**调用数学库*/
	printf("start arm_abs...\n\r");
	/**sizeof(src_vector)/sizeof(float32_t) 表示先求出数组所占用字节数然后除以每个元素占用字节数 结果是数组元素长度*/
	arm_abs_f32(src_vector,dst_vector,sizeof(src_vector)/sizeof(float32_t));
	printf("arm_abs completed\n\r");
	/**打印输出结果进行比较*/
	printf("dst_vector[0] = %f dst_vector[1] = %f\n\r",dst_vector[0],dst_vector[1]);
}

 /**
 * @brief  演示arm_add_f32功能
 * @code
 * @endcode
 * @param  None
 * @retval None
 * @author Rein
 * @update 2014-11-22
 */
void arm_add_f32_demo(void) {
	/**	
	*	首先创建两个浮点数向量A(0.0,6.0) B(0.0,-13.5)
	*/
	float32_t A[2] = {0.0f,-6.0f};
	float32_t B[2] = {0.0f,-13.5f};	
	/**再创建一个浮点数数组用来存放运算后的结果,习惯性将局部变量申请后初始化为0*/
	float32_t result_vector[2] = {0.0f,0.0f};
	/**串口打印向量A,B的初始化结果*/
	printf("arm_add_f32_demo\n\r");
	printf("A = (%f,%f)  B = (%f,%f)\n\r",A[0],A[1],B[0],B[1]);
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
	/**调用数学库*/
	printf("start arm_add...\n\r");
	/**
		这里需要注意的是 两个向量相加，必须是同维度级别的，如果需要两个不同维度的向量相加，请将低维度向量的未定义维度补0
		比如 A(0,1,2) + B(3,3),就需要自己先将B向量补足为B(3,3,0),否则计算会有问题，请注意
	*/
	arm_add_f32(A,B,result_vector,sizeof(A)/sizeof(float32_t));
	printf("arm_add completed\n\r");
	/**打印输出结果进行比较*/
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
}

 /**
 * @brief  演示dot_prod_f32功能
 * @code
 * @endcode
 * @param  None
 * @retval None
 * @author Rein
 * @update 2014-11-22
 */
void arm_dot_prod_f32_demo(void) {
	/**	
	*	首先创建两个浮点数向量A(1.0,-6.0,10.2) B(2.0,-13.5,-2.5)
	*/
	float32_t A[3] = {1.0f,-6.0f,10.2f};
	float32_t B[3] = {2.0f,-13.5f,-2.5f};	
	/**再创建一个浮点数用来存放运算后的结果,因为向量的数量积结果是一个常数而非向量习惯性将局部变量申请后初始化为0*/
	float32_t result = 0.0f;
	/**串口打印向量A,B的初始化结果*/
	printf("arm_dot_prod_f32_demo\n\r");
	printf("A = (%f,%f,%f)  B = (%f,%f,%f)\n\r",A[0],A[1],A[2],B[0],B[1],B[2]);
	printf("result = %f\n\r",result);
	/**调用数学库*/
	printf("start arm_dot_prod...\n\r");
	arm_dot_prod_f32(A,B,sizeof(A)/sizeof(float32_t),&result);
	printf("arm_dot_prod completed\n\r");
	/**打印输出结果进行比较*/
	printf("result = %f\n\r",result);
}

 /**
 * @brief  演示arm_mult_f32功能
 * @code
 * @endcode
 * @param  None
 * @retval None
 * @author Rein
 * @update 2014-11-22
 */
void arm_mult_f32_demo(void) {
	/**	
	*	首先创建两个浮点数向量A(3.0,6.0) B(2.0,-13.5)
	*/
	float32_t A[2] = {3.0f,6.0f};
	float32_t B[2] = {2.0f,-13.5f};	
	/**再创建一个浮点数用来存放运算后的结果,因为向量的数量积结果是一个常数而非向量习惯性将局部变量申请后初始化为0*/
	float32_t result_vector[2] = {0,0};
	/**串口打印向量A,B的初始化结果*/
	printf("arm_mult_f32_demo\n\r");
	printf("A = (%f,%f)  B = (%f,%f)\n\r",A[0],A[1],B[0],B[1]);
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
	/**调用数学库*/
	printf("start arm_mult...\n\r");
	arm_mult_f32(A,B,result_vector,sizeof(A)/sizeof(float32_t));
	printf("arm_mult completed\n\r");
	/**打印输出结果进行比较*/
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
}

 /**
 * @brief  演示arm_negate_f32功能
 * @code
 * @endcode
 * @param  None
 * @retval None
 * @author Rein
 * @update 2014-11-22
 */
void arm_negate_f32_demo(void) {
	/**	
	*	首先创建一个浮点数向量A(-4.0,-3.0) 
	*/
	float32_t A[2] = {-4.0f,-3.0f};
	/**再创建一个浮点数用来存放运算后的结果,因为向量的数量积结果是一个常数而非向量习惯性将局部变量申请后初始化为0*/
	float32_t result_vector[2] = {0,0};
	/**串口打印向量A的初始化结果*/
	printf("arm_negate_f32_demo\n\r");
	printf("A = (%f,%f)\n\r",A[0],A[1]);
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
	/**调用数学库*/
	printf("start arm_negate_f32...\n\r");
	arm_negate_f32(A,result_vector,sizeof(A)/sizeof(float32_t));
	printf("arm_negate completed\n\r");
	/**打印输出结果进行比较*/
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
}

 /**
 * @brief  演示arm_offset_f32功能
 * @code
 * @endcode
 * @param  None
 * @retval None
 * @author Rein
 * @update 2014-11-22
 */
void arm_offset_f32_demo(void) {
	/**	
	*	首先创建一个浮点数向量A(-4.0,-3.0),再创建一个浮点数的偏移量Offset 
	*/
	float32_t A[2] = {-4.0f,-3.0f};
	float32_t Offset = 18.85;
	/**再创建一个浮点数用来存放运算后的结果,因为向量的数量积结果是一个常数而非向量习惯性将局部变量申请后初始化为0*/
	float32_t result_vector[2] = {0,0};
	/**串口打印向量A的初始化结果*/
	printf("arm_offset_f32_demo\n\r");
	printf("A = (%f,%f) Offset = %f\n\r",A[0],A[1],Offset);
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
	/**调用数学库*/
	printf("start arm_offset...\n\r");
	arm_offset_f32(A,Offset,result_vector,sizeof(A)/sizeof(float32_t));
	printf("arm_offset completed\n\r");
	/**打印输出结果进行比较*/
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
}


 /**
 * @brief  演示arm_scale_f32功能
 * @code
 * @endcode
 * @param  None
 * @retval None
 * @author Rein
 * @update 2014-11-22
 */
void arm_scale_f32_demo(void) {
	/**	
	*	首先创建一个浮点数向量A(-4.0,-3.0),再创建一个浮点数的扩围量Scale 
	*/
	float32_t A[2] = {-4.0f,-3.0f};
	float32_t Scale = 1.5f;
	/**再创建一个浮点数用来存放运算后的结果,因为向量的数量积结果是一个常数而非向量习惯性将局部变量申请后初始化为0*/
	float32_t result_vector[2] = {0,0};
	/**串口打印向量A的初始化结果*/
	printf("arm_scale_f32_demo\n\r");
	printf("A = (%f,%f) Scale = %f\n\r",A[0],A[1],Scale);
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
	/**调用数学库*/
	printf("start arm_scale...\n\r");
	arm_scale_f32(A,Scale,result_vector,sizeof(A)/sizeof(float32_t));
	printf("arm_scale completed\n\r");
	/**打印输出结果进行比较*/
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
}

 /**
 * @brief  演示arm_sub_f32功能
 * @code
 * @endcode
 * @param  None
 * @retval None
 * @author Rein
 * @update 2014-11-22
 */
void arm_sub_f32_demo(void) {
	/**	
	*	首先创建两个浮点数向量A(0.0,6.0) B(0.0,-13.5)
	*/
	float32_t A[2] = {0.0f,-6.0f};
	float32_t B[2] = {0.0f,-13.5f};	
	/**再创建一个浮点数数组用来存放运算后的结果,习惯性将局部变量申请后初始化为0*/
	float32_t result_vector[2] = {0.0f,0.0f};
	/**串口打印向量A,B的初始化结果*/
	printf("arm_sub_f32_demo\n\r");
	printf("A = (%f,%f)  B = (%f,%f)\n\r",A[0],A[1],B[0],B[1]);
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
	/**调用数学库*/
	printf("start arm_sub...\n\r");
	/**
		这里需要注意的是 两个向量相减，必须是同维度级别的，如果需要两个不同维度的向量相减，请将低维度向量的未定义维度补0
		比如 A(0,1,2) + B(3,3),就需要自己先将B向量补足为B(3,3,0),否则计算会有问题，请注意
	*/
	arm_sub_f32(A,B,result_vector,sizeof(A)/sizeof(float32_t));
	printf("arm_sub completed\n\r");
	/**打印输出结果进行比较*/
	printf("result_vector = (%f,%f)\n\r",result_vector[0],result_vector[1]);
}





